#ifndef MonsterObject_h__
#define MonsterObject_h__

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
constexpr TCHAR* FlyMon = L"../../Client/Bin/Resource/Texture/Monster/FlyMon/Idle/IDLE_000.PNG";
constexpr TCHAR* MeleeMon = L"../../Client/Bin/Resource/Texture/Monster/MeleeMon/Idle/IDLE_000.PNG";
constexpr TCHAR* ShootMon = L"../../Client//Bin/Resource/Texture/Monster/SHootMon/Idle/IDLE_000.PNG";
constexpr TCHAR* Boss = L"../../Client//Bin/Resource/Texture/Monster/Boss/Idle/IDLE_000.PNG";
#pragma  endregion


#include "ToolGameObject.h"

class CMonsterObject final : public CToolGameObject
{
private: explicit CMonsterObject();
private: explicit CMonsterObject(LPDIRECT3DDEVICE9 pDeivce);
private: explicit CMonsterObject(const CMonsterObject& rhs);
private: virtual ~CMonsterObject();


public:	virtual	HRESULT						Init_CMonsterObjectObject();
public:	virtual _int						Update_GameObject(const _float& fDeltaTime) override;
public:	virtual void						LateUpdate_GameObject() override;
public:	virtual void						Render_GameObject() override;
public:	virtual CGameObject*				Clone_GameObject() override;
public:	static  CMonsterObject*				Create(LPDIRECT3DDEVICE9 pDevice);
public:	inline	CRcTex*						Get_Tex() { return mTexBuffer; }
public: inline	_uint						getType() { return static_cast<_uint>(meMonsterType); }
public:	inline	void						setType(const GAMEOBJECTID& eMonsterType) { meMonsterType = eMonsterType; }

private: virtual HRESULT					Add_Component() override;
private: virtual void						Free() override;


private: GAMEOBJECTID						meMonsterType;
private: CRcTex*							mTexBuffer;
};



#endif // MonsterObject_h__
