#include "stdafx.h"
using namespace Service;

#ifdef __G1
#define ENGINE Engine_G1
#include <Service/Macro.h>
#include <Service/ZenGin.h>
#include "..\plugin\aIncludes.hpp"
#include <Service/Macro.h>
#endif

#ifdef __G1A
#define ENGINE Engine_G1A
#include <Service/Macro.h>
#include <Service/ZenGin.h>
#include "..\plugin\aIncludes.hpp"
#include <Service/Macro.h>
#endif

#ifdef __G2
#define ENGINE Engine_G2
#include <Service/Macro.h>
#include <Service/ZenGin.h>
#include "..\plugin\aIncludes.hpp"
#include <Service/Macro.h>
#endif

#ifdef __G2A
#define ENGINE Engine_G2A
#include <Service/Macro.h>
#include <Service/ZenGin.h>
#include "..\plugin\aIncludes.hpp"
#include <Service/Macro.h>
#endif

HOOKSPACE(Global, true);