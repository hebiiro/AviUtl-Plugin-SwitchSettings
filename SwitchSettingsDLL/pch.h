#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <comdef.h>
#include <shlobj.h>

#include <tchar.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <locale.h>

#include "../AviUtl/aviutl_plugin_sdk/filter.h"
#include "../Common/MyTracer.h"
#include "../Detours.4.0.1/detours.h"
#pragma comment(lib, "../Detours.4.0.1/detours.lib")

//---------------------------------------------------------------------
