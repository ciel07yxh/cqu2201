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

    yxh_frame802154_t p2p_frame;
    yxh_frame802154_t rec_frame;
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
** Function name:       yxh_frame802154_create
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
  for(c = 0;c<flen.dest_addr_len;c++) {
    buf[pos++] = p->dest_addr[c];
  }

  /* Source PAN ID */
  if(flen.src_pid_len == 2) {
    buf[pos++] = p->src_pid & 0xff;
    buf[pos++] = (p->src_pid >> 8) & 0xff;
  }

  /* Source address */
  for(c = 0;c<flen.src_addr_len;c++) {
    buf[pos++] = p->src_addr[c];
  }
  
  for(c = 0;c<p2p_frame.payload_len;c++) {
    buf[pos++] = p->payload[c];
  }
  return (int)pos;
}

/*********************************************************************************************************
** Function name:       frame_para_init
** Descriptions:        Init the parameters of the frame
** input parameters:    none
** output parameters:   none
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/
void frame_para_init(void)
{
    //init the fcf
    p2p_frame.fcf.frame_type=0x01;
    p2p_frame.fcf.security_enabled=0x00;
    p2p_frame.fcf.frame_pending=0x00;
    p2p_frame.fcf.ack_required=0x01;
    //p2p_frame.panid_compression=;                  //this is to be determined in field_flen()
    p2p_frame.fcf.dest_addr_mode=FRAME802154_SHORTADDRMODE;                      //short address
    p2p_frame.fcf.frame_version=0x01;
    p2p_frame.fcf.src_addr_mode=FRAME802154_SHORTADDRMODE;                        //short address
    //init the seq
    p2p_frame.seq=0x00;
    //init the PAN id
    p2p_frame.dest_pid=get_cluster_name(DEST_ADDR);

    p2p_frame.src_pid=get_cluster_name(p2p_frame.src_addr[1]);
    //init the Adress
    p2p_frame.dest_addr[0]=DEST_ADDR;                             //chose No.1 node to receive
    p2p_frame.src_addr[0]=SRC_ADDR;  
    p2p_frame.payload=payload_to_send;
    p2p_frame.payload_len=sizeof(payload_to_send);
}

/*********************************************************************************************************
** Function name:       p2p_frame_send
** Descriptions:        send the frame
** input parameters:    null
** output parameters:   none
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/
void p2p_frame_send(void *ptr)
{
    static struct ctimer ct;

    uint8_t p2p_buf[100];
    //PRINTF("aaa");
   //initialize the p2p frame parameters
    frame_para_init();
    //yxh_frame802154_t p2p_frame=p2p_frame;
    //Initialize the p2p frame, put the structure value into a buffer
      int sendlength = yxh_frame802154_create(&p2p_frame, p2p_buf);
      //Send p2p frame
     int state = NETSTACK_RADIO.send(p2p_buf, sendlength);  
     //print send frame and result(state)
     PRINTF("The send frame is:");
     for(uint8_t a=0;a<sendlength;a++)
     {
        PRINTF("%x ",p2p_buf[a]);
     }
     PRINTF("\r\n");   
     //PRINTF("The frame is send from node %d\r\n",);
     PRINTF("The send state is %d\r\n",state);                  //  2a¨º? state = enSendState = RADIO_TX_OK=0; 
     ctimer_set(&ct,CLOCK_SECOND/10,p2p_frame_send,NULL);
}

/*********************************************************************************************************
** Function name:       yxh_frame_parse
** Descriptions:        Parses an input frame
** input parameters:    uint8_t *data, int len, yxh_frame802154_t *pf
** output parameters:   
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-18
*********************************************************************************************************/

/*----------------------------------------------------------------------------*/
/**
 *   \brief Parses an input frame.  Scans the input frame to find each
 *   section, and stores the information of each section in a
 *   yxh_frame_t structure.
 *
 *   \param data The input data from the radio chip.
 *   \param len The size of the input data
 *   \param pf The yxh_frame_t struct to store the parsed frame information.
 */
void yxh_frame802154_parse(void)
{
  
  uint8_t *p;                   
  yxh_frame802154_fcf_t fcf;
  int c;
  
  uint8_t *data = packetbuf_dataptr();
  int len = (int)packetbuf_datalen();
  yxh_frame802154_t *pf = &rec_frame;
    
  if(len < 3) {
    return ;
  }
   for(uint8_t a=0;a<len;a++)
     {
        PRINTF("%x ",data[a]);
     }
    PRINTF("\r\n");   
  
  p = data;

  /* decode the FCF */
  fcf.frame_type = p[0] & 7;
  fcf.security_enabled = (p[0] >> 3) & 1;
  fcf.frame_pending = (p[0] >> 4) & 1;
  fcf.ack_required = (p[0] >> 5) & 1;
  fcf.panid_compression = (p[0] >> 6) & 1;

  fcf.dest_addr_mode = (p[1] >> 2) & 3;
  fcf.frame_version = (p[1] >> 4) & 3;
  fcf.src_addr_mode = (p[1] >> 6) & 3;

  /* copy fcf and seqNum */
  memcpy(&pf->fcf, &fcf, sizeof(frame802154_fcf_t));
  pf->seq = p[2];
  p += 3;                             /* Skip first three bytes */

  /* Destination address, if any */
  if(fcf.dest_addr_mode) {
    /* Destination PAN */
    pf->dest_pid = p[0] + (p[1] << 8);
    p += 2;
    
    /* Destination address */
/*     l = addr_len(fcf.dest_addr_mode); */
/*     for(c = 0; c < l; c++) { */
/*       pf->dest_addr.u8[c] = p[l - c - 1]; */
/*     } */
/*     p += l; */
    if(fcf.dest_addr_mode == FRAME802154_SHORTADDRMODE) {
      linkaddr_copy((linkaddr_t *)&(pf->dest_addr), &linkaddr_null);
      pf->dest_addr[0] = p[0];
      pf->dest_addr[1] = p[1];
      p += 2;
    } else if(fcf.dest_addr_mode == FRAME802154_LONGADDRMODE) {
      for(c = 0; c < 8; c++) {
        pf->dest_addr[c] = p[c];
      }
      p += 8;
    }
  } else {
    linkaddr_copy((linkaddr_t *)&(pf->dest_addr), &linkaddr_null);
    pf->dest_pid = 0;
  }
  if(pf->dest_addr[0] != get_moteid() || pf->dest_pid != get_cluster_name(DEST_ADDR) )
  {
    PRINTF("Wrong!\r\n");
    return;
  }
  
  /* Source address, if any */
  if(fcf.src_addr_mode) {
    /* Source PAN */
    if(!fcf.panid_compression) {
      pf->src_pid = p[0] + (p[1] << 8);
      p += 2;
    } else {
      pf->src_pid = pf->dest_pid;
    }

    /* Source address */
/*     l = addr_len(fcf.src_addr_mode); */
/*     for(c = 0; c < l; c++) { */
/*       pf->src_addr.u8[c] = p[l - c - 1]; */
/*     } */
/*     p += l; */
    if(fcf.src_addr_mode == FRAME802154_SHORTADDRMODE) {
      linkaddr_copy((linkaddr_t *)&(pf->src_addr), &linkaddr_null);
      pf->src_addr[0] = p[0];
      pf->src_addr[1] = p[1];
      p += 2;
    } else if(fcf.src_addr_mode == FRAME802154_LONGADDRMODE) {
      for(c = 0; c < 8; c++) {
        pf->src_addr[c] = p[c];
      }
      p += 8;
    }
  } else {
    linkaddr_copy((linkaddr_t *)&(pf->src_addr), &linkaddr_null);
    pf->src_pid = 0;
  }

  /* header length */
  c = p - data;            //  ?¦Ì?a?a????¨ºy    
  /* payload length */
  pf->payload_len = (len - c);
  /* payload */
  pf->payload = p;
  PRINTF("Received!\r\n");
}






/*
void p2p_frame_recieve()
{
}
*/













/*********************************************************************************************************
  END FILE
*********************************************************************************************************/