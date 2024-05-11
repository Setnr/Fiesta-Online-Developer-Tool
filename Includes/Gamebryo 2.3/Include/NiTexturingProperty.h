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

#ifndef NITEXTURINGPROPERTY_H
#define NITEXTURINGPROPERTY_H

#include "NiProperty.h"
#include "NiTArray.h"
#include "NiTexture.h"
#include "NiTextureTransform.h"
#include "NiFlags.h"
#include "NiFixedString.h"

NiSmartPointer(NiTexturingProperty);

class NIMAIN_ENTRY NiTexturingProperty : public NiProperty
{
    NiDeclareRTTI;
    NiDeclareClone(NiTexturingProperty);
    NiDeclareStream;
    NiDeclareViewerStrings;
    NiDeclareFlags(unsigned short);

public:
    NiTexturingProperty();
    ~NiTexturingProperty();

    // creates a texturing property and attaches a base texture
    // created from the given filename
    NiTexturingProperty(const char* pcTextureName);
    
    // creates a texturing property and attaches a base texture
    // created from the given raw data
    NiTexturingProperty(NiPixelData* pkPixelData);

    enum FilterMode
    {
        FILTER_NEAREST,
        FILTER_BILERP,
        FILTER_TRILERP,
        FILTER_NEAREST_MIPNEAREST,
        FILTER_NEAREST_MIPLERP,
        FILTER_BILERP_MIPNEAREST,
        FILTER_MAX_MODES
    };

    enum ClampMode
    { 
        CLAMP_S_CLAMP_T,
        CLAMP_S_WRAP_T,
        WRAP_S_CLAMP_T,
        WRAP_S_WRAP_T,
        CLAMP_MAX_MODES
    };

    enum ApplyMode
    { 
        // see table below for apply mode equations
        APPLY_REPLACE,
        APPLY_DECAL,
        APPLY_MODULATE,

        // Modes no longer supported
        APPLY_DEPRECATED,
        APPLY_DEPRECATED2,

        APPLY_MAX_MODES
    };

    enum MapClassID
    {
        MAP_CLASS_BASE = 0,
        MAP_CLASS_BUMP,
        MAP_CLASS_PARALLAX,
        MAP_CLASS_SHADER,
        MAP_CLASS_MAX
    };

    class NIMAIN_ENTRY Map : public NiMemObject
    {
        NiDeclareFlags(unsigned short);
    public:
        Map();
        Map(NiTexture* pkTexture, unsigned int uiIndex,
            ClampMode eClampMode = WRAP_S_WRAP_T, 
            FilterMode eFilterMode = FILTER_BILERP,
            NiTextureTransform* pkTextureTransform = NULL);
        Map(const Map& kObj);

        virtual ~Map();

        bool operator==(const Map& kObj) const;
        bool operator!=(const Map& kObj) const;

        // image access functions
        NiTexture* GetTexture() const;
        void SetTexture(NiTexture* pkTexture);

        // clamp mode access functions
        ClampMode GetClampMode() const;
        void SetClampMode(ClampMode eClampMode);

        // filter mode access functions
        FilterMode GetFilterMode() const;
        void SetFilterMode(FilterMode eFilterMode);

        unsigned int GetTextureIndex() const;
        void SetTextureIndex(unsigned int uiIndex);

        NiTextureTransform* GetTextureTransform();
        const NiTextureTransform* GetTextureTransform() const;
        void SetTextureTransform(NiTextureTransform* pkTrextureTransform);

        // *** begin Emergent internal use only ***

        bool IsEqual(Map* pObject);
        virtual void LoadBinary(NiStream& stream);
        virtual void SaveBinary(NiStream& stream);
        virtual MapClassID GetClassID() const;

        // *** end Emergent internal use only ***
    protected:

        // flags
        enum
        {
            TEXCOORD_MASK   = 0x00FF,
            TEXCOORD_POS    = 0,
            FILTERMODE_MASK = 0x0F00,
            FILTERMODE_POS  = 8,
            CLAMPMODE_MASK  = 0x3000,
            CLAMPMODE_POS   = 12
        };

        NiTexturePtr m_spTexture;

        NiTextureTransform* m_pkTextureTransform;
    };

    class NIMAIN_ENTRY BumpMap : public Map
    {
    public:
        BumpMap();
        BumpMap(NiTexture* pkTexture, unsigned int uiIndex,
            ClampMode eClampMode = WRAP_S_WRAP_T, 
            FilterMode eFilterMode = FILTER_BILERP,
            float fLumaScale = 1.0f, float fLumaOffset = 0.0f,
            float fBumpMat00 = 0.5f, float fBumpMat01 = 0.0f,  
            float fBumpMat10 = 0.0f, float fBumpMat11 = 0.5f);

        float GetLumaScale() const;
        void SetLumaScale(float fVal);
        float GetLumaOffset() const;
        void SetLumaOffset(float fVal);
        float GetBumpMat00() const;
        void SetBumpMat00(float fVal);
        float GetBumpMat01() const;
        void SetBumpMat01(float fVal);
        float GetBumpMat10() const;
        void SetBumpMat10(float fVal);
        float GetBumpMat11() const;
        void SetBumpMat11(float fVal);

        virtual ~BumpMap();

        bool operator==(const BumpMap& kObj) const;
        bool operator!=(const BumpMap& kObj) const;

        // *** begin Emergent internal use only ***

        bool IsEqual(BumpMap* pkObject);
        virtual void LoadBinary(NiStream& kStream);
        virtual void SaveBinary(NiStream& kStream);
        virtual MapClassID GetClassID() const;
        // *** end Emergent internal use only ***
    protected:
        float m_fLumaScale;
        float m_fLumaOffset;

        float m_fBumpMat00;
        float m_fBumpMat01;
        float m_fBumpMat10;
        float m_fBumpMat11;
    };

    class NIMAIN_ENTRY ShaderMap : public Map
    {
    public:
        ShaderMap();
        ShaderMap(NiTexture* pkTexture, unsigned int uiIndex,
            ClampMode eClampMode = WRAP_S_WRAP_T, 
            FilterMode eFilterMode = FILTER_BILERP,
            unsigned int uiID = 0);

        unsigned int GetID() const;
        void SetID(unsigned int uiID);

        virtual ~ShaderMap();

        bool operator==(const ShaderMap& kObj) const;
        bool operator!=(const ShaderMap& kObj) const;

        // *** begin Emergent internal use only ***

        bool IsEqual(ShaderMap* pkObject);
        virtual void LoadBinary(NiStream& kStream);
        virtual void SaveBinary(NiStream& kStream);
        virtual MapClassID GetClassID() const;
        // *** end Emergent internal use only ***
    protected:
        unsigned int m_uiID;
    };

    class NIMAIN_ENTRY ParallaxMap : public Map
    {
    public:
        ParallaxMap();
        ParallaxMap(NiTexture* pkTexture, unsigned int uiIndex,
            ClampMode eClampMode = WRAP_S_WRAP_T, 
            FilterMode eFilterMode = FILTER_BILERP,
            float fOffset = 0.05f);

        float GetOffset() const;
        void SetOffset(float fOffset);

        virtual ~ParallaxMap();

        bool operator==(const ParallaxMap& kObj) const;
        bool operator!=(const ParallaxMap& kObj) const;

        // *** begin Emergent internal use only ***

        bool IsEqual(ParallaxMap* pkObject);
        virtual void LoadBinary(NiStream& kStream);
        virtual void SaveBinary(NiStream& kStream);  
        virtual MapClassID GetClassID() const;
        // *** end Emergent internal use only ***
    protected:
        float m_fOffset;
    };


    // attach / detach access functions
    const Map* GetBaseMap() const;
    Map* GetBaseMap();
    void SetBaseMap(Map* pkMap);

    const Map* GetDarkMap() const;
    Map* GetDarkMap();
    void SetDarkMap(Map* pkMap);

    const Map* GetDetailMap() const;
    Map* GetDetailMap();
    void SetDetailMap(Map* pkMap);

    const Map* GetGlossMap() const;
    Map* GetGlossMap();
    void SetGlossMap(Map* pkMap);

    const Map* GetGlowMap() const;
    Map* GetGlowMap();
    void SetGlowMap(Map* pkMap);

    const BumpMap* GetBumpMap() const;
    BumpMap* GetBumpMap();
    void SetBumpMap(BumpMap* pkMap);

    const Map* GetNormalMap() const;
    Map* GetNormalMap();
    void SetNormalMap(Map* pkMap);

    const ParallaxMap* GetParallaxMap() const;
    ParallaxMap* GetParallaxMap();
    void SetParallaxMap(ParallaxMap* pkMap);

    const Map* GetDecalMap(unsigned int uiIndex) const;
    Map* GetDecalMap(unsigned int uiIndex);
    void SetDecalMap(unsigned int uiIndex, Map* pkMap);
    unsigned int GetDecalArrayCount() const;
    unsigned int GetDecalMapCount() const;

    const ShaderMap* GetShaderMap(unsigned int uiIndex) const;
    ShaderMap* GetShaderMap(unsigned int uiIndex);
    void SetShaderMap(unsigned int uiIndex, ShaderMap* pkMap);
    unsigned int GetShaderArrayCount() const;
    unsigned int GetShaderMapCount() const;

    // apply modes
    ApplyMode GetApplyMode() const;
    void SetApplyMode(ApplyMode eApplymode);

    bool IsEqualFast(const NiTexturingProperty& kProp) const;

    virtual int Type() const;
    static int GetType();
    static NiTexturingProperty* GetDefault();

    bool IsMultiTexture() const; 

    // Easy access to base map
    NiTexture* GetBaseTexture() const;
    void SetBaseTexture(NiTexture* pkTexture);
    ClampMode GetBaseClampMode() const;
    void SetBaseClampMode(ClampMode eMode);
    FilterMode GetBaseFilterMode() const;
    void SetBaseFilterMode(FilterMode eMode);
    unsigned int GetBaseTextureIndex() const;
    void SetBaseTextureIndex(unsigned int uiIndex);
    NiTextureTransform* GetBaseTextureTransform() const;
    void SetBaseTextureTransform(NiTextureTransform* pkTransform);

    enum MapEnum
    {
        BASE_INDEX,
        DARK_INDEX,
        DETAIL_INDEX,
        GLOSS_INDEX,
        GLOW_INDEX,
        BUMP_INDEX,
        NORMAL_INDEX,
        PARALLAX_INDEX,
        DECAL_BASE,
        SHADER_BASE,
        INDEX_MAX
    };
    typedef NiTPrimitiveArray<Map*>  NiMapArray;

    // Get non-shader maps array
    const NiMapArray& GetMaps() const;

    // Set a single map (NOTE: can not set shader maps)
    void SetMap(unsigned int uiIndex, Map* pkMap);


    // *** begin Emergent internal use only ***

    static void _SDMInit();
    static void _SDMShutdown();

    // streaming support
    static char* GetViewerString(const char* pcPrefix, ClampMode eMode);
    static char* GetViewerString(const char* pcPrefix, FilterMode eMode);
    static char* GetViewerString(const char* pcPrefix, ApplyMode eMode);

    static const NiFixedString& GetMapNameFromID(unsigned int uiID);
    static bool GetMapIDFromName(const NiFixedString& kName, 
        unsigned int& uiID);

    bool ValidMapExists(MapEnum eMap, unsigned int uiWhichIndex = 0) const;
    // *** end Emergent internal use only ***

protected:
    void SetMultiTexture(Map* pkMap);
    void InitializeDecalCount();
    void IncrementDecalCount();
    void DecrementDecalCount();

    // flags
    enum
    {
        MULTITEXTURE_MASK   = 0x0001,
        APPLYMODE_MASK      = 0x000E,
        APPLYMODE_POS       = 1,
        DECALCOUNT_MASK     = 0x0FF0,
        DECALCOUNT_POS      = 4
    };

    NiMapArray m_kMaps;

    unsigned int uiDummy;

    typedef NiTPrimitiveArray<ShaderMap*> NiShaderMapArray;
    NiShaderMapArray* m_pkShaderMaps;

    static NiTexturingPropertyPtr ms_spDefault;
    static NiFixedString ms_akMapIDtoNames[INDEX_MAX];
};

NiSmartPointer(NiTexturingProperty);

#include "NiTexturingProperty.inl"

#endif // NITEXTURINGPROPERTY_H

