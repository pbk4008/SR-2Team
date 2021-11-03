#pragma once
#ifndef __CMELEEMON_H__
#define __CMELEEMON_H__

#include "GameObject.h"

BEGIN(Engine)

//class CRcTex;
class CRcCol;
class CTexture;
class CTransform;

END

class CMeleeMon final : public CGameObject
{
private:
	explicit CMeleeMon();
	explicit CMeleeMon(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon(const CMeleeMon& rhs);
	virtual ~CMeleeMon();

public:
	HRESULT Init_MeleeMon(SCENEID eID);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;

public:
	static CMeleeMon* Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID);

private:
	virtual HRESULT Add_Component();
	virtual void Free();

public:
	void setTextureCom(SCENEID eID);

private:
	SCENEID m_eSceneID;
	CRcCol* m_pBufferCom;
	//CRcTex* m_pBufferCom;
	CTexture* m_pTexture;

};
#endif