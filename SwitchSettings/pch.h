#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <tchar.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <locale.h>

#include "../Detours/detours.h"
#pragma comment(lib, "../Detours/detours.lib")
