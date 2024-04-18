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



/*****************************************************************************
1 ??????????
*****************************************************************************/
#include "bsp_version.h"
#include "nv_xml_dec.h"
#include "nv_debug.h"
#include "nv_comm.h"

/*****************************************************************************
2 ????????????
*****************************************************************************/

/*xml decode info*/
static XML_DOCODE_INFO xml_ctrl;

/* XML??????,??????0-9,a-z,A-Z */
static s8   g_stlxmkeywordtbl[] = { '<', '>', '/', '=', '"',
                                   ' ', '!', '?', '_', '-',
                                   ',' };

/* XML????????????????                  */
static XML_ANALYSE_STATUS_ENUM_UINT32 g_stlxmlstatus = XML_ANASTT_ORIGINAL;

/* ????????????                         */
static s8 g_aclnodelabelcommon[]               = "common_NvInfo";

/* Product????????                      */
static s8 g_aclnodelabelproduct[]              = "product";

/* Product_NvInfo????????                      */
static s8 g_aclnodelabelproductNvInfo[]          = "product_NvInfo";

/* NV????????                           */
static s8 g_aclnodelabelnv[]                   = "nv";

/* Cust????????                         */
static s8 g_aclnodelabelcust[]                 = "cust";

/* NV id????????                           */
static s8 g_aclpropertyId[]                   = "id";

/* NV ??????????????                          */
static s8 g_aclpropertyWp[]                   = "wp";


/* ??????????????????????????           */
static s8 g_separator                        = ',';

/*****************************************************************************
3 ????????
*****************************************************************************/

void xml_write_error_log(u32 ulerrorline, u16 ulnvid,
                                u32 ret)
{
    xml_ctrl.g_stlxmlerrorinfo.ulxmlline = xml_ctrl.g_stlxml_lineno;
    xml_ctrl.g_stlxmlerrorinfo.ulstatus  = g_stlxmlstatus;
    xml_ctrl.g_stlxmlerrorinfo.ulcodeline= ulerrorline;
    xml_ctrl.g_stlxmlerrorinfo.usnvid    = ulnvid;
    xml_ctrl.g_stlxmlerrorinfo.ulresult  = (u32)ret;

    return ;
}


XML_RESULT_ENUM_UINT32 xml_checkxmlkeyword(s8 currentchar)
{
    u32 lcount;

    if ((('0' <= currentchar) && ('9' >= currentchar))   /* ??????????0-9  */
        ||(('a' <= currentchar) && ('z' >= currentchar)) /* ??????????a-z  */
        ||(('A' <= currentchar) && ('Z' >= currentchar)))/* ??????????A-Z  */
    {
        return XML_RESULT_SUCCEED;
    }
    /* ?? 0-9,a-z,A-Z ?????? XML?????? */
    for (lcount=0; lcount<sizeof(g_stlxmkeywordtbl); lcount++)
    {
        if (currentchar == g_stlxmkeywordtbl[lcount])
        {
            return XML_RESULT_SUCCEED;
        }
    }
    nv_record("<%s>: err char :%c\n",__func__,currentchar);
    return XML_RESULT_FALIED_BAD_CHAR;
}



XML_RESULT_ENUM_UINT32 xml_checkcharvalidity(s8 currentchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    if (('\r' == currentchar)       /* ????????   */
        || ('\t' == currentchar))   /* ?????????? */
    {
        return XML_RESULT_SUCCEED_IGNORE_CHAR;
    }

    if ('\n' == currentchar)    /* ????????   */
    {
        xml_ctrl.g_stlxml_lineno++;
        return XML_RESULT_SUCCEED_IGNORE_CHAR;
    }

    /* ?????????????????????? */
    if ( XML_ANASTT_IGNORE == g_stlxmlstatus)
    {
        return XML_RESULT_SUCCEED;
    }

    /* ????XML???????? */
    returnval = xml_checkxmlkeyword(currentchar);
    if (XML_RESULT_SUCCEED != returnval)
    {
        nv_printf("returnval = %d\n",returnval);
        xml_write_error_log(__LINE__, 0, returnval);

        return returnval;
    }

    return XML_RESULT_SUCCEED;
}

XML_RESULT_ENUM_UINT32 xml_stringtou16(s8  *pcbuff,
                                            u16 *pusretval)
{
    u32 ultemp = 0;  /* ?????????????????????????? */
    s8   currentchar;
    s8  *pcsrc;

    pcsrc = pcbuff;

    /* ????NV ID?????????????????? */
    if (0 == *pcsrc)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_NV_ID_IS_NULL);

        return XML_RESULT_FALIED_NV_ID_IS_NULL;
    }

    /* ???????????????????????? */
    while (0 != *pcsrc)
    {
        currentchar = *pcsrc;

        /* ??????0??9?????????????????????? */
        if ((currentchar < '0') || (currentchar > '9'))
        {
            xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_OUT_OF_0_9);
            return XML_RESULT_FALIED_OUT_OF_0_9;
        }

        /* ?????????????? */
        currentchar -= '0';
        ultemp = (ultemp*10) + (u8)currentchar;

        /* ????NV ID???????? */
        if (EN_NV_ID_END <= ultemp)
        {
            xml_write_error_log(__LINE__, (u16)ultemp, XML_RESULT_FALIED_OUT_OF_MAX_VALUE);

            return XML_RESULT_FALIED_OUT_OF_MAX_VALUE;
        }

        pcsrc++;
    }

    /* ???? ?????????? */
    *pusretval = (u16)ultemp;

    return XML_RESULT_SUCCEED;
}


XML_RESULT_ENUM_UINT32 xml_stringtohex(u8 *pucsrc, u8 *pucdest)
{
    u8  ucurrent;
    u8  uctemp  = 0;
    u16 uscount = 0;

    /* pucsrc????????????????,???????????????? */
    while (0 != *pucsrc)
    {
        ucurrent = *pucsrc;

        if ((ucurrent >= 'a') && (ucurrent <= 'f'))
        {
            /* ??????????????????*/
            ucurrent -= 'a'-'A';
        }

        /* ???????????????? */
        if ((ucurrent >= 'A') && (ucurrent <= 'F'))
        {
            uscount++;
            uctemp = (u8)(uctemp * 16) + (u8)(ucurrent - 'A') + 10;
        }
        else if ((ucurrent >= '0') && (ucurrent <= '9'))
        {
            uscount++;
            uctemp = (u8)(uctemp * 16) + (u8)(ucurrent - '0');
        }
        else
        {
            /* ??????0-9,a-f,A-F?????????????????????? */
            xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_OUT_OF_0_F);

            return XML_RESULT_FALIED_OUT_OF_0_F;
        } /* end of if ((ucurrent >= 'a') && (ucurrent <= 'f')) */

        /* ??2??????????????????????????,?????????????????? */
        if (2 == uscount)
        {
            *pucdest++ = uctemp;
            uctemp = 0;
            uscount = 0;
        }

        pucsrc++;
    }

    /* ????????????????'\0' */
    *pucdest = 0;

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_stringtovalue(u8  *pucbuff,
                                                 u8  *pucretbuff,
                                                 u32 *pretbufflen)
{
    XML_RESULT_ENUM_UINT32 returnval;
    u32 ulcount = 0;    /* ?????????????????????????? */
    u8 *pcsrc;
    u8 *pcdest;
    pcsrc  = pucbuff;
    pcdest = pucbuff;

     /* ????NV VALUE?????????????????? */
    if (0 == *pcsrc)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_NV_VALUE_IS_NULL);
        return XML_RESULT_FALIED_NV_VALUE_IS_NULL;
    }


    /* ?????????????????????????? */
    while (0 != *pcsrc)
    {
        /* ???????????????????? */
        if (g_separator == *pcsrc)
        {
            /* ??????2????????????????????????,??????????????????'0' */
            if (1 == ulcount)
            {
                *pcdest = *(pcdest-1);
                *(pcdest-1) = '0';
                pcdest++;
            }

            ulcount = 0;
            pcsrc++;

            continue;
        }

        /* ??????????NV??????2??????,??????.??<nv id="1">A3E</nvs> */
        if (2 <= ulcount)
        {
            xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_OUT_OF_2_CHAR);
            return XML_RESULT_FALIED_OUT_OF_2_CHAR;
        }

        *pcdest++ = *pcsrc++;
        ulcount++;    /* ???????????????????????? */
    }


    /* ????????????????'\0' */
    *pcdest = '\0';


    /* ???????????????????? */
    *pretbufflen = (u32)(pcdest - pucbuff)/2;

    /* ???????????????? */
    pcsrc  = pucbuff;
    pcdest = pucretbuff;

    returnval =  xml_stringtohex(pcsrc, pcdest);

    return returnval;
}


XML_RESULT_ENUM_UINT32 xml_write_nv_data(void)
{
    XML_RESULT_ENUM_UINT32 returnval = NV_ERROR;
    u16 usnvitemid  = 0;
    u16 priority = 0xFFFF;
    u32 ulnvitemlen = 0;
    s32    lisnv;
    s32    liscust;
    nv_item_info_s ref_info = {0};
    nv_file_info_s file_info = {0};
    u32 ulPropertyIndex;
    u8* ref_offset = NULL;
    nv_ctrl_info_s* ctrl_info = (nv_ctrl_info_s*)NV_GLOBAL_CTRL_INFO_ADDR;

    /* ??????????????????????????,?????????????? */
    if (XML_PRODUCT_NODE_STATUS_INVALID == xml_ctrl.g_stlxmlproductinfo.envalidnode)
    {
        return XML_RESULT_SUCCEED;
    }

    /* ??????????????,??????????????,??????????????, ??<cust/> */
    xml_ctrl.g_stlxmlcurrentnode.stproperty[0].pcpropertyvalue[
                        xml_ctrl.g_stlxmlcurrentnode.stproperty[0].ulvaluelength] = '\0';

    xml_ctrl.g_stlxmlcurrentnode.pcnodevalue[xml_ctrl.g_stlxmlcurrentnode.ulvaluelength] = '\0';

    if ((0 == *xml_ctrl.g_stlxmlcurrentnode.stproperty[0].pcpropertyvalue)
        || (0 == *xml_ctrl.g_stlxmlcurrentnode.pcnodevalue))
    {
        return XML_RESULT_SUCCEED;
    }

    /* ????nv??cust????????NV?? */
    xml_ctrl.g_stlxmlcurrentnode.pcnodelabel[xml_ctrl.g_stlxmlcurrentnode.ullabelendlength] = '\0';

    lisnv = strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelnv, strlen((char *)g_aclnodelabelnv) + 1);

    liscust = strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelcust, strlen((char *)g_aclnodelabelcust) + 1);

    if ((0 != lisnv) && (0 != liscust))
    {
        return XML_RESULT_SUCCEED;
    }
    for(ulPropertyIndex = 0; ulPropertyIndex <= xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex; ulPropertyIndex++)
    {
        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname[
                    xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulnamelength] = '\0';

        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue[
                            xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulvaluelength] = '\0';

        if(!strncmp(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname, 
                                                g_aclpropertyId, strlen((char *)g_aclpropertyId) + 1))
        {
            /* ??id ??????????NV ID */
            returnval = xml_stringtou16(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue,
                                        &usnvitemid);

            if (XML_RESULT_SUCCEED != returnval)
            {
                goto out;
            }
        }
        else if(!strncmp(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname,
                                                g_aclpropertyWp, strlen((char *)g_aclpropertyWp) + 1))
        {
            /* ??wp??????????priority */
            returnval = xml_stringtou16(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue,
                                        &priority);

            if (XML_RESULT_SUCCEED != returnval)
            {
                goto out;
            }
        }
        else
        {
            /* coverity[uninit_use_in_call] */
            xml_write_error_log(__LINE__, usnvitemid, returnval);
            nv_printf("<%s>: pcpropertyvalue :%s\n",__func__, xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue);
        }

    }

    /* ????????????NV Value */
    ulnvitemlen = 0;

    returnval = xml_stringtovalue((u8 *)xml_ctrl.g_stlxmlcurrentnode.pcnodevalue,
                                  xml_ctrl.g_puclnvitem,
                                  &ulnvitemlen);

    if (XML_RESULT_SUCCEED != returnval)
    {
        xml_write_error_log(__LINE__, usnvitemid, returnval);
        goto out;
    }


    /* ????NV?? */
    /*????xml??????????????NV??????????????????????*/
    returnval = xml_nv_search_byid((u32)usnvitemid,((u8*)NV_GLOBAL_CTRL_INFO_ADDR),&ref_info,&file_info, (u8**)&ref_offset);
    if(returnval)
    {
        return XML_RESULT_SUCCEED;
    }
    /*????xml????????????,????:????comm??????????1??modem,diff????????????2??diff??????*/
    if(xml_ctrl.card_type > ref_info.modem_num)
    {
        xml_write_error_log(__LINE__, usnvitemid, BSP_ERR_NV_XML_CFG_ERR);
        nv_printf("nv:0x%x xml config error\n", usnvitemid);
        nv_printf("comm xml modem = 0x%x, diff xml modem = 0x%x\n", ref_info.modem_num ,xml_ctrl.card_type);
        return BSP_ERR_NV_XML_CFG_ERR;
    }
    if((ctrl_info->crc_mark)&NV_CTRL_MODEM_CRC)
    {
        ref_info.nv_off += (xml_ctrl.card_type - NV_USIMM_CARD_1)*(ref_info.nv_len + 4);
    }
    else
    {
        ref_info.nv_off += (xml_ctrl.card_type - NV_USIMM_CARD_1)*ref_info.nv_len;
    }

    returnval = xml_nv_write_to_mem(xml_ctrl.g_puclnvitem,ref_info.nv_len,file_info.file_id,ref_info.nv_off);
    if(returnval)
    {
        xml_write_error_log(__LINE__, usnvitemid, returnval);
        goto out;
    }

    if(0xFFFF != priority)
    {
        xml_nv_write_priority(ref_offset, priority);
    }
    return XML_RESULT_SUCCEED;
out:
    /* ??????????NV ID */
    return XML_RESULT_FALIED_WRITE_NV;
}


void xml_nodereset(void)
{
    u32 ulPropertyIndex;

    /* ????????????,??????????????0  */
    xml_ctrl.g_stlxmlcurrentnode.ullabellength= 0;

    /* ??????????,??????????????0 */
    xml_ctrl.g_stlxmlcurrentnode.ulvaluelength= 0;

    /* ????????????????,??????????????0 */
    xml_ctrl.g_stlxmlcurrentnode.ullabelendlength= 0;
    /*xml_ctrl.g_stlxmlcurrentnode.stproperty????????????0*/
    xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex = 0;
    for(ulPropertyIndex = 0; ulPropertyIndex < XML_NODE_PROPERTY_NUM; ulPropertyIndex++)
    {
        /* ??????????????,??????????????0 */
        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulnamelength= 0;

        /* ??????????????,??????????????0 */
        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulvaluelength= 0;
    }
    /* coverity[secure_coding] */
    memset(xml_ctrl.g_stlxmlcurrentnode.pcnodevalue,0, (size_t)(XML_NODE_VALUE_BUFF_LENGTH_ORIGINAL+1));

    return ;
}



XML_RESULT_ENUM_UINT32 xml_write_char_to_buff(s8   cnowchar,
                                                 s8   *pcstrbuff,
                                                 u32 *plbufflength,
                                                 bool   ulisnodevalue)
{
    /* ???????? */
    if (' ' == cnowchar)
    {
        return XML_RESULT_SUCCEED;
    }
    /* ??????????Node Value?????????? */
    if ((ulisnodevalue)
        && (*plbufflength >= XML_NODE_VALUE_BUFF_LENGTH_ORIGINAL))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_OUT_OF_BUFF_LEN);

        return XML_RESULT_FALIED_OUT_OF_BUFF_LEN;
    }


    /* ??????????Node Lable?????????? */
    if ((!ulisnodevalue)
        && (*plbufflength >= XML_NODE_LABEL_BUFF_LENGTH_ORIGINAL))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_OUT_OF_BUFF_LEN);

        return XML_RESULT_FALIED_OUT_OF_BUFF_LEN;
    }
    /* ?????????????????? */
    *(pcstrbuff + *plbufflength) = cnowchar;

    /* ????????????1 */
    (*plbufflength)++;

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_checknodelabelvalid(void)
{
    u32 ulPropertyIndex = 0;

    /* ?????????????????? */
    xml_ctrl.g_stlxmlcurrentnode.pcnodelabel[xml_ctrl.g_stlxmlcurrentnode.ullabellength] = '\0';

    /* ??????????????????????,?????????????????????????? */
    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelcommon, strlen((char *)g_aclnodelabelcommon) + 1))
    {
        /* ??????<Common_NvInfo>????,???????????????????? */
        xml_ctrl.g_stlxmlproductinfo.envalidnode= XML_PRODUCT_NODE_STATUS_VALID;

        return XML_RESULT_SUCCEED;
    }
    /* ??????<Cust>????,???????????????????? */
    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelcust, strlen((char *)g_aclnodelabelcust) + 1))
    {
        xml_ctrl.g_stlxmlproductinfo.envalidnode= XML_PRODUCT_NODE_STATUS_VALID;

        return XML_RESULT_SUCCEED;
    }
    /* ??????????????????????,?????????????????????????? */
    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelproductNvInfo, strlen((char *)g_aclnodelabelproductNvInfo) + 1))
    {
        /* ??????<product_NvInfo>????,???????????????????? */
        xml_ctrl.g_stlxmlproductinfo.envalidnode= XML_PRODUCT_NODE_STATUS_VALID;

        return XML_RESULT_SUCCEED;
    }


    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelproduct, strlen((char *)g_aclnodelabelproduct) + 1))
    {

        ulPropertyIndex = xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex;

        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue[
                      xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulvaluelength] = '\0';

        /*??????????product??????????????????*/
        /* ??????<product>??????Product id??????????????,???????????????????? */
        xml_ctrl.g_stlxmlproductinfo.envalidnode= XML_PRODUCT_NODE_STATUS_INVALID;

        if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue,
                xml_ctrl.g_stlxmlproductinfo.acproductid, strlen((char *)xml_ctrl.g_stlxmlproductinfo.acproductid) + 1))
        {
            xml_ctrl.g_stlxmlproductinfo.envalidnode= XML_PRODUCT_NODE_STATUS_VALID;
            xml_ctrl.g_stlxmlproductinfo.ulnodelevel++;
        }
        else/*product ????????????*/
        {
            xml_ctrl.g_stlxmlproductinfo.envalidnode     = XML_PRODUCT_NODE_STATUS_INVALID;
            xml_ctrl.g_stlxmlproductinfo.enxmldecodejump = XML_DECODE_STATUS_2JUMP;
        }
        return XML_RESULT_SUCCEED;
    }

    if (XML_PRODUCT_NODE_STATUS_VALID == xml_ctrl.g_stlxmlproductinfo.envalidnode)
    {
        /* ??????????????????????Product????????????<product>?????????????? */
        if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                            g_aclnodelabelproduct, strlen((char *)g_aclnodelabelproduct) + 1))
        {
            xml_ctrl.g_stlxmlproductinfo.ulnodelevel++;
        }

        return XML_RESULT_SUCCEED;
    }

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_checknodeendlabelvalid(void)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ????????????NV?? */
    returnval = xml_write_nv_data();

    if (XML_RESULT_SUCCEED != returnval)
    {
        return returnval;
    }

    xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend[xml_ctrl.g_stlxmlcurrentnode.ullabelendlength]='\0';

    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend,
                        g_aclnodelabelproduct, strlen((char *)g_aclnodelabelproduct) + 1))
    {
        if (XML_PRODUCT_NODE_STATUS_VALID == xml_ctrl.g_stlxmlproductinfo.envalidnode)
        {
            xml_ctrl.g_stlxmlproductinfo.ulnodelevel--;

            if (0 == xml_ctrl.g_stlxmlproductinfo.ulnodelevel)
            {
                /* ??????????????</product>????????????????????XML?????????? */
                xml_ctrl.g_stlxmlproductinfo.enxmldecodestate = XML_DECODE_STATUS_FINISHED;
                xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
            }
        }
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend,
                             g_aclnodelabelcommon, strlen((char *)g_aclnodelabelcommon) + 1))
    {
        /* </Common_NvInfo>??????????,?????????????????? ,??????????????????*/
        xml_ctrl.g_stlxmlproductinfo.envalidnode     = XML_PRODUCT_NODE_STATUS_INVALID;
        xml_ctrl.g_stlxmlproductinfo.enxmldecodejump = XML_DECODE_STATUS_JUMP;
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend,
                             g_aclnodelabelcust, strlen((char *)g_aclnodelabelcust) + 1))
    {
        /* </Cust>??????????,?????????????????? */
        xml_ctrl.g_stlxmlproductinfo.envalidnode = XML_PRODUCT_NODE_STATUS_INVALID;
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend,
                             g_aclnodelabelproductNvInfo, strlen((char *)g_aclnodelabelproductNvInfo) + 1))
    {
        /* ??????</product_NvInfo>??????????,????????????????XML??????????*/
        xml_ctrl.g_stlxmlproductinfo.enxmldecodestate = XML_DECODE_STATUS_FINISHED;
        xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
    }
    else
    {
        /* Do nothing */

    } /* end of if (0 == VOS_StrCmp(g_stXMLCurrentNode.pcnodelabelend, */

    /* </xx>???????????????????????? */
    xml_nodereset();

    return XML_RESULT_SUCCEED;
}


XML_RESULT_ENUM_UINT32 xml_createaproperty( void )
{
    u32 propertyIndex;

    for(propertyIndex = 0;propertyIndex < XML_NODE_PROPERTY_NUM; propertyIndex++)
    {
        /* ??????????????,+1???????????????????? */
        xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].ulnamelength = 0; /* ???????????? */

        xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].pcpropertyname
                                    = (s8*)nv_malloc(
                                       (size_t)(XML_NODE_LABEL_BUFF_LENGTH_ORIGINAL+1));

        if (NULL == xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].pcpropertyname)
        {
             xml_write_error_log(__LINE__, (u16)propertyIndex, XML_RESULT_FALIED_MALLOC);

            return XML_RESULT_FALIED_MALLOC;
        }

        /* ??????????????,+1???????????????????? */
        xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].ulvaluelength = 0; /* ???????????? */


        xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].pcpropertyvalue
                                    = (s8*)nv_malloc(
                                       (size_t)(XML_NODE_LABEL_BUFF_LENGTH_ORIGINAL+1));
        if (NULL == xml_ctrl.g_stlxmlcurrentnode.stproperty[propertyIndex].pcpropertyvalue)
        {
            xml_write_error_log(__LINE__, (u16)propertyIndex, XML_RESULT_FALIED_MALLOC);

            return XML_RESULT_FALIED_MALLOC;
        }
    }
    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_createanode(void)
{
    XML_RESULT_ENUM_UINT32 returnval;


    /* ?????????????? */
    returnval = xml_createaproperty();

    if(XML_RESULT_SUCCEED != returnval)
    {
        return returnval;
    }

    /* ????????????????,+1????????????????????*/
    xml_ctrl.g_stlxmlcurrentnode.ullabellength = 0; /* ???????????? */



    xml_ctrl.g_stlxmlcurrentnode.pcnodelabel = (s8*)nv_malloc(
                                      (size_t)(XML_NODE_LABEL_BUFF_LENGTH_ORIGINAL+1));

    if (NULL ==  xml_ctrl.g_stlxmlcurrentnode.pcnodelabel)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_MALLOC);

        return XML_RESULT_FALIED_MALLOC;
    }

    /* ????????????????????????,+1???????????????????? */
    xml_ctrl.g_stlxmlcurrentnode.ullabelendlength = 0; /* ???????????? */

    xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend = (s8*)nv_malloc(
                                        (size_t)(XML_NODE_LABEL_BUFF_LENGTH_ORIGINAL+1));

    if (NULL ==  xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_MALLOC);

        return XML_RESULT_FALIED_MALLOC;
    }

    /* ??????????????,+1???????????????????? */
    xml_ctrl.g_stlxmlcurrentnode.ulvaluelength = 0; /* ???????????? */

    xml_ctrl.g_stlxmlcurrentnode.pcnodevalue = (s8*)nv_malloc(
                                        (size_t)(XML_NODE_VALUE_BUFF_LENGTH_ORIGINAL+1));

    if (NULL == xml_ctrl.g_stlxmlcurrentnode.pcnodevalue)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_MALLOC);

        return XML_RESULT_FALIED_MALLOC;
    }

    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_procxmlorginal(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval = XML_RESULT_SUCCEED;

    /* ????<?????????? */
    if ('<' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_ENTER_LABLE;
        return XML_RESULT_SUCCEED;
    }

    /* ????>,/,",=??????XML???????? */
    if (('>' == cnowchar)
         ||('/' == cnowchar)
         ||('"' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);
        nv_record("%s  err char %c \n",__func__,cnowchar);
        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    if (XML_PRODUCT_NODE_STATUS_VALID == xml_ctrl.g_stlxmlproductinfo.envalidnode)
    {
        /* ?????????????????????????????????? */
        returnval = xml_write_char_to_buff(cnowchar,
                                     xml_ctrl.g_stlxmlcurrentnode.pcnodevalue,
                                     &(xml_ctrl.g_stlxmlcurrentnode.ulvaluelength),
                                     (bool)true);
    }

    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_procxmlignore(s8 cnowchar)
{

    /* ?????????????????????????????? */
    if ('>' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;
    }
    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_proc_xmlsingle_endlabel(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    returnval = XML_RESULT_SUCCEED;

    /* ????<?????????? */
    if ('>' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;

        /* ???????????????????????? */
        returnval = xml_checknodeendlabelvalid();

        return returnval;
    }

    /* ????<,/,",=??????XML???????? */
    if (('<' == cnowchar)
         ||('"' == cnowchar)
         ||('/' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }
    /* ?????????????????????????????????? */
    returnval = xml_write_char_to_buff(cnowchar,
                                  xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend,
                                  &(xml_ctrl.g_stlxmlcurrentnode.ullabelendlength),
                                  (bool)false);
    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_checknode_rightlabel(void)
{
    u32 ulPropertyIndex = 0;

    /* ?????????????????? */
    xml_ctrl.g_stlxmlcurrentnode.pcnodelabel[xml_ctrl.g_stlxmlcurrentnode.ullabellength] = '\0';

    if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                        g_aclnodelabelproduct, strlen((char *)g_aclnodelabelproduct) + 1))
    {
        ulPropertyIndex = xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex;

        xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue[
                      xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulvaluelength] = '\0';

        /* ????<product product_id="123456"/>????product id??????product id????????????XML?????????? */
        if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue,
                    xml_ctrl.g_stlxmlproductinfo.acproductid, strlen(xml_ctrl.g_stlxmlproductinfo.acproductid) + 1))
        {
            xml_ctrl.g_stlxmlproductinfo.enxmldecodestate = XML_DECODE_STATUS_FINISHED;
            xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
        }
        else
        {
            xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
        }
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                             g_aclnodelabelcommon, strlen((char *)g_aclnodelabelcommon) + 1))
    {
        /* <Common_NvInfo/>??????????,?????????????????? ,??????????????????*/
        xml_ctrl.g_stlxmlproductinfo.envalidnode     = XML_PRODUCT_NODE_STATUS_INVALID;
        xml_ctrl.g_stlxmlproductinfo.enxmldecodejump = XML_DECODE_STATUS_JUMP;
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                             g_aclnodelabelcust, strlen((char *)g_aclnodelabelcust) + 1))
    {
        /* <Cust/>??????????,?????????????????? */
        xml_ctrl.g_stlxmlproductinfo.envalidnode = XML_PRODUCT_NODE_STATUS_INVALID;
    }
    else if (0 == strncmp(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                             g_aclnodelabelproductNvInfo, strlen((char *)g_aclnodelabelproductNvInfo) + 1))
    {
        /* <product_NvInfo/>??????????,????????????????XML??????????*/
        xml_ctrl.g_stlxmlproductinfo.enxmldecodestate = XML_DECODE_STATUS_FINISHED;
        xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
    }
    else
    {
        /* Do nothing */

    }

    /* <xx/>???????????????????????? */
    xml_nodereset();

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_procxmlend_mustberight(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ???????? */
    if (' ' == cnowchar)
    {
        return XML_RESULT_SUCCEED;
    }

    /* ????????>,??????XML???????? */
    if ('>' != cnowchar)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    /* ???????? */
    g_stlxmlstatus = XML_ANASTT_ORIGINAL;

    /*must be right need to check label*/
    returnval = xml_checknode_rightlabel();

    if (XML_RESULT_SUCCEED != returnval)
    {
        return returnval;
    }

    if (XML_PRODUCT_NODE_STATUS_VALID == xml_ctrl.g_stlxmlproductinfo.envalidnode)
    {
        /* ????????????NV?? */
        returnval = xml_write_nv_data();

        if (XML_RESULT_SUCCEED != returnval)
        {
            return returnval;
        }

        /* <xx/>???????????????????????? */
        xml_nodereset();
    }

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_node_label(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ????/????>????????????Node???????????? */
    if ('/' == cnowchar)
    {
        /* ??????????????????,??????????????> */
        g_stlxmlstatus = XML_ANASTT_LABLE_END_MUST_RIGHT;

        return XML_RESULT_SUCCEED;
    }

    /* ???????? */
    if ('>' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;

        /* ?????????????????????? */
        returnval = xml_checknodelabelvalid();

        return returnval;
    }

    /* ????????????,???????????????? */
    if (' ' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_PROPERTY_START;

        return XML_RESULT_SUCCEED;
    }

    /* ????<,",=??????XML???????? */
    if (('<' == cnowchar)
         ||('"' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0,XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }
    /* ?????????????????????????????????? */
    returnval = xml_write_char_to_buff(cnowchar,
                                     xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                                     &(xml_ctrl.g_stlxmlcurrentnode.ullabellength),
                                     (bool)false);
    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_enter_label(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval = XML_RESULT_SUCCEED;

    /* ???????????????? */
    if ('?' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_IGNORE;

        return XML_RESULT_SUCCEED;
    }

    /* ???????? */
    if ('!' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_IGNORE;

        return XML_RESULT_SUCCEED;
    }

    /* ?????????????? */
    if ('/' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_SINGLE_ENDS_LABLE;

        return XML_RESULT_SUCCEED;
    }

    /* ???????????? */
    if ('>' == cnowchar)
    {
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;

        return XML_RESULT_SUCCEED;
    }

    /* ????<,",=??????XML???????? */
    if (('<' == cnowchar)
         ||('"' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);
        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    /* ???????? */
    if (' ' != cnowchar)
    {
        /* ????????????NV?? */
        returnval = xml_write_nv_data();

        if (XML_RESULT_SUCCEED != returnval)
        {
            return returnval;
        }

        /* ???????????????????????????????? */
        xml_nodereset();

        /* ???????????????????????????? */
        g_stlxmlstatus = XML_ANASTT_NODE_LABLE;
        /* ?????????????????????????????????? */
        returnval = xml_write_char_to_buff(cnowchar,
                                         xml_ctrl.g_stlxmlcurrentnode.pcnodelabel,
                                         &(xml_ctrl.g_stlxmlcurrentnode.ullabellength),
                                         (bool)false);
    }

    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_propertystart(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval = XML_RESULT_SUCCEED;
    u32 ulPropertyIndex;


    /* ?????????????? */
    if ('/' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_LABLE_END_MUST_RIGHT;

        return XML_RESULT_SUCCEED;
    }

    /* ???????? */
    if ('>' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;

        /* ???????????????? */
        returnval = xml_checknodelabelvalid();

        return returnval;
    }

    /* ????<,",=??????XML???????? */
    if (('<' == cnowchar)
         ||('"' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    /* ???????? */
    g_stlxmlstatus = XML_ANASTT_PROPERTY_NAME_START;

    ulPropertyIndex = xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex;
    returnval = xml_write_char_to_buff(cnowchar,
                  xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname,
                  &(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulnamelength),
                  (bool)FALSE);

    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_propertyname(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;
    u32 ulPropertyIndex;

    /* ????=?????????????? */
    if ('=' == cnowchar)
    {
        /* ?????? */
        g_stlxmlstatus = XML_ANASTT_PROPERTY_NAME_END;

        return XML_RESULT_SUCCEED;
    }

    /* ????<,>,/,"??????XML???????? */
    if (('<' == cnowchar)||('>' == cnowchar)
        ||('/' == cnowchar)||('"' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    ulPropertyIndex = xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex;
    /* ??????????????????????, ?? <nv i d="123"> */
    returnval = xml_write_char_to_buff(cnowchar,
                  xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname,
                  &(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulnamelength),
                  (bool)false);

    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_propertyname_tail(s8 cnowchar)
{

    /* ???????? */
    if ( ' ' == cnowchar)
    {
        return XML_RESULT_SUCCEED;
    }

    /* ????" */
    if ('"' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_PROPERTY_VALUE_START;

        return XML_RESULT_SUCCEED;
    }

    /* ????????"????????XML???????? */
    xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

    return XML_RESULT_FALIED_BAD_SYNTAX;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_valuestart(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;
    u32 ulPropertyIndex;

    /* ????" */
    if ('"' == cnowchar)
    {
        /* ??????,?????????????????????? */
        g_stlxmlstatus = XML_ANASTT_PROPERTY_VALUE_END;

        return XML_RESULT_SUCCEED;
    }

    /* ????<,>,/,=??????XML???????? */
    if (('<' == cnowchar)
         ||('>' == cnowchar)
         ||('/' == cnowchar)
         ||('=' == cnowchar))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

        return XML_RESULT_FALIED_BAD_SYNTAX;
    }

    ulPropertyIndex = xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex;
    /* ?????????????????????? */
    returnval = xml_write_char_to_buff(cnowchar,
                  xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue,
                  &(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].ulvaluelength),
                  (bool)false);
    return returnval;
}



XML_RESULT_ENUM_UINT32 xml_proc_xml_valuetail(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ?????????????????????? */
    if (' ' == cnowchar)
    {
        /* ????????,?????????????????????? */
        g_stlxmlstatus = XML_ANASTT_PROPERTY_START;
        xml_ctrl.g_stlxmlcurrentnode.ulPropertyIndex++;
        return XML_RESULT_SUCCEED;
    }

    /* ????'/' */
    if ('/' == cnowchar)
    {
        /* ???????? */
        g_stlxmlstatus = XML_ANASTT_LABLE_END_MUST_RIGHT;

        return XML_RESULT_SUCCEED;
    }

    /* ????'>' */
    if ('>' == cnowchar)
    {
        /* ????????,?????????????????????? */
        g_stlxmlstatus = XML_ANASTT_ORIGINAL;

        /* ???????????????? */
        returnval = xml_checknodelabelvalid();

        return returnval;
    }

    /* ????????>,/??????XML???????? */
    xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_BAD_SYNTAX);

    return XML_RESULT_FALIED_BAD_SYNTAX;
}


/*Global map table used to find the function according the xml analyse status.*/
XML_FUN g_uslxmlanalysefuntbl[] =
{
    xml_procxmlorginal,         /* ????????????????                  */
    xml_proc_xml_enter_label,       /* ????Lable????????                 */
    xml_procxmlignore,           /* ????????????????????????">"????   */
    xml_proc_xml_node_label,        /* ????????????                      */
    xml_proc_xmlsingle_endlabel,   /* ??????????????</XXX>              */
    xml_procxmlend_mustberight,   /* ???? <XXX/>??????,????????/?????? */
    xml_proc_xml_propertystart,    /* ????????????                      */
    xml_proc_xml_propertyname,     /* ????????????????                  */
    xml_proc_xml_propertyname_tail, /* ??????????????????"????????????   */
    xml_proc_xml_valuestart,       /* ??????????                        */
    xml_proc_xml_valuetail,        /* ??????????                        */
};

XML_RESULT_ENUM_UINT32 xml_analyse(s8 cnowchar)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ???????????????????? */
    returnval = xml_checkcharvalidity(cnowchar);

    if (XML_RESULT_SUCCEED_IGNORE_CHAR == returnval)
    {
        /* ?????????????????????????? */
        return XML_RESULT_SUCCEED;
    }

    if (XML_RESULT_FALIED_BAD_CHAR == returnval)
    {
        /* ???????????????????????????? */
        return XML_RESULT_FALIED_BAD_CHAR;
    }
    /* ????XML?????????????????????????? */
    returnval = g_uslxmlanalysefuntbl[g_stlxmlstatus](cnowchar);

    return returnval;
}


XML_RESULT_ENUM_UINT32 xml_decode_xml_file(FILE* pfile)
{
    s32               lreaded;           /* ???????????? */
    s32               lcount;            /* ???????????? */
    XML_RESULT_ENUM_UINT32  returnval;



    /*read 4k*/
    lreaded = (s32)nv_file_read((u8*)(xml_ctrl.g_pclfilereadbuff),1,XML_FILE_READ_BUFF_SIZE,pfile);
    if((0 == lreaded)||(lreaded > XML_FILE_READ_BUFF_SIZE)||(lreaded < 0))
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_READ_FILE);
        return XML_RESULT_FALIED_READ_FILE;
    }


    while(0 != lreaded)
    {
        for(lcount = 0;lcount < lreaded; lcount++)
        {
            returnval = xml_analyse(*(xml_ctrl.g_pclfilereadbuff + lcount));
            if(XML_RESULT_SUCCEED != returnval)
            {
                /* ???????????????????????? */
                return returnval;
            }

            /* ???????????????? */
            if (XML_DECODE_STATUS_FINISHED
                == xml_ctrl.g_stlxmlproductinfo.enxmldecodestate)
            {
                nv_record("%s: xml decode success !\n",__func__);
                return XML_RESULT_SUCCEED;
            }

            /*??????common????????????????????????*/
            if((XML_DECODE_STATUS_JUMP == xml_ctrl.g_stlxmlproductinfo.enxmldecodejump) &&
               (XML_DECODE_STATUS_JUMP == xml_ctrl.g_stlxmljumpflag))
            {
                nv_record("%s: product info decode jump  0x%x!\n",__func__,xml_ctrl.g_stlxmljumpinfo.productCatOff);
                nv_file_seek(pfile,(xml_ctrl.g_stlxmljumpinfo.productCatOff),SEEK_SET);
                xml_ctrl.g_stlxmlproductinfo.enxmldecodejump= XML_DECODE_STATUS_NOJUMP;
                g_stlxmlstatus = XML_ANASTT_ORIGINAL;
                break;
            }

            /*product ????????????*/
            if((XML_DECODE_STATUS_2JUMP == xml_ctrl.g_stlxmlproductinfo.enxmldecodejump) &&
               (XML_DECODE_STATUS_JUMP == xml_ctrl.g_stlxmljumpflag))
            {
                nv_record("%s: product id decode jump  0x%x!\n",__func__,xml_ctrl.g_stlxmljumpinfo.productIdOff);
                nv_file_seek(pfile,(xml_ctrl.g_stlxmljumpinfo.productIdOff),SEEK_SET);
                xml_ctrl.g_stlxmljumpflag = XML_DECODE_STATUS_NOJUMP;
                xml_ctrl.g_stlxmlproductinfo.enxmldecodejump = XML_DECODE_STATUS_NOJUMP;
                g_stlxmlstatus = XML_ANASTT_ORIGINAL;
                break;
            }
        }
        /* coverity[secure_coding] */
        memset(xml_ctrl.g_pclfilereadbuff,0,(size_t)XML_FILE_READ_BUFF_SIZE);

        lreaded = (s32)nv_file_read((u8*)(xml_ctrl.g_pclfilereadbuff),1,XML_FILE_READ_BUFF_SIZE,pfile);
        if((lreaded > XML_FILE_READ_BUFF_SIZE)||(lreaded < 0))
        {

            xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_READ_FILE);
            return XML_RESULT_FALIED_READ_FILE;
        }
    }

    /* ????ulnodelevel????0??????<product>???????????? */
    if (0 != xml_ctrl.g_stlxmlproductinfo.ulnodelevel)
    {
        xml_write_error_log(__LINE__,0,XML_RESULT_FALIED_PRODUCT_MATCH);
        return XML_RESULT_FALIED_PRODUCT_MATCH;
    }

    return XML_RESULT_SUCCEED;
}



XML_RESULT_ENUM_UINT32 xml_getproductid(void)
{
    u32    lproductid;

    /* ???????? */
    /* coverity[secure_coding] */
    memset(xml_ctrl.g_stlxmlproductinfo.acproductid, 0, (size_t)XML_MAX_HARDWARE_LEN);

    /* ??????????????Product id */
    lproductid=  bsp_get_version_info()->board_id;/*get hardware version*/

    xml_ctrl.g_stlxmlproductid = lproductid;

    /* coverity[secure_coding] */
    snprintf(xml_ctrl.g_stlxmlproductinfo.acproductid,
                (XML_MAX_HARDWARE_LEN - 1), "0x%x",lproductid);

    return XML_RESULT_SUCCEED;
}



void xml_initglobal(void)
{
    /* ??????Product???????? */
    xml_ctrl.g_stlxmlproductinfo.envalidnode      = XML_PRODUCT_NODE_STATUS_INVALID;
    xml_ctrl.g_stlxmlproductinfo.enxmldecodestate = XML_DECODE_STATUS_DECODING;
    xml_ctrl.g_stlxmlproductinfo.enxmldecodejump  = XML_DECODE_STATUS_JUMP_BUTT;
    xml_ctrl.g_stlxmlproductinfo.ulnodelevel      = 0; /* ????product?????????????? */

    /* ?????????????? */
    g_stlxmlstatus = XML_ANASTT_ORIGINAL;

    /* ????????????XML?????????? */
    xml_ctrl.g_stlxml_lineno    = 1;

    /*????????????????????*/
    xml_ctrl.g_stlxmljumpflag   = XML_DECODE_STATUS_JUMP_BUTT;
    /*???????????????? ????????????0??????????????*/
    /* coverity[secure_coding] */
    memset(&xml_ctrl.g_stlxmljumpinfo,0,sizeof(xml_ctrl.g_stlxmljumpinfo));

    return ;
}


/*
 * search product info through product id
 */
s32  xml_search_productinfo(u32 product_id,u8* pdata,XNV_MAP_PRODUCT_INFO* product_info)
{
    u32 low;
    u32 high;
    u32 mid;
    u32 offset;
    XNV_MAP_HEAD_STRU * map_file = (XNV_MAP_HEAD_STRU*)(unsigned long)pdata;

    low  = 1;
    high = map_file->product_num;

    while(low <= high)
    {
        mid = (low+high)/2;
        offset = (u32)(sizeof(XNV_MAP_HEAD_STRU)+(mid-1)*sizeof(XNV_MAP_PRODUCT_INFO));
        /* coverity[secure_coding] */
        memcpy((u8*)product_info,(u8*)pdata+offset,sizeof(XNV_MAP_PRODUCT_INFO));

        if(product_id < product_info->product_id)
        {
            high = mid - 1;
        }
        else if(product_id > product_info->product_id)
        {
            low = mid + 1;
        }
        else
        {
            return 0;
        }
    }
    return -1;
}

void xml_getjumpinfo(s8* map_path)
{
    FILE* fp;
    u32 ret;
    u32 file_len;
    XNV_MAP_HEAD_STRU* map_file;
    XNV_MAP_PRODUCT_INFO product_info = {0};

    if(!map_path)
        return;
    fp = nv_file_open(map_path,NV_FILE_READ);
    if(!fp)
        return;
    file_len = nv_get_file_len(fp);
    if(file_len == NV_ERROR)
    {
        nv_file_close(fp);
        return;
    }

    /* coverity[negative_returns] */
    map_file = (XNV_MAP_HEAD_STRU*)nv_malloc((size_t)file_len);
    if(!map_file)
    {
        nv_file_close(fp);
        return;
    }

    ret = nv_file_read((u8*)map_file,1,file_len,fp);
    if(ret != file_len)
    {
        nv_record("%s  ret :%d,file_len :0x%x\n",__func__,ret,file_len);
        goto out;
    }
    ret = (u32)xml_search_productinfo((u32)(xml_ctrl.g_stlxmlproductid),(u8*)map_file,&product_info);
    if(ret)
        goto out;
    /* coverity[secure_coding] */
    memcpy(&xml_ctrl.g_stlxmljumpinfo,&product_info,sizeof(product_info));
    xml_ctrl.g_stlxmljumpflag = XML_DECODE_STATUS_JUMP;

    nv_file_close(fp);
    nv_free(map_file);
    return;

out:
    nv_file_close(fp);
    nv_free(map_file);
    /* coverity[secure_coding] */
    memset(&xml_ctrl.g_stlxmljumpinfo,0,sizeof(xml_ctrl.g_stlxmljumpinfo));
    xml_ctrl.g_stlxmljumpflag = XML_DECODE_STATUS_NOJUMP;
    return;
}



XML_RESULT_ENUM_UINT32 xml_procinit(s8* map_path)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ?????????????? */
    xml_initglobal();

    /* ??????????????product id*/   /*? UDP*/
    xml_getproductid();

    /*????????????????*/
    xml_getjumpinfo(map_path);

    /* ???????????? */
    returnval = xml_createanode();

    if (XML_RESULT_SUCCEED != returnval)
    {
        return returnval;
    }

    /* ????????NV Item????????????,+1???????????????????? */  /*????????????*/
    xml_ctrl.g_puclnvitem = (u8 *)nv_malloc((size_t)(XML_NODE_VALUE_BUFF_LENGTH_ORIGINAL+1));
    if (NULL == xml_ctrl.g_puclnvitem)
    {
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_MALLOC);

        return XML_RESULT_FALIED_MALLOC;
    }

    /* ????????????????????????,+1???????????????????? */ /*?????????????*/
    xml_ctrl.g_pclfilereadbuff = (s8*)nv_malloc((size_t)(XML_FILE_READ_BUFF_SIZE + 1));
    if (NULL == xml_ctrl.g_pclfilereadbuff)
    {
        nv_free(xml_ctrl.g_puclnvitem);
        xml_write_error_log(__LINE__, 0, XML_RESULT_FALIED_MALLOC);
        return XML_RESULT_FALIED_MALLOC;
    }



    return XML_RESULT_SUCCEED;
}


void xml_freemem(void)
{
    u32 ulPropertyIndex;

    for(ulPropertyIndex = 0; ulPropertyIndex < XML_NODE_PROPERTY_NUM; ulPropertyIndex++)
    {
        nv_free(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname);

        nv_free(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyvalue);
    }
    nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel);

    nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend);

    nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodevalue);

    nv_free(xml_ctrl.g_puclnvitem);

    nv_free(xml_ctrl.g_pclfilereadbuff);
}

void xml_decodefail_freemem(void)
{
    u32 ulPropertyIndex;

    for(ulPropertyIndex = 0; ulPropertyIndex < XML_NODE_PROPERTY_NUM; ulPropertyIndex++)
    {
        if(NULL != xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname)
        {
            nv_free(xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname);
            xml_ctrl.g_stlxmlcurrentnode.stproperty[ulPropertyIndex].pcpropertyname = NULL;
        }
    }

    if(NULL != xml_ctrl.g_stlxmlcurrentnode.pcnodelabel)
    {
        nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodelabel);
        xml_ctrl.g_stlxmlcurrentnode.pcnodelabel = NULL;
    }

    if(NULL != xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend)
    {
        nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend);
        xml_ctrl.g_stlxmlcurrentnode.pcnodelabelend = NULL;
    }

    if(NULL != xml_ctrl.g_stlxmlcurrentnode.pcnodevalue)
    {
        nv_free(xml_ctrl.g_stlxmlcurrentnode.pcnodevalue);
        xml_ctrl.g_stlxmlcurrentnode.pcnodevalue = NULL;
    }

    if(NULL != xml_ctrl.g_puclnvitem)
    {
        nv_free(xml_ctrl.g_puclnvitem);
        xml_ctrl.g_puclnvitem = NULL;
    }

    if(NULL != xml_ctrl.g_pclfilereadbuff)
    {
        nv_free(xml_ctrl.g_pclfilereadbuff);
        xml_ctrl.g_pclfilereadbuff = NULL;
    }

}


u32 xml_decode_main(FILE* fp,s8* map_path,u32 card_type)
{
    XML_RESULT_ENUM_UINT32 returnval;

    /* ???????????????????????? */
    returnval = xml_procinit(map_path);
    xml_ctrl.card_type = card_type;

    if (XML_RESULT_SUCCEED != returnval)
    {
        nv_printf("xml_procinit error! returnval = %d\n",returnval);
        goto out;
    }

    /* ????xnv.xml????  */
    returnval = xml_decode_xml_file(fp);

    if (XML_RESULT_SUCCEED != returnval)
    {
        nv_printf(" xml_decode_xml_file: returnval = %d!\n",returnval);
        goto out;
    }
    /* ???????????????? */
    xml_freemem();
    /*????????????????????????????1*/
    xml_ctrl.card_type = NV_USIMM_CARD_1;
    return NV_OK;
out:
    xml_help();
	xml_decodefail_freemem();
    return returnval;
}


/*****************************************************************************
* ?? ?? ??  : xml_nv_boot_search_byid
*
* ????????  : ????id ??itemid??NV??????????
*
* ????????  : itemid: NV id
*           : pdata :
* ????????  : ref_info  : NV????
*           : file_info : NV??????????????
*           : ref_offset: ref_info??????????
* ?? ?? ??  : void
*****************************************************************************/
u32 xml_nv_search_byid(u32 itemid,u8* pdata,nv_item_info_s* ref_info,nv_file_info_s* file_info, u8** ref_offset)
{
    u32 low;
    u32 high;
    u32 mid;
    u32 offset;
    nv_ctrl_info_s* ctrl_info = (nv_ctrl_info_s*)(unsigned long)pdata;

    high = ctrl_info->ref_count;
    low  = 1;

    while(low <= high)
    {
        mid = (low+high)/2;

        offset = (u32)(NV_GLOBAL_CTRL_INFO_SIZE +NV_GLOBAL_FILE_ELEMENT_SIZE*ctrl_info->file_num + (mid -1)*NV_REF_LIST_ITEM_SIZE);
        /* coverity[secure_coding] */
        memcpy((u8*)ref_info,(u8*)ctrl_info+offset,NV_REF_LIST_ITEM_SIZE);

        if(itemid < ref_info->itemid)
        {
            high = mid-1;
        }
        else if(itemid > ref_info->itemid)
        {
            low = mid+1;
        }
        else
        {
            *ref_offset = (u8*)ctrl_info + offset;
            offset = NV_GLOBAL_CTRL_INFO_SIZE + NV_GLOBAL_FILE_ELEMENT_SIZE*(ref_info->file_id -1);
            /* coverity[secure_coding] */
            memcpy((u8*)file_info,(u8*)ctrl_info+offset,NV_GLOBAL_FILE_ELEMENT_SIZE);
            return NV_OK;
        }
    }
    return BSP_ERR_NV_NO_THIS_ID;

}
/*****************************************************************************
* ?? ?? ??  : xml_nv_boot_write_priority
*
* ????????  : ??priority????????????ref_offset????????????
*
* ????????  : ref_offset:??????????????nv??????????
*           : priority  :??????????????
*
* ????????  : ??
*
* ?? ?? ??  : void
*****************************************************************************/
void xml_nv_write_priority(u8* ref_offset , u16 priority)
{
    nv_item_info_s *ref_info;
    
    ref_info = (nv_item_info_s *)(unsigned long)ref_offset;
    ref_info->priority = priority;
    return;
}
/*
 * copy user buff to global ddr,used to write nv data to ddr
 * &file_id :file id
 * &offset:  offset of global file ddr
 */
u32 xml_nv_write_to_mem(u8* pdata,u32 size,u32 file_id,u32 offset)
{
    nv_global_info_s* ddr_info = (nv_global_info_s*)NV_GLOBAL_INFO_ADDR;

    if(!file_id)
    {
        nv_printf("para error\n");
        return BSP_ERR_NV_INVALID_PARAM;
    }
    if(offset > ddr_info->file_info[file_id-1].size)
    {
        nv_printf("[%s]:offset 0x%x\n",__FUNCTION__,offset);
        return BSP_ERR_NV_FILE_ERROR;
    }
    /* coverity[secure_coding] */
    memcpy((u8*)(NV_GLOBAL_CTRL_INFO_ADDR+ddr_info->file_info[file_id-1].offset + offset),pdata,(size_t)size);

    return NV_OK;
}


void xml_help(void)
{
    nv_printf("xml_ctrl.g_stlxmlerrorinfo.ulxmlline  =  %d\n",xml_ctrl.g_stlxmlerrorinfo.ulxmlline);
    nv_printf("xml_ctrl.g_stlxmlerrorinfo.ulstatus   =  %d\n",xml_ctrl.g_stlxmlerrorinfo.ulstatus);
    nv_printf("xml_ctrl.g_stlxmlerrorinfo.ulcodeline =  %d\n",xml_ctrl.g_stlxmlerrorinfo.ulcodeline);
    nv_printf("xml_ctrl.g_stlxmlerrorinfo.usnvid     =  %d\n",xml_ctrl.g_stlxmlerrorinfo.usnvid);
    nv_printf("xml_ctrl.g_stlxmlerrorinfo.ulresult   =  %d\n",xml_ctrl.g_stlxmlerrorinfo.ulresult);
    nv_printf("\n");
    nv_printf("g_stlxmlstatus                        = %d\n",g_stlxmlstatus);
    nv_printf("xml_ctrl.g_stlxml_lineno              = %d\n",xml_ctrl.g_stlxml_lineno);
    nv_printf("acProductId                           = %d\n",xml_ctrl.g_stlxmlproductid);
    nv_printf("xml_ctrl.g_stlxmlproductinfo.envalidnode   =  %d\n",xml_ctrl.g_stlxmlproductinfo.envalidnode);
}

EXPORT_SYMBOL(xml_analyse);
EXPORT_SYMBOL(xml_help);
EXPORT_SYMBOL(xml_write_error_log);
EXPORT_SYMBOL(xml_procinit);










