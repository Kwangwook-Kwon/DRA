/* BEGIN_COPYRIGHT                                                        */
/*                                                                        */
/* Open Diameter: Open-source software for the Diameter and               */
/*                Diameter related protocols                              */
/*                                                                        */
/* Copyright (C) 2002-2004 Open Diameter Project                          */
/*                                                                        */
/* This library is free software; you can redistribute it and/or modify   */
/* it under the terms of the GNU Lesser General Public License as         */
/* published by the Free Software Foundation; either version 2.1 of the   */
/* License, or (at your option) any later version.                        */
/*                                                                        */
/* This library is distributed in the hope that it will be useful,        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      */
/* Lesser General Public License for more details.                        */
/*                                                                        */
/* You should have received a copy of the GNU Lesser General Public       */
/* License along with this library; if not, write to the Free Software    */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307    */
/* USA.                                                                   */
/*                                                                        */
/* In addition, when you copy and redistribute some or the entire part of */
/* the source code of this software with or without modification, you     */
/* MUST include this copyright notice in each copy.                       */
/*                                                                        */
/* If you make any changes that are appeared to be useful, please send    */
/* sources that include the changed part to                               */
/* diameter-developers@lists.sourceforge.net so that we can reflect your  */
/* changes to one unified version of this software.                       */
/*                                                                        */
/* END_COPYRIGHT                                                          */
// PANAPacApplicationDlg.h : header file
//

#pragma once

#include "pana_config_manager.h"

class CIspSelectionDlg : public CDialog
{
public:
	CIspSelectionDlg(PANA_CfgProviderList &list, 
                     CWnd* pParent = NULL);
    ~CIspSelectionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ISPSELECT };

    PANA_CfgProviderInfo *Choice()
    {
        return m_Choice;
    }

protected:
    PANA_CfgProviderList &m_Selection;
    PANA_CfgProviderInfo *m_Choice;
    CImageList *m_pImageList;
    CListCtrl *m_List;
    CEdit *m_Username;
    CEdit *m_Password;
    CEdit *m_VlanName;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PopulateControls(PANA_CfgProviderInfo *info);

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemActivateListIsp(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnGetInfoTipListIsp(NMHDR *pNMHDR, LRESULT *pResult);
};
