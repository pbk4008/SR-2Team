void Init_Manangement(CManagement** pManangement)
{
	*pManangement = CManagement::GetInstance();
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
void Utility_Release()
{
	CManagement::DestroyInstance();
}
