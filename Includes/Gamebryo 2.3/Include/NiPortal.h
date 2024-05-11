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

#ifndef NIPORTAL_H
#define NIPORTAL_H

#include "NiPortalLibType.h"
#include "NiPortalMetrics.h"
#include "NiPortalSDM.h"
#include "NiAVObject.h"
#include "NiPlane.h"
#include "NiTPointerList.h"
#include "NiStream.h"
#include "NiCullingProcess.h"
static NiPortalSDM NiPortalSDMObject;

class NiCamera;
class NiFrustum;

class NIPORTAL_ENTRY NiPortal : public NiAVObject
{
    NiDeclareRTTI;
    NiDeclareStream;
    NiDeclareFlags(unsigned short);

public:
    // construction - points are COPIED by object
    NiPortal(unsigned short usVertices, const NiPoint3* pkVertex,
        NiAVObject* pkAdjoiner = 0, bool bActive = true);

    virtual ~NiPortal();

    // Access to the room that is adjacent to the room containing the portal.
    void SetAdjoiner(NiAVObject* pkAdjoiner);
    NiAVObject* GetAdjoiner() const;

    // If a portal is active, the room graph traversal is allowed to
    // propagate through the portal.  For example, this is useful to disable
    // traversal through a closed door that represents the portal.
    void SetActive(bool bActive);
    bool GetActive() const;

    // Access to the portal polygon.
    unsigned short GetVertexCount() const;
    NiPoint3* GetVertices();
    const NiPoint3* GetVertices() const;

    // Apply a transformation to the portal polygon, but bake it into the
    // model vertices.
    virtual void ApplyTransform(const NiMatrix3& kMat, const NiPoint3& kTrn,
        bool bOnLeft);

    // Access to the model bounding sphere of the portal polygon.
    void SetModelBound(const NiBound& kBound);
    const NiBound& GetModelBound() const;
    NiBound& GetModelBound();
    
    // culling pass (separate from display pass)
    virtual void OnVisible(NiCullingProcess& kCuller);

protected:
    NiPortal();

    // updates
    virtual void UpdateWorldData(); 
    virtual void UpdateWorldBound();

    // Associated with Display().
    bool ComputePortalFrustum(NiCamera& kCamera, NiFrustum& kFrustum);

    // Associated with OnVisible().
    bool UpdateFrustum(NiCullingProcess& kCuller);

    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 8
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        // switch to turn portal on/off
        ACTIVE_MASK = 0x0001,

        // flag to indicate if portal is part of current visitation path
        MARKED_MASK = 0x0002
    };

    bool GetMarked() const;
    void SetMarked(bool bMarked);

    // model points
    unsigned short m_usVertices;
    NiPoint3* m_pkModelVertex;

    // portal adjoiner _not_ refcounted (to avoid refcount cycles)
    NiAVObject* m_pkAdjoiner;

    NiBound m_kModelBound;

    NiPoint3* m_pkWorldVertex;
};

NiSmartPointer(NiPortal);
typedef NiTPointerList<NiPortalPtr> NiPortalList;
#include "NiPortal.inl"

#endif

