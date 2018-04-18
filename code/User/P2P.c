/****************************************Copyright (c)****************************************************
**                                
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           P2P.c
** Last modified Date:  2018-04-11
** Last Version:        v1.0
** Description:         
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Xaiohan Y
** Created date:        2012-06-21
** Version:             v1.0
** Descriptions:        The original version 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "includes.h"
#include "P2P.h"

/*********************************************************************************************************
**  Define global variaty
*********************************************************************************************************/
#define DEBUG 1
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

    yxh_frame802154_t initframe;
    uint8_t payload_to_send[2]={1,2};
/*********************************************************************************************************
** Function name:       addr_len
** Descriptions:        determine the length of address fields
** input parameters:    uint8_t mode
** output parameters:   static uint8_t 
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/
CC_INLINE static uint8_t                                  //??
addr_len(uint8_t mode)
{
  switch(mode) {
  case FRAME802154_SHORTADDRMODE:  /* 16-bit address */
    return 2;
  case FRAME802154_LONGADDRMODE:   /* 64-bit address */
    return 8;
  default:
    return 0;
  }
}

/*********************************************************************************************************
** Function name:       field_len
** Descriptions:        determine the length of variable fields  
** input parameters:    frame802154_t *p, field_length_t *flen
** output parameters:   none
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/
static void
field_len(yxh_frame802154_t *p, field_length_t *flen)
{
  /* init flen to zeros */
  memset(flen, 0, sizeof(field_length_t));

  /* Determine lengths of each field based on fcf and other args */
  if(p->fcf.dest_addr_mode & 3) {
    flen->dest_pid_len = 2;
  }
  if(p->fcf.src_addr_mode & 3) {
    flen->src_pid_len = 2;
  }

  /* Set PAN ID compression bit if src pan id matches dest pan id. */
  if(p->fcf.dest_addr_mode & 3 && p->fcf.src_addr_mode & 3 &&
     p->src_pid == p->dest_pid) {
    p->fcf.panid_compression = 1;

    /* compressed header, only do dest pid */
    flen->src_pid_len = 0;
  } else {
    p->fcf.panid_compression = 0;
  }

  /* determine address lengths */
  flen->dest_addr_len = addr_len(p->fcf.dest_addr_mode & 3);
  flen->src_addr_len = addr_len(p->fcf.src_addr_mode & 3);
}


/*********************************************************************************************************
** Function name:       frame802154_create
** Descriptions:        Creates a frame for transmission over the air
** input parameters:    frame802154_t *p, uint8_t *buf
** output parameters:   pose: the total length (Byte) of the frame except payload CRC
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/

/*----------------------------------------------------------------------------*/
/**
 *   \brief Creates a frame for transmission over the air.  This function is
 *   meant to be called by a higher level function, that interfaces to a MAC.
 *
 *   \param p Pointer to frame802154_t struct, which specifies the
 *   frame to send.
 *
 *   \param buf Pointer to the buffer to use for the frame.
 *
 *   \return The length of the frame header
*/
int
yxh_frame802154_create(yxh_frame802154_t *p, uint8_t *buf)       
{
  int c;
  field_length_t flen;
  uint8_t pos;

  field_len(p, &flen);
  
  /* OK, now we have field lengths.  Time to actually construct */
  /* the outgoing frame, and store it in buf */
  buf[0] = (p->fcf.frame_type & 7) |
    ((p->fcf.security_enabled & 1) << 3) |
    ((p->fcf.frame_pending & 1) << 4) |
    ((p->fcf.ack_required & 1) << 5) |
    ((p->fcf.panid_compression & 1) << 6);
  buf[1] = ((p->fcf.dest_addr_mode & 3) << 2) |
    ((p->fcf.frame_version & 3) << 4) |
    ((p->fcf.src_addr_mode & 3) << 6);

  /* sequence number */
  buf[2] = p->seq;
  pos = 3;                               //Use pose because after the 3rd Bytes, the length of the address field is variable.

  /* Destination PAN ID */
  if(flen.dest_pid_len == 2) {
    buf[pos++] = p->dest_pid & 0xff;
    buf[pos++] = (p->dest_pid >> 8) & 0xff;
  }

  /* Destination address */
  for(c = flen.dest_addr_len; c > 0; c--) {
    buf[pos++] = p->dest_addr[c - 1];
  }

  /* Source PAN ID */
  if(flen.src_pid_len == 2) {
    buf[pos++] = p->src_pid & 0xff;
    buf[pos++] = (p->src_pid >> 8) & 0xff;
  }

  /* Source address */
  for(c = flen.src_addr_len; c > 0; c--) {
    buf[pos++] = p->src_addr[c - 1];
  }
  for(c =initframe.payload_len; c > 0; c--) {
    buf[pos++] = p->src_addr[c - 1];
  }
  return (int)pos;
}

/*********************************************************************************************************
** Function name:       frame802154_parse
** Descriptions:        Parses an input frame
** input parameters:    uint8_t *data, int len, frame802154_t *pf
** output parameters:   
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/

/*----------------------------------------------------------------------------*/
/**
 *   \brief Parses an input frame.  Scans the input frame to find each
 *   section, and stores the information of each section in a
 *   frame802154_t structure.
 *
 *   \param data The input data from the radio chip.
 *   \param len The size of the input data
 *   \param pf The frame802154_t struct to store the parsed frame information.
 */


void frame_para_init(void)
{
    //init the fcf
    initframe.fcf.frame_type=0x01;
    initframe.fcf.security_enabled=0x00;
    initframe.fcf.frame_pending=0x01;
    initframe.fcf.ack_required=0x00;
    //initframe.panid_compression=;                  //this is to be determined in field_flen()
    initframe.fcf.dest_addr_mode=FRAME802154_SHORTADDRMODE;                      //short address
    initframe.fcf.frame_version=0x01;
    initframe.fcf.src_addr_mode=FRAME802154_SHORTADDRMODE;                        //short address
    //init the seq
    initframe.seq=0x00;
    //init the PAN id
    initframe.dest_pid=get_cluster_name(DESR_ADDR);

    initframe.src_pid=get_cluster_name(initframe.src_addr[1]);
    //init the Adress
    initframe.dest_addr[0]=DESR_ADDR;                             //chose No.1 node to receive
    initframe.src_addr[0]=get_moteid();  
    initframe.payload=payload_to_send;
    initframe.payload_len=sizeof(payload_to_send);
}



void p2p_frame_send(void *ptr)
{
    static struct ctimer ct;
    yxh_frame802154_t* p2p_frame=(yxh_frame802154_t*)&initframe;
    uint8_t p2p_buf[100];
    //PRINTF("aaa");
   //initialize the p2p frame parameters
    frame_para_init();

    //Initialize the p2p frame, put the structure value into a buffer
      int sendlength = yxh_frame802154_create(p2p_frame, p2p_buf);
      //Send p2p frame
     int state = NETSTACK_RADIO.send(&p2p_frame, sendlength);  
     //print send frame and result(state)
     PRINTF("The send frame is:");
     for(uint8_t a=0;a<sizeof(p2p_buf);a++)
     {
        PRINTF("%d",p2p_buf[a]);
     }
     PRINTF("\r\n");
     //PRINTF("The frame is send from node %d\r\n",);
     //PRINTF("The send frame is %d\r\n",*p2p_buf);
     //PRINTF("The send astate is %d\r\n",state);
     ctimer_set(&ct,CLOCK_SECOND/10,p2p_frame_send,NULL);
}


/*
void p2p_frame_recieve()
{
}
*/













/*********************************************************************************************************
  END FILE
*********************************************************************************************************/