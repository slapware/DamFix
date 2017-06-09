// DamFixDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>
#include "MarkupSTL.h"

using namespace std;
// CDamFixDlg dialog
class CDamFixDlg : public CDialog
{
// Construction
public:
	CDamFixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DAMFIX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_tnslist;
private:
	string m_db_long_name;
	string m_db_file_location;
	string m_queryRun;
	string m_xmlError;
    string m_newServer;
	int QuoteCheck(string& pRecord);
public:
	CString m_server;
	CString m_old_FL_label;
	CString m_new_FL_label;
	CString m_old_FL_fb;
	CString m_new_FL_fb;
	CString m_old_LN_label;
	CString m_new_LN_label;
	CString m_old_LN_fb;
	CString m_new_LN_fb;
	CString m_folder;
private:
	CString theTime(void);
	bool Validate(void);
	CString m_tns;
	bool m_changeServer;
	bool m_changeFLLabel;
	bool m_changeFLBroker;
	bool m_changeLNLabel;
	bool m_changeLNBroker;
	long m_convOK;
    bool m_LN_Update;
    bool m_FL_Update;
    bool m_updateDB;
	long m_convFAIL;
public:
	afx_msg void OnBnClickedSvrChange();
};
