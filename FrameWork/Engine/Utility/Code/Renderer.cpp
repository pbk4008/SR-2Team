#include "Engine_Include.h"
#include "Renderer.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CRenderer)
CRenderer::CRenderer()
{

}
CRenderer::~CRenderer()
{
}

HRESULT CRenderer::Insert_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	if ((eGroup < RENDERGROUP::PRIORITY || eGroup >= RENDERGROUP::MAX )|| !pGameObject)
		return E_FAIL;

	m_RenderList[(_ulong)eGroup].push_back(pGameObject);
	pGameObject->AddRef();
	return S_OK;
}

_int CRenderer::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

CComponent* CRenderer::Clone_Component()
{
	return nullptr;
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pDevice)
{
	for (auto& iter : m_RenderList[(_ulong)RENDERGROUP::PRIORITY])
		iter->Render_GameObject();
}

void CRenderer::Render_NonArpha(LPDIRECT3DDEVICE9& pDevice)
{
	
	for (auto& iter : m_RenderList[(_ulong)RENDERGROUP::NONALPHA])
		iter->Render_GameObject();
	
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pDevice)
{
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	for (auto& iter : m_RenderList[(_ulong)RENDERGROUP::ALPHA])
		iter->Render_GameObject();
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pDevice)
{
	for (auto& iter : m_RenderList[(_ulong)RENDERGROUP::UI])
		iter->Render_GameObject();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pDevice)
{
	Render_Priority(pDevice);
	Render_NonArpha(pDevice);
	Render_Alpha(pDevice);
	Render_UI(pDevice);

	Clear_RenderList();
}

void CRenderer::Clear_RenderList()
{
	for (_int i = 0; i < (_int)RENDERGROUP::MAX; i++)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), DeleteObj);
		m_RenderList[i].clear();
	}
}

void CRenderer::Free()
{
	Clear_RenderList();
	CComponent::Free();
}
