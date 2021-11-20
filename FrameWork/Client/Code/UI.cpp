#include "pch.h"
#include "UI.h"

CUI::CUI()
{

}

CUI::~CUI()
{

}

HRESULT CUI::Init_UI()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 400.f;
	m_fY = 75.f;
	m_fSizeX = 800.f;
	m_fSizeY = 150.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}

/* 직교투영행렬을 만든다. */
/* 3차원 로컬스페이스 상에 정의된 정점정보를 2차원 투영스페이스로 변환한다. */

Engine::_int CUI::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	D3DXMatrixIdentity(&m_pTransMatrix);

	m_pTransMatrix._11 = m_fSizeX;
	m_pTransMatrix._22 = m_fSizeY;

	m_pTransMatrix._41 = m_fX - (WINCX >> 1);
	m_pTransMatrix._42 = -m_fY + (WINCY >> 1);

	RECT		rcUI;

	SetRect(&rcUI, m_fX - m_fSizeX * 0.5f, m_fY - m_fSizeY * 0.5f,
		m_fX + m_fSizeX * 0.5f, m_fY + m_fSizeY * 0.5f);	

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::UI, this);

	return iExit;
}

//POINT		ptMouse;
//GetCursorPos(&ptMouse);
//ScreenToClient(g_hWnd, &ptMouse);
//
//if (TRUE == PtInRect(&rcUI, ptMouse))
//MSG_BOX("충돌!");

void CUI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{
	_matrix			OldViewMatrix, OldProjMatrix;

	m_pTextureCom->setTexture();

	_matrix			IdentityMatrix;
	D3DXMatrixIdentity(&IdentityMatrix);

	m_pDevice->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransMatrix);
	m_pDevice->SetTransform(D3DTS_VIEW, &IdentityMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 50);

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();

	m_pDevice->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

HRESULT CUI::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex 버펔콤
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CUI::Free(void)
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
