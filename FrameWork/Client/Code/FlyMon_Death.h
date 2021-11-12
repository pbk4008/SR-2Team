#pragma  once
#ifndef __FlyMon_Death_h__
#define __FlyMon_Death_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CFlyMon_Death : public CAnimation
{
	explicit CFlyMon_Death();
	explicit CFlyMon_Death(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlyMon_Death(const CFlyMon_Death& rhs);
	virtual ~CFlyMon_Death();

public:
	HRESULT Init_FlyMon_Death();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CFlyMon_Death* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // FlyMon_Death_h__