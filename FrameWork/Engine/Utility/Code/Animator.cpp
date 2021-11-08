#include "Engine_Include.h"
#include "Animator.h"
#include "Animation.h"
CAnimator::CAnimator() : m_pCulAnimNode(nullptr), m_pAnimHead(nullptr)
{
}

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pCulAnimNode(nullptr), m_pAnimHead(nullptr)
{
}

CAnimator::CAnimator(const CAnimator& rhs) : CComponent(rhs), m_pCulAnimNode(rhs.m_pCulAnimNode), m_pAnimHead(rhs.m_pAnimHead)
{
}

CAnimator::~CAnimator()
{
}

HRESULT CAnimator::Init_Animator()
{
	m_pAnimHead = new ANIMNODE(nullptr,L"Head");
	m_pCulAnimNode = m_pAnimHead;
	return S_OK;
}

_int CAnimator::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CComponent::Update_Component(fDeltaTime);

	m_pCulAnimNode->pData->Update_Component(fDeltaTime);

	return iExit;
}

void CAnimator::Render_Animator()
{
	m_pCulAnimNode->pData->Render_Animation();
}

CComponent* CAnimator::Clone_Component()
{
	return new CAnimator(*this);
}

HRESULT CAnimator::Insert_Animation(const _tchar* pName, const _tchar* pConnetName, CAnimation* pAnim, _bool bDouble)
{
	ANIMNODE* pNode = new ANIMNODE(pAnim, pName);
	ANIMNODE* pFindNode = nullptr;
	if (!lstrcmp(pConnetName, L"Head"))
	{
		pFindNode = m_pAnimHead;
	}
	else
	{
		pFindNode = Find_Node(pConnetName, m_pAnimHead);
		NULL_CHECK_RETURN(pFindNode, E_FAIL);
	}
	if (bDouble)
		pNode->pLink.emplace_back(pFindNode);

	pFindNode->pLink.emplace_back(pNode);
	return S_OK;
}

HRESULT CAnimator::Change_Animation(const _tchar* pName)
{
	for (auto pAnimNode : m_pCulAnimNode->pLink)
	{
		if (!lstrcmp(pName, pAnimNode->pName))
		{
			m_pCulAnimNode = pAnimNode;
			setAnimPlay(true);
			return S_OK;
		}
	}
	return E_FAIL;
}

CAnimator::ANIMNODE* CAnimator::Find_Node(const _tchar* pName, ANIMNODE* pNode)
{
	if (pNode->pLink.empty())
		return nullptr;
	for (auto pTmp : pNode->pLink)
	{
		if (!lstrcmp(pName, pTmp->pName))
		{
			for (auto pair : m_mapAnimGroup)
				pair.second = false;
			return pTmp;
		}
		else
		{
			if (!m_mapAnimGroup[pTmp->pName])
			{
				m_mapAnimGroup[pTmp->pName] = true;
				ANIMNODE* res = Find_Node(pName, pTmp);
				return res;
			}
		}
	}
}

void CAnimator::Delete_Animator(ANIMNODE* deleteNode)
{
	Safe_Release(deleteNode->pData);
	m_mapAnimGroup[deleteNode->pName] = true;
	if (!deleteNode->pLink.empty())
	{
		for (auto Anim : deleteNode->pLink)
		{
			if (!m_mapAnimGroup[Anim->pName])
				Delete_Animator(Anim);
		}
	}
	deleteNode->pLink.clear();
	Safe_Delete(deleteNode);
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAnimator* pInstance = new CAnimator(pDevice);
	if (FAILED(pInstance->Init_Animator()))
		Safe_Release(pInstance);
	return pInstance;
}

void CAnimator::Free()
{
	Delete_Animator(m_pAnimHead);
	m_mapAnimGroup.clear();
	CComponent::Free();
}

_bool CAnimator::getAnimPlay()
{
	NULL_CHECK_RETURN(m_pCulAnimNode->pData,false);
	return m_pCulAnimNode->pData->getPlay();
}

const _tchar* CAnimator::getCurrentAnim()
{
	NULL_CHECK_RETURN(m_pCulAnimNode->pData,L"");
	return m_pCulAnimNode->pName;
}

void CAnimator::setAnimPlay(_bool bPlay)
{
	NULL_CHECK(m_pCulAnimNode->pData);
	m_pCulAnimNode->pData->setPlay(bPlay);
}


