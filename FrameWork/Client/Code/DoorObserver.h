#pragma once
#ifndef __DOOROBSERVER_H__
#define __DOOROBSERVER_H__
#include "Base.h"
class CDoor;
class CSpawner;
class CDoorObserver final : public CBase
{
private:
	explicit CDoorObserver();
	virtual ~CDoorObserver();
public:
	HRESULT Init_Observer();
	void Insert_Spawner(CSpawner* pSpawner);
	void setDoor(CDoor* pDoor);
	void ActiveObserver();
	void ClearObserver();
	_bool CheckMonster();
	_bool CheckSpawner();
public:
	static CDoorObserver* Create();
private:
	virtual void Free();
public:
	_bool getRoomClear() { return m_bRoomClear; }
private:
	CDoor* m_pDoor;
	vector<CSpawner*> m_vecSpawner;
	_bool m_bActive;
	_bool m_bRoomClear;
};
#endif