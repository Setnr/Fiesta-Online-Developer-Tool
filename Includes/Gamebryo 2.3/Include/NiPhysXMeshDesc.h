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


#ifndef NIPHYSXMESHDESC_H
#define NIPHYSXMESHDESC_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxCooking.h>
#include <NxPhysics.h>

class NIPHYSX_ENTRY NiPhysXMeshDesc : public NiObject
{
    NiDeclareStream;
    NiDeclareRTTI;

public:
    NiPhysXMeshDesc();
    ~NiPhysXMeshDesc();
    
    bool GetIsCloth() const;
    void SetIsCloth();

    bool GetIsConvex() const;
    void SetIsConvex();

    bool GetIsTriMesh() const;
    void SetIsTriMesh();
    
    bool GetPlatform(NxPlatform& kPlatform) const;
    void SetPlatform(const NxPlatform kPlatform);
    bool IsValidPlatform() const;
    
    NxU32 GetFlags() const;
    void SetFlags(const NxU32 uiFlags);

    bool GetHardware() const;
    void SetHardware(const bool bHardware);
    NxMeshPagingMode GetMeshPagingMode() const;
    void SetMeshPagingMode(const NxMeshPagingMode eMode);

    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);

    void GetData(size_t& kSize, unsigned char** ppucData);
    void SetData(const size_t uiSize, unsigned char* pucData);

    // A map that associates all the vertices in a Gamebryo mesh with those
    // in the cloth mesh.
    void GetVertexMap(unsigned short& usSize, const unsigned short*& pusMap);
    void SetVertexMap(const unsigned short usSize, unsigned short* pusMap);

    virtual void ToConvexDesc(NxConvexShapeDesc& kConvexDesc,
        const bool bKeepData, const bool bCheckPlatform = true);
    virtual void ToTriMeshDesc(NxTriangleMeshShapeDesc& kTriMeshDesc,
        const bool bKeepData, const bool bCheckPlatform = true);
    virtual void ToClothDesc(NxClothDesc& kClothDesc, const bool bKeepData,
        const bool bCheckPlatform = true);
 
    // *** begin Emergent internal use only ***

    virtual bool StreamCanSkip();

    // *** end Emergent internal use only ***

protected:
    NxU32 m_uiMeshFlags;
    NiFixedString m_kMeshName;
    size_t m_uiMeshSize;
    unsigned char* m_pucMeshData;
    NxMeshPagingMode m_eMeshPagingMode;
    unsigned short m_usVertexMapSize;
    unsigned short* m_pusVertexMap;

    // Gamebryo flags
    enum {
        IS_CONVEX_MASK = 0x01,
        IS_CLOTH_MASK  = 0x02,
        HARDWARE_MASK  = 0x04,
        COOKED_FOR_WIN32_MASK = 0x08,
        COOKED_FOR_PS3_MASK = 0x10,
        COOKED_FOR_XENON_MASK = 0x20
    };
    
    NiDeclareFlags(unsigned char);
};

NiSmartPointer(NiPhysXMeshDesc);

#include "NiPhysXMeshDesc.inl"

#endif // NIPHYSXMESHDESC_H

