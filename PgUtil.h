#pragma once
#include <NiNode.h>
#include <NiCamera.h>


#define UtilDebugString(pcText, ...)  { char acTemp[512]; NiSprintf(acTemp, sizeof(acTemp), pcText, __VA_ARGS__); NiOutputDebugString(acTemp);}
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
    static NiNodePtr LoadNifFile(const char* File, NiTexturePalette* /*Currently Unused */) 
    {
        NiNode* Node = NiNew NiNode;

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
};