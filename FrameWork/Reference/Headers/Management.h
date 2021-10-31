#pragma once
#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__
#include "Base.h"
BEGIN(Engine)
class CScene;
class CComponent;
class ENGINE_DLL CManagement final :  public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement();
public:
	HRESULT Change_Scene(CScene* pScene);
	_int Update_Management(const _float& fDeltaTime);
	void LateUpdate_Management();
	void Render_Manangement(LPDIRECT3DDEVICE9& pDevice);
private:
	virtual void Free();
public:
	CComponent* getComponent(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType);
private:
	CScene* m_pScene;
};
END
#endif