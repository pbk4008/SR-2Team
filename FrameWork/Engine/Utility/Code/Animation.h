#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "Component.h"
BEGIN(Engine)
class CTexture;
class ENGINE_DLL CAnimation abstract : public CComponent
{
protected:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pDevice);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation();
public:
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Animation();
protected:
	virtual void Free();
public:
	void setTexture(CTexture* pTexutre);
	void setPlay(_bool bPlay) { m_bPlay = bPlay; }
protected:
	CTexture* m_pTexture;

	_float m_fCulTime;
	_float m_fPlaySpeed;
	_bool m_bPlay;
	_bool m_bLoop;
	_int m_iIndex;
};
END
#endif