#include "pch.h"
#include "UiChar.h"
#include "Player.h"

CUiChar::CUiChar()
	: mOneChar{}
	, m_fX(0)
	, m_fY(0)
	, m_fSizeX(0)
	, m_fSizeY(0)
{
	D3DXMatrixIdentity(&m_pTransMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
}

CUiChar::CUiChar(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, mOneChar{}
	, m_fX(0)
	, m_fY(0)
	, m_fSizeX(0)
	, m_fSizeY(0)
{
	D3DXMatrixIdentity(&m_pTransMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
}

CUiChar::CUiChar(const CUiChar& rhs)
	: CGameObject(rhs)
	, mOneChar{}
	, m_fX(0)
	, m_fY(0)
	, m_fSizeX(0)
	, m_fSizeY(0)
{
	D3DXMatrixIdentity(&m_pTransMatrix);
	D3DXMatrixIdentity(&m_ProjMatrix);
}

CUiChar::~CUiChar()
{

}

HRESULT CUiChar::Init_Char(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	Add_Component();

	mCharNumber = 0;

	m_fX = fX;
	m_fY = fY;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;


	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

Engine::_int CUiChar::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	D3DXMatrixIdentity(&m_pTransMatrix);

	m_pTransMatrix._11 = m_fSizeX;
	m_pTransMatrix._22 = m_fSizeY;

	m_pTransMatrix._41 = m_fX - (WINCX >> 1);
	m_pTransMatrix._42 = -m_fY + (WINCY >> 1);


	iExit = CGameObject::Update_GameObject(fDeltaTime);

	return iExit;
}

void CUiChar::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CUiChar::Render_GameObject()
{
	_matrix			OldViewMatrix, OldProjMatrix;

	_matrix			IdentityMatrix;
	D3DXMatrixIdentity(&IdentityMatrix);

	m_pDevice->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransMatrix);
	m_pDevice->SetTransform(D3DTS_VIEW, &IdentityMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 130);

	std::get<1>(mOneChar)->Render_Texture(mCharNumber);
	std::get<0>(mOneChar)->Render_Buffer();
	CGameObject::Render_GameObject();

	m_pDevice->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

Engine::CGameObject* CUiChar::Clone_GameObject()
{
	return new CUiChar(*this);
}

void CUiChar::ResetObject()
{

}

CUiChar* CUiChar::Create(LPDIRECT3DDEVICE9 pDevice, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CUiChar* pInstance = new CUiChar(pDevice);
	if (FAILED(pInstance->Init_Char(fX ,  fY ,  fSizeX ,  fSizeY)))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CUiChar::Add_Component()
{
	//Trasnform
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	//Rctex
	pComponent = std::get<0>(mOneChar) = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);
	pComponent->AddRef();

	pComponent = std::get<1>(mOneChar) = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pComponent);
	pComponent->AddRef();

	return S_OK;
}

void CUiChar::Free()
{
	CGameObject::Free();
	Safe_Release(std::get<0>(mOneChar));
	Safe_Release(std::get<1>(mOneChar));
}

void CUiChar::setTexture(const _tchar* pFileName)
{
	std::get<1>(mOneChar)->setTexture(GetTexture(pFileName, TEXTURETYPE::TEX_NORMAL));
}
