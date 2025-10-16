/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: WiegandTask.c
**   Project: TA18
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#define    WIEGAND_GLOBALS
#include   "WiegandTask.h"

/******************************************************************************
**  WIEGAND TASK
******************************************************************************/
// void WiegandTask(void *pdata)
#define KT_SUPRIMA_CARD

void WiegandProcess(void)  {
//	while(1)  {
    Check_Wiegand_Input();
    TaskRunCnt(ENUM_WIEGAND_TASK);          // Notice Activated.  20150210 Jylee
//  }
}

void Wiegand_reset_port(uint8_t nPort, uint8_t b34Type) {

  gU64WiegandValue1[nPort] = 0;
  gU64WiegandValue2[nPort] = 0;
  
  gUcWiegBitCnt[nPort] = 0;
  gUcPrevBitCnt[nPort] = 0;
  gUcSetValue[nPort] = 0;
  if( b34Type <= 2)  //0: Wiegand 26BIT  1: Wiegand 26BIT MOTO Password  2: Wiegand 26BIT HID Password  
    gUcWiegandType[nPort] = 25;
  else if (b34Type == 3)  //3 : Wiegand 34BIT
    gUcWiegandType[nPort] = 33;
  else if (b34Type == 4)  //4 : Wiegand 64BIT
    gUcWiegandType[nPort] = 65;
  else if (b34Type == 5)  //5 : Wiegand 128BIT.
    gUcWiegandType[nPort] = 129;
  else //3 : Wiegand 34BIT.. default..
    gUcWiegandType[nPort] = 33;

#ifdef HIDWIEGAND37FORMAT
  gUcWiegandType[nPort] = 36;//임시로 26받기위해..
#endif

  memset(WiegandCard[nPort], 0, MAX_BYTE_CARD); //4
}

void Wiegand_init_value(void)
{
     uint8_t nWieg;//, nCardf;
     for (nWieg = 0; nWieg < MAX_WIEGAND_PORT; nWieg++) {
    	 WiegandParameter.CommType[nWieg] = 4;
       Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//&0x7f ? 최상위비트는 순/역방향..
     }
}

     
void Check_Wiegand_Input(void)
{
  uint8_t nWieg;
  uint16_t u16CardResult;
  
  unionU64U8 union64WiegandValue;
  unionU32U8 union32WiegandValue;
  uint8_t u8AsciiString[16];
  
  for (nWieg = 0; nWieg < MAX_WIEGAND_PORT; nWieg++)
  {
    if( gUcSetValue[nWieg] == 1)
    {
        _RS485_LED_ON();
//      if(gU32WiegandValue[nWieg] == 0xffffffff){//0xffffffff 가 들어오면 다음 채널로 넘긴다..
      if((gUcWiegandType[nWieg] == 33  && gU64WiegandValue1[nWieg] == 0xffffffff) || //0xffffffff 가 들어오면 다음 채널로 넘긴다..
         (gUcWiegandType[nWieg] == 65  && gU64WiegandValue1[nWieg] == 0xffffffffffffffff) || 
         (gUcWiegandType[nWieg] == 129 && gU64WiegandValue1[nWieg] == 0xffffffffffffffff &&
        		                          gU64WiegandValue2[nWieg] == 0xffffffffffffffff))
      {
        Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//초기화 시키고..
        continue;//다음 채널로..
      }
      
      printf( "Wiegand type %d bit count %d, value %08X!\r\n",
    		  gUcWiegandType[nWieg], gUcWiegBitCnt[nWieg],  gU64WiegandValue1[nWieg]);
      if(gUcWiegandType[nWieg] == 33)
      {//W34일 경우는
        union32WiegandValue.u32Value = (uint32_t) (gU64WiegandValue1[nWieg] & 0xffffffff);      // 그냥 해도 되지만..
      }
      else if(gUcWiegandType[nWieg] == 65)
      {//여기서부터 바코드 번호키 등을 W66 으로 받아서 처리함..
        union64WiegandValue.u64Value = gU64WiegandValue1[nWieg];  //첫번째 8바이트
        for(int i = 0; i < 8; i++)
        {
          u8AsciiString[i] = union64WiegandValue.u8String[8 - i];
        }
#ifdef  GONGJU_MEDICAL_CENTER
        union32WiegandValue.u32Value = funcConverAsciiArr2U32(u8AsciiString, 8);        //변환한 값은 union32WiegandValue.u32Value 로..
#else
        union32WiegandValue.u32Value = (uint32_t) (gU64WiegandValue1[nWieg] & 0xffffffff);      // 그냥 해도 되지만..
#endif
      }
      else if(gUcWiegandType[nWieg] == 129)
      {//여기서부터 바코드 번호키 등을 W130 으로 받아서 처리함..
        union64WiegandValue.u64Value = gU64WiegandValue1[nWieg];  //첫번째 8바이트
        for(int i = 0; i < 8; i++)
        {
          u8AsciiString[i] = union64WiegandValue.u8String[7 - i];//little endian은 역순으로..
        }
        union64WiegandValue.u64Value = gU64WiegandValue2[nWieg];  //두번째 8바이트
        for(int i = 8; i < 16; i++)
        {
          u8AsciiString[i] = union64WiegandValue.u8String[15 - i];//little endian은 역순으로..
        }
        
#ifdef  GONGJU_MEDICAL_CENTER
        union32WiegandValue.u32Value = funcConverAsciiArr2U32(u8AsciiString, 16);        //변환한 값은 union32WiegandValue.u32Value 로..
#else
        union32WiegandValue.u32Value = (uint32_t) (gU64WiegandValue1[nWieg] & 0xffffffff);      // 그냥 해도 되지만..
#endif
      }
      else
      {     //if(gUcWiegandType[nWieg] == 25) {//W26일 경우는
        union32WiegandValue.u32Value = (uint32_t) (gU64WiegandValue1[nWieg] & 0x00ffffff);      // 그냥 해도 되지만..
      }

//      if (union32WiegandValue.u32Value == 0) continue; // 0은 처리하지 않음..
      if (union32WiegandValue.u32Value == 0)
      {//이게 맞을 듯..
        Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//최상위비트는 순/역방향..
        continue; // 0은 처리하지 않음..
      }


#ifdef FLASH_SAVE_TEST
      u8AsciiString[0] = 0x12;
      u8AsciiString[1] = 0x7C;
      u8AsciiString[2] = 0x00;
      u8AsciiString[3] = 0x49;
#else
      if(WiegandParameter.CommType[nWieg]&0x80)
      {  //&0x80 최상위비트는 순/역방향..
        for(int i = 0; i < 4; i++)
        	u8AsciiString[i] = union32WiegandValue.u8String[i];
      }
      else
      {  //순방향..
        for(int i = 0; i < 4; i++) //little endian 은 숫자는 거꾸로 되어 있음..
        	u8AsciiString[i] = union32WiegandValue.u8String[3 - i]; // 3,2,1,0
      }
#endif

#ifdef CARD_2301_ORDER
      WiegandCard[nWieg][0] = u8AsciiString[2];
      WiegandCard[nWieg][1] = u8AsciiString[3];
      WiegandCard[nWieg][2] = u8AsciiString[0];
      WiegandCard[nWieg][3] = u8AsciiString[1];
#else  // CARD 0123 순서
      WiegandCard[nWieg][0] = u8AsciiString[0];
      WiegandCard[nWieg][1] = u8AsciiString[1];
      WiegandCard[nWieg][2] = u8AsciiString[2];
      WiegandCard[nWieg][3] = u8AsciiString[3];
#endif

      u16CardResult = CheckRfCardDataInFlash( WiegandCard[nWieg], 1); //카드데이터 검색!!
      CheckEventCardAndLog( nWieg, WiegandCard[nWieg], u16CardResult);

 /*     printf( "Wiegand %d value=%x, WiegandCard=%02x%02x%02x%02x%02x%02x%02x%02x\r\n",nWieg, gU64WiegandValue1[nWieg],
      WiegandCard[nWieg][0], WiegandCard[nWieg][1], WiegandCard[nWieg][2], WiegandCard[nWieg][3],
      WiegandCard[nWieg][4], WiegandCard[nWieg][5], WiegandCard[nWieg][6], WiegandCard[nWieg][7] );*/
      Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//최상위비트는 순/역방향..
      _RS485_LED_OFF();

#ifdef DEBUG_MODE
		uint8_t tltBuffer[7];
		RTC_GetTimeAndDate( &toDayTime, tltBuffer);
		printf( "억세스 시간 time=20%02d-%02d-%02d %02d:%02d:%02d\r\n",
			tltBuffer[0], tltBuffer[1], tltBuffer[2], tltBuffer[3], tltBuffer[4], tltBuffer[5]);
#endif

    }
    else
    {
      if( gUcWiegBitCnt[nWieg] > 0)
      {
        if( gUcPrevBitCnt[nWieg] > MAX_TIME_BYTE_DURATION)
        {  // 100 ms
#ifdef KT_SUPRIMA_CARD
           if( gUcWiegBitCnt[nWieg] >= 33)
           {
        	   gUcSetValue[nWieg] = 1;
        	   gU64WiegandValue1[nWieg] /= 2;
           }
           else
           {
               Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//최상위비트는 순/역방향..
           }
#else
           Wiegand_reset_port(nWieg, WiegandParameter.CommType[nWieg]&0x7f);//최상위비트는 순/역방향..
#endif
//           printf( "Wiegand %d bit count %x, Canceled for timeout!\r\n",nWieg, gUcWiegBitCnt[nWieg] );
        }
        else
        {
            gUcPrevBitCnt[nWieg]++;  // 50번 이상 읽을 때까지 매번 증가 시킴.
        }
#ifdef SENMINI_WIEGAND_DELAY   // 센미니 위앤드는 느려서 일ㄱ는데 딜레이가 필요
        printf( "Wiegand %d bit count %x Duration %x!\r\n",nWieg, gUcWiegBitCnt[nWieg], gUcPrevBitCnt[nWieg] );
#endif

      }
    }
  }
}

uint32_t funcConverAsciiArr2U32(uint8_t* gU8TargetData, int nLength)
{
  uint32_t u32ReturnValue = 0;
  uint8_t  u8BufferData[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}, gU8TargetDataCount = 0;

  for (int i = 0; i < nLength; i++)   {
    if ((gU8TargetData[i] >= '0' && gU8TargetData[i] <= '9') ||
        (gU8TargetData[i] >= 'A' && gU8TargetData[i] <= 'Z') ||
        (gU8TargetData[i] >= 'a' && gU8TargetData[i] <= 'z') )
    {//알파벳 & 숫자이면..
            gU8TargetDataCount++;
    }
    else
    {
      break;
    }
  }

  if (gU8TargetDataCount < 1) //4) //1바이트 미만으로 바꿈     //4바이트 미만은 0으로 던져버림..
    return 0;

  if (gU8InstallSiteNumber == 1) {   //1       //공주의료원
    if((gU8TargetData[0] == 'E')&&(gU8TargetDataCount == 9)){//공주의료원 직원바코드(E8) // 바코드 leuart "E12345678\n\r" 0xE0BC614E
      memcpy(u8BufferData, gU8TargetData + 1, 8);         //바코드 숫자만 복사.. //8바이트를 옮겨야 하므로..
      u32ReturnValue = 0xE0000000;       //직원과 환자가 충돌할 수도 있으니.. 9999 9999 == 0x5F5 E0FF 이므로 9999 9999 는 0xE5F5 E0FF 로 변환됨..
      memcpy(gU8TargetData, u8BufferData, 8);
      gU8TargetDataCount = 8;   //앞에서부터 8자리는 일반 바코드이므로 일반바코드 루틴으로 넘어가게 함..
    }
    else if ((gU8TargetData[0] == 'G')&&(gU8TargetDataCount == 10)) {//공주의료원 보호자 : 발급차수1~8(G5) + 환자번호8자리 "G500123456\n\r"  0xFC01E240
      if((gU8TargetData[1] < '1') || (gU8TargetData[1] > '8')){  //1~8 인지 확인
  //              memset(gU8TargetData, 0, gU8TargetDataCount);
  //              gU8TargetDataCount = 0;
        return 0;
      }
      else {
        memcpy(u8BufferData, gU8TargetData + 2, 8);       //바코드 숫자만 복사..
        u32ReturnValue = (gU8TargetData[1] - '0')*0x1000000 + 0xF7000000;       //보호자 += F700 0000 + (발급번호 1~8)*100 0000 => f800 0000 ~ ff00 0000
        memcpy(gU8TargetData, u8BufferData, 8);
        gU8TargetDataCount = 8;   //앞에서부터 8자리는 일반 바코드이므로 일반바코드 루틴으로 넘어가게 함..
      }
    }
    else if ((gU8TargetData[0] == 'V')&&(gU8TargetDataCount == 11)) {//공주의료원 방문자+방문일(V10).. "V1012345678\n\r"  0x1ABC614E
      if((gU8TargetData[1] < '0') || (gU8TargetData[1] > '3')||(gU8TargetData[2] < '0') || (gU8TargetData[2] > '9')){  //01~31 인지 확인.. 물론 01~39 이긴 하지만.. 상관은 없음..
        return 0;
      }
      else {
        memcpy(u8BufferData, gU8TargetData + 3, 8);       //바코드 숫자만 복사..
        u32ReturnValue = ((gU8TargetData[1] - '0')*10+(gU8TargetData[2] - '0'))*0x1000000 + 0x10000000;      // 방문객 += (발급dd)*100 0000 + 1000 0000
        memcpy(gU8TargetData, u8BufferData, 8);
        gU8TargetDataCount = 8;   //앞에서부터 8자리는 일반 바코드이므로 일반바코드 루틴으로 넘어가게 함..
      }
    }
    //일반환자 : "123456" 0x0001E240
  }     //GONGJU_MEDICAL_CENTER
  else {
  }


  for (int i = 0; i < gU8TargetDataCount; i++){//위에서 정리된 값을 대상으로 ASCII 숫자 Array인 코드값을 int 로 바꿈..
    if (gU8TargetData[i] == '0') {  // 0 이면 통과..
      continue;
    }
    else if ((gU8TargetData[i] < '0') || (gU8TargetData[i] > '9')){  //숫자인지 확인한다.
      return 0;//u8ErrorCheckCount++;
    }
    else {
 //     u32ReturnValue += (gU8TargetData[i]-'0')*(pow(10, gU8TargetDataCount-i-1));      //10의 0~7제곱까지 곱해서 더함..
    }
  }//end for

  return u32ReturnValue;
}

void WiegandSetInputData( uint8_t nPort, uint8_t bReset)
{
//	printf("[%d]%d,", gUcWiegBitCnt[nPort], bReset);

    if( bReset == BIT_RESET )
    { // D0 low
        if( gUcWiegBitCnt[nPort] == 0) {
            __NOP();   // parity
        }
        else if( gUcWiegBitCnt[nPort] == gUcWiegandType[nPort]) {
            gUcSetValue[nPort] = 1;
        }
        else {
          if(gUcWiegBitCnt[nPort] <= 64){//64비트까지는..
            gU64WiegandValue1[nPort] *= 2;
          }
          else {
            gU64WiegandValue2[nPort] *= 2;
          }
//                    gUcWiegandValue[nPort] *= 2;
        }
    }
    else
    {  // D1 low
        if( gUcWiegBitCnt[nPort] == 0)
        {
          __NOP();     // parity
        }
        else if( gUcWiegBitCnt[nPort] == gUcWiegandType[nPort])
        {
            gUcSetValue[nPort] = 1;
        }
        else {
          if(gUcWiegBitCnt[nPort] <= 64)
          {//64비트까지는..
            gU64WiegandValue1[nPort] *= 2;
            gU64WiegandValue1[nPort]++;
          }
          else
          {
            gU64WiegandValue2[nPort] *= 2;
            gU64WiegandValue2[nPort]++;
          }
//                    gUcWiegandValue[nPort] *= 2;
//                    gUcWiegandValue[nPort] += 1;
        }
    }
    gUcWiegBitCnt[nPort]++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
// OS_EnterNestableInterrupt();      // should Interrupt Disable
    if(GPIO_Pin == GPIO_PIN_0){ // //  KEY_SET_D0
    }
    else if(GPIO_Pin == GPIO_PIN_1){ //KEY_CLEAR_D1
    }
    else if(GPIO_Pin == GPIO_PIN_2){ //W1D0
        if(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)){
//        	printf("\r\nW1D0 LOW");
           	WiegandSetInputData(0, BIT_RESET);   // D0
        }
    }
    else if(GPIO_Pin == GPIO_PIN_3){ //W3D1 -> W4D1
         if(!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)){
           	WiegandSetInputData(3, BIT_SET);   // D1
//           	printf("\r\nW4D1 LOW");
        }
    }
    else if(GPIO_Pin == GPIO_PIN_6){ //W2D0
        if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)){
           	WiegandSetInputData(1, BIT_RESET);   // D0
 //          	printf("\r\nW2D0 LOW");
        }
    }
    else if(GPIO_Pin == GPIO_PIN_7){ //W1D1
        if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)){
           	WiegandSetInputData( 0, BIT_SET);   // D1
//           	printf("\r\nW1D1 LOW");
        }
    }
    else if(GPIO_Pin == GPIO_PIN_8){ //W4D0 -> W3D0
        if(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)){
           	WiegandSetInputData( 2, BIT_RESET);   // D0
 //          	printf("\r\nW3D0 LOW");
        }
    }
    else if(GPIO_Pin == GPIO_PIN_11){ //W3D0 -> W4D0
        if(!HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11)){
           	WiegandSetInputData( 3, BIT_RESET);   // D0
 //          	printf("\r\nW4D0 LOW");
         }
    }
    else if(GPIO_Pin == GPIO_PIN_12){ //W4D1 -> W3D1
        if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)){
           	WiegandSetInputData( 2, BIT_SET);   // D1
 //         	printf("\r\nW3D1 low");
         }
    }
    else if(GPIO_Pin == GPIO_PIN_15){ //W2D1
         if(!HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_15)){
           	WiegandSetInputData( 1, BIT_SET);   // D1
  //         	printf("\r\nW2D1 LOW");
         }
    }
//    OS_LeaveNestableInterrupt();      // should Interrupt Enable
}
