#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "Component.h"
BEGIN(Engine)
class CTexture;
class CTextureMgr;
class ENGINE_DLL CAnimation abstract : public CComponent
{
protected:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pDevice);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation();
public:
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
protected:
	void InitTexture(const _tchar* pTextureName);
protected:
	virtual void Free();
public:
	_bool getPlay() { return m_bPlay; }
public:
	void setTexture(CTexture* pTexutre);
	void setPlay(_bool bPlay) { m_bPlay = bPlay; }
protected:
	CTextureMgr* m_pTextureMgr;
	CTexture* m_pTexture;

	_float m_fCulTime;
	_float m_fPlaySpeed;
	_bool m_bPlay;
	_bool m_bLoop;
	_bool m_bDelay;
	_int m_iIndex;
};
END
#endif