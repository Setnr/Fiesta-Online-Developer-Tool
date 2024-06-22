#include "WorleyBrush.h"

void WorleyBrush::UpdatePixelData()
{
	wg.resetPoints();
	wg.addRandomPoints(NumPoints);

	RGBAColor* PixelColorA = (RGBAColor*)data->GetPixels();
	int i = 0;
	float MaxDist = 0.0f;
	float MinDist = 5.f;
	int wmax = data->GetWidth();
	int hmax = data->GetHeight();
	std::vector<double> dists;
	for (int w = 0; w < wmax; w++)
	{
		for (int h = 0; h < hmax; h++)
		{
			wg.getNearestPoints(static_cast<double>(w) / wmax, static_cast<double>(h) / hmax, dists);
			PixelColorA[i] = RGBAColor(dists[PointLoc] * ColorAdjust / 0xFF);
			i++;
		}
	}
	data->MarkAsChanged();

}