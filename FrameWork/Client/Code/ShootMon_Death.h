#pragma  once
#ifndef __ShootMon_Death_h__
#define __ShootMon_Death_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CShootMon_Death : public CAnimation
{
	explicit CShootMon_Death();
	explicit CShootMon_Death(LPDIRECT3DDEVICE9 pDevice);
	explicit CShootMon_Death(const CShootMon_Death& rhs);
	virtual ~CShootMon_Death();

public:
	HRESULT Init_ShootMon_Death();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();

public:
	static CShootMon_Death* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // ShootMon_Death_h__