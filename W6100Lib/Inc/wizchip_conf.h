//* ****************************************************************************
//! \file wizchip_conf.h
//! \brief WIZCHIP Config Header File.
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



#ifndef  _WIZCHIP_CONF_H_
#define  _WIZCHIP_CONF_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup extra_functions 2. WIZnet Extra Functions
 *
 * @brief These functions is optional function. 
 * @details @ref extra_functions could be replaced with @ref WIZCHIP_IO_Functions_W6100, \n
 *          because they were made by @ref Basic_IO_function_W6100.\n
 *          These functions configure or indicate @ref _WIZCHIP_, network information, interrupt, PHY, and timer.
 */


#define W6100                          6100     ///< W6100 Definition
//Add to
//

#ifndef _WIZCHIP_
/**
 * @brief Select WIZCHIP.
 * @todo You should select one, \b W6100, etc. \n\n
 *       ex> <code> #define _WIZCHIP_      W6100 </code>
 *
 */
#define _WIZCHIP_                      W6100    // W6100
#endif

#define _WIZCHIP_IO_MODE_NONE_         0x0000
#define _WIZCHIP_IO_MODE_BUS_          0x0100   ///< Bus interface mode.\n Refer to @ref _WIZCHIP_IO_MODE_BUS_DIR_ or @ref _WIZCHIP_IO_MODE_BUS_INDIR_.
#define _WIZCHIP_IO_MODE_SPI_          0x0200   ///< SPI interface mode.\n Refer to @ref _WIZCHIP_IO_MODE_SPI_VDM_ or @ref _WIZCHIP_IO_MODE_SPI_FDM_.
//#define _WIZCHIP_IO_MODE_IIC_          0x0400
//#define _WIZCHIP_IO_MODE_SDIO_         0x0800
// Add to
//

#define _WIZCHIP_IO_MODE_BUS_DIR_      (_WIZCHIP_IO_MODE_BUS_ + 1) ///< BUS interface mode for direct.\n Refer to @ref _WIZCHIP_IO_MODE_BUS_.
#define _WIZCHIP_IO_MODE_BUS_INDIR_    (_WIZCHIP_IO_MODE_BUS_ + 2) ///< BUS interface mode for indirect.\n Refer to @ref _WIZCHIP_IO_MODE_BUS_.

#define _WIZCHIP_IO_MODE_SPI_VDM_      (_WIZCHIP_IO_MODE_SPI_ + 1) ///< SPI interface mode for variable length data.\n Refer to @ref _WIZCHIP_IO_MODE_SPI_
/**
 * @brief SPI interface mode for fixed length data mode.
 * @note If you want to use SPI FDM, Feel free to send a email to support@wiznet.io.
 * @sa _WIZCHIP_IO_MODE_SPI_
 */
#define _WIZCHIP_IO_MODE_SPI_FDM_      (_WIZCHIP_IO_MODE_SPI_ + 2) 


/**
 * @brief PHY can be accessed by @ref _PHYCR0_, _PHYCR1_.
 * @details It provides hardware access method. 
 * @note It is smaller s/w footprint than @ref _PHY_IO_MODE_MII_.
 * @sa _PHY_IO_MODE_MII_, _PHY_IO_MODE_
 * @sa ctlwizchip(), getPHYCR0(), getPHYCR1(), setPHYCR1(), getPHYSR()
 */
#define _PHY_IO_MODE_PHYCR_            0x0000

/**
 * @brief PHY can be accessed by MDC/MDIO signals of MII interface. 
 * @details It provide software access method. 
 * @note It is bigger s/w footprint than @ref _PHY_IO_MODE_PHYCR_.
 * @sa _PHY_IO_MODE_PHYCR_, _PHY_IO_MODE_
 * @sa ctlwizchip(), wiz_read_mdio(), wiz_write_mdio()
 */
#define _PHY_IO_MODE_MII_              0x0010

/**
 * @brief Select PHY Access Mode
 * @details @ref _PHY_IO_MODE_ selects PHY access method.
 * @todo You should select one of @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_.
 * @sa ctlwizchip()
 */
#define _PHY_IO_MODE_                  _PHY_IO_MODE_PHYCR_ //_PHY_IO_MODE_MII_


#if (_WIZCHIP_ == W6100)
   #define _WIZCHIP_ID_                "W6100\0"

   /**
   * @brief Define @ref _WIZCHIP_ interface mode.
   * @todo You should select interface mode of @ref _WIZCHIP_.\n\n
   *       Select one of @ref _WIZCHIP_IO_MODE_SPI_VDM_, @ref _WIZCHIP_IO_MODE_SPI_FDM_, and @ref _WIZCHIP_IO_MODE_BUS_INDIR_
   * @sa WIZCHIP_READ(), WIZCHIP_WRITE(), WIZCHIP_READ_BUF(), WIZCHIP_WRITE_BUF()
   */
   //#define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_BUS_INDIR_
   #define _WIZCHIP_IO_MODE_         _WIZCHIP_IO_MODE_SPI_VDM_
   //#define _WIZCHIP_IO_MODE_         _WIZCHIP_IO_MODE_SPV_FDM_

   typedef   uint8_t   iodata_t;       ///< IO access unit. bus width
   typedef   int16_t   datasize_t;     ///< sent or received data size
   #include "w6100.h"
   #include "Application.h"

#else
   #error "Unknown defined _WIZCHIP_. You should define 6100"
#endif

#ifndef _WIZCHIP_IO_MODE_
   #error "Undefined _WIZCHIP_IO_MODE_. You should define it !!!"
#endif

/**
 * @brief Define I/O base address of @ref _WIZCHIP_.
 * @todo If you want to use @ref _WIZCHIP_IO_MODE_BUS_ such as @ref _WIZCHIP_IO_MODE_BUS_DIR_ and @ref _WIZCHIP_IO_MODE_BUS_INDIR_,\n
 *       You should redefine @ref _WIZCHIP_IO_BASE_ to a free address space of your system.\n
 *       ex> <code> #define _WIZCHIP_IO_BASE_      0x00008000 </code> \n\n
 *       Otherwise, No need to redefine it.
 */
#if _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_
   #define _WIZCHIP_IO_BASE_            0x60000000   // for W6100 BUS
#elif _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_
   #define _WIZCHIP_IO_BASE_            0x00000000   // for W6100 SPI
#else
   #error "You should define _WIZCHIP_IO_BASE_ to fit your system memory map."
#endif


#define _WIZCHIP_SOCK_NUM_   8   ///< The count of independent SOCKET of @ref _WIZCHIP_


/********************************************************
* WIZCHIP BASIC IF functions for SPI, SDIO, I2C , ETC.
*********************************************************/
/**
 * @ingroup DATA_TYPE
 * @brief Callback function set for @ref _WIZCHIP_
 * @details @ref __WIZCHIP_T__ is a structure type to provide the callback function set \n
 * *        which is called instead of the default functions \n
 *          in order to your HOST dependent functions can access to @ref _WIZCHIP_.
 * @note If it is not registered, the default function is called.
 * @sa WIZCHIP_READ(), WIZCHIP_WRITE(), WIZCHIP_READ_BUF(), WIZCHIP_WRITE_BUF()
 * @sa reg_wizchip_cris_cbfunc(), reg_wizchip_cs_cbfunc(), reg_wizchip_bus_cbfunc(), reg_wizchip_spi_cbfunc()
 */
typedef struct __WIZCHIP_T__
{

   uint16_t  if_mode;                     ///< HOST Interface Mode
   uint8_t   id[6];                       ///< ID of CHIP such as W6100, and so on.

   ///< The set of critical section callback function.
   struct _CRIS
   {
      void (*_e_n_t_e_r_)  (void);        ///< critical section enter
      void (*_e_x_i_t_) (void);           ///< critical section exit
   }CRIS;  

   ///< The set of @ref _WIZCHIP_ select control callback function.
   struct _CS
   {
      void (*_s_e_l_e_c_t_)  (void);      ///< @ref _WIZCHIP_ selected
      void (*_d_e_s_e_l_e_c_t_)(void);    ///< @ref _WIZCHIP_ deselected
   }CS;  

   ///< The set of interface IO callback function.
   union _IF
   {
      ///< The callback function of @ref _WIZCHIP_IO_MODE_BUS_ such as @ref _WIZCHIP_IO_MODE_BUS_DIR_ and @ref _WIZCHIP_IO_MODE_BUS_INDIR_
      struct _BUS
      {
         iodata_t  (*_read_data)      (uint32_t AddrSel);               ///< Read 1 @ref iodata_t from @ref _WIZCHIP_ through BUS
         void      (*_write_data)     (uint32_t AddrSel, iodata_t wb);  ///< Write 1 @ref iodata_t to @ref _WIZCHIP_ through BUS
         void      (*_read_data_buf)  (uint32_t AddrSel, iodata_t* pBuf, datasize_t len, uint8_t addrinc);  ///< Read @ref iodata_t as many as <i>len</i> from @ref _WIZCHIP_ through BUS
         void      (*_write_data_buf) (uint32_t AddrSel, iodata_t* pBuf, datasize_t len, uint8_t addrinc);  ///< Write @ref iodata_t data as many as <i>len</i> to @ref _WIZCHIP_ through BUS
      }BUS;

      ///< The callback function of @ref _WIZCHIP_IO_MODE_SPI_ such as @ref _WIZCHIP_IO_MODE_SPI_VDM_ and @ref _WIZCHIP_IO_MODE_SPI_FDM_
      struct _SPI
      {
         uint8_t   (*_read_byte)      (void);                           ///< Read 1 byte data from @ref _WIZCHIP_ through SPI
         void      (*_write_byte)     (uint8_t wb);                     ///< Write 1 byte data to @ref _WIZCHIP_ through SPI
         void      (*_read_byte_buf)  (uint8_t* pBuf, datasize_t len);  ///< Read byte data as many as <i>len</i> from @ref _WIZCHIP_ through SPI
         void      (*_write_byte_buf) (uint8_t* pBuf, datasize_t len);  ///< Write byte data as many as <i>len</i> to @ref _WIZCHIP_ through SPI
      }SPI;
   }IF;    

}_WIZCHIP_T_;


extern _WIZCHIP_T_  WIZCHIP;  ///< @ref WIZCHIP is instance of @ref _WIZCHIP_T_ to access @ref _WIZCHIP_.


/**
 * @ingroup DATA_TYPE
 * @brief @ref _WIZCHIP_ Control Type Enumeration.
 * @details @ref ctlwizchip_type includes a general control function for @ref _WIZCHIP_ and the integrated Ethernet PHY.
 * @note If @ref _PHY_IO_MODE_ is @ref _PHY_IO_MODE_PHYCR_,\n
 *       the PHY control types such as following can configure the integrate PHY thru @ref _PHYCR0_ & @ref _PHYCR1_, or indicate thru or @ref _PHYSR_.\n
 *       - @ref CW_RESET_PHY      : It can configure the PHY only when @ref SYS_PHY_LOCK is unlock.
 *       - @ref CW_SET_PHYCONF    : It can configure the PHY only when @ref SYS_PHY_LOCK is unlock.
 *       - @ref CW_GET_PHYCONF    
 *       - @ref CW_GET_PHYSTATUS
 *       - @ref CW_SET_PHYPOWMODE : It can configure the PHY only when @ref SYS_PHY_LOCK is unlock.
 *       - @ref CW_GET_PHYPOWMODE
 *       - @ref CW_GET_PHYLINK
 *       Else if @ref _PHY_IO_MODE_ is @ref _PHY_IO_MODE_MII_, \n
 *       the PHY control types configure or indicate through two MDC/MDIO signal of MII \n
 *       regardless of @ref SYS_PHY_LOCK.
 * @sa ctlwizchip()
 */
typedef enum
{
   CW_SYS_LOCK,           ///< Lock or Unlock @ref _WIZCHIP_ with @ref SYS_CHIP_LOCK, @ref SYS_PHY_LOCK, and @ref SYS_NET_LOCK
   CW_SYS_UNLOCK,         ///< Lock or Unlock @ref _WIZCHIP_ with @ref SYS_CHIP_LOCK, @ref SYS_PHY_LOCK, and @ref SYS_NET_LOCK
   CW_GET_SYSLOCK,        ///< Get the lock status of @ref _WIZCHIP_ with @ref SYS_CHIP_LOCK, @ref SYS_PHY_LOCK, and @ref SYS_NET_LOCK

   CW_RESET_WIZCHIP,      ///< Reset @ref _WIZCHIP_ by software
   CW_INIT_WIZCHIP,       ///< Initialize to SOCKETn buffer size with n byte array typed uint8_t
   CW_GET_INTERRUPT,      ///< Get the interrupt status with @ref intr_kind 
   CW_CLR_INTERRUPT,      ///< Clear the interrupt with @ref intr_kind
   CW_SET_INTRMASK,       ///< Set the interrupt mask with @ref intr_kind
   CW_GET_INTRMASK,       ///< Get the interrupt mask with @ref intr_kind
   CW_SET_INTRTIME,       ///< Set the interrupt pending time 
   CW_GET_INTRTIME,       ///< Get the interrupt pending time
   CW_SET_IEN,            ///< Set the global interrupt enable only when @ref SYS_CHIP_LOCK is not set
   CW_GET_IEN,            ///< Get the global interrupt enable 

   CW_GET_ID,             ///< Get @ref _WIZCHIP_ name.
   CW_GET_VER,            ///< Get the version of TCP/IP TOE engine

   CW_SET_SYSCLK,         ///< Set the system clock with @ref SYSCLK_100MHZ or SYSCLK_10MHZ only when @ref SYS_CHIP_LOCK is unlock
   CW_GET_SYSCLK,         ///< Get the system clock with @ref SYSCLK_100MHZ or SYSCLK_10MHZ 

   CW_RESET_PHY,          ///< Reset PHY 
   CW_SET_PHYCONF,        ///< Set PHY operation mode (Manual/Auto, 10/100, Half/Full) with @ref wiz_PhyConf
   CW_GET_PHYCONF,        ///< Get PHY operation mode (Manual/Auto, 10/100, Half/Full) with @ref wiz_PhyConf
   CW_GET_PHYSTATUS,      ///< Get real operation mode with @ref wiz_PhyConf when PHY is linked up.  
   CW_SET_PHYPOWMODE,     ///< Set PHY power mode with @ref PHY_POWER_NORM or PHY_POWER_DOWN
   CW_GET_PHYPOWMODE,     ///< Get PHY Power mode with @ref PHY_POWER_NORM or PHY_POWER_DOWN
   CW_GET_PHYLINK         ///< Get PHY Link status with @ref PHY_LINK_ON or @ref PHY_LINK_OFF
}ctlwizchip_type;


/**
 * @ingroup DATA_TYPE
 * @brief  Network control type enumeration
 * @details @ref ctlnetwork_type includes a general network configuration function for @ref _WIZCHIP_.
 * @sa ctlnetwork().
 */
typedef enum
{
   CN_SET_NETINFO,  ///< Set Network with @ref wiz_NetInfo
   CN_GET_NETINFO,  ///< Get Network with @ref wiz_NetInfo
   CN_SET_NETMODE,  ///< Set network mode with @ref netmode_type
   CN_GET_NETMODE,  ///< Get network mode with @ref netmode_type
   CN_SET_TIMEOUT,  ///< Set network timeout with @ref wiz_NetTimeout
   CN_GET_TIMEOUT,  ///< Get network timeout with @ref wiz_NetTimeout
   CN_SET_PREFER,   ///< Set the preferred source IPv6 address of @ref _SLCR_.\n Refer to @ref IPV6_ADDR_AUTO, @ref IPV6_ADDR_LLA, @ref IPV6_ADDR_GUA
   CN_GET_PREFER,   ///< Get the preferred source IPv6 address of @ref _SLCR_.\n Refer to @ref IPV6_ADDR_AUTO, @ref IPV6_ADDR_LLA, @ref IPV6_ADDR_GUA
}ctlnetwork_type;

/**
 * @ingroup DATA_TYPE
 * @brief  Network Service Control Type enumeration
 * @details @ref ctlnetservice_type includes network management or monitor functions for @ref _WIZCHIP_.
 * @sa ctlnetservice(), wiz_IPAddress, wiz_Prefix
 */
typedef enum
{
   CNS_ARP,            ///< ARP process with @ref wiz_IPAddress
   CNS_PING,           ///< PING process with @ref wiz_IPAddress
   CNS_DAD,            ///< Duplicated IPv6 Address Detection
   /**
    * @brief Stateless Address Auto-configuration(SLAAC) with @ref wiz_Prefix. 
    * @details @ref CNS_SLAAC sends first RS message to all-router and then receives RA message from a router.
    * @note It is valid only when the first received RA option is the source link-layer address(0x01) and the second is prefix information(0x03).\n
    *       Refer to @ref SLIR_RS.
    * @sa ctlnetservice()
    * @sa CNS_GET_PREFIX
    */
   CNS_SLAAC,          
   CNS_UNSOL_NA,       ///< Unsolicited Neighbor Advertisement for update @ref _WIZCHIP_ network information to neighbors
   /**
    * @brief Get prefix information with @ref wiz_Prefix.  
    * @details @ref CNS_GET_PREFIX can get prefix information of RA message to be sent by a router without RS message.
    * @note It is valid only when @ref IK_SOCKL_RA is set and the prefix information(0x03) of RA option is first received. 
    * @sa ctlnetservice()
    * @sa CNS_SLAAC
    */
   CNS_GET_PREFIX      
}ctlnetservice_type;

/**
 * @ingroup DATA_TYPE
 * @brief Interrupt Kind
 * @details @ref intr_kind can be used as the interrupt bits of @ref _IR_, @ref _SIR_, and @ref _SLIR_,\n
 *          It can be used as the interrupt mask bits of @ref _IMR_, @ref _SIMR_, and @ref _SLIMR_,\n
 *          and also, It can be used as the interrupt clear bits of @ref _IRCLR_, @ref _Sn_IRCLR_, and @ref _SLIRCLR_.
 * @note It can be used with @b OR operation.
 * @sa ctlwizchip(), CW_GET_INTERRUPT, CW_CLR_INTERRUPT, CW_GET_INTRMASK, CW_SET_INTRMASK
 * @sa ctlnetservice(), ctlnetservice_type
 * @sa wizchip_getinterrupt(), wizchip_clrinterrupt(), wizchip_getinterruptmask(), wizchip_setinterruptmask()
 */
typedef enum
{
   IK_PPPOE_TERMINATED = (1 << 0),     ///< PPPoE Termination Interrupt
   IK_DEST_UNREACH     = (1 << 1),     ///< ICMPv4 Destination Unreachable Interrupt
   IK_IP_CONFLICT      = (1 << 2),     ///< IPv4 Address Conflict Interrupt
   IK_DEST_UNREACH6    = (1 << 4),     ///< ICMPv6 Destination Unreachable Interrupt
   IK_WOL              = (1 << 7),     ///< WOL magic packet Interrupt
   IK_NET_ALL          = (0x97),       ///< All Network Interrupt
   
   IK_SOCK_0           = (1 << 8),     ///< Socket 0 Interrupt
   IK_SOCK_1           = (1 << 9),     ///< Socket 1 Interrupt
   IK_SOCK_2           = (1 << 10),    ///< Socket 2 Interrupt
   IK_SOCK_3           = (1 << 11),    ///< Socket 3 Interrupt
   IK_SOCK_4           = (1 << 12),    ///< Socket 4 Interrupt
   IK_SOCK_5           = (1 << 13),    ///< Socket 5 Interrupt
   IK_SOCK_6           = (1 << 14),    ///< Socket 6 Interrupt
   IK_SOCK_7           = (1 << 15),    ///< Socket 7 Interrupt
   IK_SOCK_ALL         = (0xFF << 8),  ///< All Socket Interrupt

   IK_SOCKL_TOUT       = (1 << 16),    ///< @ref _SLCR_ Timeout Interrupt.\n Refer to @ref ctlnetservice_type.
   IK_SOCKL_ARP4       = (1 << 17),    ///< @ref _SLCR_ APR4 Interrupt.\n Refer to @ref CNS_ARP.
   IK_SOCKL_PING4      = (1 << 18),    ///< @ref _SLCR_ PING4 Interrupt.\n Refer to @ref CNS_PING.
   IK_SOCKL_ARP6       = (1 << 19),    ///< @ref _SLCR_ ARP6 Interrupt.\n Refer to @ref CNS_ARP.
   IK_SOCKL_PING6      = (1 << 20),    ///< @ref _SLCR_ PING6 Interrupt.\n Refer to @ref CNS_PING.
   IK_SOCKL_NS         = (1 << 21),    ///< @ref _SLCR_ NS Interrupt.\n Refer to @ref CNS_DAD.
   IK_SOCKL_RS         = (1 << 22),    ///< @ref _SLCR_ RS Interrupt.\n Refer to @ref CNS_SLAAC.
   IK_SOCKL_RA         = (1 << 23),    ///< @ref _SLCR_ RA Interrupt.\n Refer to @ref CNS_GET_PREFIX.
   IK_SOCKL_ALL        = (0xFF << 16), ///< @ref _SLCR_ All Interrupt

   IK_INT_ALL          = (0x00FFFF97)   ///< All Interrupt
}intr_kind;



#define SYS_CHIP_LOCK           (1<<2)   ///< CHIP LOCK. \n Refer to @ref CW_SYS_LOCK, @ref CW_SYS_UNLOCK, and @ref CW_GET_SYSLOCK.
#define SYS_NET_LOCK            (1<<1)   ///< NETWORK Information LOCK. \n Refer to @ref CW_SYS_LOCK, @ref CW_SYS_UNLOCK, and @ref CW_GET_SYSLOCK.
#define SYS_PHY_LOCK            (1<<0)   ///< PHY LOCK.\n Refer to @ref CW_SYS_LOCK, @ref CW_SYS_UNLOCK, and @ref CW_GET_SYSLOCK.

#define SYSCLK_100MHZ            0       ///< System Clock 100MHz.\n Refer to Refer to @ref CW_SET_SYSCLK and  @ref CW_GET_SYSCLK.
#define SYSCLK_25MHZ             1       ///< System Clock 25MHz.\n Refer to Refer to @ref CW_SET_SYSCLK and  @ref CW_GET_SYSCLK.

#define PHY_MODE_MANUAL          0       ///< Configured PHY operation mode with user setting.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
#define PHY_MODE_AUTONEGO        1       ///< Configured PHY operation mode with auto-negotiation.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
/**
 * @brief Configured PHY operation mode with 10Base-Te.
 * @details It can be set or get through @ref ctlwizchip() with @ref CW_SET_PHYCONF or @ref CW_GET_PHYCONF.
 * @note It is controlled by @ref _PHYCR1_ regardless of @ref _PHY_IO_MODE_.
 * @note After 10Base-Te mode is set, Ethernet PHY should be reset by calling @ref ctlwizchip() with @ref CW_RESET_PHY
 */
#define PHY_MODE_TE              2
#define PHY_SPEED_10             0       ///< PHY Link Speed 10.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
#define PHY_SPEED_100            1       ///< PHY Link Speed 100.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
#define PHY_DUPLEX_HALF          0       ///< PHY Link Half-Duplex.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
#define PHY_DUPLEX_FULL          1       ///< PHY Link Full-Duplex.\n Refer to @ref CW_SET_PHYCONF and @ref CW_GET_PHYCONF.
#define PHY_LINK_OFF             0       ///< PHY Link Off.\n Refer to @ref CW_GET_PHYLINK.
#define PHY_LINK_ON              1       ///< PHY Link On.\n Refer to @ref CW_GET_PHYLINK.
#define PHY_POWER_NORM           0       ///< PHY power normal mode.\n Refer to @ref CW_SET_PHYPOWMODE and @ref CW_GET_PHYPOWMODE
#define PHY_POWER_DOWN           1       ///< PHY power down mode.\n Refer to @ref CW_SET_PHYPOWMODE and @ref CW_GET_PHYPOWMODE

#define IPV6_ADDR_AUTO           0x00    ///< IPv6 Address Type : Auto.\n Refer to @ref CN_SET_PREFER, @ref CN_GET_PREFER.
#define IPV6_ADDR_LLA            0x02    ///< IPv6 Address Type : LLA. \n Refer to @ref CN_SET_PREFER, @ref CN_GET_PREFER, @ref CNS_DAD.
#define IPV6_ADDR_GUA            0x03    ///< IPv6 Address Type : GUA. \n Refer to @ref CN_SET_PREFER, @ref CN_GET_PREFER, @ref CNS_DAD.


/**
 * @ingroup DATA_TYPE
 * @brief Ethernet PHY operation mode configuration
 * @details @ref wiz_PhyConf is a structure type to configure or \n
 *          indicate the PHY operation mode such as auto-negotiation, speed, and duplex.\n
 * @note If @ref _PHY_IO_MODE_ is defined with @ref _PHY_IO_MODE_PHYCR_,\n
 *       the PHY operation mode is configured by @ref _PHYCR0_ and is indicated by @ref _PHYSR_.\n
 *       Otherwise, the PHY operation mode is configured or indicated by two MDC/MDIO signal of MII.
 * @note In order to apply @ref wiz_PhyConf to the integrated Ethernet PHY, \n
 *       It needs to reset the PHY by calling @ref ctlwizchip(@ref CW_RESET_PHY)
 * @sa ctlwizchip(), CW_SET_PHYCONF, CW_GET_PHYCONF, CW_RESET_PHY
 * @sa wizphy_setphyconf(), wizphy_getphyconf(), wizphy_reset()
 */
typedef struct wiz_PhyConf_t
{
      uint8_t mode;     ///< set by @ref PHY_MODE_MANUAL ,@ref PHY_MODE_AUTONEGO, or @ref PHY_MODE_TE
      uint8_t speed;    ///< set by @ref PHY_SPEED_10 or @ref PHY_SPEED_100
      uint8_t duplex;   ///< set by @ref PHY_DUPLEX_HALF @ref PHY_DUPLEX_FULL 
}wiz_PhyConf;

/**
 * @ingroup DATA_TYPE
 * @brief IP Address Configuration Mode
 * @details @ref ipconf_mode can be used to save the DHCP mode running on your system.
 * @sa ctlnetwork(), CN_SET_NETINFO, CN_GET_NETINFO
 * @sa wizchip_setnetinfo(), wizchip_getnetinfo(), wiz_NetInfo
 */
typedef enum
{
   NETINFO_NONE       = 0x00,    ///< No use DHCP
   NETINFO_STATIC_V4  = 0x01,    ///< Static IPv4 configuration by manually.
   NETINFO_STATIC_V6  = 0x02,    ///< Static IPv6 configuration by manually.
   NETINFO_STATIC_ALL = 0x03,    ///< Static IPv4 and IPv6 configuration by manually.
   NETINFO_SLAAC_V6   = 0x04,    ///< Stateless Adders Auto Configuration for IPv6
   NETINFO_DHCP_V4    = 0x10,    ///< Dynamic IPv4 configuration from a DHCP sever
   NETINFO_DHCP_V6    = 0x20,    ///< Dynamic IPv6 configuration from a DHCP sever
   NETINFO_DHCP_ALL   = 0x30     ///< Dynamic IPv4 and IPv6 configuration from a DHCP sever
}ipconf_mode;

/**
 * @ingroup DATA_TYPE
 * @brief Network Information for @ref _WIZCHIP_
 * @details @ref wiz_NetInfo is a structure type to configure or indicate the network information of @ref _WIZCHIP_.
 * @sa ctlnetwork(), CN_SET_NETINFO, CN_GET_NETINFO
 * @sa wizchip_setnetinfo(), wizchip_getnetinfo()
 */
typedef struct wiz_NetInfo_t
{
   uint8_t     mac[6];    ///< Source Hardware Address
   uint8_t     ip[4];     ///< Source IPv4 Address
   uint8_t     sn[4];     ///< Subnet Mask value
   uint8_t     gw[4];     ///< Gateway IPv4 Address
   uint8_t     lla[16];   ///< Source Link Local Address
   uint8_t     gua[16];   ///< Source Global Unicast Address
   uint8_t     sn6[16];   ///< IPv6 Prefix
   uint8_t     gw6[16];   ///< Gateway IPv6 Address
   uint8_t     dns[4];    ///< DNS server IPv4 Address
   uint8_t     dns6[16];  ///< DNS server IPv6 Address
   ipconf_mode ipmode;    ///< IP Configuration Mode
}wiz_NetInfo;

/**
 * @ingroup DATA_TYPE
 * @brief Network mode Configuration
 * @details @ref netmode_type includes the network mode control function such as ping, TCP/RST block and etc.
 * @sa ctlnetwork(), CN_SET_NETMODE, CN_GET_NETMODE
 */
typedef enum
{
   // NETMR Bit Values
   NM_IPB_V4            = (1 << 0),      ///< IPv4 Packet Block
   NM_IPB_V6            = (1 << 1),      ///< IPv6 Packet Block
   NM_WOL               = (1 << 2),      ///< Wake On Lan(WOL) Mode
   NM_PB6_MULTI         = (1 << 4),      ///< PING6 request from multicasting group address Block
   NM_PB6_ALLNODE       = (1 << 5),      ///< PING6 request from all-node multicasting address Block
   NM_MR_MASK           = (0x37),        ///< @ref _NETMR_ Mask value

   // NETMR2 Bit Values
   NM_PPPoE             = (1 << 8),      ///< PPPoE Mode
   NM_DHA_SELECT        = (1 << 15),     ///< Destination Hardware Address Select
   NM_MR2_MASK          = (0x09<<8),     ///< @ref _NETMR2_ Mask value

  //NET4MR Bit Values
   NM_PB4_ALL           = (1 << 16),     ///< All PING4 request Block
   NM_TRSTB_V4          = (1 << 17),     ///< TCP RST packet for IPv4 Send Block
   NM_PARP_V4           = (1 << 18),     ///< ARP request for IPv4 before PINGv4 Replay
   NM_UNRB_V4           = (1 << 19),     ///< Unreachable Destination for IPv4 Block
   NM_NET4_MASK         = (0x0F << 16),  ///< @ref _NET4MR_ Mask value

  //NET4MR Bit Values
   NM_PB6_ALL           = (1 << 24),     ///< All PING6 request Block
   NM_TRSTB_V6          = (1 << 25),     ///< TCP RST packet for IPv6 Send Block
   NM_PARP_V6           = (1 << 26),     ///< ARP request for IPv6 before PINGv4 Replay
   NM_UNRB_V6           = (1 << 27),     ///< Unreachable Destination for IPv6 Block
   NM_NET6_MASK         = (0x0F << 24),  ///< @ref _NET6MR_ Mask value

   NM_MASK_ALL          = (0x0F0F0937)   ///< @ref netmode_type all mask value
}netmode_type;

/**
 * @ingroup DATA_TYPE
 * @brief Network Timeout for @ref _WIZCHIP_
 * @details @ref wiz_NetInfo is a structure type to configure or indicate the network timeout of @ref _WIZCHIP_.
 * @sa ctlnetwork(), CN_SET_TIMEOUT, CN_GET_TIMEOUT
 * @sa wizchip_settimeout(), wizchip_gettimeout()
 */
typedef struct wiz_NetTimeout_t
{
   uint8_t  s_retry_cnt;         ///< The default retry count of SOCKETn
   uint16_t s_time_100us;        ///< The retransmission time of SOCKETn (unit 100us)
   uint8_t  sl_retry_cnt;        ///< The retry count of SOCKET-less
   uint16_t sl_time_100us;       ///< The retransmission time of SOCKET-less (unit 100us)
}wiz_NetTimeout;

/**
 * @ingroup DATA_TYPE
 * @brief Destination Information for Network Service of @ref _WIZCHIP_
 * @details @ref wiz_NetInfo is a structure type to configure or indicate a destination information of network service.
 * @sa ctlnetservice(), CNS_ARP, CNS_PING
 * @sa IK_SOCKL_TOUT, IK_SOCKL_ARP4, IK_SOCKL_ARP6, IK_SOCKL_PING4, IK_SOCKL_PING6
 */
typedef struct wiz_IPAddress_t
{
   uint8_t ip[16];               ///< Destination IP Address. \n IPv4 index : 0 to 3, IPv6 index : 0 to 15
   uint8_t len;                  ///< Destination IP Address Length.\n IPv4 : 4, IPv6 : 16.
}wiz_IPAddress;

/**
 * @ingroup DATA_TYPE
 * @brief Prefix Information
 * @details @ref wiz_Prefix is a structure type to indicate a prefix information(0x03) of received RA message from a router.
 * @sa ctlnetservice(), CNS_SLAAC, IK_SOCKL_RS
 * @sa IK_SOCKL_TOUT, IK_SOCKL_RA, CNS_GET_PREFIX
 */
typedef struct wiz_Prefix_t
{
   uint8_t  len;                 ///< Prefix Length. \n It is used to set @ref _SUB6R_ to 1 as many as <i>len</i> from LSB bit.
   uint8_t  flag;                ///< Prefix Flag
   uint32_t valid_lifetime;      ///< Valid Lifetime
   uint32_t preferred_lifetime;  ///< Preferred Lifetime
   uint8_t  prefix[16];          ///< Prefix
}wiz_Prefix;

/**
 * @ingroup DATA_TYPE
 * @brief Destination Information & Destination Hardware Address for @ref CNS_ARP
 * @details @ref wiz_ARP is a structure type to set a destination IP address for ARP-request or \n
 *          indicate a destination hardware address in APR-reply.
 * @sa ctlnetservice(), CNS_ARP
 * @sa IK_SOCKL_TOUT, IK_SOCKL_ARP4, IK_SOCKL_ARP6
 */
typedef struct wiz_ARP_t
{
   wiz_IPAddress destinfo;       ///< Destination IP address for ARP-request
   uint8_t dha[6];               ///< Destination Hardware Address when ARP-reply is received from the destination.
}wiz_ARP;

/**
 * @ingroup DATA_TYPE
 * @brief Destination Information & Destination Hardware Address for @ref CNS_ARP
 * @details @ref wiz_PING is a structure type to set a ID, sequence number, destination IP address for PING-request.
 * @sa ctlnetservice(), CNS_PING
 * @sa IK_SOCKL_TOUT, IK_SOCKL_PING4, IK_SOCKL_PING6
 */
typedef struct wiz_PING_t
{
   uint16_t id;
   uint16_t seq;
   wiz_IPAddress destinfo;
}wiz_PING;

/**
 * @brief Registers call back functions for critical section.
 * @details @ref reg_wizchip_cris_cbfunc() is for basic I/O functions \n
 *           such as @ref WIZCHIP_READ(), @ref WIZCHIP_WRITE(), @ref WIZCHIP_READ_BUF() and @ref WIZCHIP_WRITE_BUF().
 * @param cris_en : callback function for critical section enter.
 * @param cris_ex : callback function for critical section exit.
 * @todo Register your functions for critical session.
 * @note If you do not register it, the @b empty default functions are called.\n
 *       @ref wizchip_cris_enter(), @ref wizchip_cris_exit()
 */
void reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void));


/**
 * @brief Registers call back functions for @ref _WIZCHIP_ select & deselect.
 * @details @ref reg_wizchip_cs_cbfunc() registers your functions to select & deselect @ref _WIZCHIP_
 * @param cs_sel : callback function to select @ref _WIZCHIP_
 * @param cs_desel : callback function to deselect @ref _WIZCHIP_
 * @todo Register your functions to select or deselect @ref _WIZCHIP_.
 * @note If you do not register it, the @b empty default functions are called. \n
 *       @ref wizchip_cs_select(), @ref wizchip_cs_deselect()
 */
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));

/// @cond DOXY_APPLY_CODE
#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_)
/// @endcond
/**
 * @brief Registers call back functions for BUS interface.
 * @details @ref reg_wizchip_bus_cbfunc() is for basic I/O functions \n
 *          such as @ref WIZCHIP_READ(), @ref WIZCHIP_WRITE(), @ref WIZCHIP_READ_BUF() and @ref WIZCHIP_WRITE_BUF().
 * @param bus_rd   : callback function to read 1 @ref iodata_t data 
 * @param bus_wd   : callback function to write 1 @ref iodata_t data
 * @param bus_rbuf : callback function to read @ref iodata_t data buf as many as <i>len</i>
 * @param bus_wbuf : callback function to write @ref iodata_t data buf as many as <i>len</i>
 * @todo Register your functions for data read & write through BUS interface.
 * @note If you do not register it,the default functions are called. \n
 *       @ref wizchip_bus_read(), @ref wizchip_bus_write(), @ref wizchip_bus_read_buf(), @ref wizchip_bus_write_buf().
 */
void reg_wizchip_bus_cbfunc(iodata_t (*bus_rd)(uint32_t addr), 
                            void (*bus_wd)(uint32_t addr, iodata_t wb),
                            void (*bus_rbuf)(uint32_t addr, iodata_t* buf, datasize_t len, uint8_t inc),
                            void (*bus_wbuf)(uint32_t addr, iodata_t* buf, datasize_t len, uint8_t inc) );
/// @cond DOXY_APPLY_CODE
#endif
/// @endcond

/// @cond DOXY_APPLY_CODE
#if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_)
/// @endcond
/**
 * @brief Registers call back functions for SPI interface.
 * @details @ref reg_wizchip_spi_cbfunc() is for for basic I/O functions \n
 * *        such as @ref WIZCHIP_READ(), @ref WIZCHIP_WRITE(), @ref WIZCHIP_READ_BUF() and @ref WIZCHIP_WRITE_BUF().
 * @param spi_rb : callback function to read 1 byte data using SPI
 * @param spi_wb : callback function to write 1 byte data using SPI
 * @param spi_rbuf : callback function to read byte data buf as many as <i>len</i>
 * @param spi_rbuf : callback function to write byte data buf as many as <i>len</i>
 * @todo Should register your functions for data read/write through SPI interface.
 * @note If you do not register it, the empty default function are called but these function are all null function.\n
 *        So, You should overwrite them or register your functions for replacing them.\n\n
 *        The empty default functions are @ref wizchip_spi_read(), @ref wizchip_spi_write(),@ref wizchip_spi_read_buf(), and @ref wizchip_spi_write_buf().
 */
void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), 
                            void (*spi_wb)(uint8_t wb),
                            void (*spi_rbuf)(uint8_t* buf, datasize_t len),
                            void (*spi_wbuf)(uint8_t* buf, datasize_t len) );
/// @cond DOXY_APPLY_CODE
#endif
/// @endcond


/**
 * @ingroup extra_functions
 * @brief Control @ref _WIZCHIP_.
 * @details @ref ctlwizchip() resets @ref _WIZCHIP_ & integrated Ethernet PHY,\n
 *          configures PHY mode, monitors PHY(Link,Speed,Half/Full/Auto), controls interrupt & mask and so on.
 * @param cwtype : Decides to the control type
 * @param arg : arg type is dependent on cwtype.
 * @return  0 : Success \n
 *         -1 : Fail because of invalid @ref ctlwizchip_type or unsupported @ref ctlwizchip_type
 * @note When you define @ref _PHY_IO_MODE_ to @ref _PHY_IO_MODE_PHYCR_,\n
 *       If you call @ref ctlwizchip(@ref CW_RESET_PHY), you should wait for stable reset time.\n\n
 *       ex> W6100 case \n
 *          <code>
 *            ctlwizchip(CW_RESET_PHY); \n
 *            // Need to wait stable reset time 60.3ms\n
 *            your_delay_function(60300);
 *          </code>
 */          
int8_t ctlwizchip(ctlwizchip_type cwtype, void* arg);

/**
 * @ingroup extra_functions
 * @brief Controls to network.
 * @details @ref ctlnetwork() controls to network environment, mode, timeout and so on.
 * @param cntype : Decides to the control type
 * @param arg : arg type is dependent on cntype.
 * @return -1 : Fail because of invalid @ref ctlnetwork_type or unsupported @ref ctlnetwork_type \n
 *          0 : Success      
 */          
int8_t ctlnetwork(ctlnetwork_type cntype, void* arg);


/**
 * @ingroup extra_functions
 * @brief Controls to network service.
 * @details Controls to network environment, mode, timeout and so on.
 * @param cnstype : Decides to the control type
 * @param arg : arg type is dependent on cnstype.
 * @return -1 : Fail because of invalid @ref ctlnetwork_type or unsupported @ref ctlnetwork_type \n
 *          0 : Success      
 */          
int8_t ctlnetservice(ctlnetservice_type cnstype, void* arg);


/* 
 * The following functions are implemented for internal use. 
 * but You can call these functions for code size reduction instead of ctlwizchip() and ctlnetwork().
 */
 
/**
 * @ingroup extra_functions
 * @brief Soft Reset @ref _WIZCHIP_.
 * @note It backs up the network information before reset, and restores after reset.
 * @sa ctlwizchip(), CW_RESET_WIZCHIP
 * @sa SYCR0_RST
 */ 
void   wizchip_sw_reset(void);

/**
 * @ingroup extra_functions
 * @brief Initializes SOCKETn TX/RX buffer size
 * @param txsize SOCKETn TX buffer sizes.\n If it is null, SOCKETn TX buffer is initialized as the default size 2KB.
 * @param rxsize SOCKETn RX buffer sizes.\n If it is null, SOCKETn RX buffer is initialized as the default size 2KB.
 * @return 0 : Success \n
 *        -1 : Fail. Invalid buffer size
 * @sa ctlwizchip(), CW_INIT_WIZCHIP
 * @sa _Sn_TX_BSR_, _Sn_RX_BSR_
 */
int8_t wizchip_init(uint8_t* txsize, uint8_t* rxsize);

/**  
 * @ingroup extra_functions
 * @brief Clear Interrupt of @ref _WIZCHIP_.
 * @param intr : @ref intr_kind value operated OR. It can type-cast to uint32_t.
 * @sa ctlwizchip(), CW_CLR_INTERRUPT, CW_GET_INTERRUPT, CW_SET_INTRMASK, CW_GET_INTRMASK
 * @sa wizchip_getintrerupt(), wizchip_setinterruptmask(), wizchip_getinterruptmask()
 */
void wizchip_clrinterrupt(intr_kind intr);

/** 
 * @ingroup extra_functions
 * @brief Get Interrupt of @ref _WIZCHIP_.
 * @return @ref intr_kind value operated OR. It can type-cast to uint32_t.
 * @sa ctlwizchip(), CW_GET_INTERRUPT, CW_CLR_INTERRUPT, CW_SET_INTRMASK, CW_GET_INTRMASK
 * @sa wizchip_clrinterrupt(), wizchip_setinterruptmask(), wizchip_getinterruptmask()
 */
intr_kind wizchip_getinterrupt(void);

/** 
 * @ingroup extra_functions
 * @brief Mask or Unmask Interrupt of @ref _WIZCHIP_.
 * @param intr : @ref intr_kind value operated OR. It can type-cast to uint32_t.
 * @sa ctlwizchip(), CW_SET_INTRMASK, CW_GET_INTRMASK, CW_CLR_INTERRUPT, CW_GET_INTERRUPT
 * @sa wizchip_getinterruptmask(), wizchip_clrinterrupt(), wizchip_getinterrupt()
 */
void wizchip_setinterruptmask(intr_kind intr);

/** 
 * @ingroup extra_functions
 * @brief Get Interrupt mask of @ref _WIZCHIP_.
 * @return The operated OR value of @ref intr_kind. It can type-cast to uint32_t.
 * @sa ctlwizchip(), CW_GET_INTRMASK, CW_SET_INTRMASK, CW_CLR_INTERRUPT, CW_GET_INTERRUPT
 * @sa wizchip_setinterruptmask(), wizchip_clrinterrupt(), wizchip_getinterrupt()
 */
intr_kind wizchip_getinterruptmask(void);

/** 
 * @ingroup extra_functions
 * @brief Get the Link status of integrated PHY.
 * @details @ref wizphy_getphylink() gets the link status of the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_. \n
 * @return : @ref PHY_LINK_ON or @ref PHY_LINK_OFF.
 * @sa ctlwizchip(), CW_GET_PHYLINK
 * @sa _PHY_IO_MODE_
 */
int8_t wizphy_getphylink(void);            ///< get the link status of phy in WIZCHIP. No use in W5100

/** 
 * @ingroup extra_functions
 * @brief Reset the integrated PHY.
 * @details @ref wizphy_reset() resets the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_. \n
 * @note In @ref _PHY_IO_MODE_PHYCR_, It needs a stable reset time. \n
 *       So you need to wait for the stable reset time.\n
 *       The stable reset time for each @ref _WIZCHIP_ maybe different.
 * @sa ctlwizchip(), CW_RESET_PHY
 * @sa _PHY_IO_MODE_
 */
void wizphy_reset(void);                   ///< Reset phy. Vailid only in W5500

/**
 * @ingroup extra_functions
 * @details @ref wizphy_setphyconf() set a operation mode of the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_.\n
 * @param phyconf : @ref wiz_PhyConf
 * @note The operation mode can be applied to Ethernet PHY after the Ethernet PHY is reset by @ref wizphy_reset().
 * @sa ctlwizchip(), CW_SET_PHYCONF, CW_GET_PHYCONF, CW_GET_PHYSTATUS, CW_RESET_PHY
 * @sa _PHY_IO_MODE_, wizphy_getphyconf(), wizphy_getphystatus(), wizphy_reset()
 */
void wizphy_setphyconf(wiz_PhyConf* phyconf);  

/**
 * @ingroup extra_functions
 * @brief Get the integrated Ethernet PHY operation mode.
 * @details @ref wizphy_getphyconf() gets a operation mode of the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_.\n
 * @param phyconf : @ref wiz_PhyConf
 * @note It gets just the configured value for PHY operation, not real PHY operation.\n
 *       To get real PHY operation, you can call @ref wizphy_getphystatus()
 * @sa ctlwizchip(), CW_GET_PHYCONF, CW_SET_PHYCONF, CW_GET_PHYSTATUS
 * @sa _PHY_IO_MODE_, wizphy_setphyconf(), wizphy_getphystatus()
 */
void wizphy_getphyconf(wiz_PhyConf* phyconf); 

/**
 * @ingroup extra_functions
 * @brief Get the real PHY operation status when link is established.
 * @details @ref wizphy_getphystatus() gets a operation mode of the integrated Ethernet PHY. \n
 * @param phyconf : @ref wiz_PhyConf
 * @sa ctlwizchip(), CW_GET_PHYSTATUS, CW_GET_PHYCONF, CW_SET_PHYCONF 
 * @sa wizphy_setphyconf(), wizphy_getphyconf() 
 */ 
void wizphy_getphystatus(wiz_PhyConf* phyconf);
 
/**
 * @ingroup extra_functions
 * @brief Set the power mode of integrated Ethernet PHY.
 * @details @ref wizphy_setphypmode() sets a power mode of the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_.\n
 * @param pmode @ref PHY_POWER_NORM or @ref PHY_POWER_DOWN
 * @note When the integrated Ethernet PHY enters in power down mode, \n
 *       the system clock of @ref _WIZCHIP_ is changed to the lowest speed. \n
 *       So, you should adjust the access time of @ref _WIZCHIP_ to the changed system clock.
 * @sa ctlwizchip(), CW_SET_PHYPOWMODE, CW_GET_PHYPOWMODE
 * @sa _PHY_IO_MODE_, wizphy_setphypmode(), wizphy_getphypmode()
 */   
void wizphy_setphypmode(uint8_t pmode);    

/**
 * @ingroup extra_functions
 * @brief get the power mode of integrated Ethernet PHY.
 * @details @ref wizphy_getphypmode() gets a power mode of the integrated Ethernet PHY \n
 *          through @ref _PHY_IO_MODE_PHYCR_ or @ref _PHY_IO_MODE_MII_.\n
 * @return @ref PHY_POWER_NORM or @ref PHY_POWER_DOWN
 * @note When the integrated Ethernet PHY enters in power down mode,\n
 *       the system clock of @ref _WIZCHIP_ is changed to the lowest speed. \n
 *       So, you should adjust the access time of @ref _WIZCHIP_ to the changed system clock.
 * @sa ctlwizchip(), CW_SET_PHYPOWMODE, CW_GET_PHYPOWMODE
 * @sa _PHY_IO_MODE_, wizphy_setphypmode(), wizphy_getphypmode()
 */   
int8_t wizphy_getphypmode(void);

/**
 * @ingroup extra_functions
 * @brief Set the network information for @ref _WIZCHIP_
 * @param pnetinfo : @ref wiz_NetInfo
 * @sa ctlnetwork(), CN_SET_NETINFO, CN_GET_NETINFO
 * @sa wizchip_getnetinfo()
 */
void wizchip_setnetinfo(wiz_NetInfo* pnetinfo);

/**
 * @ingroup extra_functions
 * @brief Get the network information of @ref _WIZCHIP_
 * @param pnetinfo : @ref wiz_NetInfo
 * @sa ctlnetwork(), CN_GET_NETINFO, CN_SET_NETINFO
 * @sa wizchip_setnetinfo()
 */
void wizchip_getnetinfo(wiz_NetInfo* pnetinfo);

/**
 * @ingroup extra_functions
 * @brief Set the network mode such as WOL, PPPoE, PING Block, and etc. 
 * @param netmode : @ref netmode_type.
 * @sa ctlnetwork(), CN_SET_NETMODE, CN_GET_NETMODE
 * @sa wizchip_getnetmode()
 */
void wizchip_setnetmode(netmode_type netmode);

/**
 * @ingroup extra_functions
 * @brief Get the network mode such as WOL, PPPoE, PING Block, and etc. 
 * @return @ref netmode_type.
 * @sa ctlnetwork(), CN_GET_NETMODE, CN_SET_NETMODE
 * @sa wizchip_setnetmode()
 */
netmode_type wizchip_getnetmode(void);

/**
 * @ingroup extra_functions
 * @brief Set retransmission time values and retry counts.
 * @param nettime : @ref wiz_NetTimeout. 
 * @sa ctlnetwork(), CN_SET_TIMEOUT, CN_GET_TIMEOUT
 * @sa wizchip_gettimeout()
 */
void wizchip_settimeout(wiz_NetTimeout* nettime);

/**
 * @ingroup extra_functions
 * @brief Get retransmission time values and retry counts.
 * @param nettime : @ref wiz_NetTimeout. 
 * @sa ctlnetwork(), CN_GET_TIMEOUT, CN_SET_TIMEOUT
 * @sa wizchip_settimeout()
 */
void wizchip_gettimeout(wiz_NetTimeout* nettime);

/**
 * @ingroup extra_functions
 * @brief ARP process.
 * @details @ref wizchip_arp() processes ARP. \n
 *          It sends the APR-request to destination and waits to receive the ARP-reply.
 * @param arp @ref wiz_ARP.\n
 *            It sets a destination IP address and indicates the destination hardware address.
 * @return 0 : success, destination hardware address is valid.\n
 *         -1 : fail. destination hardware address is invalid because timeout is occurred.\n
 * @sa ctlnetservice(), CNS_ARP
 */
int8_t wizchip_arp(wiz_ARP* arp);

/**
 * @ingroup extra_functions
 * @brief PING process.
 * @details @ref wizchip_ping() processes PING. \n
 *          It sends the PING-request to destination and waits to receive the PING-reply.
 * @param ping @ref wiz_PING, It sets a destination IP address, ID, SEQ of PING-request message
 * @return 0 : success, PING-reply is successfully received.\n
 *         -1 : fail. Timeout is occurred.\n
 * @sa ctlnetservice(), CNS_PING
 */
int8_t wizchip_ping(wiz_PING* ping);

/**
 * @ingroup extra_functions
 * @brief DAD(Duplcated Address Detection) process.
 * @details @ref wizchip_dad() detects the duplication of source IPv6 address.\n
 *          It sends a NA message for DAD to all-node multicasting address(FF02::01).
 * @param ipv6 : IPv6 address to be detected the duplication.
 * @return 0 : success, There is no duplicated address. \n
 *         -1 : fail. @ref _WIZCHIP_ source IP address to use is duplicated with a neighbor's one.
 * @sa ctlnetservice(), CNS_DAD
 */
int8_t wizchip_dad(uint8_t* ipv6);

/**
 * @ingroup extra_functions
 * @brief Stateless Address Auto Configuration(SLAAC) process.
 * @details @ref wizchip_slaac() get a prefix information from a router for SLAAC.\n
 *          It sends first a RS message to all-router and waits to receive a RS message with prefix information option from a router.
 * @param prefix @ref wiz_Prefix
 * @return 0 : success, RA message is successfully received, and <i>prefix</i> is valid.  \n
 *         -1 : fail. Timeout is occurred.
 * @note It is valid only when the prefix information type(0x03) of RA option received first.\n
 *       The prefix option should be in the order of prefix length, prefix flag, valid lifetime, default lifetime and prefix address. \n
 *       For more detail, Refer to @ref SLIR_RS.
 * @sa ctlnetservice(), CNS_SLAAC
 */
int8_t wizchip_slaac(wiz_Prefix* prefix);

/**
 * @ingroup extra_functions
 * @brief Unsolicited NA process.
 * @details @ref wizchip_unsolicited() updates the network information of @ref _WIZCHIP_ to neighbors.\n
 *          It sends a unsolicited NA message with @ref _LLAR_ or @ref _GUAR_ to neighbors \n
 *          in order to update the network information of @ref _WIZCHIP_.\n
 *          Because the unsolicited NA message have no reply, timeout is always occurred.
 * @return always 0. Timeout is occurred.
 * @sa ctlnetservice(), CNS_UNSOL
 */
int8_t wizchip_unsolicited(void);

/**
 * @ingroup extra_functions
 * @brief Get a prefix information of RA message from a router.
 * @details @ref wizchip_getprefix() get a prefix information of RA is periodically sent by a router. \n
 * @return 0 : success, a RS message is successfully received from a router.
 *         -1 : fail, a RS message is not received from a router yet.
 * @note It is valid only when the prefix information type(0x03) of RA option received first.\n
 *       The prefix option should be in the order of prefix length, prefix flag, valid lifetime, default lifetime and prefix address. \n
 *       For more detail, Refer to @ref SLIR_RS.
 * @sa ctlnetservice(), CNS_GET_PREFIX
 */
int8_t wizchip_getprefix(wiz_Prefix * prefix);


#ifdef __cplusplus
}
#endif

#endif   // _WIZCHIP_CONF_H_
