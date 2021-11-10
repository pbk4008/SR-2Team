#ifndef ShootMon_Attack_h__
#define ShootMon_Attack_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CShootMon_Attack : public CAnimation
{
	explicit CShootMon_Attack();
	explicit CShootMon_Attack(LPDIRECT3DDEVICE9 pDevice);
	explicit CShootMon_Attack(const CShootMon_Attack& rhs);
	virtual ~CShootMon_Attack();

public:
	HRESULT Init_ShootMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CShootMon_Attack* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // ShootMon_Attack_h__
