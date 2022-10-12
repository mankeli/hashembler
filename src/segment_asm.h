#pragma once

namespace hashembler
{

#include "opcodes.h"
#include "opcodemacros.h"

class segment_asm_c: public segment_c
{
public:

	void add_op(int opcode, value_t operand, int mode)
	{
		if (g_pass > 0 && data[m_datapos] != opcode)
		{
			cerr << f("hmm. opcode mismatch at pc 0x%04X\n", m_pc);
			exit(1);
			return;
		}

		add_byte(opcode);

		if (mode == 1)
		{
			add_byte(operand);
		}
		else if (mode == 2)
		{
			add_byte(LB(operand));
			add_byte(HB(operand));
		}
		else if (mode == 3)
		{
			int diff = operand - (m_pc + 1);
			if (diff > 127 || diff < -128)
			{
				cerr << f("branch is too far. %04X to %04X\n", operand, m_pc);
				g_pass_vars_left++;
				diff = 0;
			}

			if (diff < 0)
			{
				diff = 0x100 + diff;
			}
			add_byte(diff & 0xFF);
		}
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

};

};