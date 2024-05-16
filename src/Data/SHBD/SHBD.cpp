#include "SHBD.h"

void ShineBlockData::AppendToTexture(NiDynamicTexturePtr texture, int x, int y, int width, int height)
{
	int BrushTexturePitch = 0;
	unsigned int* BrushTextureColorPtr = (unsigned int*)(texture->Lock(BrushTexturePitch));
	const NiPixelFormat* SHBDTexturePixelFormat = texture->GetPixelFormat();

	unsigned int Blocked = (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Blocked |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Blocked |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	unsigned int Walkable = (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Walkable |= (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	Walkable |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Walkable |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);


	for (int h = y; h <= y + height; h++)
	{
		int i = MapSize * h;
		for (int w = x; w <= x + width; w++)
		{
			unsigned int* NewPtr = BrushTextureColorPtr + (h * texture->GetWidth()) + w;
			int offset = i + (w / 8);
			int Shift = w % 8;
			if ((Data[offset] >> Shift) & 0x1)
				*NewPtr = Blocked;
			else
				*NewPtr = Walkable;
		}
	}

	texture->UnLock();
}