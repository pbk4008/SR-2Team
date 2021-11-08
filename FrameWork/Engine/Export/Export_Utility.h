#pragma once
#ifndef __EXPORT_UTILITY_H__
#define __EXPORT_UTILITY_H__
#include "Engine_Include.h"
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include "Management.h"
#include "Transform.h"

#include "ProtoMgr.h"
#include "TextureMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "CubeTex.h"
#include "TerrainTex.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Animation.h"
#include "Animator.h"

//Management
inline HRESULT Init_Manangement(CManagement** pManangement);
inline HRESULT Change_Scene(CScene* pScene);
inline _int Update_Management(const _float& fDeltaTime);
inline void LateUpdate_Management();
inline void Render_Manangement(LPDIRECT3DDEVICE9& pDevice);
inline CComponent* Get_Component(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType);
inline CGameObject* GetGameObject(LAYERID eLayerID, GAMEOBJECTID eObjID);
inline CGameObject* GetGameObject(GAMEOBJECTID eObjID);

//ProtoMgr
inline HRESULT Init_ProtoMgr();
inline HRESULT Init_ComProto(COMPONENTID eID, CComponent* pComponent);
inline HRESULT Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject);
template<typename T>
T* Clone_ComProto(COMPONENTID eID);
template<typename T>
T* Clone_ObjProto(GAMEOBJECTID eID);

//TextureMgr
inline CTextureMgr* Init_TextureMgr();
inline HRESULT Insert_Texture(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const _tchar* pPath, const _tchar* pState, const _uint& iCnt);
inline vector<LPDIRECT3DBASETEXTURE9>* GetTexture(const _tchar* pTag, TEXTURETYPE eType);

//Renderer
inline CRenderer* Init_RenderComponent();
inline HRESULT Insert_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
inline void Render_GameObject(LPDIRECT3DDEVICE9 pDevice);
inline void Clear_RenderList();

inline void Utility_Release();
#include "Export_Utility.inl"
#endif