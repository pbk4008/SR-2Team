#ifndef QuadCubeTex_h__
#define QuadCubeTex_h__
#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CQuadCubeTex final : public CVIBuffer
{
private:
	explicit CQuadCubeTex();
	explicit CQuadCubeTex(LPDIRECT3DDEVICE9 pDevice);
	explicit CQuadCubeTex(const CQuadCubeTex& rhs);
	virtual ~CQuadCubeTex();
public:
	virtual HRESULT Init_Buffer();
	virtual void Render_Buffer();
	virtual CComponent* Clone_Component();
public:
	static CQuadCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
};

END

#endif // QuadCubeTex_h__
