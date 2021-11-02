#include "Engine_Include.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "ProtoMgr.h"

CGameObject::CGameObject() : m_pDevice(nullptr), m_bActive(false), m_pTransform(nullptr)
{
    m_pProtoMgr = CProtoMgr::GetInstance();
    m_pProtoMgr->AddRef();
}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice), m_bActive(false), 
m_pTransform(nullptr), m_pProtoMgr(nullptr)
{
    m_pDevice->AddRef();
    m_pProtoMgr = CProtoMgr::GetInstance();
    m_pProtoMgr->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs) : m_pDevice(rhs.m_pDevice),m_bActive(rhs.m_bActive), 
m_pTransform(static_cast<CTransform*>(rhs.m_pTransform->Clone_Component()))
,m_pProtoMgr(rhs.m_pProtoMgr)
{
    m_pProtoMgr->AddRef();
    m_bActive = true;//클론함수 호출하면 오브젝트 활성화 상태로 바꿈
    m_pDevice->AddRef();
    for (_int i = 0; i < (_int)COMPONENTTYPE::TYPE_END; i++)
    {
        for (auto iter : rhs.m_mapComponent[i])
        {
            if (iter.first == COMPONENTID::TRANSFORM)
                m_mapComponent[i].emplace(iter.first, m_pTransform);
            else
                m_mapComponent[i].emplace(iter.first, iter.second->Clone_Component());
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
        iExit=iter.second->Update_Component(fDeltaTime);
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

void CGameObject::Add_Component()
{
    m_pTransform = m_pProtoMgr->Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);
    m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::TRANSFORM, m_pTransform);
    m_pTransform->AddRef();
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

CComponent* CGameObject::getComponent(COMPONENTID eID,COMPONENTTYPE eType)
{
    CComponent* pCom = Find_Component(eID, eType);
    NULL_CHECK_RETURN(pCom, nullptr);
    return pCom;
}
