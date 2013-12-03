#pragma once 
#include "debugf.h"

#include <vector>
#include <list>
#include <fstream>
#include <stdint.h>
#include <string.h>

using std::vector;
using std::list;
using std::string;

using std::ios;
using std::ofstream;

#include "datatypes.h"
#include "utils.h"

#include "segment_base.h"
#include "segment_basic.h"
#include "segment_asm.h"

namespace hashembler
{

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
		ofstream file(fn.c_str(), ios::binary);
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