#include "SHNManager.h"
namespace SHN {
	std::map<SHNType, std::shared_ptr<CDataReader>> SHNManager::SHNList;
	std::map<unsigned short, struct ItemViewInfo*> SHNManager::IDItemsView;
	std::map<std::string, struct ItemViewInfo*> SHNManager::InxItemsView;
	std::map<unsigned short, struct ItemInfo*> SHNManager::IDItems;
	std::map<std::string, struct ItemInfo*> SHNManager::InxItems;
	std::map < std::string, std::vector<struct ItemViewDummy*>> SHNManager::ItemViewDummyList;
	std::map<unsigned char, struct HairInfo*> SHNManager::IDHairInfo;
	std::map<unsigned char, struct HairColorInfo*> SHNManager::IDHairColorInfo;
}