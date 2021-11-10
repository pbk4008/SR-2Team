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
		CAnimation* pData;
		const _tchar* pName;
		vector<tagAnimNode*> pLink;
		tagAnimNode() : pData(nullptr), pName(nullptr)
		{
			pLink.reserve(2);
		}
		tagAnimNode(CAnimation* _pData,const _tchar* _pName) : pData(_pData), pName(_pName)
		{
			pLink.reserve(2);
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
	void Change_AnimationTexture(const _tchar* pName);
private:
	ANIMNODE* Find_Node(const _tchar* pName, ANIMNODE* pNode);
	void Delete_Animator(ANIMNODE* deleteNode);
	void ChangeTexture(ANIMNODE* ChangeNode, const _tchar* pName);
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
	map<const _tchar*,_bool> m_mapAnimGroup;
	ANIMNODE* m_pCulAnimNode;
	ANIMNODE* m_pAnimHead;
};
END
#endif