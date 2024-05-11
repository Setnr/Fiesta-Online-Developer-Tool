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

#ifndef NIMATERIALINSTANCE_H
#define NIMATERIALINSTANCE_H

#include "NiMemObject.h"
#include "NiMaterial.h"
#include "NiShader.h"

class NiDynamicEffectState;
class NiGeometry;
class NiPropertyState;
class NiSkinInstance;

NiSmartPointer(NiMaterialInstance);

class NIMAIN_ENTRY NiMaterialInstance : public NiMemObject
{
public:
    NiMaterialInstance();
    NiMaterialInstance(const NiMaterialInstance&);
    NiMaterialInstance(NiMaterial* pkMaterial);
    ~NiMaterialInstance();

    NiMaterial* GetMaterial() const;
    void ClearCachedShader();

    NiShader* GetCachedShader(const NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects) const;
    NiShader* GetCurrentShader(NiGeometry* pkGeometry, 
        const NiSkinInstance* pkSkin, const NiPropertyState* pkState, 
        const NiDynamicEffectState* pkEffects);

    void SetMaterialNeedsUpdate(bool bNeedsUpdate);

    // *** begin Emergent internal use only ***
    NiMaterialInstance(NiMaterial* pkMaterial, 
        unsigned int uiMaterialExtraData);
    void SetMaterialExtraData(unsigned int uiExtraData);
    unsigned int GetMaterialExtraData() const;

    enum
    {
        DEFAULT_EXTRA_DATA = NiShader::DEFAULT_IMPLEMENTATION
    };

    void Reinitialize();
    // *** end Emergent internal use only ***

protected:
    NiShaderPtr m_spCachedShader;
    NiMaterialPtr m_spMaterial;
    unsigned int m_uiMaterialExtraData;

    enum MaterialUpdateFlag
    {
        CLEAN,
        DIRTY,
        UNKNOWN
    };

    MaterialUpdateFlag m_eNeedsUpdate;
};

#include "NiMaterialInstance.inl"

#endif // NIMATERIALINSTANCE_H
