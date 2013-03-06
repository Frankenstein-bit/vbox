
/* $Id$ */
/** @file
 * VirtualBox Main - Guest process handling.
 */

/*
 * Copyright (C) 2012-2013 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ____H_GUESTPROCESSIMPL
#define ____H_GUESTPROCESSIMPL

#include "VirtualBoxBase.h"
#include "GuestCtrlImplPrivate.h"

class Console;
class GuestSession;

/**
 * Class for handling a guest process.
 */
class ATL_NO_VTABLE GuestProcess :
    public VirtualBoxBase,
    public GuestObject,
    VBOX_SCRIPTABLE_IMPL(IGuestProcess)
{
public:
    /** @name COM and internal init/term/mapping cruft.
     * @{ */
    VIRTUALBOXBASE_ADD_ERRORINFO_SUPPORT(GuestProcess, IGuestProcess)
    DECLARE_NOT_AGGREGATABLE(GuestProcess)
    DECLARE_PROTECT_FINAL_CONSTRUCT()
    BEGIN_COM_MAP(GuestProcess)
        VBOX_DEFAULT_INTERFACE_ENTRIES(IGuestProcess)
        COM_INTERFACE_ENTRY(IProcess)
    END_COM_MAP()
    DECLARE_EMPTY_CTOR_DTOR(GuestProcess)

    int     init(Console *aConsole, GuestSession *aSession, ULONG aProcessID, const GuestProcessStartupInfo &aProcInfo);
    void    uninit(void);
    HRESULT FinalConstruct(void);
    void    FinalRelease(void);
    /** @}  */

    /** @name IProcess interface.
     * @{ */
    STDMETHOD(COMGETTER(Arguments))(ComSafeArrayOut(BSTR, aArguments));
    STDMETHOD(COMGETTER(Environment))(ComSafeArrayOut(BSTR, aEnvironment));
    STDMETHOD(COMGETTER(ExecutablePath))(BSTR *aExecutablePath);
    STDMETHOD(COMGETTER(ExitCode))(LONG *aExitCode);
    STDMETHOD(COMGETTER(Name))(BSTR *aName);
    STDMETHOD(COMGETTER(PID))(ULONG *aPID);
    STDMETHOD(COMGETTER(Status))(ProcessStatus_T *aStatus);

    STDMETHOD(Read)(ULONG aHandle, ULONG aToRead, ULONG aTimeoutMS, ComSafeArrayOut(BYTE, aData));
    STDMETHOD(Terminate)(void);
    STDMETHOD(WaitFor)(ULONG aWaitFlags, ULONG aTimeoutMS, ProcessWaitResult_T *aReason);
    STDMETHOD(WaitForArray)(ComSafeArrayIn(ProcessWaitForFlag_T, aFlags), ULONG aTimeoutMS, ProcessWaitResult_T *aReason);
    STDMETHOD(Write)(ULONG aHandle, ULONG aFlags, ComSafeArrayIn(BYTE, aData), ULONG aTimeoutMS, ULONG *aWritten);
    STDMETHOD(WriteArray)(ULONG aHandle, ComSafeArrayIn(ProcessInputFlag_T, aFlags), ComSafeArrayIn(BYTE, aData), ULONG aTimeoutMS, ULONG *aWritten);
    /** @}  */

public:
    /** @name Public internal methods.
     * @{ */
    int callbackDispatcher(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, PVBOXGUESTCTRLHOSTCALLBACK pSvcCb);
    inline int checkPID(uint32_t uPID);
    static Utf8Str guestErrorToString(int guestRc);
    bool isReady(void);
    int readData(uint32_t uHandle, uint32_t uSize, uint32_t uTimeoutMS, void *pvData, size_t cbData, size_t *pcbRead, int *pGuestRc);
    static HRESULT setErrorExternal(VirtualBoxBase *pInterface, int guestRc);
    int startProcess(int *pGuestRc);
    int startProcessAsync(void);
    int terminateProcess(int *pGuestRc);
    int waitFor(uint32_t fWaitFlags, ULONG uTimeoutMS, ProcessWaitResult_T &waitResult, int *pGuestRc);
    int writeData(uint32_t uHandle, uint32_t uFlags, void *pvData, size_t cbData, uint32_t uTimeoutMS, uint32_t *puWritten, int *pGuestRc);
    /** @}  */

protected:
    /** @name Protected internal methods.
     * @{ */
    inline bool isAlive(void);
    int onGuestDisconnected(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, GuestCtrlCallback *pCallback, PVBOXGUESTCTRLHOSTCALLBACK pSvcCbData);
    int onProcessInputStatus(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, GuestCtrlCallback *pCallback, PVBOXGUESTCTRLHOSTCALLBACK pSvcCbData);
    int onProcessNotifyIO(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, GuestCtrlCallback * pCallback, PVBOXGUESTCTRLHOSTCALLBACK pSvcCbData);
    int onProcessStatusChange(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, GuestCtrlCallback *pCallback, PVBOXGUESTCTRLHOSTCALLBACK pSvcCbData);
    int onProcessOutput(PVBOXGUESTCTRLHOSTCBCTX pCbCtx, GuestCtrlCallback *pCallback, PVBOXGUESTCTRLHOSTCALLBACK pSvcCbData);
    int prepareExecuteEnv(const char *pszEnv, void **ppvList, ULONG *pcbList, ULONG *pcEnvVars);
    int setProcessStatus(ProcessStatus_T procStatus, int procRc);
    int signalWaiters(ProcessWaitResult_T enmWaitResult, int rc = VINF_SUCCESS);
    static DECLCALLBACK(int) startProcessThread(RTTHREAD Thread, void *pvUser);
    /** @}  */

private:

    struct Data
    {
        /** The process startup information. */
        GuestProcessStartupInfo  mProcess;
        /** Exit code if process has been terminated. */
        LONG                     mExitCode;
        /** PID reported from the guest. */
        ULONG                    mPID;
        /** The current process status. */
        ProcessStatus_T          mStatus;
        int                      mRC;
        /** The mutex for protecting the waiter(s). */
        RTSEMMUTEX               mWaitMutex;
        /** How many waiters? At the moment there can only
         *  be one. */
        uint32_t                 mWaitCount;
        /** The actual process event for doing the waits.
         *  At the moment we only support one wait a time. */
        GuestProcessWaitEvent   *mWaitEvent;
    } mData;
};

/**
 * Guest process tool flags.
 */
/** No flags specified. */
#define GUESTPROCESSTOOL_FLAG_NONE            0
/** Run until next stream block from stdout has been
 *  read in completely, then return.
 */
#define GUESTPROCESSTOOL_FLAG_STDOUT_BLOCK    RT_BIT(0)

/**
 * Internal class for handling a VBoxService tool ("vbox_ls", vbox_stat", ...).
 */
class GuestProcessTool
{
public:

    GuestProcessTool(void);

    virtual ~GuestProcessTool(void);

public:

    int Init(GuestSession *pGuestSession, const GuestProcessStartupInfo &startupInfo, bool fAsync, int *pGuestRc);

    GuestProcessStream &GetStdOut(void) { return mStdOut; }

    GuestProcessStream &GetStdErr(void) { return mStdErr; }

    int Wait(uint32_t fFlags, int *pGuestRc);

    int WaitEx(uint32_t fFlags, GuestProcessStreamBlock *pStreamBlock, int *pGuestRc);

    int GetCurrentBlock(uint32_t uHandle, GuestProcessStreamBlock &strmBlock);

    bool IsRunning(void);

    int TerminatedOk(LONG *pExitCode);

    void Terminate(void);

protected:

    GuestSession               *pSession;
    ComObjPtr<GuestProcess>     pProcess;
    GuestProcessStartupInfo     mStartupInfo;
    GuestProcessStream          mStdOut;
    GuestProcessStream          mStdErr;
};

#endif /* !____H_GUESTPROCESSIMPL */

