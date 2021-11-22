#pragma once
#ifndef __ASTAR_H__
#define __ASTAR_H__
#include "Base.h"
#include "NaviMesh.h"
class CAstar final : public CBase
{
private:
	explicit CAstar();
	virtual ~CAstar();
public:
	HRESULT Init_AStar(const _vec3& vPos);
	void StartAstar(const _vec3& vStart, const _vec3& vGoal);
	_int getIndex(const _vec3& vPos);
	_bool getObstacle(const _ulong& dwIndex);
private:
	_bool IsPicking(const _vec3& vPos, const _ulong& dwIndeex);
	_bool PathFind(const _int& dwStartIndex, const _int& dwGoalIndex, const _vec3& vGoalPos);
	void MakeRoute(const _int& iGoalIndex);
	_bool CheckOpen(const _ulong& dwIndex);
	_bool CheckClose(const _ulong& dwIndex);
public:
	static  CAstar* Create(const _vec3& vPos);
private:
	virtual void Free();
public:
	inline list<CELL*>& getRoute() { return m_BestList; }
private:
	_int m_iStartIndex;
	list<_ulong> m_OpenList;
	list<_ulong> m_CloseList;
	list<CELL*> m_BestList;
	vector<CELL*> m_vecCell;
	vector<list<CELL*>> m_vecMesh;
};
#endif