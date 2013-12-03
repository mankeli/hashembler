#include "debugf.h"
#include "hashembler.h"

using namespace hashembler;

segment_basic_c basicstub;
segment_asm_c mainprg;

void hook_to_irq()
{
	inc 0xd020

	lda 0x314
	sta §jmp_mod + 0
	lda 0x315
	sta §jmp_mod + 1

	lda #((§irq_routine >> 0) & 0xFF)
	sta 0x314
	lda #((§irq_routine >> 8) & 0xFF)
	sta 0x315


	rts
}

void gen_irq_routine()
{
	§irq_routine:
	
	§top_wait_loop:
	bit 0xd011
	bpl §top_wait_loop

	§bottom_wait_loop:
	bit 0xd011
	bmi §bottom_wait_loop

	§xscroll = §* + 1;
	ldx #0x08
	dex
	bmi §move_datas

	stx §xscroll
	txa
	and #0x10
	sta 0xd011


	jmp §go_back

§move_datas:
	jmp §go_back

	ldx #0x08
	stx §xscroll
	txa
	and #0x10
	sta 0xd011

	inc 0xd020

	int x,y;
	for (y = 0; y < 25; y++)
	{
		for (x = 1; x < 41; x++)
		{
			if (x == 40)
				lda 0x400 + y*40+0
			else
				lda 0x400 + y*40+x

			sta 0x400 + y*40+x-1
		}
	}

	dec 0xd020

§go_back:
	§jmp_mod = §* + 1;
	jmp 0x1000
}

void genis(int pass)
{
	basicstub.begin(0x801, pass);

	basicstub.add_sys(666, §program_start);
	basicstub.add_end();

	mainprg.begin(0x1000, pass);

	§program_start = §*;
	hook_to_irq();
	gen_irq_routine();
}

int main()
{
	assemble(genis);

/*
	cerr << "\n\nlets hear the results:\n";
	cerr << "basicstub: \n" << basicstub.dump();
	cerr << "\n";
	cerr << "mainprg: \n" << mainprg.dump();
*/
	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&mainprg);

	make_prg("border_test.prg", 0x0801, segs);
}