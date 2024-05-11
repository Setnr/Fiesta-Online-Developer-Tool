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


#ifndef NIPHYSXCONVEXMESH_H
#define NIPHYSXCONVEXMESH_H

#include "NiPhysXMemStream.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXLibType.h"

class NiPhysXMeshDesc;

class NIPHYSX_ENTRY NiPhysXConvexMesh : public NiObject
{
    NiDeclareRTTI;

public:
    NiPhysXConvexMesh();
    virtual ~NiPhysXConvexMesh();
    
    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);
    
    NxConvexMesh* GetMesh() const;
    void SetMesh(NxConvexMesh* pkMesh);
    
    const bool GetStreamInline() const;
    void SetStreamInline(const bool bVal);

    NiPhysXMeshDesc* GetMeshDesc() const;
    void SetMeshDesc(NiPhysXMeshDesc* pkDesc);

    static NiPhysXConvexMesh* Create(NiPhysXMeshDesc* pkDesc,
        const NiFixedString& kName);
    static NiPhysXConvexMesh* Create(NiPhysXMeshDesc* pkDesc,
        const NiPhysXMemStream& kStream, const NiFixedString& kName);
        
protected:
    bool NiPhysXConvexMesh::CreateMesh();
    
    NiFixedString m_kName;    
    NxConvexMesh* m_pkMesh;
    bool m_bStreamInline;
    NiPhysXMeshDesc* m_pkMeshDesc;
};

NiSmartPointer(NiPhysXConvexMesh);

#include "NiPhysXConvexMesh.inl"

#endif // #ifdef NIPHYSXCONVEXMESH_H

