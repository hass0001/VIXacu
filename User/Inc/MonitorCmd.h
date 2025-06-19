/*
 * MonitorCmd.h
 *
 *  Created on: Dec 21, 2024
 *      Author: Administrator
 */

#ifndef INC_MONITORCMD_H_
#define INC_MONITORCMD_H_

/******************************************************************************
**    Type Define
******************************************************************************/
#define FWD_TYP_START (1)
#define FWD_TYP_HEADER (2)
#define FWD_TYP_DATA  (3)
#define FWD_TYP_TAIL (4)
#define FWD_TYP_UPLOAD (5)
#define FWD_TYP_UPEND (6)
#define FWD_TYP_END (7)

#define FWD_TYP_SET_READER (0x10)
#define FWD_TYP_SET_CARD   (0x11)

#define FWD_TYP_SET_FIRE   (0x21)
#define FWD_TYP_CANCEL_FIRE   (0x22)

#define FWD_TYP_SET_TIME (0x45)
#define FWD_TYP_SET_PSN (0x46)
#define FWD_TYP_CLEAR_CARD (0x47)

#define ATE_TYP_READY (0x50)
#define ATE_TYP_FIRMWARE (0x51)
#define ATE_TYP_HARD (0x52)
#define ATE_TYP_MEMORY (0x53)
#define ATE_TYP_PARAMTER (0x54)
#define ATE_TYP_COMMUNICATION (0x55)
#define ATE_TYP_TERMINAL (0x56)
#define ATE_TYP_SET_FACTORY (0x57)
#define ATE_TYP_PANEL (0x58)
#define ATE_TYP_LED_TEST	(0x59)

#define ATE_TYP_AUDIO	(0x5A)
#define ATE_TYP_BUZZER	(0x5B)
#define ATE_TYP_SFLASH_READ (0x5C)
#define ATE_TYP_SFLASH_WRITE   (0x5D)
#define ATE_TYP_EEPROM_READ (0x5E)
#define ATE_TYP_EEPROM_WRITE   (0x5F)

#define FWD_TYP_SET_NETMODE	(0x75)
#define FWD_TYP_SET_DEFAULT	(0x77)

#define FWD_TYP_HISTORY_ON (0x80)
#define FWD_TYP_SET_IP	(0x8A)
#define FWD_TYP_GET_IP	(0x8B)
#define FWD_TYP_SET_HTTP_IP	(0x8C)
#define FWD_TYP_GET_HTTP_IP	(0x8D)

#define FWD_TYP_HISTORY_OFF (0xC0)

#define ATE_STATE_PRM (0)
#define ATE_STATE_STX (1)
#define ATE_STATE_LEN (2)
#define ATE_STATE_CMD (3)
#define ATE_STATE_DAT (4)
#define ATE_STATE_CHE (5)
#define ATE_STATE_ETX (6)

#define FWD_STATE_PRM ATE_STATE_PRM
#define FWD_STATE_STX ATE_STATE_STX
#define FWD_STATE_LEN ATE_STATE_LEN
#define FWD_STATE_CMD ATE_STATE_CMD
#define FWD_STATE_DAT ATE_STATE_DAT
#define FWD_STATE_CHE ATE_STATE_CHE
#define FWD_STATE_ETX ATE_STATE_ETX


#define FW_UPD_PROTOCOL_PRM (0xFF)
#define FW_UPD_PROTOCOL_STX (0x02)
#define FW_UPD_PROTOCOL_ETX (0x03)
/*
#define FW_UPD_PROTOCOL_ST (0x01)
#define FW_UPD_PROTOCOL_TR (0x02)
#define FW_UPD_PROTOCOL_EP (0x04)
*/

#define ATE_CHAR_PRM FW_UPD_PROTOCOL_PRM
#define ATE_CHAR_STX FW_UPD_PROTOCOL_STX
#define ATE_CHAR_ETX FW_UPD_PROTOCOL_ETX

#define NESSLAB900_HEADER        (0x3e)     //'>'

#define ATE_ERROR_TYPE 		(0x02)
#define ATE_ERROR_CHECKSUM  (0x03)


#endif /* INC_MONITORCMD_H_ */
