#include "Engine_Include.h"
#include "Component.h"

CComponent::CComponent() : m_pDevice(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

CComponent::CComponent(const CComponent& rhs) : m_pDevice(rhs.m_pDevice)
{
	m_pDevice->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
