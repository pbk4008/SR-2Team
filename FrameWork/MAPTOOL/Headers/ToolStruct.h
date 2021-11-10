#ifndef ToolStruct_h__
#define ToolStruct_h__

// Terrain�� �����ϱ� ���� ����ü 
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
