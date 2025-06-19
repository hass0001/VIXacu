/*
 * vixLibrary.h
 *
 *  Created on: Nov 1, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXLIBRARY_H_
#define INC_VIXLIBRARY_H_


ErrState xCONVERT_u32Tou8Array(const uint32_t Src, uint8_t* pDest, uint16_t Length, uint8_t ByteOrder);
uint32_t bCONVERT_u8ArrayTou32(const uint8_t* pSrc);
ErrState xCONVERT_ArraryToString(const char* pSrc, uint16_t Length, uint8_t* pDest);
uint16_t bCONVERT_StringToArray(const char* pSrc, uint8_t* pDest);


#endif /* INC_VIXLIBRARY_H_ */
