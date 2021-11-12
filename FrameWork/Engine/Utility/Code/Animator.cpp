#include "Engine_Include.h"
#include "Animator.h"
#include "Animation.h"
CAnimator::CAnimator() : m_pCulAnimNode(nullptr), m_pAnimHead(nullptr)
{
}

CAnimator::CAnimator(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_pCulAnimNode(nullptr), m_pAnimHead(nullptr)
{
}

CAnimator::CAnimator(const CAnimator& rhs) : CComponent(rhs), m_pCulAnimNode(nullptr), m_pAnimHead(nullptr)
{
	m_pAnimHead = new ANIMNODE(nullptr, L"Head",m_vecAnimGroup.size());
	//m_mapAnimGroup.emplace(m_pAnimHead->pName, false);
	m_vecAnimGroup.emplace_back(make_pair(m_pAnimHead->pName, false));
	m_pCulAnimNode = m_pAnimHead;
}

CAnimator::~CAnimator()
{
}

HRESULT CAnimator::Init_Animator()
{
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
	ANIMNODE* pNode = new ANIMNODE(pAnim, pName,m_vecAnimGroup.size());
	pNode->dwRefCnt++;
	m_vecAnimGroup.emplace_back(make_pair(pName, false));
	//m_mapAnimGroup.emplace(pName, false);
	ANIMNODE* pFindNode = nullptr;
	if (!lstrcmp(pConnetName, L"Head"))
		pFindNode = m_pAnimHead;
	else
	{
		pFindNode = Find_Node(pConnetName, m_pAnimHead);
		NULL_CHECK_RETURN(pFindNode, E_FAIL);
	}
	if (bDouble)
	{
		pNode->pLink.emplace_back(pFindNode);
		pFindNode->dwRefCnt++;
	}
	pFindNode->pLink.emplace_back(pNode);
	NameGroupReset();
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

HRESULT CAnimator::Connet_Animation(const _tchar* pName, const _tchar* pConnectName, _bool bDouble)
{
	ANIMNODE* pCurNode = Find_Node(pConnectName,m_pAnimHead);
	NULL_CHECK_RETURN(pCurNode, E_FAIL);
	NameGroupReset(); 
	ANIMNODE* pConnetNode = Find_Node(pName, m_pAnimHead);
	NULL_CHECK_RETURN(pConnetNode, E_FAIL);
	NameGroupReset();
	pConnetNode->pLink.emplace_back(pCurNode);
	pCurNode->dwRefCnt++;
	if (bDouble)
	{
		pCurNode->pLink.emplace_back(pConnetNode);
		pConnetNode->dwRefCnt++;
	}

	return S_OK;
}

void CAnimator::Change_AnimationTexture(const _tchar* pName)
{
	ChangeTexture(m_pAnimHead, pName);
	NameGroupReset();
}

CAnimator::ANIMNODE* CAnimator::Find_Node(const _tchar* pName, ANIMNODE* pNode)
{
	if (pNode->pLink.empty())
		return nullptr;
	for (auto pTmp : pNode->pLink)
	{
		if (!lstrcmp(pName, pTmp->pName))
			return pTmp;
		else
		{
			//if (!m_mapAnimGroup[pTmp->pName])
			if (!m_vecAnimGroup[pTmp->iIndex].second)
			{
				m_vecAnimGroup[pTmp->iIndex].second = true;
				ANIMNODE* res = Find_Node(pName, pTmp);
				if (!res)
					continue;
				else
					return res;
			}
		}
	}
	return nullptr;
}

void CAnimator::Delete_Animator(ANIMNODE* deleteNode)
{
	if (!deleteNode)
		return;
	Safe_Release(deleteNode->pData);
	//m_mapAnimGroup[deleteNode->pName] = true;
	m_vecAnimGroup[deleteNode->iIndex].second = true;
	if (!deleteNode->pLink.empty())
	{
		for (auto Anim : deleteNode->pLink)
		{
			//if (!m_mapAnimGroup[Anim->pName])
			if (!Anim)
				continue;
			if (!m_vecAnimGroup[Anim->iIndex].second)
				Delete_Animator(Anim);
			Safe_Release(Anim);
		}
	}
	deleteNode->pLink.clear();
	Safe_Release(deleteNode);
}

void CAnimator::ChangeTexture(ANIMNODE* ChangeNode, const _tchar* pName)
{
	if (m_pAnimHead != ChangeNode)
	{
		ChangeNode->pData->setTexture(pName);
		//m_mapAnimGroup[ChangeNode->pName] = true;
		m_vecAnimGroup[ChangeNode->iIndex].second = true;
	}
	for (auto pNode : ChangeNode->pLink)
	{
		if(!m_vecAnimGroup[pNode->iIndex].second)
			ChangeTexture(pNode, pName);
	}
}

void CAnimator::NameGroupReset()
{
	//for (auto iter = m_mapAnimGroup.begin(); iter != m_mapAnimGroup.end(); iter++)
	for (auto iter = m_vecAnimGroup.begin(); iter != m_vecAnimGroup.end(); iter++)
		(*iter).second = false;
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
	m_vecAnimGroup.clear();
	//m_mapAnimGroup.clear();
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


