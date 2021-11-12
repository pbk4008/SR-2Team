HRESULT Init_Manangement(CManagement** pManangement)
{
	*pManangement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManangement, E_FAIL);
	return S_OK;
}
HRESULT Change_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Change_Scene(pScene);
}
_int Update_Management(const _float& fDeltaTime)
{
	return CManagement::GetInstance()->Update_Management(fDeltaTime);
}
void LateUpdate_Management()
{
	CManagement::GetInstance()->LateUpdate_Management();
}
void Render_Manangement(LPDIRECT3DDEVICE9& pDevice)
{
	CManagement::GetInstance()->Render_Manangement(pDevice);
}
CComponent* Get_Component(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType)
{
	return CManagement::GetInstance()->getComponent(eLayerID, eObjID, eComID, eType);
}
CGameObject* GetGameObject(LAYERID eLayerID, GAMEOBJECTID eObjID)
{
	return CManagement::GetInstance()->getGameObject(eLayerID, eObjID);
}
CGameObject* GetGameObject(GAMEOBJECTID eObjID)
{
	return CManagement::GetInstance()->getGameObject(eObjID);
}
HRESULT Add_GameObject(LAYERID eLayerID, GAMEOBJECTID eObjID, CGameObject* pObj)
{
	return CManagement::GetInstance()->add_GameObject(eLayerID, eObjID, pObj);
}
HRESULT Init_ProtoMgr()
{
	CProtoMgr* pInstance = CProtoMgr::GetInstance();
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	return S_OK;
}
HRESULT Init_ComProto(COMPONENTID eID, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Init_ComProto(eID, pComponent);
}
HRESULT Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject)
{
	return CProtoMgr::GetInstance()->Init_ObjProto(eID, pGameObject);
}
template<typename T>
T* Clone_ComProto(COMPONENTID eID)
{
	return CProtoMgr::GetInstance()->Clone_ComProto<T>(eID);
}
template<typename T>
T* Clone_ObjProto(GAMEOBJECTID eID)
{
	return CProtoMgr::GetInstance()->Clone_ObjProto<T>(eID);
}


CTextureMgr* Init_TextureMgr()
{
	CTextureMgr* pInstance = CTextureMgr::GetInstance();
	NULL_CHECK_RETURN(pInstance, nullptr);

	return pInstance;
}
HRESULT Insert_Texture(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const _tchar* pPath, const _tchar* pState, const _uint& iCnt)
{
	return CTextureMgr::GetInstance()->Insert_Texture(pDevice, eType, pPath, pState, iCnt);
}
vector<LPDIRECT3DBASETEXTURE9>* GetTexture(const _tchar* pTag, TEXTURETYPE eType)
{
	return CTextureMgr::GetInstance()->getTexture(pTag, eType);
}


CRenderer* Init_RenderComponent()
{
	CRenderer* pInstance = CRenderer::GetInstance();
	NULL_CHECK_RETURN(pInstance, nullptr);
	return pInstance;
}
HRESULT Insert_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject)
{
	return CRenderer::GetInstance()->Insert_RenderGroup(eGroup, pGameObject);
}
void Render_GameObject(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer::GetInstance()->Render_GameObject(pDevice);
}
void Clear_RenderList()
{
	CRenderer::GetInstance()->Clear_RenderList();
}

CCollisionMgr* Init_CollisionMgr()
{
	CCollisionMgr* pInstance = CCollisionMgr::GetInstance();
	NULL_CHECK_RETURN(pInstance, nullptr);

	return pInstance;
}
HRESULT Insert_Collision(CCollision* pCollision)
{
	return CCollisionMgr::GetInstance()->Insert_Collision(pCollision);
}
void ClearCollisionList()
{
	CCollisionMgr::GetInstance()->ClearCollisionList();
}

void Utility_Release()
{
	CManagement::DestroyInstance();
	CRenderer::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CCollisionMgr::DestroyInstance();
	CTextureMgr::DestroyInstance();
}