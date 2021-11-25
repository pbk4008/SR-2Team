#include "pch.h"
#include "Shuriken.h"
#include "ShurikenAnim.h"
#include "SphereCollision.h"
#include "ShurikenHit.h"

CShuriken::CShuriken() : m_pAnimation(nullptr), m_fSpeed(0.f), m_pCollision(nullptr), m_pEffect(nullptr)
, m_eScene(SCENEID::STAGE_END)
{
    ZeroMemory(&m_vFirstPos, sizeof(_vec3));
    ZeroMemory(&m_vLook, sizeof(_vec3));
}

CShuriken::CShuriken(LPDIRECT3DDEVICE9 pDevice) : CBullet(pDevice), m_pAnimation(nullptr), m_pCollision(nullptr)
, m_fSpeed(0.f), m_pEffect(nullptr), m_eScene(SCENEID::STAGE_END)
{
    ZeroMemory(&m_vFirstPos, sizeof(_vec3));
    ZeroMemory(&m_vLook, sizeof(_vec3));
}

CShuriken::CShuriken(const CShuriken& rhs) : CBullet(rhs), m_pAnimation(nullptr)
, m_fSpeed(rhs.m_fSpeed), m_pCollision(nullptr), m_pEffect(nullptr), m_eScene(rhs.m_eScene)
{
    Add_Component();

    setEffect();
}

CShuriken::~CShuriken()
{
}

HRESULT CShuriken::Init_Shuriken()
{
    CBullet::Add_Component();
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fSpeed = 10.f;
    return S_OK;
}

_int CShuriken::Update_GameObject(const _float& fDeltaTime)
{
    int iExit = 0;
    _vec3 vAngle = _vec3(70.f, m_fAngle, 0.f);
    //m_pTransform->setAngle(MATRIXINFO::MAT_UP, m_fAngle);
    m_pTransform->setAngle(vAngle);
    iExit = CBullet::Update_GameObject(fDeltaTime);
    Move(fDeltaTime);
    m_fDestroyTime += fDeltaTime;
    if (m_fDestroyTime > 3.f)
    {
        m_fDestroyTime = 0.f;
        setActive(false);
        return iExit;
    }
    m_pCollision->Collison(COLLISIONTAG::MONSTER);
    if (m_pCollision->WallCollision(m_eScene))
    {
        m_pEffect->setActive(true);
        m_pEffect->setStart(m_pTransform->getPos());
        setActive(false);
        return iExit;
    }
    Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
    
    return iExit;
}

void CShuriken::LateUpdate_GameObject()
{
    CBullet::LateUpdate_GameObject();
    if (m_pCollision->getHit())
    {
        m_pEffect->setActive(true);
        m_pEffect->setStart(m_pTransform->getPos());

        m_pCollision->ResetCollision();
        cout << "몬스터 충돌" << endl;
        setActive(false);
    }
}

void CShuriken::Render_GameObject()
{
    m_pDevice->SetTransform(D3DTS_WORLD,&m_pTransform->getWorldMatrix());
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    CBullet::Render_GameObject();

    m_pAnimation->Render_Animation();
    m_pBuffer->Render_Buffer();
    m_pCollision->Render_Collision();
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameObject* CShuriken::Clone_GameObject()
{
    return new CShuriken(*this);
}

void CShuriken::ResetObject()
{
    m_pTransform->setPos(m_vFirstPos);
    m_pTransform->setScale(1.f, 1.f, 1.f);
    m_fDestroyTime = 0.f;
    m_eScene = SCENEID::STAGE_END;
}

void CShuriken::Move(const _float& fDeltaTime)
{
    D3DXVec3Normalize(&m_vLook, &m_vLook);
    _vec3 vPos = m_pTransform->getPos();
    
    vPos += m_vLook * m_fSpeed * fDeltaTime;

    m_pTransform->setPos(vPos);
}

void CShuriken::setEffect()
{
    m_pEffect = static_cast<CShurikenEff*>(GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::SHURIKENEFF));
    if (!m_pEffect)
    {
        m_pEffect = Clone_ObjProto<CShurikenEff>(GAMEOBJECTID::SHURIKENEFF);
        m_pEffect->AddRef();
        Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::SHURIKENEFF, m_pEffect);
    }
    m_pEffect->setActive(false);
    m_pEffect->setStart(m_pTransform->getPos());
}

HRESULT CShuriken::Add_Component()
{
    m_pAnimation = CShurikenAnim::Create(m_pDevice);
    NULL_CHECK_RETURN(m_pAnimation,E_FAIL);
    m_pAnimation->AddRef();
    m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);
    
    m_pCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
    m_pCollision->setRadius(1.f);
    m_pCollision->setActive(true);
    m_pCollision->setTransform(m_pTransform);
    m_pCollision->setTag(COLLISIONTAG::BULLET);
    m_pCollision->setTrigger(COLLISIONTRIGGER::INTERACT);
    m_pCollision->setTarget(this);
    m_pCollision->AddRef();
    m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, m_pCollision);
    
    Insert_ObjCollision(m_pCollision);
   
    return S_OK;
}

void CShuriken::Free()
{
    CBullet::Free();
    Safe_Release(m_pCollision);
    Safe_Release(m_pAnimation);
    Safe_Release(m_pEffect);
}

void CShuriken::setPos(const _vec3& vPos)
{
    m_pTransform->setPos(vPos);
    m_pTransform->setScale(1.f, 1.f, 1.f);
    m_vFirstPos = vPos;
}

CShuriken* CShuriken::Create(LPDIRECT3DDEVICE9 pDevice)
{
    CShuriken* pInstance = new CShuriken(pDevice);
    if (FAILED(pInstance->Init_Shuriken()))
        Safe_Release(pInstance);
    return pInstance;
}
