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

// maybe these should be functions instead?
#define SETSEG(_s) { set_segment(&_s); }
#define SEG (__current_seg)

#define PC() (__current_seg->m_pc)

#define SEGLABEL(_s,_n) (_s.get_variable(_n))
#define SEGPC(_s) (_s.m_pc)
#define B(_n) {__current_seg->add_byte(_n); }
#define W(_n) {__current_seg->add_byte(LB(_n)); __current_seg->add_byte(HB(_n)); }
#define PAGE {__current_seg->align_to_page(); }
#define PRINTPOS(_txt, ...) { printf(_txt": %04X\n", ##__VA_ARGS__, PC()); }

namespace hashembler
{
	int g_pass;
	int g_pass_vars_left;

class segment_c;

segment_c *__current_seg = NULL;
static void set_segment(segment_c *seg)
{
	__current_seg = seg;
}

static void stopassembly(const char *reason)
{
	printf("FATAL: %s\n", reason);
	fflush(stdout);
	exit(1);
}

};

#include "segment_base.h"
#include "segment_basic.h"
#include "segment_asm.h"

#include "context.h"

namespace hashembler
{
static void assemble(void (*func)(void))
{
	cerr << "assemble(): Starting assembly..\n";
	int i;
	for (i = 0;; i++)
	{
		cerr << f(" - now entering pass %i - \n", i);;
		g_pass = i;
		g_pass_vars_left = 0;
		srand(0);
		func();
	//	cerr << "\n";
		cerr << f("issues left: %i\n", g_pass_vars_left);
		if (g_pass_vars_left == 0)
		{
			cerr << "assemble(): Assembly finished..\n";
			break;
		}
	}
}


static void make_prg(string fn, value_t loadaddr, list<segment_c *> segs)
{
	ofstream file(fn.c_str(), ios::binary);
	value_t curpc = loadaddr;

	// TODO: detect segment overlaps more wisely

	cerr << "make_prg(): writing .prg to " << fn << "\n";
	cerr << f("  writing begins at %X\n", curpc);

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

			// cerr << f("we are at %X. finding next seg\n", curpc);
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
				// cerr << f("writing %i bytes\n", lowest_seg->m_datapos);
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
				cerr << f("  uh oh. segment overlap at pos 0x%X (lowest segment is 0x%X)\n", curpc, lowest_startpc);
				break;
			}

		}
		cerr << f("  writing ended at %X\n", curpc);
		file.close();
	}
}


static void make_mprg(string fn, list<segment_c *> segs)
{
	ofstream file(fn.c_str(), ios::binary);

	// TODO: detect segment overlaps!

	cerr << "writing .mprg to " << fn << "\n";

	if (file.is_open())
	{
		uint8_t byte;

		for (auto &it: segs)
		{
			if (it)
			{
				value_t loadaddr = it->m_startpc;
				value_t length = it->m_datapos;

				cerr << f("  writing seg: 0x%04X bytes to 0x%04X-0x%04X\n", length, loadaddr, loadaddr+length-1);

				byte = (length >> 0) & 0xFF;
				file.write((const char *)&byte, 1);
				byte = (length >> 8) & 0xFF;
				file.write((const char *)&byte, 1);


				byte = (loadaddr >> 0) & 0xFF;
				file.write((const char *)&byte, 1);
				byte = (loadaddr >> 8) & 0xFF;
				file.write((const char *)&byte, 1);



				file.write((const char *)it->data.data(), length);
			}
		}

		byte = 0;
		file.write((const char *)&byte, 1);
		file.write((const char *)&byte, 1);


		file.close();
	}
}


};
