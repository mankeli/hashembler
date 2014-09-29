#pragma once

namespace hashembler
{

class segment_basic_c: public segment_c
{


public:
	void add_sys(value_t linenumber, value_t addr)
	{
				/*
		 0801 1   2    3   4    5     6   7   8   9   a    b   c
		.byte $0c,$08, $0a,$00, $9e,  $20,$32,$33,$30,$34, $00,$00,$00

		080c = next line
		000a = line number
		9e = sys token?
		*/

		printf("address is %X\n", addr);

		value_t nextaddr = m_pc+0x0b;

		data[m_datapos + 0] = getbyte(nextaddr, 0);
		data[m_datapos + 1] = getbyte(nextaddr, 1);

		data[m_datapos + 2] = getbyte(linenumber, 0);
		data[m_datapos + 3] = getbyte(linenumber, 1);

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
};

};