#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__
template<typename T>
void Safe_Delete(T& point)
{
	if (point)
	{
		delete point;
		point = nullptr;
	}
}

template<typename T>
void Safe_DeleteArr(T& point)
{
	if (point)
	{
		delete[] point;
		point = nullptr;
	}
}

template<typename T>
unsigned long Safe_Release(T& point)
{
	unsigned long dwRefCnt = 0;
	if (point)
	{
		dwRefCnt = point->Release();
		if (dwRefCnt == 0)
			point = nullptr;
	}
	return dwRefCnt;
}
#endif