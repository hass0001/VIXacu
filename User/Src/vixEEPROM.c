/*
 * vixI2cDriver.c
 *
 *  Created on: Oct 8, 2024
 *      Author: intellivix
 */

#include "vixConfig.h"



//#define _I2C1_HANDLE						hi2c1
#define _EEPROM_I2C_HANDLE				hi2c1


//static OS_SEMAPHORE OS_I2C_SEMAPHORE;
//static OS_MUTEX	OS_EEPROM_MUTEX;

//static OS_STACKPTR				int OS_STACK_SIZE_TEST_EEPROM_TASK[512];			// Task Stack Size
//static OS_TASK					OS_CTRL_BLOCK_TEST_EEPROM_TASK;					// Task Control Block



/*
@Name: SEARCH_I2cSlavesAll
@Brief: i2c ������ ��� device �˻� (0-N����)
@Param: None
@Return: None
@Note: 
*/
void SEARCH_I2cSlavesAll(void)
{
	uint8_t u8DevAddr;

	for(u8DevAddr=0; u8DevAddr < 128; u8DevAddr++)
	//for(u8DevAddr=0; u8DevAddr < 254; u8DevAddr++)
	{
		if(HAL_I2C_IsDeviceReady(&_EEPROM_I2C_HANDLE, (u8DevAddr << 1), _TRIALS_3, _10MS) == HAL_OK)
		{
			printf("i2c device addr:[0x%02x]\r\n", (u8DevAddr << 1));
		}
	}

	return;
}

void I2CWPEnable(void)
{
	HAL_GPIO_WritePin(EEPROM_WP_D6_GPIO_Port, EEPROM_WP_D6_Pin, GPIO_PIN_SET);
}

void I2CWPDisable(void)
{
	HAL_GPIO_WritePin(EEPROM_WP_D6_GPIO_Port, EEPROM_WP_D6_Pin, GPIO_PIN_RESET);
}

/*
@Name: RECOVER_I2cError
@Brief: i2c ���� �߻� �� Slave ��Ŷ��� ���� ����ó�� ����
@Param: i2c HAL handler
@Return: None
@Note: 
*/
void RECOVER_I2cError(I2C_HandleTypeDef *hi2c)
{
	uint8_t u8ToggleCnt;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_I2C_DeInit(hi2c);
	//osDelay(5);
	//OS_Delay(2);
	MyMsDelay(2);

	GPIO_InitStruct.Pin = I2C1_SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(I2C1_SCL_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = I2C1_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(I2C1_SDA_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);
	//osDelay(1);
	//OS_Delay(1);
	MyMsDelay(1);

	for(u8ToggleCnt=0; u8ToggleCnt < 9; u8ToggleCnt++)
	{
		HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);
		//osDelay(1);
		//OS_Delay(1);
		MyMsDelay(1);
		HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);
		//osDelay(1);
		//OS_Delay(1);
		MyMsDelay(1);
	}

	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_RESET);
	//osDelay(1);
	//OS_Delay(1);
	MyMsDelay(1);
	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_RESET);
	//osDelay(1);
	//OS_Delay(1);
	MyMsDelay(1);

	HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, GPIO_PIN_SET);			// high
	//osDelay(1);
	//OS_Delay(1);
	MyMsDelay(1);
	HAL_GPIO_WritePin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin, GPIO_PIN_SET);			// high
	//osDelay(1);
	//OS_Delay(1);
	MyMsDelay(1);

	MX_I2C1_Init();
	//osDelay(5);
	//OS_Delay(5);
	MyMsDelay(5);

	printf("RECOVER_I2cError\r\n");

	return;
}


/*
@Name: xWRITE_DataToEeprom
@Brief: EEPROM�� ������ ����
@Param: 
@Param: 
@Param: 
@Return: None
@Note: 
*/
ErrState xWRITE_DataToEeprom(uint16_t WriteAddr, uint8_t* pData, uint16_t Length)
{
	uint16_t u16Timeout;
	uint16_t LengthSum=0, LengthCurrent;
	uint8_t u8LoopCount;

	if(pData == NULL)
		return _NullErr;

	if(Length == 0 || Length > _EEPROM_CAPACITY_BYTE_SIZE)
		return _LenErr;

	//OS_SEMAPHORE_Give(&OS_I2C_SEMAPHORE);
	//OS_SEMAPHORE_TakeBlocked(&OS_I2C_SEMAPHORE);
	//OS_MUTEX_LockBlocked(&OS_EEPROM_MUTEX);

	LengthCurrent = 64-(WriteAddr%64);    //그 블럭에서 쓸 수 있는 만큼만 준다..
	if(Length > LengthCurrent){     //64byte 단위로만 연속으로 쓸 수 있음.. 즉 63번지에서는 1바이트만 연속으로 쓸수 있고 0번지에서는 64바이트까지만 연속으로 쓸 수 있다..
	    u8LoopCount = ((64-LengthCurrent)+Length)/64;        //최소 1이상임..
	}
	else {
	    LengthCurrent = Length;
	    u8LoopCount = 0;
	}
	for (int i = 0; i <= u8LoopCount; i++){
	    I2CWPDisable();
	    MyMsDelay(1);

		//printf("[xWRITE_DataToEeprom] u16BytesToWrite:[%d], u16RemainingBytes:[%d], WriteAddr:[%ld]\r\n", u16BytesToWrite, (u16RemainingBytes-u16BytesToWrite), WriteAddr);

		if(HAL_I2C_Mem_Write(&_EEPROM_I2C_HANDLE, (uint16_t)_EEPROM_I2C_ADDR, WriteAddr+ LengthSum, _I2C_MEMADD_SIZE_16BIT, pData+ LengthSum, LengthCurrent, _200MS) != HAL_OK)
		{
			RECOVER_I2cError(&_EEPROM_I2C_HANDLE);

			//OS_MUTEX_Unlock(&OS_EEPROM_MUTEX);
			return _Fail;
		}
	    MyMsDelay(1);
		u16Timeout = 10;
		while(u16Timeout)
		{
			if(HAL_I2C_GetState(&_EEPROM_I2C_HANDLE) == HAL_I2C_STATE_READY)
				break;
			else
				u16Timeout--;

			MyMsDelay(1);
			//OS_Delay(1);
		}
		MyMsDelay(_24LC128_PAGE_WRITETIME_MAX);

	    I2CWPEnable();
	    LengthSum += LengthCurrent;
	    LengthCurrent = ((Length - LengthSum) >= 64) ? 64 : (Length - LengthSum);
	}

	//OS_SEMAPHORE_Give(&OS_I2C_SEMAPHORE);
	//OS_MUTEX_Unlock(&OS_EEPROM_MUTEX);

	return _Ok;
}


ErrState xERASE_EepromPage(uint8_t PageNum)
{
	uint8_t u8PageCnt, u8Trials;
	uint16_t u16WriteAddr;
	uint8_t buff[_24LC128_PAGE_SIZE];

	if(PageNum > _EEPROM_LAST_PAGE_NUM)
		return _Invalid;

	memset(buff, _EEPROM_ALL_ERASE, _24LC128_PAGE_SIZE);

	if(PageNum == _EEPROM_ALL_ERASE)
	{
		for(u8PageCnt=_EEPROM_FIRST_PAGE_NUM, u16WriteAddr=0; u8PageCnt < _EEPROM_LAST_PAGE_NUM; u8PageCnt++)
		{
			for(u8Trials=0; u8Trials < 2; u8Trials++)
			{	
				if(_Ok == xWRITE_DataToEeprom(u16WriteAddr, buff, _24LC128_PAGE_SIZE))
				{
					printf("<xERASE_EepromPage> [%d] Page erased\r\n", u8PageCnt); 
					u16WriteAddr += _24LC128_PAGE_SIZE;
					break;					
				}
				else
					printf("<xWRITE_DataToEeprom> xWRITE_BurstToEeprom Fail\r\n");
			}
		}
	}
	else
	{
		u16WriteAddr = (_24LC128_PAGE_SIZE * PageNum);

		for(u8Trials=0; u8Trials < 2; u8Trials++)
		{
			if(_Ok == xWRITE_DataToEeprom(u16WriteAddr, buff, _24LC128_PAGE_SIZE))
			{
				printf("<xERASE_EepromPage> [%d] Page erased\r\n", u8PageCnt);
				break;
			}
			else
				printf("<xWRITE_DataToEeprom> xWRITE_BurstToEeprom Fail\r\n");
		}
	}

	return _Ok;
}


ErrState xREAD_DataFromEeprom(uint16_t ReadAddr, uint8_t* pData, uint16_t Length)
{
	uint8_t u8Cnt, u16Timeout;

	if(pData == NULL)
		return _NullErr;

	if(Length == 0 || Length > _EEPROM_CAPACITY_BYTE_SIZE)
		return _LenErr;

	//printf("xREAD_DataFromEeprom\r\n");

	//OS_SEMAPHORE_Give(&OS_I2C_SEMAPHORE);
	//OS_SEMAPHORE_TakeBlocked(&OS_I2C_SEMAPHORE);
	//OS_MUTEX_LockBlocked(&OS_EEPROM_MUTEX);

	if(HAL_I2C_Mem_Read(&_EEPROM_I2C_HANDLE, (uint16_t)_EEPROM_I2C_ADDR, ReadAddr, _I2C_MEMADD_SIZE_16BIT, pData, Length, _200MS) != HAL_OK)
	{
		RECOVER_I2cError(&_EEPROM_I2C_HANDLE);
	}

	u16Timeout = 10;
	while(u16Timeout)
	{
		if(HAL_I2C_GetState(&_EEPROM_I2C_HANDLE) == HAL_I2C_STATE_READY)
			break;
		else
			u16Timeout--;

		MyMsDelay(1);
		//OS_Delay(1);
	}

	//OS_SEMAPHORE_Give(&OS_I2C_SEMAPHORE);
	//OS_MUTEX_Unlock(&OS_EEPROM_MUTEX);

	return _Ok;
}


#if 1
void TEST_Eeprom(void)
{
	uint16_t size = _24LC128_PAGE_SIZE+18;
	uint8_t wBuf[size], rBuf[size];
	static uint16_t addr = 0x017e;
	uint16_t i=1;
	uint8_t ret;


//	while(addr <  (_24LC128_PAGE_SIZE * 20))
	{
		if(addr > (_24LC128_PAGE_SIZE * 20))
			addr = 0;

		for( i = 0; i < size; i++ )
			wBuf[i] = i;

		ret = xWRITE_DataToEeprom(addr, wBuf, size);
		if(ret != _Ok) {
			printf("TEST_Eeprom !ok ret:[%02x]\r\n", ret);
		}

		MyMsDelay(_24LC128_PAGE_WRITETIME_MAX);
		//OS_Delay(_24LC128_PAGE_WRITETIME_MAX);

		memset(rBuf, 0, size);
		ret = xREAD_DataFromEeprom(addr, rBuf, size);
		if(ret != _Ok) {
			printf("TEST_Eeprom !ok ret:[%02x]\r\n", ret);
		}
		
		for( i = 0; i < size; i++ )
		{
			if( wBuf[i] != rBuf[i])
				printf("wBuf[%d]%02x != rBuf[%d]%02x/", i, wBuf[i], i, rBuf[i]);
		}
		addr+=size;
		MyMsDelay(100);
	}

	return;
}
#else
void TEST_Eeprom(void)
{
	uint16_t size = 64;
	uint8_t wBuf[size], rBuf[size];
	uint16_t addr;
	uint16_t i, j, k;
	uint8_t ret, trial=3;

	SEARCH_I2cSlavesAll();

	while(1)
	{
		printf("<TEST_Eeprom> 1-byte write: ");
		for(k=0, addr = 0; k < 10; k++, addr++)
		{
			xWRITE_DataToEeprom(addr, &k, 1);
			MyMsDelay(_24LC128_PAGE_WRITETIME_MAX);
			xREAD_DataFromEeprom(addr, rBuf, 1);
			printf("%d ", k);
		}
		printf("\r\n");

		MyMsDelay(5000);
	
		xERASE_EepromPage(_EEPROM_ALL_ERASE);

		MyMsDelay(_24LC128_PAGE_WRITETIME_MAX);


		j = 0;
		for(k=0, addr = 0; k < (_EEPROM_LAST_PAGE_NUM + 1); k++, addr += _24LC128_PAGE_SIZE)
		{
			j++;
			memset(wBuf, j, size);

			printf("wBuf: ");
			for(i=0; i < size; i++)
			{
				wBuf[i] = j;
				printf("%d ", wBuf[i]);
			}
			printf("\r\n");

			for(i=0; i < trial; i++)
			{
				ret = xWRITE_DataToEeprom(addr, wBuf, size);
				if(ret != _Ok) {
					printf("ret:[%02x]\r\n", ret);
				}
				else {
					break;
				}
			}

			MyMsDelay(_24LC128_PAGE_WRITETIME_MAX);

			for(i=0; i < trial; i++)
			{
				memset(rBuf, 0, size);
				ret = xREAD_DataFromEeprom(addr, rBuf, size);
				if(ret != _Ok) {
					printf("ret:[%02x]\r\n", ret);
				}
				else {
					break;
				}
			}

			printf("rBuf: ");
			for(i=0; i < size; i++)
			{
				printf("%d ", rBuf[i]);
			}
			printf("\r\n");
		}

		MyMsDelay(5000);
	}

	return;
}
#endif


void CREATE_EepromSync(void)
{
	//OS_SEMAPHORE_Create(&OS_I2C_SEMAPHORE, 3);
	//OS_MUTEX_Create(&OS_EEPROM_MUTEX);

	return;
}


void CREATE_EepromTest(void)
{
	//OS_TASK_CREATE(&OS_CTRL_BLOCK_TEST_EEPROM_TASK, "TEST_Eeprom", 100, TEST_Eeprom, OS_STACK_SIZE_TEST_EEPROM_TASK);

	return;
}


