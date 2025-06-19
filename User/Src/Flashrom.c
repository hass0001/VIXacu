/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: Flashrom.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
/******************************************************************************
**   Include Files
******************************************************************************/
#define    FLASH_GLOBALS 
#include   "FlashRom.h"

extern IWDG_HandleTypeDef hiwdg;

const uint16_t conJulianDateArray[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};//365
                                //      31,+28,+31, +30, +31, +30, +31, +31, +30, +31, +30,  +31  // ���� �ϼ�..

void SPI2_sFlash_Init( void ){
//   OS_CREATERSEMA(&FlashExtSem);
}

#ifdef  SFLASH_READ_TEMP
uint32_t  sFlash4kAddrTemp;
uint8_t sFlashBufferTempTemp[SIZE_OF_OVERCARD_DATA];
#endif

uint8_t   sFlashBuffer[4096];  //1Sector 0x1000 == 4096
uint16_t  u16SectorNum, u16CardAddress, u16SectorOffset;
uint32_t  sFlash4kAddr;
uint16_t gnCountPin;
/* 카드 종류
0: 마스터카드 - 어떤 시간이라도 모든 출입문을 출입할 수 있는 카드..
1: 상시출입카드 - 특별카드timecode 에 지정된 시간을 제외한 시간에 출입이 가능.
2: 단기(방문)카드 - 임시카드. 기간이 지정되어 있는카드..
3: 당일카드 - 임시카드와 같지만 기간이 보통 당일로만 되어 있음..
4: 출입시간제한카드 - 출입시간제한timecode 에 지정된 시간만 출입이 가능
5: 특별카드 - 지정카드. 특별카드 timecode에 지정된 시간도 출입이 가능 (마스터카드를 제외하고 나머지는 그 시간에 출입 못함)
*/

uint16_t CheckMpCardDataInFlash( uint8_t *cardData)
{	//기본 address형 mobile card data에서 찾기.. 카드 index값과 함께 return..
	uint16_t  u16ReturnValue;
	uint16_t  u16SavedJulianDate, u16CurrentJulianDate; //MMDDYY

	u16CardAddress  = cardData[0]*0x100 + cardData[1];//주소를 계산하기 위해서..시리얼 번호. cardData[2,3]은 회사 번호로 바뀌지 않음
	sFlash4kAddr = FLASH_MpCardData_Start + (u16CardAddress * SIZE_OF_MPCARD_MSLOT);
	//#define	FLASH_MpCardData_Start		0x00092000L
	// 해당 어드레스 슬롯의 데이터만 가지고 판단 하므로 슬롯 길이 만큼만 읽는디.
	xREAD_DataFromExNorFlash( sFlash4kAddr, sFlashBuffer, SIZE_OF_MPCARD_MSLOT);       //SIZE_OF_MPCARD_MSLOT 512

	uint8_t nItem =  sFlashBuffer[NUM_OF_MPCARD_DATA_PER_MSLOT * SIZE_OF_MPCARD_DATA];	// 0x200+(31*16))

	for(uint8_t i = 0; i < nItem; i++)   //   0~31
	{		//10
		if( memcmp( sFlashBuffer+(i*SIZE_OF_MPCARD_DATA), cardData+2, 6) == 0)
		{	//있으면..
			return u16ReturnValue = Card_Registerd_OK;	//	Card_No_Granted    1
		}
	}
 /* //기본 저장공간에 없으면 추가공간에서 확인
  for(uint16_t i = 0; i < NUM_OF_DIR_MPCARD_SECTOR; i++)
  { // 2048
	  sFlash4kAddr = FLASH_MpCardData_Direct_Start + (i*SIZE_OF_DIR_MPCARD);  //
	  xREAD_DataFromExNorFlash( sFlash4kAddr, sFlashBuffer, SIZE_OF_DIR_MPCARD );  // 섹터 데이터 읽음

	  for(uint16_t j = 0; j < NUM_OF_DIR_MPCARD_PER_SECTOR; j++)
	  { // 256
		  if( memcmp( (sFlashBuffer+(j*SIZE_OF_BYTE_MPCARD),  cardData, 8) == 0)
		  { //종류 : 상시ID(1), 일시ID(2), 당일카드ID(3)
			  memcpy( sRegMpPin,  &sFlashBuffer[j*SIZE_OF_BYTE_MPCARD+8], 8);	// 찾은 Pin 번호 저장
		      memcpy( sRegMpPass, &sFlashBuffer[j*SIZE_OF_BYTE_MPCARD+16], 2);	// 찾은 Pin의 비번 저장
		      sRegLocation = 2;
			  return u16ReturnValue = Card_Registerd_OK;//Card_Registerd_OK    0
		  }
	  }
  }*/
  return Card_Not_Registerd;  //#define Card_Not_Registerd    3
}

uint16_t CheckRfCardDataInFlash( uint8_t *cardData)
{	//기본 address형 rf card data에서 찾기.. 카드 index값과 함께 return..
	uint16_t  u16ReturnValue;
	uint16_t  u16SavedJulianDate, u16CurrentJulianDate; //MMDDYY

	memcpy( CardParameter.stParam.CardId,  cardData, 4);	// 찾은 카드번호 저장 에서 미등록 번호도 펴시하기 위해 전부 저장

	u16CardAddress  = cardData[0]*0x100 + cardData[1];//주소를 계산하기 위해서..시리얼 번호. cardData[2,3]은 회사 번호로 바뀌지 않음
	sFlash4kAddr = FLASH_RfCardData_Start + (u16CardAddress * SIZE_OF_RFCARD_RSLOT);
	//#define	FLASH_MpCardData_Start		0x02092000L
	// 해당 어드레스 슬롯의 데이터만 가지고 판단 하므로 슬롯 길이 만큼만 읽는디.
	xREAD_DataFromExNorFlash( sFlash4kAddr, sFlashBuffer, SIZE_OF_RFCARD_RSLOT);       //SIZE_OF_RFCARD_RSLOT 256

	uint8_t nItem =  sFlashBuffer[NUM_OF_RFCARD_DATA_PER_RSLOT * SIZE_OF_RFCARD_DATA];	// 0x200+(31*16))

#ifdef DEBUG_MODE
	printf( "CheckRfCardDataInFlash u16CardAddress=%04X, sFlash4kAddr=%08X,nItem=%02X\r\n",
			u16CardAddress,			sFlash4kAddr,			nItem);
#endif
	if( (nItem == 0x0) || (nItem == 0xFF) )	// first time to write
	{
		return Card_Not_Registerd;
	}
	else if( nItem <= NUM_OF_RFCARD_DATA_PER_RSLOT )  //   15면 가득 찼슴
	{	// 슬롯이 비어 있슴
		for(uint8_t i = 0; i < nItem; i++)		//  	1~15
		{		//10
			if( (sFlashBuffer[i*SIZE_OF_RFCARD_DATA+0] == cardData[2]) && (sFlashBuffer[i*SIZE_OF_RFCARD_DATA+1] == cardData[3]) )
			{	//있으면..
//				memcpy( CardParameter.stParam.CardId,  cardData, 4);	// 찾은 카드번호 저장 에서 미등록 번호도 펴시하기 위해 전부 저장
				memcpy( CardParameter.stParam.Pin,  &sFlashBuffer[i*SIZE_OF_RFCARD_DATA+2], 8);	// 찾은 Pin 번호 저장
				memcpy( CardParameter.stParam.PassCode, &sFlashBuffer[i*SIZE_OF_RFCARD_DATA+11], 2);	// 찾은 Pin의 비번 저장
				CardParameter.stParam.Location = 1;
				CardParameter.stParam.DoorAuth_CardKind = sFlashBuffer[i*SIZE_OF_RFCARD_DATA+10];

				u16ReturnValue = ((sFlashBuffer[i*SIZE_OF_RFCARD_DATA+10] & 0xF0) * 0x100) +      // 문이 4개인 TA4 는 출입권한인 앞쪽 4개만 잘라서 16비트중 앞쪽 8비트에 배정 //Door Auth(4) 0000
						  	  	 ((sFlashBuffer[i*SIZE_OF_RFCARD_DATA+10] & 0x0F) << 4);          // 카드타입이 있는 뒤쪽 4비트만 왼쉬프스4 하고 결과값과 합쳐 리턴 antiPass(1), cardType(3) resultvalue(4)
#ifdef DEBUG_MODE
				printf( "Exist  u16ReturnValue=%04X(%02X번째)\r\n",u16ReturnValue, i);
#endif
				if( ( (sFlashBuffer[i*SIZE_OF_RFCARD_DATA+10]&0x07) == 0x02) ||
				    ( (sFlashBuffer[i*SIZE_OF_RFCARD_DATA+10]&0x07) == 0x03) )
				{ // 단기(임시)카드 또는 당일카드일 경우 출입기간을 체크함..

					u16SavedJulianDate = sFlashBuffer[i*SIZE_OF_RFCARD_DATA+11]*0x100 +
							             sFlashBuffer[i*SIZE_OF_RFCARD_DATA+12];
					// const uint16_t conJulianDateArray[13] = {0, 1:0, 2:31, 3:59, 4:90, 5:120, 6:151, 7:181, 8:212, 9:243, 10:273, 11:304, 12:334};//365
					u16CurrentJulianDate = toDayTime.YEAR+(conJulianDateArray[toDayTime.MON] + toDayTime.DATE)*100; // 올해 julian date * 100 + YY

					if((u16SavedJulianDate%100 <= u16CurrentJulianDate%100)&&(u16SavedJulianDate/100 < u16CurrentJulianDate/100))
					{//저장된 시간보다 현재시간이 크면..
						//            if(u16SavedJulianDate < u16CurrentJulianDate){//저장된 시간보다 현재시간이 크면..
						return u16ReturnValue += Card_No_Granted;//Card_No_Granted    1
						//  return (sFlashBuffer[i * SIZE_OF_BYTE_CARD_DATA + 10] & 0xF0) * 0x100 +      // 문이 4개인 TA4 는 출입권한인 앞쪽 4개만 잘라서 16비트중 앞쪽 8비트에 배정 //Door Auth(4) 0000
						//        ((sFlashBuffer[i * SIZE_OF_BYTE_CARD_DATA + 10] & 0x0F) << 4) + Card_No_Granted;//Card_No_Granted    1 //카드타입이 있는 뒤쪽 4비트만 왼쉬프스4 하고 결과값과 합쳐 리턴 antiPass(1), cardType(3) resultvalue(4)
					}
				}
				return u16ReturnValue += Card_Registerd_OK;//Card_Registerd_OK    0
			}
		}
	}
	else	// numItem => 15 ~ 255
	{
#ifdef DEBUG_MODE
		printf( "Card_Not_Registerd  u16ReturnValue=%04X\r\n",u16ReturnValue);
#endif
		return Card_Not_Registerd;
	}
  // 없으면 기본 저장공간에 없으면 추가공간에서 확인
  for(uint16_t i = 0; i < NUM_OF_DIR_RFCARD_SECTOR; i++)
  { // 1024
	  sFlash4kAddr = FLASH_RfCardData_Direct_Start + (i*SIZE_OF_DIR_RFCARD_SLOT);  //
	  xREAD_DataFromExNorFlash( sFlash4kAddr, sFlashBuffer, SIZE_OF_DIR_RFCARD_SLOT );  // 섹터 데이터 읽음

	  uint16_t nUid = sFlashBuffer[NUM_OF_DIR_RFCARD_PER_SECTOR*SIZE_OF_DIR_RFCARD_DATA];
	  for(uint16_t j = 0; j < nUid; j++)
	  { // 227
		  if( memcmp( sFlashBuffer+(j*SIZE_OF_DIR_RFCARD_DATA),  cardData, 4) == 0)
		  { //종류 : 상시ID(1), 일시ID(2), 당일카드ID(3)
				memcpy( CardParameter.stParam.Pin,      &sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+4], 8);	// 찾은 Pin 번호 저장
				memcpy( CardParameter.stParam.PassCode, &sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+13], 2);	// 찾은 Pin의 비번 저장
				CardParameter.stParam.Location = 2;
				CardParameter.stParam.DoorAuth_CardKind = sFlashBuffer[i*SIZE_OF_RFCARD_DATA+12];

				u16ReturnValue = ((sFlashBuffer[j * SIZE_OF_DIR_RFCARD_DATA + 12] & 0xF0) * 0x100) +      // 문이 4개인 TA4 는 출입권한인 앞쪽 4개만 잘라서 16비트중 앞쪽 8비트에 배정 //Door Auth(4) 0000
		                       ((sFlashBuffer[j * SIZE_OF_DIR_RFCARD_DATA + 12] & 0x0F) << 4);          // 카드타입이 있는 뒤쪽 4비트만 왼쉬프스4 하고 결과값과 합쳐 리턴 antiPass(1), cardType(3) resultvalue(4)
#ifdef DEBUG_MODE
				printf( "Exist Extended u16ReturnValue=%04X\r\n",u16ReturnValue);
#endif
		      if(((sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+12]&0x07) == 0x02)||
		         ((sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+12]&0x07) == 0x03))
		      { //종류 : 상시ID(1), 일시ID(2), 당일카드ID(3)
		          uint16_t u16SavedJulianDate, u16CurrentJulianDate; //MMDDYY
		          u16SavedJulianDate = sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+13]*0x100 +
		        		               sFlashBuffer[j*SIZE_OF_DIR_RFCARD_DATA+14];
		          u16CurrentJulianDate = toDayTime.YEAR+(conJulianDateArray[toDayTime.MON] + toDayTime.DATE)*100; // 올해 julian date * 100 + YY
		          if(u16SavedJulianDate < u16CurrentJulianDate)
		          {//저장된 시간보다 현재시간이 크면..
		        	  return u16ReturnValue += Card_No_Granted;//Card_No_Granted    1
		//            return (sFlashBuffer[i*SIZE_OF_BYTE_CARD_DATA+3] & 0xF0) * 0x100 +      // 문이 4개인 TA4 는 출입권한인 앞쪽 4개만 잘라서 16비트중 앞쪽 8비트에 배정 //Door Auth(4) 0000
		//                  ((sFlashBuffer[i*SIZE_OF_BYTE_CARD_DATA+3] & 0x0F) << 4) + Card_No_Granted;   //Card_No_Granted    1  //카드타입이 있는 뒤쪽 4비트만 왼쉬프스4 하고 결과값과 합쳐 리턴 antiPass(1), cardType(3) resultvalue(4)
		          }
		      }
		      return u16ReturnValue += Card_Registerd_OK;//Card_Registerd_OK    0
		//      return (sFlashBuffer[i*SIZE_OF_BYTE_CARD_DATA+3] & 0xF0) * 0x100 +      //앞쪽 4개만 유효하므로.. TA4
		//             ((sFlashBuffer[i*SIZE_OF_BYTE_CARD_DATA+3] & 0x0F) << 4) + Card_Registerd_OK;//Card_Registerd_OK    0
		  }
		  else
		  {
#ifdef DEBUG_MODE
				printf( "Card_Not_Registerd Extened Area u16ReturnValue=%04X\r\n",u16ReturnValue);
#endif
		  }
	  }
  }
#ifdef DEBUG_MODE
	printf( "CheckRfCardDataInFlash Result Card%02X%02X%02X%002X,Pin%02X%02X%02X%02X%02X%02X%02X%02X,DoorAuth_CardKind=%02X\r\n",
		CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
		CardParameter.stParam.Pin[0], CardParameter.stParam.Pin[1],CardParameter.stParam.Pin[2],CardParameter.stParam.Pin[3],
		CardParameter.stParam.Pin[4],CardParameter.stParam.Pin[5],CardParameter.stParam.Pin[6],CardParameter.stParam.Pin[7],
		CardParameter.stParam.DoorAuth_CardKind);  //
#endif
  return Card_Not_Registerd;  //#define Card_Not_Registerd    3
}


uint8_t WriteMpCardDataToSerialFlash( uint8_t *stCardData)
{   //address형 card data 쓰기..
	uint16_t ulFor, lastbufIndex, rdbufIndex;
	uint8_t numItem = 0, setItem;
	uint8_t nReturn = 0;
	uint16_t rdSectorIndex;
	uint32_t rdSectorAddr;
	uint32_t bitRegOrNot;
	uint32_t wrSectorAddr;

	u16CardAddress  = stCardData[0]*0x100 + stCardData[1];//(*(stCardData+0))*0x100+(*(stCardData+1));
	sFlash4kAddr = FLASH_MpCardData_Start + (u16CardAddress * SIZE_OF_MPCARD_MSLOT);
	rdSectorAddr =  sFlash4kAddr & 0xFFFFF000;	//
	rdSectorIndex = sFlash4kAddr & 0x00000FFF;	// 0xN00
	xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);       //  Read 4K (==1Sector) old data

	//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
	lastbufIndex = rdSectorIndex + ( NUM_OF_MPCARD_DATA_PER_MSLOT * SIZE_OF_MPCARD_DATA );	// 0x200+(31*16))
	numItem = sFlashBuffer[lastbufIndex+0];  //  last information buffer[0]
	if((numItem == 0x0) || (numItem == 0xFF))	// first time to write
	{
		setItem = 0;
		sFlashBuffer[lastbufIndex+0] = 0;
		nReturn = Card_Registerd_OK;
	}
	else if( numItem < NUM_OF_MPCARD_DATA_PER_MSLOT )  //   31 => 가득 찼슴
	{	// 슬롯이 비어 있슴
		setItem = numItem;
		nReturn = Card_Registerd_OK;
	}
	else	// numItem => 32 ~ 255
	{
		nReturn = Card_No_Space;
	}

	if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
	{
		rdbufIndex = rdSectorIndex + (setItem*SIZE_OF_MPCARD_DATA);
		memcpy( sFlashBuffer+rdbufIndex+0, stCardData+2, 16);

		sFlashBuffer[lastbufIndex+0] += 1;
		WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
		return nReturn;
	}
	else
	{	// 	어드레스 모드에서 저장 안 되니 확장 모드에서 저장하도록 시도. 저장되어 있지도, 자리가 없어 새로 등록도 안되면.. 추가공간에..
		for(uint16_t i = 0; i < NUM_OF_DIR_MPCARD_SECTOR; i++)   //	2048
		{
			sFlash4kAddr = FLASH_MpCardData_Direct_Start + (i * 0x1000);
			rdSectorAddr = sFlash4kAddr;
			xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);  //  Read 4K (==1Sector) old data

			//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
			numItem = sFlashBuffer[IDX_OF_DIR_MPCARD_COUNT];  //  last information buffer[0]
			if( (numItem == 0x0) || (numItem == 0xFF))
			{	// 처음 저장 인것
				setItem = 0;
				sFlashBuffer[IDX_OF_DIR_MPCARD_COUNT] = 0;
				nReturn = Card_Registerd_OK;
				break;
			}
			else if( numItem < NUM_OF_DIR_MPCARD_PER_SECTOR) //	227
			{
				setItem = numItem;
				nReturn = Card_Registerd_OK;
				break;
			}
			else
			{ 	// 갯 수 넘어감. 공간 없슴
				nReturn = Card_No_Space;
				continue;
			}
		}

		if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
		{
			rdbufIndex = (setItem*SIZE_OF_DIR_MPCARD_DATA);
			memcpy( sFlashBuffer+rdbufIndex+0, stCardData, 18);
			sFlashBuffer[IDX_OF_DIR_MPCARD_COUNT] += 1;

			WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
			return nReturn;
		}
	}
	return Card_No_Space;//추가공간에도 넣을 공간이 없으면..
}

uint8_t ClearRfCardIndexOnSerialFlash( void )
{   //address형 card data 쓰기..
	uint16_t ulFor, lastbufIndex, rdbufIndex;
	uint8_t numItem = 0, setItem;
	uint8_t nReturn = 0;
	uint16_t rdSectorIndex;
	uint32_t rdSectorAddr;
	uint32_t bitRegOrNot;
	uint32_t wrSectorAddr;
	uint32_t nUid, nForAddr;

	for ( nForAddr = FLASH_RfCardData_Start; nForAddr < FLASH_RfCardData_End; nForAddr += 0x1000)
	{
		xREAD_DataFromExNorFlash( nForAddr, sFlashBuffer, 0x1000);

		for ( nUid = 0; nUid < NUM_OF_RSLOT_PER_SECTOR; nUid++)
		{
			rdSectorIndex = nUid * SIZE_OF_RFCARD_RSLOT;
			//  sFlashBuffer // 마지막 정보를 읽고 데이터 숫자를 0으로 지음
			lastbufIndex = rdSectorIndex + ( NUM_OF_RFCARD_DATA_PER_RSLOT * SIZE_OF_RFCARD_DATA );
			// 0xN00+(15*16))
			sFlashBuffer[lastbufIndex+0] = 0;  //  last information buffer[0]
		}

		WriteSFlash4kSectorData(nForAddr, sFlashBuffer);
	}

	// 다이렉트 섹터 데이터 크리어 -> 기본 저장공간에 지우고 추가공간에서 확인
	  for(uint16_t i = 0; i < NUM_OF_DIR_RFCARD_SECTOR; i++)
	  { // 1024
		  sFlash4kAddr = FLASH_RfCardData_Direct_Start + (i*SIZE_OF_DIR_RFCARD_SLOT);  //
		  xREAD_DataFromExNorFlash( sFlash4kAddr, sFlashBuffer, SIZE_OF_DIR_RFCARD_SLOT );  // 섹터 데이터 읽음

		  sFlashBuffer[NUM_OF_DIR_RFCARD_PER_SECTOR*SIZE_OF_DIR_RFCARD_DATA] = 0;
		  WriteSFlash4kSectorData(sFlash4kAddr, sFlashBuffer);
	  }

	return 1;//추가공간에도 넣을 공간이 없으면..
}

uint8_t WriteRfCardDataToSerialFlash( uint8_t *stCardData)
{   //address형 card data 쓰기..
	static uint8_t sameCard = 0;
	if( CheckRfCardDataInFlash(stCardData) == Card_Not_Registerd)
	{
		NewWriteRfCardDataToSerialFlash(stCardData);
		sameCard = 0;
	}
	else   /// 이미 등록 되어 있슴
	{
		if( sameCard >= 3)  // 같은 데이터가 0(두번째), 1(세번째), 2(네번째), 3(다섰번 째는 수정되었으므로 오버라이트)
		{
			OverWriteRfCardDataToSerialFlash(stCardData);
			sameCard = 0;
		}
		else
		{
			sameCard++;
		}
	}
}

uint8_t OverWriteRfCardDataToSerialFlash( uint8_t *stCardData)
{   //address형 card data 쓰기..
	uint16_t ulFor, lastbufIndex, rdbufIndex;
	uint8_t numItem = 0, setItem;
	uint8_t nReturn = 0;
	uint16_t rdSectorIndex;
	uint32_t rdSectorAddr;
	uint32_t bitRegOrNot;
	uint32_t wrSectorAddr;

	u16CardAddress  = stCardData[0]*0x100 + stCardData[1];//(*(cardData+0))*0x100+(*(cardData+1));
	sFlash4kAddr = FLASH_RfCardData_Start + (u16CardAddress * SIZE_OF_RFCARD_RSLOT);
	rdSectorAddr =  sFlash4kAddr & 0xFFFFF000;	//
	rdSectorIndex = sFlash4kAddr & 0x00000FFF;	// 0xN00

#ifdef DEBUG_MODE
	printf( "OverWriteRfCardDataToSerialFlash rdSectorAddr=%08X, rdSectorIndex=%08X\r\n",
			rdSectorAddr,			rdSectorIndex);
#endif
	xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);
	//  Read 4K (==1Sector) old data. 섹터로 읽어야 나중에 쓸때 섹터로 쓸 수 있다.
	//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
	lastbufIndex = rdSectorIndex + ( NUM_OF_RFCARD_DATA_PER_RSLOT * SIZE_OF_RFCARD_DATA );
	// 0xN00+(15*16))
	numItem = sFlashBuffer[lastbufIndex+0];  //  last information buffer[0]

#ifdef DEBUG_MODE
	printf( "OverWriteRfCardDataToSerialFlash u16CardAddress=%04X, lastbufIndex=%04X,numItem=%02X\r\n",
			u16CardAddress,			lastbufIndex,			numItem);
#endif
	if( (numItem == 0x0) || (numItem == 0xFF) )	// first time to write
	{
		setItem = 0;
		sFlashBuffer[lastbufIndex+0] = 0;
		nReturn = Card_Registerd_OK;   /// Error -> 있다고 했는데 쓰인 적이 없는 경우
	}
	else if( numItem < NUM_OF_RFCARD_DATA_PER_RSLOT )  //   15면 가득 찼슴
	{	// 슬롯이 비어 있슴
		setItem = numItem;
		nReturn = Card_Registerd_OK;
	}
	else	// numItem => 15 ~ 255
	{
		nReturn = Card_No_Space;  // 있다고 했는데 없으므로 확장 영역 쪽에서 찾음.
	}
#ifdef DEBUG_MODE
	printf( "OverWrite  nReturn=%02X\r\n",nReturn);
#endif
	if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
	{

		if( setItem == 0)
		{ // 있다고 했는데 카운트가 0 이면 오류
			rdbufIndex = rdSectorIndex ;  // 0xN00+(setItem*16))  ,, 0xN00+(0*16)
			memcpy( sFlashBuffer+rdbufIndex+0, stCardData+2, 14);
			sFlashBuffer[lastbufIndex+0] += 1;  //  있는 것을 덮어 썻으므로
		}
		else
		{
			for(uint8_t i = 0; i < setItem; i++)		//  	1~15
			{		//10
				rdbufIndex = rdSectorIndex + (i*SIZE_OF_RFCARD_DATA);  // 0xN00+(setItem*16))

				if( (sFlashBuffer[rdbufIndex+0] == stCardData[2]) && (sFlashBuffer[rdbufIndex+1] == stCardData[3]) )
				{	//있으면..
					memcpy( sFlashBuffer+rdbufIndex+0, stCardData+2, 14);

#ifdef DEBUG_MODE
					printf( "OverWrite 있으면 =%04X, sFlashBuffer=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",rdbufIndex,
							sFlashBuffer[rdbufIndex+0],sFlashBuffer[rdbufIndex+1],sFlashBuffer[rdbufIndex+2],sFlashBuffer[rdbufIndex+3],
							sFlashBuffer[rdbufIndex+4],sFlashBuffer[rdbufIndex+5],sFlashBuffer[rdbufIndex+6],sFlashBuffer[rdbufIndex+7],
							sFlashBuffer[rdbufIndex+8],sFlashBuffer[rdbufIndex+9],sFlashBuffer[rdbufIndex+10],sFlashBuffer[rdbufIndex+11],
							sFlashBuffer[rdbufIndex+12],sFlashBuffer[rdbufIndex+13]);
#endif
					//		sFlashBuffer[lastbufIndex+0] += 1;  //  있는 것을 덮어 썻으므로
					i = setItem;  // for 문을 나가기 위해
					break;
				}
			}
		}

#ifdef DEBUG_MODE
		printf( "OverWrite count=%02X\r\n",sFlashBuffer[lastbufIndex+0]);  //  있는 것을 덮어 썻으므로	);
#endif
		WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
		return nReturn;
	}
	else  // 확장 영역에 있는 경우
	{	// 	어드레스 모드에서 저장 안 되니 확장 모드에서 저장하도록 시도. 저장되어 있지도, 자리가 없어 새로 등록도 안되면.. 추가공간에..
		for(uint16_t i = 0; i < NUM_OF_DIR_RFCARD_SECTOR; i++)   //	1024  개의 섹터 안에 4K 씩 읽어 찾음
		{
			sFlash4kAddr = FLASH_RfCardData_Direct_Start + (i * 0x1000);
			rdSectorAddr = sFlash4kAddr;
			xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);  //  Read 4K (==1Sector) old data

			uint16_t nUid = sFlashBuffer[NUM_OF_DIR_RFCARD_PER_SECTOR*SIZE_OF_DIR_RFCARD_DATA]; //  last information buffer[0]
			for(uint16_t j = 0; j < nUid; j++)
			{ // 227
				if( memcmp( sFlashBuffer+(j*SIZE_OF_DIR_RFCARD_DATA),  stCardData, 4) == 0)  // 같은 데이터 찾음, 덮어쓰고 나가기
				{ //종류 : 상시ID(1), 일시ID(2), 당일카드ID(3)

					if( (numItem == 0x0) || (numItem == 0xFF) )
					{	// 처음 저장 인것 setItem  있다고 했는데 처음인 것은 에러
						setItem = 0;
						rdbufIndex = (setItem*SIZE_OF_DIR_RFCARD_DATA);
						memcpy( sFlashBuffer+rdbufIndex+0, stCardData, 16);
						sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT] = 1;
						nReturn = Card_Registerd_OK;
						break;
					}
					else if( numItem < NUM_OF_DIR_RFCARD_PER_SECTOR) //	227
					{
						setItem = numItem;
						rdbufIndex = (setItem*SIZE_OF_DIR_RFCARD_DATA);
						memcpy( sFlashBuffer+rdbufIndex+0, stCardData, 16);
			//		sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT] = 1;  // ejvdjTmrldlmFH
						nReturn = Card_Registerd_OK;
						break;
					}
					else
					{ 	// 갯 수 넘어감. 공간 없슴
						nReturn = Card_No_Space;
						continue;
					}
				}
			}

			if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
			{
#ifdef DEBUG_MODE
				printf( "OverWrite 확장 count=%02X\r\n",sFlashBuffer[lastbufIndex+0]);  //  있는 것을 덮어 썻으므로	);
#endif
				WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
				return nReturn;
			}
			else
			{
#ifdef DEBUG_MODE
				printf( "OverWrite 확장 Card_No_Space=%02X\r\n",sFlashBuffer[lastbufIndex+0]);  //  있는 것을 덮어 썻으므로	);
#endif
			}
		}
	}
	return Card_No_Space; //추가공간에도 넣을 공간이 없으면..
}


uint8_t NewWriteRfCardDataToSerialFlash( uint8_t *stCardData)
{   //address형 card data 쓰기..
	uint16_t ulFor, lastbufIndex, rdbufIndex;
	uint8_t numItem = 0, setItem;
	uint8_t nReturn = 0;
	uint16_t rdSectorIndex;
	uint32_t rdSectorAddr;
	uint32_t bitRegOrNot;
	uint32_t wrSectorAddr;

	u16CardAddress  = stCardData[0]*0x100 + stCardData[1];//(*(cardData+0))*0x100+(*(cardData+1));
	sFlash4kAddr = FLASH_RfCardData_Start + (u16CardAddress * SIZE_OF_RFCARD_RSLOT);
	rdSectorAddr =  sFlash4kAddr & 0xFFFFF000;	//
	rdSectorIndex = sFlash4kAddr & 0x00000FFF;	// 0xN00

#ifdef DEBUG_MODE
	printf( "NewWriteRfCardDataToSerialFlash rdSectorAddr=%08X, rdSectorIndex=%08X\r\n",
			rdSectorAddr,			rdSectorIndex);
#endif
	xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);
	//  Read 4K (==1Sector) old data. 섹터로 읽어야 나중에 쓸때 섹터로 쓸 수 있다.
	//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
	lastbufIndex = rdSectorIndex + ( NUM_OF_RFCARD_DATA_PER_RSLOT * SIZE_OF_RFCARD_DATA );
	// 0xN00+(15*16))
	numItem = sFlashBuffer[lastbufIndex+0];  //  last information buffer[0]

#ifdef DEBUG_MODE
	printf( "NewWriteRfCardDataToSerialFlash u16CardAddress=%04X, sFlash4kAddr=%08X,nItem=%02X, lastbufIndex=%04X\r\n",
			u16CardAddress,			sFlash4kAddr,			numItem, lastbufIndex);
#endif
	if( (numItem == 0x0) || (numItem == 0xFF) )	// first time to write
	{
		setItem = 0;
		sFlashBuffer[lastbufIndex+0] = 0;
		nReturn = Card_Registerd_OK;
	}
	else if( numItem < NUM_OF_RFCARD_DATA_PER_RSLOT )  //   15면 가득 찼슴
	{	// 슬롯이 비어 있슴
		setItem = numItem;
		nReturn = Card_Registerd_OK;
	}
	else	// numItem => 15 ~ 255
	{
		nReturn = Card_No_Space;
	}

	if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
	{
		rdbufIndex = rdSectorIndex + (setItem*SIZE_OF_RFCARD_DATA);  // 0xN00+(setItem*16))
		memcpy( sFlashBuffer+rdbufIndex+0, stCardData+2, 14);

		sFlashBuffer[lastbufIndex+0] += 1;
#ifdef DEBUG_MODE
		printf( "NewWriteRfCardDataToSerialFlash rdSectorAddr=%08X, rdbufIndex=%04X,nItem=%02X, lastbufIndex=%04X\r\n",
				rdSectorAddr,			rdbufIndex,			numItem, lastbufIndex);

		printf( "NewWrite 새로운=%04X, sFlashBuffer=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",rdbufIndex,
				sFlashBuffer[rdbufIndex+0],sFlashBuffer[rdbufIndex+1],sFlashBuffer[rdbufIndex+2],sFlashBuffer[rdbufIndex+3],
				sFlashBuffer[rdbufIndex+4],sFlashBuffer[rdbufIndex+5],sFlashBuffer[rdbufIndex+6],sFlashBuffer[rdbufIndex+7],
				sFlashBuffer[rdbufIndex+8],sFlashBuffer[rdbufIndex+9],sFlashBuffer[rdbufIndex+10],sFlashBuffer[rdbufIndex+11],
				sFlashBuffer[rdbufIndex+12],sFlashBuffer[rdbufIndex+13]);
#endif
		WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
		return nReturn;
	}
	else
	{	// 	어드레스 모드에서 저장 안 되니 확장 모드에서 저장하도록 시도. 저장되어 있지도, 자리가 없어 새로 등록도 안되면.. 추가공간에..
		for(uint16_t i = 0; i < NUM_OF_DIR_RFCARD_SECTOR; i++)   //	1024
		{
			sFlash4kAddr = FLASH_RfCardData_Direct_Start + (i * 0x1000);
			rdSectorAddr = sFlash4kAddr;
			xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);  //  Read 4K (==1Sector) old data

			//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
			numItem = sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT];  //  last information buffer[0]

#ifdef DEBUG_MODE
			printf( "NewWriteRfCardDataToSerialFlash rdSectorAddr=%08X, sFlash4kAddr=%08X,nItem=%02X, lastbufIndex=%04X\r\n",
					rdSectorAddr,			sFlash4kAddr,			numItem, lastbufIndex);
#endif
			if( (numItem == 0x0) || (numItem == 0xFF) )
			{	// 처음 저장 인것
				setItem = 0;
				sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT] = 0;
				nReturn = Card_Registerd_OK;
				break;
			}
			else if( numItem < NUM_OF_DIR_RFCARD_PER_SECTOR) //	227
			{
				setItem = numItem;
				nReturn = Card_Registerd_OK;
				break;
			}
			else
			{ 	// 갯 수 넘어감. 공간 없슴
				nReturn = Card_No_Space;
				continue;
			}
		}
#ifdef DEBUG_MODE
		printf( "NewWriteRfCardDataToSerialFlash sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT]=%02X\r\n",
				sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT]);
#endif
		if( nReturn == Card_Registerd_OK)   // 버퍼에 쓰기 완료한 데이터를 플래쉬 섹터에 저장
		{
			rdbufIndex = (setItem*SIZE_OF_DIR_RFCARD_DATA);
			memcpy( sFlashBuffer+rdbufIndex+0, stCardData, 16);

			sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT] += 1;

#ifdef DEBUG_MODE
			printf( "NewWriteRfCardDataToSerialFlash rdSectorAddr=%08X, sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT]=%02X\r\n",
					rdSectorAddr, sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT]);

			printf( "NewWrite 새로운 확장=%04X, sFlashBuffer=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",rdbufIndex,
					sFlashBuffer[rdbufIndex+0],sFlashBuffer[rdbufIndex+1],sFlashBuffer[rdbufIndex+2],sFlashBuffer[rdbufIndex+3],
					sFlashBuffer[rdbufIndex+4],sFlashBuffer[rdbufIndex+5],sFlashBuffer[rdbufIndex+6],sFlashBuffer[rdbufIndex+7],
					sFlashBuffer[rdbufIndex+8],sFlashBuffer[rdbufIndex+9],sFlashBuffer[rdbufIndex+10],sFlashBuffer[rdbufIndex+11],
					sFlashBuffer[rdbufIndex+12],sFlashBuffer[rdbufIndex+13]);
#endif

			WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
			return nReturn;
		}
		else
		{
#ifdef DEBUG_MODE
			printf( "NewWriteRfCardDataToSerialFlash Card_No_Space=%02X, nReturn=%02X\r\n",
					sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT], nReturn);
#endif
		}
	}
	return Card_No_Space;//추가공간에도 넣을 공간이 없으면..
}

uint8_t WriteRfCardDataBlockToSerialFlash(uint8_t *cardData){//address형 card data 블럭으로 쓰기..
/*
ID_Address(2) : 카드데이터 중 앞2바이트
ID단위길이(1) 사용기간포함자료는 5, 출입문권한만들어있으면 3, 기타 구성에 따라 다름
    5바이트의 카드정보 데이터.. cardData[2], cardData[3], {Door Auth(4), antiPass(1), cardType(3)}, u16SavedJulianDate(16)
    3바이트의 카드정보 데이터.. cardData[2], cardData[3], {Door Auth(8)}
ID개수(1) : 저장할 데이터 개수 (1, 4, 8, 16에 따라 다르므로)
ID데이터(240) : 최대 길이를 그냥 고정시킴 (16MB 일 경우 최대값은 240바이트임)

이는 반드시 ACU 메모리에 저장되는 형태의 데이터로 전송해야 함
이는 기본영역에만 해당하며 확장영역은 그냥 놔두는걸로..
*/
	uint16_t ulFor;
	uint32_t wrAddr;

  return  Card_Not_Registerd; //추가공간 끝까지 찾았는데도 없으면..
}


uint8_t DeleteRfCardDataOnSerialFlash( uint8_t *cardData, uint8_t u8Flag)
{	//본래 영역 카드데이터 지우기..
	uint16_t ulFor, lastbufIndex, rdbufIndex;
	uint8_t numItem = 0, setItem;
	uint8_t nReturn = 0;
	uint16_t rdSectorIndex;
	uint32_t rdSectorAddr;
	uint32_t bitRegOrNot;
	uint32_t wrSectorAddr;

	u16CardAddress  = cardData[0]*0x100 + cardData[1];//(*(cardData+0))*0x100+(*(cardData+1));
	sFlash4kAddr = FLASH_RfCardData_Start + (u16CardAddress * SIZE_OF_RFCARD_RSLOT);
	//  Read 4K (==1Sector) old data. 섹터로 읽어야 나중에 쓸때 섹터로 쓸 수 있다.
	rdSectorAddr =  sFlash4kAddr & 0xFFFFF000;	//
	rdSectorIndex = sFlash4kAddr & 0x00000FFF;	// 0xN00
	xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);       //  Read 4K (==1Sector) old data
	//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
	lastbufIndex = rdSectorIndex + ( NUM_OF_RFCARD_DATA_PER_RSLOT * SIZE_OF_RFCARD_DATA );	// 0투00 + (15*16) = 0xB00 + 240
	numItem = sFlashBuffer[lastbufIndex];  //  last information buffer[0]
	if( (numItem == 0x0) || (numItem == 0xFF) )	// first time to write 쓰여진 데이터가 하나도 없슴
	{
		return Card_Not_Registerd;
	}
	else if( (numItem > 0) && (numItem < NUM_OF_RFCARD_DATA_PER_RSLOT) )  // 빈 공간이 있슴 , 15 면 => 가득 찼슴
	{	// 슬롯이 비어 있슴 이면 이중에 하나 일 수 있습
		uint16_t nIdx = 0;
		for( uint8_t ni = 0; ni < numItem; ni++)
		{
			nIdx = rdSectorIndex + (ni * SIZE_OF_RFCARD_DATA);   // 섹터로 읽은 버퍼에서 지정된 어드레스 배열에서 읽어 판단.
			if( (sFlashBuffer[nIdx+0] == cardData[2]) && (sFlashBuffer[nIdx+1] == cardData[3]) )
			{
				setItem = ni;
				break;
			}
		}
		if (setItem < numItem)
		{	// 지정 자리의 데이터를 마지막 데아터로 변경하고 카드 데이터 수를 하나 줄인다.
			uint16_t nDstIdx = rdSectorIndex + (setItem * SIZE_OF_RFCARD_DATA);   // 지울 데이터 Index
			uint16_t nSrcIdx = rdSectorIndex + ((numItem-1) * SIZE_OF_RFCARD_DATA);   // 복사할 마지막 데이터 Index
			memcpy( sFlashBuffer+nDstIdx, sFlashBuffer+nSrcIdx, SIZE_OF_RFCARD_DATA);
			sFlashBuffer[lastbufIndex] -= 1;

			WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
			return Card_Deleted_OK;
		}
		else
		{
			return Card_Not_Registerd;
		}
	}
	else	// numItem => 16 ~ 255
	{	// 어드레스 영역에는 가득 차서 확장 영역에서 찾아 지움
		for(uint16_t ni = 0; ni < NUM_OF_DIR_RFCARD_SECTOR; ni++)   //	1024 Sectors
		{
			sFlash4kAddr = FLASH_RfCardData_Direct_Start + (ni * 0x1000);
			rdSectorAddr = sFlash4kAddr;
			xREAD_DataFromExNorFlash( rdSectorAddr, sFlashBuffer, 0x1000);  //  Read 4K (==1Sector) old data

			//  sFlashBuffer // 마지막 정보를 읽고 데이터 처리
			numItem = sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT];  //  last information buffer[0]
			if( (numItem == 0x0) || (numItem == 0xFF) )
			{	// 처음 저장 인것
				return Card_Not_Registerd;
			}
			else if( (numItem > 0) && (numItem < NUM_OF_DIR_RFCARD_PER_SECTOR) )	//	227
			{  // 이 섹터 슬롯에 있어야 한다.
				for( uint8_t nj = 0; nj < numItem; nj++)
				{
					rdbufIndex = (nj*SIZE_OF_DIR_RFCARD_DATA);
					if( memcmp( sFlashBuffer+rdbufIndex, cardData, 4) == 0) // 있으면 지우지
					{
						setItem = nj;
						break;
					}
				}
				if (setItem < numItem)
				{	// 지정 자리의 데이터를 마지막 데아터로 변경하고 카드 데이터 수를 하나 줄인다. 섹터 슬롯의 마지막 데이터를 옮긴다.
					uint16_t nDstIdx = rdbufIndex;   // 지울 데이터 Index
					uint16_t nSrcIdx = ((numItem-1) * SIZE_OF_DIR_RFCARD_DATA);   // 복사할 마지막 데이터 Index
					memcpy( sFlashBuffer+nDstIdx, sFlashBuffer+nSrcIdx, SIZE_OF_DIR_RFCARD_DATA);

					sFlashBuffer[IDX_OF_DIR_RFCARD_COUNT] -= 1;
					WriteSFlash4kSectorData(rdSectorAddr, sFlashBuffer);
					return Card_Deleted_OK;
				}
				else
				{
					continue;
				}
			}
			else
			{ 	// 갯 수 넘어감. 공간 없슴
				continue;
			}
		}
	}
	return  Card_Not_Registerd; //추가공간 끝까지 찾았는데도 없으면..
}

uint8_t EraseSFlash(uint32_t u32StartAddress, uint32_t u32EndAddress)
{
	uint32_t sFlash_addr = u32StartAddress;

	for(sFlash_addr = u32StartAddress; sFlash_addr < u32EndAddress;  sFlash_addr += 0x00001000L)
	{//0x1000 == 4KB
		MyMsDelay(1); // 100 mS
		HAL_IWDG_Refresh(&hiwdg);
#ifdef DEBUG_MODE
			printf( "*");
#endif
		xERASE_ExFlash4kSector(sFlash_addr);
	}
	return TRUE;
}

uint8_t ClearCardDataOfSerialFlashAll(void)
{  //카드데이터 수를 모두 0으로, 추가데이터는 sector erase..
	EraseSFlash( FLASH_RfCardData_Start, FLASH_RfCardData_End);
	return TRUE;
}

//
//uint8_t ClearEventDataOfSerialFlashAll(void){  //카드데이터 수를 모두 0으로, 추가데이터는 sector erase..
//  uint32_t sFlash_addr = FLASH_EventHisory_Start;
//  if(FLASH_CardData_End - sFlash_addr >= 0x00010000L){//0x10000 == 65536 == 64KB
//    for(; FLASH_EventHisory_End - sFlash_addr >= 0x00010000L;  sFlash_addr += 0x00010000L){//0x10000 == 65536 == 64KB
//      OS_Delay(1); // 100 mS
//      ExtFlashBlock64kErase(sFlash_addr);
//    }
//  }
//
//  uint32_t sFlash_addr = FLASH_EventHisory_Start;
//  if(FLASH_EventHisory_End - sFlash_addr >= 0x00008000L){//0x08000 == 32KB
//    for(; FLASH_EventHisory_End - sFlash_addr >= 0x00008000L;  sFlash_addr += 0x00008000L){//0x08000 == 32KB
//      OS_Delay(1); // 100 mS
//      ExtFlashBlock32kErase(sFlash_addr);
//    }
//  }
//  if(FLASH_EventHisory_End - sFlash_addr >= 0x00001000L){//0x1000 == 4KB
//    for(; FLASH_EventHisory_End - sFlash_addr >= 0x00001000L;  sFlash_addr += 0x00001000L){//0x1000 == 4KB
//      OS_Delay(1); // 100 mS
//      xERASE_ExFlash4kSector(sFlash_addr);
//    }
//  }
//  return TRUE;
//}

void WriteSFlash4kSectorData( uint32_t sectorAddress, uint8_t * buf4kData)
{
	uint32_t wrSectorAddr;
	uint16_t nf;
	uint8_t  rdBuffer[4096];  // 1Sector 0x1000 == 4096

	xERASE_ExFlash4kSector(sectorAddress);
	wrSectorAddr = sectorAddress;
	for( uint16_t ulFor = 0; ulFor < QSPI_SUBSECTOR_SIZE; ulFor+=MX25L51245G_PAGE_SIZE)
	{
		xWRITE_DataToExNorFlash(wrSectorAddr, buf4kData+ulFor, MX25L51245G_PAGE_SIZE);
		wrSectorAddr += MX25L51245G_PAGE_SIZE;

		HAL_IWDG_Refresh(&hiwdg);
	}
/*
	xREAD_DataFromExNorFlash( sectorAddress, rdBuffer, 0x1000);  //  Read 4K (==1Sector) old data

	for( nf = 0; nf <  QSPI_SUBSECTOR_SIZE; nf++)
	{
		if( buf4kData[nf] != rdBuffer[nf] )
			printf("error[%d]", nf);
	}
*/
}


//////////////  Authority functions  ///////////////

uint8_t AddAuthorityToSerialFlash( uint8_t *strPinData, uint8_t *strCardData, uint8_t nPort)
{   //.
	uint16_t numItem = 0;
	uint8_t nReturn = 0;
	uint16_t rdIndex;
	uint32_t rdSecCnt;

	if( CheckRfCardDataInFlash(strCardData) == Card_Not_Registerd)
	{
#ifdef DEBUG_MODE
		printf( "AddAuthorityToSerialFlash isn't stCardData.\n\r");
#endif
	}
	else   /// 등록 되어 있슴
	{
#ifdef DEBUG_MODE
		printf( "AddAuthorityToSerialFlash Result Card%02X%02X%02X%002X,Pin%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
			CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
			CardParameter.stParam.Pin[0], CardParameter.stParam.Pin[1],CardParameter.stParam.Pin[2],CardParameter.stParam.Pin[3],
			CardParameter.stParam.Pin[4],CardParameter.stParam.Pin[5],CardParameter.stParam.Pin[6],CardParameter.stParam.Pin[7]);
#endif
		CardParameter.stParam.DoorAuth_CardKind |= (0x80 >> nPort);  //  All door 1111 권한  || 1:상시 ID.  2:일시 ID.  3:당일카드 ID.

		OverWriteRfCardDataToSerialFlash( CardParameter.u8strParam);
	}

	return 1; //
}

uint8_t DeleteAuthorityFromSerialFlash( uint8_t *strPinData, uint8_t *strCardData, uint8_t nPort)
{
	uint16_t numItem = 0;
	uint8_t nReturn = 0;
	uint16_t rdIndex;
	uint32_t rdSecCnt;

	if( CheckRfCardDataInFlash(strCardData) == Card_Not_Registerd)
	{
#ifdef DEBUG_MODE
		printf( "DeleteAuthorityFromSerialFlash isn't stCardData.\n\r");
#endif
	}
	else   /// 등록 되어 있슴
	{
#ifdef DEBUG_MODE
		printf( "DeleteAuthorityFromSerialFlash Result Card%02X%02X%02X%002X,Pin%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
			CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
			CardParameter.stParam.Pin[0], CardParameter.stParam.Pin[1],CardParameter.stParam.Pin[2],CardParameter.stParam.Pin[3],
			CardParameter.stParam.Pin[4],CardParameter.stParam.Pin[5],CardParameter.stParam.Pin[6],CardParameter.stParam.Pin[7]);
#endif
		CardParameter.stParam.DoorAuth_CardKind &= ~(0x80 >> nPort);  //  All door 1111 권한  || 1:상시 ID.  2:일시 ID.  3:당일카드 ID.

		OverWriteRfCardDataToSerialFlash( CardParameter.u8strParam);
	}

	return 1; //
}

uint8_t GetAuthorityFromSerialFlash( uint8_t *strPinData, uint8_t *strCardData)
{
	uint16_t numItem = 0;
	uint8_t nReturn = 0;
	uint16_t rdIndex;
	uint32_t rdSecCnt;

	if( CheckRfCardDataInFlash(strCardData) == Card_Not_Registerd)
	{
#ifdef DEBUG_MODE
		printf( "GetAuthorityFromSerialFlash isn't stCardData.\n\r");
#endif
	}
	else   /// 등록 되어 있슴
	{
#ifdef DEBUG_MODE
		printf( "GetAuthorityFromSerialFlash Result Card%02X%02X%02X%002X, Atho=%02X\r\n",
			CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
			CardParameter.stParam.DoorAuth_CardKind);
#endif
		nReturn = CardParameter.stParam.DoorAuth_CardKind;  //  All door 1111 권한  || 1:상시 ID.  2:일시 ID.  3:당일카드 ID.
	}

	return nReturn; //
}


