#ifndef CubeTex_h__
#define CubeTex_h__

#include "ToolGameObject.h"

// 0�� : Z-
// 1�� : Z+
// 2�� : X-
// 3�� : X+
// 4�� : Y-
// 5�� : Y+

class CCubeObject final : public CToolGameObject
{
private:
	explicit CCubeObject();
	explicit CCubeObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeObject(const CCubeObject& rhs);
	virtual ~CCubeObject();

public:
	HRESULT Init_CCubeObject();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	inline void Get_arrTex(std::array<CRcTex*, 6>& rhs) { std::copy(m_pCubeTex.begin(), m_pCubeTex.end(), rhs.begin());}
public:
	static CCubeObject* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	std::array<CRcTex*, 6> m_pCubeTex;
};


#endif // CubeTex_h__
