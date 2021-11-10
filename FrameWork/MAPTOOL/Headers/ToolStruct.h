#ifndef ToolStruct_h__
#define ToolStruct_h__

// Terrain을 저장하기 위한 구조체 
typedef struct tagTerrainInfo
{
	int X;
	int Z;
	int Interval;
	int Detail;

	bool operator==(const tagTerrainInfo& OtherInfo)
	{
		if (X == OtherInfo.X && Z == OtherInfo.Z && Interval == OtherInfo.Interval && Detail == OtherInfo.Detail)
			return true;
		return false;
	}
}TERRAININFO;

#endif // ToolStruct_h__
