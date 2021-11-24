#include "pch.h"
#include "DoorObserver.h"
#include "Door.h"
#include "Spawner.h"

CDoorObserver::CDoorObserver() : m_bActive(false) , m_pDoor(nullptr), m_bRoomClear(false)
{
}

CDoorObserver::~CDoorObserver()
{
}

HRESULT CDoorObserver::Init_Observer()
{
	m_vecSpawner.reserve(3);
	return S_OK;
}

void CDoorObserver::Insert_Spawner(CSpawner* pSpawner)
{
	if (!pSpawner)
		return;
	pSpawner->setActive(false);
	pSpawner->AddRef();
	m_vecSpawner.emplace_back(pSpawner);
}

void CDoorObserver::setDoor(CDoor* pDoor)
{
	m_pDoor = pDoor;
	m_pDoor->AddRef();
}

void CDoorObserver::ActiveObserver()
{
	if (m_pDoor->getEnter())
	{
		if (!m_bActive)
		{
			for (auto pSpawner : m_vecSpawner)
				pSpawner->setActive(true);
			m_bActive = true;
		}
	}
}

void CDoorObserver::ClearObserver()
{
	for (auto pSpawner : m_vecSpawner)
		pSpawner->setActive(false);
	m_pDoor->setClear(true);
	m_bRoomClear = true;
}

_bool CDoorObserver::CheckMonster()
{
	if (m_vecSpawner.front()->getCurrentMonCount() <= 0)
		return true;
	return false;
}

_bool CDoorObserver::CheckSpawner()
{
	if (m_bActive)
	{
		_bool bCheck = false;
		for (auto pSpawner : m_vecSpawner)
		{
			if (!pSpawner->getActive())
				bCheck = true;
			else
				bCheck = false;
		}
		return bCheck;
	}
	return false;
}

CDoorObserver* CDoorObserver::Create()
{
	CDoorObserver* pInstance = new CDoorObserver;
	if (FAILED(pInstance->Init_Observer()))
		Safe_Release(pInstance);
	return pInstance;
}

void CDoorObserver::Free()
{
	for_each(m_vecSpawner.begin(), m_vecSpawner.end(), DeleteObj);
	m_vecSpawner.clear();
	m_vecSpawner.shrink_to_fit();

	Safe_Release(m_pDoor);
}
