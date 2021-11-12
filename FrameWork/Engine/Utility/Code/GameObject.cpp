#include "Export_Utility.h"
#include "Component.h"

CGameObject::CGameObject() : m_pDevice(nullptr), m_bActive(false), m_pTransform(nullptr), m_bClone(false)
{

}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice), m_bActive(false), m_bClone(false),
m_pTransform(nullptr)
{

    m_pDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs) : m_pDevice(rhs.m_pDevice), m_bActive(rhs.m_bActive), m_bClone(rhs.m_bClone)
, m_pTransform(Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM))
{
    m_bClone = true;
    if (rhs.m_pDevice)
        m_pDevice->AddRef();
    m_bActive = true;//클론함수 호출하면 오브젝트 활성화 상태로 바꿈
    for (_int i = 0; i < (_int)COMPONENTTYPE::TYPE_END; i++)
    {
        for (auto iter : rhs.m_mapComponent[i])
        {
            if (iter.first == COMPONENTID::TRANSFORM)
            {
                m_mapComponent[i].emplace(iter.first, m_pTransform);
                m_pTransform->AddRef();
            }
            else if (iter.first == COMPONENTID::COLLISION)
            {
                continue;
            }
            else if (iter.first == COMPONENTID::ANIMATION)
            {
                continue;
            }
            else
            {
                m_mapComponent[i].emplace(iter.first, iter.second);
                iter.second->AddRef();
            }
        }
    }

}

CGameObject::~CGameObject()
{
}

_int CGameObject::Update_GameObject(const _float& fDeltaTime)
{
    _int iExit = 0;
    for (auto& iter : m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC])
    {
        if (!iter.second->getActive())//활성화 상태가 아니라면 Update를 돌리지 않는다
            continue;
        iExit = iter.second->Update_Component(fDeltaTime);
        if (iExit & 0x80000000)
            return iExit;
    }
    return iExit;
}

void CGameObject::LateUpdate_GameObject()
{
}

void CGameObject::Render_GameObject()
{
}

void CGameObject::ResetObject()
{
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, COMPONENTTYPE eType)
{
    auto iter = m_mapComponent[(_ulong)eType].find(eID);
    if (iter == m_mapComponent[(_ulong)eType].end())
        return nullptr;
    return iter->second;
}

HRESULT CGameObject::Add_Component()
{
    m_pTransform = Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);
    m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::TRANSFORM, m_pTransform);
    m_pTransform->AddRef();
    return S_OK;
}

void CGameObject::Free()
{
    Safe_Release(m_pTransform);
    for (_ulong i = 0; i < (_ulong)COMPONENTTYPE::TYPE_END; ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), DeleteMap);
        m_mapComponent[i].clear();
    }
    Safe_Release(m_pDevice);
}

CComponent* CGameObject::getComponent(COMPONENTID eID, COMPONENTTYPE eType)
{
    CComponent* pCom = Find_Component(eID, eType);
    NULL_CHECK_RETURN(pCom, nullptr);
    return pCom;
}

void CGameObject::setActive(const _bool& bActive)
{
    m_bActive = bActive;
    if (bActive)
    {
        for (_int i = 0; i < (_int)COMPONENTTYPE::TYPE_END; i++)
        {
            for (auto pCom : m_mapComponent[i])
                pCom.second->setActive(true);
        }
    }
    else
    {
        for (_int i = 0; i < (_int)COMPONENTTYPE::TYPE_END; i++)
        {
            for (auto pCom : m_mapComponent[i])
                pCom.second->setActive(false);
        }
    }
}
