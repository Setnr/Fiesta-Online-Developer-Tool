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

#ifndef NIPSYSMESHUPDATEMODIFIER_H
#define NIPSYSMESHUPDATEMODIFIER_H

#include "NiPSysModifier.h"
#include <NiAVObject.h>

class NIPARTICLE_ENTRY NiPSysMeshUpdateModifier : public NiPSysModifier
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysMeshUpdateModifier);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiPSysMeshUpdateModifier(const char* pcName);

    virtual void Update(float fTime, NiPSysData* pkData);
    virtual void Initialize(NiPSysData* pkData, unsigned short usNewParticle);

    NiAVObject* GetMeshAt(unsigned int uiIndex) const;
    NiAVObjectPtr SetMeshAt(unsigned int uiIndex, NiAVObject* pkMesh);

    unsigned int GetMeshCount() const;

    // *** begin Emergent internal use only ***
    void ProcessClone(NiCloningProcess& kCloning);

    // Streaming
    void PostLinkObject(NiStream& kStream);

    // *** end Emergent internal use only ***

protected:
    // For cloning and streaming only.
    NiPSysMeshUpdateModifier();


    NiTObjectArray<NiAVObjectPtr> m_kMeshes;
};

NiSmartPointer(NiPSysMeshUpdateModifier);

#include "NiPSysMeshUpdateModifier.inl"

#endif  // #ifndef NIPSYSMESHUPDATEMODIFIER_H
