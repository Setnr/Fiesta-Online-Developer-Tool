// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NIOFPOLYGON_H
#define NIOFPOLYGON_H

#include <NiBillboardNode.h>
#include <NiColor.h>
#include <NiMaterialProperty.h>
#include <NiSearchPath.h>
#include <NiSmartPointer.h>
#include <NiSpecularProperty.h>
#include <NiTexturingProperty.h>
#include <NiStencilProperty.h>

#include <mgapistd.h>

#include "NiOpenFlightLibType.h"

NiSmartPointer(NiAlphaProperty);
NiSmartPointer(NiVertexColorProperty);
NiSmartPointer(NiWireframeProperty);

class NiPoint2;


// Enzyme classes for transferring Multigen polygon/vertex information
// to NiTriShape objects.

class NIOPENFLIGHT_ENTRY NiOFPolygon
{
public:
    NiOFPolygon ();
    ~NiOFPolygon ();

    NiAVObject* CreateShape (mgrec* pkPoly, bool bFreezeBillboards);

    static void ClearTextureCache () { ms_kTextureCache.RemoveAll(); }
    static void ClearMaterialCache () { ms_kMaterialCache.RemoveAll(); }

    static void SetFilePath(const char* pcReference) 
    { 
        ms_kPath.SetReferencePath(pcReference);
        
        char acTextures[MAX_PATH];
        NiSprintf(acTextures, MAX_PATH, "%stextures", pcReference);
        ms_kPath.SetDefaultPath(acTextures);
    }

    // Accessors
    static void SetUseInternalTextures(bool bInt) 
    { ms_bInternalTextures = bInt; }

    static bool GetUseInternalTextures() 
    { return ms_bInternalTextures; }

    static void SetUseTrueDoubleSided(bool bUseTrueDoubleSided) 
    { ms_bUseTrueDoubleSided = bUseTrueDoubleSided; }

    static bool GetUseTrueDoubleSided() 
    { return ms_bUseTrueDoubleSided; }

protected:
    static NiMaterialProperty* GetMaterial (mgrec* pkPoly, bool& bAlpha, 
        bool& bSpec);
    void GetAlphaAndBillboard (mgrec* pkPoly, bool& bBillboard,
        NiBillboardNode::FaceMode& eBillboardType, bool& bAlpha);

    static void GetDrawType (mgrec* pkPoly, bool& bDoubleSided,
        bool& bWireFrame);
    static NiTexturingPropertyPtr GetTexture (mgrec* pkPoly);

    static void GetFaceColor (mgrec* pkPoly, NiColorA& kColor,
        NiVertexColorPropertyPtr& spVC, bool& bGouraud, bool& bLit);
    static NiPoint3 GetFaceNormal (mgrec* pkPoly);
    
    static NiPoint3 GetVertexCoord (mgrec* pkVtx);
    static NiPoint3 GetVertexNormal (mgrec* pkVtx, 
        const NiPoint3& kFaceNormal);
    static NiPoint2 GetVertexTexture (mgrec* pkVtx);
    static NiColorA GetVertexColor (mgrec* pkVtx, const NiColorA& kFaceColor,
        const NiColorA& kDiffuseColor, bool bGouraud);

    // shared properties
    static NiAlphaPropertyPtr ms_spAlpha;
    static NiSpecularPropertyPtr ms_spSpec;
    static NiVertexColorPropertyPtr ms_spVCVertex;
    static NiVertexColorPropertyPtr ms_spVCLit;
    static NiVertexColorPropertyPtr ms_spVCLitTimesVertex;
    static NiWireframePropertyPtr ms_spWire;
    static NiStencilPropertyPtr ms_spStencilTwoSidedProperty;

    // cached properties
    static NiTStringMap<NiTexturingPropertyPtr> ms_kTextureCache;
    static NiTStringMap<NiMaterialPropertyPtr> ms_kMaterialCache;

    static NiSearchPath ms_kPath;
    static bool ms_bInternalTextures;
    static bool ms_bUseTrueDoubleSided;

    friend class NiOFPolygonInit;
};


#endif
