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

#ifndef NICURSORRENDERCLICK_H
#define NICURSORRENDERCLICK_H

#include "NiCursorLibType.h"
#include <NiRenderClick.h>
#include "NiCursor.h"

class NICURSOR_ENTRY NiCursorRenderClick : public NiRenderClick
{
    NiDeclareRTTI;

public:
    NiCursorRenderClick();

    // Functions for reporting statistics about the most recent frame.
    virtual unsigned int GetNumObjectsDrawn() const;
    virtual float GetCullTime() const;
    virtual float GetRenderTime() const;

    // Functions for accessing NiCursor objects.
    void AppendCursor(NiCursor* pkCursor);
    void PrependCursor(NiCursor* pkCursor);
    void RemoveCursor(NiCursor* pkCursor);
    void RemoveAllCursors();
    NiTPointerList<NiCursorPtr>& GetCursors();
    const NiTPointerList<NiCursorPtr>& GetCursors() const;

protected:
    // Function for drawing NiCursor objects.
    virtual void PerformRendering(unsigned int uiFrameID);

    // List of NiCursor objects.
    NiTPointerList<NiCursorPtr> m_kCursors;

    // Rendering statistics.
    unsigned int m_uiNumObjectsDrawn;
    float m_fRenderTime;
};

NiSmartPointer(NiCursorRenderClick);

#include "NiCursorRenderClick.inl"

#endif  // #ifndef NICURSORRENDERCLICK_H
