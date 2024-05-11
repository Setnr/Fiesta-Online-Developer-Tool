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

#ifndef NISTRIPIFY_H
#define NISTRIPIFY_H

#include "NiStripifyLibType.h"
#include <NiObjectNET.h>
// Uncomment this define to use the nVidia algorithm for 'next generation'
// stripification. Otherwise, the Xbox routines will be used in that case.
//#define _STRIPIFY_USE_NVIDIA_
#if defined(_STRIPIFY_USE_NVIDIA_)
#include "NvTriStrip.h"
#endif  //#if defined(_STRIPIFY_USE_NVIDIA_)

#include <NiMemObject.h>
#include <NiTSet.h>
#include <NiTMap.h>

class NiAVObject;
class NiNode;
class NiSkinPartition;
class NiTriShape;
class NiTriShapeData;
class NiTriStrips;
class NiTriStripsData;
class NiBoneLODController;
class NiTriBasedGeom;
class NiPSysMeshEmitter;
class NiRoom;

class NISTRIPIFY_ENTRY NiStripify : public NiMemObject
{
public:
    // convert NiTriShape to NiTriStrips
    static void Stripify(NiNode* pkNode);
    
    static NiTriStrips* Stripify(NiTriShape* pkTriShape);
#if defined(_STRIPIFY_USE_NVIDIA_)
    static NiTriStrips* Stripify_nVidia(NiTriShape* pkTriShape);
#else   //#if defined(_STRIPIFY_USE_NVIDIA_)
    static NiTriStrips* NiStripify::Stripify_Xbox(NiTriShape* pkTriShape);
#endif  //#if defined(_STRIPIFY_USE_NVIDIA_)

    static void Stripify(NiSkinPartition* pkPartition);
#if defined(_STRIPIFY_USE_NVIDIA_)
    static NiTriStrips* StripifyPartitions_nVidia(
        NiSkinPartition* pkSkinPartition);
#else   //#if defined(_STRIPIFY_USE_NVIDIA_)
    static NiTriStrips* StripifyPartitions_Xbox(
        NiSkinPartition* pkSkinPartition);
#endif  //#if defined(_STRIPIFY_USE_NVIDIA_)

    // callback to indicate whether an object should be converted to strips
    typedef bool (*OkayCallback)(NiAVObject* pkObject, void* pvData);
    // return value of SetOkayCallback is previous value of callback.
    static void SetOkayCallback(OkayCallback pfnCallback, void* pvData);
    static void GetOkayCallback(OkayCallback& pfnCallback, void*& pvData);

    // callback to indicate that an object has been converted to strips
    typedef bool (*FinishedCallback)(NiAVObject* pkObject);
    // return value of SetOkayCallback is previous value of callback.
    static void SetFinishedCallback(FinishedCallback pfnCallback);
    static void GetFinishedCallback(FinishedCallback& pfnCallback);

    // Callback to indicate that an object has caused an error in
    // stripification.  This can be ignored and errors will simply
    // fail silently and some stripification may not occur.
    enum ErrorCode
    {
        STRIPPED_TRISHAPE_TOO_LARGE,
        STRIPPED_PARTITION_TOO_LARGE
    };
    typedef void (*ErrorCallback)(unsigned int uiCode, NiObject* pkObj,
        const char* pcErrorString);
    static void SetErrorCallback(ErrorCallback pfnCallback);
    static void GetErrorCallback(ErrorCallback& pfnCallback);

    // statistics
    // m_uiConvertedShapes is the number of NiTriShapeData objects that have
    // been converted. (The OkayCallback can be used to cause certain
    // objects not to be converted)
    // m_uiTriangles is the sum of the triangle counts for the NiTriShapeData
    // objects that were converted.
    // m_uiStrips is the sum of the number of strips in all the 
    // NiTriStripsData objects that have been created.

    class Stats : public NiMemObject
    {
    public:
        Stats();
        void Reset();

        unsigned int m_uiConvertedShapes;
        unsigned int m_uiShapeTriangles;
        unsigned int m_uiStripsTriangles;
        unsigned int m_uiStrips;
        unsigned int m_uiStripLengthSum;
    };
    static void GetStats(Stats& kStats);
    static void GetnVStats(Stats& kStats);
    // Reinitialize all the values returned by GetStats to 0.
    static void ResetStats();
    static void ResetnVStats();

    static void ResetIndexSum(void);
    static unsigned int GetIndexSum(void);

    static void SetUse_NextGeneration(bool bUse_NextGeneration);
    
    static void Init();
    static void Shutdown();
protected:
    static bool NoStripifyExtraDataIsNotPresent(NiObjectNET* pkObject);

    // Map NiTriShapeData to NiTriStripsData. Used to convert scene graphs 
    // with instanced objects.
    typedef NiTMap<NiTriShapeData*, NiTriStripsData*> ShapeMap;

    static void Stripify(NiNode* pkNode, ShapeMap& map);
    static NiTriStrips* Stripify(NiTriShape* pkTriShape, ShapeMap& map);

    static FinishedCallback ms_pfnFinishedCallback;
    static OkayCallback ms_pfnOkayCallback;
    static ErrorCallback ms_pfnErrorCallback;
    static void* ms_pvCallbackData;
    static Stats ms_kStats;
    static Stats ms_knVStats;

    static unsigned int ms_uiIndexSum;

    static bool ms_bUse_NextGeneration;

#if defined(_STRIPIFY_USE_NVIDIA_)
    static void Run_nVidia_Stripper(const unsigned short* pusTriList, 
        unsigned int uiTriangles, unsigned int uiNumVerts, 
        PrimitiveGroup** ppOldPG, PrimitiveGroup** ppNewPG, 
        unsigned short& usNumSections);
    static bool Get_nVidia_Stripper_Results(PrimitiveGroup* pOldPG, 
        PrimitiveGroup* pNewPG, unsigned short& usNumSections, 
        unsigned short& usTriangles, unsigned short& usStrips, 
        unsigned short*& pusStripLengths, unsigned short*& pusStripLists);
    static unsigned int FindNewIndex(unsigned int uiOldIndex, 
        PrimitiveGroup* pOldPG, PrimitiveGroup* pNewPG);
#endif  //#if defined(_STRIPIFY_USE_NVIDIA_)

    // bone LOD controller static functions and variables
    static NiTPrimitiveSet<NiBoneLODController*>* ms_pkBoneLODSet;
    static void BuildBoneLODSet(NiAVObject* pkObject);
    static void UpdateBoneLODSet(NiTriShape* pkShape, NiTriStrips* pkStrips);
    static bool CheckForBoneLOD(NiTriBasedGeom* pkGeom);
    static void StripifyBoneLODPartitions();

    // Particle System Mesh Emitter static functions and variables
    static NiTPrimitiveSet<NiPSysMeshEmitter*>* ms_pkMeshEmitterSet;
    static void BuildMeshEmitterSet(NiAVObject* pkObject);
    static void UpdateMeshEmitterSet(NiTriShape* pkShape, 
        NiTriStrips* pkStrip);

    // Portal fixture static functions and variables
    static NiTPrimitiveSet<NiRoom*>* ms_pkPortalRoomSet;
    static void BuildFixtureSet(NiAVObject* pkObject);
    static bool UpdateFixtureSet(NiTriShape* pkShape, NiTriStrips* pkStrips);
};

#endif // NISTRIPIFY_H
