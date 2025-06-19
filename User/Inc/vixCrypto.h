/*
 * vixCrypto.h
 *
 *  Created on: Oct 31, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXCRYPTO_H_
#define INC_VIXCRYPTO_H_


#define _CRC32_ACCUMULATE						0
#define _CRC32_CALCULATE						1

#define _CRC32_BYTE_LEN						4

#define _RNG_BYTE_LEN							4
#define _RNG_BYTE_LEN_MAX						32



//ErrState xCALCULATE_CRC32(uint8_t* pSrc, uint32_t SrcLen, uint32_t* CRC);
uint32_t xCALCULATE_CRC32(uint8_t* pSrc, uint32_t SrcLen);
ErrState xGENERATE_RandomNumber(uint8_t* pData, uint8_t Length, uint8_t ByteOrder);


void TEST_RNG(void);
void TEST_CRC32(void);



#endif /* INC_VIXCRYPTO_H_ */
