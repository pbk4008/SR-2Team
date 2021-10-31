#pragma once
#ifndef __EXPORT_UTILITY_H__
#define __EXPORT_UTILITY_H__
#include "Engine_Include.h"
#include "Management.h"
#include "Scene.h"
#include "Component.h"

inline void Init_Manangement(CManagement** pManangement);
inline HRESULT Change_Scene(CScene* pScene);
inline _int Update_Management(const _float& fDeltaTime);
inline void LateUpdate_Management();
inline void Render_Manangement(LPDIRECT3DDEVICE9& pDevice);
inline CComponent* Get_Component(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType);


inline void Utility_Release();
#include "Export_Utility.inl"
#endif