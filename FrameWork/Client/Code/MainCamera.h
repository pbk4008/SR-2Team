#pragma once
#ifndef __MAINCAMERA_H__
#define __MAINCAMERA_H__
#include "GameObject.h"
BEGIN(Engine)
class CCamera;
END
class CMainCamera final : public CGameObject
{
private:
	explicit CMainCamera();
	explicit CMainCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CMainCamera(const CMainCamera& rhs);
	virtual ~CMainCamera();
public:
	HRESULT Init_MainCamera();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CMainCamera* Clone_GameObject() ;
private:
	void FollowTarget();
public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTarget(CGameObject* pTarget);
private:
	CGameObject* m_pTarget;
	CCamera* m_pCamera;
};
#endif