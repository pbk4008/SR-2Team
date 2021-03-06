#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBackGround final : public CGameObject
{
private:
	explicit CBackGround();
	explicit CBackGround(LPDIRECT3DDEVICE9 pDevice);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();
public:
	HRESULT Init_BackGround(SCENEID eID);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pDevice,SCENEID eID);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	SCENEID m_eSceneID;
	CRcTex* m_pBufferCom;
	CTexture* m_pTexture;
};
#endif