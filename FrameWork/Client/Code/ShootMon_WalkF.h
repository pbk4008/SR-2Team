#pragma  once
#ifndef __ShootMon_WalkF_h__
#define __ShootMon_WalkF_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CShootMon_WalkF : public CAnimation
{
	explicit CShootMon_WalkF();
	explicit CShootMon_WalkF(LPDIRECT3DDEVICE9 pDevice);
	explicit CShootMon_WalkF(const CShootMon_WalkF& rhs);
	virtual ~CShootMon_WalkF();

public:
	HRESULT Init_ShootMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CShootMon_WalkF* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // ShootMon_WalkF_h__