
#define Q74X_QUEUE_GLOBAL

/******************************************************************************
**    Include Files
******************************************************************************/

#include "vixConfig.h"

gStMsgList CsData_0[NUM_CS_RQUEUE_SIZE];
gStMsgList CsData_1[NUM_CS_RQUEUE_SIZE];
gStMsgList CsData_2[NUM_CS_RQUEUE_SIZE];
gStMsgList CsData_3[NUM_CS_RQUEUE_SIZE];

gStRtlogList CsRtlog[NUM_CS_RQUEUE_SIZE];

// ������ ����
void QUEUE_Init_Ring_Queues(void)
{
		uiEnCsMsg_0 = 0;
		uiDeCsEnd_0 = 0;
		uiEnCsMsg_1 = 0;
		uiDeCsEnd_1 = 0;
		uiEnCsMsg_2 = 0;
		uiDeCsEnd_2 = 0;
		uiEnCsMsg_3 = 0;
		uiDeCsEnd_3 = 0;
}

///////////////////////  General Queue
void QUEUE_En_Cs_Queue(uint8_t nPort, gStMsgList* member)
{
	switch(nPort)
	{
	case 0:
		CsData_0[uiEnCsMsg_0].CsLenth = member->CsLenth;
		memcpy( CsData_0[uiEnCsMsg_0].CsMsges, member->CsMsges, NUM_CS_INDEX_DATA);

		uiEnCsMsg_0++;
		if( uiEnCsMsg_0 >= NUM_CS_RQUEUE_SIZE) {
			uiEnCsMsg_0  = 0;
		}
		break;
	case 1:
		CsData_1[uiEnCsMsg_1].CsLenth = member->CsLenth;
		memcpy( CsData_1[uiEnCsMsg_1].CsMsges, member->CsMsges, NUM_CS_INDEX_DATA);

		uiEnCsMsg_1++;
		if( uiEnCsMsg_1 >= NUM_CS_RQUEUE_SIZE) {
			uiEnCsMsg_1  = 0;
		}
		break;
	case 2:
		CsData_2[uiEnCsMsg_2].CsLenth = member->CsLenth;
		memcpy( CsData_2[uiEnCsMsg_2].CsMsges, member->CsMsges, NUM_CS_INDEX_DATA);

		uiEnCsMsg_2++;
		if( uiEnCsMsg_2 >= NUM_CS_RQUEUE_SIZE) {
			uiEnCsMsg_2  = 0;
		}
		break;
	case 3:
		CsData_3[uiEnCsMsg_3].CsLenth = member->CsLenth;
		memcpy( CsData_3[uiEnCsMsg_3].CsMsges, member->CsMsges, NUM_CS_INDEX_DATA);

		uiEnCsMsg_3++;
		if( uiEnCsMsg_3 >= NUM_CS_RQUEUE_SIZE) {
			uiEnCsMsg_3  = 0;
		}
		break;
	default:
		break;
	}
}

gStMsgList* QUEUE_De_Cs_Queue(uint8_t nPort)
{

	gStMsgList *cs_list = NULL;

	switch(nPort)
	{
	case 0:
		if ( uiDeCsEnd_0 == uiEnCsMsg_0 ) {
			return NULL;
		}
		else {
			cs_list = (gStMsgList *)&CsData_0[uiDeCsEnd_0];
			uiDeCsEnd_0++;
			if( uiDeCsEnd_0 >= NUM_CS_RQUEUE_SIZE) {
				uiDeCsEnd_0 = 0;
			}
		}
		break;
	case 1:
		if ( uiDeCsEnd_1 == uiEnCsMsg_1 ) {
			return NULL;
		}
		else {
			cs_list = (gStMsgList *)&CsData_1[uiDeCsEnd_1];
			uiDeCsEnd_1++;
			if( uiDeCsEnd_1 >= NUM_CS_RQUEUE_SIZE) {
				uiDeCsEnd_1 = 0;
			}
		}
		break;
	case 2:
		if ( uiDeCsEnd_2 == uiEnCsMsg_2 ) {
			return NULL;
		}
		else {
			cs_list = (gStMsgList *)&CsData_2[uiDeCsEnd_2];
			uiDeCsEnd_2++;
			if( uiDeCsEnd_2 >= NUM_CS_RQUEUE_SIZE) {
				uiDeCsEnd_2 = 0;
			}
		}
		break;
	case 3:
		if ( uiDeCsEnd_3 == uiEnCsMsg_3 ) {
			return NULL;
		}
		else {
			cs_list = (gStMsgList *)&CsData_3[uiDeCsEnd_3];
			uiDeCsEnd_3++;
			if( uiDeCsEnd_3 >= NUM_CS_RQUEUE_SIZE) {
				uiDeCsEnd_3 = 0;
			}
		}
		break;
	default:
		break;
	}
    return cs_list;
}

void QUEUE_Cls_Cs_Queue(uint8_t nPort)
{
	switch(nPort)
	{
	case 0:
		uiEnCsMsg_0 = 0;
		uiDeCsEnd_0 = 0;
		break;
	case 1:
		uiEnCsMsg_1 = 0;
		uiDeCsEnd_1 = 0;
		break;
	case 2:
		uiEnCsMsg_2 = 0;
		uiDeCsEnd_2 = 0;
		break;
	case 3:
		uiEnCsMsg_3 = 0;
		uiDeCsEnd_3 = 0;
		break;
	default:
		break;
	}
}

int QUEUE_Cnt_Cs_Queue(uint8_t nPort)
{
    int i=0;

	switch(nPort)
	{
	case 0:
		if( uiEnCsMsg_0 > uiDeCsEnd_0)
			i = uiEnCsMsg_0 - uiDeCsEnd_0;
		else if( uiEnCsMsg_0 == uiDeCsEnd_0)
			i = 0;
		else
			i = (uiEnCsMsg_0+NUM_CS_RQUEUE_SIZE) - uiDeCsEnd_0;
		break;
	case 1:
		if( uiEnCsMsg_1 > uiDeCsEnd_1)
			i = uiEnCsMsg_1 - uiDeCsEnd_1;
		else if( uiEnCsMsg_1 == uiDeCsEnd_1)
			i = 0;
		else
			i = (uiEnCsMsg_1+NUM_CS_RQUEUE_SIZE) - uiDeCsEnd_1;
		break;
	case 2:
		if( uiEnCsMsg_2 > uiDeCsEnd_2)
			i = uiEnCsMsg_2 - uiDeCsEnd_2;
		else if( uiEnCsMsg_2 == uiDeCsEnd_2)
			i = 0;
		else
			i = (uiEnCsMsg_2+NUM_CS_RQUEUE_SIZE) - uiDeCsEnd_2;
		break;
	case 3:
		if( uiEnCsMsg_3 > uiDeCsEnd_3)
			i = uiEnCsMsg_3 - uiDeCsEnd_3;
		else if( uiEnCsMsg_3 == uiDeCsEnd_3)
			i = 0;
		else
			i = (uiEnCsMsg_3+NUM_CS_RQUEUE_SIZE) - uiDeCsEnd_3;
		break;
	default:
		break;
	}
    return i;
}



///////////////////////  Rtlog Queue
void QUEUE_En_Rt_Queue( gStRtlogList* member)
{
  CsRtlog[uiEnCsRtlog].CsPort = member->CsPort;
  memcpy( CsRtlog[uiEnCsRtlog].CsMsges, member->CsMsges, NUM_CS_INDEX_DATA);

  uiEnCsRtlog++;
    if( uiEnCsRtlog >= NUM_CS_RQUEUE_SIZE) {
    	uiEnCsRtlog = 0;
    }
}

gStRtlogList* QUEUE_De_Rt_Queue(void)
{
	gStRtlogList *cs_list = NULL;

    if ( uiDeCsRtlog == uiEnCsRtlog ) {
        return NULL;
    }
    else {
        cs_list = (gStRtlogList *)&CsRtlog[uiDeCsRtlog];
        uiDeCsRtlog++;
        if( uiDeCsRtlog >= NUM_CS_RQUEUE_SIZE) {
        	uiDeCsRtlog = 0;
        }
    }
    return cs_list;
}

void QUEUE_Cls_Rt_Queue(void)
{
	uiDeCsRtlog = 0;
	uiEnCsRtlog = 0;
}

int QUEUE_Cnt_Rt_Queue(void)
{
    int i=0;
    if( uiEnCsRtlog > uiDeCsRtlog)
        i = uiEnCsRtlog - uiDeCsRtlog;
    else if( uiEnCsRtlog == uiDeCsRtlog)
        i = 0;
    else
        i = (uiEnCsRtlog+NUM_CS_RQUEUE_SIZE) - uiDeCsRtlog;
    return i;
}

