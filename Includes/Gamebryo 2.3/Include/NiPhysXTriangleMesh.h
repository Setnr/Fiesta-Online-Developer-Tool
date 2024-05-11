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


#ifndef NIPHYSXTRIANGLEMESH_H
#define NIPHYSXTRIANGLEMESH_H

#include "NiPhysXLibType.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXMemStream.h"

class NiPhysXMeshDesc;

class NIPHYSX_ENTRY NiPhysXTriangleMesh : public NiObject
{
    NiDeclareRTTI;

public:
    NiPhysXTriangleMesh();
    virtual ~NiPhysXTriangleMesh();
    
    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);
    
    NxTriangleMesh* GetMesh() const;
    void SetMesh(NxTriangleMesh* pkMesh);
    
    const bool GetStreamInline() const;
    void SetStreamInline(const bool bVal);
    
    NiPhysXMeshDesc* GetMeshDesc() const;
    void SetMeshDesc(NiPhysXMeshDesc* pkDesc);

    static NiPhysXTriangleMesh* Create(NiPhysXMeshDesc* pkDesc,
        const NiFixedString& kName);
    static NiPhysXTriangleMesh* Create(NiPhysXMeshDesc* pkDesc,
        const NiPhysXMemStream& kStream, const NiFixedString& kName);
        
protected:
    bool NiPhysXTriangleMesh::CreateMesh();
    
    NiFixedString m_kName;    
    NxTriangleMesh* m_pkMesh;
    bool m_bStreamInline;
    NiPhysXMeshDesc* m_pkMeshDesc;
};

NiSmartPointer(NiPhysXTriangleMesh);

#include "NiPhysXTriangleMesh.inl"

#endif // #ifdef NIPHYSXTRIANGLEMESH_H

