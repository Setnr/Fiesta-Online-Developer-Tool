#include <NiTriShapeData.h>
#include <vector>
class NiTriShapeDataExtended : public NiTriShapeData 
{
public:
    NiTriShapeDataExtended(std::vector<std::pair<int, int>> WHList, unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture,
        unsigned short usNumTextureSets, NiGeometryData::DataFlags eNBTMethod,
        unsigned short usTriangles, unsigned short* pusTriList) : NiTriShapeData(usVertices, pkVertex, pkNormal, pkColor, pkTexture, usNumTextureSets,eNBTMethod, usTriangles, pusTriList), WHList(WHList)
    {
        for (int i = 0; i < WHList.size(); i++)
        {
            x = WHList[i].first; 
            y = WHList[i].second;
            InternList.push_back(&pkVertex[i]);
        }
    }

private:
    std::vector<std::vector<NiPoint3>> InternList;
};

#ifndef NiTriShapeDataExtended_H
#define NiTriShapeDataExtended_H
    NiImplementRTTI(NiTriShapeDataExtended, NiGeometryData);
#endif