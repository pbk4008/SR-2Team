#ifndef FlyMon_Attack_h__
#define FlyMon_Attack_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CFlyMon_Attack : public CAnimation
{
	explicit CFlyMon_Attack();
	explicit CFlyMon_Attack(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlyMon_Attack(const CFlyMon_Attack& rhs);
	virtual ~CFlyMon_Attack();

public:
	HRESULT Init_FlyMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CFlyMon_Attack* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // FlyMon_Attack_h__
