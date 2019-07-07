/* $Id$ */
/** @file
 * VBox Qt GUI - UIWizardNewCloudVM class declaration.
 */

/*
 * Copyright (C) 2009-2019 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef FEQT_INCLUDED_SRC_wizards_newcloudvm_UIWizardNewCloudVM_h
#define FEQT_INCLUDED_SRC_wizards_newcloudvm_UIWizardNewCloudVM_h
#ifndef RT_WITHOUT_PRAGMA_ONCE
# pragma once
#endif

/* GUI includes: */
#include "UIWizard.h"

/* COM includes: */
#include "COMEnums.h"
#include "CCloudClient.h"
#include "CVirtualSystemDescription.h"
#include "CVirtualSystemDescriptionForm.h"

/** New Cloud VM wizard. */
class UIWizardNewCloudVM : public UIWizard
{
    Q_OBJECT;

public:

    /** Basic page IDs. */
    enum
    {
        Page1,
        Page2
    };

    /** Expert page IDs. */
    enum
    {
        PageExpert
    };

    /** Constructs new cloud vm wizard passing @a pParent to the base-class. */
    UIWizardNewCloudVM(QWidget *pParent);

    /** Prepares all. */
    virtual void prepare() /* override */;

    /** Defines Cloud @a comClient object. */
    void setClient(const CCloudClient &comClient) { m_comClient = comClient; }
    /** Returns Cloud Client object. */
    CCloudClient client() const { return m_comClient; }

    /** Defines Virtual System @a comDescription object. */
    void setVSD(const CVirtualSystemDescription &comDescription) { m_comVSD = comDescription; }
    /** Returns Virtual System Description object. */
    CVirtualSystemDescription vsd() const { return m_comVSD; }

    /** Defines Virtual System Description @a comForm object. */
    void setVSDForm(const CVirtualSystemDescriptionForm &comForm) { m_comVSDForm = comForm; }
    /** Returns Virtual System Description Form object. */
    CVirtualSystemDescriptionForm vsdForm() const { return m_comVSDForm; }

    /** Creates VSD Form. */
    bool createVSDForm();

    /** Creates New Cloud VM. */
    bool createCloudVM();

protected:

    /** Handles translation event. */
    virtual void retranslateUi() /* override */;

private:

    /** Holds the Cloud Client object reference. */
    CCloudClient                   m_comClient;
    /** Holds the Virtual System Description object reference. */
    CVirtualSystemDescription      m_comVSD;
    /** Holds the Virtual System Description Form object reference. */
    CVirtualSystemDescriptionForm  m_comVSDForm;
};

/** Safe pointer to new cloud vm wizard. */
typedef QPointer<UIWizardNewCloudVM> UISafePointerWizardNewCloudVM;

#endif /* !FEQT_INCLUDED_SRC_wizards_newcloudvm_UIWizardNewCloudVM_h */
