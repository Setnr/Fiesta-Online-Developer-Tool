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


#ifndef NIPHYSXCLOTHMESH_H
#define NIPHYSXCLOTHMESH_H

#include "NiPhysXMemStream.h"

#include <NiMain.h>
#include <NiSystem.h>

#include <NxPhysics.h>

#include "NiPhysXLibType.h"

class NiPhysXMeshDesc;

class NIPHYSX_ENTRY NiPhysXClothMesh : public NiObject
{
    NiDeclareRTTI;

public:
    NiPhysXClothMesh();
    virtual ~NiPhysXClothMesh();
    
    const NiFixedString& GetName() const;
    void SetName(const NiFixedString& kName);
    
    NxClothMesh* GetMesh() const;
    void SetMesh(NxClothMesh* pkMesh);
    
    const bool GetStreamInline() const;
    void SetStreamInline(const bool bVal);

    NiPhysXMeshDesc* GetMeshDesc() const;
    void SetMeshDesc(NiPhysXMeshDesc* pkDesc);

    static NiPhysXClothMesh* Create(NiPhysXMeshDesc* pkDesc,
        const NiFixedString& kName);
    static NiPhysXClothMesh* Create(NiPhysXMeshDesc* pkDesc,
        NiPhysXMemStream& kStream, const NiFixedString& kName);
        
protected:
    bool NiPhysXClothMesh::CreateMesh();
    
    NiFixedString m_kName;    
    NxClothMesh* m_pkMesh;
    bool m_bStreamInline;
    NiPhysXMeshDesc* m_pkMeshDesc;
};

NiSmartPointer(NiPhysXClothMesh);

#include "NiPhysXClothMesh.inl"

#endif // #ifdef NIPHYSXCLOTHMESH_H

