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
	CTX("hello");
	// assemble the basic stub
	basicstub.begin(0x801);
	basicstub.add_sys(666, L("begin1"));
	basicstub.add_sys(667, L("begin2"));
	basicstub.add_end();

	// assemble program
	initprg.begin(SEGPC(basicstub));
	set_segment(&initprg);

	LPC("teksti1")
	const char *hellotext1 = "HELLO ";
	SEG->add_string(hellotext1);

	LPC("teksti2")
	const char *hellotext2 = "KITTY";
	SEG->add_string(hellotext2);


	LPC("begin1");
	// output cool text using kernal routines
	LDXi(3);
	JSR(0xFFC9);

	LDXi(0);
	LPC("writeloop");
	LDAx(L("teksti1"));
	JSR(0xFFD2);
	INX();
	CPXi(strlen(hellotext1));
	BNE(L("writeloop"));

	RTS();



	LPC("begin2");
	// output cool text using kernal routines
	LDXi(3);
	JSR(0xFFC9);

	LDXi(0);
	LPC("writeloop");
	LDAx(L("teksti2"));
	JSR(0xFFD2);
	INX();
	CPXi(strlen(hellotext2));
	BNE(L("writeloop"));

	RTS();
}

