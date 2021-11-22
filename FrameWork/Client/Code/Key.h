#pragma once
#ifndef __KEY_H__
#define __KEY_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CKey final : public CGameObject
{
private:
	explicit CKey();
	explicit CKey(LPDIRECT3DDEVICE9 pDevice);
	explicit CKey(const CKey& rhs);
	virtual ~CKey();
public:
	HRESULT Init_Key();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void setCollision();
	void UpDownMove(const _float& fDeltaTime);
public:
	static CKey* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CRcTex* m_pBuffer;
	CTexture* m_pTexture;
	CSphereCollision* m_pInteract;
	_bool m_bUpDownCheck;
	_float m_fMoveTime;
};
#endif