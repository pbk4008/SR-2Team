#ifndef MeleeMon_Attack_h__
#define MeleeMon_Attack_h__

#include "Animation.h"
#include "Texture.h"
BEGIN(Engine)
class CTexture;
END

class CMeleeMon_Attack : public CAnimation
{
	explicit CMeleeMon_Attack();
	explicit CMeleeMon_Attack(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon_Attack(const CMeleeMon_Attack& rhs);
	virtual ~CMeleeMon_Attack();

public:
	HRESULT Init_MeleeMon_WalkF();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

private:
	void Setting_Animation(const _float& fDeltaTime);
	void ResetTimer();

public:
	static CMeleeMon_Attack* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // MeleeMon_Attack_h__
