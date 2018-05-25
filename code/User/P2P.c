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
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif



yxh_frame802154_t rec_frame;

uint8_t payload_to_send[2]={1,2};
uint16_t frame_sequence = 0;
uint16_t recframe_count = 1;
uint16_t sentframe_count = 1;
uint16_t bufframe_count = 1;

//uint8_t synch_node_flag  = 0x01;
uint8_t sendflag = 0;

time_para synch={
  0,
  0,
  0,
  0,
  timeoffset_calc,
  get_synch_time
};

#define MAX_QUEUED_PACKETS 60
MEMB(frame_size, yxh_frame802154_t, MAX_QUEUED_PACKETS);
MEMB(packet_size, struct packet_list, MAX_QUEUED_PACKETS);
LIST(packet_list);
/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        ��ȡͬ��ʱ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

rtimer_clock_t get_synch_time(time_para *timepara)
{
  return  RTIMER_NOW() - timepara->time_offset;
} 

/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        ����ʱ��ƫ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/



void timeoffset_calc(time_para *timepara,uint32_t time)
{
  timepara->time_offset = timepara->time_stamp -time;
  //������ȡ��
  timepara->time_offset_period_align=timepara->time_offset%PEROID_LENGTH;
  //����Ϊ����
  if(timepara->time_offset_period_align<0)
      timepara->time_offset_period_align+=PEROID_LENGTH;
} 




/*********************************************************************************************************
** Function name:       addr_len
** Descriptions:        determine the length of address fields
** input parameters:    uint8_t mode
** output parameters:   static uint8_t 
** Returned value:      0
** Created by:          Xiaohan Y
** Created Date:        2018-04-11
*********************************************************************************************************/
CC_INLINE static uint8_t                                  
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
  
  buf[pos++] = p->frame_seq;
  
    buf[pos++] = p->time_stamp & 0xff;
    buf[pos++] = (p->time_stamp >> 8) & 0xff;
    buf[pos++] = (p->time_stamp >> 16) & 0xff;
    buf[pos++] = (p->time_stamp >> 24) & 0xff;
 
  buf[pos++] = p->send_type;
  
  for(c = 0;c<p->payload_len;c++) {
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
time_para *timepara = & synch; 
void frame_para_init(yxh_frame802154_t *p,void *ftype)
{
    //init the fcf
    p->fcf.frame_type=0x01;
    p->fcf.security_enabled=0x00;
    p->fcf.frame_pending=0x00;
    p->fcf.ack_required=0x01;
    //p2p_frame.panid_compression=;                                      //this is to be determined in field_flen()
    p->fcf.dest_addr_mode=FRAME802154_SHORTADDRMODE;                      //short address
    p->fcf.frame_version=0x01;
    p->fcf.src_addr_mode=FRAME802154_SHORTADDRMODE;                        //short address
    //init the seq
    p->seq=0x00;
    //init the PAN id

    p->src_pid=get_cluster_name(get_moteid());
    //init the Adress

    p->src_addr[0]=get_moteid();
    p->src_addr[1]=0x00;
    
    
    p->payload=payload_to_send;
    p->payload_len=sizeof(payload_to_send);

     if(*((uint8_t*)ftype) == FRAME_TYPE_P2P )
     {
       p->dest_pid=get_cluster_name(get_moteid()-15);
       p->dest_addr[0]=get_moteid()-15;  
       p->dest_addr[1]= 0x00;
     } else if(*((uint8_t*)ftype) ==FRAME_TYPE_TIME_SYNCH ) 
     {
       p->dest_pid = 0xFFFF;
       p->dest_addr[0] = 0xFF;
       p->dest_addr[1] = 0xFF;
       p->fcf.ack_required=0x00;
     } else if (*((uint8_t*)ftype) ==FRAME_TYPE_INTERF)
     {
       p->dest_pid=get_cluster_name(get_moteid()+1);
       p->dest_addr[0]=0x01;  
       p->dest_addr[1]= 0x00;
     }
     p->frame_seq = frame_sequence++;
     p->time_stamp = timepara->get_synch_time(timepara);
     p->send_type = *((uint8_t*)ftype);    
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
struct ctimer ctSendP;
void yxh_frame_send(void * type)
{
    yxh_frame802154_t frame_to_send;
    yxh_frame802154_t *yxh_frame = &frame_to_send;    //frame_to_send��Ϊ�˸�ָ��yxh_frame�����ַ
    uint8_t frame_buf[100];
    
   //initialize the p2p frame parameters
    frame_para_init(yxh_frame,type);
    
    //ֻ��200֡����Ȼframe_seqװ����
    if(yxh_frame->frame_seq >200 )
      return;
    
#if BUFFER        //ʹ��֡����
    //ʱ��ͬ��ֱ֡�ӷ���
    if((*((uint8_t*)type))==FRAME_TYPE_TIME_SYNCH){
      int sendlength = yxh_frame802154_create(yxh_frame, frame_buf);
      int state = NETSTACK_RADIO.send(frame_buf, sendlength); 
      
      //��ӡ��������
      PRINTF("state=%d\r\n",state);
      for(uint8_t a=0;a<sendlength;a++)
      {
        PRINTF("%x ",frame_buf[a]);
      }
      PRINTF("\r\n\r\n"); 
      
    }else{
      //p2p֡�Ż���
      qx_sendBuf(yxh_frame);
      
    }   
#else          //��ʹ��֡����    
    yxh_frame->time_stamp = timepara->get_synch_time(timepara);
    int sendlength = yxh_frame802154_create(yxh_frame, frame_buf);
    int state = NETSTACK_RADIO.send(frame_buf, sendlength);
    uart_printf("No.%d frame is sent to the air! \r\n",bufframe_count++);
    
    if(state == 0)
    {
      uart_printf("No.%d frame is sent successfully! \r\n",sentframe_count++);
    }
    //��������
    PRINTF("state=%d\r\n",state);
    for(uint8_t a=0;a<sendlength;a++)
    {
      PRINTF("%x ",frame_buf[a]);
    }
    PRINTF("\r\n\r\n"); 
    
#endif   
    //�����Լ�
    if((*((uint8_t*)type))!=FRAME_TYPE_TIME_SYNCH){
      ctimer_set(&ctSendP,CLOCK_SECOND/10,yxh_frame_send, type); 
    }  
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
   
  
  if(len <= 3) {
    return ;
  }
 
  
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
  
  /*
  if(fcf.frame_type == 0x2 && len == 3)
  {
    PRINTF("ACK  received!\r\n");
    return;
  }
  */
  
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

    
    pf->frame_seq = p[0];
    p++;
    
    pf->time_stamp = p[0] | p[1]<<8 | p[2]<<16 | p[3]<<24 ;
    p += 4;
    
    pf->send_type = p[0];
    
    /* header length */
    c = p - data;          
    /* payload length */
    pf->payload_len = (len - c);
    /* payload */
    pf->payload = p;
    
//�жϽ��սڵ�����������֡
    if(get_moteid() != TIME_SYNCH_NODE )
    {
      deal_recframe(pf);
    
     }else{
      //ͬ���ڵ�ֻ����ͬ��֡�����յ�����ʲô������    
      return;  
    } 
  
    //��ӡ���յ���֡
    PRINTF("The received frame is: ");
    for(uint8_t a=0;a<len;a++)
    {
      PRINTF("%x ",data[a]);
    }
    PRINTF("\r\n");  

  }
  

/*********************************************************************************************************
** Function name:       deal_recframe
** Descriptions:        �Խ��յ�֡������
** input parameters:    
** output parameters:   ��
** Returned value:      0
** Created by:          ԬС��
** Created Date:        2018-05-24
*********************************************************************************************************/
void deal_recframe(yxh_frame802154_t * rec)
{
  static struct ctimer ct1;
  time_para * timesynch = &synch;
  uint8_t moteId = get_moteid();
  //��ͬ���ڵ���յ�������ݲ�ͬ֡���ͣ�send_frame��������ͬ����
  switch(rec->send_type){
  case FRAME_TYPE_TIME_SYNCH:
//    //ֻͬ��һ��
//    if(timesynch->IsSyched == true)
//    {
//      break;
//    }
    timesynch->IsSyched = true;
    //��������սڵ��time_stamp ��time_offset_period_alignֵ
    timesynch->timeoffset(timesynch,rec->time_stamp);                  
    //��ӡʱ��ʱ��ƫ��ֵ��ͬ�����ʱ��
    PRINTF("time-offset is %d \r\nthe synchronized time is  %d \r\n",timesynch->time_offset,timesynch->get_synch_time(timesynch));    
    //Դ�ڵ����ʱ��ͬ���󣬾���5���ʱ�俪ʼ��P2P֡(undone)
    if( moteId >= SRC_ADDR && moteId <= SRC_ADDR+4 )
    {
      static uint8_t type1 = FRAME_TYPE_P2P;
      ctimer_set(&ct1,5*CLOCK_SECOND,yxh_frame_send, (void*)(&type1));
    }
    break;
    
  case FRAME_TYPE_P2P:
    {        
      PRINTF("The time of stamp is: %u\r\n", rec->time_stamp);
      PRINTF("The Rtime of receiving moument is: %u\r\n",synch.time_stamp);
      PRINTF("The time of receiving moument is: %u\r\n",(synch.time_stamp - synch.time_offset));
      //ʱ��
      uint32_t delay = (synch.time_stamp - synch.time_offset)- rec->time_stamp;
      PRINTF("The delay of p2p is: %u\r\n", delay);
      uart_printf("%d ", delay);
      PRINTF("No.%d p2pFrame receiceved by destination node!\r\n",recframe_count++);      
      break;
    }
  }   //switch end
  
}


/*********************************************************************************************************
** Function name:       time_synch_gps
** Descriptions:        ����GPSʱ��ͬ���ź�ģ��
** input parameters:    ͬ��֡���ʹ���
** output parameters:   ��
** Returned value:      0
** Created by:          ԬС��
** Created Date:        2018-05-07
*********************************************************************************************************/
void time_synch_gps(void *ptr)
{
    static struct ctimer ct1;
 
    uint8_t time_to_synch;
    if(ptr!=NULL)
    {
      PRINTF("time_synch_gps %d\r\n",(*(uint8_t *)ptr));
      time_to_synch=(*(uint8_t *)ptr)--;
      if(!time_to_synch){
        PRINTF("time synch is over");
          return;
      }
    }

    //NETSTACK_RADIO.set_value(SET_CHANNEL,17);
   // yxh_frame802154_t synch_frame;
    //frame_para_init(&synch_frame,FRAME_TYPE_TIME_SYNCH);
    //frame_to_send = synch_frame;
    static uint8_t type = FRAME_TYPE_TIME_SYNCH;
    yxh_frame_send((void *)&type);
    ctimer_set(&ct1,CLOCK_SECOND/10,time_synch_gps, ptr); 
    
}

/*********************************************************************************************************
** FFunction name:       interferencing
** Descriptions:        ���Žڵ㷢�͸����ź�
** input parameters:    ��
** output parameters:   ��
** Returned value:      0
** Created by:          ԬС��
** Created Date:        2018-05-11
*********************************************************************************************************/
void interferencing(void)
{
    static struct ctimer  ct1; 
    static uint8_t type = FRAME_TYPE_INTERF;
    ctimer_set(&ct1,5*CLOCK_SECOND,yxh_frame_send, (void *)&type); 
}




/*********************************************************************************************************
** Function name:       
** Descriptions:        
** input parameters:    ��
** output parameters:   ��
** Returned value:      0
** Created by:          
** Created Date:        2018-05-11
*********************************************************************************************************/
uint8_t startSend=0;
struct ctimer ctSendAir;

//���뻺��
void qx_sendBuf(yxh_frame802154_t * frame){
  
  if(list_length(packet_list)>50){//�ռ�����
    return;
  }
  //����ظ�ֵ
  struct packet_list* q;
  q = memb_alloc(&packet_size);
  q->data=memb_alloc(&frame_size);  
  //fcf��ֵ
  q->data->fcf.frame_type=frame->fcf.frame_type;        /**< 3 bit. Frame type field, see 802.15.4 */
  q->data->fcf.security_enabled=frame->fcf.security_enabled;  /**< 1 bit. True if security is used in this frame */
  q->data->fcf.frame_pending=frame->fcf.frame_pending;     /**< 1 bit. True if sender has more data to send */
  q->data->fcf.ack_required=frame->fcf.ack_required;      /**< 1 bit. Is an ack frame required? */
  q->data->fcf.panid_compression=frame->fcf.panid_compression; /**< 1 bit. Is this a compressed header? */
  q->data->fcf.dest_addr_mode=frame->fcf.dest_addr_mode;    /**< 2 bit. Destination address mode, see 802.15.4 */
  q->data->fcf.frame_version=frame->fcf.frame_version;     /**< 2 bit. 802.15.4 frame version */
  q->data->fcf.src_addr_mode=frame->fcf.src_addr_mode;     /**< 2 bit. Source address mode, see 802.15.4 */
  //֡��ֵ
  q->data->seq=frame->seq;                    /**< Sequence number */
  q->data->dest_pid=frame->dest_pid;              /**< Destination PAN ID */
  for(int i=0;i<8;i++){
    q->data->dest_addr[i]=frame->dest_addr[i];           /**< Destination address */
    q->data->src_addr[i]=frame->src_addr[i];            /**< Source address */  
  }  
  q->data->src_pid=frame->src_pid;               /**< Source PAN ID */
  q->data->payload=frame->payload;               /**< Pointer to 802.15.4 payload */
  q->data->frame_seq=frame->frame_seq;
  q->data->time_stamp=frame->time_stamp;
  q->data->send_type=frame->send_type;
  q->data->payload_len=frame->payload_len;                /**< Length of payload field */         //  ??

  //��ӽ������ ���51
  list_add(packet_list, q);
  uart_printf("list���ÿռ�Ϊ%d\r\n",list_length(packet_list));
  uart_printf("No.%d frame is successfully put in the buffer! \r\n",bufframe_count++);
  
  //��ʼɨ�軺���
  if(startSend==0){
    startSend=1;
     //ctimer_set(&ctSendAir,CLOCK_SECOND/10+CLOCK_SECOND/100*get_moteid(),sendAir, NULL); //tdma
    ctimer_set(&ctSendAir,CLOCK_SECOND/10,sendAir, NULL); //csma
  }
}


/*********************************************************************************************************
** Function name:       
** Descriptions:        
** input parameters:    ��
** output parameters:   ��
** Returned value:      0
** Created by:          
** Created Date:        2018-05-11
*********************************************************************************************************/
struct ctimer ctSendAir;

void sendAir(void *p){

  //��黺���
  if(list_length(packet_list)!= 0){//����ز�Ϊ��
    struct packet_list *q = list_head(packet_list);
    if(q!=NULL){
      //�޸�ʱ�����Ȼ�� װ֡�ҷ���
      q->data->time_stamp = timepara->get_synch_time(timepara);
      uint8_t buf[100];
      int sendlength = yxh_frame802154_create(q->data,buf);    
      int state = NETSTACK_RADIO.send(buf,sendlength); 
      
      //��������
      PRINTF("state=%d\r\n",state);
      for(uint8_t a=0;a<sendlength;a++)
      {
        PRINTF("%x ",buf[a]);
      }
      PRINTF("\r\n\r\n"); 
 
      //n�η���δ���յ�ACK������һ֡
      if(state == 3)
        sendflag += 1;      
      if(sendflag == 3)
      {
        list_remove(packet_list, q);
        memb_free(&frame_size,q->data);
        memb_free(&packet_size, q);
        uart_printf("Send failed, next to send!\r\n");
      }

      //�������
      if(state == 0){    
        list_remove(packet_list, q);
        memb_free(&frame_size,q->data);
        memb_free(&packet_size, q);
        uart_printf("list���ÿռ�Ϊ%d\r\n",list_length(packet_list));
        uart_printf("No.%d frame is successfully sent! \r\n",sentframe_count++);
        sendflag = 0;
      }
      
      
    }  
  }
  
  //ѭ��ɨ�軺���
  ctimer_set(&ctSendAir,CLOCK_SECOND/10,sendAir, NULL); 
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/