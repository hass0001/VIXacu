
#ifndef _CS74X_QUEUE_H_
#define _CS74X_QUEUE_H_

/* Includes ------------------------------------------------------------------*/
/******************************************************************************
**    Include Files
******************************************************************************/
#include "httpClient.h"
/******************************************************************************
**    Define
******************************************************************************/
#ifdef   Q74X_QUEUE_GLOBAL
#define  QUEUE_EXT
#else
#define  QUEUE_EXT   extern
#endif

#define NUM_CS_INDEX_DATA   25
#define NUM_CS_RQUEUE_SIZE   20

// gStMsgList 구조체 만들기.
typedef struct CsMsgList {
  uint8_t CsLenth;
  uint8_t CsMsges[NUM_CS_INDEX_DATA];
} __attribute__((packed)) gStMsgList;

// gStRtlogList 구조체 만들기.
typedef struct gStRtlogList {
  uint8_t CsPort;
  uint8_t CsMsges[NUM_CS_INDEX_DATA];
} __attribute__((packed)) gStRtlogList;

QUEUE_EXT uint8_t   uiEnCsMsg_0;
QUEUE_EXT uint8_t   uiDeCsEnd_0;

QUEUE_EXT uint8_t   uiEnCsMsg_1;
QUEUE_EXT uint8_t   uiDeCsEnd_1;

QUEUE_EXT uint8_t   uiEnCsMsg_2;
QUEUE_EXT uint8_t   uiDeCsEnd_2;

QUEUE_EXT uint8_t   uiEnCsMsg_3;
QUEUE_EXT uint8_t   uiDeCsEnd_3;

QUEUE_EXT uint8_t   uiEnCsRtlog;
QUEUE_EXT uint8_t   uiDeCsRtlog;

/////////////  Global variables. /////////////
QUEUE_EXT void QUEUE_Init_Ring_Queues(void);

QUEUE_EXT void QUEUE_En_Cs_Queue(uint8_t nPort, gStMsgList* member);
QUEUE_EXT gStMsgList* QUEUE_De_Cs_Queue(uint8_t nPort);
QUEUE_EXT void QUEUE_Cls_Cs_Queue(uint8_t nPort);
QUEUE_EXT int QUEUE_Cnt_Cs_Queue(uint8_t nPort);

QUEUE_EXT void QUEUE_En_Rt_Queue(gStRtlogList* member);
QUEUE_EXT gStRtlogList* QUEUE_De_Rt_Queue(void);
QUEUE_EXT void QUEUE_Cls_Rt_Queue(void);
QUEUE_EXT int QUEUE_Cnt_Rt_Queue(void);

#endif // _CS74X_UART_H_
