#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera();

public:
public:
	HRESULT		Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	_int		Update_Object(const _float& fTimeDelta);
	virtual CComponent* Clone_Component();
private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Mouse_Fix(void);

private:
	_bool	m_bFix = false;
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);

private:
	virtual void	Free(void);

};


#endif // DynamicCamera_h__
