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

#ifndef NID3D10SHADERINTERFACE_H
#define NID3D10SHADERINTERFACE_H

#include <NiShader.h>

#include "NiD3D10RendererLibType.h"
#include "NiD3D10VertexDescription.h"

class NID3D10RENDERER_ENTRY NiD3D10ShaderInterface : public NiShader
{
    NiDeclareRTTI;

public:
    NiD3D10ShaderInterface();
    virtual ~NiD3D10ShaderInterface();

    virtual bool Initialize();

    // Vertex Description
    virtual NiD3D10VertexDescription* GetVertexDescription() const;
    virtual void SetVertexDescription(NiD3D10VertexDescription* pkVertexDesc);

    // Allow for complete shader reconstruction
    virtual void DestroyRendererData();
    virtual void RecreateRendererData();

    // Packing particles
    virtual NiGeometryData::RendererData* PrepareGeometryForRendering_Points(
        NiGeometry* pkGeometry, const NiSkinPartition::Partition* pkPartition,
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState) = 0;

    // Packing lines
    virtual NiGeometryData::RendererData* PrepareGeometryForRendering_Lines(
        NiGeometry* pkGeometry, const NiSkinPartition::Partition* pkPartition,
        NiGeometryData::RendererData* pkRendererData, 
        const NiPropertyState* pkState) = 0;

    // *** begin Emergent internal use only

    // This is used by the shader factory to flag that this shader was the
    // best implementation for the hardware.
    virtual bool GetIsBestImplementation() const;
    virtual void SetIsBestImplementation(bool bIsBest);
    // *** end Emergent internal use only

protected:
    bool m_bIsBestImplementation;

    NiD3D10VertexDescriptionPtr m_spVertexDescription;
};

NiSmartPointer(NiD3D10ShaderInterface);

//#include "NiD3D10ShaderInterface.inl"

#endif  //#ifndef NID3D10SHADERINTERFACE_H
