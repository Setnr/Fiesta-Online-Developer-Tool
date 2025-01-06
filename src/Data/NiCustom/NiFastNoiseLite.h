#pragma once

#include <FastNoiseLite/FastNoiseLite.h>

NiSmartPointer(NiFastNoiseLite);
class NiFastNoiseLite : public FastNoiseLite 
{
public:
	NiFastNoiseLite(int seed = 1337) : FastNoiseLite(seed){}

	void Draw();

	void CreateTexture(int Size);
private:
	NiSourceTexturePtr _Texture;
	NiScreenElementsPtr _ScreenElement;
	bool _Show;
	int _TextureSize = 0;
};