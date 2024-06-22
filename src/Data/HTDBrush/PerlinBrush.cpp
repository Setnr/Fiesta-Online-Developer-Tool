#include "PerlinBrush.h"

void PerlinBrush::UpdatePixelData()
{
	RGBAColor* PixelColorA = (RGBAColor*)data->GetPixels();
	int i = 0;
	int wmax = data->GetWidth();
	int hmax = data->GetHeight();
	for (int w = 0; w < wmax; w++)
	{
		for (int h = 0; h < hmax; h++)
		{
			PixelColorA[i] = RGBAColor(perlin.octave2D_01(static_cast<float>(w) / wmax, static_cast<float>(h) / hmax, Octaves));
			i++;
		}
	}
	data->MarkAsChanged();
}