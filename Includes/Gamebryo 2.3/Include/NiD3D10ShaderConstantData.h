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

#ifndef NID3D10SHADERCONSTANTDATA_H
#define NID3D10SHADERCONSTANTDATA_H

#include "NiD3D10RendererLibType.h"

#include "NiD3D10Headers.h"
#include "NiD3D10DataStream.h"

NiSmartPointer(NiD3D10ShaderConstantData);

class NID3D10RENDERER_ENTRY NiD3D10ShaderConstantData : public NiRefObject
{
public:
    static bool Create(size_t uiBufferSize, unsigned int uiAccessHints,
        NiD3D10ShaderConstantDataPtr& spShaderConstantData);

    NiD3D10DataStream* GetDataStream() const;
    void SetDataStream(NiD3D10DataStream* pkDataStream);

    enum 
    {
        BUFFER_INDEX_UNASSIGNED = UINT_MAX
    };

    unsigned int GetBufferIndex() const;
    void SetBufferIndex(unsigned int uiBufferIndex);    

protected:
    NiD3D10ShaderConstantData(size_t uiBufferSize, unsigned int uiAccessHints);
    virtual ~NiD3D10ShaderConstantData();

    NiD3D10DataStreamPtr m_spDataStream;
    unsigned int m_uiBufferIndex;
};

#endif // NID3D10SHADERCONSTANTDATA_H
