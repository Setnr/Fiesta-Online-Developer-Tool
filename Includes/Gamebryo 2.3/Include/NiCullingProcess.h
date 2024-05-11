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

#ifndef NICULLINGPROCESS_H
#define NICULLINGPROCESS_H

#include "NiFrustumPlanes.h"
#include "NiVisibleArray.h"

class NiAVObject;
class NiCamera;
class NiGeometry;

class NIMAIN_ENTRY NiCullingProcess : public NiRefObject
{
public:
    // The input NiVisibleArray provides storage for the potentially visible 
    // set of objects.  This array may be shared by other NiCullingProcess 
    // objects.
    //
    // Because the array may be shared, the application is responsible for
    // initially clearing the array before starting any culling processes
    // that share it.
    //
    // If the input array is deleted but the NiCullingProcess object using
    // it persists, the application is responsible for not using the
    // NiCullingProcess object.  To be safe in this situation, use the
    // SetVisibleSet function to reset the internal visible set pointer to
    // NULL.
    //
    // The input array can be NULL, in which case the storage for the
    // potentially visible set must be provided via the Process function
    // described later in this class.
    NiCullingProcess(NiVisibleArray* pkVisibleSet);
    virtual ~NiCullingProcess();

    // Construct the potentially visible set of objects in the scene.
    //
    // The input array provides storage for the potentially visible set of
    // objects.  This array may be shared by other NiCullingProcess objects.
    // Because the array may be shared, the application is responsible for
    // initially clearing the array before starting any culling processes
    // that share it.
    //
    // If the NiCullingProcess constructor was passed a nonnull visible set
    // pointer and you want Process to use that set, pass null to the
    // visible set pointer of Process.  If you pass a nonnull visible set
    // pointer to Process, the function will use that set instead of the
    // one passed to the NiCullingProcess constructor.  If the
    // NiCullingProcess constructor is passed a null visible set pointer
    // and if Process is passes a null visible set pointer, Process does
    // nothing.
    //
    // This function is considered "atomic" in the sense that (1) the input
    // camera and scene should not be modified and (2) the input array must
    // exist during the lifetime of the function call.
    virtual void Process(const NiCamera* pkCamera, NiAVObject* pkScene,
        NiVisibleArray* pkVisibleSet);

    // Access to the visible set, camera, and frustum currently in use by
    // Process.  The frustum is copied by Process so that derived classes may
    // overwrite it as needed without changing the camera's copy.
    void SetVisibleSet(NiVisibleArray* pkVisibleSet);
    NiVisibleArray* GetVisibleSet();
    const NiCamera* GetCamera() const;
    const NiFrustum& GetFrustum() const;
    void SetFrustum(const NiFrustum& kFrustum);
    const NiFrustumPlanes& GetFrustumPlanes() const;

protected:
    // This is designed to allow only subclasses to change the value of
    // m_bUseVirtualAppend (see discussion of Append and AppendVirtual below)
    NiCullingProcess(NiVisibleArray* pkVisibleSet, bool bUseVirtualAppend);

    // Append is called each time a potentially visible geometry object is
    // encountered during the Process call.  The base version simply adds
    // the object to the visible array, so it is made inline.  This is the
    // most common case for subclasses, as well.  However, it is not the only
    // case, so we must allow for virtual behavior; for example, the portal
    // system maintains a set of unique potentially visible objects, so its
    // Append must detect if the input object is already in the potentially
    // visible set.
    // As a result, we use a "mixture" of virtual and nonvirtual behavior.
    // This base class declares and defines an inline function that handles
    // most cases directly - inside of this nonvirtual function, we test a
    // boolean flag - if this flag is set "true", then we call the virtual
    // member AppendVirtual instead of the code in the inline function.
    // Thus, we gain the performance benefit of an inline function for most
    // culling process classes, but allow for virtual overrides as required
    // by subclasses.
    // NiGeometry::OnVisible is the only class needing to call this function
    // publicly outside of the NiCullingProcess class hierarchy.
    friend class NiGeometry;
    inline  void Append(NiGeometry& kVisible);

    // If this is overridden in a subclass, the subclass version _must_ not
    // call the base class Append, or else a recursive loop will result
    virtual void AppendVirtual(NiGeometry& kVisible) { /* */ }

    // This function is called only by NiAVObject::Cull during a recursive
    // traversal started by Process(const NiCamera*, NiAVObject*,
    // NiVisibleArray*).
    friend class NiAVObject;
    virtual void Process(NiAVObject* pkObject);

    // The bool that declares a given class's need to use the virtual version
    // of Append, AppendVirtual.  It is declared const so that it can only be
    // set at construction time.
    const bool m_bUseVirtualAppend;

    // The potentially visible objects in the input scene to Process.
    NiVisibleArray* m_pkVisibleSet;

    // The camera and frustum currently in use by Process.
    const NiCamera* m_pkCamera;
    NiFrustum m_kFrustum;

    NiFrustumPlanes m_kPlanes;
};

NiSmartPointer(NiCullingProcess);

#include "NiCullingProcess.inl"

#endif
