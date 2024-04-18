/*
* Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
* foss@huawei.com
*
* If distributed as part of the Linux kernel, the following license terms
* apply:
*
* * This program is free software; you can redistribute it and/or modify
* * it under the terms of the GNU General Public License version 2 and
* * only version 2 as published by the Free Software Foundation.
* *
* * This program is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* * GNU General Public License for more details.
* *
* * You should have received a copy of the GNU General Public License
* * along with this program; if not, write to the Free Software
* * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*
* Otherwise, the following license terms apply:
*
* * Redistribution and use in source and binary forms, with or without
* * modification, are permitted provided that the following conditions
* * are met:
* * 1) Redistributions of source code must retain the above copyright
* *    notice, this list of conditions and the following disclaimer.
* * 2) Redistributions in binary form must reproduce the above copyright
* *    notice, this list of conditions and the following disclaimer in the
* *    documentation and/or other materials provided with the distribution.
* * 3) Neither the name of Huawei nor the names of its contributors may
* *    be used to endorse or promote products derived from this software
* *    without specific prior written permission.
*
* * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef _RNIC_CTX_H_
#define _RNIC_CTX_H_

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "vos.h"
#include "PsLogdef.h"
#include "PsTypeDef.h"
#include "ImmInterface.h"
#include "ImsaRnicInterface.h"
#include "AtRnicInterface.h"
#include "RnicLinuxInterface.h"
#include "RnicTimerMgmt.h"
#include "RnicProcMsg.h"
#include "RnicConfigInterface.h"
#include "NVIM_Interface.h"
#include "TafNvInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  1 ????????????
*****************************************************************************/

/*****************************************************************************
  2 ??????
*****************************************************************************/

#define RNIC_MAC_ADDR_LEN               (6)                                     /* RNIC MAC??????????6???? */
#define RNIC_MAC_HDR_LEN                (14)

#define RNIC_MAX_PACKET                 (1536)                                  /* RNIC????????????????????1500???? */
#define RNIC_R_IMS_MAX_PACKET           (2048)                                  /* R_IMS RNIC????????????????????2000???? */

#define RNIC_IPV4_VERSION               (4)                                     /* IP??????IP V4?????? */
#define RNIC_IPV6_VERSION               (6)                                     /* IP??????IP V6?????? */

#define RNIC_IP_HEAD_VER_OFFSET_MASK    (0x0F)                                  /* IP?????????????????????? */
#define RNIC_IP_HEAD_DEST_ADDR_OFFSET   (4 * 4)                                 /* IP????????IP?????????? */

#define RNIC_IPV4_HDR_LEN               (20)                                    /* IPV4??????????*/
#define RNIC_IPV4_BROADCAST_ADDR        (0xFFFFFFFF)                            /* IPV4?????????? */

#define RNIC_DEFAULT_MTU                (1500)                                  /* RNIC??????MTU?? */

#define RNIC_ETH_TYPE_ARP               (0x0806)                                /* ????????????????ARP??????  */
#define RNIC_ETH_TYPE_IP                (0x0800)                                /* ????????????????IPv4??????  */
#define RNIC_ETH_TYPE_IPV6              (0x86DD)                                /* ????????????????IPv6??????  */
#define RNIC_ETH_TYPE                   (0x0001)                                /* ????????????ARP??????HardwareFormat??????????????????????  */
#define RNIC_ARP_REPLY_TYPE             (0x0002)                                /* ????????????ARP??????OpCode????????????ARP????????         */

#define RNIC_NET_ID_MAX_NUM             (RNIC_RMNET_ID_BUTT)
#define RNIC_MODEM_ID_MAX_NUM           (MODEM_ID_BUTT)

#define RNIC_MAX_IPV6_ADDR_LEN          (16)                                    /* IPV6???????? */

#define RNIC_MAX_DSFLOW_BYTE            (0xFFFFFFFF)                            /* RNIC?????????? */

#define RNIC_DIAL_DEMA_IDLE_TIME        (600)

#define RNIC_IPV4_ADDR_LEN              (4)                                     /* IPV4???????? */
#define RNIC_IPV6_ADDR_LEN              (16)                                    /* IPV6???????? */

#define RNIC_INVALID_SPE_PORT           (-1)

/* RabId????????????ModemId, 00????Modem0, 01????Modem1, 10????Modem1 */
#define RNIC_RABID_TAKE_MODEM_1_MASK    (0x40)                                  /* Rabid????Modem1?????? */
#define RNIC_RABID_TAKE_MODEM_2_MASK    (0x80)                                  /* Rabid????Modem2?????? */
#define RNIC_RABID_UNTAKE_MODEM_MASK    (0x3F)                                  /* Rabid????Modem?????? */

/* ????RABID?? */
#define RNIC_RAB_ID_MAX_NUM             (11)
#define RNIC_RAB_ID_OFFSET              (5)

/* Rab Id???????? */
#define RNIC_RAB_ID_MIN                 (5)

/* Rab Id???????? */
#define RNIC_RAB_ID_MAX                 (15)

#define RNIC_RAB_ID_INVALID             (0xFF)

#define RNIC_PDN_ID_MAX_NUM             (20)
#define RNIC_PDN_ID_OFFSET              (1)
#define RNIC_PDN_ID_MIN                 (1)
#define RNIC_PDN_ID_MAX                 (20)
#define RNIC_PDN_ID_INVALID             (0xFF)

#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
#define RNIC_NAPI_POLL_DEFAULT_WEIGHT   (16)                                    /* NAPI ????Poll???????????????????? */
#define RNIC_NAPI_POLL_MAX_WEIGHT       (64)                                    /* NAPI ????Poll???????????????????????????? */
#define RNIC_POLL_QUEUE_MAX_LEN         (2560)                                  /* NAPI Poll?????????????? */
#endif

#define RNIC_IS_MODEMID_VALID(usModemId)\
            ((usModemId) < MODEM_ID_BUTT)

#define RNIC_PDN_ID_IS_VALID(ucPdnId) \
            (((ucPdnId) >= RNIC_PDN_ID_MIN) && ((ucPdnId) <= RNIC_PDN_ID_MAX))

#define RNIC_RAB_ID_IS_VALID(ucRabId) \
            (((ucRabId) >= RNIC_RAB_ID_MIN) && ((ucRabId) <= RNIC_RAB_ID_MAX))

#define RNIC_RMNET_IS_VALID(RmNetId) \
            ((RmNetId) < RNIC_RMNET_ID_BUTT)

#ifdef CONFIG_VOWIFI_NEW_FRW
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
#define RNIC_RMNET_R_IS_VALID(RmNetId) \
            ((RNIC_RMNET_ID_R_IMS00 == RmNetId) || (RNIC_RMNET_ID_R_IMS10 == RmNetId))
#else
#define RNIC_RMNET_R_IS_VALID(RmNetId) \
            (RNIC_RMNET_ID_R_IMS00 == RmNetId)
#endif
#endif

#define RNIC_GET_IP_VERSION(ucFirstData) \
            (((ucFirstData) >> 4) & (RNIC_IP_HEAD_VER_OFFSET_MASK))

/* ????IPV4????PDP??????PDNID */
#define RNIC_GET_SPEC_NET_IPV4_PDNID(index)         (RNIC_GET_SPEC_NET_PDP_CTX(index)->stIpv4PdpInfo.ucPdnId)

/* ????IPV6????PDP??????PDNID */
#define RNIC_GET_SPEC_NET_IPV6_PDNID(index)         (RNIC_GET_SPEC_NET_PDP_CTX(index)->stIpv6PdpInfo.ucPdnId)

/* ????????Modem??PDNID???? */
#define RNIC_GET_SPEC_MODEM_PDNID_INFO()            (&g_stRnicCtx.stPdnIdInfo)

/* ????PDNID??????????ID */
#define RNIC_GET_RM_NET_ID_BY_PDN_ID(PDNId) \
            (RNIC_GET_SPEC_MODEM_PDNID_INFO()->aucRmNetId[PDNId - RNIC_PDN_ID_OFFSET])

#define RNIC_SET_FLOW_CTRL_STATUS(status, index)    (g_stRnicCtx.astSpecCtx[index].enFlowCtrlStatus = (status))
#define RNIC_GET_FLOW_CTRL_STATUS(index)            (g_stRnicCtx.astSpecCtx[index].enFlowCtrlStatus)

/* ???????????????????????? */
#define RNIC_GET_SPEC_NET_CTX(index)                (&(g_stRnicCtx.astSpecCtx[index]))

/* ??????????????PDP?????????? */
#define RNIC_GET_SPEC_NET_PDP_CTX(index)            (&(g_stRnicCtx.astSpecCtx[index].stPdpCtx))

/* ????IPV4????PDP??????RABID */
#define RNIC_GET_SPEC_NET_IPV4_RABID(index)         (RNIC_GET_SPEC_NET_PDP_CTX(index)->stIpv4PdpInfo.ucRabId)

/* ????IPV6????PDP??????RABID */
#define RNIC_GET_SPEC_NET_IPV6_RABID(index)         (RNIC_GET_SPEC_NET_PDP_CTX(index)->stIpv6PdpInfo.ucRabId)

/* ????IPV4????PDP??????RABID */
#define RNIC_GET_SPEC_NET_IPV4_REG_STATE(index)     (RNIC_GET_SPEC_NET_PDP_CTX(index)->stIpv4PdpInfo.enRegStatus)

/* ?????????????????? */
#define RNIC_GET_DIAL_MODE()                        (g_stRnicCtx.stDialMode.enDialMode)

/* ?????????????????? */
#define RNIC_GET_UL_CTX_ADDR(index)                 (&g_stRnicCtx.astSpecCtx[index].stUlCtx)

/* ?????????????????? */
#define RNIC_GET_DL_CTX_ADDR(index)                 (&g_stRnicCtx.astSpecCtx[index].stDlCtx)

/* ??????????????????modem type */
#define RNIC_GET_SPEC_NET_MODEM_TYPE(index)         (g_stRnicCtx.astSpecCtx[index].enModemType)

/* ????????Modem??RABID???? */
#define RNIC_GET_SPEC_MODEM_RABID_INFO(index)       (&g_stRnicCtx.astRabIdInfo[index])

/* ???????????????????? */
#define RNIC_GET_SPEC_NET_PRIV_PTR(index)           (g_stRnicCtx.astSpecCtx[index].pstPriv)

/* ????????ID??????ModemId */
#define RNIC_GET_MODEM_ID_BY_NET_ID(index)          (g_astRnicManageTbl[index].enModemId)

/* ????????ID????????MAC???? */
#define RNIC_GET_DST_MAC_ADDR(index)                (g_astRnicManageTbl[index].stIpv4Ethhead.aucEtherDhost)

/* ????????ID??????MAC?? */
#define RNIC_GET_ETH_HDR_ADDR(index)                (&(g_astRnicManageTbl[index].stIpv4Ethhead))

/* ??????????IPF???? */
#define RNIC_GET_IPF_MODE()                         (g_stRnicCtx.ucIpfMode)

#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
#define RNIC_GET_Net_API()                          (g_stRnicCtx.ucNetInterfaceMode)
#define RNIC_GET_NAPI_WEIGHT()                      (g_stRnicCtx.ucNapiPollWeight)
#define RNIC_GET_PollBuff_QUE()                     (&(g_stRnicCtx.stPollBuffQue))
#endif

/*****************************************************************************
  3 ????????
*****************************************************************************/


enum RNIC_NETCARD_STATUS_TYPE_ENUM
{
    RNIC_NETCARD_STATUS_CLOSED,                                                 /* RNIC?????????? */
    RNIC_NETCARD_STATUS_OPENED,                                                 /* RNIC?????????? */
    RNIC_NETCARD_STATUS_BUTT
};
typedef VOS_UINT8 RNIC_NETCARD_STATUS_ENUM_UINT8;


enum RNIC_PDP_REG_STATUS_ENUM
{
    RNIC_PDP_REG_STATUS_DEACTIVE,                                               /* ???????? */
    RNIC_PDP_REG_STATUS_ACTIVE,                                                 /* ???????? */
    RNIC_PDP_REG_STATUS_BUTT
};
typedef VOS_UINT32 RNIC_PDP_REG_STATUS_ENUM_UINT32;


enum RNIC_FLOW_CTRL_STATUS_ENUM
{
    RNIC_FLOW_CTRL_STATUS_STOP          = 0x00,                                 /* ???????? */
    RNIC_FLOW_CTRL_STATUS_START         = 0x01,                                 /* ???????? */
    RNIC_FLOW_CTRL_STATUS_BUTT          = 0xFF
};
typedef VOS_UINT32 RNIC_FLOW_CTRL_STATUS_ENUM_UINT32;


enum RNIC_IPF_MODE_ENUM
{
    RNIC_IPF_MODE_INT  = 0x00,                                                   /* ?????????? */
    RNIC_IPF_MODE_THRD = 0x01,                                                   /* ?????????? */
    RNIC_IPF_MODE_BUTT
};
typedef VOS_UINT8 RNIC_IPF_MODE_ENUM_UINT8;

#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)

enum RNIC_NET_IF_ENUM
{
    RNIC_NET_IF_NETRX  = 0x00,                                                   /* Net_rx */
    RNIC_NET_IF_NAPI   = 0x01,                                                   /* Napi */
    RNIC_NET_IF_BUTT
};
typedef VOS_UINT8 RNIC_NET_IF_ENUM_UINT8;
#endif

/*****************************************************************************
  4 ????????????
*****************************************************************************/


/*****************************************************************************
  5 ??????????
*****************************************************************************/


/*****************************************************************************
  6 ????????
*****************************************************************************/


/*****************************************************************************
  7 STRUCT????
*****************************************************************************/


typedef struct
{
    VOS_UINT8                           aucEtherDhost[RNIC_MAC_ADDR_LEN];       /* destination ethernet address */
    VOS_UINT8                           aucEtherShost[RNIC_MAC_ADDR_LEN];       /* source ethernet address */
    VOS_UINT16                          usEtherType;                            /* packet type ID */
    VOS_UINT8                           aucReserved[2];
}RNIC_ETH_HEADER_STRU;


typedef struct
{
    const VOS_CHAR                     *pucRnicNetCardName;                     /* ???????? */
    RNIC_ETH_HEADER_STRU                stIpv4Ethhead;                          /* IPV4???????? */
    RNIC_ETH_HEADER_STRU                stIpv6Ethhead;                          /* IPV6???????? */
    MODEM_ID_ENUM_UINT16                enModemId;                              /* Modem Id */
    RNIC_RMNET_ID_ENUM_UINT8            enRmNetId;                              /* ????ID */
    VOS_UINT8                           aucReserved[5];
}RNIC_NETCARD_ELEMENT_TAB_STRU;


typedef struct
{
    struct net_device                  *pstDev;                                 /* ????????Linux???????????????????? */
    struct net_device_stats             stStats;                                /* Linxu?????????????????????????? */
    RNIC_NETCARD_STATUS_ENUM_UINT8      enStatus;                               /* ???????????????? */
    RNIC_RMNET_ID_ENUM_UINT8            enRmNetId;                              /* ??????????????ID */
    VOS_UINT8                           aucRsv[6];                              /* ???? */
#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
    struct napi_struct                  stNapi;                                 /* Linxu??????????NAPI???? */
#endif
}RNIC_NETCARD_DEV_INFO_STRU;


typedef struct
{
    VOS_UINT32                          OP_EnableDestAddr      : 1;             /* ????????IP???????????????? */
    VOS_UINT32                          OP_EnableDestMask      : 1;             /* ???????????????????????????? */
    VOS_UINT32                          OP_EnableDestPortLow   : 1;             /* ?????????????????????????????? */
    VOS_UINT32                          OP_EnableDestPortHigh  : 1;             /* ?????????????????????????????? */
    VOS_UINT32                          OP_EnableSrcPortLow    : 1;             /* ???????????????????????????? */
    VOS_UINT32                          OP_EnableSrcPortHigh   : 1;             /* ???????????????????????????? */
    VOS_UINT32                          OP_EnableReserved      : 26;            /* ???? */
    VOS_UINT32                          ulDestAddr;                             /* ????IP???? */
    VOS_UINT32                          ulDestMask;                             /* ???????????? */
    VOS_UINT16                          usDestPortLow;                          /* ?????????????? */
    VOS_UINT16                          usDestPortHigh;                         /* ?????????????? */
    VOS_UINT16                          usSrcPortLow;                           /* ???????????? */
    VOS_UINT16                          usSrcPortHigh;                          /* ???????????? */
}RNIC_FILTER_INFO_STRU;


typedef struct RNIC_FTI
{
    VOS_INT32                           ulFilterId;                             /* ????????????ID */
    RNIC_FILTER_INFO_STRU               stFilter;                               /* ???????????????? */
    struct RNIC_FTI                    *pNextItem;                              /* ???????????????? */
}RNIC_FILTER_LST_STRU;


typedef struct
{
    RNIC_PDP_REG_STATUS_ENUM_UINT32     enRegStatus;                            /* ??????PDP?????????????? */

    VOS_UINT8                           ucRabId;                                /* ???????? */
    /* Added by l60609 for L-C??????????, 2014-01-06, Begin */
    VOS_UINT8                           ucPdnId;
    /* Added by l60609 for L-C??????????, 2014-01-06, End */

    VOS_UINT8                           aucReserved[2];
    VOS_UINT32                          ulIpv4Addr;                             /* IP???? */
}RNIC_IPV4_PDP_INFO_STRU;


typedef struct
{
    RNIC_PDP_REG_STATUS_ENUM_UINT32     enRegStatus;                            /* ??????PDP?????????????? */

    /* Modified by l60609 for L-C??????????, 2014-01-06, Begin */
    VOS_UINT8                           ucRabId;                                /* ???????? */
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           aucReserved[2];
    /* Modified by l60609 for L-C??????????, 2014-01-06, End */

    VOS_UINT8                           aucIpv6Addr[RNIC_MAX_IPV6_ADDR_LEN];    /* ??AT??????IPV6????????????????":" */
}RNIC_IPV6_PDP_INFO_STRU;


typedef struct
{
    RNIC_PDP_REG_STATUS_ENUM_UINT32     enRegStatus;                            /* ??????PDP?????????????? */

    /* Modified by l60609 for L-C??????????, 2014-01-06, Begin */
    VOS_UINT8                           ucRabId;                                /* ???????? */
    VOS_UINT8                           ucPdnId;
    VOS_UINT8                           aucReserved[2];
    /* Modified by l60609 for L-C??????????, 2014-01-06, End */

    VOS_UINT32                          ulIpv4Addr;                             /* IP???? */
    VOS_UINT8                           aucIpv6Addr[RNIC_MAX_IPV6_ADDR_LEN];    /* ??AT??????IPV6????????????????":" */
}RNIC_IPV4V6_PDP_INFO_STRU;


typedef struct
{
    RNIC_IPV4_PDP_INFO_STRU             stIpv4PdpInfo;                          /* IPV4??PDP???? */
    RNIC_IPV6_PDP_INFO_STRU             stIpv6PdpInfo;                          /* IPV6??PDP???? */
    RNIC_IPV4V6_PDP_INFO_STRU           stIpv4v6PdpInfo;                        /* IPV4V6??PDP???? */
    IMM_ZC_HEAD_STRU                    stImsQue;
    /* ????????????????2???????????? */
    RNIC_FILTER_LST_STRU               *pstFilterList;                          /* ?????????????? */
    VOS_UINT8                          *pucFragmBuf;                            /* ????IP?????????? */
} RNIC_PDP_CTX_STRU;


typedef struct
{
    RNIC_RMNET_ID_ENUM_UINT8            aucRmNetId[RNIC_RAB_ID_MAX_NUM];        /* RABID??????????ID */
    VOS_UINT8                           aucReserved[5];
}RNIC_RABID_INFO_STAU;

/* Modified by l60609 for L-C??????????, 2014-01-06, Begin */

typedef struct
{
    RNIC_RMNET_ID_ENUM_UINT8            aucRmNetId[RNIC_PDN_ID_MAX_NUM];        /* PDNID??????????ID */
    VOS_UINT8                           aucReserved[4];
}RNIC_PDNID_INFO_STAU;
/* Modified by l60609 for L-C??????????, 2014-01-06, End */

/*****************************************************************************
 ????????  : RNIC_DSFLOW_STATS_STRU
 ????????  : ????????????
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCurrentRecvRate;                      /* ??????????????????PDP??????2?????????????????????? */
    VOS_UINT32                          ulPeriodRecvPktNum;                     /* ??????????????????,?????????????????????????????????????????????????? */
    VOS_UINT32                          ulTotalRecvFluxLow;                     /* ???????????????????? */
    VOS_UINT32                          ulTotalRecvFluxHigh;                    /* ???????????????????? */

    VOS_UINT32                          ulCurrentSendRate;                      /* ??????????????????PDP??????2?????????????????????? */
    VOS_UINT32                          ulPeriodSendPktNum;                     /* ????????????????,?????????????????????????????????????????????????? */
    VOS_UINT32                          ulTotalSendFluxLow;                     /* ???????????????????? */
    VOS_UINT32                          ulTotalSendFluxHigh;                    /* ???????????????????? */

} RNIC_DSFLOW_STATS_STRU;


typedef struct
{
    RNIC_PDP_CTX_STRU                   stPdpCtx;                               /* RNIC??PDP?????? */
    RNIC_DSFLOW_STATS_STRU              stDsFlowStats;                          /* ???????? */
    RNIC_FLOW_CTRL_STATUS_ENUM_UINT32   enFlowCtrlStatus;                       /* ???????? */
    MODEM_ID_ENUM_UINT16                enModemId;                              /* ????????????modem */
    RNIC_MODEM_TYPE_ENUM_UINT8          enModemType;                            /* ????????????????????????modem??????????modem */
    RNIC_RMNET_ID_ENUM_UINT8            enRmNetId;                              /* ??????????????ID */
    IMSA_RNIC_IMS_RAT_TYPE_ENUM_UINT8   enRatType;                              /* ?????? */
    VOS_UINT8                           aucReserved[7];                         /* ???? */
    VOS_INT32                           lSpePort;                               /* SPE???????? */
    VOS_UINT32                          ulIpfPortFlg;                           /* SPE????IPF???? */

    /* ??????????????Netcard???????????????? */
    RNIC_NETCARD_DEV_INFO_STRU         *pstPriv;                                /* ???????????? */
}RNIC_SPEC_CTX_STRU;


typedef struct
{
    /****** RNIC???????????????????? ******/
    RNIC_SPEC_CTX_STRU                  astSpecCtx[RNIC_NET_ID_MAX_NUM];        /* ?????????????????? */

    RNIC_RABID_INFO_STAU                astRabIdInfo[RNIC_MODEM_ID_MAX_NUM];    /* RabId??????????ID */

    /* Modified by l60609 for L-C??????????, 2014-01-06, Begin */
    RNIC_PDNID_INFO_STAU                stPdnIdInfo;                            /* ????sdio pdn id??????????id*/
    /* Modified by l60609 for L-C??????????, 2014-01-06, End */

    /****** RNIC???????????? ******/
    RNIC_TIMER_CTX_STRU                 astTimerCtx[RNIC_MAX_TIMER_NUM];

    /****** RNIC?????????? ******/
#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
    IMM_ZC_HEAD_STRU                    stPollBuffQue;                          /* Poll???????? */
#endif

    RNIC_DIAL_MODE_STRU                 stDialMode;                             /* ???????? */
    VOS_UINT32                          ulTiDialDownExpCount;                   /* ?????????????????????????????? */

    VOS_UINT32                          ulSetTimer4WakeFlg;                     /* ????????Timer4???????? */

    VOS_SEM                             hResetSem;                              /* ??????????????????????????  */

    VOS_UINT8                           ucIpfMode;                              /* IPF????ADS??????????????, 0: ??????????(????)??1???????????? */
#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
    VOS_UINT8                           ucNetInterfaceMode;                     /* RNIC??????????????Linux????????????????????, 0: Net_rx(????)??1??NAPI???? */
    VOS_UINT8                           ucNapiPollWeight;                       /* RNIC????NAPI????????poll???????????? */
    VOS_UINT8                           aucRsv[5];
#else
    VOS_UINT8                           aucRsv[7];
#endif

}RNIC_CTX_STRU;
/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

extern RNIC_CTX_STRU                                g_stRnicCtx;
extern const RNIC_NETCARD_ELEMENT_TAB_STRU          g_astRnicManageTbl[];


/*****************************************************************************
  10 ????????
*****************************************************************************/
VOS_VOID RNIC_InitCtx(
    RNIC_CTX_STRU                      *pstRnicCtx
);
VOS_VOID RNIC_InitPdpCtx(
    RNIC_PDP_CTX_STRU                  *pstPdpCtx,
    VOS_UINT8                           ucRmNetId
);
VOS_VOID RNIC_InitDialMode(
    RNIC_DIAL_MODE_STRU                *pstDialMode
);
VOS_VOID RNIC_InitIpv4PdpCtx(
    RNIC_IPV4_PDP_INFO_STRU            *pstIpv4PdpCtx
);
VOS_VOID RNIC_InitIpv6PdpCtx(
    RNIC_IPV6_PDP_INFO_STRU            *pstIpv6PdpCtx
);
VOS_VOID RNIC_InitIpv4v6PdpCtx(
    RNIC_IPV4V6_PDP_INFO_STRU          *pstIpv4v6PdpCtx,
    VOS_UINT8                           ucRmNetId
);
RNIC_CTX_STRU* RNIC_GetRnicCtxAddr(VOS_VOID);
VOS_UINT32 RNIC_GetTiDialDownExpCount( VOS_VOID);
VOS_VOID RNIC_IncTiDialDownExpCount( VOS_VOID);
VOS_VOID RNIC_ClearTiDialDownExpCount( VOS_VOID);

VOS_UINT32 RNIC_GetCurrentUlRate(VOS_UINT8 ucIndex);
VOS_VOID RNIC_SetCurrentUlRate(
    VOS_UINT32                          ulULDataRate,
    VOS_UINT8                           ucRmNetId
);
VOS_UINT32 RNIC_GetCurrentDlRate(VOS_UINT8 ucIndex);
VOS_VOID RNIC_SetCurrentDlRate(
    VOS_UINT32                          ulDLDataRate,
    VOS_UINT8                           ucIndex
);
RNIC_DIAL_MODE_STRU* RNIC_GetDialModeAddr(VOS_VOID);
RNIC_PDP_CTX_STRU* RNIC_GetPdpCtxAddr(VOS_UINT8 ucIndex);

RNIC_TIMER_CTX_STRU*  RNIC_GetTimerAddr( VOS_VOID );
VOS_VOID RNIC_SetTimer4WakeFlg(VOS_UINT32 ulFlg);
VOS_UINT32 RNIC_GetTimer4WakeFlg(VOS_VOID);

VOS_VOID RNIC_InitRabidInfo(
    RNIC_RABID_INFO_STAU               *pstRabIdInfo
);

VOS_VOID RNIC_InitPdnIdInfo(
    RNIC_PDNID_INFO_STAU               *pstPdnIdInfo
);

RNIC_SPEC_CTX_STRU *RNIC_GetSpecNetCardCtxAddr(VOS_UINT8 ucIndex);
RNIC_RABID_INFO_STAU *RNIC_GetSpecModemRabIdInfoAddr(
    MODEM_ID_ENUM_UINT16                enModemId
);

VOS_VOID RNIC_ClearNetDsFlowStats(RNIC_RMNET_ID_ENUM_UINT8 enRmNetId);
RNIC_SPEC_CTX_STRU* RNIC_GetNetCntxtByRmNetId(RNIC_RMNET_ID_ENUM_UINT8 enRmNetId);
#if defined(CONFIG_BALONG_SPE)
RNIC_SPEC_CTX_STRU* RNIC_GetNetCntxtBySpePort(VOS_INT32 lPort);
#endif

VOS_VOID RNIC_ResetDialMode(
    RNIC_DIAL_MODE_STRU                *pstDialMode
);
VOS_VOID RNIC_InitResetSem(VOS_VOID);
VOS_SEM RNIC_GetResetSem(VOS_VOID);

VOS_VOID RNIC_InitIpfMode(
    RNIC_CTX_STRU                      *pstRnicCtx
);

#if (FEATURE_ON == FEATURE_RNIC_NAPI_GRO)
VOS_VOID RNIC_InitRnicNetInterfaceMode(
    RNIC_CTX_STRU                      *pstRnicCtx
);
VOS_VOID RNIC_CheckNetIfCfgValid(
    RNIC_CTX_STRU                      *pstRnicCtx
);
#endif

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of RnicCtx.h */

