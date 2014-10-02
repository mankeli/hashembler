#include "debugf.h"
#include "hashembler.h"

using namespace hashembler;

// define segments 
segment_basic_c basicstub;
segment_asm_c initprg;

void genis();

int main(int argc, char **argv)
{
	// this invokes the hashembler. genis() function is called twice in current version.
	assemble(genis);

	// list of segments to be included
	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&initprg);

	// write .prg !
	make_prg("hello.prg", 0x0801, segs);

	return 0;
}


void genis()
{
	// assemble the basic stub
	basicstub.begin(0x801);
	basicstub.add_sys(666, SEGLABEL(initprg, "begin"));
	basicstub.add_end();

	// assemble program
	initprg.begin(SEGPC(basicstub));
	set_segment(&initprg);

	LPC("teksti")
	const char *hellotext = "I HOPE YOU DIE :)";
	SEG.add_string(hellotext);

	LPC("begin");

	// output cool text using kernal routines
	LDXi(3);
	JSR(0xFFC9);
	LDXi(0);
	LPC("writeloop");
	LDAx(L("teksti"));
	JSR(0xFFD2);
	INXa();
	CPXi(strlen(hellotext));
	BNE(L("writeloop"));

	RTSa();
}

