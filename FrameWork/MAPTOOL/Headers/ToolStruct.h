#ifndef ToolStruct_h__
#define ToolStruct_h__

// Terrain을 저장하기 위한 구조체 
typedef struct tagTerrainInfo
{
	int X;
	int Z;
	int Interval;
	int Detail;

	tagTerrainInfo() : X(0), Z(0), Interval(0), Detail(0) {}

	bool operator==(const tagTerrainInfo& rhs)
	{
		if (X == rhs.X && Z == rhs.Z && Interval == rhs.Interval && Detail == rhs.Detail)
			return true;
		return false;
	}
}TERRAININFO;

typedef struct tagQuadInfo
{
	int Interval;
	int Detail;

	tagQuadInfo() : Interval(0), Detail(0) {}

	bool operator==(const tagQuadInfo& rhs)
	{
		if (Interval == rhs.Interval &&
			Detail == rhs.Detail)
			return true;
		return false;
	}
}QUADINFO;

typedef struct tagCubeInfo
{
	int Interval;
	int Detail;

	tagCubeInfo() : Interval(0), Detail(0) {}

	bool operator==(const tagCubeInfo& rhs)
	{

	}
}CUBEINFO;

typedef struct tagTextureInfo
{
	std::wstring strTextureFolder;
	std::wstring strTextureName;
	CTexture* pTexture;

	tagTextureInfo() : pTexture(nullptr) {}
	tagTextureInfo(TCHAR* _strTextureFolder, TCHAR* _strTextureName, CTexture* _pTexture)
	{
		strTextureFolder = _strTextureFolder ;
		strTextureFolder = _strTextureName;
		pTexture = _pTexture;
	}

}TEXTUREINFO;

#endif // ToolStruct_h__
