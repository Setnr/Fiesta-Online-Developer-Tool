#include "SHBDScene.h"
#include "FiestaOnlineTool.h"
#include "IngameWorld.h"
#include "Logger.h"

#include <filesystem>
SHBDScene::SHBDScene(MapInfo* Info) : _SHBD(Info)
{
	CanSwitch = false;
	ShowLoadMenu = false;
	_Info = Info;
	_Editor = NiNew EditorScene(Info);
	this->SetupScene(EditorScenePtr, Camera);
	Pitch = 1.57f * 2.0f;
	Yaw = -1.57f;
	Roll = 1.57f;
	MoveStatus = true;
	//NiPoint3 point = Camera->GetTranslate();
	//point -= NiPoint3::UNIT_Z;
	//Camera->LookAtWorldPoint(point, NiPoint3(0.0, 1.0, 0.0));

	float BlockWidth = 0;
	float BlockHeight = 0;
	if(!_Editor->GetOneBlockSize(&BlockWidth, &BlockHeight))
	{
		LogInfo("No Ini Data for " + Info->MapFolderName + " assuming Sizes");
		BlockWidth = 50.f;
		BlockHeight = 50.f;
	}

	BaseNode = NiNew NiNode;
	std::vector<char> SHBDData = _SHBD.GetDataRefrence();
	PixelSize = _SHBD.GetMapSize() * BlockWidth / _SHBD.GetSHBDSize();
	BrushSize = 0;

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

	std::vector<NiPoint3> NormalList = { World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir };
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


	NiTexture::FormatPrefs kPrefs;
	kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

	NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
	alphaprop->SetAlphaBlending(true);
	alphaprop->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);


	int TextureCt = _SHBD.GetSHBDSize() / TextureSize;
	if (_SHBD.GetSHBDSize() % TextureSize != 0)
		TextureCt++;
		
	for (int TextureH = 0; TextureH < TextureCt; TextureH++) 
	{
		std::vector<NiPixelDataPtr> TextureList;
		for (int TextureW = 0; TextureW < TextureCt; TextureW++)
		{
			NiPoint3 BL(TextureW * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
			NiPoint3 BR((TextureW + 1) * TextureSize * PixelSize, TextureH * TextureSize * PixelSize, 1.f);
			NiPoint3 TL(TextureW * TextureSize * PixelSize, (TextureH + 1) * TextureSize * PixelSize, 1.f);
			NiPoint3 TR((TextureW + 1)* TextureSize* PixelSize, (TextureH + 1)* TextureSize* PixelSize, 1.f);

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
					if(charIndex < SHBDData.size())
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

			NiSourceTexturePtr texture = NiSourceTexture::Create(pixel, kPrefs);
			texture->SetStatic(false);
			NiTexturingPropertyPtr Texture = NiNew NiTexturingProperty;
			Texture->SetBaseTexture(texture);
			Texture->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
			Texture->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
			

			BlockShape->AttachProperty(Texture);
			BaseNode->AttachChild(BlockShape);
		}
		TextureConnector.push_back(TextureList);
	}


	if (std::filesystem::exists(PgUtil::CreateMapFolderPath(Info->KingdomMap, Info->MapFolderName, "test.nif")))
		std::filesystem::remove(PgUtil::CreateMapFolderPath(Info->KingdomMap, Info->MapFolderName, "test.nif"));


	ResetSHBD();
	CanSwitch = true;

	//NiPoint3 Intersect(0.f, 0.f, 0.f);
	//CreateBrushTexture(Intersect);
	LogInfo("Successfully Added SHBD");
}

bool SHBDScene::ResetSHBD() 
{
	NiPoint3 CameraTranslate = Camera->GetTranslate();
	BaseNode->SetTranslate(NiPoint3(0.f, 0.f, CameraTranslate.z - 10.f));
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);
	return true;
}

void SHBDScene::Draw(NiRenderer* renderer)
{
	NiVisibleArray m_kVisible;
	NiCullingProcess m_spCuller(&m_kVisible);

	NiDrawScene(Camera, EditorScenePtr, m_spCuller);
	
	NiVisibleArray m_kVisible2;
	NiCullingProcess m_spCuller2(&m_kVisible2);
	
	NiDrawScene(Camera, BaseNode, m_spCuller2);
	//DrawSHBDOverlay();
}

void SHBDScene::UpdateCamera(float fTime)
{
	ImGuiIO& io = ImGui::GetIO();

	FiestaScene::UpdateCamera(fTime);
	float DeltaTime = fTime - FiestaOnlineTool::GetLastUpdateTime();
	if (io.MouseWheel != 0.0f)
	{
		NiPoint3 NodePositon = BaseNode->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);

		float SpeedUp = io.MouseWheel;
		if (io.KeyShift)
			SpeedUp *= 5.0f;
		NodePositon.z = NodePositon.z + 115.f * DeltaTime * SpeedUp;

		BaseNode->SetTranslate(NodePositon);
	}

	if (ImGui::IsAnyItemHovered())
		return;


	NiPoint3 kOrigin, kDir;
	long X, Y;
	FiestaOnlineTool::GetMousePosition(X, Y);
	NiPoint3 Intersect(0.f, 0.f, 0.f);
	if (this->Camera->WindowPointToRay(X, Y, kOrigin, kDir))
	{
		NiPick _Pick;
		_Pick.SetTarget(BaseNode);
		_Pick.SetPickType(NiPick::FIND_FIRST);
		_Pick.SetSortType(NiPick::SORT);
		_Pick.SetIntersectType(NiPick::TRIANGLE_INTERSECT);
		_Pick.SetFrontOnly(false);
		_Pick.SetReturnNormal(true);
		_Pick.SetObserveAppCullFlag(true);

		if (_Pick.PickObjects(kOrigin, kDir, true))
		{
			NiPick::Results& results = _Pick.GetResults();
			for (int i = 0; i < results.GetSize(); i++)
			{
				auto result = results.GetAt(i);
				Intersect = result->GetIntersection();
				CreateBrushTexture(Intersect);
			}

		}
	}
	static POINT CursorPos;
	POINT CurrentCursorPos;
	GetCursorPos(&CurrentCursorPos);
	auto xoffset = CurrentCursorPos.x - CursorPos.x;
	auto yoffset = CurrentCursorPos.y - CursorPos.y;
	CursorPos = CurrentCursorPos;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
	{
		NiPoint3 CameraPosition = Camera->GetTranslate();
		NiPoint3 MoveDirect(0.0f, 0.0f, 0.0f);
		float SpeedUp = 3.0f;
		if (io.KeyShift)
			SpeedUp = 10.0f;
		NiPoint3 UpVector = Camera->GetWorldUpVector();
		UpVector.z = 0.f;

		NiPoint3 RightDirect = Camera->GetWorldRightVector();
		RightDirect.z = 0.f;

		if(yoffset)
			MoveDirect += UpVector * yoffset;
		if (xoffset)
			MoveDirect -= RightDirect * xoffset;

		Camera->SetTranslate(CameraPosition + MoveDirect);
	}
	if (_Editor->ResetCamera())
		ResetSHBD();
}



void SHBDScene::CreateBrushTexture(NiPoint3& BrushPositon)
{

	std::vector<char> SHBDData = _SHBD.GetDataRefrence();

	for (int h = ResetPoint.second - BrushSize; h <= ResetPoint.second + BrushSize && h < _SHBD.GetSHBDSize(); h++)
	{
		if (h < 0)
			continue;
		int i = _SHBD.GetMapSize() * h;
		for (int w = ResetPoint.first - BrushSize; w <= ResetPoint.first + BrushSize && w < _SHBD.GetSHBDSize(); w++)
		{
			if(w < 0)
				continue;
			int TextureH = h / TextureSize;
			int TextureW = w / TextureSize;
			NiPixelDataPtr pixleData = TextureConnector[TextureH][TextureW];
			pixleData->MarkAsChanged();
			int TextureInternH = h % TextureSize;
			int TextureInternW = w % TextureSize;
			unsigned int* NewPtr = ((unsigned int*)pixleData->GetPixels()) + (TextureInternH * pixleData->GetWidth()) + TextureInternW;
			
			int hges = h;
			int wges = w;

			int index = hges * _SHBD.GetSHBDSize() + wges;

			size_t charIndex = index / 8;
			if(charIndex < SHBDData.size())
			{
				size_t bitIndex = index % 8;

				if ((SHBDData[charIndex] >> bitIndex) & 0x1)
					*NewPtr = Blocked;
				else
					*NewPtr = Walkable;
			}
		}
	}

	int middlew = BrushPositon.x / PixelSize;
	int middleh = BrushPositon.y / PixelSize;

	for (int h = middleh - BrushSize; h <= middleh + BrushSize && h < _SHBD.GetSHBDSize(); h++)
	{
		if (h < 0)
			continue;
		int i = _SHBD.GetMapSize() * h;
		
		for (int w = middlew - BrushSize; w <= middlew + BrushSize && w < _SHBD.GetSHBDSize(); w++)
		{
			if (w < 0)
				continue;
			int TextureH = h / TextureSize;
			int TextureW = w / TextureSize;
			NiPixelDataPtr pixleData = TextureConnector[TextureH][TextureW];
			pixleData->MarkAsChanged();
			int TextureInternH = h % TextureSize;
			int TextureInternW = w % TextureSize;
			unsigned int* NewPtr = ((unsigned int*)pixleData->GetPixels()) + (TextureInternH * pixleData->GetWidth()) + TextureInternW;
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				int offset = i + (w / 8);
				int Shift = w % 8;

				_SHBD.UpdateSHBDData(offset, Shift, MoveStatus);
				
			}
			*NewPtr = BrushColor;
		}
	}
	ResetPoint = std::pair<int, int>(middlew, middleh);
	
	
}

void SHBDScene::DrawImGui()
{ 
	StartScene::DrawImGui(); 
	ImGuiIO& io = ImGui::GetIO();
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	int h = 110;
	int w = 200;
	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - w, io.DisplaySize.y - h));
	ImGui::SetNextWindowSize(ImVec2(w, h));

	bool UpdateTexture = true;
	ImGui::Begin("SHBD-Editor", NULL, flags);
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Scroll to move SHBD Higher or Lower");
		ImGui::TextUnformatted("Hold Middlemouse to move WorldMap");
		ImGui::TextUnformatted("LeftClick to change selected area");
		ImGui::TextUnformatted("Move with WASD");
		ImGui::TextUnformatted("Press Shift to Move faster");
		ImGui::TextUnformatted("Right Click to Rotate Cam");
		ImGui::TextUnformatted("Press R to Reset the Camera");
		ImGui::TextUnformatted("Copyright Gamebryo / Gamgio / IDK");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	if (ImGui::Checkbox("Set Blocked Area", &MoveStatus))
		UpdateTexture = false;
	bool Remove = !MoveStatus;
	if (ImGui::Checkbox("Remove Blocked Area", &Remove))
		MoveStatus = false;
	if(ImGui::SliderInt("BrushSize", &BrushSize, 0, 100))
		UpdateTexture = false;

	ImGui::End();



}

void SHBDScene::CreateMenuBar()
{
	StartScene::CreateMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save SHBD"))
		{
			SaveSHBD();
		}
		ImGui::EndMenu();
	}
}

void SHBDScene::SaveSHBD()
{
	_SHBD.Save();
}