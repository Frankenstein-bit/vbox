/* $Id$ */
/** @file
 * IPRT - Multiprocessor Event Notifications, Ring-0 Driver, Solaris.
 */

/*
 * Copyright (C) 2008 Oracle Corporation
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
#include "the-solaris-kernel.h"
#include "internal/iprt.h"
#include <iprt/mp.h>

#include <iprt/err.h>
#include "r0drv/mp-r0drv.h"


static int rtMpNotificationSolarisCallback(cpu_setup_t enmSolarisEvent, int iCpu, void *pvUser)
{
    NOREF(pvUser);

    /* ASSUMES iCpu == RTCPUID */
    switch (enmSolarisEvent)
    {
        case CPU_INIT:
        case CPU_CONFIG:
        case CPU_UNCONFIG:
            break;

        case CPU_ON:
            rtMpNotificationDoCallbacks(RTMPEVENT_ONLINE, iCpu);
            break;

        case CPU_OFF:
            rtMpNotificationDoCallbacks(RTMPEVENT_OFFLINE, iCpu);
            break;

        case CPU_CPUPART_IN:
        case CPU_CPUPART_OUT:
            /** @todo are these relevant? */
            break;
    }
    return 0;
}


DECLHIDDEN(int) rtR0MpNotificationNativeInit(void)
{
    mutex_enter(&cpu_lock);
    register_cpu_setup_func(rtMpNotificationSolarisCallback, NULL);
    mutex_exit(&cpu_lock);
    return VINF_SUCCESS;
}


DECLHIDDEN(void) rtR0MpNotificationNativeTerm(void)
{
    mutex_enter(&cpu_lock);
    unregister_cpu_setup_func(rtMpNotificationSolarisCallback, NULL);
    mutex_exit(&cpu_lock);
}

