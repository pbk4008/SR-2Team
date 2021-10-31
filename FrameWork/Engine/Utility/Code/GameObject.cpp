#include "Engine_Include.h"
#include "GameObject.h"
#include "Component.h"

CGameObject::CGameObject() : m_pDevice(nullptr), m_dwIndex(0)
{
}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)
{
    m_pDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs) : m_pDevice(rhs.m_pDevice), m_dwIndex(rhs.m_dwIndex)
{
   /* m_pDevice->AddRef();
    for (auto iter : rhs.m_mapComponent)
    {
        iter.second->AddRef();
        m_mapComponent.emplace(iter.first, iter.second.Clone_Component());
    }*/
}

CGameObject::~CGameObject()
{
}

_int CGameObject::Update_GameObject(const _float& fDeltaTime)
{
    _int iExit = 0;
    for (auto& iter : m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC])
    {
        /*iExit = iter.second->Update_Componet(fDeltaTime);*/
        if (iExit & 0x80000000)
            return iExit;
    }
    return iExit;
}

CComponent* CGameObject::Find_Component(COMPONENTID eID,COMPONENTTYPE eType)
{
    auto iter = m_mapComponent[(_ulong)eType].find(eID);
    if (iter == m_mapComponent[(_ulong)eType].end())
        return nullptr;
    return iter->second;
}

void CGameObject::Free()
{
    Safe_Release(m_pDevice);
    for (_ulong i = 0; i < (_ulong)COMPONENTTYPE::TYPE_END; ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), DeleteMap);
        m_mapComponent[i].clear();
    }
}

CComponent* CGameObject::getComponent(COMPONENTID eID,COMPONENTTYPE eType)
{
    CComponent* pCom = Find_Component(eID, eType);
    NULL_CHECK_RETURN(pCom, nullptr);
    return pCom;
}
