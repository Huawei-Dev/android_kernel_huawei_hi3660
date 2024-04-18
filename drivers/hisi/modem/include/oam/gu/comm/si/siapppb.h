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



#ifndef __SIAPPPB_H__
#define __SIAPPPB_H__

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

/**/
/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "sitypedef.h"
#include "vos.h"
#include "TafOamInterface.h"
#include "UsimPsInterface.h"
#include "omnvinterface.h"

#pragma pack(4)

/*****************************************************************************
  2 ??????
*****************************************************************************/

#define SI_PB_PHONENUM_MAX_LEN          40    /*????????????????????????????????????*/
#define SI_PB_ALPHATAG_MAX_LEN          242   /*????????????????????????????????????*/

#define SI_PB_EMAIL_MAX_LEN             64    /*????????????????????????????????????*/

#define SI_PB_NUMBER_LEN                (20)  /* ????????????BCD???????? */

/* ????????????????5??????????????????16?? */
#define USIM_MAX_ECC_RECORDS    16
#define USIM_ECC_LEN            3

/*******************************************************************************
  3 ????????
*******************************************************************************/

typedef SI_UINT32  SI_PB_AlPHATAG_TYPE;
#define SI_PB_ALPHATAG_TYPE_UCS2_80     0x80            /*????????????????16-bit universal multiple-octet coded character set (ISO/IEC10646 [32])*/
#define SI_PB_ALPHATAG_TYPE_UCS2_81     0x81
#define SI_PB_ALPHATAG_TYPE_UCS2_82     0x82
#define SI_PB_ALPHATAG_TYPE_GSM         0x00            /*????????????????GSM 7 bit default alphabet (3GPP TS 23.038)*/
#define SI_PB_ALPHATAG_TYPE_UCS2        0x80

#define SI_PB_CONTENT_VALID             0x01
#define SI_PB_CONTENT_INVALID           0x00

#define SI_PB_LAST_TAG_TRUE             1

#define SI_PB_ALPHATAG_TRANSFER_TAG     0x1B

enum PB_FDN_CHECK_RSLT_ENUM
{
    PB_FDN_CHECK_SUCC                   = 0,
    PB_FDN_CHECK_NUM1_FAIL              = 1,
    PB_FDN_CHECK_NUM2_FAIL              = 2,
    PB_FDN_CHECK_BOTH_NUM_FAIL          = 3,
    PB_FDN_CHECK_RSLT_BUTT
};
typedef VOS_UINT32 PB_FDN_CHECK_RSLT_ENUM_UINT32;

/*****************************************************************************
 ??????    : SI_PB_REQ_ENUM_UINT32
 ????????  : PB????????????????
*****************************************************************************/
enum SI_PB_REQ_ENUM
{
    SI_PB_READ_REQ                      = 0,        /* ??????????????           */
    SI_PB_SET_REQ                       = 1,        /* ??????????????           */
    SI_PB_MODIFY_REQ                    = 2,        /* ??????????????           */
    SI_PB_DELETE_REQ                    = 3,        /* ??????????????           */
    SI_PB_QUERY_REQ                     = 4,        /* ??????????????           */
    SI_PB_ADD_REQ                       = 5,        /* ??????????????           */
    SI_PB_SEARCH_REQ                    = 6,        /* ??????????????           */
    SI_PB_SREAD_REQ                     = 7,        /* ??????????????????       */
    SI_PB_SMODIFY_REQ                   = 8,        /* ??????????????????       */
    SI_PB_SADD_REQ                      = 9,        /* ??????????????????       */
    SI_PB_UPDATE_AGOBAL                 = 10,       /* ????Acpu??????????????   */
    SI_PB_UPDATE_CURPB                  = 11,       /* ??????????????????       */
    SI_PB_FDN_CHECK_REQ                 = 12,       /* FDN????????????          */
    SI_PB_ECALL_QRY_REQ                 = 13,       /* ECALL????????????        */

    SI_PB_REQ_BUTT
};
typedef SI_UINT32 SI_PB_REQ_ENUM_UINT32;

/*****************************************************************************
 ??????    : SI_PB_CNF_ENUM_UINT32
 ????????  : PB??????????????????????
*****************************************************************************/
enum SI_PB_CNF_ENUM
{
    SI_PB_EVENT_INFO_IND                = 0,    /* ??????????????????           */
    SI_PB_EVENT_READ_CNF                = 1,    /* ??????????????????           */
    SI_PB_EVENT_QUERY_CNF               = 2,    /* ??????????????????           */
    SI_PB_EVENT_SET_CNF                 = 3,    /* ??????????????????           */
    SI_PB_EVENT_ADD_CNF                 = 4,    /* ??????????????????           */
    SI_PB_EVENT_MODIFY_CNF              = 5,    /* ??????????????????           */
    SI_PB_EVENT_DELETE_CNF              = 6,    /* ??????????????????           */
    SI_PB_EVENT_SEARCH_CNF              = 7,    /* ??????????????????           */
    SI_PB_EVENT_SREAD_CNF               = 8,    /* ??????????????????????       */
    SI_PB_EVENT_SADD_CNF                = 9,    /* ??????????????????????       */
    SI_PB_EVENT_SMODIFY_CNF             = 10,   /* ??????????????????????       */
    SI_PB_EVENT_FDNCHECK_CNF            = 11,   /* FDN????????????????          */
    SI_PB_EVENT_ECALLQUERY_CNF          = 12,   /* ECALL????????????????        */
    SI_PB_EVENT_ECALLINIT_IND           = 13,   /* ECALL??????????????????      */
    SI_PB_EVENT_BUTT
};

typedef VOS_UINT32 SI_PB_CNF_ENUM_UINT32;

/*****************************************************************************
 ??????    : SI_PB_STORAGE_ENUM_UINT32
 ????????  : PB??????????????
*****************************************************************************/
enum SI_PB_STORAGE_ENUM
{
    SI_PB_STORAGE_UNSPECIFIED           = 0,       /*??????????????????????????????????????*/
    SI_PB_STORAGE_SM                    = 1,       /*????????????ADN*/
    SI_PB_STORAGE_ME                    = 2,       /*????????????NV*/
    SI_PB_STORAGE_BOTH                  = 3,       /*????????????ADN??NV*/
    SI_PB_STORAGE_ON                    = 4,       /*????????????MSISDN*/
    SI_PB_STORAGE_FD                    = 5,       /*????????????FDN*/
    SI_PB_STORAGE_EC                    = 6,       /*????????????ECC*/
    SI_PB_STORAGE_BD                    = 7,       /*????????????BDN*/
    SI_PB_STORAGE_SD                    = 8,       /*????????????SDN*/
    SI_PB_STORAGE_BUTT
};

typedef VOS_UINT32 SI_PB_STORAGE_ENUM_UINT32;

/*****************************************************************************
  4 ????????????
*****************************************************************************/

/*****************************************************************************
  5 STRUCT????
*****************************************************************************/
/* ?????????????????? */
enum PB_NUMBER_TYPE_ENUM
{
    PB_NUMBER_TYPE_NORMAL        = 129,            /* ???????????? */
    PB_NUMBER_TYPE_INTERNATIONAL = 145,            /* ???????????? */
    PB_NUMBER_TYPE_BUTT
};

/*??????????????????????????????*/
typedef struct
{
    SI_UINT16                InUsedNum;           /*??????????????*/
    SI_UINT16                TotalNum;            /*??????????*/
    SI_UINT16                TextLen;             /*??????text??????????*/
    SI_UINT16                NumLen;              /*??????number??????????*/
    SI_UINT16                EMAILTextLen;        /*Emai????*/
    SI_UINT16                ANRNumberLen;        /*ANR????????*/
}SI_PB_EVENT_QUERY_CNF_STRU;

typedef struct
{
    SI_UINT16                ADNRecordNum ;              /*ADN????????*/
    SI_UINT16                ADNTextLen ;                /*ADN????????????*/
    SI_UINT16                ADNNumberLen;              /*ADN????????*/
    SI_UINT16                FDNRecordNum ;              /*FDN????????*/
    SI_UINT16                FDNTextLen ;                /*FDN????????????*/
    SI_UINT16                FDNNumberLen;              /*FDN????????*/
    SI_UINT16                BDNRecordNum ;              /*BDN????????*/
    SI_UINT16                BDNTextLen ;                /*BDN????????????*/
    SI_UINT16                BDNNumberLen;              /*BDN????????*/
    SI_UINT16                MSISDNRecordNum;            /*MSISDN????????*/
    SI_UINT16                MSISDNTextLen ;             /*MSISDN????????????*/
    SI_UINT16                MSISDNNumberLen;           /*MSISDN????????*/
    SI_UINT16                EMAILTextLen;              /*EMAIL????*/
    SI_UINT16                ANRNumberLen;              /*ANR????????*/
    SI_UINT16                FdnState;                   /*FDN????????*/
    SI_UINT16                BdnState;                   /*BDN????????*/
    SI_UINT16                CardType;                   /*????????0 SIM????1 USIM??*/
}SI_PB_EVENT_INFO_IND_STRU;

typedef struct
{
    SI_UINT16                InUsedNum;           /*??????????????*/
    SI_UINT16                TotalNum;            /*??????????*/
    SI_UINT16                TextLen;             /*??????text??????????*/
    SI_UINT16                NumLen;              /*??????number??????????*/
    SI_UINT16                EMAILTextLen;        /*Emai????*/
    SI_UINT16                ANRNumberLen;        /*ANR????????*/
}SI_PB_EVENT_SET_CNF_STRU;

typedef struct
{
    SI_UINT8                 NumberType;                      /*??????????145??129*/
    SI_UINT8                 NumberLength;                    /*??????????????????NumberType??????*/
    SI_UINT8                 Number[SI_PB_PHONENUM_MAX_LEN+2];  /*??????????ASCII??????????*/
}SI_PB_ADDITION_NUM_STRU;

typedef struct
{
    SI_UINT32                EmailLen;                        /*Email????*/
    SI_UINT8                 Email[SI_PB_EMAIL_MAX_LEN];      /*Email??????ASCII??????????*/
}SI_PB_EMAIL_STRU;

typedef struct
{
    SI_UINT16                Index;                           /*??????index*/
    SI_UINT8                 AlphaTag[SI_PB_ALPHATAG_MAX_LEN];/*??????????????????????????*/
    SI_UINT8                 ucAlphaTagLength;                /*????????, ??????????????UNICODE????????????????????????*/
    SI_UINT8                 NumberType;                      /*??????????145??129*/
    SI_UINT8                 NumberLength;                    /*??????????????????NumberType??????*/
    SI_UINT8                 ValidFlag;                       /*????????????*/
    SI_UINT8                 Number[SI_PB_PHONENUM_MAX_LEN];  /*????1??????ASCII??????????*/
    SI_PB_AlPHATAG_TYPE      AlphaTagType;                    /*????????*/
    SI_PB_ADDITION_NUM_STRU  AdditionNumber[3];
    SI_PB_EMAIL_STRU         Email;
}SI_PB_RECORD_STRU;

typedef struct
{
    SI_UINT16                RecordNum;   /*??????????*/
    SI_UINT16                Rsv;
    SI_PB_RECORD_STRU        PBRecord; /*????????????*/
}SI_PB_EVENT_READ_CNF_STRU;

typedef struct
{
    SI_UINT16                RecordNum;   /*??????????*/
    SI_UINT16                Rsv;
    SI_PB_RECORD_STRU        PBRecord; /*????????????*/
}SI_PB_EVENT_SEARCH_CNF_STRU;

typedef struct
{
    SI_UINT16                Index; /*????????????????????*/
}SI_PB_EVENT_ADD_CNF_STRU;

typedef struct
{
    SI_UINT16                ClientId;
    SI_UINT8                 OpId;
    SI_UINT8                 Reserve;
    SI_UINT32                PBEventType;
    SI_UINT32                PBError;
    SI_PB_STORATE_TYPE       Storage;
    SI_UINT32                PBLastTag;
    union
    {
        SI_PB_EVENT_QUERY_CNF_STRU      PBQueryCnf ;
        SI_PB_EVENT_INFO_IND_STRU       PBInfoInd ;
        SI_PB_EVENT_SET_CNF_STRU        PBSetCnf ;
        SI_PB_EVENT_READ_CNF_STRU       PBReadCnf ;
        SI_PB_EVENT_ADD_CNF_STRU        PBAddCnf ;
        SI_PB_EVENT_SEARCH_CNF_STRU     PBSearchCnf;
    }PBEvent;
}SI_PB_EVENT_INFO_STRU;

typedef struct
{
    SI_BOOL                  bESC;                       /* ESC???????????????? */
    SI_UINT8                 aucEccCode[USIM_ECC_LEN];   /* Emergency Call Code */
    SI_UINT8                 ucESC;                      /* ESC?????? */
    SI_UINT8                 ucLen;                      /* ESC???????? */
    SI_UINT8                 AlphaIdentifier[253];        /* ESC???????? */
    SI_UINT8                 aucReserved[2];
}SI_PB_ECC_RECORD_STRU;

typedef struct
{
    SI_BOOL                  bEccExists;         /* ECC???????????? */
    SI_UINT32                ulReocrdNum;        /* ECC???????????????????? */
    SI_PB_ECC_RECORD_STRU    astEccRecord[USIM_MAX_ECC_RECORDS];
}SI_PB_ECC_DATA_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                  ulMsgName;          /* ??????   */
}PBMsgBlock;


/*****************************************************************************
 ??????    : SI_PB_FDN_NUM_STRU
 ????????  : FDN????????
*****************************************************************************/
typedef struct
{
    VOS_UINT32                  ulNum1Len;
    VOS_UINT8                   aucNum1[SI_PB_NUMBER_LEN];
    VOS_UINT32                  ulNum2Len;
    VOS_UINT8                   aucNum2[SI_PB_NUMBER_LEN];
}SI_PB_FDN_NUM_STRU;

/*****************************************************************************
 ??????    : SI_PB_FDN_CHECK_REQ_STRU
 ????????  : FDN????????????????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;          /* ?????? */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;
    VOS_UINT32                          ulSendPara;
    VOS_UINT32                          ulContextIndex;
    SI_PB_FDN_NUM_STRU                  stFDNNum;           /* FDN???????? */
}SI_PB_FDN_CHECK_REQ_STRU;

/*****************************************************************************
 ??????    : SI_PB_FDN_CHECK_REQ_STRU
 ????????  : FDN??????????????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;          /* ?????? */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;          /* APP???? */
    VOS_UINT32                          ulSendPara;         /* ???????? */
    VOS_UINT32                          ulContextIndex;     /* index?? */
    PB_FDN_CHECK_RSLT_ENUM_UINT32       enResult;           /* ???????? */
}PB_FDN_CHECK_CNF_STRU;

/*****************************************************************************
??????    : SI_PB_ECALL_QRY_REQ_STRU
????????  : Ecall????FDN??SDN??????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;          /* ?????? */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;          /* APP???? */
    SI_PB_STORAGE_ENUM_UINT32           enStorage;          /* ??????????????????SI_PB_STORAGE_FD??SI_PB_STORAGE_SD */
    VOS_UINT8                           ucListLen;          /* ?????????????????? */
    VOS_UINT8                           aucList[3];         /* ?????????????????? */
}SI_PB_ECALLQRY_REQ_STRU;



typedef struct
{
    VOS_UINT8                           ucTon;              /* ???????? */
    VOS_UINT8                           ucLength;           /* BCD???????? */
    VOS_UINT8                           ucIndex;            /* ??1???? */
    VOS_UINT8                           aucRsv[1];
    VOS_UINT8                           aucCallNumber[SI_PB_PHONENUM_MAX_LEN/2];
}SI_PB_ECALL_NUM_STRU;




typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;          /* ?????? */
    VOS_UINT32                          ulRslt;             /* VOS_OK: ????????;VOS_ERR:????????(??????????????????????????) */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;          /* APP???? */
    SI_PB_STORAGE_ENUM_UINT32           enStorage;          /* ??????????,??????????SDN??FDN */
    VOS_UINT32                          ulRecordNum;        /* ???????????????????????? */
    SI_PB_ECALL_NUM_STRU                astECallNumber[2];
}SI_PB_ECALLQRY_CNF_STRU;

/*****************************************************************************
??????    : SI_PB_ECALL_INIT_IND_STRU
????????  : ECALL??????????????????
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgName;          /* ?????? */
    USIMM_CARDAPP_ENUM_UINT32           enAppType;          /* APP???? */
    VOS_UINT16                          usFdnRecordNum;     /* FDN???????? */
    VOS_UINT16                          usSdnRecordNum;     /* SDN???????? */
}SI_PB_ECALLINIT_IND_STRU;

/*****************************************************************************
  5 ????????
*****************************************************************************/

extern SI_UINT32 SI_PB_Read(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_UINT16 Index1,SI_UINT16 Index2);

extern SI_UINT32 SI_PB_Set(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage);

extern SI_UINT32 SI_PB_Modify(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_PB_RECORD_STRU* pRecord) ;

extern SI_UINT32 SI_PB_SModify(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_PB_RECORD_STRU* pRecord);

extern SI_UINT32 SI_PB_SRead(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage, SI_UINT16 Index1,SI_UINT16 Index2);

extern SI_UINT32 SI_PB_Delete(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_UINT16 Index);

extern SI_UINT32 SI_PB_Add(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_PB_RECORD_STRU *pRecord);

extern SI_UINT32 SI_PB_SAdd(SI_UINT16 ClientId,SI_UINT8 OpId,SI_PB_STORATE_TYPE Storage,SI_PB_RECORD_STRU *pRecord);

extern SI_UINT32 SI_PB_Search(SI_UINT16 ClientId,  SI_UINT8 OpId, SI_PB_STORATE_TYPE Storage, SI_UINT8 ucLength, SI_UINT8 *pucContent);

extern SI_UINT32 SI_PB_Query(SI_UINT16 ClientId,SI_UINT8 OpId);

extern VOS_UINT32 SI_PB_GetEccNumber(SI_PB_ECC_DATA_STRU *pstEccData);

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
extern VOS_UINT32 SI_PB_GetXeccNumber(SI_PB_ECC_DATA_STRU *pstEccData);
#endif

extern VOS_UINT32 WuepsPBPidInit(enum VOS_INIT_PHASE_DEFINE InitPhrase);

extern VOS_VOID SI_PB_MemSet(VOS_UINT8 ucLen, VOS_UINT8 ucValue,VOS_UINT8 *pucMem);

extern VOS_VOID   SI_PB_PidMsgProc(struct MsgCB *pstPBMsg);

extern VOS_UINT32 SI_PB_GetStorateType(VOS_VOID);

extern VOS_UINT32 SI_PB_GetSPBFlag(VOS_VOID);

/*??????????????*/


extern VOS_UINT32 SI_PB_GetEccNumber_Instance(
    MODEM_ID_ENUM_UINT16                enModemID,
    SI_PB_ECC_DATA_STRU                *pstEccData);




#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of SiAppPB.h */

