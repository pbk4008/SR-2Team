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

void Utility_Release()
{
	CProtoMgr::DestroyInstance();
	CManagement::DestroyInstance();
}
