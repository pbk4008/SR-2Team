#ifndef __ENGINE_FUNCTOR_H__
#define __ENGINE_FUNCTOR_H__
auto DeleteMap = [](auto& MapIter) 
{
    _ulong dwRefCnt = 0;
    dwRefCnt = MapIter.second->Release();

    if (0 == dwRefCnt)
        MapIter.second = nullptr;
    
};

auto DeleteObj = [](auto& pInstance) {
    _ulong dwRefCnt = 0;
    dwRefCnt = pInstance->Release();

    if (0 == dwRefCnt)
        pInstance = nullptr;
};
#endif