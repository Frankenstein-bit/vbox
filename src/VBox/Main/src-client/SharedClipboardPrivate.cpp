/* $Id$ */
/** @file
 * Private Shared Clipboard code.
 */

/*
 * Copyright (C) 2019 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#define LOG_GROUP LOG_GROUP_SHARED_CLIPBOARD
#include "LoggingNew.h"

#include <VMMDev.h>

#include <VBox/err.h>

#ifdef VBOX_WITH_SHARED_CLIPBOARD_TRANSFERS
# include <VBox/GuestHost/SharedClipboard-transfers.h>
#endif

#include "ConsoleImpl.h"
#include "SharedClipboardPrivate.h"

SharedClipboard* SharedClipboard::s_pInstance = NULL;

SharedClipboard::SharedClipboard(const ComObjPtr<Console> &pConsole)
    : m_pConsole(pConsole)
{
    LogFlowFuncEnter();
}

SharedClipboard::~SharedClipboard(void)
{
    LogFlowFuncEnter();
}

int SharedClipboard::hostCall(uint32_t u32Function, uint32_t cParms, PVBOXHGCMSVCPARM paParms) const
{
    /* Forward the information to the VMM device. */
    Assert(!m_pConsole.isNull());
    VMMDev *pVMMDev = m_pConsole->i_getVMMDev();
    if (!pVMMDev)
        return VERR_COM_OBJECT_NOT_FOUND;

    return pVMMDev->hgcmHostCall("VBoxSharedClipboard", u32Function, cParms, paParms);
}

/* static */
DECLCALLBACK(int) SharedClipboard::hostServiceCallback(void *pvExtension, uint32_t u32Function,
                                                       void *pvParms, uint32_t cbParms)
{
    LogFlowFunc(("pvExtension=%p, u32Function=%RU32, pvParms=%p, cbParms=%RU32\n",
                 pvExtension, u32Function, pvParms, cbParms));

    RT_NOREF(u32Function, pvParms, cbParms);

    SharedClipboard *pThis = reinterpret_cast<SharedClipboard *>(pvExtension);
    AssertPtrReturn(pThis, VERR_INVALID_POINTER);

    int rc = VINF_SUCCESS;

    LogFlowFuncLeaveRC(rc);
    return rc;
}

