#pragma once
#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__
#include "Component.h"
BEGIN(Engine)
class CAnimation;
class ENGINE_DLL CAnimator final : public CComponent
{
public:
	typedef struct tagAnimNode
	{
		_ulong dwRefCnt;
		CAnimation* pData;
		const _tchar* pName;
		_int iIndex;
		vector<tagAnimNode*> pLink;
		tagAnimNode() : pData(nullptr), pName(nullptr), iIndex(0), dwRefCnt(0)
		{
			pLink.reserve(2);
		}
		tagAnimNode(CAnimation* _pData,const _tchar* _pName,const _int& pIndex) : pData(_pData), pName(_pName), iIndex(pIndex), dwRefCnt(0)
		{
			pLink.reserve(2);
		}
		_ulong Release()
		{
			if (dwRefCnt == 0)
				delete this;
			else
				return dwRefCnt--;
			return 0;
		}
	}ANIMNODE;
private:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 pDevice);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();
public:
	HRESULT Init_Animator();
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Animator();
	virtual CComponent* Clone_Component();
	HRESULT Insert_Animation(const _tchar* pName, const _tchar* pConnetName, CAnimation* pAnim, _bool bDouble=false);
	HRESULT Change_Animation(const _tchar* pName);
	HRESULT Connet_Animation(const _tchar* pName, const _tchar* pConnectName, _bool bDouble=false);
	void Change_AnimationTexture(const _tchar* pName);
	
private:
	ANIMNODE* Find_Node(const _tchar* pName, ANIMNODE* pNode);
	void Delete_Animator(ANIMNODE* deleteNode);
	void ChangeTexture(ANIMNODE* ChangeNode, const _tchar* pName);
	void NameGroupReset();
public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	_bool getAnimPlay();
	const _tchar* getCurrentAnim();
public:
	void setAnimPlay(_bool bPlay);
private:
	vector<pair<const _tchar*, _bool>> m_vecAnimGroup;
	//map<const _tchar*,_bool> m_mapAnimGroup;
	ANIMNODE* m_pCulAnimNode;
	ANIMNODE* m_pAnimHead;
};
END
#endif