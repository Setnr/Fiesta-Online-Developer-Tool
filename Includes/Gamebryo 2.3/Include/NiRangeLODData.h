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

#ifndef NIRANGELODDATA_H
#define NIRANGELODDATA_H

#include "NiLODData.h"
#include "NiPoint3.h"

class NIMAIN_ENTRY NiRangeLODData : public NiLODData
{
    NiDeclareRTTI;
    NiDeclareClone(NiRangeLODData);
    NiDeclareStream;
    NiDeclareViewerStrings;

public:
    NiRangeLODData();
    virtual ~NiRangeLODData();

    // Determine the LOD Level do be used based on this camera and Node.
    // NiRangeLODData will use the range from m_kWorldCenter to the Camera.
    virtual int GetLODLevel(const NiCamera* pkCamera, NiLODNode* pkLOD) const;

    // Update any internal world data base on this LOD node.  NiRangeLODData
    // will update the m_kWorldCenter.
    virtual void UpdateWorldData(NiLODNode* pkLOD);

    // NiLODData are never shared so we need a duplication function for
    // cloning.
    virtual NiLODData* Duplicate();

    void SetCenter(const NiPoint3& kCenter);
    const NiPoint3& GetCenter() const;
    const NiPoint3& GetWorldCenter() const;

    unsigned int GetNumRanges() const;
    void SetNumRanges(unsigned int uiNumRanges);

    void GetRange(unsigned int uiIndex, float& fNear, float& fFar) const;
    void SetRange(unsigned int uiIndex, float fNear, float fFar);

    void GetRangeSqr(unsigned int uiIndex, float& fNearSqr, float& fFarSqr) 
        const;

    virtual int GetLODIndex(int iLODLevel) const;
protected:

    class NIMAIN_ENTRY Range : public NiMemObject
    {
    public:
        float m_fNear;
        float m_fFar;
        float m_fWorldNear;  // derived from model near
        float m_fWorldFar;   // derived from model far
    };


    // Center point for all LOD's which is use for the range calculation
    NiPoint3 m_kCenter;
    NiPoint3 m_kWorldCenter;

    // Ranges use to choose a LOD
    unsigned int m_uiNumRanges;
    Range* m_pkRanges;
};


typedef NiPointer<NiRangeLODData> NiRangeLODDataPtr;

//---------------------------------------------------------------------------
//  Inline include
#include "NiRangeLODData.inl"

//---------------------------------------------------------------------------

#endif  // NIRANGELODDATA_H

