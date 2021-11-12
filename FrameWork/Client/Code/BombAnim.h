#pragma once
#ifndef __BOMBANIM_H__
#define __BOMBANIM_H__
class CBombAnim final : public CAnimation
{
private:
	explicit CBombAnim();
	explicit CBombAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBombAnim(const CBombAnim& rhs);
	virtual ~CBombAnim();
public:
	HRESULT Init_BombAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CBombAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};
#endif