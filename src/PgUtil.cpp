#include "PgUtil.h"
#include <EasyBMP/EasyBMP.h>
#include <Logger.h>
#include <NiCustom/NiPickable.h>
#include <NiGeometry.h>
#include <NiTexturingProperty.h>
#include <NiVertexColorProperty.h>
#include <algorithm>

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

void PgUtil::SaveTexture(std::string Path, NiPixelDataPtr PixelData, bool flipped)
{

    try {
        BMP bmp;
        bmp.SetSize(PixelData->GetWidth(), PixelData->GetHeight());
        RGBApixel* PixelDataArray = (RGBApixel*)PixelData->GetPixels();
        for (int w = 0; w < PixelData->GetWidth(); w++)
        {
            for (int h = PixelData->GetHeight() - 1; h >= 0; h--)
            {
                RGBApixel a = *(RGBApixel*)PixelData->operator()(w,h);
                RGBApixel fixed;
                fixed.Red = a.Blue;
                fixed.Green = a.Green;
                fixed.Blue = a.Red;
                if (PixelData->GetPixelFormat().GetBitsPerPixel() == 32)
                    fixed.Alpha = a.Alpha;
                else
                    fixed.Alpha = 0xFF;
                int realh = h;
                if (flipped)
                    realh = PixelData->GetHeight() - h - 1;
                bmp.SetPixel(w, realh , fixed);
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

void PgUtil::SaveTexture(std::string Path, NiSourceTexturePtr Texture, bool flipped)
{
    SaveTexture(Path, Texture->GetSourcePixelData(), flipped);
    return;
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
NiCamera* PgUtil::CreateNewCamera() 
{
    NiCamera* Camera = NiNew NiCamera;
    if (!Camera)
        return NULL;
    NiFrustum WorldFrustum(-0.25, 0.25, 0.1875, -0.1875, 1.0, 6000.0, false);
    //
    float m_fCameraFOV = 50.0f;
    float fTop = m_fCameraFOV / 180.0 * 0.5 * NI_PI;
    float fTopa = tan(fTop);
    float v4 = fTopa;
    float fRight = fTopa;
    float v5 = 1600; /*TODO DYNAMIC*/
    float v6 = 900;
    float fTopb;
    if (900 >= (double)1600)
    {
        fTopb = v4 * (v6 / v5);
        v4 = fTopb;
    }
    else
    {
        fRight = v5 / v6 * v4;
    }
    float fTopc = -fRight;
    float v7 = fTopc;
    WorldFrustum.m_fLeft = fTopc;
    WorldFrustum.m_fRight = fRight;
    WorldFrustum.m_fTop = v4;
    float fTopd = -v4;
    WorldFrustum.m_fBottom = fTopd;
    //


    Camera->SetViewFrustum(WorldFrustum);
    return Camera;
}
NiScreenElements* PgUtil::CreateScreenElement(float width, float height, NiSourceTexturePtr texture)
{
    NiScreenElements* pkScreenTexture = NiNew NiScreenElements(NiNew NiScreenElementsData(false, true, 1));
    pkScreenTexture->Insert(4);
    pkScreenTexture->SetRectangle(0, 0.4f, 0.4f, 0.8f, 0.8f);
    pkScreenTexture->UpdateBound();
    pkScreenTexture->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    pkScreenTexture->SetColors(0, NiColorA::WHITE);
    auto& io = ImGui::GetIO();

    float fLeft = 0.0f;
    float fBottom = 0.0f;
    float fRight = width / io.DisplaySize.x + fLeft;
    float fTop = height / io.DisplaySize.y + fBottom;
    pkScreenTexture->SetVertex(0, 0, NiPoint2(fLeft, 1.0f - fBottom));
    pkScreenTexture->SetVertex(0, 1, NiPoint2(fRight, 1.0f - fBottom));
    pkScreenTexture->SetVertex(0, 2, NiPoint2(fRight, 1.0f - fTop));
    pkScreenTexture->SetVertex(0, 3, NiPoint2(fLeft, 1.0f - fTop));
    pkScreenTexture->UpdateBound();

    NiTexturingPropertyPtr pTexture = NiNew NiTexturingProperty();
    pTexture->SetBaseTexture(texture);
    pTexture->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
    pTexture->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);

    auto transform = NiNew NiTextureTransform();
    //transform->SetScale(NiPoint2(-1.f, 1.f));
    transform->SetRotate(-3.14159 / 2);
    //transform->SetTransformMethod(NiTextureTransform::MAYA_DEPRECATED_TRANSFORM);

    pTexture->SetBaseTextureTransform(transform);
    pkScreenTexture->AttachProperty(pTexture);

    NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
    pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
    pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
    pkScreenTexture->AttachProperty(pVertex);

    NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
    pAlpha->SetAlphaBlending(true);
    pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
    pkScreenTexture->AttachProperty(pAlpha);

    pkScreenTexture->UpdateProperties();
    pkScreenTexture->UpdateEffects();
    pkScreenTexture->Update(0.0f);
    return pkScreenTexture;
}
NiScreenElements* PgUtil::CreateScreenElement(NiSourceTexturePtr texture)
{
    float width = texture->GetWidth();
    float height = texture->GetHeight();
    NiScreenElements* pkScreenTexture = NiNew NiScreenElements(NiNew NiScreenElementsData(false, true, 1));
    pkScreenTexture->Insert(4);
    pkScreenTexture->SetRectangle(0, 0.4f, 0.4f, 0.8f, 0.8f);
    pkScreenTexture->UpdateBound();
    pkScreenTexture->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);
    pkScreenTexture->SetColors(0, NiColorA::WHITE);
    auto& io = ImGui::GetIO();

    float fLeft = 0.0f;
    float fBottom = 0.0f;
    float fRight = width / io.DisplaySize.x + fLeft;
    float fTop = height / io.DisplaySize.y + fBottom;
    pkScreenTexture->SetVertex(0, 0, NiPoint2(fLeft, 1.0f - fBottom));
    pkScreenTexture->SetVertex(0, 1, NiPoint2(fRight, 1.0f - fBottom));
    pkScreenTexture->SetVertex(0, 2, NiPoint2(fRight, 1.0f - fTop));
    pkScreenTexture->SetVertex(0, 3, NiPoint2(fLeft, 1.0f - fTop));
    pkScreenTexture->UpdateBound();

    NiTexturingPropertyPtr pTexture = NiNew NiTexturingProperty();
    pTexture->SetBaseTexture(texture);
    pTexture->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
    pTexture->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);

    auto transform = NiNew NiTextureTransform();
    //transform->SetScale(NiPoint2(-1.f, 1.f));
    transform->SetRotate(-3.14159 / 2);
    //transform->SetTransformMethod(NiTextureTransform::MAYA_DEPRECATED_TRANSFORM);

    pTexture->SetBaseTextureTransform(transform);
    pkScreenTexture->AttachProperty(pTexture);

    NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
    pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
    pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
    pkScreenTexture->AttachProperty(pVertex);

    NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
    pAlpha->SetAlphaBlending(true);
    pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
    pkScreenTexture->AttachProperty(pAlpha);

    pkScreenTexture->UpdateProperties();
    pkScreenTexture->UpdateEffects();
    pkScreenTexture->Update(0.0f);
    return pkScreenTexture;
}
void PgUtil::FixColor(NiColorA& Color)
{
    Color.r = std::clamp(Color.r, 0.0f, 1.0f);
    Color.g = std::clamp(Color.g, 0.0f, 1.0f);
    Color.b = std::clamp(Color.b, 0.0f, 1.0f);
    return;
}
void PgUtil::MakePositiveVector(NiPoint3& Vector)
{
    if (Vector.x < 0.0f)
        Vector.x *= -1.f;
    if (Vector.y < 0.0f)
        Vector.y *= -1.f;
    if (Vector.z < 0.0f)
        Vector.z *= -1.f;
}