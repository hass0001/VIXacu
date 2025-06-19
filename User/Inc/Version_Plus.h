/*************************************************************************************
**
**	@file		version_model.h
**	@date	2012/07/03
**	
**	버전 데이터는 총 8bytes로 구성되며, 마지막 4bytes는 확장용이다.
**	
**	--------------------------------------------------------------------------------
**	| Version No(1) | Stable No(1) | Development No(2) | Customizing Str1(2) | Customizing Str2 (2) |
**	--------------------------------------------------------------------------------
*/

#ifndef __VERSION_HEADER__
#define __VERSION_HEADER__

#define VS_VERSION	(1)		// ���� (0 ~ 9), 1byte
#define VS_STABLE	(14)		// ���� (0 ~ 99), 1byte
#define VS_DEVELOPMENT  (7)		// ���� (0 ~ 99), 1bytes
#define VS_CUSTOMIZING1	("VX")	//Ư�� �� ������� �ۼ��� �ҽ� �ڵ�� ������ 2 Character�� �ο� �Ѵ�. ��) TECSEN : ("TS"), Ư������ ���ٸ� 0���� �ο� �Ѵ�.
#define VS_CUSTOMIZING2	("KR")		//국가를 나타내거나 필요할 시 사용된다. 예) KOREA : ("KR"), 확장 불 필요 시에는 0으로 부여한다. 

///////////////////// SYSTEM Defines
#define MODEL_NUM       4       //1: ta1000     2: ta2000       4:TA000        8: aru8
#define SFLASH_SIZE     4       //1메가일경우 카드타입 이런건 버리고 출입문권한만 저장함. aru8의 경우 8개의 출입문이기 때문에..
#define NO_ACK_TIME     2000   //100 == 1초 iTA4WaitLimitTimer++ > NO_ACK_TIME)//10000)//12000)  //120sec check
//1.4.2 부터는 연결유지시간 없앰.. -> 1.4.4 에서 다시 살림.. disconnection 안되면 계속 연결안되는 부작용이 있음..

///////////////////// SYSTEM Option...
#define CHAFON_RU6403 0x14  //중국산 900mhz reader의 첫바이트는 패킷의 총길이인데 다들 비슷할 수 있으므로.. 나중에 확인해서 고치는 것이 좋을 듯..
//#define CHAFON_RU5016 //중국산 900mhz reader.. test용으로 산거 안테나 큰거..
//#define ANTI_THEFT_GATE //이랜드 도난방지용.. 도난방지출입문용..
// #define ALLCARD_GRANT_MODE
#define ENABLE_FIRE_ANNOUNCE     //comment 시키면 udp든 manager 에서 들어오는 신호든 무시하고 자기꺼먼 작동시키게 됨..
#define DOOR_OPENED_TOO_LONG_SEC 20
//#define FLASH_SAVE_TEST                 ///특정어드레스 저장여부 확인을 위해 wiegand input을 바꿈..

//강원랜드처럼 485 consol 장치처럼 운용할 때.. 이는 cs-10s, sen70에 응용할 수 있음..
#define RS485ID 0
//#define RS485_CLIENT_MODE     //활성화 되면 PC에서 485로 원격 조종하는 모드로 진행..

#define WIEGAND_INT_DELAY_TIME_US  0    //0~50 그 이상도 가능할 수도..


///////////////////// Global Defines
// #define  DEBUG_MODE
#define  LOG_DEBUG
// #define SFLASH_READ_TEMP  // sFlash 읽기 테스트 위함..

#ifdef   VERSION_GLOBALS
#define  VERSION_EXT
#else
#define  VERSION_EXT   extern
#endif

#if defined (MODEL_NUM) && (MODEL_NUM == 1)          //1: ta1000
#elif defined (MODEL_NUM) && (MODEL_NUM == 2)        //2: ta2000
#elif defined (MODEL_NUM) && (MODEL_NUM == 8)        //8: aru8
#else //#elif defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000
#endif	//MODEL_NUM

VERSION_EXT char fwCompileTime[9];

VERSION_EXT void VS_GetVersionStr(char*);
VERSION_EXT void VS_GetVersionStrFromData(unsigned char*, char*);
VERSION_EXT void VS_GetVersionData(unsigned char*);
VERSION_EXT char VS_CompareVersion(unsigned char* , unsigned char*);

///////////////////// 적용되는데만 번호를 부여한다... 나머지는 모두 0으로..
#define SITE_DEFAULT                    0       
// #define GONGJU_MEDICAL_CENTER           1       //공주의료원 
#define JINJU_MEDICAL_CENTER            0       //진주경상대병원 일단 0557508414  10자리를 받아서 넘기는 것부터 적용.. 73병동 격리실용.. 향후 추가 가능성 있음..
#define YONGIN_DAVOS_HOSPITAL           0       //용인 다보스병원
#define SEVERANCE_HOSPITAL              0       //연세 세브란스병원
#define WONGWANG_SANBON                 0       //원광대학교 산본병원
#define CHEONGJU_HANA                   0       //청주하나병원
#define YONGIN_MYEONGJU_HOSPITAL        0       //용인 명주병원

#endif
