#pragma once
#ifndef __CMELEEMON_H__
#define __CMELEEMON_H__

#include "Monster.h"
#include "Player.h"

class CMeleeMon : public CMonster
{
private:
	explicit CMeleeMon();
	explicit CMeleeMon(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon(const CMeleeMon& rhs);
	virtual ~CMeleeMon();

public:
	HRESULT Init_MeleeMon();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	
public:
	static CMeleeMon* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void	Attack();

private:
	virtual HRESULT Add_Component();
	virtual void	Free();

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CCamera*		m_pCamera;
	CPlayer*		m_pPlayer;

	_float			m_fXPos;
	_float			m_fYPos;
	_float			m_fZPos;
	_float			m_fSpeed;
};
#endif