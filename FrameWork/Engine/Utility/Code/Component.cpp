#include "Engine_Include.h"
#include "Component.h"

CComponent::CComponent() : m_pDevice(nullptr), m_bActive(false)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice), m_bActive(false)
{
	m_pDevice->AddRef();
}

CComponent::CComponent(const CComponent& rhs) : m_pDevice(rhs.m_pDevice),m_bActive(rhs.m_bActive)
{
	m_bActive = true;
	if(m_pDevice)
		m_pDevice->AddRef();
}

CComponent::~CComponent()
{
}

_int CComponent::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
