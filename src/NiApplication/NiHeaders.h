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
#ifdef _COMPILENILIB
#pragma comment(lib, "NiDX9Renderer.lib")
#else
#pragma comment(lib, "NiDX9Renderer23VC80D.lib")
#endif
#endif
#ifdef _RELEASE
#ifdef _COMPILENILIB
#pragma comment(lib, "NiDX9Renderer.lib")
#else
#ifdef NISHIPPING
#pragma comment(lib, "NiDX9Renderer23VC80S.lib")
#else
#pragma comment(lib, "NiDX9Renderer23VC80R.lib")
#endif
#endif
#endif
#pragma comment(lib, "dxguid.lib")


#include <NiD3D10Renderer.h>
#ifdef _DEBUG
#ifdef _COMPILENILIB
#pragma comment(lib, "NiD3D10Renderer.lib")
#else
#pragma comment(lib, "NiD3D10Renderer23VC80D.lib")
#endif
#endif
#ifdef _RELEASE
#ifdef _COMPILENILIB
#pragma comment(lib, "NiD3D10Renderer.lib")
#else
#ifdef NISHIPPING
#pragma comment(lib, "NiD3D10Renderer23VC80S.lib")
#else
#pragma comment(lib, "NiD3D10Renderer23VC80R.lib")
#endif
#endif
#endif
#include "PgUtil.h" 

#include <Windows.h>

#include <NiD3DShaderProgramFactory.h>
#include <NiD3DShader.h>

#define _LIB
#include <NSBShaderLib.h>
#include <NSFParserLib.h>
#include <NiD3DXEffectShaderLib.h>
#include <NSBShaderLibLibType.h>
#include <NiD3DXEffectShaderLibType.h>
#undef _LIB

#if defined(WIN32) 
#ifdef _DEBUG
#ifdef _COMPILENILIB
#pragma comment(lib, "NiBinaryShaderLibDX9.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D10.lib")
#pragma comment(lib, "NSBShaderLibDX9.lib")
#pragma comment(lib, "NSFParserLibDX9.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX9.lib")
#else
#pragma comment(lib, "NiBinaryShaderLibDX923VC80D.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D1023VC80D.lib")
#pragma comment(lib, "NSBShaderLibDX923VC80D.lib")
#pragma comment(lib, "NSFParserLibDX923VC80D.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX923VC80D.lib")
#endif
#endif 
#ifdef _RELEASE
#ifdef _COMPILENILIB
#pragma comment(lib, "NiBinaryShaderLibDX9.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D10.lib")
#pragma comment(lib, "NSBShaderLibDX9.lib")
#pragma comment(lib, "NSFParserLibDX9.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX9.lib")
#else
#ifdef NISHIPPING
#pragma comment(lib, "NiBinaryShaderLibDX923VC80S.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D1023VC80S.lib")
#pragma comment(lib, "NSBShaderLibDX923VC80S.lib")
#pragma comment(lib, "NSFParserLibDX923VC80S.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX923VC80S.lib")
#else
#pragma comment(lib, "NiBinaryShaderLibDX923VC80R.lib")
#pragma comment(lib, "NiD3D10BinaryShaderLibD3D1023VC80R.lib")
#pragma comment(lib, "NSBShaderLibDX923VC80R.lib")
#pragma comment(lib, "NSFParserLibDX923VC80R.lib")
#pragma comment(lib, "NiD3DXEffectShaderLibDX923VC80R.lib")
#endif
#endif
#endif

#endif