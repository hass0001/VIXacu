/* Define to prevent recursive inclusion -------------------------------------*/
#define    CHECKSUM_GLOBALS 
#include "CheckSum.h"

uint32_t getExtFlashCRC( uint32_t fAddr, uint32_t fLen )
{
    uint32_t ulFlashAddr, ulCRC = 0;
	uint8_t rData;
    uint32_t  ulCheckLen;
    
    ulFlashAddr = fAddr;
    ulCheckLen  = fLen;
    
    ulCRC = ~ulCRC;
    while(ulCheckLen--) {
    	xREAD_DataFromExNorFlash( ulFlashAddr, &rData, 1);
        ulCRC = gCrcTable[(ulCRC ^ rData) & 0xFF] ^ (ulCRC >> 8);
        ulFlashAddr++;
    }
    ulCRC = ~ulCRC;

	return ulCRC;
}

void makeCrcGlobalTable(void)
{
	makeCRCtable(gCrcTable, 0xEDB88320);
}
/*
uint32_t calcCRC(uint8_t *mem, uint32_t size, uint32_t gCRC)
{
    uint32_t ulCRC = gCRC;

	ulCRC = ~ulCRC;

	while(size--)
		ulCRC = gCrcTable[(ulCRC ^ *(mem++)) & 0xFF] ^ (ulCRC >> 8);

	return ~ulCRC;
}
*/
void makeCRCtable(uint32_t *table, uint32_t id) {
	uint32_t i, j, k;

	for(i = 0; i < 256; ++i) {
		k = i;
		for(j = 0; j < 8; ++j) {
			if (k & 1)
				k = (k >> 1) ^ id;
			else
				k >>= 1;
		}
		table[i] = k;
	}
}

