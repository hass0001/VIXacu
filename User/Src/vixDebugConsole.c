/*
 * vixConsole.c
 *
 *  Created on: Oct 2, 2024
 *      Author: intellivix
 */

/*#include <stdio.h>
#include <stdarg.h>
#include "usart.h"
#include "vixDebugConsole.h"
#include "cmsis_os2.h"*/

#include "vixConfig.h"


//static OS_MUTEX					OS_DPRINTF_MUTEX;

#define _UART_CONSOLE_HANDLE				huart4

#ifdef __GNUC__		// CubeIDE define __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif


GETCHAR_PROTOTYPE
{
	uint8_t ch=0;
	__HAL_UART_CLEAR_OREFLAG(&_UART_CONSOLE_HANDLE);
	HAL_UART_Receive(&_UART_CONSOLE_HANDLE, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	//HAL_UART_Transmit(&huart6, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&_UART_CONSOLE_HANDLE, (uint8_t*)&ch, 1, 500);
	return ch;
}

int _write(int file, char* ptr, int len)
{
	HAL_UART_Transmit(&_UART_CONSOLE_HANDLE, (uint8_t*)ptr, len, 500);
	return len;
}

/*
@Name: Dprintf
@Brief: Uart로 debug 메시지 출력 함수
@Param: 
@Return: None
@Note: 현재 printf로 디버그 메시지를 출력하고 있으나 Dprintf로 변경하는 작업 필요
@Note: Dprintf 함수 안에 조건 컴파일 처리하여 디버그 메시지 출력하지 않도록 할 수 있음 Ex) #define _DEBUG_MODE
@Note: 디버그 메시지 printing에 대한 Mutex나 Semaphore 관리가 용이함
@Note: float 소수점 출력이 필요한 경우 함수를 추가 개선해야 함
*/


void OPEN_DebugConsole(void)
{
	 nFwdPacketIndex = 0;
	 nFwdByteToReceive = 0;
	 __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
     FwdTx1Enable = 0;
	return;
}




