// DamFixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DamFix.h"
#include "DamFixDlg.h"
#include ".\damfixdlg.h"
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include "ocidamfix.h"
#include "OraKey.h"
#include "ServerName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	::SetWindowLong(this->GetSafeHwnd( ),GWL_EXSTYLE,::GetWindowLong(this->GetSafeHwnd( ),GWL_EXSTYLE)^WS_EX_LAYERED);
	::SetLayeredWindowAttributes(this->GetSafeHwnd( ),RGB(0,0,0), 190, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDamFixDlg dialog



CDamFixDlg::CDamFixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDamFixDlg::IDD, pParent)
	, m_server(_T(""))
	, m_old_FL_label(_T(""))
	, m_new_FL_label(_T(""))
	, m_old_FL_fb(_T(""))
	, m_new_FL_fb(_T(""))
	, m_old_LN_label(_T(""))
	, m_new_LN_label(_T(""))
	, m_old_LN_fb(_T(""))
	, m_new_LN_fb(_T(""))
	, m_folder(_T(""))
	, m_changeFLLabel(true)
	, m_changeFLBroker(true)
	, m_changeLNLabel(true)
	, m_changeLNBroker(true)
    , m_FL_Update(true)
    , m_LN_Update(true)
	, m_changeServer(false)
    , m_updateDB(false)
	, m_convOK(0)
	, m_convFAIL(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDamFixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_tnslist);
	DDX_Text(pDX, IDC_SERVER, m_server);
	DDX_Text(pDX, IDC_OLD_FL_LABEL, m_old_FL_label);
	DDX_Text(pDX, IDC_NEW_FL_LABEL, m_new_FL_label);
	DDX_Text(pDX, IDC_OLD_FL_FB, m_old_FL_fb);
	DDX_Text(pDX, IDC_NEW_FL_FB, m_new_FL_fb);
	DDX_Text(pDX, IDC_OLD_LN_LABEL, m_old_LN_label);
	DDX_Text(pDX, IDC_NEW_LN_LABEL, m_new_LN_label);
	DDX_Text(pDX, IDC_OLD_LN_FB, m_old_LN_fb);
	DDX_Text(pDX, IDC_NEW_LN_FB, m_new_LN_fb);
	DDX_Text(pDX, IDC_FOLDER, m_folder);
}

BEGIN_MESSAGE_MAP(CDamFixDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SVRCHANGE, OnBnClickedSvrChange)
END_MESSAGE_MAP()


// CDamFixDlg message handlers

BOOL CDamFixDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	/////////////////////////////////////////////////////
	// Smart Pointer memory protection for heap object //
	/////////////////////////////////////////////////////
	boost::scoped_ptr<OraKey> pReg(new OraKey() );
	LONG lReg = pReg->Open();
	CString m_file;
	if (lReg == ERROR_SUCCESS)
	{
	    pReg->Read (m_file);
	}
	pReg->Parse_file(m_file);       // Read the location of Oracle installation,
	pReg->fill_list(m_tnslist);     // open and read settings from tnsnames.ora
	pReg->Close();                  // Present in drop down list for selection.
	
	::SetWindowLong(this->GetSafeHwnd( ),GWL_EXSTYLE,::GetWindowLong(this->GetSafeHwnd( ),GWL_EXSTYLE)^WS_EX_LAYERED);
	::SetLayeredWindowAttributes(this->GetSafeHwnd( ),RGB(0,0,0), 215, LWA_ALPHA);

	char szPath[MAX_PATH];
    ::GetCurrentDirectory (sizeof (szPath), szPath); 
	std::string pXmlSetting = szPath;
	pXmlSetting += "\\current.xml";
	CMarkupSTL xml;
	if(!xml.Load(pXmlSetting.c_str() ) )
	{
		return TRUE;  // return TRUE  unless you set the focus to a control
	}
	else
	{
		xml.FindElem();
		xml.IntoElem();

		xml.FindElem("Server");
		m_server = xml.GetData().c_str();
		xml.FindElem("Folder");
		m_folder = xml.GetData().c_str();

		xml.FindElem("Old_FL_Label");
		m_old_FL_label = xml.GetData().c_str();
		xml.FindElem("New_FL_Label");
		m_new_FL_label = xml.GetData().c_str();
		xml.FindElem("Old_FL_FB");
		m_old_FL_fb = xml.GetData().c_str();
		xml.FindElem("New_FL_FB");
		m_new_FL_fb = xml.GetData().c_str();

		xml.FindElem("Old_LN_Label");
		m_old_LN_label = xml.GetData().c_str();
		xml.FindElem("New_LN_Label");
		m_new_LN_label = xml.GetData().c_str();
		xml.FindElem("Old_LN_FB");
		m_old_LN_fb = xml.GetData().c_str();
		xml.FindElem("New_LN_FB");
		m_new_LN_fb = xml.GetData().c_str();
		UpdateData(false);	// vars to screen
	}
    //////////////////////////////////
    // Set up Error XML data file.  //
    //////////////////////////////////
    CMarkupSTL errxml( "<?xml version=\"1.0\"?>\r\n" );
    errxml.AddElem("RUN_REPORT");
    errxml.IntoElem();
    errxml.AddElem("RUN_DATE");
    errxml.AddChildElem("NOW", theTime().operator LPCTSTR() );
    m_xmlError = errxml.GetDoc();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDamFixDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDamFixDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDamFixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDamFixDlg::OnBnClickedOk()
{
	UpdateData(true);
    /////////////////////////////////////////////////////////
    // Determine if DB Updates or Report only mode usage.  //
    /////////////////////////////////////////////////////////
    string pMsgTemp;
    pMsgTemp = "Operation mode choice, make decision.\n";
    pMsgTemp += "Select YES for Database Updates performed.\n";
    pMsgTemp += "Select NO for Report only, no Updates performed.\n";
    pMsgTemp += "Select CANCEL to abort and cancel operation.";
    int rys = AfxMessageBox(pMsgTemp.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);
    switch (rys) 
    {
        case IDYES:
            m_updateDB = true;
            break;
        
        case IDNO:
            m_updateDB = false;
            break;
        
        case IDCANCEL:
            return;
            break;
    
        default:
            return;
    }

    rys = AfxMessageBox( "Are you REALLY REALLY sure ALL entries are correct ?",  MB_YESNOCANCEL | MB_ICONQUESTION);
	if ( (rys == IDNO) || (rys == IDCANCEL))
	{
	 return;
	}

	if(m_folder.IsEmpty())
	{
	    rys = AfxMessageBox("Root Folder EMPTY, Proceed anyway ?", MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (rys == IDNO) || (rys == IDCANCEL))
		{
		return;
		}
	}

	int nIndex = m_tnslist.GetCurSel();
	if( nIndex == CB_ERR)
	{
	    AfxMessageBox("No Oracle connection Name provided!", MB_ICONSTOP);
		return;
	}
    m_tnslist.GetLBText( nIndex, m_tns);
    /////////////////////////////////////////////////
    // Validate we have information to work with.  //
    /////////////////////////////////////////////////
	if(!Validate())
	{
	    AfxMessageBox("Not enough information supplied.", MB_ICONSTOP);
		return;
	}
    /////////////////////////////////////////////
    // Nothing to Update, someone screwed up.  //
    /////////////////////////////////////////////
    if( (!m_LN_Update) && (!m_FL_Update) )
    {
        AfxMessageBox("Nothing to UPDATE", MB_ICONSTOP);
		return;
    }
    //////////////////////////////////////////////////////////
    // Ensure Database Update is correct mode of operation  //
    //////////////////////////////////////////////////////////
	if(m_updateDB)
	{
	    rys = AfxMessageBox("Database Update's are ON, are you sure ?", MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (rys == IDNO) || (rys == IDCANCEL))
		{
		return;
		}
	}
    /////////////////////////////////////////////
    // Start to work on information provided.  //
    /////////////////////////////////////////////
	BeginWaitCursor();
    /////////////////////////////////////////////////////////////////////////
    //  Get the subset of records that meet search requirements to modify  //
    /////////////////////////////////////////////////////////////////////////
	std::string str, supsz, szerror;
	std::ostringstream strstrm;
    //////////////////////////////
    // Special seperator chars  //
    //////////////////////////////
	char p3tx = 0x03;   // 
	char p4tx = 0x04;   // 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Example FILE_LOCATION database entry                                                                              //
// HCUSNY27F[NTFS]D21C:DAMChildrens2005SpringLittleHouseBox_0060754281SnapshotsLittle House Collection.eps  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_folder.IsEmpty())
	{
		strstrm << "SELECT RECORD_ID FROM DOC_RENDITIONS WHERE FILE_LOCATION LIKE '";
		strstrm << m_server.operator LPCTSTR() << p3tx << p3tx << "F[NTFS]" << p3tx;
		strstrm << m_old_FL_label.operator LPCTSTR() << p3tx << "C:" << p4tx ;
		strstrm << m_old_LN_fb.operator LPCTSTR() << p4tx << "%'";
	}
	else
	{
		strstrm << "SELECT RECORD_ID FROM DOC_RENDITIONS WHERE FILE_LOCATION LIKE '";
		strstrm << m_server.operator LPCTSTR() << p3tx << p3tx << "F[NTFS]" << p3tx;
		strstrm << m_old_FL_label.operator LPCTSTR() << p3tx << "C:" << p4tx << m_old_LN_fb.operator LPCTSTR();
		strstrm << p4tx << m_folder.operator LPCTSTR() << "%'";
	}

	supsz = strstrm.str();
	strstrm.str("");
#ifdef _DEBUG
	OutputDebugString("Query SQL = ");
	OutputDebugString(supsz.c_str() );
	OutputDebugString("\n");
#endif
	string m_ouser, m_opass, m_stdtns;
	std::string m_thr_error;
	m_ouser = "prodtele";
	m_opass = "prodtele";
	m_stdtns = m_tns.operator LPCTSTR();
    ////////////////////////////////////////////////
    // Smart pointer heap protection local scope  //
    ////////////////////////////////////////////////
	boost::scoped_ptr<COciDamFix> pOci(new COciDamFix(m_ouser, m_opass, m_stdtns) );
    ////////////////////////////////////////////////////////
    // Check Object internal flag for connection success  //
    ////////////////////////////////////////////////////////
	if (!pOci->IsOK())
	{
        m_thr_error = pOci->get_error();
		AfxMessageBox(m_thr_error.c_str(), MB_ICONSTOP);
	    return;
	}
       //////////////////////////////////////////////////
       // Create the vector of record_id's to process  //
       //////////////////////////////////////////////////
	   m_queryRun = supsz;
	   szerror = pOci->make_docVec(supsz);
	   ///////////////////////////////////
	   //  Check for error in statement //
	   ///////////////////////////////////
	   if (!szerror.empty())
	   {
        m_thr_error = pOci->get_error();
		AfxMessageBox(m_thr_error.c_str(), MB_ICONSTOP);
	    return;
	   }
#ifdef _DEBUG
        size_t vsz = pOci->docVec.size();                   // Get the size of the       
        strstrm << "pOci Record_id Vector size = ";         // vector of record_id's
        strstrm << static_cast<unsigned int>(vsz) << endl;  // to ensure all files
        supsz = strstrm.str();                              // are accounted for
        strstrm.str("");                                    // that meet search
        OutputDebugString(supsz.c_str() );                  // condition provided.
#endif
	long id2chk = 0; // record_id from doc_renditions
	string m_lname;	 // long_name from doc_renditions
	string m_floc;	 // file_location from doc_renditions
	static const basic_string <char>::size_type npos = -1;
	basic_string <char>::size_type indexfbname, indexlbname;
        //////////////////////////////////
        // iterator for record_id list  //
        //////////////////////////////////
	    doclist::iterator itr = pOci->docVec.begin();
	    while (itr != pOci->docVec.end() )
	    {
	     id2chk = (*itr);               // Get next record_id to process
		 if(id2chk == 0)
		 {
			 itr++;
			 continue;
		 }
         ///////////////////////////////////////////////////
         // Build query for record_id in id2chk variable  //
         ///////////////////////////////////////////////////
	     strstrm << "SELECT long_name, file_location FROM doc_renditions WHERE RECORD_ID = ";
	     strstrm << id2chk;
	     supsz = strstrm.str();         // copy stream to string
	     strstrm.str("");               // clear stream
         /////////////////////////////////////////////////////////
         // Perform the database fetch for record_id in id2chk  //
         /////////////////////////////////////////////////////////
	     pOci->GetOCI2(supsz, m_lname, m_floc);
		 m_db_long_name = m_lname;	    // set LONG_NAME session variable
		 m_db_file_location = m_floc;	// set FILE_LOCATION session variable
        ///////////////////////////////////////////////////////////
        // Find the old disk label in DOC_RENDITIONS->LONG_NAME  //
        ///////////////////////////////////////////////////////////
		if(m_changeLNLabel)
		{
			indexlbname = m_lname.find ( m_old_LN_label.operator LPCTSTR() );
			if(indexlbname == npos)
			{
                m_convFAIL++;
				OutputDebugString("find LongName Label failed.\n");
                CMarkupSTL errxml;
                errxml.SetDoc(m_xmlError.c_str());
				errxml.FindElem("RUN_REPORT");
                errxml.IntoElem();
                string etmp;
                errxml.AddElem("ERROR");
                errxml.AddChildElem("TYPE", "FIND LONG_NAME Label failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                errxml.AddChildElem("Record_ID", etmp.c_str() );
				if(!m_lname.empty())
					errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
				else
					errxml.AddChildElem("LONG_NAME", "Empty string" );
				if(!m_floc.empty() )
					errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
				else
					errxml.AddChildElem("FILE_LOCATION", "Empty string" );
                m_xmlError = errxml.GetDoc();
            }
			else
				m_lname.replace(indexlbname, m_old_LN_label.GetLength(), m_new_LN_label.operator LPCTSTR());
		}
        ///////////////////////////////////////////////////////////////
        // Find the old disk label in DOC_RENDITIONS->FILE_LOCATION  //
        ///////////////////////////////////////////////////////////////
		if(m_changeFLLabel)
		{
			indexlbname = m_floc.find ( m_old_FL_label.operator LPCTSTR() );
			if(indexlbname == npos)
			{
				OutputDebugString("find old File Location LABEL failed.\n");
                m_convFAIL++;
                CMarkupSTL errxml;
                errxml.SetDoc(m_xmlError.c_str());
				errxml.FindElem("RUN_REPORT");
                string etmp;
                errxml.IntoElem();
                errxml.AddElem("ERROR");
                errxml.AddChildElem("TYPE", "FIND FILE_LOCATION Label failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                errxml.AddChildElem("Record_ID", etmp.c_str() );
				if(!m_lname.empty())
					errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
				else
					errxml.AddChildElem("LONG_NAME", "Empty string" );
				if(!m_floc.empty() )
					errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
				else
					errxml.AddChildElem("FILE_LOCATION", "Empty string" );
                m_xmlError = errxml.GetDoc();
			}
			else
				m_floc.replace(indexlbname, m_old_FL_label.GetLength(), m_new_FL_label.operator LPCTSTR());
		}
        /////////////////////////////////////////////////////////////////
        // Find the File Broker name in DOC_RENDITIONS->FILE_LOCATION  //
        /////////////////////////////////////////////////////////////////
		if(m_changeFLBroker)
		{
			indexfbname = m_floc.find ( m_old_FL_fb.operator LPCTSTR() );
			if(indexfbname == npos)
			{
                m_convFAIL++;
				OutputDebugString("find File Broker in File Location failed.\n");
				bool xres;
                CMarkupSTL errxml;
                xres = errxml.SetDoc(m_xmlError.c_str());
				xres = errxml.FindElem("RUN_REPORT");
                xres = errxml.IntoElem();
                string etmp;
                xres = errxml.AddElem("ERROR");
                xres = errxml.AddChildElem("TYPE", "FIND FILE_LOCATION File Broker failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                xres = errxml.AddChildElem("Record_ID", etmp.c_str() );
                xres = errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
                xres = errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
                m_xmlError = errxml.GetDoc();
			}
			else
				m_floc.replace(indexfbname, m_old_FL_fb.GetLength(), m_new_FL_fb.operator LPCTSTR());
		}
        /////////////////////////////////////////////////////////////
        // Find the File Broker name in DOC_RENDITIONS->LONG_NAME  //
        /////////////////////////////////////////////////////////////
		if(m_changeLNBroker)
		{
			indexfbname = m_lname.find ( m_old_LN_fb.operator LPCTSTR() );
			if(indexfbname == npos)
			{
				OutputDebugString("find Long OLD Name File Broker name failed.\n");
                m_convFAIL++;
                CMarkupSTL errxml;
                errxml.SetDoc(m_xmlError.c_str());
				errxml.FindElem("RUN_REPORT");
                errxml.IntoElem();
                string etmp;
                errxml.AddElem("ERROR");
                errxml.AddChildElem("TYPE", "FIND LONG_NAME File Broker failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                errxml.AddChildElem("Record_ID", etmp.c_str() );

				if(!m_lname.empty())
					errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
				else
					errxml.AddChildElem("LONG_NAME", "Empty string" );
				if(!m_floc.empty() )
					errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
				else
					errxml.AddChildElem("FILE_LOCATION", "Empty string" );

                m_xmlError = errxml.GetDoc();
			}
			else
				m_lname.replace(indexfbname, m_old_LN_fb.GetLength(), m_new_LN_fb.operator LPCTSTR());
		}
        ////////////////////////////////////////////////////////
        // Find the Server Name in DOC_RENDITIONS->LONG_NAME  //
        ////////////////////////////////////////////////////////
        if (m_changeServer) 
        {
			indexfbname = m_lname.find ( m_server.operator LPCTSTR() );
			if(indexfbname == npos)
			{
                m_convFAIL++;
				OutputDebugString("find LONG_Name Server name failed.\n");
                CMarkupSTL errxml;
                errxml.SetDoc(m_xmlError.c_str());
				errxml.FindElem("RUN_REPORT");
                errxml.IntoElem();
                string etmp;
                errxml.AddElem("ERROR");
                errxml.AddChildElem("TYPE", "FIND LONG_NAME Server failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                errxml.AddChildElem("Record_ID", etmp.c_str() );
				if(!m_lname.empty())
					errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
				else
					errxml.AddChildElem("LONG_NAME", "Empty string" );
				if(!m_floc.empty() )
					errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
				else
					errxml.AddChildElem("FILE_LOCATION", "Empty string" );
                m_xmlError = errxml.GetDoc();
			}
			else
				m_lname.replace(indexfbname, m_server.GetLength(), m_newServer.c_str());
        }
        ////////////////////////////////////////////////////////////
        // Find the Server Name in DOC_RENDITIONS->FILE_LOCATION  //
        ////////////////////////////////////////////////////////////
        if (m_changeServer) 
        {
			indexlbname = m_floc.find ( m_server.operator LPCTSTR() );
			if(indexlbname == npos)
			{
                m_convFAIL++;
				OutputDebugString("find FILE_LOCATION Server name failed.\n");
                CMarkupSTL errxml;
                errxml.SetDoc(m_xmlError.c_str());
				errxml.FindElem("RUN_REPORT");
                errxml.IntoElem();
                string etmp;
                errxml.AddElem("ERROR");
                errxml.AddChildElem("TYPE", "FIND FILE_LOCATION Server failed" );
                strstrm << id2chk;
                etmp = strstrm.str();    // copy stream to string
                strstrm.str("");         // clear stream
                errxml.AddChildElem("Record_ID", etmp.c_str() );
				if(!m_lname.empty())
					errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
				else
					errxml.AddChildElem("LONG_NAME", "Empty string" );
				if(!m_floc.empty() )
					errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
				else
					errxml.AddChildElem("FILE_LOCATION", "Empty string" );
                m_xmlError = errxml.GetDoc();
			}
			else
				m_floc.replace(indexlbname, m_server.GetLength(), m_newServer.c_str());
        }
        ///////////////////////////////////////////////
        // Quote Fix for SQL statement if required.  //
        ///////////////////////////////////////////////
		QuoteCheck(m_lname);
		QuoteCheck(m_floc);
        ////////////////////////////////
        // Update FILE_LOCATION only  //
        ////////////////////////////////
        if( (!m_LN_Update) && (m_FL_Update) )
		{
			strstrm << "UPDATE DOC_RENDITIONS SET FILE_LOCATION = '" << m_floc;
			strstrm << "' WHERE RECORD_ID = " << id2chk;
			supsz = strstrm.str();      // copy stream to string
			strstrm.str("");            // clear stream 
		}
        ////////////////////////////
        // Update LONG_NAME only  //
        ////////////////////////////
        if( (m_LN_Update) && (!m_FL_Update) )
		{
			strstrm << "UPDATE DOC_RENDITIONS SET LONG_NAME = '" << m_lname;
			strstrm << "' WHERE RECORD_ID = " << id2chk;
			supsz = strstrm.str();      // copy stream to string
			strstrm.str("");            // clear stream 
		}
        /////////////////////////////////////////
        // Update LONG_NAME and FILE_LOCATION  //
        /////////////////////////////////////////
        if( (m_LN_Update) && (m_FL_Update) )
		{
			strstrm << "UPDATE DOC_RENDITIONS SET LONG_NAME = '" << m_lname;
			strstrm << "', FILE_LOCATION = '" <<  m_floc << "' WHERE RECORD_ID = " << id2chk;
			supsz = strstrm.str();      // copy stream to string
			strstrm.str("");            // clear stream
		}
        ////////////////////////////////////////////////
        // Perform Update with SQL built from logic.  //
        ////////////////////////////////////////////////
        if (m_updateDB) 
        {
            szerror = pOci->UpOCI(supsz);
        }
        //////////////////////////////////
        // Check for errors in update.  //
        //////////////////////////////////
		if (!szerror.empty())
		{
			strstrm << "Error in UPDATE DOC_RENDITIONS -> " << endl;
			strstrm << szerror.c_str() << endl;
			strstrm << "Record_ID = " << id2chk << endl;
			strstrm << "LN  = " << m_lname << endl;
			strstrm << "FL  = " << m_floc << endl;
			szerror = strstrm.str();    // copy stream to string
			strstrm.str("");            // clear stream
			// m_xmlError entry here
			CMarkupSTL errxml;
			errxml.SetDoc(m_xmlError.c_str());
			errxml.FindElem("RUN_REPORT");
            errxml.IntoElem();
			string etmp;
			errxml.AddElem("ERROR");
			errxml.AddChildElem("TYPE", szerror.c_str() );
			strstrm << id2chk;
			etmp = strstrm.str();    // copy stream to string
			strstrm.str("");         // clear stream
			errxml.AddChildElem("Record_ID", etmp.c_str() );
			errxml.AddChildElem("LONG_NAME", m_lname.c_str() );
			errxml.AddChildElem("FILE_LOCATION", m_floc.c_str() );
			m_xmlError = errxml.GetDoc();
			// Display error
			AfxMessageBox(szerror.c_str(), MB_ICONSTOP);
			OutputDebugString(szerror.c_str());
			m_convFAIL++;
		}
		else
			m_convOK++;

	     itr++;
	    }	// while (itr != nvec.end() ) ------------ LOOP ------------ LOOP ----------
        /////////////////////////////////////////////////////
        // Save settings to XML file in current directory  //
        /////////////////////////////////////////////////////
		char szPath[MAX_PATH];
        ::GetCurrentDirectory (sizeof (szPath), szPath); 
		std::string pXmlSetting = szPath;
		pXmlSetting += "\\current.xml";
		CMarkupSTL xml;

		xml.SetDoc( "<?xml version=\"1.0\"?>\r\n" );
		xml.AddElem("SESSION");
		xml.AddChildElem("Server", m_server.operator LPCTSTR() );
		xml.AddChildElem("Folder", m_folder.operator LPCTSTR() );
		xml.AddChildElem("Old_FL_Label", m_old_FL_label.operator LPCTSTR() );
		xml.AddChildElem("New_FL_Label", m_new_FL_label.operator LPCTSTR() );
		xml.AddChildElem("Old_FL_FB", m_old_FL_fb.operator LPCTSTR() );
		xml.AddChildElem("New_FL_FB", m_new_FL_fb.operator LPCTSTR() );

		xml.AddChildElem("Old_LN_Label", m_old_LN_label.operator LPCTSTR() );
		xml.AddChildElem("New_LN_Label", m_new_LN_label.operator LPCTSTR() );
		xml.AddChildElem("Old_LN_FB", m_old_LN_fb.operator LPCTSTR() );
		xml.AddChildElem("New_LN_FB", m_new_LN_fb.operator LPCTSTR() );
		xml.Save(pXmlSetting.c_str() );
        ///////////////////////////////////////////////////
        // Save errors to XML file in current directory  //
        ///////////////////////////////////////////////////
		std::string pXmlErrors = szPath;
		pXmlErrors += "\\errors_";
		pXmlErrors += theTime().operator LPCTSTR();
		pXmlErrors += ".xml";
		CMarkupSTL errxml;
        errxml.SetDoc(m_xmlError.c_str());
		errxml.Save(pXmlErrors.c_str() );
        ////////////////////
        // Done with XML  //
        ////////////////////
	    EndWaitCursor();
        if (m_updateDB) 
        {
            strstrm << "Converted = " << m_convOK << endl;
        } 
        else 
        {
            strstrm << "Checked OK = " << m_convOK << endl;
        }
		strstrm << "Errors = " << m_convFAIL << endl;
		strstrm << "Query Run was " << m_queryRun;
		szerror = strstrm.str();    // copy stream to string
		strstrm.str("");            // clear stream         
		AfxMessageBox(szerror.c_str(), MB_ICONINFORMATION);

        pOci.reset();
//        OnOK();
}

int CDamFixDlg::QuoteCheck(string& pRecord)
{
	static const basic_string <char>::size_type npos = -1;
	basic_string <char>::size_type indexQchar;
	
	indexQchar = 0;
	while((indexQchar = pRecord.find ( 0x27, indexQchar ) ) != npos) // Quote ' Fix
	{
	    pRecord.replace(indexQchar, 1, "''");
	    indexQchar++;       // two required to ensure exit
	    indexQchar++;
	}
	
	return static_cast<int>(indexQchar);
}

bool CDamFixDlg::Validate(void)
{
	bool retVal = true;
	int qAns;
	std::string pMsg;

	if(m_server.IsEmpty())
	{
	    AfxMessageBox("No Server Name provided!", MB_ICONSTOP);
		retVal = false;
	}

	if(m_old_FL_label.IsEmpty())	// OLD FILE LOCATION disc Label
	{
	    AfxMessageBox("No OLD File Location LABEL Name provided!", MB_ICONSTOP);
		retVal = false;
	}

	if(m_new_FL_label.IsEmpty())	// NEW FILE LOCATION disc Label
	{
	    pMsg = "No NEW File Location LABEL Name provided!\n";
	    pMsg += "Leave Disc Label name un-changed ?";
		qAns = AfxMessageBox(pMsg.c_str(),  MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (qAns == IDNO) || (qAns == IDCANCEL))
		{
			retVal = false;
			return retVal;
		}
		else
		{
			m_changeFLLabel = false;
			retVal = true;
		}
	}

	if(m_old_FL_fb.IsEmpty())		// OLD FILE LOCATION File Broker Name
	{
	    AfxMessageBox("No OLD File Location FILE BROKER Name provided!", MB_ICONSTOP);
		retVal = false;
	}

	if(m_new_FL_fb.IsEmpty())		// NEW FILE LOCATION File Broker Name
	{
	    pMsg = "No NEW File Location FILE BROKER Name provided!\n";
	    pMsg += "Leave FILE BROKER name un-changed ?";
		qAns = AfxMessageBox(pMsg.c_str(),  MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (qAns == IDNO) || (qAns == IDCANCEL))
		{
			retVal = false;
			return retVal;
		}
		else
		{
			m_changeFLBroker = false;
			retVal = true;
		}
	}

	if(m_old_LN_label.IsEmpty())	// OLD LONG NAME disc Label
	{
	    AfxMessageBox("No OLD Long Name LABEL Name provided!", MB_ICONSTOP);
		retVal = false;
	}

	if(m_new_LN_label.IsEmpty())	// NEW LONG NAME disc Label
	{
	    pMsg = "No NEW Long Name LABEL Name provided!\n";
	    pMsg += "Leave Disc Label name un-changed ?";
		qAns = AfxMessageBox(pMsg.c_str(),  MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (qAns == IDNO) || (qAns == IDCANCEL))
		{
			retVal = false;
			return retVal;
		}
		else
		{
			m_changeLNLabel = false;
			retVal = true;
		}
	}

	if(m_old_LN_fb.IsEmpty())		// OLD LONG NAME File Broker Name
	{
	    AfxMessageBox("No OLD Long Name FILE BROKER Name provided!", MB_ICONSTOP);
		retVal = false;
	}

	if(m_new_LN_fb.IsEmpty())		// NEW LONG NAME File Broker Name
	{
	    pMsg = "No NEW Long Name FILE BROKER Name provided!\n";
	    pMsg += "Leave FILE BROKER name un-changed ?";
		qAns = AfxMessageBox(pMsg.c_str(),  MB_YESNOCANCEL | MB_ICONQUESTION);
		if ( (qAns == IDNO) || (qAns == IDCANCEL))
		{
			retVal = false;
			return retVal;
		}
		else
		{
			m_changeLNBroker = false;
			retVal = true;
		}
	}
    /////////////////////////////////////////////////////////
    // Determine what has changed to build SQL statement.  //
    /////////////////////////////////////////////////////////
    if (!m_changeServer) 
    {
        if ( (m_changeLNLabel) || (m_changeLNBroker) )
            m_LN_Update = true;
        else
            m_LN_Update = false;

        if ( (m_changeFLBroker) || (m_changeFLLabel) )
            m_FL_Update = true;
        else
            m_FL_Update = false;
    }
    else
    {
        m_FL_Update = true;
        m_LN_Update = true;
    }

	return retVal;
}

CString CDamFixDlg::theTime(void)
{
	CTime nowTime;
	nowTime = CTime::GetCurrentTime();
	CString stime = nowTime.Format( "%m-%d-%y-%H-%M-%S" );
	return(stime);
}

void CDamFixDlg::OnBnClickedSvrChange()
{
    UpdateData(true);

    if (m_server.IsEmpty() ) 
    {
        AfxMessageBox("Enter Current Server Name First!", MB_ICONSTOP);
        return;
    }

    int qAns;
	std::string pMsg;

    pMsg = "Do you wish to CHANGE the Server Name\n";
    pMsg += "for selection's found ?";
    qAns = AfxMessageBox(pMsg.c_str(),  MB_YESNOCANCEL | MB_ICONQUESTION);
    if ( (qAns == IDNO) || (qAns == IDCANCEL))
    {
        m_changeServer = false;
    }
    else
    {
        CServerName* dlg = new CServerName(m_server);
        
        if ( dlg->DoModal() != IDOK )
        {
             delete dlg;
             m_changeServer = false;
             AfxMessageBox("No change to Server Name made!", MB_ICONSTOP);
        }
        else
        {
            m_newServer = dlg->m_newServer.operator LPCTSTR();
            delete dlg;
            if (!m_newServer.empty()) 
                m_changeServer = true;
            else
                m_changeServer = false;
        }
    }

}
