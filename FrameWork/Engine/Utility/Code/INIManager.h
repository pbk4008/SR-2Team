#pragma once
#ifndef INIManager_h__
#define INIManager_h__

#include "Base.h"
BEGIN(Engine)

	struct INIDATA
	{
		std::string section;
		std::string key;
		std::string value;
	};

	class ENGINE_DLL INIManager : public CBase
	{
	DECLARE_SINGLETON(INIManager)

	private:   explicit      INIManager(void) noexcept { __noop; }
	private:   virtual         ~INIManager(void) noexcept { __noop; }

	private:   using         VecIniData = std::vector<INIDATA>;
	private:   using         VecIniDataIter = std::vector<INIDATA>::iterator;
	private:   using         VecIniDatas = std::vector<VecIniData>;
	private:   using         VecIniDatasIter = std::vector<VecIniData>::iterator;

	public:      void         AddData(const std::string& section, const std::string& key, const std::string& value);
	public:      void         SaveIni(std::string& fileName);
	public:      std::string&      LoadDataString( std::string& fileName, const std::string& section, const std::string& key);
	public:      int            LoadDataInteger(std::string& fileName, const std::string& section, const std::string& key);

	private:   VecIniDatas      m_vIniData;
	private:  std::string		m_strResult;
	};
END
#endif // INIManager_h__