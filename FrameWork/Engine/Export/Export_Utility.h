#pragma once
#ifndef __EXPORT_UTILITY_H__
#define __EXPORT_UTILITY_H__
#include "Engine_Include.h"
#include "Scene.h"
#include "Component.h"
#include "Management.h"

#include "ProtoMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "Texture.h"

//Management
inline HRESULT Init_Manangement(CManagement** pManangement);
inline HRESULT Change_Scene(CScene* pScene);
inline _int Update_Management(const _float& fDeltaTime);
inline void LateUpdate_Management();
inline void Render_Manangement(LPDIRECT3DDEVICE9& pDevice);
inline CComponent* Get_Component(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType);

//ProtoMgr
inline HRESULT Init_ComProto(COMPONENTID eID, CComponent* pComponent);
inline HRESULT Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject);
template<typename T>
T* Clone_ComProto(COMPONENTID eID);
template<typename T>
T* Clone_ObjProto(GAMEOBJECTID eID);

inline void Utility_Release();
#include "Export_Utility.inl"
#endif