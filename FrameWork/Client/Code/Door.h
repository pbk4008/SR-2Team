#pragma once
#ifndef __DOOR_H__
#define __DOOR_H__
#include "GameObject.h"
class CCube;
class CDoor final : public CGameObject
{
private:
	explicit CDoor();
	explicit CDoor(LPDIRECT3DDEVICE9 pDevice);
	explicit CDoor(const CDoor& rhs);
	virtual ~CDoor();
public:
	HRESULT Init_Door();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void setCollision();
	void DoorOpen(const _float& fDeltaTime);
public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	_bool getClear() { return m_bClear; }
	_bool getEnter() { return m_bEnter; }
public:
	void setTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3& vPos);
	void setTrigger(_vec3 vPos);
	void setIndex(const _int& iIndex) { m_iIndex = iIndex; }
	void setClear(_bool bClear) { m_bClear = bClear; }
private:
	CCube* m_pDoorModel;
	CSphereCollision* m_pInteract;

	_int m_iIndex;
	_float m_fDoorTime;
	_float m_fDoorSpeed;
	_bool m_bEnter;
	_bool m_bClear;
};
#endif