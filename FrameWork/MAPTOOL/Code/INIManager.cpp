#include "pch.h"
#include "INIManager.h"

IMPLEMENT_SINGLETON(INIManager)

	void INIManager::AddData(std::string section, std::string key, std::string value)
	{
		INIDATA iniData;
		iniData.section = section;
		iniData.key = key;
		iniData.value = value;

		VecIniData vIniData;
		vIniData.push_back(iniData);
		m_vIniData.push_back(vIniData);
	}

	void INIManager::SaveIni(std::string fileName)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		for (size_t i = 0; i < m_vIniData.size(); i++)
		{
			VecIniData vData = m_vIniData[i];
			WritePrivateProfileStringA(vData[0].section.c_str(), vData[0].key.c_str(), vData[0].value.c_str(), fileName.c_str());
			vData.clear();
		}

		m_vIniData.clear();
	}

	std::string INIManager::LoadDataString(std::string fileName, std::string section, std::string key)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		char* data = new char[64];
		GetPrivateProfileStringA(section.c_str(), key.c_str(), "", data, 64, fileName.c_str());

		std::string result = data;
		delete data;

		return result;
	}

	int INIManager::LoadDataInteger(std::string fileName, std::string section, std::string key)
	{
		char str[256];
		fileName = "\\" + fileName + ".ini";
		GetCurrentDirectoryA(256, str);

		fileName = str + fileName;

		return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, fileName.c_str());
	}
