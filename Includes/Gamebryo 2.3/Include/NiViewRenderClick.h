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

#ifndef NIVIEWRENDERCLICK_H
#define NIVIEWRENDERCLICK_H

#include "NiRenderClick.h"
#include "NiRenderView.h"
#include "NiRenderListProcessor.h"

class NIMAIN_ENTRY NiViewRenderClick : public NiRenderClick
{
    NiDeclareRTTI;

public:
    NiViewRenderClick();

    // Functions for reporting statistics about the most recent frame.
    virtual unsigned int GetNumObjectsDrawn() const;
    virtual float GetCullTime() const;
    virtual float GetRenderTime() const;

    // Functions for accessing the list of render views.
    void AppendRenderView(NiRenderView* pkRenderView);
    void PrependRenderView(NiRenderView* pkRenderView);
    void RemoveRenderView(NiRenderView* pkRenderView);
    void RemoveAllRenderViews();
    NiTPointerList<NiRenderViewPtr>& GetRenderViews();
    const NiTPointerList<NiRenderViewPtr>& GetRenderViews() const;
    NiRenderView* GetRenderViewByName(const NiFixedString& kName) const;
    NiTListIterator GetRenderViewPosByName(const NiFixedString& kName) const;
    NiRenderView* GetRenderViewAndPosByName(const NiFixedString& kName,
        NiTListIterator& kIter) const;

    // Functions for accessing the render list processor.
    void SetProcessor(NiRenderListProcessor* pkProcessor,
        void* pvProcessorData = NULL);
    NiRenderListProcessor* GetProcessor() const;
    void* GetProcessorData() const;

    // *** begin Emergent internal use only ***
    static void _SDMInit();
    static void _SDMShutdown();
    // *** end Emergent internal use only ***

protected:
    // Function that performs the rendering for the render click.
    virtual void PerformRendering(unsigned int uiFrameID);

    // Render view object to use when rendering.
    NiTPointerList<NiRenderViewPtr> m_kRenderViews;

    // Render list processor to use when rendering.
    NiRenderListProcessorPtr m_spProcessor;
    void* m_pvProcessorData;

    // Array of processed geometry that is reused from frame to frame.
    NiVisibleArray m_kProcessedGeometry;

    // Rendering statistics.
    unsigned int m_uiNumObjectsDrawn;
    float m_fCullTime;
    float m_fRenderTime;

    // Default render list processor to use if none is specified.
    static NiRenderListProcessorPtr ms_spDefaultProcessor;
};

NiSmartPointer(NiViewRenderClick);

#include "NiViewRenderClick.inl"

#endif  // #ifndef NIVIEWRENDERCLICK_H
