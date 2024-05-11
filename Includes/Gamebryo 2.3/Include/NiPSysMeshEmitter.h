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

#ifndef NIPSYSMESHEMITTER_H
#define NIPSYSMESHEMITTER_H

#include "NiPSysEmitter.h"
#include <NiGeometry.h>
#include <NiSkinPartition.h>

class NiTriBasedGeom;

class NIPARTICLE_ENTRY NiPSysMeshEmitter : public NiPSysEmitter
{
    NiDeclareRTTI;
    NiDeclareClone(NiPSysMeshEmitter);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    enum MeshEmissionType
    {
        NI_EMIT_FROM_VERTICES = 0,
        NI_EMIT_FROM_FACE_CENTER,
        NI_EMIT_FROM_EDGE_CENTER,
        NI_EMIT_FROM_FACE_SURFACE,
        NI_EMIT_FROM_EDGE_SURFACE,
        NI_EMIT_MAX
    };

    enum InitialVelocityType
    {
        NI_VELOCITY_USE_NORMALS = 0,
        NI_VELOCITY_USE_RANDOM,
        NI_VELOCITY_USE_DIRECTION,
        NI_VELOCITY_MAX
    };

    NiPSysMeshEmitter(const char* pcName, NiGeometry* pkGeomEmitter = NULL, 
        MeshEmissionType eMeshEmissionType = NI_EMIT_FROM_VERTICES,
        InitialVelocityType eInitialVelocityType = NI_VELOCITY_USE_NORMALS, 
        NiPoint3 kEmitAxis = NiPoint3::UNIT_X, float fSpeed = 0.0f,
        float fSpeedVar = 0.0f, float fDeclination = 0.0f,
        float fDeclinationVar = 0.0f, float fPlanarAngle = 0.0f,
        float fPlanarAngleVar = 0.0f,
        NiColorA kInitialColor = NiColorA::WHITE, float fInitialRadius = 1.0f,
        float fLifeSpan = 0.0f, float fLifeSpanVar = 0.0f, 
        float fRadiusVar = 1.0f);

    void AddGeometryEmitter(NiGeometry* pkGeomEmitter);
    void RemoveGeometryEmitter(unsigned int uiWhichEmitter);
    void RemoveAllGeometryEmitters();

    NiGeometry* GetGeometryEmitter(unsigned int uiWhichEmitter);
    unsigned int GetGeometryEmitterCount();
    unsigned int GetGeometryEmitterArrayCount();

    MeshEmissionType GetMeshEmissionType();
    void SetMeshEmissionType(MeshEmissionType eMeshEmissionType);

    InitialVelocityType GetInitialVelocityType();
    void SetInitialVelocityType(InitialVelocityType eInitialVelocityType);

    const NiPoint3& GetEmitAxis();
    void SetEmitAxis(const NiPoint3& kEmitAxis);

    // *** begin Emergent internal use only ***
    virtual void ProcessClone(NiCloningProcess& kCloning);
    // *** end Emergent internal use only ***

protected:

    // For cloning and streaming only.
    NiPSysMeshEmitter();

    virtual void ComputeInitialPositionAndVelocity(
        NiPoint3& kPosition, NiPoint3& kVelocity);

    bool EmitFromVertex(NiGeometry* pkGeomEmitter, NiPoint3& kPosition,
        NiPoint3& kVelocity);
    bool EmitFromFace(NiGeometry* pkGeomEmitter, NiPoint3& kPosition,
        NiPoint3& kVelocity);
    bool EmitFromEdge(NiGeometry* pkGeomEmitter, NiPoint3& kPosition,
        NiPoint3& kVelocity);
    void TransformIntoCoordinateSpace(NiAVObject* pkObj, NiPoint3& kPosition,
        NiPoint3& kVelocity);
    void TransformIntoSkinnedCoordinateSpace(NiSkinInstance* pkSkin,
        NiPoint3& kPosition, NiPoint3& kVelocity);

    class NiSkinnedEmitterData : public NiRefObject
    {
    public:
        bool CreatePartition(NiGeometry* pkGeom);

        NiSkinPartitionPtr m_spSkinPartition;
    };

    typedef NiPointer<NiSkinnedEmitterData> NiSkinnedEmitterDataPtr;

    bool EmitFromSkinnedVertex(NiSkinnedEmitterData* pkSkinEmitterData, 
        NiGeometry* pkGeomEmitter, NiPoint3& kPosition, NiPoint3& kVelocity);
    bool EmitFromSkinnedFace(NiSkinnedEmitterData* pkSkinEmitterData, 
        NiGeometry* pkGeomEmitter, NiPoint3& kPosition, NiPoint3& kVelocity);
    bool EmitFromSkinnedEdge(NiSkinnedEmitterData* pkSkinEmitterData, 
        NiGeometry* pkGeomEmitter, NiPoint3& kPosition, NiPoint3& kVelocity);
    void DeformBySkin(NiPoint3& kVert, NiPoint3& kNormal,
        NiTriBasedGeom* pkGeom, NiSkinInstance* pkSkin,
        NiSkinPartition::Partition* pkPart, unsigned int uiWhichIndex);

    NiSkinnedEmitterData* GetSkinnedEmitterData(unsigned int uiWhichEmitter);
    unsigned int GetSkinnedEmitterDataCount();

    void SetPrecacheAttributes(NiGeometry* pkObj);

    // Emitter object and size.
    NiTObjectArray<NiGeometryPtr>  m_kGeomEmitterArray;
    NiTObjectArray<NiSkinnedEmitterDataPtr> m_kSkinnedEmitterData;
    InitialVelocityType m_eInitVelocityType;
    MeshEmissionType m_eEmissionType;
    NiPoint3 m_kEmitAxis;
};

NiSmartPointer(NiPSysMeshEmitter);

#include "NiPSysMeshEmitter.inl"

#endif  // #ifndef NIPSYSMESHEMITTER_H
