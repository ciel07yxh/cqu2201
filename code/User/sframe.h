/****************************************Copyright (c)****************************************************
**                               
**                                                       
**  
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               sframe.h
** Last modified Date:      2018-04-17
** Last Version:            v1.0
** Description:             
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Xiaohan Y
** Created date:            2018-04-17
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
#ifndef __SFRMAE_H__ 
#define __SFRMAE_H__ 


/*********************************************************************************************************
**  Define global variaty
*********************************************************************************************************/


/**
 *    @brief  The IEEE 802.15.4 frame has a number of constant/fixed fields that
 *            can be counted to make frame construction and max payload
 *            calculations easier.  We added DFT & MID field on the standard frame structure.
 *
 *            These include:
 *            1. FCF                  - 2 bytes       - Fixed
 *            2. Sequence number      - 1 byte        - Fixed
 *            3. Addressing fields    - 4 - 20 bytes  - Variable
 *            4. Aux security header  - 0 - 14 bytes  - Variable
 *            5. DFT                  - 2 bits        - Fixed
 *            6. MID                  - 4 bits        - Fixed
 *            7. CRC                  - 2 bytes       - Fixed
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
}sframe_fcf_t;

typedef struct {
  yxh_frame802154_fcf_t fcf;          /**< Frame control field  */
  uint8_t seq;                    /**< Sequence number */
  uint16_t dest_pid;              /**< Destination PAN ID */
  uint8_t dest_addr[8];           /**< Destination address */
  uint16_t src_pid;               /**< Source PAN ID */
  uint8_t src_addr[8];            /**< Source address */
  frame802154_aux_hdr_t aux_hdr;  /**< Aux security header */          
  uint8_t DFT;                    /**< Data Frame Type(2 bits): 0x00 Time synchronization Frame;0x01 BSM Info. Frame;0x10 Traffic statistics Frame;0x11 Reserved*/
  uint8_t MID;                    /**< Message ID(4 bits) */
  uint8_t *payload;               /**< Pointer to 802.15.4 payload */
  int payload_len;                /**< Length of payload field */        
}sframe_t;


#endif /* __CONTIKI_CONF_H__ */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/










