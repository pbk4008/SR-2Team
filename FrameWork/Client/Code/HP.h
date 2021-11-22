#pragma once
#ifndef HP_h__
#define HP_h__

#include "GameObject.h"

BEGIN(Engine)
class  CRcTex;
class  CTexture;
class  CTransform;
class  CRenderer;
END

class CBoss;
class CHP : public CGameObject
{
private:
	explicit CHP();
	explicit CHP(LPDIRECT3DDEVICE9 pDevice);
	explicit CHP(const CHP& rhs);
	virtual ~CHP();

public:
	virtual HRESULT Init_HP();
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;


	static CHP* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Component();
	virtual void Free(void);

private:
	CRcTex*		m_pBufferCom;
	CTexture*	m_pTexture;
	CBoss*		m_pBoss;

private:
	_matrix	 m_pTransMatrix;
	_matrix	 m_ProjMatrix;
	_float	 m_fX;
	_float	 m_fY;
	_float	 m_fSizeX;
	_float	 m_fSizeY;
	_int	 m_iLength;
};

#endif // HP_h__
