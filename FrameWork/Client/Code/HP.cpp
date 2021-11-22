#include "pch.h"
#include "HP.h"
#include "Boss.h"

CHP::CHP()
	:m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_pBufferCom(nullptr),
	m_pTexture(nullptr)
{

}

CHP::CHP(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_pBufferCom(nullptr),
	m_pTexture(nullptr)
{

}

CHP::CHP(const CHP& rhs)
	:m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY), m_pBufferCom(rhs.m_pBufferCom),
	m_pTexture(rhs.m_pTexture)
{

}

CHP::~CHP()
{

}

HRESULT CHP::Init_HP()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	this->setActive(true);

	m_pTexture->setTexture(GetTexture(L"Boss_HPFull", TEXTURETYPE::TEX_NORMAL));

	//m_pBoss->getHP2();

	m_fX = WINCX * 0.5f;
	m_fY = WINCY - 550.f;
	m_fSizeX = 100.f;
	m_fSizeY = 10.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);

	return S_OK;
}


Engine::_int CHP::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;


	VTXTEX* pVertex = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vBuff;
	vBuff = m_pBufferCom->Get_VtxBuffer();
	vBuff->Lock(0, 0, (void**)&pVertex, 0);
	_vec3 CurVtx1;
	_vec3 CurVtx2;
	CurVtx1 = pVertex[1].vPos;
	CurVtx2 = pVertex[2].vPos;
	if (GetAsyncKeyState('T'))
	{
		CurVtx1.x -= 0.1f;
		CurVtx2.x -= 0.1f;

		pVertex[1].vPos = CurVtx1;
		pVertex[2].vPos = CurVtx2;
	}
	vBuff->Unlock();


	D3DXMatrixIdentity(&m_pTransMatrix);

	m_pTransMatrix._11 = m_fSizeX;
	m_pTransMatrix._22 = m_fSizeY;

	m_pTransMatrix._41 = m_fX - (WINCX >> 1);
	m_pTransMatrix._42 = -m_fY + (WINCY >> 1);

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CHP::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CHP::Render_GameObject()
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
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 50);

	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();

	m_pDevice->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CHP* CHP::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHP* pInstance = new CHP(pDevice);
	if (FAILED(pInstance->Init_HP()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHP::Clone_GameObject()
{
	return new CHP(*this);
}

HRESULT CHP::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = CRcTex::Create(m_pDevice);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	pComponent = m_pTexture = CTexture::Create(m_pDevice);
	pComponent->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pComponent);

	return S_OK;
}

void CHP::Free(void)
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
