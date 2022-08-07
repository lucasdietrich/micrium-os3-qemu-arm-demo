#include <app.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include  <cpu_core.h>
#include  <lib_mem.h>

#include  <Source/net.h>
#include  <Source/net_ascii.h>
#include  <IF/net_if.h>
#include  <IF/net_if_ether.h>

#include  <IP/IPv4/net_ipv4.h>

#include <net_dev_cfg.h>

/* 
uC-TCP-IP/Examples/Init/init_ether.c
*/

void *NetDev_API_TemplateEtherDMA = NULL;
void *NetDev_BSP_BoardDev_Nbr = NULL;
// void *NetDev_Cfg_Ether_1 = NULL;
void *NetPhy_API_Generic = NULL;
// void *NetPhy_Cfg_Ether_1 = NULL;

int app_net_init(void)
{
    NET_IF_NBR      if_nbr;
    NET_ERR         err_net;

    err_net = Net_Init(&NetRxTaskCfg,                           /* See Note #2.                                         */
                       &NetTxDeallocTaskCfg,
                       &NetTmrTaskCfg);
    if (err_net != NET_ERR_NONE) {
        return (DEF_FAIL);
    }
                                                                /* -------------- ADD ETHERNET INTERFACE -------------- */
                                                                /* See Note #3.                                         */
    if_nbr = NetIF_Add((void    *)&NetIF_API_Ether,             /* See Note #3b.                                        */
                       (void    *)&NetDev_API_TemplateEtherDMA, /* TODO Device driver API,    See Note #3c.             */
                       (void    *)&NetDev_BSP_BoardDev_Nbr,     /* TODO BSP API,              See Note #3d.             */
                       (void    *)&NetDev_Cfg_Ether_1,          /* TODO Device configuration, See Note #3e.             */
                       (void    *)&NetPhy_API_Generic,          /* TODO PHY driver API,       See Note #3f.             */
                       (void    *)&NetPhy_Cfg_Ether_1,          /* TODO PHY configuration,    See Note #3g.             */
                                  &err_net);
    if (err_net != NET_IF_ERR_NONE) {
        return (DEF_FAIL);
    }

                                                                /* ------------- START ETHERNET INTERFACE ------------- */
                                                                /* See Note #4.                                         */
    NetIF_Start(if_nbr, &err_net);                              /* Makes the interface ready to receive and transmit.   */
    if (err_net != NET_IF_ERR_NONE) {
        return (DEF_FAIL);
    }


#ifdef NET_IPv4_MODULE_EN

	NET_IPv4_ADDR addr_ipv4;
	NET_IPv4_ADDR msk_ipv4;
	NET_IPv4_ADDR gateway_ipv4;
                                                                /* --------- CONFIGURE IPV4 STATIC ADDRESSES ---------- */
                                                                /* For Dynamic IPv4 configuration, uC/DHCPc is required */

                                                                /* TODO Update IPv4 Addresses following your network ...*/
                                                                /* ... requirements.                                    */

                                                                /* See Note #5.                                         */
    NetASCII_Str_to_IP("10.10.10.64",                           /* Convert Host IPv4 string address to 32 bit address.  */
                       &addr_ipv4,
                        NET_IPv4_ADDR_SIZE,
                       &err_net);

    NetASCII_Str_to_IP("255.255.255.0",                         /* Convert IPv4 mask string to 32 bit address.          */
                       &msk_ipv4,
                        NET_IPv4_ADDR_SIZE,
                       &err_net);

    NetASCII_Str_to_IP("10.10.10.1",                            /* Convert Gateway string address to 32 bit address.    */
                       &gateway_ipv4,
                        NET_IPv4_ADDR_SIZE,
                       &err_net);

    NetIPv4_CfgAddrAdd(if_nbr,                                  /* Add a statically-configured IPv4 host address,   ... */
                       addr_ipv4,                               /* ... subnet mask, & default gateway to the        ... */
                       msk_ipv4,                                /* ... interface. See Note #6.                          */
                       gateway_ipv4,
                      &err_net);
    if (err_net != NET_IPv4_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif

    return DEF_OK;
}