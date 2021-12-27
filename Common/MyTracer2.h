#pragma once

#ifdef _DEBUG

HANDLE g_traceFile = 0;

void trace_init(HINSTANCE instance, LPCTSTR suffix)
{
	// dllDirectory\_dll@exe[suffix].txt

	TCHAR dllDirectoryName[MAX_PATH];
	::GetModuleFileName(instance, dllDirectoryName, MAX_PATH);
	::PathRemoveFileSpec(dllDirectoryName);
	//::StringCbCopy(dllDirectoryName, sizeof(dllDirectoryName), _T("C:\\Temp"));

	TCHAR dllFileSpec[MAX_PATH];
	::GetModuleFileName(instance, dllFileSpec, MAX_PATH);
	::PathStripPath(dllFileSpec);

	TCHAR exeFileSpec[MAX_PATH];
	::GetModuleFileName(0, exeFileSpec, MAX_PATH);
	::PathStripPath(exeFileSpec);

	TCHAR traceFilePath[MAX_PATH];
	::StringCbCopy(traceFilePath, sizeof(traceFilePath), dllDirectoryName);
	::StringCbCat(traceFilePath, sizeof(traceFilePath), _T("\\_"));
	::StringCbCat(traceFilePath, sizeof(traceFilePath), dllFileSpec);
	::StringCbCat(traceFilePath, sizeof(traceFilePath), _T("@"));
	::StringCbCat(traceFilePath, sizeof(traceFilePath), exeFileSpec);

	if (suffix)
		::StringCbCat(traceFilePath, sizeof(traceFilePath), suffix);

	::StringCbCat(traceFilePath, sizeof(traceFilePath), _T(".txt"));

	g_traceFile = ::CreateFile(traceFilePath, GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
/*
	BYTE bom[] = { 0xFF, 0xFE };
	DWORD writeSize = 0;
	::WriteFile(g_traceFile, bom, sizeof(bom), &writeSize, 0);
*/
}

void trace_term()
{
	::CloseHandle(g_traceFile), g_traceFile = 0;
}

void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	DWORD cbLength = (DWORD)(::lstrlen(output) * sizeof(TCHAR));
	DWORD writeSize = 0;
	::WriteFile(g_traceFile, output, cbLength, &writeSize, 0);
}

#else

void trace_init(HINSTANCE instance, LPCTSTR name)
{
}

void trace_term()
{
}

void ___outputLog(LPCTSTR text, LPCTSTR output)
{
}

#endif
