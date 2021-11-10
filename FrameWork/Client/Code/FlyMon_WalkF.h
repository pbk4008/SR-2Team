#pragma  once
#ifndef __FlyMon_WalkF_h__
#define __FlyMon_WalkF_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CFlyMon_WalkF : public CAnimation
{
	explicit CFlyMon_WalkF();
	explicit CFlyMon_WalkF(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlyMon_WalkF(const CFlyMon_WalkF& rhs);
	virtual ~CFlyMon_WalkF();

public:
	HRESULT Init_FlyMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CFlyMon_WalkF* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // FlyMon_WalkF_h__