#ifndef ItemObject_h__
#define ItemObject_h__

#pragma region ClinetPath
//constexpr TCHAR* HP20PATH =		L"../Bin/Resource/Texture/Item/HP20.png";
//constexpr TCHAR* HP50PATH =		L"../Bin/Resource/Texture/Item/HP50.png";
//constexpr TCHAR* HP100PATH =		L"../Bin/Resource/Texture/Item/HP100.png";
//constexpr TCHAR* SHURIKEN20PATH = L"../Bin/Resource/Texture/Item/SHURIKEN20.png";
//constexpr TCHAR* SHURIKEN50PATH = L"../Bin/Resource/Texture/Item/SHURIKEN50.png";
//constexpr TCHAR* BOMB2PATH =		L"../Bin/Resource/Texture/Item/BOMB2.png";
//constexpr TCHAR* BOMB5PATH =		L"../Bin/Resource/Texture/Item/BOMB5.png";
#pragma endregion

#pragma  region ToolPath
constexpr TCHAR* HP20PATH = L"../../Client/Bin/Resource/Texture/Item/HP20.png";
constexpr TCHAR* HP50PATH = L"../../Client/Bin/Resource/Texture/Item/HP50.png";
constexpr TCHAR* HP100PATH = L"../../Client//Bin/Resource/Texture/Item/HP100.png";
constexpr TCHAR* SHURIKEN20PATH = L"../../Client//Bin/Resource/Texture/Item/SHURIKEN20.png";
constexpr TCHAR* SHURIKEN50PATH = L"../../Client//Bin/Resource/Texture/Item/SHURIKEN50.png";
constexpr TCHAR* BOMB2PATH = L"../../Client//Bin/Resource/Texture/Item/BOMB2.png";
constexpr TCHAR* BOMB5PATH = L"../../Client//Bin/Resource/Texture/Item/BOMB5.png";
#pragma  endregion

#include "ToolGameObject.h"

class CItemObject final : public CToolGameObject
{
private: explicit CItemObject();
private: explicit CItemObject(LPDIRECT3DDEVICE9 pDeivce);
private: explicit CItemObject(const CItemObject& rhs);
private: virtual ~CItemObject();


public:	virtual	HRESULT						Init_CItemObjectObject();
public:	virtual _int						Update_GameObject(const _float& fDeltaTime) override;
public:	virtual void						LateUpdate_GameObject() override;
public:	virtual void						Render_GameObject() override;
public:	virtual CGameObject*				Clone_GameObject() override;
public:	static  CItemObject*						Create(LPDIRECT3DDEVICE9 pDevice);
public:	inline	std::array<CRcTex*, 6>&		Get_Tex() { return marrTexBuffer; }
public: inline	_uint						getType() { return static_cast<_uint>(meType); }
public:	inline	void						setType(const eITEM& eType) { meType = eType; }
public: inline	void						getItemPower(int& itemPower) { itemPower = muiItemPower; }
public:	inline	void						setItemPower(const int& itemPower) { muiItemPower = itemPower; }
public: inline	CCollision*					getCollider() { return mpCollider; }

private: virtual HRESULT					Add_Component() override;
private: virtual void						Free() override;


private: CCollision*						mpCollider;
private: _uint								muiItemPower;
private: eITEM								meType;
private: std::array<CRcTex*, 6>				marrTexBuffer;

};



#endif // ItemObject_h__
