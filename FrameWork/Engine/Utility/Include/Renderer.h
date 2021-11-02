#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__
#include "Component.h"
BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CRenderer final : public CComponent
{
	DECLARE_SINGLETON(CRenderer)
private:
	explicit CRenderer();
	virtual ~CRenderer();
public:
	HRESULT Insert_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
	void Render_GameObject(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	list<CGameObject*> m_RenderList[(_ulong)RENDERGROUP::MAX];
};
END
#endif