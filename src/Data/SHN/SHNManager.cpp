#include "SHNManager.h"
namespace SHN {
	std::map<SHNType, std::shared_ptr<CDataReader>> SHNManager::SHNList;
}