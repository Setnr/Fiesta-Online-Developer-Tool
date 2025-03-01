#pragma once
#include <iostream>

#include <windows.h>

#include <NiNode.h>
#include <NiCamera.h>
#include <NiSourceTexture.h>
#include <NiScreenElements.h>
#include "Logger.h"

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
};
