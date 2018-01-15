#include "ConfigCsv.hpp"

#include <sstream>
#include <algorithm>

namespace Flounder
{

	ConfigCsv::ConfigCsv(const std::string &filename) :
			m_fileCsv(new FileCsv(filename, ',')),
			m_values(new std::map<std::string, std::string>())
	{
	}

	ConfigCsv::~ConfigCsv()
	{
		Save();
		m_fileCsv->Save();
		delete m_fileCsv;
		delete m_values;
	}

	void ConfigCsv::Load()
	{
		m_fileCsv->Load();

		for (unsigned int i = 0; i < m_fileCsv->GetRowCount(); i++)
		{
			RowCsv row = m_fileCsv->GetRow(0);
			m_values->insert(row.m_elements.at(0), row.m_elements.at(1));
		}
	}

	void ConfigCsv::Save()
	{
		m_fileCsv->Clear();

		for (auto value : *m_values)
		{
			m_fileCsv->PushRow(RowCsv({value.first, value.second}));
		}
	}

	std::string ConfigCsv::Get(const std::string &key, const std::string &normal)
	{
		if (m_values->find(key) == m_values->end() )
		{
			return normal;
		}

		return m_values->at(key);
	}

	void ConfigCsv::Set(const std::string &key, const std::string &value)
	{
		if (m_values->find(key) == m_values->end() )
		{
			m_values->insert(key, value);
			return;
		}

		m_values->at(key) = value;
	}

	bool ConfigCsv::Get(const std::string &key, const bool &normal)
	{
		/*std::string value = Get(key, "" + normal);
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);
		std::istringstream is(value);
		bool b;
		is >> std::boolalpha >> b;
		return b;*/
		return false;
	}

	int ConfigCsv::Get(const std::string &key, const int &normal)
	{
		//std::string value = Get(key, "" + normal);
		//return atoi(value.c_str());
		return 0;
	}

	float ConfigCsv::Get(const std::string &key, const float &normal)
	{
		std::ostringstream strs;
		strs << normal;
		std::string str = strs.str();
		std::string value = Get(key, str);
		return static_cast<float>(atof(value.c_str()));
	}

	double ConfigCsv::Get(const std::string &key, const double &normal)
	{
		std::ostringstream strs;
		strs << normal;
		std::string str = strs.str();
		std::string value = Get(key, str);
		return atof(value.c_str());
	}
}
