#pragma once
#include <NiNode.h>
#include <NiCamera.h>
class PgUitl 
{
public:
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
        sizeof(NiNode);
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