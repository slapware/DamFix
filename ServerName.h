#pragma once


// CServerName dialog

class CServerName : public CDialog
{
	DECLARE_DYNAMIC(CServerName)

public:
	CServerName(CString pCurr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerName();

// Dialog Data
	enum { IDD = IDD_SERVERNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_currServer;
	CString m_newServer;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
