/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                    Copyright 2004-2021 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                  NETWORK DEVICE CONFIGURATION FILE
*
*                                               LAN911x
*
* Filename : net_dev_cfg.c
* Version  : V3.06.01
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#define  MICRIUM_SOURCE
#define  NET_DEV_CFG_MODULE


/*
*********************************************************************************************************
*                                              INCLUDES
*********************************************************************************************************
*/

#include  <Source/net_cfg_net.h>
#include  <IF/net_if.h>
#include  <IF/net_if_ether.h>

/* ARM DAI 0385D AN385 page 10 */
#define LAN9115_DEVICE_REGS_BASE_ADDR 	(0x40200000u)
#define LAN9115_DEVICE_REGS_SIZE 	(0x00100000u)	


#ifdef  NET_IF_ETHER_MODULE_EN

/*
*********************************************************************************************************
*                                           LOCAL HOST CONFIGURATION
*********************************************************************************************************
*/

#if 0
NET_IF_CFG_LOCAL  NetIF_Cfg_Local = {
    1518,                                                       /* Size of large buffers.                               */
      10,                                                       /* Number of large buffers.                             */
     256,                                                       /* Size of small buffers.                               */
      10                                                        /* Number of small buffers.                             */
};
#endif


/*
*********************************************************************************************************
*                                            LAN911x 0 CONFIGURATION
*********************************************************************************************************
*/


NET_DEV_CFG_ETHER  NetDev_Cfg_LAN911x_0 = {

    NET_IF_MEM_TYPE_MAIN,           /* Desired receive  buffer memory pool type :                                       */
                                    /*   NET_IF_MEM_TYPE_MAIN        buffers allocated from main memory                 */
                                    /*   NET_IF_MEM_TYPE_DEDICATED   buffers allocated from (device's) dedicated memory */
    1518u,                          /* Desired size      of device's large receive  buffers (in octets) [see Note #2].  */
      50u,                          /* Desired number    of device's large receive  buffers             [see Note #3a]. */
       4u,                          /* Desired alignment of device's       receive  buffers (in octets) [see Note #4].  */
       0u,                          /* Desired offset from base receive  index, if needed   (in octets) [see Note #5a]. */


    NET_IF_MEM_TYPE_MAIN,           /* Desired transmit buffer memory pool type :                                       */
                                    /*   NET_IF_MEM_TYPE_MAIN        buffers allocated from main memory                 */
                                    /*   NET_IF_MEM_TYPE_DEDICATED   buffers allocated from (device's) dedicated memory */
    1614u,                          /* Desired size      of device's large transmit buffers (in octets) [see Note #2].  */
      25u,                          /* Desired number    of device's large transmit buffers             [see Note #3b]. */
     152u,                          /* Desired size      of device's small transmit buffers (in octets) [see Note #2].  */
      25u,                          /* Desired number    of device's small transmit buffers             [see Note #3b]. */
       4u,                          /* Desired alignment of device's       transmit buffers (in octets) [see Note #4].  */
       0u,                          /* Desired offset from base transmit index, if needed   (in octets) [see Note #5b]. */


    0x00000000u,                    /* Base address   of dedicated memory, if available.                                */
             0u,                    /* Size           of dedicated memory, if available (in octets).                    */


    NET_DEV_CFG_FLAG_NONE,          /* Desired option flags, if any (see Note #6).                                      */


             1u,                    /* Desired number of device's receive  descriptors (see Note #7).                   */
             1u,                    /* Desired number of device's transmit descriptors.                                 */


    	LAN9115_DEVICE_REGS_BASE_ADDR,                    /* Base address   of device's hardware/registers.                                   */

	LAN9115_DEVICE_REGS_BASE_ADDR,                    /* Size           of device's data bus (in bits), if available.                     */


   "00:50:C2:25:60:02",             /* Desired device hardware address; may be NULL address or string ...               */
                                    /* ... if  device hardware address configured or set at run-time.                   */
};

const  NET_PHY_CFG_ETHER  NetPhy_Cfg_Ether_1 = {
    1u,                             /* Phy bus address.                                                                 */
    NET_PHY_BUS_MODE_MII,           /* Phy bus mode.                                                                    */
    NET_PHY_TYPE_INT,               /* Phy type.                                                                        */
    NET_PHY_SPD_AUTO,               /* Auto-Negotiation determines link speed.                                          */
    NET_PHY_DUPLEX_AUTO,            /* Auto-Negotiation determines link duplex.                                         */
};


/*
*********************************************************************************************************
*                                            LAN911x 1 CONFIGURATION
*********************************************************************************************************
*/

NET_DEV_CFG_ETHER  NetDev_Cfg_LAN911x_1 = {

    NET_IF_MEM_TYPE_MAIN,           /* Desired receive  buffer memory pool type :                                       */
                                    /*   NET_IF_MEM_TYPE_MAIN        buffers allocated from main memory                 */
                                    /*   NET_IF_MEM_TYPE_DEDICATED   buffers allocated from (device's) dedicated memory */
    1518u,                          /* Desired size      of device's large receive  buffers (in octets) [see Note #2].  */
      50u,                          /* Desired number    of device's large receive  buffers             [see Note #3a]. */
       4u,                          /* Desired alignment of device's       receive  buffers (in octets) [see Note #4].  */
       0u,                          /* Desired offset from base receive  index, if needed   (in octets) [see Note #5a]. */


    NET_IF_MEM_TYPE_MAIN,           /* Desired transmit buffer memory pool type :                                       */
                                    /*   NET_IF_MEM_TYPE_MAIN        buffers allocated from main memory                 */
                                    /*   NET_IF_MEM_TYPE_DEDICATED   buffers allocated from (device's) dedicated memory */
    1614u,                          /* Desired size      of device's large transmit buffers (in octets) [see Note #2].  */
      25u,                          /* Desired number    of device's large transmit buffers             [see Note #3b]. */
     152u,                          /* Desired size      of device's small transmit buffers (in octets) [see Note #2].  */
      25u,                          /* Desired number    of device's small transmit buffers             [see Note #3b]. */
       4u,                          /* Desired alignment of device's       transmit buffers (in octets) [see Note #4].  */
       0u,                          /* Desired offset from base transmit index, if needed   (in octets) [see Note #5b]. */


    0x00000000u,                    /* Base address   of dedicated memory, if available.                                */
             0u,                    /* Size           of dedicated memory, if available (in octets).                    */


    NET_DEV_CFG_FLAG_NONE,          /* Desired option flags, if any (see Note #6).                                      */


             1u,                    /* Desired number of device's receive  descriptors (see Note #7).                   */
             1u,                    /* Desired number of device's transmit descriptors.                                 */


    0xFFFFFFFFu,                    /* Base address   of device's hardware/registers.                                   */

             0u,                    /* Size           of device's data bus (in bits), if available.                     */


   "00:50:C2:25:60:02",             /* Desired device hardware address; may be NULL address or string ...               */
                                    /* ... if  device hardware address configured or set at run-time.                   */
};



#endif /* NET_IF_ETHER_MODULE_EN */
