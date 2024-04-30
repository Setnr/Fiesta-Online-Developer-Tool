#include "SHNManager.h"
#include "PgUtil.h"
#include "Logger.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "ImGui/imgui.h"
#include "md5/md5.h"
SHNManager SHNManager::Manager;
void SHNManager::Init() 
{
	auto reader = std::make_shared<CDataReader>();
	std::string Name = "MapInfo";
	reader->Load(Name);
	Manager.Add(SHNType::MapInfo,reader);
}

bool CDataReader::Load(std::string& FileName)
{
	_FileName = FileName;
	std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(".\\ressystem\\" + FileName + ".shn");
	if (!std::filesystem::exists(Path))
	{
        LogError("Faild to Load " + FileName);
		return false;
	}
	std::ifstream InputStream;
    InputStream.open(Path, std::ios::in | std::ios::binary | std::ios::ate);
	if (!InputStream)
	{
        LogError("Faild to open " + FileName + " " + std::strerror(errno));
		return false;
	}
    InputStream.seekg(0, std::ios::beg);
    auto size = std::filesystem::file_size(Path);
    std::vector<char> FullData(size);
    InputStream.read(FullData.data(), size);
    auto CryptedSize = size - 0x24;
    memcpy_s(&Header, sizeof(HEAD), FullData.data(), sizeof(HEAD));
    if (Header.nDataMode == CDataReader::HEAD::DATA_MODE_ENCRYPTION) {
        Encription((FullData.data() + 0x24), CryptedSize);
    }
    memcpy_s(&Header, sizeof(HEAD), FullData.data(), sizeof(HEAD));
    auto SHNDataSize = size - 52 - (Header.nNumOfField * sizeof(HEAD::FIELD));
    SHNData = FullData;
    SHNStart = SHNData.data() + (size - SHNDataSize);
    MD5Hash = md5(SHNData.data(), Header.nFileSize);
   
    SelectedRow.resize(Header.nNumOfRecord);

	return true;
}

void CDataReader::DumpStructure() 
{
	std::ofstream ofs;
	std::string Path = PgUtil::CreateFullFilePathFromBaseFolder(".\\ressystem\\" + _FileName + ".sdts");
	ofs.open(Path);
	ofs << "struct " << _FileName << std::endl; 
	ofs << "{" << std::endl;
	for (int i = 0; i < this->Header.nNumOfField; i++) 
	{
		auto field = this->Header.Field[i];
		ofs << FIELD_TYPELISTtoString(field.Type) << " " << field.Name << ";" << std::endl;
	}
	ofs << "};" << std::endl;
}

unsigned int CDataReader::GetRows() 
{
    return this->Header.nNumOfRecord;
}

bool CDataReader::DrawHeader() 
{
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV  | ImGuiTableFlags_BordersInnerH;
    if (!ImGui::BeginTable(_FileName.c_str(), this->Header.nNumOfField + 1, flags))
        return false;
    for (int i = 0; i < this->Header.nNumOfField; i++)
    {
        ImGui::TableSetupColumn(this->Header.Field[i].Name);
    }
    ImGui::TableSetupColumn("");
    ImGui::TableHeadersRow();
    return true;
}

void CDataReader::DrawRow(unsigned int row)
{
    SHNRow* srow = (SHNRow*)SHNStart;
    for(int j = 0; j < row; j++)
    {
        srow = (SHNRow * )((int)srow + srow->ColLen);
    }
    int Offset = sizeof(SHNRow);
    ImGui::TableNextRow(0,30.f);

    for (int i = 0; i < this->Header.nNumOfField; i++)
    {
        ImGui::TableSetColumnIndex(i);
        if(this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_FILENAME ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_FILEAUTO ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_STR ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_STRAUTO ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_STR_ARRAY ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_STRAUTO_ARRAY ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_VARSTR ||
            this->Header.Field[i].Type == HEAD::FIELD::TYPE_LIST_INXSTR)
            ImGui::Text((char*)((int)srow + Offset));
        else
        {
            std::string s;
            switch (this->Header.Field[i].Size) 
            {
            case 1:
                s = std::to_string(*(char*)((int)srow + Offset));
                break;
            case 2:
                s = std::to_string(*(unsigned short*)((int)srow + Offset));
                break;
            case 4:
                s = std::to_string(*(unsigned int*)((int)srow + Offset));
                break;
            }
            ImGui::Text(s.c_str());
        }
        Offset += Header.Field[i].Size;
    }
    
    ImGui::TableSetColumnIndex(this->Header.nNumOfField);

}

std::string CDataReader::FIELD_TYPELISTtoString(HEAD::FIELD::TYPE_LIST type) 
{
    switch (type) {
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_END:
        return "TYPE_LIST_END";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_BYTE:
        return "TYPE_LIST_BYTE";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_WORD:
        return "TYPE_LIST_WORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_DWORD:
        return "TYPE_LIST_DWORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_QWORD:
        return "TYPE_LIST_QWORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_FLOAT:
        return "TYPE_LIST_FLOAT";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_FILENAME:
        return "TYPE_LIST_FILENAME";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_FILEAUTO:
        return "TYPE_LIST_FILEAUTO";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_REMARK:
        return "TYPE_LIST_REMARK";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_STR:
        return "TYPE_LIST_STR";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_STRAUTO:
        return "TYPE_LIST_STRAUTO";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INX:
        return "TYPE_LIST_INX";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INXBYTE:
        return "TYPE_LIST_INXBYTE";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INXWORD:
        return "TYPE_LIST_INXWORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INXDWORD:
        return "TYPE_LIST_INXDWORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INXQWORD:
        return "TYPE_LIST_INXQWORD";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_BYTE_BIT:
        return "TYPE_LIST_BYTE_BIT";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_WORD_BIT:
        return "TYPE_LIST_WORD_BIT";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_DWORD_BIT:
        return "TYPE_LIST_DWORD_BIT";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_QWORD_BIT:
        return "TYPE_LIST_QWORD_BIT";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_BYTE_ARRAY:
        return "TYPE_LIST_BYTE_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_WORD_ARRAY:
        return "TYPE_LIST_WORD_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_DWORD_ARRAY:
        return "TYPE_LIST_DWORD_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_QWORD_ARRAY:
        return "TYPE_LIST_QWORD_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_STR_ARRAY:
        return "TYPE_LIST_STR_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_STRAUTO_ARRAY:
        return "TYPE_LIST_STRAUTO_ARRAY";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_VARSTR:
        return "TYPE_LIST_VARSTR";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_INXSTR:
        return "TYPE_LIST_INXSTR";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_UNKNOWNED:
        return "TYPE_LIST_UNKNOWNED";
    case HEAD::FIELD::TYPE_LIST::TYPE_LIST_TWO_INX:
        return "TYPE_LIST_TWO_INX";
    default:
        return "Unknown";
    }
}

std::shared_ptr<CDataReader> SHNManager::Get(SHNType type)
{
    return Manager.InternalGet(type);
}

std::shared_ptr<CDataReader> SHNManager::InternalGet(SHNType type)
{
    auto it = SHNList.find(type);
    if (it == SHNList.end())
        return nullptr;
    return it->second;
}

