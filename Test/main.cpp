#include <NetworkManager.h>
#pragma comment(lib, "LS")

void main()
{
	CNetworkManager* pManager = CNetworkManager::GetInstance();

	if (pManager)
	{
		if (pManager->Initialize(true))
		{
			pManager->Update();
		}
	}
	CNetworkManager::DestroyInstance();
}