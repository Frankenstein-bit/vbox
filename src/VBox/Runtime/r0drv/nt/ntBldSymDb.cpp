﻿/* $Id$ */
/** @file
 * IPRT - RTDirCreateUniqueNumbered, generic implementation.
 */

/*
 * Copyright (C) 2013 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */


/*******************************************************************************
*   Header Files                                                               *
*******************************************************************************/
#include <Windows.h>
#include <Dbghelp.h>

#include <iprt/alloca.h>
#include <iprt/dir.h>
#include <iprt/initterm.h>
#include <iprt/list.h>
#include <iprt/mem.h>
#include <iprt/message.h>
#include <iprt/path.h>
#include <iprt/stream.h>
#include <iprt/string.h>
#include <iprt/err.h>

#include "r0drv/nt/symdb.h"


/*******************************************************************************
*   Structures and Typedefs                                                    *
*******************************************************************************/
/** A structure member we're interested in. */
typedef struct MYMEMBER
{
    /** The member name. */
    const char * const          pszName;
    /** Reserved.  */
    uint32_t    const           fFlags;
    /** The offset of the member. UINT32_MAX if not found. */
    uint32_t                    off;
    /** The size of the member. */
    uint32_t                    cb;
    /** Alternative names, optional.
     * This is a string of zero terminated strings, ending with an zero length
     * string (or double '\\0' if you like). */
    const char * const          pszzAltNames;
} MYMEMBER;
/** Pointer to a member we're interested. */
typedef MYMEMBER *PMYMEMBER;

/** Members we're interested in. */
typedef struct MYSTRUCT
{
    /** The structure name. */
    const char * const          pszName;
    /** Array of members we're interested in. */
    MYMEMBER                   *paMembers;
    /** The number of members we're interested in. */
    uint32_t const              cMembers;
    /** Reserved.  */
    uint32_t const              fFlags;
} MYSTRUCT;

/** Set of structures for one kernel. */
typedef struct MYSET
{
    /** The list entry. */
    RTLISTNODE      ListEntry;
    /** The source PDB. */
    char           *pszPdb;
    /** The OS version we've harvested structs for */
    RTNTSDBOSVER    OsVerInfo;
    /** The structures and their member. */
    MYSTRUCT        aStructs[1];
} MYSET;
/** Pointer a set of structures for one kernel. */
typedef MYSET *PMYSET;


/*******************************************************************************
*   Global Variables                                                           *
*******************************************************************************/
/** Set if verbose operation.*/
static bool     g_fVerbose = false;

/** The members of the KPRCB structure that we're interested in. */
static MYMEMBER g_aKprcbMembers[] =
{
    { "QuantumEnd",         0,  UINT32_MAX, UINT32_MAX, NULL },
    { "DpcQueueDepth",      0,  UINT32_MAX, UINT32_MAX, "DpcData[0].DpcQueueDepth\0" },
    { "VendorString",       0,  UINT32_MAX, UINT32_MAX, NULL },
};

/** The structures we're interested in. */
static MYSTRUCT g_aStructs[] =
{
    { "_KPRCB", &g_aKprcbMembers[0], RT_ELEMENTS(g_aKprcbMembers), 0 },
};

/** List of data we've found. This is sorted by version info. */
static RTLISTANCHOR g_SetList;





/**
 * For debug/verbose output.
 *
 * @param   pszFormat           The format string.
 * @param   ...                 The arguments referenced in the format string.
 */
static void MyDbgPrintf(const char *pszFormat, ...)
{
    if (g_fVerbose)
    {
        va_list va;
        va_start(va, pszFormat);
        RTPrintf("debug: ");
        RTPrintfV(pszFormat, va);
        va_end(va);
    }
}


/**
 * Returns the name we wish to use in the C code.
 * @returns Structure name.
 * @param   pStruct             The structure descriptor.
 */
static const char *figureCStructName(MYSTRUCT const *pStruct)
{
    const char *psz = pStruct->pszName;
    while (*psz == '_')
        psz++;
    return psz;
}


/**
 * Returns the name we wish to use in the C code.
 * @returns Member name.
 * @param   pStruct             The member descriptor.
 */
static const char *figureCMemberName(MYMEMBER const *pMember)
{
    return pMember->pszName;
}


/**
 * Creates a MYSET with copies of all the data and inserts it into the
 * g_SetList in a orderly fashion.
 *
 * @param   pOut        The output stream.
 */
static void generateHeader(PRTSTREAM pOut)
{
    RTStrmPrintf(pOut,
                 "/* $" "I" "d" ": $ */\n" /* avoid it being expanded */
                 "/** @file\n"
                 " * IPRT - NT kernel type helpers - Autogenerated, do NOT edit.\n"
                 " */\n"
                 "\n"
                 "/*\n"
                 " * Copyright (C) 2013 Oracle Corporation\n"
                 " *\n"
                 " * This file is part of VirtualBox Open Source Edition (OSE), as\n"
                 " * available from http://www.virtualbox.org. This file is free software;\n"
                 " * you can redistribute it and/or modify it under the terms of the GNU\n"
                 " * General Public License (GPL) as published by the Free Software\n"
                 " * Foundation, in version 2 as it comes in the \"COPYING\" file of the\n"
                 " * VirtualBox OSE distribution. VirtualBox OSE is distributed in the\n"
                 " * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.\n"
                 " *\n"
                 " * The contents of this file may alternatively be used under the terms\n"
                 " * of the Common Development and Distribution License Version 1.0\n"
                 " * (CDDL) only, as it comes in the \"COPYING.CDDL\" file of the\n"
                 " * VirtualBox OSE distribution, in which case the provisions of the\n"
                 " * CDDL are applicable instead of those of the GPL.\n"
                 " *\n"
                 " * You may elect to license modified versions of this file under the\n"
                 " * terms and conditions of either the GPL or the CDDL or both.\n"
                 " */\n"
                 "\n"
                 "\n"
                 "#ifndef ___r0drv_nt_symdbdata_h\n"
                 "#define ___r0drv_nt_symdbdata_h\n"
                 "\n"
                 "#include \"r0drv/nt/symdb.h\"\n"
                 "\n"
                 );

    /*
     * Generate types.
     */
    for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
    {
        const char *pszStructName = figureCStructName(&g_aStructs[i]);

        RTStrmPrintf(pOut,
                     "typedef struct RTNTSDBTYPE_%s\n"
                     "{\n",
                     pszStructName);
        PMYMEMBER paMembers = g_aStructs[i].paMembers;
        for (uint32_t j = 0; j < g_aStructs->cMembers; j++)
        {
            const char *pszMemName = figureCMemberName(&paMembers[j]);
            RTStrmPrintf(pOut,
                         "    uint32_t off%s;\n"
                         "    uint32_t cb%s;\n",
                         pszMemName, pszMemName);
        }

        RTStrmPrintf(pOut,
                     "} RTNTSDBTYPE_%s;\n"
                     "\n",
                     pszStructName);
    }

    RTStrmPrintf(pOut,
                 "\n"
                 "typedef struct RTNTSDBSET\n"
                 "{\n"
                 "    RTNTSDBOSVER%-20s OsVerInfo;\n", "");
    for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
    {
        const char *pszStructName = figureCStructName(&g_aStructs[i]);
        RTStrmPrintf(pOut, "    RTNTSDBTYPE_%-20s %s\n", pszStructName, pszStructName);
    }
    RTStrmPrintf(pOut,
                 "} RTNTSDBSET;\n"
                 "typedef RTNTSDBSET const *PCRTNTSDBSET;\n"
                 "\n");

    /*
     * Output the data.
     */
    RTStrmPrintf(pOut,
                 "\n"
                 "#ifndef RTNTSDB_NO_DATA\n"
                 "const RTNTSDBSET g_rtNtSdbSets[] = \n"
                 "{\n");
    PMYSET pSet;
    RTListForEach(&g_SetList, pSet, MYSET, ListEntry)
    {
        RTStrmPrintf(pOut,
                     "    {   /* Source: %s */\n"
                     "        /*.OsVerInfo = */\n"
                     "        {\n"
                     "            /* .uMajorVer = */ %u,\n"
                     "            /* .uMinorVer = */ %u,\n"
                     "            /* .fChecked  = */ %s,\n"
                     "            /* .fSmp      = */ %s,\n"
                     "            /* .uCsdNo    = */ %u,\n"
                     "            /* .uBuildNo  = */ %u,\n"
                     "        },\n",
                     pSet->pszPdb,
                     pSet->OsVerInfo.uMajorVer,
                     pSet->OsVerInfo.uMinorVer,
                     pSet->OsVerInfo.fChecked ? "true" : "false",
                     pSet->OsVerInfo.fSmp     ? "true" : "false",
                     pSet->OsVerInfo.uCsdNo,
                     pSet->OsVerInfo.uBuildNo);
        for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
        {
            const char *pszStructName = figureCStructName(&g_aStructs[i]);
            RTStrmPrintf(pOut,
                         "        /* .%s = */\n"
                         "        {\n", pszStructName);
            PMYMEMBER paMembers = g_aStructs[i].paMembers;
            for (uint32_t j = 0; j < g_aStructs->cMembers; j++)
            {
                const char *pszMemName = figureCMemberName(&paMembers[j]);
                RTStrmPrintf(pOut,
                             "            /* .off%-25s = */ %#06x,\n"
                             "            /* .cb%-26s = */ %#06x,\n",
                             pszMemName, paMembers[j].off,
                             pszMemName, paMembers[j].cb);
            }
            RTStrmPrintf(pOut,
                         "        },\n");
        }
        RTStrmPrintf(pOut,
                     "    },\n");
    }

    RTStrmPrintf(pOut,
                 "};\n"
                 "#endif /* !RTNTSDB_NO_DATA */\n"
                 "\n");

    RTStrmPrintf(pOut, "\n#endif\n");
}


/**
 * Creates a MYSET with copies of all the data and inserts it into the
 * g_SetList in a orderly fashion.
 *
 * @returns Fully complained exit code.
 * @param   pOsVerInfo      The OS version info.
 */
static RTEXITCODE saveStructures(PRTNTSDBOSVER pOsVerInfo, const char *pszPdb)
{
    /*
     * Allocate one big chunk, figure it's size once.
     */
    static size_t s_cbNeeded = 0;
    if (s_cbNeeded == 0)
    {
        s_cbNeeded = RT_OFFSETOF(MYSET, aStructs[RT_ELEMENTS(g_aStructs)]);
        for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
            s_cbNeeded += sizeof(MYMEMBER) * g_aStructs[i].cMembers;
    }

    size_t cbPdb = strlen(pszPdb) + 1;
    PMYSET pSet = (PMYSET)RTMemAlloc(s_cbNeeded + cbPdb);
    if (!pSet)
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "Out of memory!\n");

    /*
     * Copy over the data.
     */
    memcpy(&pSet->OsVerInfo, pOsVerInfo, sizeof(pSet->OsVerInfo));
    memcpy(&pSet->aStructs[0], g_aStructs, sizeof(g_aStructs));

    PMYMEMBER pDst = (PMYMEMBER)&pSet->aStructs[RT_ELEMENTS(g_aStructs)];
    for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
    {
        pSet->aStructs[i].paMembers = pDst;
        memcpy(pDst, g_aStructs[i].paMembers, g_aStructs[i].cMembers * sizeof(*pDst));
        pDst += g_aStructs[i].cMembers;
    }

    pSet->pszPdb = (char *)pDst;
    memcpy(pDst, pszPdb, cbPdb);

    /*
     * Link it.
     */
    PMYSET pInsertBefore;
    RTListForEach(&g_SetList, pInsertBefore, MYSET, ListEntry)
    {
        if (rtNtOsVerInfoCompare(&pSet->OsVerInfo, &pInsertBefore->OsVerInfo) >= 0)
            break;
    }
    RTListPrepend(&pInsertBefore->ListEntry, &pSet->ListEntry);

    return RTEXITCODE_SUCCESS;
}


/**
 * Checks that we found everything.
 *
 * @returns Fully complained exit code.
 */
static RTEXITCODE checkThatWeFoundEverything(void)
{
    RTEXITCODE rcExit = RTEXITCODE_SUCCESS;
    for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
    {
        PMYMEMBER paMembers = g_aStructs[i].paMembers;
        uint32_t  j         = g_aStructs[i].cMembers;
        while (j-- > 0)
        {
            if (paMembers[j].off == UINT32_MAX)
                rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, " Missing %s::%s\n", g_aStructs[i].pszName, paMembers[j].pszName);
        }
    }
    return rcExit;
}


/**
 * Matches the member against what we're looking for.
 *
 * @returns Number of hits.
 * @param   cWantedMembers  The number members in paWantedMembers.
 * @param   paWantedMembers The members we're looking for.
 * @param   pszPrefix       The member name prefix.
 * @param   pszMember       The member name.
 * @param   offMember       The member offset.
 * @param   cbMember        The member size.
 */
static uint32_t matchUpStructMembers(unsigned cWantedMembers, PMYMEMBER paWantedMembers,
                                     const char *pszPrefix, const char *pszMember,
                                     uint32_t offMember, uint32_t cbMember)
{
    size_t   cchPrefix = strlen(pszPrefix);
    uint32_t cHits     = 0;
    uint32_t iMember   = cWantedMembers;
    while (iMember-- > 0)
    {
        if (   !strncmp(pszPrefix, paWantedMembers[iMember].pszName, cchPrefix)
            && !strcmp(pszMember, paWantedMembers[iMember].pszName + cchPrefix))
        {
            paWantedMembers[iMember].off = offMember;
            paWantedMembers[iMember].cb  = cbMember;
            cHits++;
        }
        else if (paWantedMembers[iMember].pszzAltNames)
        {
            char const *pszCur = paWantedMembers[iMember].pszzAltNames;
            while (*pszCur)
            {
                size_t cchCur = strlen(pszCur);
                if (   !strncmp(pszPrefix, pszCur, cchPrefix)
                    && !strcmp(pszMember, pszCur + cchPrefix))
                {
                    paWantedMembers[iMember].off = offMember;
                    paWantedMembers[iMember].cb  = cbMember;
                    cHits++;
                    break;
                }
                pszCur += cchCur + 1;
            }
        }
    }
    return cHits;
}


/**
 * Resets the writable structure members prior to processing a PDB.
 *
 * While processing the PDB, will fill in the sizes and offsets of what we find.
 * Afterwards we'll use look for reset values to see that every structure and
 * member was located successfully.
 */
static void resetMyStructs(void)
{
    for (uint32_t i = 0; i < RT_ELEMENTS(g_aStructs); i++)
    {
        PMYMEMBER paMembers = g_aStructs[i].paMembers;
        uint32_t  j         = g_aStructs[i].cMembers;
        while (j-- > 0)
        {
            paMembers[j].off = UINT32_MAX;
            paMembers[j].cb  = UINT32_MAX;
        }
    }
}


/**
 * Find members in the specified structure type (@a idxType).
 *
 * @returns Fully bitched exit code.
 * @param   hFake           Fake process handle.
 * @param   uModAddr        The module address.
 * @param   idxType         The type index of the structure which members we're
 *                          going to process.
 * @param   cWantedMembers  The number of wanted members.
 * @param   paWantedMembers The wanted members.  This will be modified.
 * @param   offDisp         Displacement when calculating member offsets.
 * @param   pszStructNm     The top level structure name.
 * @param   pszPrefix       The member name prefix.
 * @param   pszLogTag       The log tag.
 */
static RTEXITCODE findMembers(HANDLE hFake, uint64_t uModAddr, uint32_t idxType,
                              uint32_t cWantedMembers, PMYMEMBER paWantedMembers,
                              uint32_t offDisp, const char *pszStructNm, const char *pszPrefix, const char *pszLogTag)
{
    RTEXITCODE  rcExit   = RTEXITCODE_SUCCESS;

    DWORD cChildren = 0;
    if (!SymGetTypeInfo(hFake, uModAddr, idxType, TI_GET_CHILDRENCOUNT, &cChildren))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "%s: TI_GET_CHILDRENCOUNT failed on _KPRCB: %u\n", pszLogTag, GetLastError());

    MyDbgPrintf(" %s: cChildren=%u (%#x)\n", pszStructNm, cChildren);
    TI_FINDCHILDREN_PARAMS *pChildren;
    pChildren = (TI_FINDCHILDREN_PARAMS *)alloca(RT_OFFSETOF(TI_FINDCHILDREN_PARAMS, ChildId[cChildren]));
    pChildren->Start = 0;
    pChildren->Count = cChildren;
    if (!SymGetTypeInfo(hFake, uModAddr, idxType, TI_FINDCHILDREN, pChildren))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "%s: TI_FINDCHILDREN failed on _KPRCB: %u\n", pszLogTag, GetLastError());

    for (uint32_t i = 0; i < cChildren; i++)
    {
        //MyDbgPrintf(" %s: child#%u: TypeIndex=%u\n", pszStructNm, i, pChildren->ChildId[i]);
        IMAGEHLP_SYMBOL_TYPE_INFO enmErr;
        PWCHAR      pwszMember = NULL;
        uint32_t    idxRefType = 0;
        uint32_t    offMember = 0;
        uint64_t    cbMember = 0;
        uint32_t    cMemberChildren = 0;
        if (   SymGetTypeInfo(hFake, uModAddr, pChildren->ChildId[i], enmErr = TI_GET_SYMNAME, &pwszMember)
            && SymGetTypeInfo(hFake, uModAddr, pChildren->ChildId[i], enmErr = TI_GET_OFFSET, &offMember)
            && SymGetTypeInfo(hFake, uModAddr, pChildren->ChildId[i], enmErr = TI_GET_TYPE, &idxRefType)
            && SymGetTypeInfo(hFake, uModAddr, idxRefType, enmErr = TI_GET_LENGTH, &cbMember)
            && SymGetTypeInfo(hFake, uModAddr, idxRefType, enmErr = TI_GET_CHILDRENCOUNT, &cMemberChildren)
            )
        {
            offMember += offDisp;

            char *pszMember;
            int rc = RTUtf16ToUtf8(pwszMember, &pszMember);
            if (RT_SUCCESS(rc))
            {
                matchUpStructMembers(cWantedMembers, paWantedMembers, pszPrefix, pszMember, offMember, cbMember);

                /*
                 * Gather more info and do some debug printing. We'll use some
                 * of this info below when recursing into sub-structures
                 * and arrays.
                 */
                uint32_t fNested      = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_NESTED, &fNested);
                uint32_t uDataKind    = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_DATAKIND, &uDataKind);
                uint32_t uBaseType    = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_BASETYPE, &uBaseType);
                uint32_t uMembTag     = 0; SymGetTypeInfo(hFake, uModAddr, pChildren->ChildId[i], TI_GET_SYMTAG, &uMembTag);
                uint32_t uBaseTag     = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_SYMTAG, &uBaseTag);
                uint32_t cElements    = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_COUNT, &cElements);
                uint32_t idxArrayType = 0; SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_ARRAYINDEXTYPEID, &idxArrayType);
                MyDbgPrintf(" %#06x LB %#06llx %c%c %2d %2d %2d %2d %2d %4d %s::%s%s\n",
                            offMember, cbMember,
                            cMemberChildren > 0 ? 'c' : '-',
                            fNested        != 0 ? 'n' : '-',
                            uDataKind,
                            uBaseType,
                            uMembTag,
                            uBaseTag,
                            cElements,
                            idxArrayType,
                            pszStructNm,
                            pszPrefix,
                            pszMember);

                /*
                 * Recurse into children.
                 */
                if (cMemberChildren > 0)
                {
                    size_t cbNeeded = strlen(pszMember) + strlen(pszPrefix) + sizeof(".");
                    char *pszSubPrefix = (char *)RTMemTmpAlloc(cbNeeded);
                    if (pszSubPrefix)
                    {
                        strcat(strcat(strcpy(pszSubPrefix, pszPrefix), pszMember), ".");
                        RTEXITCODE rcExit2 = findMembers(hFake, uModAddr, idxRefType, cWantedMembers,
                                                         paWantedMembers, offMember,
                                                         pszStructNm,
                                                         pszSubPrefix,
                                                         pszLogTag);
                        if (rcExit2 != RTEXITCODE_SUCCESS)
                            rcExit = rcExit2;
                        RTMemTmpFree(pszSubPrefix);
                    }
                    else
                        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "out of memory\n");
                }
                /*
                 * Recurse into arrays too.
                 */
                else if (cElements > 0 && idxArrayType > 0)
                {
                    BOOL fRc;
                    uint32_t idxElementRefType = 0;
                    fRc = SymGetTypeInfo(hFake, uModAddr, idxRefType, TI_GET_TYPE, &idxElementRefType); Assert(fRc);
                    uint64_t cbElement = cbMember / cElements;
                    fRc = SymGetTypeInfo(hFake, uModAddr, idxElementRefType, TI_GET_LENGTH, &cbElement); Assert(fRc);
                    MyDbgPrintf("idxArrayType=%u idxElementRefType=%u cbElement=%u\n", idxArrayType, idxElementRefType, cbElement);

                    size_t cbNeeded = strlen(pszMember) + strlen(pszPrefix) + sizeof("[xxxxxxxxxxxxxxxx].");
                    char *pszSubPrefix = (char *)RTMemTmpAlloc(cbNeeded);
                    if (pszSubPrefix)
                    {
                        for (uint32_t iElement = 0; iElement < cElements; iElement++)
                        {
                            RTStrPrintf(pszSubPrefix, cbNeeded, "%s%s[%u].", pszPrefix, pszMember, iElement);
                            RTEXITCODE rcExit2 = findMembers(hFake, uModAddr, idxElementRefType, cWantedMembers,
                                                             paWantedMembers,
                                                             offMember + iElement * cbElement,
                                                             pszStructNm,
                                                             pszSubPrefix,
                                                             pszLogTag);
                            if (rcExit2 != RTEXITCODE_SUCCESS)
                                rcExit = rcExit2;
                        }
                        RTMemTmpFree(pszSubPrefix);
                    }
                    else
                        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "out of memory\n");
                }

                RTStrFree(pszMember);
            }
            else
                rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "%s: RTUtf16ToUtf8 failed on %s child#%u: %Rrc\n",
                                        pszLogTag, pszStructNm, i, rc);
        }
        /* TI_GET_OFFSET fails on bitfields, so just ignore+skip those. */
        else if (enmErr != TI_GET_OFFSET || GetLastError() != ERROR_INVALID_FUNCTION)
            rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "%s: SymGetTypeInfo(,,,%d,) failed on %s child#%u: %u\n",
                                    pszLogTag, enmErr, pszStructNm, i, GetLastError());
        LocalFree(pwszMember);
    } /* For each child. */

    return rcExit;
}


/**
 * Lookup up structures and members in the given module.
 *
 * @returns Fully bitched exit code.
 * @param   hFake           Fake process handle.
 * @param   uModAddr        The module address.
 * @param   pszLogTag       The log tag.
 */
static RTEXITCODE findStructures(HANDLE hFake, uint64_t uModAddr, const char *pszLogTag)
{
    RTEXITCODE   rcExit   = RTEXITCODE_SUCCESS;
    PSYMBOL_INFO pSymInfo = (PSYMBOL_INFO)alloca(sizeof(*pSymInfo));
    for (uint32_t iStruct = 0; iStruct < RT_ELEMENTS(g_aStructs); iStruct++)
    {
        pSymInfo->SizeOfStruct = sizeof(*pSymInfo);
        pSymInfo->MaxNameLen   = 0;
        if (!SymGetTypeFromName(hFake, uModAddr, g_aStructs[iStruct].pszName, pSymInfo))
            return RTMsgErrorExit(RTEXITCODE_FAILURE, "%s: Failed to find _KPRCB: %u\n", pszLogTag, GetLastError());

        MyDbgPrintf(" %s: TypeIndex=%u\n", g_aStructs[iStruct].pszName, pSymInfo->TypeIndex);
        MyDbgPrintf(" %s: Size=%u (%#x)\n", g_aStructs[iStruct].pszName, pSymInfo->Size, pSymInfo->Size);

        rcExit = findMembers(hFake, uModAddr, pSymInfo->TypeIndex,
                             g_aStructs[iStruct].cMembers, g_aStructs[iStruct].paMembers, 0 /* offDisp */,
                             g_aStructs[iStruct].pszName, "", pszLogTag);
        if (rcExit != RTEXITCODE_SUCCESS)
            return rcExit;
    } /* for each struct we want */
    return rcExit;
}


static bool strIEndsWith(const char *pszString, const char *pszSuffix)
{
    size_t cchString = strlen(pszString);
    size_t cchSuffix = strlen(pszSuffix);
    if (cchString < cchSuffix)
        return false;
    return RTStrICmp(pszString + cchString - cchSuffix, pszSuffix) == 0;
}


/**
 * Use various hysterics to figure out the OS version details from the PDB path.
 *
 * This ASSUMES quite a bunch of things:
 *      -# Working on unpacked symbol packages. This does not work for
 *         windbg symbol stores/caches.
 *      -# The symbol package has been unpacked into a directory with the same
 *       name as the symbol package (sans suffixes).
 *
 * @returns Fully complained exit code.
 * @param   pszPdb              The path to the PDB.
 * @param   pVerInfo            Where to return the version info.
 */
static RTEXITCODE FigurePdbVersionInfo(const char *pszPdb, PRTNTSDBOSVER pVerInfo)
{
    const char *pszFilename = RTPathFilename(pszPdb);

    if (!RTStrICmp(pszFilename, "ntkrnlmp.pdb"))
        pVerInfo->fSmp = true;
    else if (!RTStrICmp(pszFilename, "ntoskrnl.pdb"))
        pVerInfo->fSmp = false;
    else
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "Doesn't recognize the filename '%s'...", pszFilename);

    /* testing only */
    if (strIEndsWith(pszPdb, "ntkrnlmp.pdb\\B2DA40502FA744C18B9022FD187ADB592\\ntkrnlmp.pdb"))
    {
        pVerInfo->uMajorVer = 6;
        pVerInfo->uMinorVer = 1;
        pVerInfo->fChecked  = false;
        pVerInfo->uCsdNo    = 1;
        pVerInfo->uBuildNo  = 7601;
    }
    else
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "Giving up on '%s'...\n", pszPdb);

    return RTEXITCODE_SUCCESS;
}


/**
 * Process one PDB.
 *
 * @returns Fully bitched exit code.
 * @param   pszPdb              The path to the PDB.
 */
static RTEXITCODE processOnePdb(const char *pszPdb)
{
    /*
     * We need the size later on, so get that now and present proper IPRT error
     * info if the file is missing or inaccessible.
     */
    RTFSOBJINFO ObjInfo;
    int rc = RTPathQueryInfoEx(pszPdb, &ObjInfo, RTFSOBJATTRADD_NOTHING, RTPATH_F_FOLLOW_LINK);
    if (RT_FAILURE(rc))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "RTPathQueryInfo fail on '%s': %Rrc\n", pszPdb, rc);

    /*
     * Figure the windows version details for the given PDB.
     */
    RTNTSDBOSVER OsVerInfo;
    RTEXITCODE rcExit = FigurePdbVersionInfo(pszPdb, &OsVerInfo);
    if (rcExit != RTEXITCODE_SUCCESS)
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "Failed to figure the OS version info for '%s'.\n'", pszPdb);

    /*
     * Create a fake handle and open the PDB.
     */
    static uintptr_t s_iHandle = 0;
    HANDLE hFake = (HANDLE)++s_iHandle;
    if (!SymInitialize(hFake, NULL, FALSE))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "SymInitialied failed: %u\n", GetLastError());

    uint64_t uModAddr = UINT64_C(0x1000000);
    uModAddr = SymLoadModuleEx(hFake, NULL /*hFile*/, pszPdb, NULL /*pszModuleName*/,
                               uModAddr, ObjInfo.cbObject, NULL /*pData*/, 0 /*fFlags*/);
    if (uModAddr != 0)
    {
        MyDbgPrintf("*** uModAddr=%#llx \"%s\" ***\n", uModAddr, pszPdb);

        char szLogTag[32];
        RTStrCopy(szLogTag, sizeof(szLogTag), RTPathFilename(pszPdb));

        /*
         * Find the structures.
         */
        rcExit = findStructures(hFake, uModAddr, szLogTag);
        if (rcExit == RTEXITCODE_SUCCESS)
            rcExit = checkThatWeFoundEverything();
        if (rcExit == RTEXITCODE_SUCCESS)
        {
            /*
             * Save the details for later when we produce the header.
             */
            rcExit = saveStructures(&OsVerInfo, pszPdb);
        }
    }
    else
        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "SymLoadModuleEx failed: %u\n", GetLastError());

    if (!SymCleanup(hFake))
        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "SymCleanup failed: %u\n", GetLastError());
    return rcExit;
}


/** The size of the directory entry buffer we're using.  */
#define MY_DIRENTRY_BUF_SIZE (sizeof(RTDIRENTRYEX) + RTPATH_MAX)


/**
 * Recursively processes relevant files in the specified directory.
 *
 * @returns Fully complained exit code.
 * @param   pszDir              Pointer to the directory buffer.
 * @param   cchDir              The length of pszDir in pszDir.
 * @param   pDirEntry           Pointer to the directory buffer.
 */
static RTEXITCODE processDirSub(char *pszDir, size_t cchDir, PRTDIRENTRYEX pDirEntry)
{
    Assert(cchDir > 0); Assert(pszDir[cchDir] == '\0');

    /* Make sure we've got some room in the path, to save us extra work further down. */
    if (cchDir + 3 >= RTPATH_MAX)
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "Path too long: '%s'\n", pszDir);

    /* Open directory. */
    PRTDIR pDir;
    int rc = RTDirOpen(&pDir, pszDir);
    if (RT_FAILURE(rc))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "RTDirOpen failed on '%s': %Rrc\n", pszDir, rc);

    /* Ensure we've got a trailing slash (there is space for it see above). */
    if (!RTPATH_IS_SEP(pszDir[cchDir - 1]))
    {
        pszDir[cchDir++] = RTPATH_SLASH;
        pszDir[cchDir]   = '\0';
    }

    /*
     * Process the files and subdirs.
     */
    RTEXITCODE rcExit = RTEXITCODE_SUCCESS;
    for (;;)
    {
        /* Get the next directory. */
        size_t cbDirEntry = MY_DIRENTRY_BUF_SIZE;
        rc = RTDirReadEx(pDir, pDirEntry, &cbDirEntry, RTFSOBJATTRADD_UNIX, RTPATH_F_ON_LINK);
        if (RT_FAILURE(rc))
            break;

        /* Skip the dot and dot-dot links. */
        if (   (pDirEntry->cbName == 1 && pDirEntry->szName[0] == '.')
            || (pDirEntry->cbName == 2 && pDirEntry->szName[0] == '.' && pDirEntry->szName[1] == '.'))
            continue;

        /* Check length. */
        if (pDirEntry->cbName + cchDir + 3 >= RTPATH_MAX)
        {
            rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "Path too long: '%s' in '%.*s'\n", pDirEntry->szName, cchDir, pszDir);
            break;
        }

        if (RTFS_IS_FILE(pDirEntry->Info.Attr.fMode))
        {
            /* Is this a file which might interest us? */
            static struct { const char *psz; size_t cch; } const s_aNames[] =
            {
                RT_STR_TUPLE("ntoskrnl.dbg"),
                RT_STR_TUPLE("ntoskrnl.pdb"),
                RT_STR_TUPLE("ntkrnlmp.dbg"),
                RT_STR_TUPLE("ntkrnlmp.pdb"),
            };
            int i = RT_ELEMENTS(s_aNames);
            while (i-- > 0)
                if (   s_aNames[i].cch == pDirEntry->cbName
                    && !RTStrICmp(s_aNames[i].psz, pDirEntry->szName))
                {
                    /*
                     * Found debug info file of interest, process it.
                     */
                    memcpy(&pszDir[cchDir], pDirEntry->szName, pDirEntry->cbName + 1);
                    RTEXITCODE rcExit2 = processOnePdb(pszDir);
                    if (rcExit2 != RTEXITCODE_SUCCESS)
                        rcExit = rcExit2;
                    break;
                }
        }
        else if (RTFS_IS_DIRECTORY(pDirEntry->Info.Attr.fMode))
        {
            /*
             * Recurse into the subdirectory.
             * Note! When we get back pDirEntry will be invalid.
             */
            memcpy(&pszDir[cchDir], pDirEntry->szName, pDirEntry->cbName + 1);
            RTEXITCODE rcExit2 = processDirSub(pszDir, cchDir + pDirEntry->cbName, pDirEntry);
            if (rcExit2 != RTEXITCODE_SUCCESS)
                rcExit = rcExit2;
        }
    }
    if (rc != VERR_NO_MORE_FILES)
        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "RTDirReadEx failed: %Rrc\npszDir=%.*s", rc, cchDir, pszDir);

    rc = RTDirClose(pDir);
    if (RT_FAILURE(rc))
        rcExit = RTMsgErrorExit(RTEXITCODE_FAILURE, "RTDirClose failed: %Rrc\npszDir=%.*s", rc, cchDir, pszDir);
    return rcExit;
}


/**
 * Recursively processes relevant files in the specified directory.
 *
 * @returns Fully complained exit code.
 * @param   pszDir              The directory to search.
 */
static RTEXITCODE processDir(const char *pszDir)
{
    char szPath[RTPATH_MAX];
    int rc = RTPathAbs(pszDir, szPath, sizeof(szPath));
    if (RT_FAILURE(rc))
        return RTMsgErrorExit(RTEXITCODE_FAILURE, "RTPathAbs failed on '%s': %Rrc\n", pszDir, rc);

    union
    {
        uint8_t         abPadding[MY_DIRENTRY_BUF_SIZE];
        RTDIRENTRYEX    DirEntry;
    } uBuf;
    return processDirSub(szPath, strlen(szPath), &uBuf.DirEntry);
}


int main(int argc, char **argv)
{
    int rc = RTR3InitExe(argc, &argv, 0 /*fFlags*/);
    if (RT_FAILURE(rc))
        return RTMsgInitFailure(rc);

    RTListInit(&g_SetList);

    /*
     * Parse options.
     */


    /*
     * Do job.
     */
    RTEXITCODE rcExit = processOnePdb(argv[argc - 1]);

    if (rcExit == RTEXITCODE_SUCCESS)
    {
        generateHeader(g_pStdOut);
    }

    return rcExit;
}
