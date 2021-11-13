#ifndef QuadTex_h__
#define QuadTex_h__

#include "ToolGameObject.h"

class CQuadObject final : public CToolGameObject
{
private:
	explicit CQuadObject();
	explicit CQuadObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CQuadObject(const CQuadObject& rhs);
	virtual ~CQuadObject();

public:
	HRESULT Init_CQuadObject();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	CRcTex* Get_Tex() { return m_pQuadTex; }
public:
	static CQuadObject* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CRcTex* m_pQuadTex;
};



#endif // QuadTex_h__
