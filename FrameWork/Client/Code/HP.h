#pragma once
#ifndef HP_h__
#define HP_h__

#include "GameObject.h"
#include "Boss.h"

class CHP : public CGameObject
{
private:
	explicit CHP();
	explicit CHP(LPDIRECT3DDEVICE9 pDevice);
	explicit CHP(const CHP& rhs);
	virtual ~CHP();

public:
	HRESULT Init_CHP();
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	virtual void ResetObject() override;

public:
	static CHP* Create(LPDIRECT3DDEVICE9 pDevice);
	void setLength(const _float& fLength);

private:
	virtual HRESULT Add_Component() override;
	virtual void Free() override;

public:
	void setTexture(const _tchar* pTextureName);

private:
	CTexture*			m_pTexture;
	CRcTex*				m_pBufferCom;
	CBoss*				m_pBoss;


	_float				m_fLength;
};

#endif // HP_h__