#include "Engine_Include.h"
#include "INIManager.h"

IMPLEMENT_SINGLETON(INIManager)

	void INIManager::AddData(const std::string& section, const std::string& key, const std::string& value)
	{
		INIDATA iniData;
		iniData.section = section;
		iniData.key = key;
		iniData.value = value;

		VecIniData vIniData;
		vIniData.push_back(iniData);

		m_vIniData.push_back(vIniData);
	}

	void INIManager::SaveIni( std::string& fileName)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		for (auto& vData : m_vIniData)
		{
			WritePrivateProfileStringA(vData.front().section.c_str(), vData.front().key.c_str(), vData.front().value.c_str(), fileName.c_str());
		}

		m_vIniData.clear();
	}

	std::string& INIManager::LoadDataString( std::string& fileName, const std::string& section, const std::string& key)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		char* data = new char[256];
		GetPrivateProfileStringA(section.c_str(), key.c_str(), "", data, 256, fileName.c_str());

		m_strResult = data;
		delete[] data;

		return m_strResult;
	}

	int INIManager::LoadDataInteger(std::string& fileName, const std::string& section, const std::string& key)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, fileName.c_str());
	}

	