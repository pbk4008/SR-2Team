#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "GameObject.h"
class CMainCamera;
class CPlayerModel;
class CBullet;
BEGIN(Engine)
class CSphereCollision;
END
class CPlayer final : public CGameObject
{
public:
	enum class STATE {IDLE, ATTACK,WALK, HIT,MAX};
	enum class ATTACKTYPE { SWORD, SHURIKEN, BOMB };
private:
	explicit CPlayer();
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();
public:
	HRESULT Init_Player();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
private:
	void KeyInput(const float& fDelatTime);
	void ChangeState();
	void ChangeAttackType();
	void Dash(const _float& fDeltaTime);
	CBullet* Shoot(GAMEOBJECTID eID, _bool& bCheck);
	void TelePort(const _float& fDeltaTime, const _vec3& vLook, const _float fPower = 1);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setCamera(CMainCamera* pCamera);
	void setModel(CPlayerModel* pModel);
public:
	ATTACKTYPE getAttackType() { return m_eCurType; }
private:
	STATE m_eCulState;
	STATE m_ePreState;

	ATTACKTYPE m_eCurType;
	ATTACKTYPE m_ePreType;

	_float m_fSpeed;
	_bool m_bAttack;
	_bool m_bJump;
	_bool m_bHide;
	_bool m_bDash;
	_bool m_bDashDelay;
	_float m_fAngle;
	_float m_fDashTime;

	CMainCamera* m_pMainCamera;
	CPlayerModel* m_pModel;
	
	CSphereCollision* m_pHitCollision;
	CSphereCollision* m_pAtkCollision;
};
#endif