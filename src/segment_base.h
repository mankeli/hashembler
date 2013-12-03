#pragma once

namespace hashembler
{

class segment_c
{
public:
	list<variable_c> variables;

	vector<uint8_t> data;

	value_t m_startpc;
	value_t m_pc;
	value_t m_datapos;
	int m_pass;

	virtual void begin(value_t pc, int pass)
	{
		m_startpc = pc;
		m_pc = pc;
		m_pass = pass;
		m_datapos = 0;

		// TODO: make this change size in a better way
		data.resize(65536);
	}


	value_t &get_variable(string mulkku)
	{
		for (auto &it: variables)
		{
			if (it.m_name == mulkku)
			{
				cerr << f("var '%s' = 0x%08X", it.m_name.c_str(), it.m_value) << "\n";
				return it.m_value;
			}
		}
		cerr << "oops! cannot find " << mulkku << ", setting to 0xDEADFACE\n";

		variables.push_back(variable_c(mulkku, 0xDEADFACE));
		return variables.back().m_value;
	}

	virtual string dump()
	{
		int i;
		string dump = "";
		for (i = 0; i < m_datapos; i++)
			dump += f("0x%04X: 0x%02X\n", i, data[i]);

		return dump;
	}

	virtual value_t get_pos()
	{
		return 0;
	}
};

};