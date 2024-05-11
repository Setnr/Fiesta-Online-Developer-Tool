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

#ifndef NISCREENFILLINGRENDERVIEW_H
#define NISCREENFILLINGRENDERVIEW_H

#include "NiRenderView.h"
#include "NiScreenElements.h"
#include "NiDynamicEffect.h"
#include "NiCriticalSection.h"

class NIMAIN_ENTRY NiScreenFillingRenderView : public NiRenderView
{
    NiDeclareRTTI;

public:
    NiScreenFillingRenderView();
    virtual ~NiScreenFillingRenderView();

    // Function that sets the screen-space camera data for the renderer.
    virtual void SetCameraData(const NiRect<float>& kViewport);

    // Functions for manipulating the properties on the screen-filling quad.
    void AttachProperty(NiProperty* pkProperty);
    void DetachProperty(NiProperty* pkProperty);
    void DetachAllProperties();
    NiProperty* GetProperty(int iType) const;
    NiPropertyPtr RemoveProperty(int iType);

    // Functions for manipulating dynamic effects for the screen-filling quad.
    void AttachEffect(NiDynamicEffect* pkEffect);
    void DetachEffect(NiDynamicEffect* pkEffect);
    void DetachAllEffects();

    // Access to the underlying screen-filling quad geometry object so that
    // materials and shaders can be attached.
    NiGeometry& GetScreenFillingQuad() const;

protected:
    // Function that adds the screen-filling quad to the PV geometry array.
    virtual void CalculatePVGeometry();

    // The screen-filling quad.
    NiScreenElementsPtr m_spScreenFillingQuad;

    // List of dynamic effects and an effect state to use when rendering the
    // screen-filling quad.
    NiDynamicEffectList m_kEffectList;
    NiDynamicEffectStatePtr m_spEffectState;

    // Boolean variables indicating whether or not UpdateProperties or
    // UpdateEffects must be called before the next render.
    bool m_bPropertiesChanged;
    bool m_bEffectsChanged;

    // Common geometry data object for all instances of this class.
    static NiScreenElementsData* ms_pkScreenFillingQuadData;
    static int ms_iQuadDataRefCount;
    static NiCriticalSection ms_kQuadDataLock;
};

NiSmartPointer(NiScreenFillingRenderView);

#include "NiScreenFillingRenderView.inl"

#endif  // #ifndef NISCREENFILLINGRENDERVIEW_H
