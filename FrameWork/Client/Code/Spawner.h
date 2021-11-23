#pragma once
#ifndef __SPAWNER_H__
#define __SPAWNER_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CAnimation;
END
class CSpawner final : public CGameObject
{
private:
	explicit CSpawner();
	explicit CSpawner(LPDIRECT3DDEVICE9 pDevice);
	explicit CSpawner(const CSpawner& rhs);
	virtual ~CSpawner();
public:
	HRESULT Init_Spawner();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void SettingAnimation();
	void SpawnMonster(const _float& fDeltaTime);
	void RandomMonSpawn();
public:
	static CSpawner* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setSettingSpawner(const _ulong& dwMaxCount);
	void setTransform(const _vec3& vPos, const _vec3& vAngle);
	void setIndex(const _ulong dwIndex) { m_dwIndex = dwIndex; }
	void SettingCollision();
private:
	CRcTex* m_pBuffer;
	CAnimation* m_pAnimation;
	CSphereCollision* m_pInteract;
	_ulong m_dwMaxMonCount;
	_ulong m_dwCurrentMonCount;
	_ulong m_dwIndex;
	_float m_fSpawnTime;
	_float m_fCollisionTime;
	bool bCheck;
};
#endif