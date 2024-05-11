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

#ifndef NSBIMPLEMENTATION_H
#define NSBIMPLEMENTATION_H

#include "NiBinaryShaderLibLibType.h"
#include "NSBRequirements.h"
#include "NSBRenderStates.h"
#include "NSBStateGroup.h"
#include "NSBConstantMap.h"
#include "NSBUserDefinedDataSet.h"

class NSBPass;
NiSmartPointer(NiBinaryShader);

class NIBINARYSHADERLIB_ENTRY NSBImplementation : public NiMemObject
{
public:
    NSBImplementation();
    ~NSBImplementation();

    // Name
    const char* GetName();
    void SetName(const char* pcName);
    const char* GetDesc();
    void SetDesc(const char* pcDesc);

    // User-defined class name
    const char* GetClassName();
    void SetClassName(const char* pcClassName);

    // Requirements
    NSBRequirements* GetRequirements();

    // PackingDef
    const char* GetPackingDef();
    void SetPackingDef(const char* pcPackingDef);

    // RenderStateGroup
    NSBStateGroup* GetRenderStateGroup();

    // Constant Maps
    unsigned int GetVertexConstantMapCount();
    unsigned int GetGeometryConstantMapCount();
    unsigned int GetPixelConstantMapCount();
    NSBConstantMap* GetVertexConstantMap(unsigned int uiIndex);
    NSBConstantMap* GetGeometryConstantMap(unsigned int uiIndex);
    NSBConstantMap* GetPixelConstantMap(unsigned int uiIndex);

    // Passes
    unsigned int GetPassCount();
    NSBPass* GetPass(unsigned int uiIndex, bool bCreate = true);

    // User defined data set
    NSBUserDefinedDataSet* GetUserDefinedDataSet();
    void SetUserDefinedDataSet(NSBUserDefinedDataSet* pkUDDSet);

    // *** begin Emergent internal use only
    // Index
    unsigned int GetIndex();
    void SetIndex(unsigned int uiIndex);

    void SetSoftwareVP(bool bSoftwareVP);
    bool GetSoftwareVP() const;

    bool SetupNiBinaryShader(NiBinaryShader& kShader, 
        NiShaderDesc* pkShaderDesc);
    bool ReleaseShader();

    // Serialization
    bool SaveBinary(NiBinaryStream& kStream);
    bool LoadBinary(NiBinaryStream& kStream);

#if defined(_DEBUG)
    void Dump(FILE* pf);
#endif  //#if defined(_DEBUG)
    // *** end Emergent internal use only

protected:
    bool SaveBinaryRequirements(NiBinaryStream& kStream);
    bool SaveBinaryPackingDef(NiBinaryStream& kStream);
    bool SaveBinaryPasses(NiBinaryStream& kStream);

    bool LoadBinaryRequirements(NiBinaryStream& kStream);
    bool LoadBinaryPackingDef(NiBinaryStream& kStream);
    bool LoadBinaryPasses(NiBinaryStream& kStream);

    char* m_pcName;
    char* m_pcDesc;
    char* m_pcClassName;

    unsigned int m_uiIndex;

    NSBRequirements* m_pkRequirements;

    // User defined data set
    NSBUserDefinedDataSetPtr m_spUserDefinedDataSet;

    // Packing definition name
    char* m_pcPackingDef;

    // 'Global' render state settings for the shader
    NSBStateGroup* m_pkRenderStateGroup;
    //  'Global' vertex shader mapping for the shader
    NiTPrimitiveSet<NSBConstantMap*> m_kVertexShaderConstantMaps;
    //  'Global' geometry shader mapping for the shader
    NiTPrimitiveSet<NSBConstantMap*> m_kGeometryShaderConstantMaps;
    //  'Global' pixel shader mapping for the shader
    NiTPrimitiveSet<NSBConstantMap*> m_kPixelShaderConstantMaps;

    NiTPrimitiveArray<NSBPass*> m_kPasses;

    NiBinaryShaderPtr m_spShader;

    bool m_bSoftwareVP;
};

#include "NSBImplementation.inl"

#endif  //NSBIMPLEMENTATION_H
