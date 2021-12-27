#include "pch.h"

int WINAPI WinMain(
	HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR     cCmdLine,
	int       showCmd)
{
	// AviUtl.exe のファイルパスを取得する。
	TCHAR exeFileName[MAX_PATH] = {};
	::GetModuleFileName(instance, exeFileName, MAX_PATH);
	::PathRemoveFileSpec(exeFileName);
	::PathAppend(exeFileName, _T("AviUtl.exe"));

	// SwitchSettings.dll のファイルパスを取得する。
	TCHAR dllFileName[MAX_PATH] = {};
	::GetModuleFileName(instance, dllFileName, MAX_PATH);
	::PathRemoveFileSpec(dllFileName);
	::PathAppend(dllFileName, _T("SwitchSettings.dll"));

	// カレントディレクトリを取得する。
	TCHAR currentDirectory[MAX_PATH] = {};
	::GetModuleFileName(instance, currentDirectory, MAX_PATH);
	::PathRemoveFileSpec(currentDirectory);

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = {};

#ifdef UNICODE
	DetourCreateProcessWithDllW(
#else
	DetourCreateProcessWithDllA(
#endif
		0,
		exeFileName,
		0,
		0,
		false,
		0,
		0,
		currentDirectory,
		&si,
		&pi,
		dllFileName,
		0);

	return 0;
}
