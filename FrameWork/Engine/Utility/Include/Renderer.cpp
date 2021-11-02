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

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pDevice)
{
	for (_ulong i = 0; i < (_ulong)RENDERGROUP::MAX; i++)
	{
		for (auto& iter : m_RenderList[i])
		{
			iter->Render_GameObject();
			Safe_Release(iter);
		}
		m_RenderList[i].clear();
	}
}

void CRenderer::Free()
{
	for (_int i = 0; i < (_int)RENDERGROUP::MAX; i++)
	{
		for_each(m_RenderList[i].begin(), m_RenderList[i].end(), DeleteObject);
		m_RenderList[i].clear();
	}
	CComponent::Free();
}
