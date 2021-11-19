#pragma once
#ifndef Item_h__
#define Item_h__

constexpr const TCHAR* HP20PATH = L"../../Client/Bin/Resource/Texture/Item/HP20.png";
constexpr const TCHAR* HP50PATH = L"../../Client/Bin/Resource/Texture/Item/HP50.png";
constexpr const TCHAR* HP100PATH = L"../../Client//Bin/Resource/Texture/Item/HP100.png";
constexpr const TCHAR* SHURIKEN20PATH = L"../../Client//Bin/Resource/Texture/Item/SHURIKEN20.png";
constexpr const TCHAR* SHURIKEN50PATH = L"../../Client//Bin/Resource/Texture/Item/SHURIKEN50.png";
constexpr const TCHAR* BOMB2PATH = L"../../Client//Bin/Resource/Texture/Item/BOMB2.png";
constexpr const TCHAR* BOMB5PATH = L"../../Client//Bin/Resource/Texture/Item/BOMB5.png";

#include "GameObject.h"

class CItem final : public CGameObject
{
private: explicit CItem();
private: explicit CItem(LPDIRECT3DDEVICE9 pDevice);
private: explicit CItem(const CItem& rhs);
private: virtual ~CItem();

public:		HRESULT Init_CItem();
public:		virtual _int Update_GameObject(const _float& fDeltaTime) override;
public:		virtual void LateUpdate_GameObject() override;
public:		virtual void Render_GameObject() override;
public:		virtual CGameObject* Clone_GameObject() override;
public:		virtual void ResetObject() override;
public:	inline			void setItemPower(const _uint& itemPower) { mItemPower = itemPower; }
public:	inline			void getItemPower(_uint& itemPower) { itemPower = mItemPower; }
public: inline	CCollision* getCollider() { return mpCollider; }


public:		static CItem* Create(LPDIRECT3DDEVICE9 pDevice);

private:	virtual HRESULT Add_Component() override;
private:	virtual void Free() override;

public:		void setTexture(const _tchar* pTextureName);

private:	CTexture* mItemTexture;
private:	std::array<CRcTex*,6> mItemPlane;
private:	_uint mItemPower;
private: CCollision* mpCollider;


};


#endif // Item_h__
