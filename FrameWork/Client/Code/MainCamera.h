#pragma once
#ifndef __MAINCAMERA_H__
#define __MAINCAMERA_H__
#include "GameObject.h"
#include "Player.h"
BEGIN(Engine)
class CCamera;
END
class CPlayer;
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
	CPlayer::STATE Hit();
	void CameraZoomInAndOut(const _float& fDeltaTime);
	void CameraZoomReset();
private:
	void FollowTarget();
public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTarget(CTransform* pTarget);
private:
	CCamera* m_pCamera;
	_bool m_bLRCheck;
	_float m_fHitDelay;
	_int m_iRoutin;
};
#endif