#include "PgUtil.h"
#include <EasyBMP/EasyBMP.h>
#include <Logger.h>
#include <NiCustom/NiPickable.h>
#include <NiGeometry.h>

std::string PgUtil::ClientFolderPath = "";
std::string PgUtil::ApplicationPath = "";

std::string PgUtil::PathFromApplicationFolder(std::string File) 
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
std::string PgUtil::PathFromClientFolder(std::string File) 
{
    if (!File.empty())
    {
        if (File.at(0) == '.')
        {
            if (File.at(1) == '\\')
                return ClientFolderPath + File.substr(1);
            else
                return ClientFolderPath + "\\" + File.substr(1);
        }
        else
            return ClientFolderPath + "\\" + File;
    }
    return ClientFolderPath;
}
char PgUtil::CatchCamera(NiAVObject* pkObject, NiCameraPtr* pkCamera)
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
std::string PgUtil::GetMapFolderPath(char KingdomMap, std::string MapName)
{
    std::string BasePath = ".\\resmap\\";
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
    BasePath += "\\" + MapName + "\\";
    return BasePath;
}
void PgUtil::SaveTexture(std::string Path, NiSourceTexturePtr Texture)
{
    try {
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
    catch (std::exception e)
    {
        LogError("Failed to safe: " + Path);
        LogError("Error-Msg: " + e.what());
    }
    catch (...)
    {
        LogError("Failed to safe: " + Path);
    }
}
bool PgUtil::LoadNodeNifFile(const char* File, NiNodePtr* spNode, NiTexturePalette* /*Currently Unused */)
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
NiNodePtr PgUtil::LoadNifFile(const char* File, NiTexturePalette*/*Currently Unused */, bool IsPickable)
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
void PgUtil::LookAndMoveAtWorldPoint(NiCameraPtr Camera, NiPoint3 Point)
{
    Camera->SetTranslate(Point + NiPoint3(250.f, 250.f, 250.f));
    Camera->Update(0.0f);
    Camera->LookAtWorldPoint(Point, NiPoint3::UNIT_Z);
    Camera->LookAtWorldPoint(Point, NiPoint3::UNIT_Z);
    
    Camera->Update(0.0f);
}