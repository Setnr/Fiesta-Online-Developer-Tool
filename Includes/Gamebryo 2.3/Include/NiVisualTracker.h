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

#ifndef NIVISUALTRACKER_H
#define NIVISUALTRACKER_H

#include <NiVisualTrackerLibType.h>
#include <NiRefObject.h>
#include <NiColor.h>
#include <NiLines.h>
#include <NiPoint2.h>
#include <NiNode.h>
#include <NiScreenTexture.h>
#include <NiPoint3.h>

NiSmartPointer(NiTexture);

class NIVISUALTRACKER_ENTRY NiVisualTracker : public NiRefObject
{
    public:
        NiVisualTracker(float fMaxValue, unsigned int uiNumDecimalPlaces,
            NiRect<float> kWindowRect, const char* pcName, 
            bool bShow = true, 
            unsigned int uiNumGraphs = 5);

        ~NiVisualTracker();

        class NIVISUALTRACKER_ENTRY GraphCallbackObject : public NiMemObject
        {
            public:
                virtual ~GraphCallbackObject() {};
                virtual float TakeSample(float fTime) = 0;
        };

        // Insert a tracker into the list. Returns the tracker index.
        unsigned int AddGraph(GraphCallbackObject* pkObject, 
            const char* pcName, const NiColor& kColor, 
            unsigned int uiNumSamplesToKeep, float fMinSampleTime, bool bShow);
        void RemoveGraph(const char* pcName);
        void RemoveAll();
        
        // Accessors
        const char* GetName();
        unsigned int GetGraphCount();
        void SetName(const char* pcName, unsigned int uiWhichGraph);
        const char* GetName(unsigned int uiWhichGraph);
        unsigned int GetGraphIndexByName(const char* pcName);

        // Toggle the visibility of the graph. This determines if the tracker 
        // is currently visible, but does not affect its sampling.
        void ShowGraph(bool bShow, unsigned int uiWhichGraph);
        bool GetGraphShow(unsigned int uiWhichGraph);
        void SetShow(bool bShow);
        bool GetShow();

        void Update();

        void Draw();

    protected:

        
        class ScreenText : public NiMemObject
        {
        public:
            ScreenText(unsigned int uiMaxChars,
                NiScreenTextureArray* pkScreenTextures, 
                const NiColorA& kColor);
            ~ScreenText();

            // Will truncate the string to the pre-set max number of characters
            void SetString(const char* pcString);
            const char* GetString() const;

            // Works in screen space pixel coords: <0 - (m-1), 0 - (n-1)>,
            // where the screen size is (m,n), to set the position of the 
            // upper-left edge of the text.
            void SetTextOrigin(unsigned int uiX, unsigned int uiY);
            void GetTextOrigin(unsigned int& uiX, unsigned int& uiY) const;
            
            void SetScrollDown(bool bDown);
            bool GetScrollDown() const;

            void SetColor(NiColorA& kNewColor);
            const NiColorA& GetColor() const;

            void SetVisible(bool bVisible);

            short GetHeight() const;
            short GetWidth() const;
            short GetCharacterSpacing() const;

            // Sets the line length for word wrapping  This should always be 
            // less than the text size, or else truncation will occur.
            void SetMaxLineLength(unsigned int uiColumns);
            unsigned int GetMaxLineLength() const;

            // Will not append beyond the pre-set max number of characters.
            void AppendCharacter(char cChar);
            void DeleteLastCharacter();

            void RecreateText();

            static NiTexture* GetTexture();

        protected:
            void Init(unsigned int uiMaxChars, 
                NiScreenTextureArray* pkScreenTextures, 
                const NiColorA& kColor);

            NiScreenTexturePtr m_spScreenTexture;
            NiScreenTextureArray* m_pkScreenTextures;
            NiColorA m_kColor;

            unsigned int m_uiTextOriginX;
            unsigned int m_uiTextOriginY;
            
            char* m_pcString;
            unsigned int m_uiMaxChars;
            unsigned int m_uiNumChars;
            unsigned int m_uiMaxCols;

            unsigned int m_uiNumRects;
            unsigned int m_uiNumCurrentRows;
            unsigned int m_uiCurrentColumn;

            bool m_bScrollDown;

            static unsigned int ms_uiCharWidth;
            static unsigned int ms_uiCharHeight;
            static unsigned int ms_uiCharSpacingX;
            static unsigned int ms_uiCharSpacingY;
            static const unsigned int ms_uiCharBaseU;
            static const unsigned int ms_uiCharBaseV;
            static NiTexturePtr ms_spTextTexture;
            static const char* ms_pcTextImage;
            static const unsigned int ms_uiASCIIMin;
            static const unsigned int ms_uiASCIIMax;
            static unsigned int ms_uiASCIICols;
            static int ms_iCount;
        };

        class GraphCallbackObjectData : public NiMemObject
        {
            public:
                GraphCallbackObjectData(GraphCallbackObject* pkObject, 
                    const char* pcName, const NiColor& kColor, 
                    unsigned int uiNumSamplesToKeep, float fMaxValue, 
                    float fSamplingTime, bool bShow, 
                    const NiRect<float>& kDimensions, NiNode* pkParentNode,
                    unsigned int& uiLegendX, unsigned int& uiLegendY,
                    NiScreenTextureArray& kTextures);

                ~GraphCallbackObjectData();

                void Update(float fTime);
                void Draw();

                bool GetShow();
                void SetShow(bool bShow);
                static NiLines* CreateLines(unsigned int uiNumVertices,
                    const NiColor& kColor, const NiRect<float>& kDimensions);

                const char* GetName();
                void SetName(const char* pcName);

                void AdjustLegend(int iX, int iY);
                unsigned int GetLegendWidth();

            protected:  
                GraphCallbackObject* m_pkCallbackObj;
                bool m_bShow;
                unsigned int m_uiNumSamplesToKeep;
                float m_fSamplingTime;
                NiLinesPtr m_spLines;
                NiColor m_kColor;
                float m_fLastTime;
                char m_acName[256];
                float m_fMaxValue;
                unsigned int m_uiNextVertex;
                NiRect<float> m_kDimensions;
                ScreenText* m_pkText;
        };

        NiTPrimitiveArray<GraphCallbackObjectData*> m_kCallbackData;
        NiRect<float> m_kDimensions;  
        NiRect<float> m_kGraphLineDimensions;
        NiNodePtr m_spWindowRoot;
        NiLinesPtr m_spBorders;
        float m_fMaxValue;
        bool m_bShow;
        char m_acName[256];
        ScreenText* m_pkTextTitle;
        ScreenText* m_pkTextZero;
        ScreenText* m_pkTextTwentyFive;
        ScreenText* m_pkTextFifty;
        ScreenText* m_pkTextSeventyFive;
        ScreenText* m_pkTextOneHundred;
        NiScreenTextureArray m_kScreenTextures;

        unsigned int m_uiLegendX;
        unsigned int m_uiLegendY;
};

NiSmartPointer(NiVisualTracker);

#include "NiVisualTracker.inl"

#endif
