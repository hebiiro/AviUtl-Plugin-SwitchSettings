#include "pch.h"
#include "../Common/MyTracer2.h"

//---------------------------------------------------------------------

HINSTANCE g_instance = 0;
_bstr_t g_folderName = L"";
char g_aviutl_ini[MAX_PATH] = {};
char g_exedit_ini[MAX_PATH] = {};
char g_default_cfg[MAX_PATH] = {};

//---------------------------------------------------------------------

#define DECLARE_PROC(resultType, callType, procName, args) \
	typedef resultType (callType *Type_##procName) args; \
	Type_##procName true_##procName = procName; \
	resultType callType hook_##procName args

//---------------------------------------------------------------------

DECLARE_PROC(UINT, WINAPI, GetPrivateProfileIntA, (
    _In_     LPCSTR lpAppName,
    _In_     LPCSTR lpKeyName,
    _In_     INT nDefault,
    _In_opt_ LPCSTR lpFileName))
{
	MY_TRACE(_T("GetPrivateProfileIntA(%hs)\n"), lpFileName);

	if (lpFileName)
	{
		if (::StrStrIA(lpFileName, "aviutl.ini")) lpFileName = g_aviutl_ini; else 
		if (::StrStrIA(lpFileName, "exedit.ini")) lpFileName = g_exedit_ini;
	}

	return true_GetPrivateProfileIntA(
		lpAppName,
		lpKeyName,
		nDefault,
		lpFileName);
}

DECLARE_PROC(DWORD, WINAPI, GetPrivateProfileStringA, (
    _In_opt_ LPCSTR lpAppName,
    _In_opt_ LPCSTR lpKeyName,
    _In_opt_ LPCSTR lpDefault,
    _Out_writes_to_opt_(nSize, return + 1) LPSTR lpReturnedString,
    _In_     DWORD nSize,
    _In_opt_ LPCSTR lpFileName))
{
	MY_TRACE(_T("GetPrivateProfileStringA(%hs)\n"), lpFileName);

	if (lpFileName)
	{
		if (::StrStrIA(lpFileName, "aviutl.ini")) lpFileName = g_aviutl_ini; else 
		if (::StrStrIA(lpFileName, "exedit.ini")) lpFileName = g_exedit_ini;
	}

	return true_GetPrivateProfileStringA(
		lpAppName,
		lpKeyName,
		lpDefault,
		lpReturnedString,
		nSize,
		lpFileName);
}

DECLARE_PROC(BOOL, WINAPI, WritePrivateProfileStringA, (
    _In_opt_ LPCSTR lpAppName,
    _In_opt_ LPCSTR lpKeyName,
    _In_opt_ LPCSTR lpString,
    _In_opt_ LPCSTR lpFileName))
{
	MY_TRACE(_T("WritePrivateProfileStringA(%hs)\n"), lpFileName);

	if (lpFileName)
	{
		if (::StrStrIA(lpFileName, "aviutl.ini")) lpFileName = g_aviutl_ini; else 
		if (::StrStrIA(lpFileName, "exedit.ini")) lpFileName = g_exedit_ini;
	}

	return true_WritePrivateProfileStringA(
		lpAppName,
		lpKeyName,
		lpString,
		lpFileName);
}

DECLARE_PROC(HANDLE, WINAPI, CreateFileA, (
	_In_ LPCSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile))
{
	MY_TRACE(_T("CreateFileA(%hs)\n"), lpFileName);

	if (lpFileName)
	{
		if (::StrStrIA(lpFileName, "デフォルト.cfg")) lpFileName = g_default_cfg;
	}

	return true_CreateFileA(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

DECLARE_PROC(HANDLE, WINAPI, CreateFileW, (
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile))
{
	MY_TRACE(_T("CreateFileW(%ws)\n"), lpFileName);

	return true_CreateFileW(
		lpFileName,
		dwDesiredAccess,
		dwShareMode,
		lpSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
}

//---------------------------------------------------------------------
#if 0
template<class T>
class Freer
{
public:
	T m_value;
	Freer(T value = 0) : m_value(value){}
	~Freer(){ ::CoTaskMemFree(m_value); }
	operator T() const { return m_value; }
	T* operator&(){ return &m_value; }
};
#endif
HRESULT getFolderName()
{
	_COM_SMARTPTR_TYPEDEF(IShellItem, IID_IShellItem);
	_COM_SMARTPTR_TYPEDEF(IFileDialog, IID_IFileDialog);

	HRESULT hr = E_FAIL;

	// 初期フォルダの IDL を取得する。
	WCHAR initFolderName[MAX_PATH] = {};
	::GetModuleFileNameW(g_instance, initFolderName, MAX_PATH);
	::PathRemoveExtensionW(initFolderName);
//	Freer<LPITEMIDLIST> idl = 0;
	LPITEMIDLIST idl = 0;
	hr = ::SHILCreateFromPath(initFolderName, &idl, 0);
	if (FAILED(hr)) return hr;

	// 初期フォルダのシェルアイテムを取得する。
	IShellItemPtr si;
	hr = ::SHCreateShellItem(0, 0, idl, &si);
	::CoTaskMemFree(idl); // ここで IDL を解放する。
	if (FAILED(hr)) return hr;

	// COM 版ファイル選択ダイアログのインスタンスを作成する。
	IFileDialogPtr pDialog;
	hr = ::CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pDialog));
	if (FAILED(hr)) return hr;

	// フォルダを選択するように設定をする。
	DWORD options = 0;
	pDialog->GetOptions(&options);
	pDialog->SetOptions(options | FOS_PICKFOLDERS);

	// 初期フォルダを設定する。
	hr = pDialog->SetFolder(si);
	if (FAILED(hr)) return hr;

	// ファイル選択ダイアログを表示する。
	hr = pDialog->Show(::GetDesktopWindow());
	if (FAILED(hr)) return hr;

	// 結果を取得する。
	IShellItem *pItem = NULL;
	hr = pDialog->GetResult(&pItem);
	if (FAILED(hr)) return hr;

	// 結果からファイルパスを取得する。
	PWSTR pPath = NULL;
	hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
	if (FAILED(hr)) return hr;

	// ファイルパスを格納する。
	g_folderName = pPath;
	MY_TRACE_WSTR(g_folderName.GetBSTR());

	// メモリを解放する。
	::CoTaskMemFree(pPath);

	return S_OK;
}

void createFilePath()
{
	if (!g_folderName.GetBSTR())
		return;

	::StringCbCopyA(g_aviutl_ini, sizeof(g_aviutl_ini), g_folderName);
	::StringCbCatA(g_aviutl_ini, sizeof(g_aviutl_ini), "\\aviutl.ini");
	MY_TRACE_STR(g_aviutl_ini);

	::StringCbCopyA(g_exedit_ini, sizeof(g_exedit_ini), g_folderName);
	::StringCbCatA(g_exedit_ini, sizeof(g_exedit_ini), "\\exedit.ini");
	MY_TRACE_STR(g_exedit_ini);

	::StringCbCopyA(g_default_cfg, sizeof(g_default_cfg), g_folderName);
	::StringCbCatA(g_default_cfg, sizeof(g_default_cfg), "\\デフォルト.cfg");
	MY_TRACE_STR(g_default_cfg);
}

//---------------------------------------------------------------------

static int (WINAPI * TrueEntryPoint)(VOID) = NULL;
static int (WINAPI * RawEntryPoint)(VOID) = NULL;

int WINAPI TimedEntryPoint(VOID)
{
	MY_TRACE(_T("TimedEntryPoint()\n"));

	HRESULT hr = ::CoInitialize(0);
	if (SUCCEEDED(hr))
	{
		hr = getFolderName();
		::CoUninitialize();
	}

	if (hr != S_OK)
	{
		::ExitProcess(0);
		return 0;
	}

	createFilePath();

	return TrueEntryPoint();
}

//---------------------------------------------------------------------

EXTERN_C BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			DetourRestoreAfterWith();

			// ロケールを設定する。
			// これをやらないと日本語テキストが文字化けするので最初に実行する。
			_tsetlocale(LC_ALL, _T(""));

			trace_init(instance, _T(""));

			MY_TRACE(_T("DLL_PROCESS_ATTACH\n"));

			g_instance = instance;

			TrueEntryPoint = (int (WINAPI *)(VOID))DetourGetEntryPoint(NULL);
			RawEntryPoint = TrueEntryPoint;

			DetourTransactionBegin();
			DetourUpdateThread(::GetCurrentThread());
			DetourAttach(&(PVOID&)TrueEntryPoint, TimedEntryPoint);
			DetourAttach((PVOID*)&true_CreateFileA, hook_CreateFileA);
//			DetourAttach((PVOID*)&true_CreateFileW, hook_CreateFileW);
			DetourAttach((PVOID*)&true_GetPrivateProfileIntA, hook_GetPrivateProfileIntA);
			DetourAttach((PVOID*)&true_GetPrivateProfileStringA, hook_GetPrivateProfileStringA);
			DetourAttach((PVOID*)&true_WritePrivateProfileStringA, hook_WritePrivateProfileStringA);
			if (DetourTransactionCommit() == NO_ERROR)
			{
				MY_TRACE(_T("API フックに成功しました\n"));
			}
			else
			{
				MY_TRACE(_T("API フックに失敗しました\n"));
				return FALSE;
			}

			break;
		}
	case DLL_PROCESS_DETACH:
		{
			MY_TRACE(_T("DLL_PROCESS_DETACH\n"));

			trace_term();

			break;
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------
