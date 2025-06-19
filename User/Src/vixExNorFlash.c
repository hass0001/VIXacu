
#include "vixConfig.h"


#define _MX25L_QSPI1_HANDLE					hospi1


#define xOSPI_NOR_QPI_DISABLE      0x0
#define xOSPI_NOR_QPI_ENABLE       0x1


/*
@Name: OPEN_ExNorFlash
@Brief: External Nor Flash OSPI�� Open
@Param: None
@Return: None
@Note: ���� �� Chip Reset �ʿ�
*/


void OPEN_ExNorFlash(void)
{
	uint8_t manufactureId[3] = { 0, 0, 0 };
	//MX25LM51245G_Info_t get_info;
	
	xOSPI_ResetFlash();

	xOSPI_Config(&_MX25L_QSPI1_HANDLE);

	xOSPI_ReadChipID(&_MX25L_QSPI1_HANDLE, manufactureId);
//	printf("READ_FlashChipID\r\n");
//	printf("[%02x][%02x][%02x]\r\n", manufactureId[0], manufactureId[1], manufactureId[2]);

	return;
}

ErrState xWRITE_DataToExNorFlash(uint32_t WriteAddr, uint8_t* pData, uint16_t Length)
{
	if(xOSPI_WriteEnable(&_MX25L_QSPI1_HANDLE) != _Ok)
	{
		printf("[Fail] xWRITE_DataToExNorFlash => WriteEnable\r\n");
		return _Fail;
	}

	OSPI_RegularCmdTypeDef sCommand = { 0 };

	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_4PAGE_PROG_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= WriteAddr;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_4_LINES;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_4_LINES;
  	sCommand.NbData                     = Length;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Configure the command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

  	/* Transmission of the data */
  	if(HAL_OSPI_Transmit(&_MX25L_QSPI1_HANDLE, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

	/* Configure automatic polling mode to wait for end of program */
   	if(xOSPI_AutoPollingMemReady(&_MX25L_QSPI1_HANDLE, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != _Ok)
	{
   	    	return _Fail;
   	}
//   	printf("Success wirte Flash.\r\n");

//	MyMsDelay(MEMORY_REG_WRITE_DELAY);

	return _Ok;
}

ErrState xREAD_DataFromExNorFlash(uint32_t ReadAddr, uint8_t* pData, uint16_t Length)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };

	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_QREAD_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= ReadAddr;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_1_LINE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_4_LINES;
  	sCommand.NbData                     = Length;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 8U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xREAD_DataFromExNorFlash => HAL_OSPI_Command\r\n");
		return _Fail;
	}

	if(HAL_OSPI_Receive(&_MX25L_QSPI1_HANDLE, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xREAD_DataFromExNorFlash => HAL_OSPI_Receive\r\n");
		return _Fail;
	}

	return _Ok;
}

ErrState xOSPI_ResetFlash(void)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_AutoPollingTypeDef sConfig = { 0 };

	/* SPI mode - Reset Enable */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_RESET_ENABLE_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_24_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
  	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
  	sCommand.NbData                     = 0;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the Reset Enable RSTEN command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
 // 		printf("[Fail] xOSPI_ResetFlash => Reset enable\r\n");
    	return _Fail;
  	}

 // 	printf("[OK] xOSPI_ResetFlash => Reset enable\r\n");

  	/* More than 30ns */
	MyMsDelay(1);

	/* SPI mode - Reset command */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_RESET_MEMORY_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_24_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
  	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
  	sCommand.NbData                     = 0;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the Reset Memory RST command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
 //// 		printf("[Fail] xOSPI_ResetFlash => Reset command\r\n");
  		return _Fail;
  	}

 // 	printf("[OK] xOSPI_ResetFlash => Reset command\r\n");

  	/* More than 40ns */
//	MyMsDelay(1);

	/* Configure automatic polling mode to wait the memory is ready */
    sCommand.Instruction  = MX25L51245G_READ_STATUS_REG_CMD;
	sCommand.DataMode     = HAL_OSPI_DATA_1_LINE;
	sCommand.NbData       = 1;
	sCommand.DataDtrMode  = HAL_OSPI_DATA_DTR_DISABLE;

	sConfig.Match         = 0;
	sConfig.Mask          = MX25L51245G_SR_WIP;
	sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
	sConfig.Interval      = 0x10;
	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
//		printf("[Fail] xOSPI_ResetFlash => Read Status command\r\n");
		return _Fail;
	}

	if (HAL_OSPI_AutoPolling(&_MX25L_QSPI1_HANDLE, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_ResetFlash => Memory Ready\r\n");
		return _Fail;
	}

////	printf("[OK] xOSPI_ResetFlash => Memory Ready\r\n");
  	return _Ok;
}


/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  *         SPI/OPI; 1-0-1/1-0-8
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  ID 3 bytes IDs pointer
  * @param  DualFlash Dual flash mode state
  * @retval error status
  */
ErrState xOSPI_ReadChipID(OSPI_HandleTypeDef *hospi, uint8_t* ManufactureID)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };

	sCommand.OperationType			= HAL_OSPI_OPTYPE_COMMON_CFG;
	sCommand.FlashId				= HAL_OSPI_FLASH_ID_1;
	sCommand.InstructionMode		= HAL_OSPI_INSTRUCTION_1_LINE;
	sCommand.InstructionSize		= HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.InstructionDtrMode		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.AddressSize			= HAL_OSPI_ADDRESS_32_BITS;
	sCommand.AddressMode			= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressDtrMode			= HAL_OSPI_ADDRESS_DTR_DISABLE;
	sCommand.AlternateBytesMode		= HAL_OSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode 				= HAL_OSPI_DATA_1_LINE;
	sCommand.DataDtrMode			= HAL_OSPI_DATA_NONE;
	sCommand.DQSMode				= HAL_OSPI_DQS_DISABLE;
	sCommand.SIOOMode 				= HAL_OSPI_SIOO_INST_EVERY_CMD;
	sCommand.Address				= 0;
	sCommand.NbData					= 3U;
	sCommand.DummyCycles 			= 0U;
	sCommand.Instruction 			= MX25LM51245G_READ_ID_CMD;

	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}

	if(HAL_OSPI_Receive(&_MX25L_QSPI1_HANDLE, ManufactureID, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}

	return _Ok;
}



ErrState xOSPI_Config(OSPI_HandleTypeDef *hospi)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };
//	OSPI_AutoPollingTypeDef sConfig = { 0 };
	uint8_t test_buffer[2] = { 0 };

	/* read status register */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_READ_STATUS_REG_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.NbData                     = 1U;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;




	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_Config => Read Status Register Command\r\n");
		return _Fail;
	}

	
	if(HAL_OSPI_Receive(&_MX25L_QSPI1_HANDLE, &test_buffer[0], HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}


	/* read cfg register */
	sCommand.Instruction 				= MX25L51245G_READ_CFG_REG_CMD;


	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_Config => Read Cfg Register Command\r\n");
		return _Fail;
	}

	if(HAL_OSPI_Receive(&_MX25L_QSPI1_HANDLE, &test_buffer[1], HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}

//	printf("READ Register SR:%02x, CR:%02x\r\n", test_buffer[0], test_buffer[1]);

    /*modify buffer to enable quad mode*/
	test_buffer[0] |= 0x40;

	test_buffer[1] |= 0x27;

	/* Enable write operations */
   	if(xOSPI_WriteEnable(&_MX25L_QSPI1_HANDLE) != _Ok)
   	{
   		return _Fail;
	}

	/* Write Configuration register (with SPI protocol) */
   	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
   	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
   	sCommand.Instruction        		= MX25L51245G_WRITE_STATUS_REG_CMD;
    sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
   	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
   	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
   	sCommand.Address					= 0;
   	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
   	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
   	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
   	sCommand.AlternateBytes 	     	= 0;
   	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
   	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
   	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
   	sCommand.NbData                     = 2U;
   	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
   	sCommand.DummyCycles        		= 0U;
   	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
   	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;



	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}
	
   	if(HAL_OSPI_Transmit(&_MX25L_QSPI1_HANDLE, test_buffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
   		return _Fail;
   	}

 //  	printf("Enable Quad mode.\r\n");

   	if(xOSPI_AutoPollingMemReady(&_MX25L_QSPI1_HANDLE, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != _Ok)
   	{
   		return _Fail;
   	}


	

   	/* 4Bit address Enable  */
   	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
  	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
  	sCommand.Instruction        		= MX25L51245G_ENTER_4BYTE_MODE_CMD;
   	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
   	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
   	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
   	sCommand.Address					= 0;
   	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
   	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
   	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
   	sCommand.AlternateBytes 	     	= 0;
   	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
   	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
   	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
   	sCommand.NbData                     = 0U;
   	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
   	sCommand.DummyCycles        		= 0U;
   	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
   	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

   	if(HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
   		return _Fail;
	}

//	printf("Enable 4-byte address mode.\r\n");

   	/* Wait that the configuration is effective and check that memory is ready */
//   	MyMsDelay(MX25LM51245G_WRITE_REG_MAX_TIME);

#if 0 /* Read status/cfg register and confirm test */
	/* read status register */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_READ_STATUS_REG_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.NbData                     = 1U;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;




	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_Config => Read Status Register Command\r\n");
		return _Fail;
	}


	if(HAL_OSPI_Receive(hospi, &test_buffer[0], HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}


	/* read cfg register */
	sCommand.Instruction 				= MX25L51245G_READ_CFG_REG_CMD;


	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_Config => Read Cfg Register Command\r\n");
		return _Fail;
	}

	if(HAL_OSPI_Receive(&_MX25L_QSPI1_HANDLE, &test_buffer[1], HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return _Fail;
	}

//	printf("READ Register SR:%02x, CR:%02x\r\n", test_buffer[0], test_buffer[1]);
#endif

	return _Ok;
}


ErrState xOSPI_EraseChip(void)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };

  	/* Enable write operations */
  	if(xOSPI_WriteEnable(&_MX25L_QSPI1_HANDLE) != _Ok)
  	{
    		return _Fail;
  	}

	/* Erase command */
   	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
   	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
   	sCommand.Instruction        		= MX25L51245G_CHIP_ERASE_CMD;
    sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
   	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
   	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
   	sCommand.Address					= 0;
   	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
   	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
   	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
   	sCommand.AlternateBytes 	     	= 0;
   	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
   	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
   	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
   	sCommand.NbData                     = 0U;
   	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
   	sCommand.DummyCycles        		= 0U;
   	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
   	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    	return _Fail;
  	}

	if(xOSPI_AutoPollingMemReady(&_MX25L_QSPI1_HANDLE, MX25L51245G_BULK_ERASE_MAX_TIME) != _Ok)
	{
//		printf("[Fail] xOSPI_EraseChip => AutoPollingMemReady\r\n");
		return _Fail;
	}
//	printf("Success Chip Erase\r\n");

//	MyMsDelay(MEMORY_REG_WRITE_DELAY);

	return _Ok;
}

ErrState xERASE_ExFlash64kBlock(uint32_t AddrBlock64)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };

	uint32_t Block64 = (uint32_t)(AddrBlock64/MX25L51245G_BLOCK_SIZE);

  	if(Block64 >= (uint32_t)(MX25L51245G_FLASH_SIZE/MX25L51245G_BLOCK_SIZE))
  	{
    		return _Fail;
  	}

  	/* Enable write operations */
  	if(xOSPI_WriteEnable(&_MX25L_QSPI1_HANDLE) != _Ok)
  	{
    		return _Fail;
  	}

	/* Erase command */
   	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
   	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
   	sCommand.Instruction        		= MX25L51245G_BLOCK_ERASE_64K_CMD;
    sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
   	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
   	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
   	sCommand.Address					= (Block64 * MX25L51245G_BLOCK_SIZE);
   	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_1_LINE;
   	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
   	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
   	sCommand.AlternateBytes 	     	= 0;
   	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
   	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
   	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
   	sCommand.NbData                     = 0U;
   	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
   	sCommand.DummyCycles        		= 0U;
   	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
   	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    	return _Fail;
  	}

	if(xOSPI_AutoPollingMemReady(&_MX25L_QSPI1_HANDLE, MX25L51245G_BLOCK_64K_ERASE_MAX_TIME) != _Ok)
	{
		return _Fail;
	}
//	printf("Success Block 64K Erase\r\n");


//	MyMsDelay(MEMORY_REG_WRITE_DELAY);

	return _Ok;
}


ErrState xERASE_ExFlash4kSector(uint32_t AddrSector)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };

	uint32_t Sector = (uint32_t)(AddrSector/MX25L51245G_SECTOR_SIZE);

  	if(Sector >= (uint32_t)(MX25L51245G_FLASH_SIZE/MX25L51245G_SECTOR_SIZE))
  	{
    		return _Fail;
  	}

  	/* Enable write operations */
  	if(xOSPI_WriteEnable(&_MX25L_QSPI1_HANDLE) != _Ok)
  	{
    		return _Fail;
  	}

	/* Erase command */
   	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
   	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
   	sCommand.Instruction        		= MX25L51245G_SECTOR_ERASE_4K_CMD;
    sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
   	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
   	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
   	sCommand.Address					= (Sector * MX25L51245G_SECTOR_SIZE);
   	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_1_LINE;
   	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
   	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
   	sCommand.AlternateBytes 	     	= 0;
   	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
   	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
   	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
   	sCommand.NbData                     = 0U;
   	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
   	sCommand.DummyCycles        		= 0U;
   	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
   	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;


  	/* Send the command */
  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

	if(xOSPI_AutoPollingMemReady(&_MX25L_QSPI1_HANDLE, MX25L51245G_SECTOR_4K_ERASE_MAX_TIME) != _Ok)
	{
		return _Fail;
	}
//	printf("Success Sector 4K Erase\r\n");


//	MyMsDelay(MEMORY_REG_WRITE_DELAY);

	return _Ok;
}




ErrState xOSPI_WriteEnable(OSPI_HandleTypeDef *hospi)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_AutoPollingTypeDef sConfig = { 0 };

	/* Initialize the write enable command */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_WRITE_ENABLE_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_NONE;
  	sCommand.NbData                     = 0U;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;


  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
 // 		printf("[Fail] xOSPI_WriteEnable => Write Enable Command\r\n");
    	return _Fail;
  	}

	MyMsDelay(1);


  	/* Configure automatic polling mode to wait for memory ready ------ */
  	sCommand.Instruction  = MX25L51245G_READ_STATUS_REG_CMD;
  	sCommand.DataMode     = HAL_OSPI_DATA_1_LINE;
  	sCommand.NbData       = 1U;
  	sCommand.DataDtrMode  = HAL_OSPI_DATA_DTR_DISABLE;

  	sConfig.Match         = 0x02;
  	sConfig.Mask          = MX25L51245G_SR_WEL;
 	sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
 	sConfig.Interval      = 0x10;
 	sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;



	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_WriteEnable => Read Status Command\r\n");
		return _Fail;
	}



    if (HAL_OSPI_AutoPolling(&_MX25L_QSPI1_HANDLE, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
 //   	printf("[Fail] xOSPI_WriteEnable => latch WEL bit \r\n");
   		return _Fail;
   	}

 //  	printf("[OK] xOSPI_WriteEnable => latch WEL bit \r\n");

  	return _Ok;
}


/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout: Timeout for auto-polling
  * @retval None
  */
ErrState xOSPI_AutoPollingMemReady(OSPI_HandleTypeDef *hospi, uint32_t Timeout)
{

	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_AutoPollingTypeDef sConfig = { 0 };

	/* Configure automatic polling mode to wait for memory ready ------ */  
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
 	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
 	sCommand.Instruction        		= MX25L51245G_READ_STATUS_REG_CMD;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
 	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
	sCommand.Address					= 0;
	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize				= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.AddressDtrMode				= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AlternateBytes 	     	= 0;
 	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.AlternateBytesSize         = HAL_OSPI_ALTERNATE_BYTES_8_BITS;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.NbData                     = 1U;
  	sCommand.DataDtrMode                = HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;


  	if(HAL_OSPI_Command(&_MX25L_QSPI1_HANDLE, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		printf("[Fail] xOSPI_AutoPolling => Read Status Command\r\n");
		return _Fail;
	}


  	sConfig.Match         = 0x00;
    sConfig.Mask          = MX25L51245G_SR_WIP;
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;


    if (HAL_OSPI_AutoPolling(&_MX25L_QSPI1_HANDLE, &sConfig, Timeout) != HAL_OK)
    {
 //   	printf("[Fail] xOSPI_AutoPolling => Memory Ready\r\n");
   		return _Fail;
   	}

  // 	printf("[OK] xOSPI_AutoPolling => Memory Ready\r\n");


  	return _Ok;
}


ErrState xOSPI_EnableMemoryMappedMode(OSPI_HandleTypeDef *hospi)
{
	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_MemoryMappedTypeDef sMemMappedCfg = { 0 };

	xOSPI_WriteEnable(hospi);

 	/* Initialize the read command */
  	sCommand.OperationType      		= HAL_OSPI_OPTYPE_READ_CFG;
  	sCommand.FlashId					= HAL_OSPI_FLASH_ID_1;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
  	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
  	sCommand.Instruction        	 	= MX25L51245G_FAST_READ_CMD;
                                   //MX25LM51245G_4_BYTE_ADDR_FAST_READ_CMD)
  	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_1_LINE;
  	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AddressSize        		= HAL_OSPI_ADDRESS_24_BITS;
                                 // HAL_XSPI_ADDRESS_32_BITS;
  	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.DataDtrMode        		= HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= DUMMY_CYCLES_READ;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the read command */
  	if(HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}


  	/* Initialize the program command */
  	sCommand.OperationType      	= HAL_OSPI_OPTYPE_WRITE_CFG;
  	sCommand.Instruction        	= MX25L51245G_PAGE_PROG_CMD;
                                    // MX25LM51245G_4_BYTE_PAGE_PROG_CMD)
  	sCommand.DummyCycles        	= 0U;

  	/* Send the write command */
  	if(HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

  	/* Configure the memory mapped mode */
  	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

  	if(HAL_OSPI_MemoryMapped(hospi, &sMemMappedCfg) != HAL_OK)
  	{
    		return _Fail;
  	}

  	return _Ok;
}


/**
  * @brief  Read Flash configuration register value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value configuration register value pointer
  * @retval error status
  */
ErrState xMX25LM51245G_ReadCfgRegister(OSPI_HandleTypeDef *hospi, uint8_t *Value)
{
	OSPI_RegularCmdTypeDef sCommand = {0};

	/* Initialize the reading of configuration register */
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
  	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
  	sCommand.Instruction   			= MX25LM51245G_READ_CFG_REG_CMD;
  	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_NONE;
  	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AddressSize        		= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.Address            			= 1U;
  	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.DataDtrMode      			= HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.NbData             			= 1U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the command */
  	if(HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

  	/* Reception of the data */
  	if(HAL_OSPI_Receive(hospi, Value, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
    		return _Fail;
  	}

	return _Ok;
}


/**
  * @brief  Read Flash configuration register 2 value
  *         SPI/OPI
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  Rate Transfer rate STR or DTR
  * @param  Value configuration register 2 value pointer
  * @retval error status
  */
ErrState xMX25LM51245G_ReadCfg2Register(OSPI_HandleTypeDef *hospi, uint32_t ReadAddr, uint8_t *Value)
{
	OSPI_RegularCmdTypeDef sCommand = {0};

  	/* Initialize the reading of status register */
  	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG;
  	sCommand.InstructionMode    		= HAL_OSPI_INSTRUCTION_1_LINE;
  	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;
  	sCommand.Instruction        		= MX25LM51245G_READ_CFG_REG2_CMD;
  	sCommand.AddressMode        		= HAL_OSPI_ADDRESS_1_LINE;
  	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE;
  	sCommand.AddressSize        		= HAL_OSPI_ADDRESS_32_BITS;
  	sCommand.Address            		= ReadAddr;
  	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE;
  	sCommand.DataMode           		= HAL_OSPI_DATA_1_LINE;
  	sCommand.DataDtrMode        		= HAL_OSPI_DATA_DTR_DISABLE;
  	sCommand.DummyCycles        		= 0U;
  	sCommand.NbData             		= 1U;
  	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE;
  	sCommand.SIOOMode           		= HAL_OSPI_SIOO_INST_EVERY_CMD;

  	/* Send the command */
  	if(HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
  		return _Fail;
  	}

  	/* Reception of the data */
  	if(HAL_OSPI_Receive(hospi, Value, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  	{
  		return _Fail;
 	}

	return _Ok;
}


/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval error status
  */
void MX25LM51245G_GetFlashInfo(MX25LM51245G_Info_t *pInfo)
{
	/* Configure the structure with the memory configuration */
  	pInfo->FlashSize              = MX25LM51245G_FLASH_SIZE;
  	pInfo->EraseSectorSize        = MX25LM51245G_SECTOR_64K;
  	pInfo->EraseSectorsNumber     = (MX25LM51245G_FLASH_SIZE / MX25LM51245G_SECTOR_64K);
  	pInfo->EraseSubSectorSize     = MX25LM51245G_SUBSECTOR_4K;
  	pInfo->EraseSubSectorNumber   = (MX25LM51245G_FLASH_SIZE / MX25LM51245G_SUBSECTOR_4K);
  	pInfo->EraseSubSector1Size    = MX25LM51245G_SUBSECTOR_4K;
  	pInfo->EraseSubSector1Number  = (MX25LM51245G_FLASH_SIZE / MX25LM51245G_SUBSECTOR_4K);
  	pInfo->ProgPageSize           = MX25LM51245G_PAGE_SIZE;
  	pInfo->ProgPagesNumber        = (MX25LM51245G_FLASH_SIZE / MX25LM51245G_PAGE_SIZE);

  	return;
}

void TEST_ExNorFlash(void)
{
	uint16_t q, r, i=1;
	uint16_t count;
	static uint32_t addr=0x9b8000;
	uint32_t sector=0x9b8;
	uint32_t block=0x9b0000;

	uint16_t size_x2 = 4096;
	uint8_t wBuf[4096], rBuf[4096];

	for(count=0; count < size_x2; count++)
	{
		wBuf[count] = count;
	}
	//xOSPI_EraseSector(sector);
	//xERASE_ExFlash64kBlock(block);
	//xOSPI_EraseChip(); MX25L51245G_BLOCK_SIZE
	xERASE_ExFlash4kSector(sector*MX25L51245G_SECTOR_SIZE);

	memset(rBuf, 0, size_x2);

	for(count=0; count < size_x2; count+=256)
	{
		xWRITE_DataToExNorFlash(addr+count, wBuf+count, 256);
	}

	xREAD_DataFromExNorFlash(addr, rBuf, size_x2);
	printf("rBuf test 1: ");
	for(count=0; count < size_x2; count++)
	{
		if( wBuf[count] != rBuf[count])
			printf(" different %d [%02x : %02x] ", count, rBuf[count], wBuf[count]);
	}
	printf("\r\n");
//	HAL_Delay(2000);
	// Sector test
	/*for(count=0; count < size_x2; count+=256)
	{
		xWRITE_DataToExNorFlash(addr+count, wBuf+count, 256);
	}*/

	xERASE_ExFlash4kSector(sector*MX25L51245G_SECTOR_SIZE);

	for(count=0; count < size_x2; count++)
	{
		wBuf[count] = count+1;
	}
	memset(rBuf, 0, size_x2);

	for(count=0; count < size_x2; count+=256)
	{
		xWRITE_DataToExNorFlash(addr+count, wBuf+count, 256);
	}
	xREAD_DataFromExNorFlash(addr, rBuf, size_x2);
	printf("rBuf test s: ");
	for(count=0; count < size_x2; count++)
	{
		if( wBuf[count] != rBuf[count])
			printf(" xxxxxx %d [%02x : %02x] ", count, rBuf[count], wBuf[count]);
	}
	printf("\r\n");


//	addr = 0x2000000;
//	xREAD_DataFromExNorFlash(addr, rBuf, 1);

//	HAL_Delay(2000);
	return;
}

void CREATE_ExNorFlashTest(void)
{
	// OS_TASK_CREATE(&OS_CTRL_BLOCK_TEST_EXNORFLASH_TASK, "TEST_ExNorFlash", 100, TEST_ExNorFlash, OS_STACK_SIZE_TEST_EXNORFLASH_TASK);

	return;
}



/**
  * @brief  Command completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_CmdCpltCallback(OSPI_HandleTypeDef *hospi)
{
  //CmdCplt++;
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_RxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  //RxCplt++;
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
 void HAL_OSPI_TxCpltCallback(OSPI_HandleTypeDef *hospi)
{
  //TxCplt++; 
}

/**
  * @brief  Transfer Error callback.
  * @param  hospi: OSPI handle
  * @retval None
  */
void HAL_OSPI_ErrorCallback(OSPI_HandleTypeDef *hospi)
{
  //Error_Handler();
}


