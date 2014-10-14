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

	virtual void begin(value_t pc)
	{
		m_startpc = pc;
		m_pc = pc;
		m_datapos = 0;

//		void set_segment(segment_c *seg);

		set_segment(this);

		// TODO: make this change size in a better way
		data.resize(65536);
	}

	value_t &get_variable_idx(string mulkku, int extra)
	{
		char tmpstr[1024];
		sprintf(tmpstr,"%s%i", mulkku.c_str(), extra);
		return get_variable(tmpstr);
	}

	value_t &get_variable(string mulkku)
	{
		for (auto &it: variables)
		{
			if (it.m_name == mulkku)
			{
				// cerr << f("var '%s' = 0x%08X", it.m_name.c_str(), it.m_value) << "\n";
				if (it.m_value == 0xDEADFACE)
					g_pass_vars_left++;
				return it.m_value;
			}
		}

//		if (g_pass == 1)
//			cerr << "oops! cannot find " << mulkku << ", defaulting to 0xDEADFACE\n";
		g_pass_vars_left++;

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
		return m_pc;
	}

	virtual value_t get_size()
	{
		return m_datapos;
	}

	virtual void add_statement(string opname, string adrname, value_t operand)
	{
		printf("i'm sorry but this doesn't work. sorry also that i'm so hackish :-D\n");
	}

	void add_byte(uint8_t byte)
	{
		data[m_datapos] = byte;

		m_datapos++;
		m_pc++;
	}

	void add_string(const char *str)
	{
		int len = strlen(str);
		int i;
		for (i = 0; i < len; i++)
		{
			data[m_datapos] = str[i];
			m_datapos++;
			m_pc++;
		}
	}

	void align_to_page()
	{
		int bytesleft = 0x100 - (m_pc & 0xFF);
		m_datapos += bytesleft;
		m_pc += bytesleft;
	}

	void reserve(int amount)
	{
		m_datapos += amount;
		m_pc += amount;
	}


	bool load_prg(string fn, bool beginnew)
	{
		FILE *fp = fopen(fn.c_str(), "rb");
		if (!fp)
			return true;
		uint16_t load_addr;
		fread(&load_addr, 2, 1, fp);
		// cerr << f("load address: %04X\n", load_addr);
		if (beginnew)
		{
			begin(load_addr);
			get_variable("start") = load_addr;
		}

		get_variable(fn) = m_pc;

		while(!feof(fp))
		{
			add_byte(fgetc(fp));
		}
		fclose(fp);
		return false;
	}
};

};