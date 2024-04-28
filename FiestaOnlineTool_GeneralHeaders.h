#pragma once
#include <NiSystemLibType.h>
#include <NiDX9RendererLibType.h>
#include <NiMainLibType.h>
#include <NiAnimationLibType.h>
#include <NSBShaderLibLibType.h>
#include <NSFParserLibLibType.h>
#include <NiInputLibType.h>
#include <NiViewMathLibType.h>

#include <NiAnimation.h>
#include <NiParticle.h>
#include <NiSystem.h>
#include <NiCollision.h>
#include <NiMain.h>

#include <NiDI8InputSystem.h>
#include <NiDX9Renderer.h>
#ifdef _DEBUG
#pragma comment(lib, "NiDX9Renderer23VC80D.lib")
#endif
#ifdef _RELEASE
#pragma comment(lib, "NiDX9Renderer23VC80R.lib")
#endif
#pragma comment(lib, "dxguid.lib")


#include <NiD3D10Renderer.h>
#ifdef _DEBUG
#pragma comment(lib, "NiD3D10Renderer23VC80D.lib")
#endif
#ifdef _RELEASE
#pragma comment(lib, "NiD3D10Renderer23VC80R.lib")
#endif
#include "PgUtil.h" 
#include "PgWin.h"

#include <Windows.h>
#include "StartScene.h"

#include <NiD3DShaderProgramFactory.h>

#define _LIB
#include <NSBShaderLib.h>
#include <NSFParserLib.h>
#include <NiD3DXEffectShaderLib.h>
#include <NSBShaderLibLibType.h>
#include <NiD3DXEffectShaderLibType.h>
#undef _LIB

#if defined(WIN32) 
#ifdef _DEBUG
#pragma comment(lib, "NiBinaryShaderLibDX923VC80D.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D1023VC80D.lib")
#pragma comment(lib, "NSBShaderLibDX923VC80D.lib")
#pragma comment(lib, "NSFParserLibDX923VC80D.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX923VC80D.lib")
#endif 
#ifdef _RELEASE
#pragma comment(lib, "NiBinaryShaderLibDX923VC80R.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D1023VC80R.lib")
#pragma comment(lib, "NSBShaderLibDX923VC80R.lib")
#pragma comment(lib, "NSFParserLibDX923VC80R.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX923VC80R.lib")
#endif

#endif