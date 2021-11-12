#pragma  once
#ifndef __MeleeMon_WalkF_h__
#define __MeleeMon_WalkF_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CMeleeMon_WalkF : public CAnimation
{
	explicit CMeleeMon_WalkF();
	explicit CMeleeMon_WalkF(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon_WalkF(const CMeleeMon_WalkF& rhs);
	virtual ~CMeleeMon_WalkF();

public:
	HRESULT Init_MeleeMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CMeleeMon_WalkF* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void Free();

public:
	void setTransform(CTransform* pTransform);

private:
	CTransform* m_pTransform;
	_float m_fSpeed;
	_float m_fDelayTime;
	_float m_fDirChangeTime;
};

#endif // MeleeMon_WalkF_h__