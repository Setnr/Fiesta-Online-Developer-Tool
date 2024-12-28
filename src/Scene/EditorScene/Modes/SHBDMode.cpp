#include "SHBDMode.h"
#include <NiPick.h>
#include <FiestaOnlineTool.h>
#include <thread>
#include <future>
#include <Data/IngameWorld/WorldChanges/FogChange.h>

NiImplementRTTI(SHBDMode, TerrainMode);

void SHBDMode::Draw()
{
	EditMode::Draw();

	NiVisibleArray kVisible;
	NiCullingProcess kCuller(&kVisible);
	NiDrawScene(Camera, _BaseNode, kCuller);
}
void SHBDMode::Update(float fTime)
{
	TerrainMode::Update(fTime);
	if (_Update)
	{
		UpdateSHBD();
	}
	auto SHBD = kWorld->GetSHBD();
	if (SHBD->HadDirectUpdate() || TextureConnector.empty())
		CreateSHBDNode();

	_BaseNode->Update(fTime);
	_BaseNode->UpdateProperties();
}
void SHBDMode::ProcessInput()
{
	TerrainMode::ProcessInput();

	ImGuiIO& io = ImGui::GetIO();
	float DeltaTime = FiestaOnlineTool::GetDeltaTime();
	if (io.MouseWheel != 0.0f)
	{
		if (!ImGui::IsKeyDown((ImGuiKey)VK_MENU))
		{
			NiPoint3 NodePositon = _SHBDNode->GetTranslate();
			NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);

			float SpeedUp = io.MouseWheel;
			if (io.KeyShift)
				SpeedUp *= 5.0f;
			NodePositon.z = NodePositon.z + 115.f * DeltaTime * SpeedUp;

			_SHBDNode->SetTranslate(NodePositon);
		}
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) 
	{
		auto SHBD = kWorld->GetSHBD();
		_Data = SHBD->GetSHBDData();
		_Update = true;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) 
	{
		auto SHBD = kWorld->GetSHBD();
		auto NewData = SHBD->GetSHBDData();
		SHBDChangePtr Change = NiNew SHBDChange(kWorld, _Data, NewData);
		kWorld->AttachStack(NiSmartPointerCast(WorldChange, Change));
		_Update = false;
	}
}
void SHBDMode::UpdateSHBD() 
{
	auto ini = kWorld->GetShineIni();
	auto SHBD = kWorld->GetSHBD();

	float PixelSize = SHBD->GetMapSize() * ini->GetOneBlockWidht() / SHBD->GetSHBDSize();

	int middlew = MouseIntersect.x / PixelSize;
	int middleh = MouseIntersect.y / PixelSize;
	for (int w = middlew - _BrushSize; w <= middlew + _BrushSize && w < (int)SHBD->GetSHBDSize(); w++)
	{
		if (w < 0)
			continue;
		for (int h = middleh - _BrushSize; h <= middleh + _BrushSize && h < (int)SHBD->GetSHBDSize(); h++)
		{
			if (h < 0)
				continue;
			if (!((w - middlew) * (w - middlew) + (h - middleh) * (h - middleh) <= _BrushSize * _BrushSize))
				continue;

			SHBD->UpdateSHBDData(w, h, _Walkable);

			int TextureH = h / TextureSize;
			int TextureW = w / TextureSize;
			NiPixelDataPtr pixleData = TextureConnector[TextureH][TextureW];

			pixleData->MarkAsChanged();
			int TextureInternH = h % TextureSize;
			int TextureInternW = w % TextureSize;
			unsigned int* NewPtr = ((unsigned int*)pixleData->GetPixels()) + (TextureInternH * pixleData->GetWidth()) + TextureInternW;
			if (_Walkable)
				*NewPtr = Walkable;
			else
				*NewPtr = Blocked;
		}
	}
}
void SHBDMode::CreateSHBDNode() 
{
	auto SHBD = kWorld->GetSHBD();
	auto ini = kWorld->GetShineIni();

	TextureConnector.clear();
	_SHBDNode->RemoveAllChildren();
	float PixelSize = SHBD->GetMapSize() * ini->GetOneBlockWidht() / SHBD->GetSHBDSize();

	std::vector<NiPoint3> NormalList = { NiPoint3::UNIT_Z ,NiPoint3::UNIT_Z ,NiPoint3::UNIT_Z ,NiPoint3::UNIT_Z };
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

	int TextureCt = SHBD->GetSHBDSize() / TextureSize;
	if (SHBD->GetSHBDSize() % TextureSize != 0)
		TextureCt++;
	const NiPixelFormat* SHBDTexturePixelFormat = &NiPixelFormat::RGBA32;
	Blocked = (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Blocked |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Blocked |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);

	Walkable = (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_RED)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_RED);
	Walkable |= (0xCD << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_GREEN)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_GREEN);
	Walkable |= (0x0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_BLUE)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_BLUE);
	Walkable |= (0xA0 << SHBDTexturePixelFormat->GetShift(NiPixelFormat::COMP_ALPHA)) & SHBDTexturePixelFormat->GetMask(NiPixelFormat::COMP_ALPHA);


	std::vector<std::shared_future<std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>>>> _ThreadList;
	for (int TextureH = 0; TextureH < TextureCt; TextureH++)
	{
		auto future = std::async(std::launch::async,
			[this, TextureCt, TextureH, NormalList, ColorList, TextureList1, TriangleList, alphaprop,PixelSize, SHBD]
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

							int index = hges * SHBD->GetSHBDSize() + wges;

							size_t charIndex = index / 8;
							if (charIndex >= SHBD->GetMapSize() * SHBD->GetSHBDSize())
								continue;
							if (SHBD->IsWalkable(wges,hges))
							{
								*pixeloffset = Walkable;
							}else
							{
								*pixeloffset = Blocked;
							}

							pixeloffset++;
						}
					}

					NiSourceTexturePtr texture = NiSourceTexture::Create(pixel, kPrefs);
					texture->SetStatic(false);
					NiTexturingPropertyPtr Texture = NiNew NiTexturingProperty;
					Texture->SetBaseTexture(texture);
					Texture->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
					Texture->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);


					BlockShape->AttachProperty(Texture);
					Shapes.push_back(BlockShape);
				}
				return std::pair<std::vector<NiPixelDataPtr>, std::vector<NiTriShapePtr>>(TextureList, Shapes);
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
					_SHBDNode->AttachChild(shape);
				it++;
				continue;
			}
			Sleep(1);
		}
	}
	_SHBDNode->UpdateEffects();
	_SHBDNode->UpdateProperties();
	_SHBDNode->Update(0.0f);
}
void SHBDMode::UpdateMouseIntersect()
{
	NiPoint3 kOrigin, kDir;
	auto Point = FiestaOnlineTool::CurrentMousePosition();
	if (Camera->WindowPointToRay(Point.x, Point.y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(true);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);
		_Pick.SetTarget(_SHBDNode);
		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();
			if (results.GetSize() > 0)
			{
				MouseIntersect = results.GetAt(0)->GetIntersection();
				return;
			}
		}
	}
}