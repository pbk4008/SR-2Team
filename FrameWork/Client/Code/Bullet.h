#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
END
class CBullet abstract : public CGameObject
{
protected:
	explicit CBullet();
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();
public:
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
protected:
	virtual HRESULT Add_Component();
	virtual void Free();
protected:
	CRcTex* m_pBuffer;
};
#endif