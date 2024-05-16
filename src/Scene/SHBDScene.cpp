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
	TextureSize = 64;
	while (true)
	{
		if (_SHBD.GetSHBDSize() <= TextureSize)
			break;
		if (TextureSize > 8192)
		{
			LogError("Workable Size does not match up");
			return;
		}
		TextureSize *= 2;
	}
	PixelSize = _SHBD.GetMapSize() * BlockWidth / _SHBD.GetSHBDSize();
	BrushSize = 0;
	NiPoint3 BL(0.f, 0.f, 1.f);
	NiPoint3 BR(TextureSize * PixelSize, 0.f, 1.f);
	NiPoint3 TL(0.f, TextureSize * PixelSize, 1.f);
	NiPoint3 TR(TextureSize * PixelSize, TextureSize * PixelSize, 1.f);
	struct Triangle
	{
		unsigned short one;
		unsigned short two;
		unsigned short three;
	};
	std::vector<NiPoint3> VerticesList = {BL,BR,TL,TR};
	std::vector<NiPoint3> NormalList = { World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir ,World::ms_kUpDir };
	std::vector<NiColorA> ColorList = {NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f),NiColorA(0.f,1.0f,0.f,1.f), NiColorA(0.f,1.0f,0.f,1.f)};
	std::vector<NiPoint2> TextureList1 = { NiPoint2(0.f,0.f),NiPoint2(1.f,0.f),NiPoint2(0.f,1.f),NiPoint2(1.f,1.f)};
	std::vector<NiPoint2> TextureList2 = { NiPoint2(0.f,0.f),NiPoint2(0.25f * static_cast<float>(_SHBD.GetMapSize()),0.f ),NiPoint2(0.f,0.25f * static_cast<float>(_SHBD.GetMapSize())),NiPoint2(0.25f * static_cast<float>(_SHBD.GetMapSize()),0.25f * static_cast<float>(_SHBD.GetMapSize())) }; //2048 
	std::vector<Triangle> TriangleList;

	for (int i = 0; i < TextureList2.size(); i++)
		TextureList1.push_back(TextureList2.at(i));

	TriangleList.push_back(Triangle(0, 1, 2));
	TriangleList.push_back(Triangle(2, 1, 3));

	NiPoint3* pkVertix = NiNew NiPoint3[(int)VerticesList.size()];
	NiPoint3* pkNormal = NiNew NiPoint3[(int)NormalList.size()];
	NiColorA* pkColor = NiNew NiColorA[(int)ColorList.size()];
	NiPoint2* pkTexture = NiNew NiPoint2[(int)TextureList1.size()];

	unsigned short* pusTriList = (unsigned short*)NiAlloc(char, 12 * (TriangleList.size() / 2));// NiNew NiPoint3[TriangleList.size() / 2];

	memcpy(pkVertix, &VerticesList[0], (int)VerticesList.size() * sizeof(NiPoint3));
	memcpy(pkNormal, &NormalList[0], (int)NormalList.size() * sizeof(NiPoint3));
	memcpy(pkColor, &ColorList[0], (int)ColorList.size() * sizeof(NiColorA));
	memcpy(pkTexture, &TextureList1[0], (int)TextureList1.size() * sizeof(NiPoint2));
	memcpy(pusTriList, &TriangleList[0], (int)TriangleList.size() * 3 * sizeof(unsigned short));

	NiTriShapeDataPtr data = NiNew NiTriShapeData((unsigned short)VerticesList.size(), pkVertix, pkNormal, pkColor, pkTexture, 2, NiGeometryData::DataFlags::NBT_METHOD_NONE, (unsigned short)TriangleList.size(), pusTriList);
	NiTriShapePtr Shape = NiNew NiTriShape(data);

	NiTexture::FormatPrefs kPrefs;
	kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::SMOOTH;
	kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
	kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;



	SHBDDataTexture = NiDynamicTexture::Create(TextureSize, TextureSize, kPrefs);


	NiTexturingPropertyPtr spText = NiNew NiTexturingProperty;
	spText->SetBaseTexture(SHBDDataTexture);
	spText->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);
	spText->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);

	NiAlphaPropertyPtr alphaprop = NiNew NiAlphaProperty();
	alphaprop->SetAlphaBlending(true);
	alphaprop->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	

	Shape->AttachProperty(spText);
	Shape->AttachProperty(alphaprop);

	Shape->CalculateNormals();

	Shape->Update(0.0);
	Shape->UpdateEffects();
	Shape->UpdateProperties();
	Shape->Update(0.0);

	if (std::filesystem::exists(PgUtil::CreateMapFolderPath(Info->KingdomMap, Info->MapFolderName, "test.nif")))
		std::filesystem::remove(PgUtil::CreateMapFolderPath(Info->KingdomMap, Info->MapFolderName, "test.nif"));
	BaseNode = NiNew NiNode;
	BaseNode->AttachChild(Shape);

	NiPoint3 CameraTranslate = Camera->GetTranslate();
	BaseNode->SetTranslate(NiPoint3(0.f,0.f, CameraTranslate.z - 10.f));
	BaseNode->UpdateEffects();
	BaseNode->UpdateProperties();
	BaseNode->Update(0.0);
	CanSwitch = true;

	const NiPixelFormat* SHBDTexturePixelFormat = SHBDDataTexture->GetPixelFormat();
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

	NiPoint3 Intersect(0.f, 0.f, 0.f);
	CreateBrushTexture(Intersect);
	LogInfo("Successfully Added SHBD");
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
	auto& io = ImGui::GetIO();

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
}



void SHBDScene::CreateBrushTexture(NiPoint3& BrushPositon)
{
	int BrushTexturePitch = 0;
	unsigned int* BrushTextureColorPtr = (unsigned int*)(SHBDDataTexture->Lock(BrushTexturePitch));

	bool initmap = false;
	auto it = OffsetMap.find(BrushTextureColorPtr);
	std::vector<char> SHBDData = _SHBD.GetDataRefrence();
	if (it == OffsetMap.end())
	{
		OffsetMap.insert({ BrushTextureColorPtr , std::vector<std::pair<unsigned int*, unsigned int>>() });
		for (int h = 0; h < _SHBD.GetSHBDSize(); h++)
		{
			int i = _SHBD.GetMapSize() * h;
			for (int w = 0; w < _SHBD.GetSHBDSize(); w++)
			{
				unsigned int* NewPtr = BrushTextureColorPtr + (h * TextureSize) + w;
				int offset = i + (w / 8);
				int Shift = w % 8;
				if ((SHBDData[offset] >> Shift) & 0x1)
					*NewPtr = Blocked;
				else
					*NewPtr = Walkable;
			}
		}
		SHBDDataTexture->UnLock();
		return;
	}

	int middlew = BrushPositon.x / PixelSize;
	int middleh = BrushPositon.y / PixelSize;

	for (auto vec : it->second)
	{
		*vec.first = vec.second;
	}
	it->second.clear();

	for (int h = middleh - BrushSize; h <= middleh + BrushSize; h++)
	{
		int i = _SHBD.GetMapSize() * h;
		for (int w = middlew - BrushSize; w <= middlew + BrushSize; w++)
		{
			unsigned int* NewPtr = BrushTextureColorPtr + (h * TextureSize) + w;
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{

				int offset = i + (w / 8);
				int Shift = w % 8;
				if (MoveStatus)
				{
					SHBDData[offset] |= (1 << Shift);
					*NewPtr = Blocked;
				}
				else
				{
					SHBDData[offset] &= ~(1 << Shift);
					*NewPtr = Walkable;
				}
				for (auto s = OffsetMap.begin(); s != OffsetMap.end(); s++)
				{
					unsigned int* OtherMapPtr = s->first + (h * TextureSize) + w;
					s->second.push_back({ OtherMapPtr ,*NewPtr });
				}
			}
			it->second.push_back({ NewPtr, *NewPtr });
			*NewPtr = BrushColor;
		}
	}

	SHBDDataTexture->UnLock();
}

void SHBDScene::DrawImGui()
{ 
	StartScene::DrawImGui(); 
	auto io = ImGui::GetIO();
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
		ImGui::TextUnformatted("Copyright Gamebryo / Gamgio / IDK");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	if (ImGui::Checkbox("Set Blocked Area", &MoveStatus))
		UpdateTexture = false;
	bool Remove = !MoveStatus;
	if (ImGui::Checkbox("Rmove Blocked Area", &Remove))
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