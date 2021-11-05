#pragma once
#ifndef __CMELEEMON_H__
#define __CMELEEMON_H__

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CMeleeMon : public CGameObject
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

private:
	void	Follow_Mouse();
	void	Key_Input();
public:
	static CMeleeMon* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void	Free();

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;

	_float			m_fXPos;
	_float			m_fYPos;
	_float			m_fZPos;
};
#endif