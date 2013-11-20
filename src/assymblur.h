#pragma once 
/*

	variable:  - label
	.var variable = $02
	.var variable = $2302


*/

/*
	ei tarvita kickass tapaista addressingmodehommaa,
	koska varmaan voidaan makroihin laittaa se erikseenkin. sitäpaitsi se on muutenkin
	C-tulkin homma. (tai oikeestaan preprosessorin)
	mutta c-tulkin syntaksia sen pitäis olla


NÄMÄ PERVERSSIT AJATUKSET VAIN TULEVAT PÄÄHÄNI,
mutta entäs jos mitään kuvia yms ei edes lataa ensimmäisellä passilla?

*/

#include "debugf.h"

#include <vector>
#include <list>
#include <fstream>
#include <stdint.h>
#include <string.h>

#include "opcodes.h"


using std::vector;
using std::list;
using std::string;

using std::ios;
using std::ofstream;

typedef uint32_t value_t;

// 6502:lla oikeastaan vain yksi datatype
class variable_c
{
public:
	string m_name;
	value_t m_value;

	variable_c(string name, value_t value)
	{
		m_name = name;
		m_value = value;
	}
};

class statement_c
{
public:
	value_t m_pos;
	uint8_t m_opcode;
	//variable_c *m_operand;
	value_t m_operand;

	statement_c(value_t pos, uint8_t opcode,  value_t operand)
	{
		m_pos = pos;
		m_opcode = opcode;
		m_operand = operand;
	}

	void set_operand(value_t operand)
	{
		m_operand = operand;
	}
};

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
				return it.m_value;
		}
		cerr << "oops! cannot find " << mulkku << "\n";

/*
		static value_t placeholder_hack;
		return placeholder_hack;*/

		variables.push_back(variable_c(mulkku, 0));
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

static const uint8_t getbyte(value_t value, int num)
{
	return (value >> (num * 8)) & 0xFF;
}

class segment_basic_c: public segment_c
{
public:

	void begin(value_t pc, int pass)
	{
		segment_c::begin(pc, pass);
	}

	void add_sys(value_t linenumber, value_t addr)
	{
				/*
		 0801 1   2    3   4    5     6   7   8   9   a    b   c
		.byte $0c,$08, $0a,$00, $9e,  $20,$32,$33,$30,$34, $00,$00,$00

		080c = next line
		000a = line number
		9e = sys token?
		*/
		value_t nextaddr = m_pc+0x0b;

		data[m_datapos + 0] = getbyte(nextaddr, 0);
		data[m_datapos + 1] = getbyte(nextaddr, 1);

		data[m_datapos + 2] = getbyte(linenumber, 2);
		data[m_datapos + 3] = getbyte(linenumber, 3);

		data[m_datapos + 4] = 0x9e;

		data[m_datapos + 5] = 48 + (addr/10000) % 10;
		data[m_datapos + 6] = 48 + (addr/1000) % 10;
		data[m_datapos + 7] = 48 + (addr/100) % 10;
		data[m_datapos + 8] = 48 + (addr/10) % 10;
		data[m_datapos + 9] = 48 + (addr/1) % 10;

		data[m_datapos + 10] = 0;

		m_pc += 11;
		m_datapos += 11;
	}

	void add_end()
	{
		data[m_datapos + 0] = 0;
		data[m_datapos + 1] = 0;
		m_pc += 2;
		m_datapos += 2;
	}

	value_t get_pos()
	{
		return m_pc;
	}
};

class segment_asm_c: public segment_c
{
public:
	list<statement_c> statements;

	void add_statement(string opname, string adrname, value_t operand)
	{
		int opcode;
		for (opcode = 0; opcode < 256; opcode++)
		{
			if (instructions[opcode].name == opname && instructions[opcode].addrmodename == adrname)
				break;
		}

		if (opcode == 256)
		{
			cerr << f("what? opcode %s,%s doesn't exist\n", opname.c_str(), adrname.c_str());
			return;
		}

		//modelist = ['imp', 'imm', 'zp', 'izx', 'izy', 'zpx', 'zpy', 'rel', 'abs', 'abx', 'aby', 'ind' ]

		if (m_pass > 0 && data[m_datapos] != opcode)
		{
			cerr << f("hmm. opcode mismatch at pc 0x%04X\n", m_pc);
			return;
		}

		data[m_datapos] = opcode;
		if (adrname == "imm" || adrname == "zp" || adrname == "izx" || adrname == "izy" || adrname == "zpx" || adrname == "zpy")
		{
			data[m_datapos + 1] = operand & 0xFF;
		}
		else if (adrname == "abs" || adrname == "abx" || adrname == "aby" || adrname == "ind")
		{
			data[m_datapos + 1] = (operand >> 0) & 0xFF;
			data[m_datapos + 2] = (operand >> 8) & 0xFF;
		}
		else if (adrname == "rel")
		{
			int diff = operand - (m_pc + 2);

			if (diff > 127 || diff < -128)
			{
				cerr << f("branch är liian kaukana %04X to %04X\n", operand, m_pc);
			}

			if (diff < 0)
			{
				diff = 0x100+diff;
			}
			data[m_datapos + 1] = diff & 0xFF;
		}

		m_pc += modesizes[instructions[opcode].addrmode];
		m_datapos += modesizes[instructions[opcode].addrmode];
	}


	string dump()
	{
		string tmp;
		tmp += "labels:\n";

		for (auto &it: variables)
		{
			tmp+=f("variable: %s 0x%04X\n", it.m_name.c_str(), it.m_value);
		}

		tmp += "statements:\n";

		for (auto &it: statements)
		{
			tmp+=f("op: %02X, operand %08X\n", it.m_opcode, it.m_operand);
		}

		tmp += segment_c::dump();

		return tmp;
	}

	value_t get_pos()
	{
		return m_pc;
	}

};

class assymblur
{
public:
	static void assemble(void (*func)(int))
	{
		cerr << "assembling..\n";
		int i;
		for (i = 0; i < 2; i++)
		{
			cerr << f("\nnow entering pass %i\n", i);;
			func(i);
			cerr << "\n";
		}
	}

	static void make_prg(string fn, value_t loadaddr, list<segment_c *> segs)
	{
		ofstream file(fn, ios::binary);
		value_t curpc = loadaddr;

		// detect segment overlaps more wisely

		cerr << "writing results to " << fn << "\n";

		if (file.is_open())
		{
			uint8_t byte;
			byte = (loadaddr >> 0) & 0xFF;
			file.write((const char *)&byte, 1);
			byte = (loadaddr >> 8) & 0xFF;
			file.write((const char *)&byte, 1);

			while(1)
			{
				value_t lowest_startpc = 0xFFFFFFFF;
				segment_c *lowest_seg = NULL;

				cerr << f("we are at %X. finding next seg\n", curpc);
				for (auto &it: segs)
				{
					if (it)
					{
						if (it->m_startpc < lowest_startpc)
						{
							lowest_startpc = it->m_startpc;
							lowest_seg = it;
						}
					}
				}

				//cerr << f("next seg: %p\n", lowest_seg);

				if (!lowest_seg)
					break;

				if (lowest_startpc == curpc)
				{
					cerr << f("writing %i bytes\n", lowest_seg->m_datapos);
					file.write((const char *)lowest_seg->data.data(), lowest_seg->m_datapos);
					curpc += lowest_seg->m_datapos;

					for (auto &it: segs)
					{
						if (it == lowest_seg)
							it = NULL;
					}
				}
				else if (lowest_startpc > curpc)
				{
					byte = 0xDA;
					for (;curpc < lowest_startpc; curpc++)
					{
						file.write((const char *)&byte, 1);
					}
				}
				else
				{
					cerr << f("uh oh. segment overlap at pos 0x%X (lowest segment is 0x%X)\n", curpc, lowest_startpc);
					break;
				}

			}
			cerr << f("writing ended at %X\n", curpc);
			file.close();
		}
	}
};