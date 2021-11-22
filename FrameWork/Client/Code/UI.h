#pragma once
#ifndef UI_h__
#define UI_h__

#include "GameObject.h"

class CUiChar;
class CPlayer;

class CUI : public CGameObject
{
private:
	explicit CUI();
	explicit CUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CUI(const CUI& rhs);
	virtual ~CUI();
public:
	virtual HRESULT Init_UI();
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	static CUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Component();
	virtual void Free(void);
private: HRESULT BackGroundUiInit();
private: HRESULT frontCharInit();
private:
	CRcTex*		m_pBufferCom;
	CTexture*	m_pTexture;
private:
	_matrix	 m_pTransMatrix;
	_matrix	 m_ProjMatrix;
	_float	 m_fX;
	_float	 m_fY;
	_float	 m_fSizeX;
	_float	 m_fSizeY;
private:std::array<CUiChar*, 3> mHpUI; //체력숫자 ex ) 1 0 0
private:std::array<CUiChar*, 2> mShurikenFrontUI; // 표창 넘버 ex) 2:
private:std::array<CUiChar*, 2> mBombFrontUI; // 폭탄  넘버 ex) 3:
private:std::array<CUiChar*, 5> mShurikenUI; // 표창 숫자 ex)  99/99
private:std::array<CUiChar*, 5> mBombUI; // 폭탄 숫자 ex)  20/20
private:std::tuple<_int, _int, _int> mPlayerState;
private:CPlayer* mPlayer;
private:_bool		mPlayerTargetCheck;

};

#endif // UI_h__
