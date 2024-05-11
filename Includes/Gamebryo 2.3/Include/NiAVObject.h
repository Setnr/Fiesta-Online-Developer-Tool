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

#ifndef NIAVOBJECT_H
#define NIAVOBJECT_H

#include "NiBound.h"
#include "NiObjectNET.h"
#include "NiPlane.h"
#include "NiPoint3.h"
#include "NiProperty.h"
#include "NiFlags.h"
#include "NiQuaternion.h"
#include "NiTransform.h"
#include "NiCollisionObject.h"

class NiBitVector;
class NiCullingProcess;
class NiNode;
class NiPick;
class NiRenderer;
NiSmartPointer(NiAVObject);
NiSmartPointer(NiPropertyState);
NiSmartPointer(NiDynamicEffectState);

class NIMAIN_ENTRY NiAVObject : public NiObjectNET
{
    NiDeclareRTTI;
    NiDeclareAbstractClone(NiAVObject);
    NiDeclareAbstractStream;
    NiDeclareViewerStrings;
public:
    // destruction(abstract base class, constructor is protected)
    virtual ~NiAVObject();

    // parent access
    NiNode* GetParent();
    const NiNode* GetParent() const;

    // updates
    void Update(float fTime, bool bUpdateControllers = true);
    void UpdateSelected(float fTime);
    virtual void UpdateControllers(float fTime);
    void UpdateObjectControllers(float fTime, bool bProperties = true);
    void UpdateProperties();
    void UpdateEffects();
    virtual void UpdateNodeBound();

    // transforms
    void SetTranslate(const NiPoint3& kTrn);
    void SetTranslate(float x, float y, float z);
    const NiPoint3& GetTranslate() const;
    void SetRotate(const NiMatrix3& kRot);
    const NiMatrix3& GetRotate() const;
    void SetRotate(float fAngle, float x, float y, float z);
    void GetRotate(float& fAngle, float& x, float& y, float& z) const;
    void SetRotate(const NiQuaternion& kQuat);
    void GetRotate(NiQuaternion& kQuat) const;
    void SetRotateAboutCenter(const NiMatrix3& kRot, const NiPoint3& kCen,
        const NiPoint3& kTrans = NiPoint3::ZERO);
    void SetScale(float fScale);
    float GetScale() const;
    virtual void ApplyTransform(const NiMatrix3& kMat, const NiPoint3& kTrn,
        bool bOnLeft);
    const NiPoint3& GetWorldTranslate() const;
    const NiMatrix3& GetWorldRotate() const;
    float GetWorldScale() const;

    // Set local transform of object
    void SetLocalTransform(const NiTransform& kTransform);

    // Sets the local transform such that it's world position will match
    // the provided world transform. 
    // NOTE: Parent must have been updated to have correct world location.
    // NOTE: This object must have update called to reflect this transform.
    void SetLocalFromWorldTransform(const NiTransform& kWorldTransform);


    // bounds
    const NiBound& GetWorldBound() const;

    // properties
    NiPropertyList& GetPropertyList();
    const NiPropertyList& GetPropertyList() const;
    void AttachProperty(NiProperty* pkProperty);
    void DetachProperty(NiProperty* pkProperty);
    void DetachAllProperties();
    NiProperty* GetProperty(int iType) const;
    NiPropertyPtr RemoveProperty(int iType);

    // material updates
    virtual void SetMaterialNeedsUpdate(bool bNeedsUpdate);
    virtual void SetDefaultMaterialNeedsUpdateFlag(bool bFlag);

    // application-specified culling
    void SetAppCulled(bool bAppCulled);
    bool GetAppCulled() const;
    // name search
    virtual NiAVObject* GetObjectByName(const NiFixedString& kName);

    // application-specified selective update
    bool GetSelectiveUpdate() const;
    void SetSelectiveUpdate(bool bUpdate);
    bool GetSelectiveUpdateTransforms() const;
    void SetSelectiveUpdateTransforms(bool bUpdate);
    bool GetSelectiveUpdatePropertyControllers() const;
    void SetSelectiveUpdatePropertyControllers(bool bUpdate);
    bool GetSelectiveUpdateRigid() const;
    void SetSelectiveUpdateRigid(bool bRigid);
    bool GetSelectiveUpdateTransformsOverride() const;
    void SetSelectiveUpdateTransformsOverride(bool bOverride);
    virtual void SetSelectiveUpdateFlags(bool& bSelectiveUpdate, 
        bool bSelectiveUpdateTransforms, bool& bRigid);

    // for occlusion culling
    void SetDisplayObject(bool bDisplay);
    bool GetDisplayObject() const;

    void SetCollisionObject(NiCollisionObject* pkCollisionObject);
    NiCollisionObject* GetCollisionObject() const;

    bool IsNode();
    bool IsLeaf();

    // *** begin Emergent internal use only ***

    // Updates intended only for use with 3rd party physics engine.
    void SetWorldTranslate(const NiPoint3& kTrn);
    void SetWorldRotate(const NiMatrix3& kRot);
    void SetWorldScale(float fScale);
    void SetWorldBound(const NiBound& kBound);

    // updates
    virtual void UpdateDownwardPass(float fTime, bool bUpdateControllers);
    virtual void UpdateSelectedDownwardPass(float fTime);
    virtual void UpdateRigidDownwardPass(float fTime);
    bool IsVisualObject() const;

    virtual void UpdatePropertiesDownward(NiPropertyState* pkParentState);
    virtual void UpdateEffectsDownward(NiDynamicEffectState* pkParentState);

    virtual void UpdateWorldData();
    virtual void UpdateWorldBound();

    // property updates
    NiPropertyStatePtr PushLocalProperties(NiPropertyState* pkParentState,
        bool bCopyOnChange);

    // parent manipulation(applications should never explicity call these)
    void AttachParent(NiNode* pkParent);
    void DetachParent();

    // culling pass (separate from display pass)
    void Cull(NiCullingProcess& kCuller);
    virtual void OnVisible(NiCullingProcess& kCuller);

    // application-specified selective update
    void DoSelectedUpdate(float fTime);

    // cloning
    virtual void ProcessClone(NiCloningProcess& kCloning);

    // transforms
    const NiTransform& GetWorldTransform() const;
    const NiTransform& GetLocalTransform() const;

    // renderer data
    virtual void PurgeRendererData(NiRenderer* pkRenderer);

    // *** end Emergent internal use only ***

protected:
    // construction(abstract base class)
    NiAVObject();

    // Declared here as there is already a flag vector available.  However,
    // only subclasses actually make access to this public
    inline void SetSortObject(bool bSort);
    inline bool GetSortObject() const;

    void SetNodeBit();

    // parent pointer
    NiNode* m_pkParent;

    // --- Begin NIF conversion code
    // Deprecated flags - use new flag code
    enum
    {
        MAX_POS = 6
    };
    // --- End NIF conversion code

    // flags
    enum
    {
        APP_CULLED_MASK                 = 0x0001,

        // Selective update flags
        SELECTIVE_UPDATE_MASK           = 0x0002,
        SELECTIVE_XFORMS_MASK           = 0x0004,
        SELECTIVE_PROP_CONTROLLER_MASK  = 0x0008,
        SELECTIVE_RIGID_MASK            = 0x0010,

        // For use with occlusion culling system
        DISPLAY_OBJECT_MASK             = 0x0020,

        // For use with sorting/accumulation system
        DISABLE_SORTING                 = 0x0040,
        
        // Selective update over-ride flags
        SELECTIVE_XFORMS_OVERRIDE_MASK  = 0x0080,

        // Is the object a leaf or a node.
        IS_NODE                         = 0x0100
    };

    // bound
    NiBound m_kWorldBound;

    // transforms
    // NOTE: Variable declarations whose order effects assembly language code 
    // begin here. Do not change.
    void CopyTransforms(const NiAVObject* pkSrc);
    NiTransform m_kLocal;
    NiTransform m_kWorld;
    // NOTE: Variable declarations whose order effects assembly language code
    // end here.
    // properties
    NiPropertyList m_kPropertyList;

    // cloning
    void CloneProperties(
        NiAVObject* pkDest, 
        NiPropertyList* pkList,
        NiCloningProcess& kCloning);
    void ProcessPropertyClones(
        NiPropertyList* pkList,
        NiCloningProcess& kCloning);

    // set selective update flags
    bool HasTransformController() const;
    bool HasPropertyController() const;

    NiCollisionObjectPtr m_spCollisionObject;
    
    NiDeclareFlags(unsigned short);
};

#include "NiAVObject.inl"

#endif

