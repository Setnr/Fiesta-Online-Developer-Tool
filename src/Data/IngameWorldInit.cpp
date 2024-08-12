#include "IngameWorld.h"
#include "../Logger/Logger.h"
#include <filesystem>
#include <thread>
#include <future>
#include <NiPick.h>

#include "SHMD/SHMD.h"


#define EditorSceneError(Msg) {LogError(Msg); return false;}

bool World::LoadSHMD() 
{
	std::ifstream SHMD;
	std::string _FilePath = PgUtil::CreateFilePathFromMapInfo(_Info->KingdomMap, _Info->MapFolderName, "shmd");
	SHMD.open(_FilePath, std::ios::in);

	if (!SHMD.is_open())
	{
		EditorSceneError("Failed to open SHMDFile");
	}

	if (!CheckSHMDVersion(SHMD))
		EditorSceneError("SHMD has wrong FileFormat");

	if (!LoadSky(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Sky");
	if (!LoadWater(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Water");
	if (!LoadGroundObject(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GroundObject");
	std::string GlobalLightPath = _FilePath.substr(0, _FilePath.length() - std::string(_Info->MapFolderName).length()) + "GlobalLight.nif";
	if (!LoadGlobalLight(SHMD, GlobalLightPath, this))
		EditorSceneError("SHMD - Failed to Load GlobalLight");
	if (!LoadFog(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Fog");
	if (!LoadBackGroundColor(SHMD, this))
		EditorSceneError("SHMD - Failed to Load BackGroundColor");
	if (!LoadFrustum(SHMD, this))
		EditorSceneError("SHMD - Failed to Load Frustum");

	std::vector<std::pair<std::string, std::vector<ObjectPosition>>> ObjectList;
	if (!LoadGlobalObjects(SHMD, ObjectList))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightAmbient(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");
	if (!LoadDirectionLightDiffuse(SHMD, this))
		EditorSceneError("SHMD - Failed to Load GlobalObjects");


	std::vector<std::shared_future<void>> _ThreadList;
	for (auto obj : ObjectList)
	{
		auto future = std::async(std::launch::async, [this, obj]
			{
				NiNodePtr MainObj;
				NiPickablePtr Obj;
				for (auto pos = obj.second.begin(); pos < obj.second.end(); pos++)
				{
					std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(obj.first);
					if (!MainObj)
					{
						MainObj = PgUtil::LoadNifFile(Path.c_str(), NULL, PICKABLEOBJECTS);

					}
					Obj = (NiPickable*)MainObj->Clone();
					NiNodePtr ptr = &*Obj;
					this->AttachGroundObj(ptr);
#if PICKABLEOBJECTS
					Obj->SetSHMDPath(obj.first);
#endif
					Obj->SetDefaultCopyType(Obj->COPY_UNIQUE);
					Obj->SetTranslate(pos->pos);
					NiMatrix3 m;
					pos->quater.ToRotation(m);
					Obj->SetRotate(m);
					Obj->SetScale(pos->Scale);

					Obj->DisableSorting();
					Obj->SetSelectiveUpdateRigid(true);

				}
			});
		_ThreadList.push_back(future.share());
	}

	{
		using namespace std::chrono_literals;
		while (_ThreadList.size())
		{
			auto status = _ThreadList.at(_ThreadList.size() - 1).wait_for(20ms);
			if (status == std::future_status::ready)
			{
				_ThreadList.pop_back();
				continue;
			}
			Sleep(1);
		}
	}
}
bool World::LoadSHBD() 
{
	float BlockWidth = _InitFile.OneBlock_width;
	float BlockHeight = _InitFile.OneBlock_height;

	SHBDNode = NiNew NiNode;
	std::vector<char> SHBDData = _SHBD.GetDataRefrence();
	PixelSize = _SHBD.GetMapSize() * BlockWidth / _SHBD.GetSHBDSize();

	const NiPixelFormat* SHBDTexturePixelFormat = &NiPixelFormat::RGBA32;
	Blocked = (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Blocked |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Blocked |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	Walkable = (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Walkable |= (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	Walkable |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Walkable |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	BrushColor = (0xFF << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	BrushColor |= (0xA2 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	BrushColor |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	BrushColor |= (0xFF << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	std::vector<NiPoint3> NormalList = { TerrainWorld::ms_kUpDir ,TerrainWorld::ms_kUpDir ,TerrainWorld::ms_kUpDir ,TerrainWorld::ms_kUpDir };
	std::vector<NiColorA> ColorList = { NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f),NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f) };
	std::vector<NiPoint2> TextureList1 = { NiPoint2(0.f,0.f),NiPoint2(1.f,0.f),NiPoint2(0.f,1.f),NiPoint2(1.f,1.f) };
	std::vector<NiPoint2> TextureList2 = { NiPoint2(0.f,0.f),NiPoint2(0.5f,0.f),NiPoint2(0.f,0.5f),NiPoint2(0.5f,0.5f) }; //2048 
	for (int i = 0; i < TextureList2.size(); i++)
		TextureList1.push_back(TextureList2.at(i));
	struct Triangle
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	std::vector<Triangle> TriangleList;

	TriangleList.push_back(Triangle(0, 1, 2));
	TriangleList.push_back(Triangle(2, 1, 3));

	NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
	alphaprop->SetAlphaBlending(true);
	alphaprop->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);


	int TextureCt = _SHBD.GetSHBDSize() / TextureSize;
	if (_SHBD.GetSHBDSize() % TextureSize != 0)
		TextureCt++;

	std::vector<std::shared_future<std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>>>> _ThreadList;
	for (int TextureH = 0; TextureH < TextureCt; TextureH++)
	{
		auto future = std::async(std::launch::async, 
			[this, TextureCt, TextureH, NormalList,ColorList,TextureList1, TriangleList,alphaprop,SHBDData]
			{
				NiTexture::FormatPrefs kPrefs;
				kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
				kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
				kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

				std::vector<NiPixelDataPtr> TextureList;
				std::vector<NiTriShapePtr> Shapes;
				for (int TextureW = 0; TextureW < TextureCt; TextureW++)
				{
					NiPoint3 BL(TextureW * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
					NiPoint3 BR((TextureW + 1) * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
					NiPoint3 TL(TextureW * TextureSize * PixelSize, (TextureH + 1) * TextureSize * PixelSize, 1.f);
					NiPoint3 TR((TextureW + 1) * TextureSize * PixelSize, (TextureH + 1) * TextureSize * PixelSize, 1.f);

					std::vector<NiPoint3> VerticesList = { BL,BR,TL,TR };

					NiPoint3* pkVertix = NiNew NiPoint3[4];
					memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));

					NiPoint3* pkNormal = NiNew NiPoint3[(int)NormalList.size()];
					NiColorA* pkColor = NiNew NiColorA[(int)ColorList.size()];
					NiPoint2* pkTexture = NiNew NiPoint2[(int)TextureList1.size()];

					unsigned short* pusTriList = (unsigned short*)NiAlloc(char, 12 * (TriangleList.size() / 2));// NiNew NiPoint3[TriangleList.size() / 2];

					memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
					memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
					memcpy(pkTexture, &TextureList1[0], (int)TextureList1.size() * sizeof(NiPoint2));
					memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));


					NiTriShapeDataPtr data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 2, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
					NiTriShapePtr BlockShape = NiNew NiTriShape(data);
					BlockShape->AttachProperty(alphaprop);
					BlockShape->CalculateNormals();

					NiPixelData* pixel = NiNew NiPixelData(TextureSize, TextureSize, NiPixelFormat::RGBA32);

					auto pixeloffset = (unsigned int*)pixel->GetPixels();
					TextureList.push_back(pixel);
					for (int h = 0; h < TextureSize; h++)
					{
						for (int w = 0; w < TextureSize; w++)
						{
							unsigned int* NewPtr = (pixeloffset + (h * pixel->GetWidth()) + w);
							int hges = h + TextureH * TextureSize;
							int wges = w + TextureW * TextureSize;

							int index = hges * _SHBD.GetSHBDSize() + wges;

							size_t charIndex = index / 8;
							if (charIndex < SHBDData.size())
							{
								size_t bitIndex = index % 8;

								if ((SHBDData[charIndex] >> bitIndex) & 0x1)
									*pixeloffset = Blocked;
								else
									*pixeloffset = Walkable;
							}

							pixeloffset++;
						}
					}

					NiSourceTexturePtr texture = NiSourceTexture::Create(pixel,kPrefs);
					texture->SetStatic(false);
					NiTexturingPropertyPtr Texture = NiNew NiTexturingProperty;
					Texture->SetBaseTexture(texture);
					Texture->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
					Texture->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);


					BlockShape->AttachProperty(Texture);
					Shapes.push_back(BlockShape);
					//SHBDNode->AttachChild(BlockShape);
				}
				return std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>> (TextureList, Shapes);
			});
		_ThreadList.push_back(future.share());
	}
	{
		using namespace std::chrono_literals;
		auto it = _ThreadList.begin();
		while (it != _ThreadList.end())
		{
			auto status = it->wait_for(10ms);
			if (status == std::future_status::ready)
			{
				TextureConnector.push_back(it->get().first);
				for (auto shape : it->get().second)
					SHBDNode->AttachChild(shape);
				it++;
				continue;
			}
			Sleep(1);
		}
	}
	SHBDNode->Update(0.0f);
	ResetSHBD(GetSpawnPoint());
	return true;
}

void World::ResetSHBD(NiPoint3 SpawnPoint)
{
	SHBDNode->SetTranslate(NiPoint3(0.f, 0.f, SpawnPoint.z - 10.f));
	SHBDNode->UpdateEffects();
	SHBDNode->UpdateProperties();
	SHBDNode->Update(0.0);
}

NiPoint3 World::GetSpawnPoint() 
{
	NiPick _Pick;
	_Pick.SetTarget(GetWorldScene());
	_Pick.SetPickType(NiPick::FIND_ALL);
	_Pick.SetSortType(NiPick::SORT);
	_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
	_Pick.SetFrontOnly(false);
	_Pick.SetReturnNormal(true);
	_Pick.SetObserveAppCullFlag(true);
	NiPoint3 kOrigin(_Info->RegenX, _Info->RegenY, 0.f);
	if (_Pick.PickObjects(kOrigin, TerrainWorld::ms_kUpDir, true))
	{
		NiPick::Results& results = _Pick.GetResults();
		for (int i = 0; i < results.GetSize(); i++)
		{
			auto result = results.GetAt(i);
			if (result->GetIntersection().z > kOrigin.z)
				kOrigin.z = result->GetIntersection().z;
		}

	}
	kOrigin.z += 100.f;
	return kOrigin;
}