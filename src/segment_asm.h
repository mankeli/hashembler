#pragma once

namespace hashembler
{

#include "opcodes.h"

class segment_asm_c: public segment_c
{
public:

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

		tmp += segment_c::dump();

		return tmp;
	}

	value_t get_pos()
	{
		return m_pc;
	}

};

};