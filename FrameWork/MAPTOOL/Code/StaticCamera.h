#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"


class CStaticCamera : public CCamera
{
private:
	explicit CStaticCamera();
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera(void);

public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	_int		Update_Object(const _float& fTimeDelta);

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(void);

private:
	_float			m_fDistance = 10.f;
	_float			m_fSpeed = 10.f;
	_float			m_fAngle = 0.f;
public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
};



#endif // StaticCamera_h__
