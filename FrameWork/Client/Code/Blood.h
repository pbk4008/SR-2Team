#pragma once
#ifndef __BLOOD_H__
#define __BLOOD_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBlood final : public CGameObject
{
private:
	explicit CBlood();
	explicit CBlood(LPDIRECT3DDEVICE9 pDevice);
	explicit CBlood(const CBlood& rhs);
	virtual ~CBlood();
public:
	HRESULT Init_Blood();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void TerrainCheck();
public:
	static CBlood* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setStart(const _vec3 vStart);
private:
	CRcTex* m_pBuffer;
	CTexture* m_pTexture;

	_vec3 m_vStart;
};
#endif