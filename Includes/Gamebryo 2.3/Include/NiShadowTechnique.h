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

#ifndef NISHADOWTECHNIQUE_H
#define NISHADOWTECHNIQUE_H

#include "NiFixedString.h"
#include "NiShadowManager.h"
#include "NiStandardMaterial.h"

class NiMaterialFragmentNode;

class NIMAIN_ENTRY NiShadowTechnique : public NiMemObject
{
    NiDeclareRootRTTI(NiShadowTechnique);
public:
    enum
    {
        INVALID_SHADOWTECHNIQUE_ID = 16
    };

    NiShadowTechnique(const NiFixedString& kName, 
        const NiFixedString kReadFragmentName, 
        const NiFixedString kWriteFragmentName, 
        bool bUseCubeMapForPointLight = false,
        bool bWriteBatchable = false);

    NiShadowTechnique(const NiFixedString& kName, 
        const NiFixedString kDirReadFragmentName, 
        const NiFixedString kPointReadFragmentName, 
        const NiFixedString kSpotReadFragmentName, 
        const NiFixedString kDirWriteFragmentName, 
        const NiFixedString kPointWriteFragmentName, 
        const NiFixedString kSpotWriteFragmentName, 
        bool bUseCubeMapForPointLight,
        bool bWriteBatchable);

    virtual ~NiShadowTechnique(){};

    const NiFixedString& GetName() const;

    const NiFixedString& GetReadFragmentName(
        NiStandardMaterial::LightType eType) const;
    const NiFixedString& GetWriteFragmentName(
        NiStandardMaterial::LightType eType) const;
    
    unsigned short GetActiveTechniqueSlot();
    void SetActiveTechniqueSlot(unsigned short usSlot);
    
    unsigned short GetTechniqueID();
    void SetTechniqueID(unsigned short usID);

    bool IsWriteBatchable();
    void SetWriteBatchable(bool bBatchable);

    bool GetUseCubeMapForPointLight();
    void SetUseCubeMapForPointLight(bool bUseCubeMap);

    NiTexture::FormatPrefs& GetTextureFormatPrefs();
    void SetTextureFormatPrefs(NiTexture::FormatPrefs& pkPrefs);

    virtual void AppendPostProcessRenderClicks(NiShadowMap* pkShadowMap,
        NiShadowClickGenerator* pkClickGenerator);
    virtual void PrepareShadowMap(NiShadowMap* pkShadowMap);

    void SetDefaultDepthBias(NiStandardMaterial::LightType eType,
        bool bRenderBackfaces, float fDepthBias);
    float GetDefaultDepthBias(NiStandardMaterial::LightType eType,
        bool bRenderBackfaces);

protected:
    NiFixedString m_kName;

    NiFixedString m_kReadFragmentName[NiStandardMaterial::LIGHT_MAX];
    NiFixedString m_kWriteFragmentName[NiStandardMaterial::LIGHT_MAX];
    float m_afDefaultDepthBias[NiStandardMaterial::LIGHT_MAX * 2];

    NiTexture::FormatPrefs m_kFormatPrefs;

    unsigned short m_uiActiveTechniqueSlot;
    unsigned short m_usTechniqueID;
    bool m_bWriteBatchable;
    bool m_bUseCubeMapForPointLight;
};


#include "NiShadowTechnique.inl"


#endif // NISHADOWTECHNIQUE_H
