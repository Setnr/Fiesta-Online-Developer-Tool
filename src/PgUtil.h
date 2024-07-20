#pragma once
#include <NiNode.h>
#include <NiCamera.h>
#include <NiSourceTexture.h>
#include <iostream>
#include <NiGeometry.h>

#include "NiPickable.h"
#include <NiPortal.h>
#include <NiRoom.h>
#include "Data/Version.h"
#include <windows.h>
#include <wininet.h>
#include <EasyBMP/EasyBMP.h>
#include <NiTextureTransform.h>
#define PICKABLEOBJECTS 1

class PgUtil 
{
public:
    static void CreateScreenGeometryDataFromTriShapeData(NiGeometryData* pkTriShapeData, int* iLeft, int* iTop, unsigned int* uiWidth, unsigned int* uiHeight)
    {
        if (pkTriShapeData->GetVertexCount() <= 0)
        {
            *iLeft = 0;
            *iTop = 0;
            *uiWidth = 0;
            *uiHeight = 0;
        }
        NiPoint3* Point = pkTriShapeData->GetVertices();
        int Left = Point->x;
        int Top = Point->y;
        int Bottom = Point->y;
        int Right = Point->x;
        for (int i = 0; i < pkTriShapeData->GetVertexCount(); i++) 
        {
            if (Point[i].x < Left)
                Left = Point[i].x;
            if (Point[i].x > Right)
                Right = Point[i].x;

            if (Point[i].y < Top)
                Top = Point[i].y;
            if (Point[i].y > Bottom)
                Bottom = Point[i].y;
        }

        *iLeft = Left;
        *iTop = Top;
        *uiWidth = Right - Left;
        *uiHeight = Bottom - Top;
    }
    static void NowLoadingText(int LoadingRate) 
    {
        /*
        * ToDo:
        * Copy and fix Code from World::NowLoadingText
        */
    }
	static char CatchCamera(NiAVObject* pkObject, NiCameraPtr* pkCamera) 
	{
        if (!pkObject)
            return 0;
        if (pkObject->IsKindOf(&NiCamera::ms_RTTI))
        {
            *pkCamera = (NiCamera*)pkObject;
            return 1;
        }
        if (pkObject->IsKindOf(&NiNode::ms_RTTI))
        {
            NiNode* ConvertedObj = (NiNode*)pkObject;
            for (int i = 0; i < ConvertedObj->GetChildCount(); i++)
            {
                NiAVObject* obj = ConvertedObj->GetAt(i);

                if (CatchCamera(obj, pkCamera))
                    return 1;
            }
        }
        return 0;
	}
    static NiNodePtr LoadNifFile(const char* File, NiTexturePalette*  = NULL/*Currently Unused */, bool IsPickable = false) 
    {
        NiNode* Node;
        if (IsPickable)
            Node = NiNew NiPickable;
        else
            Node = NiNew NiNode;
        NiStream kStream;
        bool bLoaded = kStream.Load(File);
        NIASSERT(bLoaded);
        for (unsigned int ct = 0; ct < kStream.GetObjectCount(); ct++)
        {
            NiObject* obj = kStream.GetObjectAt(ct);
            if (obj->IsKindOf(&NiNode::ms_RTTI) || obj->IsKindOf(&NiGeometry::ms_RTTI))
                Node->AttachChild((NiAVObject*)obj, 0);
        }
         
        return Node;
    }
    static bool LoadNodeNifFile(const char* File, NiNodePtr* spNode,  NiTexturePalette* /*Currently Unused */)
    {
        NiStream kStream;
        bool bLoaded = kStream.Load(File);
        NIASSERT(bLoaded)
        if (kStream.GetObjectCount() != 1) 
        {
            return NULL;
        }
        NiObject* obj = kStream.GetObjectAt(0);
        if (obj->IsKindOf(&NiNode::ms_RTTI))
            *spNode = (NiNode*)obj;
    
        return true;

    }
    static bool LoadTerrainNif(const char* File, NiNodePtr* spNode, NiTexturePalette* /*Currently Unused */)
    {
        bool oldFlag = NiSourceTexture::GetDestroyAppDataFlag();
        NiSourceTexture::SetDestroyAppDataFlag(false);
        NiStream kStream;
        bool bLoaded = kStream.Load(File);
        NiSourceTexture::SetDestroyAppDataFlag(oldFlag);
        NIASSERT(bLoaded)
        if (kStream.GetObjectCount() != 1)
        {
            std::cout << "k Stream has to much root objects" << std::endl;
            return NULL;
        }
        NiObject* obj = kStream.GetObjectAt(0);
        if (obj->IsKindOf(&NiNode::ms_RTTI))
            *spNode = (NiNode*)obj;
        return true;
    }

    static std::string CreateFullFilePathFromBaseFolder(std::string File)
    {
        if (!File.empty()) 
        {
            if (File.at(0) == '.')
                return FolderPath + File.substr(1);
            else
                return FolderPath + "\\" + File;
        }
        return FolderPath;
    }
    static std::string FolderPath;
    static std::string CreateMapFolderPath(char KingdomMap, std::string MapName, std::string FileType) 
    {
        std::string BasePath = CreateFullFilePathFromBaseFolder("") + "\\resmap\\";
        switch (KingdomMap)
        {
        case 0:
        case 4:
        case 6:
        case 7:
            BasePath += "field";
            break;
        case 1:
            BasePath += "KDfield";
            break;
        case 2:
            BasePath += "MHfield";
            break;
        case 3:
            BasePath += "IDfield";
            break;
        }
        BasePath += "\\" + MapName + "\\" + MapName + "." + FileType;
        return BasePath;
    }
    static void LoadingScreen(NiRenderer* Renderer, std::string LoadingScreen, float Percent, bool Map);
    static NiScreenElements* CreateProgressbar(bool Map, float Percent);

    static std::string CreateFullFilePathFromApplicationFolder(std::string File)
    {
        if (!File.empty())
        {
            if (File.at(0) == '.')
                return ApplicationPath + File.substr(1);
            else
                return ApplicationPath + "\\" + File;
        }
        return ApplicationPath;
    }
    static std::string ApplicationPath;

    static Version::Status CheckVersion() 
    {
        
        // ------------------------------------------------------------------------
   
        // ------------------------------------------------------------------------
        
        Version Cur(Version::CurVersion);
        DWORD dwFlags;
        if (!InternetGetConnectedState(&dwFlags, 0))
            return Version::Status::CantCheck;
           

        Version latest(GetLatestVersion());
        if (Cur < latest)
            return Version::Status::New;
        return Version::Status::OK;
    }
    static void SaveTexture(std::string Path, NiSourceTexturePtr Texture) 
    {
        BMP bmp;
        bmp.SetSize(Texture->GetWidth(), Texture->GetHeight());
        RGBApixel* PixelData = (RGBApixel*)Texture->GetSourcePixelData()->GetPixels();
        for (int w = 0; w < Texture->GetWidth(); w++) 
        {
            for (int h = Texture->GetHeight() - 1; h >= 0; h--)
            {
                int XPart = w;

                int PreFullLines = Texture->GetWidth() * h;
                int YPartNormal = PreFullLines;
                int PointOffsetNormal = XPart + YPartNormal;
                bmp.SetPixel(w, Texture->GetHeight() - h - 1, PixelData[PointOffsetNormal]);
            }
        }
        bmp.WriteToFile(Path.c_str());
    }

    static NiScreenElements* CreateScreenElement(float width, float height, NiSourceTexturePtr texture);

    static void SaveNode(std::string Path, NiObject* Node) 
    {
        NiStream s;
        s.InsertObject(Node);
        s.Save(Path.c_str());
    }
    /**
      *   \brief Chceks if a Element is hovered
      *
      *   \param pElement -> Pointer to the NiScreenElement to check
      *   \param x -> FloatValue of relativ X - Position above Element
      *   \param y -> FloatValue of relativ Y - Position above Element
      *   \return true if Element is hovered / false if Element is not Hovered
      *
      **/
    static bool HoveringScreenElement(NiScreenElements* pElement, float& x, float& y);

     /**
      *   \brief Calculates x and y based on TextureTransform on NiScreenElement
      *     be Careful with rotation
      *     if(PgUtil::HoveringScreenElement(pScreenElementTextureEdit, x, y)))
      *          ZoomAndPanScreenElementCalculation(x,y,pkTexutre,pkTransform);
      *
      *   \param x & y -> Floats 0.f - 1.f matching Position over ScreenElement will be adjusted so x and y matches to point within texture
      *   \param Texture The used Texture for the ScreenElement
      *   \param TextureTransForm The Transform of the Texture
      *   \return std::pair<int,int> with x and y Pixel 
      *
      **/
    static std::pair<int,int> ZoomAndPanScreenElementCalculation(float& x, float& y, NiSourceTexturePtr Texture, NiTextureTransform* TextureTransForm) 
    {
        //
        auto data = Texture->GetSourcePixelData();
        NiPoint2 Scale = TextureTransForm->GetScale();
        auto Translate = TextureTransForm->GetTranslate();
        float MaxValue = Scale.x;

        y = y * Scale.x + (1.f - Scale.x) - Translate.x;
        x = x * Scale.x + Translate.y;
        int yPixel = data->GetHeight() - (data->GetHeight() * y);  //Currently no clue why x and y are wrong order
        int xPixel = (data->GetWidth() * x);
        return { xPixel,yPixel };
    }
};