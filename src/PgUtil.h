#pragma once
#include <iostream>

#include <windows.h>

#include <NiNode.h>
#include <NiCamera.h>
#include <NiSourceTexture.h>
#include <NiScreenElements.h>

class PgUtil 
{
public:
	static std::string PathFromApplicationFolder(std::string File);
	static std::string ApplicationPath;
	static std::string PathFromClientFolder(std::string File);
	static std::string ClientFolderPath;
	static char CatchCamera(NiAVObject* pkObject, NiCameraPtr* pkCamera);
	static std::string GetMapFolderPath(char KingdomMap, std::string MapName);
	static void SaveTexture(std::string Path, NiSourceTexturePtr Texture);
	static bool LoadNodeNifFile(const char* File, NiNodePtr* spNode, NiTexturePalette* /*Currently Unused */);
	static NiNodePtr LoadNifFile(const char* File, NiTexturePalette* = NULL/*Currently Unused */, bool IsPickable = false);
	static void LookAndMoveAtWorldPoint(NiCameraPtr Camera, NiPoint3 Point);
	static NiCamera* CreateNewCamera();
	static NiScreenElements* CreateScreenElement(float width, float height, NiSourceTexturePtr texture);
	static NiScreenElements* CreateScreenElement(NiSourceTexturePtr texture);
	static void MakePositiveVector(NiPoint3& Vector);
	static void FixColor(NiColorA& Color);
};
