#include "SHNManager.h"
#include "PgUtil.h"
void SHNManager::Init() 
{
	PgUtil::CreateFullFilePathFromBaseFolder(".\\ressystem\\MapInfo.shn");
}