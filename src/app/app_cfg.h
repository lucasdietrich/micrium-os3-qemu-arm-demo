/*
 * Copyright (c) 2022 Lucas Dietrich <ld.adecy@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

#include <stdio.h>

/* TODO Move */
#define APP_TRACE_DBG(x) printf x




#define CLK_OS_CFG_TASK_PRIO 13
#define CLK_OS_CFG_TASK_STK_SIZE 512

#define DNSc_OS_CFG_INSTANCE_TASK_PRIO 		13
#define DNSc_OS_CFG_INSTANCE_TASK_STK_SIZE	512

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_FS_EN               DEF_ENABLED                /* Enable/disable the file system initialization code.  */


/*
*********************************************************************************************************
*                                          FS CONFIGURATION
*
* Note(s) : This section define various prerprocessor constant used by the example initialization code
*           located in fs_app.c to configure the file system.
*********************************************************************************************************
*/

#define  APP_CFG_FS_DEV_CNT          3                          /* Maximum number of opened devices.                    */
#define  APP_CFG_FS_VOL_CNT          3                          /* Maximum number of opened volumes.                    */
#define  APP_CFG_FS_FILE_CNT         5                          /* Maximum number of opened files.                      */
#define  APP_CFG_FS_DIR_CNT          5                          /* Maximum number of opened directories.                */
#define  APP_CFG_FS_BUF_CNT          (4 * APP_CFG_FS_VOL_CNT)   /* Internal buffer count.                               */
#define  APP_CFG_FS_DEV_DRV_CNT      3                          /* Maximum number of different device drivers.          */
#define  APP_CFG_FS_WORKING_DIR_CNT  5                          /* Maximum number of active working directories.        */
#define  APP_CFG_FS_MAX_SEC_SIZE     512                        /* Maximum sector size supported.                       */

#define  APP_CFG_FS_IDE_EN           DEF_DISABLED               /* Enable/disable the IDE\CF initialization.            */
#define  APP_CFG_FS_MSC_EN           DEF_DISABLED               /* Enable/disable the MSC initialization.               */
#define  APP_CFG_FS_NAND_EN          DEF_DISABLED               /* Enable/disable the NAND initialization.              */
#define  APP_CFG_FS_NOR_EN           DEF_DISABLED               /* Enable/disable the NOR initialization.               */
#define  APP_CFG_FS_RAM_EN           DEF_ENABLED                /* Enable/disable the RAMDisk initialization.           */
#define  APP_CFG_FS_SD_EN            DEF_DISABLED               /* Enable/disable the SD (SPI) initialization.          */
#define  APP_CFG_FS_SD_CARD_EN       DEF_DISABLED               /* Enable/disable the SD (Card) initialization.         */


/*
*********************************************************************************************************
*                                    RAMDISK DRIVER CONFIGURATION
*********************************************************************************************************
*/

#define  APP_CFG_FS_RAM_NBR_SECS     1024                       /* RAMDisk size in sectors.                             */
#define  APP_CFG_FS_RAM_SEC_SIZE     512                        /* RAMDisk sector size in byte.                         */

#endif /* _APP_CFG_H_ */