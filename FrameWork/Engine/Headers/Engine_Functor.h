#ifndef __ENGINE_FUNCTOR_H__
#define __ENGINE_FUNCTOR_H__
BEGIN(Engine)
    auto DeleteMap = [](auto& MapIter)
    {
        _ulong dwRefCnt = 0;
        dwRefCnt = MapIter.second->Release();

        if (0 == dwRefCnt)            
            MapIter.second = nullptr;
    };

    auto DeleteObj = [](auto& pInstance)
    {
        _ulong dwRefCnt = 0;
        dwRefCnt = pInstance->Release();

        if (0 == dwRefCnt)
            pInstance = nullptr;
    };


    class CFind_Tag
    {
    public:
        explicit CFind_Tag() : m_pTag(nullptr)
        {}
        explicit CFind_Tag(const _tchar* pTag) : m_pTag(pTag)
        {}
        ~CFind_Tag() {}
    public:
        template<typename T>
        _bool operator()(const T& pair)
        {
            if (!lstrcmp(m_pTag, pair.first))
                return true;
            return false;
        }
    private:
        const _tchar* m_pTag;
    };
END
#endif