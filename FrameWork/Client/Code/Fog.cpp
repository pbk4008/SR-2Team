#include "pch.h"
#include "Fog.h"

CFog::CFog()
{
}

CFog::CFog(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice)
{
}

CFog::CFog(const CFog& rhs) : CGameObject(rhs)
{
}

CFog::~CFog()
{
}

HRESULT CFog::Init_Fog()
{
	return S_OK;
}

_int CFog::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	return iExit;
}

void CFog::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CFog::Render_GameObject()
{
	CGameObject::Render_GameObject();
}

CGameObject* CFog::Clone_GameObject()
{
	return new CFog(*this);
}

void CFog::ResetObject()
{

}

CFog* CFog::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFog* pInstance = new CFog(pDevice);
	if (FAILED(pInstance->Init_Fog()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CFog::Add_Component()
{
	CGameObject::Add_Component();
	return S_OK;
}

void CFog::Free()
{
	CGameObject::Free();
}