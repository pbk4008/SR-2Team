#pragma once
#ifndef __FOG_H__
#define __FOG_H__
#include "GameObject.h"
class CFog final : public CGameObject
{
private:
	explicit CFog();
	explicit CFog(LPDIRECT3DDEVICE9 pDevice);
	explicit CFog(const CFog& rhs);
	virtual ~CFog();
public:
	HRESULT Init_Fog();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
public:
	static CFog* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CCollision* m_pCollision;
	
};
#endif