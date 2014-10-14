#include "debugf.h"
#include "hashembler.h"

#include <math.h>

using namespace hashembler;

// HCL's sideborder example ported to Hashembler

segment_basic_c basicstub;
segment_asm_c initprg;

void genis()
{
	basicstub.begin(0x801);
	basicstub.add_sys(666, SEGLABEL(initprg, "begin"));
	basicstub.add_end();

	initprg.begin(SEGPC(basicstub));
	set_segment(&initprg);

	LPC("begin");
	SEIa();
	LPC("loop1");
	LDAi(0x1b);
	STA(0xd011);

	LDAi(0x3d);
	LPC("rpl1")
	CMP(0xd012);
	BNE(L("rpl1"));

	LDA(0xd012);
	ANDi(0x07);
	ORAi(0x18);
	STA(0xd011);

	// waitings
	NOPa();
	NOPa();
	NOPa();
	NOPa();
	NOPa();
//	BIT(0xea);
	BITz(0xea);

	LDXi(0);
	LPC("loop2");
	CLCa();
	LDA(0xd012);
	ADCi(0x4);
	ANDi(0x7);
	ORAi(0x18);
	STA(0xd011);

	// 2 + (2 + 3) * 4 + (2 + 2) + 2
/*	LDYi(5);
	LPC("nopwait");
	DEYa();
	BNE(L("nopwait"));
	NOPa();*/


	int i;
	for (i = 0; i < 28; i+=2)
	{
		NOPa();
	}
//	INC(0xd021);

	DEC(0xd016);
	INC(0xd016);

	INXa();
	CPXi(0x80);
	BCC(L("loop2"));

	LDA(0xd012);
	ANDi(0x07);
	ORAi(0x18);
	STA(0xd011);

	JMP(L("loop1"));
}
int main(int argc, char **argv)
{
	assemble(genis);

	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&initprg);

	make_prg("sideborder.prg", 0x0801, segs);

	return 0;
}
