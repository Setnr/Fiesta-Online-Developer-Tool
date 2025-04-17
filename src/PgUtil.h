#pragma once
#include <iostream>

#include <windows.h>

#include <NiMain.h>
#include <NiCamera.h>
#include <NiSourceTexture.h>
#include <NiScreenElements.h>
#include <NiActorManager.h>
#include "Logger.h"
#include "SHN/SHNStruct.h"
#include <NiProperty.h>
#include <NiSourceTexture.h>
#include <NiFont.h>
#include <filesystem>

class PgUtil 
{
public:
	static std::string PathFromApplicationFolder(std::string File);
	static std::string ApplicationPath;
	static std::string PathFromClientFolder(std::string File);
	static std::string ClientFolderPath;
	static std::string PathFromServerFolder(std::string File);
	static std::string ServerFolderPath;
	static char CatchCamera(NiAVObject* pkObject, NiCameraPtr* pkCamera);
	static std::string GetMapFolderPath(char KingdomMap, std::string MapName);
	static void SaveTexture(std::string Path, NiSourceTexturePtr Texture, bool flipped = false);
	static bool LoadNodeNifFile(const char* File, NiNodePtr* spNode, NiTexturePalette* /*Currently Unused */);
	template<typename T>
	static NiPointer<T> LoadNifFile(const char* File, NiTexturePalette* = NULL/*Currently Unused */)
	{
		auto Node = NiNew T;
		if (!NiIsKindOf(NiNode, Node))
		{
			LogError("Failed to Load Nif because Template Type isnt part of Node Type");
			return NiPointer<T>(NULL);
		}
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
	static void LookAndMoveAtWorldPoint(NiCameraPtr Camera, NiPoint3 Point);
	static NiCamera* CreateNewCamera();
	static NiScreenElements* CreateScreenElement(float width, float height, NiSourceTexturePtr texture);
	static NiScreenElements* CreateScreenElement(NiSourceTexturePtr texture);
	static void MakePositiveVector(NiPoint3& Vector);
	static void FixColor(NiColorA& Color);
	static void SaveTexture(std::string Path, NiPixelDataPtr PixelData, bool flipped = false);
	static NiActorManager* CreatePlayerNPC(unsigned short NPCViewIndex);
	static bool AttachItem(NiAVObjectPtr ptr, const char* slot,  const char* item, bool gender, BaseCharClass baseclass);
	static std::string GetBaseClassName(BaseCharClass baseclass)
	{
		switch (baseclass)
		{
		default:
		case BaseCharClass::CC_FIGHT:
			return "Fighter";
			break;

		case BaseCharClass::CC_CLERIC:
			return "Cleric";
			break;
		case BaseCharClass::CC_ARCHER:
			return "Archer";
			break;
		case BaseCharClass::CC_MAGE:
			return "Mage";
			break;
		case BaseCharClass::CC_JOKER:
			return "Joker";
			break;
		case BaseCharClass::CC_SENTINEL:
			return "Sentinel";
			break;
		}
		return "Fighter";
	}

	static bool CatchGeomentry(NiAVObject* obj, NiGeometryPtr* geomentry) 
	{
		if (NiIsKindOf(NiNode, obj)) 
		{
			NiNode* node = NiSmartPointerCast(NiNode, obj);
			for (int i = 0; i < node->GetChildCount(); i++) 
			{
				if (CatchGeomentry(node->GetAt(i), geomentry))
					return true;
			}
		}
		else 
		{
			if (NiIsKindOf(NiGeometry, obj)) 
			{
				*geomentry = NiSmartPointerCast(NiGeometry, obj);
				return true;
			}
			
		}
		return false;
	}
	static NiNode* FindBoneRootNodes(NiNode* node) 
	{
		if (node->GetExtraDataSize()) 
		{
			for (int i = 0; i < node->GetExtraDataSize(); i++) 
			{
				auto data = node->GetExtraDataAt(i);
				if (NiIsKindOf(NiStringExtraData, data)) 
				{
					NiStringExtraDataPtr stringdata = NiSmartPointerCast(NiStringExtraData, data);
					if (stringdata->GetValue().Contains("NiBoneLOD#BoneRoot"))
						return node;
				}
			}
		}
		for (int i = 0; i < node->GetChildCount(); i++) 
		{
			auto child = node->GetAt(i);
			if (NiIsKindOf(NiNode, child))
			{
				auto result = FindBoneRootNodes((NiNode*)child);
				if (result)
					return result;
			}
		}
		return nullptr;
	}
	static std::string GetGenderString(bool Gender) 
	{
		if (Gender)
			return "m";
		return "f";
	}
	static std::pair<NiNodePtr, NiSourceTexturePtr> LoadSubItem(std::string FileName, BaseCharClass ClassID, bool Gender, std::string TextureName)
	{
		std::string NifPath = "reschar\\" + GetBaseClassName(ClassID) + "-" + GetGenderString(Gender) + "\\";
		std::string TexturePath = NifPath + TextureName + ".dds";
		NifPath = NifPath + FileName + ".nif";
		
		NiNodePtr node = PgUtil::LoadNifFile<NiNode>(PgUtil::PathFromClientFolder(NifPath).c_str());
		NiSourceTexturePtr tex = NiSourceTexture::Create(PgUtil::PathFromClientFolder(TexturePath).c_str());

		return { node,tex };


	}
	static NiSourceTexturePtr LoadTextureWithPixelData(std::string Path) 
	{
		NiImageConverterPtr conv = NiImageConverter::GetImageConverter();

		NiPixelDataPtr ReadImage = conv->ReadImageFile(Path.c_str(), 0);

		NiTexture::FormatPrefs BlendPref;
		BlendPref.m_ePixelLayout = NiTexture::FormatPrefs::PixelLayout::TRUE_COLOR_32;
		BlendPref.m_eMipMapped = NiTexture::FormatPrefs::MipFlag::YES;
		BlendPref.m_eAlphaFmt = NiTexture::FormatPrefs::ALPHA_DEFAULT;

		return NiSourceTexture::Create(ReadImage, BlendPref);
	}
	static std::string CreateResCharPath(std::string ItemInx, BaseCharClass Class, bool Gender) 
	{
		std::string Path;
		if (ItemInx.starts_with("_"))
		{
			Path = PathFromClientFolder("reschar\\common\\" + PgUtil::GetGenderString(Gender) + "\\" + ItemInx);
			if(!std::filesystem::exists(Path))
				Path = PathFromClientFolder("reschar\\common\\" + ItemInx);
		}
		else
		{
			Path = PathFromClientFolder("reschar\\" + PgUtil::GetBaseClassName(Class) + "-" + PgUtil::GetGenderString(Gender) + "\\" + ItemInx);
		}
		if (!std::filesystem::exists(Path))
		{
			return "-";
		}
		return Path;
	}

};
