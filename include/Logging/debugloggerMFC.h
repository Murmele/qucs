#ifndef DEBUGLOGGERMFC_H
#define DEBUGLOGGERMFC_H

#include "DebugLogger.h"
#include <comdef.h>

typedef std::basic_ostream<TCHAR> stream_type;

inline stream_type &operator<<(stream_type &log, const CString &text)
{
	log << text.operator LPCTSTR();
	return log;
}

inline stream_type &operator<<(stream_type &log, const COleDateTime &dateTime)
{
	log << dateTime.Format();
	return log;
}

inline stream_type &operator<<(stream_type &log, const COleCurrency &money)
{
	log << money.Format();
	return log;
}

inline stream_type &operator<<(stream_type &log, const COleDateTimeSpan &timeSpan)
{
	log << timeSpan.Format(_T("%H:%M:%S"));
	return log;
}

inline stream_type &operator<<(stream_type &log, const COleVariant &var)
{
	COleVariant varNew(var);
	varNew.ChangeType(VT_BSTR);
	_bstr_t sb(varNew.bstrVal);
	
	log << _T("{ ") << var.vt << _T(", \"") << sb.operator TCHAR*() << _T("\" }");
	return log;
}

inline stream_type &operator<<(stream_type &log, const _bstr_t &text)
{
	log << text.operator TCHAR*();
	return log;
}

inline stream_type &operator<<(stream_type &log, const _variant_t &var)
{
	_variant_t varNew(var);
	varNew.ChangeType(VT_BSTR);
	_bstr_t sb(varNew.bstrVal);
	
	log << _T("{ ") << var.vt << _T(", \"") << sb.operator TCHAR*() << _T("\" }");
	return log;
}

inline stream_type &operator<<(stream_type &log, const GUID &guid)
{
	wchar_t out[100];
	StringFromGUID2(guid, out, 100);
	
	log << out;
	
	return log;
}

inline stream_type &operator<<(stream_type &log, const _com_error &error)
{
	log << _T("{ ") 
		<< std::hex << std::showbase << error.Error() 
		<< _T(", \"") 
		<< (error.ErrorMessage()?error.ErrorMessage():_T("")) 
		<< _T("\", \"") 
		<< (*error.Description().GetAddress()?error.Description():_T("")) 
		<< _T("\", ") 
		<< error.GUID() 
		<< _T(", \"") 
		<< (*error.Source().GetAddress()?error.Source():_T("")) 
		<< _T("\" }");
	return log;
}

inline stream_type &operator<<(stream_type &log, const CException &exc)
{
	TCHAR szCause[512];

	const_cast<CException *>(&exc)->GetErrorMessage(szCause, sizeof(szCause)/sizeof(TCHAR));

	log << _T("{ ") 
		<< exc.GetRuntimeClass()->m_lpszClassName 
		<< _T(", \"") 
		<< szCause 
		<< _T("\" }");
	return log;
}

#endif 
