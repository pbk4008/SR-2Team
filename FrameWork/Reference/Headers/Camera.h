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
	HRESULT Init_Camera(const _vec3& pEye, const _vec3& pAt, const _vec3& pUp
		, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
	HRESULT Init_Camera();
	virtual _int Update_Component(const _float& fDeltaTime)override ;
	virtual CComponent* Clone_Component()override;
public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pDevice, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp
		, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);
public:
	virtual void Free();
<<<<<<< HEAD
protected:
=======
public:
	inline _vec3& getEye() { return m_vEye; }
	inline _vec3& getAt() { return m_vAt; }
public:
	inline void setEye(const _vec3& vEye) { m_vEye = vEye; }
	inline void setAt(const _vec3& vAt) { m_vAt = vAt; }
private:
>>>>>>> main
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