// ServerName.cpp : implementation file
//

#include "stdafx.h"
#include "DamFix.h"
#include "ServerName.h"
#include ".\servername.h"


// CServerName dialog

IMPLEMENT_DYNAMIC(CServerName, CDialog)
CServerName::CServerName(CString pCurr, CWnd* pParent /*=NULL*/)
	: CDialog(CServerName::IDD, pParent)
	, m_currServer(pCurr)
	, m_newServer(_T(""))
{
}

CServerName::~CServerName()
{
}

void CServerName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEWSERVER, m_newServer);
	DDX_Text(pDX, IDC_CURSERVER, m_currServer);
}


BEGIN_MESSAGE_MAP(CServerName, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CServerName message handlers

void CServerName::OnBnClickedOk()
{
	UpdateData(true);

	OnOK();
}

BOOL CServerName::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
