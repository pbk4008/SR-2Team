#pragma once
#ifndef UI_h__
#define UI_h__

#include "GameObject.h"

BEGIN(Engine)
class  CRcTex;
class  CTexture;
class  CTransform;
class  CRenderer;
END

class CUI : public CGameObject
{
private:
	explicit CUI();
	virtual ~CUI();

public:
	virtual HRESULT Init_UI();
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual void Render_GameObject() override;

	static CUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT Add_Component();
	virtual void Free(void);

private:
	CRcTex*		m_pBufferCom;
	CTexture*	m_pTextureCom;
	CRenderer*  m_pRenderer;

private:
	_matrix	 m_pTransMatrix;
	_matrix	 m_ProjMatrix;
	_float	 m_fX;
	_float	 m_fY;
	_float	 m_fSizeX;
	_float	 m_fSizeY;
};

#endif // UI_h__
