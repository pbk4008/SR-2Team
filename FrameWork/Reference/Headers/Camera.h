#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CCamera : public CComponent
{
protected:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();
public:
	HRESULT Init_Camera();
	virtual _int Update_Component(const _float& fDeltaTime)override ;
	virtual CComponent* Clone_Component()override;
public:

public:
	virtual void Free();
protected:
	_vec3 m_vEye;
	_vec3 m_vAt;
	_vec3 m_vUp;
	_float m_fAspect;
	_float m_fFov;
	_float m_fNear;
	_float m_fFar;
	_matrix m_matView;
	_matrix m_matProjection;
	_bool m_bProjection;
};
END
#endif