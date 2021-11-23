#pragma  once
#ifndef __MeleeMon_Death_h__
#define __MeleeMon_Death_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CMeleeMon_Death : public CAnimation
{
	explicit CMeleeMon_Death();
	explicit CMeleeMon_Death(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon_Death(const CMeleeMon_Death& rhs);
	virtual ~CMeleeMon_Death();

public:
	HRESULT Init_MeleeMon_Death();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();

public:
	static CMeleeMon_Death* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // MeleeMon_Death_h__