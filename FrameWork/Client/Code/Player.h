#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "GameObject.h"

class CPlayer final : public CGameObject
{
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
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	void KeyInput(const float& fDelatTime);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTexture;
	_float m_fSpeed;
};
#endif