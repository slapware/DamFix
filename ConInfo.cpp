// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "ConInfo.h"



//##ModelId=3E0647010267
void ConInfo::Serialize(CArchive& ar)
{
	// TODO: Add your specialized code here.
}

//##ModelId=3E064701026F
void ConInfo::AssertValid() const
{
	// TODO: Add your specialized code here.
}

//##ModelId=3E0647010271
void ConInfo::Dump(CDumpContext& dc) const
{
	// TODO: Add your specialized code here.
}





//  Constructor
//##ModelId=3E0647010266
ConInfo::ConInfo()
{
	    // ToDo: Add your specialized code here and/or call the base class
}



//##ModelId=3E0647010254
void ConInfo::set_user(CString& value)
{
	m_user = value;
	return;
}

//##ModelId=3E064701025C
const CString ConInfo::get_pswd() const
{
	return m_pswd;
}

//##ModelId=3E064701025E
void ConInfo::set_pswd(CString& value)
{
	m_pswd = value;
	return;
}



//##ModelId=3E0647010234
const CString ConInfo::get_schema() const
{
	return m_schema;
}

//##ModelId=3E0647010236
void ConInfo::set_schema(CString& value)
{
	m_schema = value;
	return;
}

//##ModelId=3E064701023E
const CString ConInfo::get_tns() const
{
	return m_tns;
}

//##ModelId=3E0647010240
void ConInfo::set_tns(CString& value)
{
	m_tns = value;
	return;
}

//##ModelId=3E0647010248
const double ConInfo::get_mins() const
{
	return m_mins;
}

//##ModelId=3E064701024A
void ConInfo::set_mins(double value)
{
	m_mins = value;
	return;
}



//  Destructor
//##ModelId=3E0647010221
ConInfo::~ConInfo()
{
	    // ToDo: Add your specialized code here and/or call the base class
}

//##ModelId=3E0647010222
bool ConInfo::operator<(ConInfo& rhs) const
{
	// ToDo: Add your specialized code here and/or call the base class
	if(m_schema < rhs.get_schema() )
		return true;
	return false;
}

//##ModelId=3E064701022B
ConInfo& ConInfo::operator=(ConInfo& rhs)
{
	// ToDo: Add your specialized code here and/or call the base class
	if(&rhs != this)
	{
		m_mins = rhs.get_mins();
		m_user = rhs.get_user();
		m_pswd = rhs.get_pswd();
		m_tns = rhs.get_tns();
		m_schema = rhs.get_schema();
	}
	return *this;
}
///
///  Copy Constructor
ConInfo::ConInfo(const ConInfo& orig)
{
	    // ToDo: Add your specialized code here and/or call the base class
	m_mins = orig.get_mins();
	m_user = orig.get_user();
	m_pswd = orig.get_pswd();
	m_tns = orig.get_tns();
	m_schema = orig.get_schema();
}



//##ModelId=3E0647010252
 const CString ConInfo::get_user() const
{
	return m_user;
}



//##ModelId=3E0647010215
 ConInfo::ConInfo(CString user_, CString pswd_, CString tns_, CString schema_, double mins_):
	m_user(
		user_),
	m_pswd(
		pswd_),
	m_tns(
		tns_),
	m_schema(
		schema_),
	m_mins(
		mins_)

{
	// TODO: Add your specialized code here.
}

