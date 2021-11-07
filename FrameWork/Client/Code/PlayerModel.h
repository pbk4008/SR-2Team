#pragma once
#ifndef __PLAYERMODLE_H__
#define __PLAYERMODLE_H__
#include "GameObject.h"
class CPlayer_AttackAnim;
class CPlayerModel final : public CGameObject
{
private:
	explicit CPlayerModel();
	explicit CPlayerModel(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerModel(const CPlayerModel& rhs);
	virtual ~CPlayerModel();
public:
	HRESULT Init_PlayerModel();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
public:
	static CPlayerModel* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTarget(CTransform* pTarget);
public:
	inline void setAttack(_bool bAttack) { m_bAttack = bAttack; }
private:
	CRcTex* m_pBufferCom;
	CPlayer_AttackAnim* m_pAtkAnim;
	_bool m_bAttack;
};
#endif