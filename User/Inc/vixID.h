/*
 * vixID.h
 *
 *  Created on: Nov 20, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXID_H_
#define INC_VIXID_H_


#define _ID_BYTE_LEN_MAX						16
#define _ID_SAVE_BYTE_SIZE_MAX				32
#define _ID_CAPACITY_PER_NOR_SECTOR			128



typedef struct _ID_DownloadTypeDef {
	uint8_t Mask;
	uint8_t ID[_ID_BYTE_LEN_MAX];
	uint8_t PW[2];
	uint8_t RegTime[5];
	uint8_t ExprTime[5];
	uint8_t Reserved[2];
} __attribute__((packed)) ID_DownloadTypeDef;


typedef struct _NOR_IdTypeDef {
	uint32_t IdCount;
	uint32_t iCurrentAddr;
	uint16_t iDelMaskCnt[100];
	uint16_t iRegMaskCnt[100];
} __attribute__((packed)) NOR_IdTypeDef;




#endif /* INC_VIXID_H_ */
