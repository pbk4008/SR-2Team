#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__
namespace Engine
{

	typedef struct tagVtxCol
	{
		_vec3 vPos;
		D3DXCOLOR tColor;
	}VTXCOL;
	const _ulong FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	typedef struct tagVtxTex
	{
		tagVtxTex() {}
		tagVtxTex(const _vec3& _vpos, const _vec3& _vNormal ,const _vec2& _vUv)
		{
			vPos = _vpos;
			vNormal = _vNormal;
			vUV = _vUv;
		}
		_vec3 vPos;
		_vec3 vNormal;
		_vec2 vUV;
	}VTXTEX;
	const _ulong FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVtxCubeTex
	{
		tagVtxCubeTex() {}
		tagVtxCubeTex(const _vec3& _vpos,const _vec3& _vNormal ,const _vec2& _vUv)
		{
			vPos = _vpos;
			vNormal = _vNormal;
			vUV = _vUv;
		}
		_vec3 vPos;
		_vec3 vNormal;
		_vec2 vUV;
	}VTXCUBE;
	const _ulong FVF_CUBE = D3DFVF_XYZ |D3DFVF_NORMAL| D3DFVF_TEX1;
	typedef struct tagIndex16
	{
		_ushort _0;
		_ushort _1;
		_ushort _2;
	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong _0;
		_ulong _1;
		_ulong _2;
	}INDEX32;

	typedef struct tagUnsignedPoint
	{
		_ulong x;
		_ulong z;
	}UPOINT;
	typedef struct NaviCell
	{
		_vec3 vPoint[3];
		_vec3 vMid[3];
		_vec3 vCenter;
		_float fDist;
		_ulong dwIndex;
		_int iParentIndex;
		//LPDIRECT3DVERTEXBUFFER9 pVtx;
		_bool bCheck;
		NaviCell() :dwIndex(0), iParentIndex(-1), fDist(0.f), bCheck(true)
		{
			bCheck = true;
			ZeroMemory(vPoint, sizeof(vPoint));
			ZeroMemory(vMid, sizeof(vMid));
			ZeroMemory(vCenter, sizeof(_vec3));
		}
		~NaviCell()
		{
		}
	}CELL;
}
#endif