/****************************************Copyright (c)****************************************************
**                               
**                                                       
**  
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               P2P.h
** Last modified Date:      2012-06-21
** Last Version:            v1.0
** Description:             
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Xiaohan Y
** Created date:            2018-04-10
** Version:                 v1.0
** Descriptions:            The original version 
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
#ifndef __P2P_H__ 
#define __P2P_H__ 

#define FRAME802154_SHORTADDRMODE   (0x02)           
#define FRAME802154_LONGADDRMODE    (0x03)

#define DEST_ADDR    (0x18)                //choose the destination address  no.02
#define SRC_ADDR    (0x15)                //choose the destination address   no.01
#define TIME_SYNCH_NODE               0x19
#define INTERFERENCE_NODE               0x1a

#define TIME_SYNCH_TIMES               3

#define FRAME_TYPE_TIME_SYNCH   0x01
#define FRAME_TYPE_P2P          0x02
#define FRAME_TYPE_INTERF       0x03

#define YXH_RECV    1
#define INTERFERENCE    1

#define BSM_FRE_HZ      10                              //BSM 10Hz
#define PEROID_LENGTH   (RTIMER_SECOND/BSM_FRE_HZ)      //BSM 周期时间     
/*********************************************************************************************************
**  Define global variaty
*********************************************************************************************************/


/**
 *    @brief  The IEEE 802.15.4 frame has a number of constant/fixed fields that
 *            can be counted to make frame construction and max payload
 *            calculations easier.
 *
 *            These include:
 *            1. FCF                  - 2 bytes       - Fixed
 *            2. Sequence number      - 1 byte        - Fixed
 *            3. Addressing fields    - 4 - 20 bytes  - Variable
 *            4. Aux security header  - 0 - 14 bytes  - Variable
 *            5. CRC                  - 2 bytes       - Fixed
*/

typedef struct {
  uint8_t frame_type;        /**< 3 bit. Frame type field, see 802.15.4 */
  uint8_t security_enabled;  /**< 1 bit. True if security is used in this frame */
  uint8_t frame_pending;     /**< 1 bit. True if sender has more data to send */
  uint8_t ack_required;      /**< 1 bit. Is an ack frame required? */
  uint8_t panid_compression; /**< 1 bit. Is this a compressed header? */
  /*   uint8_t reserved; */  /**< 3 bit. Unused bits */
  uint8_t dest_addr_mode;    /**< 2 bit. Destination address mode, see 802.15.4 */
  uint8_t frame_version;     /**< 2 bit. 802.15.4 frame version */
  uint8_t src_addr_mode;     /**< 2 bit. Source address mode, see 802.15.4 */
}yxh_frame802154_fcf_t;

typedef struct {
  yxh_frame802154_fcf_t fcf;          /**< Frame control field  */
  uint8_t seq;                    /**< Sequence number */
  uint16_t dest_pid;              /**< Destination PAN ID */
  uint8_t dest_addr[8];           /**< Destination address */
  uint16_t src_pid;               /**< Source PAN ID */
  uint8_t src_addr[8];            /**< Source address */
  //frame802154_aux_hdr_t aux_hdr;  /**< Aux security header */             //  No Aux security header
  uint8_t *payload;               /**< Pointer to 802.15.4 payload */
  uint8_t frame_seq;
  uint32_t time_stamp;
  uint8_t send_type;
  int payload_len;                /**< Length of payload field */         //  ??
}yxh_frame802154_t;

typedef struct time_para{
        uint8_t  IsSyched;
        //uint32_t time_stamp;
        int32_t  time_offset;
        int32_t  time_offset_period_align;         //用于时间同步周期对齐
        void (*timeoffset)(struct time_para *timepara,uint32_t time);
        rtimer_clock_t (*get_synch_time)(struct time_para *timepara);
} time_para;

/**
 *  \brief Structure that contains the lengths of the various addressing and security fields
 *  in the 802.15.4 header.  This structure is used in \ref frame802154_create()
 */
typedef struct {
  uint8_t dest_pid_len;    /**<  Length (in bytes) of destination PAN ID field */
  uint8_t dest_addr_len;   /**<  Length (in bytes) of destination address field */
  uint8_t src_pid_len;     /**<  Length (in bytes) of source PAN ID field */
  uint8_t src_addr_len;    /**<  Length (in bytes) of source address field */
  //uint8_t aux_sec_len;     /**<  Length (in bytes) of aux security header field */
} field_length_t;

void frame_para_init(yxh_frame802154_t *p,void *ftype);//init the parameters used in creating the frame and the operation machianism
//void peration_init();        //init the parameters used in the operation machianism
int yxh_frame802154_create(yxh_frame802154_t *p, uint8_t *buf);
void yxh_frame_send(void *type);
void interferencing(void); 

void yxh_frame802154_parse(void);
rtimer_clock_t get_synch_time(time_para *timepara);
void timeoffset_calc(time_para *timepara,uint32_t time);
//int frame802154_hdrlen(frame802154_t *p);
//void frame802154_recieve();
void time_synch_gps(void *ptr);


#endif /* __CONTIKI_CONF_H__ */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/










