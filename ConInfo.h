#include "stdafx.h"
// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CONINFO_3DDC60F901D4_INCLUDED
#define _INC_CONINFO_3DDC60F901D4_INCLUDED

/// This class contains the connection information for the 
/// database login. Designed to be used as collection of 
/// pointers within STL vector using smart pointers.
class ConInfo 
{
private:
	//##ModelId=3E06470101ED
	CString m_tns;

	//##ModelId=3E06470101F7
	CString m_schema;

	//##ModelId=3E06470101F8
	double m_mins;

	//##ModelId=3E0647010201
	CString m_user;

	//##ModelId=3E064701020B
	CString m_pswd;

public:
	//##ModelId=3E0647010215
	ConInfo(CString user_, CString pswd_, CString tns_, CString schema_, double mins_);

	//  Destructor
	//##ModelId=3E0647010221
	~ConInfo();

	//##ModelId=3E0647010222
	bool operator<(ConInfo& rhs) const;

	//##ModelId=3E064701022B
	ConInfo& operator=(ConInfo& rhs);

	//  Copy Constructor
	//##ModelId=3E064701022D
	ConInfo(const ConInfo& orig);

	//##ModelId=3E0647010234
	const CString get_schema() const;

	//##ModelId=3E0647010236
	void set_schema(CString& value);

	//##ModelId=3E064701023E
	const CString get_tns() const;

	//##ModelId=3E0647010240
	void set_tns(CString& value);

	//##ModelId=3E0647010248
	const double get_mins() const;

	//##ModelId=3E064701024A
	void set_mins(double value);

	//##ModelId=3E0647010252
	 const CString get_user() const;

	//##ModelId=3E0647010254
	void set_user(CString& value);

	//##ModelId=3E064701025C
	const CString get_pswd() const;

	//##ModelId=3E064701025E
	void set_pswd(CString& value);

	//  Constructor
	//##ModelId=3E0647010266
	ConInfo();

	//##ModelId=3E0647010267
	virtual void Serialize(CArchive& ar);

	//##ModelId=3E064701026F
	virtual void AssertValid() const;

	//##ModelId=3E0647010271
	virtual void Dump(CDumpContext& dc) const;

};




#endif /* _INC_CONINFO_3DDC60F901D4_INCLUDED */
