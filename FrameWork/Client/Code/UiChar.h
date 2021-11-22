#pragma once
#ifndef Char_h__
#define Char_h__

class CPlayer;

#include "GameObject.h"
class CUiChar final : public CGameObject
{
private:
	explicit CUiChar();
	explicit CUiChar(LPDIRECT3DDEVICE9 pDevice);
	explicit CUiChar(const CUiChar& rhs);
	virtual ~CUiChar();
public:
	HRESULT Init_Char(_float fX, _float fY, _float fSizeX, _float fSizeY);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
public:
	static CUiChar* Create(LPDIRECT3DDEVICE9 pDevice,_float fX, _float fY, _float fSizeX,_float fSizeY);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:void setTexture(const _tchar* pFineName);
public:void setTextureNumber(const _ulong& textureNumber) { mCharNumber = textureNumber;}
public:void setSize(const _float& sizex, const _float& sizey) { m_fSizeX = sizex; m_fSizeY = sizey; }

private:std::pair<CRcTex*, CTexture*> mOneChar;
private:_ulong	 mCharNumber;
private:_matrix	 m_pTransMatrix;
private:_matrix	 m_ProjMatrix;
private:_float	 m_fX;
private:_float	 m_fY;
private:_float	 m_fSizeX;
private:_float	 m_fSizeY;
};


#endif // Char_h__

