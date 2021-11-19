#ifndef TestCubeTex_h__
#define TestCubeTex_h__

#include "ToolGameObject.h"

class CTestCubeObject final : public CToolGameObject
{
private:
	explicit CTestCubeObject();
	explicit CTestCubeObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CTestCubeObject(const CTestCubeObject& rhs);
	virtual ~CTestCubeObject();

public:
	HRESULT Init_CTestCubeObject();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
public: inline CCubeTex* getTestCubeTex() { return m_pCubeTex; }
public:
	static CTestCubeObject* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CCubeTex* m_pCubeTex;
};


#endif // TestCubeTex_h__
