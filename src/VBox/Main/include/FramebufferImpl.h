/** @file
 *
 * VirtualBox COM class implementation
 */

/*
 * Copyright (C) 2006-2007 innotek GmbH
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation,
 * in version 2 as it comes in the "COPYING" file of the VirtualBox OSE
 * distribution. VirtualBox OSE is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * If you received this file as part of a commercial VirtualBox
 * distribution, then only the terms of your commercial VirtualBox
 * license agreement apply instead of the previous paragraph.
 */

#ifndef ____H_FRAMEBUFFERIMPL
#define ____H_FRAMEBUFFERIMPL

#include "VirtualBoxBase.h"


class ATL_NO_VTABLE InternalFramebuffer :
    public VirtualBoxBase,
    public IFramebuffer
{
public:
    InternalFramebuffer();
    virtual ~InternalFramebuffer();

    DECLARE_NOT_AGGREGATABLE(InternalFramebuffer)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(InternalFramebuffer)
        COM_INTERFACE_ENTRY(IFramebuffer)
    END_COM_MAP()

    NS_DECL_ISUPPORTS

    // public methods only for internal purposes
    HRESULT init (ULONG width, ULONG height, ULONG depth);

    // IFramebuffer properties
    STDMETHOD(COMGETTER(Address)) (BYTE **address);
    STDMETHOD(COMGETTER(Width)) (ULONG *width);
    STDMETHOD(COMGETTER(Height)) (ULONG *height);
    STDMETHOD(COMGETTER(ColorDepth)) (ULONG *colorDepth);
    STDMETHOD(COMGETTER(LineSize)) (ULONG *lineSize);
    STDMETHOD(COMGETTER(PixelFormat)) (FramebufferPixelFormat_T *pixelFormat);
    STDMETHOD(COMGETTER(HeightReduction)) (ULONG *heightReduction);
    STDMETHOD(COMGETTER(Overlay)) (IFramebufferOverlay **aOverlay);
    STDMETHOD(COMGETTER(RenderMode)) (FramebufferRenderMode_T *renderMode);
    STDMETHOD(COMSETTER(RenderMode)) (FramebufferRenderMode_T  renderMode);

    // IFramebuffer methods
    STDMETHOD(Lock)();
    STDMETHOD(Unlock)();
    STDMETHOD(NotifyUpdate)(ULONG x, ULONG y,
                            ULONG w, ULONG h,
                            BOOL *finished);
    STDMETHOD(RequestResize)(ULONG uScreenId, FramebufferPixelFormat_T pixelFormat, BYTE *vram,
                             ULONG lineSize, ULONG w, ULONG h,
                             BOOL *finished);
    STDMETHOD(OperationSupported)(FramebufferAccelerationOperation_T operation,
                                  BOOL *supported);
    STDMETHOD(VideoModeSupported)(ULONG width, ULONG height, ULONG bpp, BOOL *supported);
    STDMETHOD(SolidFill)(ULONG x, ULONG y, ULONG width, ULONG height,
                         ULONG color, BOOL *handled);
    STDMETHOD(CopyScreenBits)(ULONG xDst, ULONG yDst, ULONG xSrc, ULONG ySrc,
                              ULONG width, ULONG height, BOOL *handled);

    STDMETHOD(GetVisibleRegion)(ULONG * aPcRect, BYTE * aPRect);
    STDMETHOD(SetVisibleRegion)(ULONG aCRect, BYTE * aPRect);

private:
    int mWidth;
    int mHeight;
    int mDepth;
    int mLineSize;
    uint8_t *mData;
    RTSEMMUTEX mMutex;

    /* Framebuffer render mode */
    FramebufferRenderMode_T mRenderMode;
};


#endif // ____H_FRAMEBUFFERIMPL
