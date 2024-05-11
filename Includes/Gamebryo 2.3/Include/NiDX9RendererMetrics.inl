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

// Uncomment this line for debug output.
// #define NIDX9RENDERMETRICSDEBUGOUTPUT

//---------------------------------------------------------------------------
inline void NiDx9RendererMetrics::RecordMetrics()
{
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_PRIMITIVE, 
        ms_aiPerFrameMetrics[DRAW_PRIMITIVE]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TRIS, 
        ms_aiPerFrameMetrics[DRAW_TRIS]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_VERTS, 
        ms_aiPerFrameMetrics[DRAW_VERTS]);
    NIMETRICS_DX9RENDERER_ADDVALUE(RENDER_STATE_CHANGES, 
        ms_aiPerFrameMetrics[RENDER_STATE_CHANGES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(RENDER_TARGET_CHANGES, 
        ms_aiPerFrameMetrics[RENDER_TARGET_CHANGES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(VERTEX_BUFFER_CHANGES, 
        ms_aiPerFrameMetrics[VERTEX_BUFFER_CHANGES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(PIXEL_SHADER_CHANGES, 
        ms_aiPerFrameMetrics[PIXEL_SHADER_CHANGES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(VERTEX_SHADER_CHANGES, 
        ms_aiPerFrameMetrics[VERTEX_SHADER_CHANGES]);

    NIMETRICS_DX9RENDERER_ADDVALUE(VERTEX_BUFFER_SIZE, 
        ms_aiPerFrameMetrics[VERTEX_BUFFER_SIZE]);
    NIMETRICS_DX9RENDERER_ADDVALUE(INDEX_BUFFER_SIZE, 
        ms_aiPerFrameMetrics[INDEX_BUFFER_SIZE]);

    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TIME_LINES, 
        ms_afPerFrameTimes[DRAW_TIME_LINES - DRAW_TIME_LINES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TIME_POINTS, 
        ms_afPerFrameTimes[DRAW_TIME_POINTS - DRAW_TIME_LINES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TIME_SHAPE, 
        ms_afPerFrameTimes[DRAW_TIME_SHAPE - DRAW_TIME_LINES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TIME_STRIPS, 
        ms_afPerFrameTimes[DRAW_TIME_STRIPS - DRAW_TIME_LINES]);
    NIMETRICS_DX9RENDERER_ADDVALUE(DRAW_TIME_SCREENTEXTURE, 
        ms_afPerFrameTimes[DRAW_TIME_SCREENTEXTURE - DRAW_TIME_LINES]);

    ms_aiPerFrameMetrics[DRAW_PRIMITIVE] = 0;
    ms_aiPerFrameMetrics[DRAW_TRIS] = 0;
    ms_aiPerFrameMetrics[DRAW_VERTS] = 0;
    ms_aiPerFrameMetrics[RENDER_STATE_CHANGES] = 0;
    ms_aiPerFrameMetrics[RENDER_TARGET_CHANGES] = 0;
    ms_aiPerFrameMetrics[VERTEX_BUFFER_CHANGES] = 0;
    ms_aiPerFrameMetrics[PIXEL_SHADER_CHANGES] = 0;
    ms_aiPerFrameMetrics[VERTEX_SHADER_CHANGES] = 0;
    memset(ms_afPerFrameTimes, 0, sizeof(ms_afPerFrameTimes));
}
//---------------------------------------------------------------------------
inline void NiDx9RendererMetrics::IncrementMetric(
    NiDx9RendererMetrics::NiMetric eMetric, int iInc)
{
    // Don't need any semaphores here because 
    ms_aiPerFrameMetrics[eMetric] += iInc;

#ifdef NIDX9RENDERMETRICSDEBUGOUTPUT
    if (eMetric == VERTEX_BUFFER_SIZE)
    {
        char acBuffer[256];
        NiSprintf(acBuffer, sizeof(acBuffer), 
            "METRICDEBUG: AddToVertexBuffer: %i (%i)\n", iInc, 
            ms_aiPerFrameMetrics[eMetric]);
        NiOutputDebugString(acBuffer);
    }
    else if (eMetric == INDEX_BUFFER_SIZE)
    {
        char acBuffer[256];
        NiSprintf(acBuffer, sizeof(acBuffer), 
            "METRICDEBUG: AddToIndexBuffer: %i (%i)\n", iInc, 
            ms_aiPerFrameMetrics[eMetric]);
        NiOutputDebugString(acBuffer);
    }
#endif // #ifdef NIDX9RENDERMETRICSDEBUGOUTPUT
}
//---------------------------------------------------------------------------
inline void NiDx9RendererMetrics::RecordTime(NiMetric eMetric, float fTime)
{
    ms_afPerFrameTimes[eMetric - DRAW_TIME_LINES] += fTime;
}
//---------------------------------------------------------------------------
inline NiDx9ScopeTimer::NiDx9ScopeTimer(
    NiDx9RendererMetrics::NiMetric eMetric)
{
    m_eMetric = eMetric;
    Start();
}
//---------------------------------------------------------------------------
inline NiDx9ScopeTimer::~NiDx9ScopeTimer()
{
    Stop();
    NiDx9RendererMetrics::RecordTime(m_eMetric, GetElapsed());
}
//---------------------------------------------------------------------------
