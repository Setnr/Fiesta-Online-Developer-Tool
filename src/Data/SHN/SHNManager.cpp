#include "SHNManager.h"
namespace SHN {
	std::map<SHNType, std::shared_ptr<CDataReader>> SHNManager::SHNList;
	std::map<unsigned short, struct ItemViewInfo*> SHNManager::IDItemsView;
	std::map<std::string, struct ItemViewInfo*> SHNManager::InxItemsView;
	std::map<unsigned short, struct ItemInfo*> SHNManager::IDItems;
	std::map<std::string, struct ItemInfo*> SHNManager::InxItems;
}