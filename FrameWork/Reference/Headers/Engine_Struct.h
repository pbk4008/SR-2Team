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
		_vec3 vPos;
		_vec2 vUV;
	}VTXTEX;
	const _ulong FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVtxCubeTex
	{
		_vec3 vPos;
		_vec3 vUV;
	}VTXCUBE;
	const _ulong FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
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
}
#endif