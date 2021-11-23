#include "Engine_Include.h"
#include "INIManager.h"

IMPLEMENT_SINGLETON(INIManager)

	void INIManager::AddData(const std::string& section, const std::string& key, const std::string& value)
	{

		m_vIniData.push_back(VecIniData{ INIDATA{ section,key,value } });
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

	std::string& INIManager::LoadDataString( const std::string& fileName, const std::string& section, const std::string& key)
	{
		char str[256];
		GetCurrentDirectoryA(256, str);
		strcat_s(str, "\\");

		std::string strFileName = str + fileName + ".ini";

		char* data = new char[256];
		GetPrivateProfileStringA(section.c_str(), key.c_str(), "", data, 256, strFileName.c_str());

		m_strResult = "";

		m_strResult = data;
		delete[] data;

		return m_strResult;
	}

	int INIManager::LoadDataInteger(const std::string& fileName, const std::string& section, const std::string& key)
	{
		char str[256];
		GetCurrentDirectoryA(256, str);
		strcat_s(str, "\\");

		std::string strFileName = str + fileName + ".ini";

		return GetPrivateProfileIntA(section.c_str(), key.c_str(), 0, strFileName.c_str());
	}

	