//*****************************************************************************
//
//! \file w6100.c
//! \brief W6100 HAL Implements file.
//! \version 1.0.0
//! \date 2019/01/01
//! \par  Revision history
//!       <2019/01/01> 1st Release
//! \author MidnightCow
//! \copyright
//!
//! Copyright (c)  2019, WIZnet Co., LTD.
//!
//! Permission is hereby granted, free of charge, to any person obtaining a copy
//! of this software and associated documentation files (the "Software"), to deal
//! in the Software without restriction, including without limitation the rights 
//! to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//! copies of the Software, and to permit persons to whom the Software is 
//! furnished to do so, subject to the following conditions: 
//!
//! The above copyright notice and this permission notice shall be included in
//! all copies or substantial portions of the Software. 
//!
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//! AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//! OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//! SOFTWARE. 
//!
//*****************************************************************************

#include "vixConfig.h"


#define _WIZCHIP_SPI_VDM_OP_    0x00
#define _WIZCHIP_SPI_FDM_LEN1_  0x01
#define _WIZCHIP_SPI_FDM_LEN2_  0x02
#define _WIZCHIP_SPI_FDM_LEN4_  0x03

#define _W6100_SPI_OP_          _WIZCHIP_SPI_VDM_OP_

extern WIZ_DatapackTypeDef WIZ_Datapack;

//////////////////////////////////////////////////
void WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb)
{
	uint8_t tAD[4];

	_W6100_NSS_SELECT();

	tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   	tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   	tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
   	tAD[3] = wb;
	tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);

	WRITE_BurstToW6100(tAD, 4);

	_W6100_NSS_DESELECT();

	return;
}


uint8_t WIZCHIP_READ(uint32_t AddrSel)
{
	uint8_t ret;
  	uint8_t tAD[3];

	_W6100_NSS_SELECT();

   	tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   	tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   	tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
	tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);

	WRITE_BurstToW6100(tAD, 3);
	ret = READ_aByteFromW6100();

	_W6100_NSS_DESELECT();

	return ret;
}


void WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
	uint8_t tAD[3];

	_W6100_NSS_SELECT();

	tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
   	tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
   	tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
	tAD[2] |= (_W6100_SPI_WRITE_ | _W6100_SPI_OP_);

	WRITE_BurstToW6100(tAD, 3);
	WRITE_BurstToW6100(pBuf, len);

	_W6100_NSS_DESELECT();

	return;
}


void WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, datasize_t len)
{
	uint8_t tAD[3];

	_W6100_NSS_SELECT();

	tAD[0] = (uint8_t)((AddrSel & 0x00FF0000) >> 16);
	tAD[1] = (uint8_t)((AddrSel & 0x0000FF00) >> 8);
	tAD[2] = (uint8_t)(AddrSel & 0x000000ff);
	tAD[2] |= (_W6100_SPI_READ_ | _W6100_SPI_OP_);

	WRITE_BurstToW6100(tAD, 3);
	READ_BurstFromW6100(pBuf, len);

	_W6100_NSS_DESELECT();

   	return;
}


datasize_t getSn_TX_FSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(_Sn_TX_FSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(_Sn_TX_FSR_(sn),1));
   }while (val != prev_val);
   return val;
}

datasize_t getSn_RX_RSR(uint8_t sn)
{
   datasize_t prev_val=-1,val=0;
   do
   {
      prev_val = val;
      val = WIZCHIP_READ(_Sn_RX_RSR_(sn));
      val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(_Sn_RX_RSR_(sn),1));
   }while (val != prev_val);
   return val;
}

void wiz_send_data(uint8_t sn, uint8_t *wizdata, datasize_t len)
{
   datasize_t ptr = 0;
   uint32_t addrsel = 0;
   ptr = getSn_TX_WR(sn);
   addrsel = ((uint32_t)ptr << 8) + WIZCHIP_TXBUF_BLOCK(sn);
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);
   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, datasize_t len)
{
   datasize_t ptr = 0;
   uint32_t addrsel = 0;
   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   addrsel = ((uint32_t)ptr << 8) + WIZCHIP_RXBUF_BLOCK(sn);
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

void wiz_recv_ignore(uint8_t sn, datasize_t len)
{
   setSn_RX_RD(sn,getSn_RX_RD(sn)+len);
}


/// @cond DOXY_APPLY_CODE
#if (_PHY_IO_MODE_ == _PHY_IO_MODE_MII_)
/// @endcond
void wiz_mdio_write(uint8_t phyregaddr, uint16_t var)
{
   setPHYRAR(phyregaddr);
   setPHYDIR(var);
   setPHYACR(PHYACR_WRITE);
   while(getPHYACR());  //wait for command complete
}

uint16_t wiz_mdio_read(uint8_t phyregaddr)
{
   setPHYRAR(phyregaddr);
   setPHYACR(PHYACR_READ);
   while(getPHYACR());  //wait for command complete
   return getPHYDOR();
}

#else
#define _WIZCHIP_SPI_VDM_OP_    0x00
#define _WIZCHIP_SPI_FDM_LEN1_  0x01
#define _WIZCHIP_SPI_FDM_LEN2_  0x02
#define _WIZCHIP_SPI_FDM_LEN4_  0x03
#endif
//
// If you want to use SPI FDM mode, Feel free contact to WIZnet. 
// http://forum.wiznet.io
//

