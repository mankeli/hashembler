#include "debugf.h"
#include "hashembler.h"

using namespace hashembler;

segment_basic_c basicstub;
segment_asm_c initprg;
segment_asm_c mainprg;

void hook_to_irq()
{
	§program_start:

	lda 0x314
	sta §mainprg.jmp_mod + 0
	lda 0x315
	sta §mainprg.jmp_mod + 1

	lda #((§mainprg.irq_routine >> 0) & 0xFF)
	sta 0x314
	lda #((§mainprg.irq_routine >> 8) & 0xFF)
	sta 0x315

	rts
}

void gen_irq_routine()
{
	§irq_routine:

	§prescaler_1 = §* + 1;
	lda #0x00
	beq §pos1
	dec §prescaler_1
	jmp §go_back
§pos1:

	§prescaler_2 = §* + 1;
	lda #0x20
	sta §prescaler_1
	beq §pos2

	dec §prescaler_2
§pos2:



	§top_wait_loop:
	bit 0xd011
	bpl §top_wait_loop

	§bottom_wait_loop:
	bit 0xd011
	bmi §bottom_wait_loop

	§xscroll = §* + 1;
	ldx #0x08
	dex
	stx §xscroll
	bmi §move_datas

	txa
	sta 0xd016

	jmp §go_back

§move_datas:

	ldx #0x07
	stx §xscroll
	stx 0xd016

	int x,y;
	for (y = 0; y < 25; y++)
	{
		for (x = 0; x < 40; x++)
		{
			lda 0x400 + y*40+x

			if (x == 0)
				sta 0x400 + y*40+39
			else
				sta 0x400 + y*40+x-1
		}
	}

	dec.z 0xD3
	bpl §no_cursor_wrap
	lda #39
	sta.z 0xD3

§no_cursor_wrap:

§go_back:
	§jmp_mod = §* + 1;
	jmp 0x1000
}

void genis(int pass)
{
	basicstub.begin(0x801, pass);
	basicstub.add_sys(666, §initprg.program_start);
	basicstub.add_end();

	initprg.begin(0x1000, pass);
	hook_to_irq();

	mainprg.begin(0x1050, pass);
	gen_irq_routine();
}

int main()
{
	assemble(genis);

	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&initprg);
	segs.push_back(&mainprg);

	make_prg("border_test.prg", 0x0801, segs);
}