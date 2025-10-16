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
#define VS_STABLE	(15)		// ���� (0 ~ 99), 1byte
#define VS_DEVELOPMENT  (31)	// ���� (0 ~ 99), 1bytes
#define VS_CUSTOMIZING1	("VX")	//Ư�� �� ������� �ۼ��� �ҽ� �ڵ�� ������ 2 Character�� �ο� �Ѵ�. ��) TECSEN : ("TS"), Ư������ ���ٸ� 0���� �ο� �Ѵ�.
#define VS_CUSTOMIZING2	("KR")	//국가를 나타내거나 필요할 시 사용된다. 예) KOREA : ("KR"), 확장 불 필요 시에는 0으로 부여한다.

///////////////////// SYSTEM Defines
#define MODEL_NUM       4       //1: ta1000     2: ta2000       4:TA000        8: aru8
#define SFLASH_SIZE     4       //1메가일경우 카드타입 이런건 버리고 출입문권한만 저장함. aru8의 경우 8개의 출입문이기 때문에..
#define NO_ACK_TIME     2000   //100 == 1초 iTA4WaitLimitTimer++ > NO_ACK_TIME)//10000)//12000)  //120sec check
// 1.4.2 부터는 연결유지시간 없앰.. -> 1.4.4 에서 다시 살림.. disconnection 안되면 계속 연결안되는 부작용이 있음..
// 1.15.11   -> 2025.08.07   KT 스케쥴 점검
// 1.15.12   -> 2025.08.11   KT 스케쥴 점검을 위한 TEST Exflash 제거
// 1.15.13   -> 2025.08.11   VIXpass 동기화 지속 위한 커맨드 ID 널 삽입
// 1.15.14   -> 2025.08.12   서버 연결이 안되면 While 문에서 멈추는 버그 수정
// 1.15.15   -> 2025.08.17   화재 신호 연속 입력으로 신호 무시 입력
// 1.15.17   -> 2025.08.19   모바일 카드 번호, KT 카드 번호 지원으로 넘버 0,1 -> 2,3 으로
// 1.15.18   -> 2025.08.20   KT 카드 번호 목적층 표시를 위해 All 카드 읽기 조건으로
// 1.15.19   -> 2025.08.21   새로운 카드일때 권한 분류, 이전은 F 권한
// 1.15.20   -> 2025.08.23   초기화 후 전체 릴리즈 버젼
// 1.15.21   -> 2025.08.23   rtLog 응답 신속히 Rtlog tns
// 1.15.22   -> 2025.08.25   1층 안되는포트 교체로 실행 안되는 프로그램 비교를 위해 HAL_init  누락 되어있었음
// 1.15.23   -> 2025.08.30   스피드 게이트 목적층제어 빠르게 한 것
// 1.15.24   -> 2025.09.02   등록 안된 사용자 하드코딩으로 인증
// 1.15.25   -> 2025.09.04   화재 관련 플래그 ENABLE_FIRE_ANNOUNCE 정의 >> UDP TXP 명령 수용
// 1.15.27   -> 2025.09.14   확장영역 부분 카드 데이터 저장 오류 수정 16마이트 쓰기->18바이트 쓰기
// 1.15.28   -> 2025.09.23   동기화 누락 과 화재 연동 점검, 전체 열림 닫힘, 추가
// 1.15.30   -> 2025.09.23   구의 B1 누락 추가

///////////////////// SYSTEM Option...
//#define CHAFON_RU6403 0x14  //중국산 900mhz reader의 첫바이트는 패킷의 총길이인데 다들 비슷할 수 있으므로.. 나중에 확인해서 고치는 것이 좋을 듯..
//#define CHAFON_RU5016 //중국산 900mhz reader.. test용으로 산거 안테나 큰거..
#define ANTI_THEFT_GATE //이랜드 도난방지용.. 도난방지출입문용..

//#define ALLCARD_GRANT_MODE
// #define ENABLE_FIRE_ANNOUNCE     //comment 시키면 udp든 manager 에서 들어오는 신호든 무시하고 자기꺼먼 작동시키게 됨..
#define DOOR_OPENED_TOO_LONG_SEC 20
// #define FLASH_SAVE_TEST                 ///특정어드레스 저장여부 확인을 위해 wiegand input을 바꿈..

//강원랜드처럼 485 consol 장치처럼 운용할 때.. 이는 cs-10s, sen70에 응용할 수 있음..
#define RS485ID 0
#define RS485_CLIENT_MODE     //활성화 되면 PC에서 485로 원격 조종하는 모드로 진행..

#define WIEGAND_INT_DELAY_TIME_US  0    //0~50 그 이상도 가능할 수도..

///////////////////// Global Defines
//#define  DEBUG_MODE

//#define  LOG_DEBUG
//#define  SYNC_DEBUG

//
// #define SFLASH_READ_TEMP  // sFlash 읽기 테스트 위함..
///////////////////// 적용되는데만 번호를 부여한다... 나머지는 모두 0으로..
//

//#define GUUI_PULLMAN_485		1       //구의 풀만
#define KTWEST_WIEGAND		1       //광화문 KTwest

#ifdef GUUI_PULLMAN_485
#define GUUI_AKT_MEMBER
#define GUUI_PORT_GRANTED
#endif

#ifdef KTWEST_WIEGAND
#define CARD_2301_ORDER			1		//	광화문 KTwest 모바일 카드 번호 앞의 일련이 같아 뒤로 보냄#define GRVERNMENT_MEMBER   // 정부 기관 카드 수동 인증
#define KT_WEST_MEMBER	// KT 카드 수동 인증
#define GRVERNMENT_MEMBER  // 정부기관 맴버
#endif

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

#endif
