#include "debugf.h"
#include "hashembler.h"

#include "math.h"

using namespace hashembler;

segment_basic_c basicstub;
segment_asm_c initprg;

void MOV8i(value_t outaddr, value_t inval)
{
	LDAi(inval);
	STA(outaddr);
}

void MOV16(value_t outaddr, value_t inaddr)
{
	LDA(inaddr + 0);
	STA(outaddr + 0);
	LDA(inaddr + 1);
	STA(outaddr + 1);
}
void MOV16i(value_t outaddr, value_t inval)
{
	LDAi(LB(inval));
	STA(outaddr + 0);
	LDAi(HB(inval));
	STA(outaddr + 1);
}

void killirq()
{
	LDAi(0x7f)
	STA(0xdc0d)
	STA(0xdd0d)
	LDA(0xdc0d)
	LDA(0xdd0d)
}

void write_colortab(int *t1, int *t2, int numcol)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		float intensity = (float)i / 255.f;
		int num1 = intensity * (numcol - 2);
		int num2 = num1;

		int colmix;
		float dither = 0.5f;
		dither = (rand() & 65535) / 65535.f;
		if (intensity < dither)
			colmix = t1[num1] | (t2[num1] << 4);
		else
			colmix = t1[num2] | (t2[num2] << 4);
		
		B(colmix)
	}
}

void makeroto()
{
	int x,y;
//	LDAi(0x8);
	LDAz(0x30);
	ADCi(0x80);
	STAz(0x10);
//	LDAz(0x0a);
//	STAz(0x18);
//	STAz(0x28);
	for (y = 0; y < 25; y++)
	{
		LDAz(0x10);
		CLCa();
		ADCz(0x18);
		STAz(0x10);
		STAz(0x20);
		TAXa();

		for (x = 0; x < 40; x++)
		{
			CLCa()
			ADCz(0x28);
			TAXa();
			LDYx(L("ct1"))

//			LDYi(x);
			STY(0x400+y*40+x)
//			STY(0xD800+y*40+x)

		}

#if 0
		for (x = 0; x < 40; x++)
		{
#if 0
			if ((x & 3) == 0)
			{
				PHAa();
				ANDi(0x07);
				STA(0xd016);
				PLAa();
			}
#endif
			CLCa()
			ADCz(0x28);
			TAXa();
			LDYx(L("ct1"))

//			LDYi(x);
			STY(0x400+y*40+x)
//			STY(0xD800+y*40+x)

		}
#endif
	}
}

void maketable(const char *label, int ar[], int num)
{
	LPC(label);
	int i;
	for (i = 0; i < num; i++)
	{
		B(ar[i]);
	}
}

void irqroutine()
{
	LPC("irqrut");
	LSR(0xd019);
	INC(0xd020);

	LDAz(0x0A);
	CLCa();
	ADCi(0x01);
	STAz(0x0A);
	PHAa();

	LDAz(0x0B);
	CLCa();
	ADCi(0x05);
	STAz(0x0B);

	TAXa();
	LDAx(L("sinex"))
	STAz(0x18);
	LDAx(L("siney"))
	STAz(0x28);

	LDAx(L("bsinex"))
	CLCa();
	ADCx(L("bsiney"))
	STAz(0x30);

	PLAa(); TAXa();
#if 0
	LDAx(L("sinex2"))
	CLCa();
	ADCz(0x18);
	STAz(0x18);
	LDAx(L("siney2"))
	CLCa();
	ADCz(0x28);
	STAz(0x28);
#endif
#if 0
	LDAx(L("bsinex2"))
	CLCa();
	ADCx(L("bsiney2"))
	CLCa();
	ADCz(0x0B);
	STAz(0x0B);
#endif
	makeroto();

	DEC(0xd020);

	RTIa();

	int i;
	int xsine[256];
	int ysine[256];
	int bxsine[256];
	int bysine[256];
	for (i = 0; i < 256; i++)
	{
//		xsine[i] = cos(i * (M_PI / 512.f)) * 7;
//		ysine[i] = sin(i * (M_PI / 512.f)) * 5;
		xsine[i] = cos(i * (M_PI / 128.f)) * 7;
		ysine[i] = sin(i * (M_PI / 128.f)) * 5;
		bxsine[i] = 256 - xsine[i] * 12;
		bysine[i] = 256 - ysine[i] * 20;
	}

	maketable("sinex", xsine, 256);
	maketable("siney", ysine, 256);
	maketable("bsinex", bxsine, 256);
	maketable("bsiney", bysine, 256);

	LPC("keptab")
	for (i = 0; i < 256; i++)
	{
		B((i + 0x28))
	}
}


void genis()
{
	basicstub.begin(0x801);
	basicstub.add_sys(666, SEGLABEL(initprg, "begin"));
	basicstub.add_end();

	initprg.begin(SEGPC(basicstub));
	set_segment(&initprg);

	irqroutine();
	LPC("begin");

	SEIa();
	killirq();
	LDAi(0x35);
	STAz(1);

	MOV16i(0xFFFE, L("irqrut"));

	MOV8i(0xD01A, 0x01);
	MOV8i(0xD012, 0x00);
	MOV8i(0xD011, 0xBB);



	LDXi(0);
	LPC("copyloop");
	int i;
	for (i = 0; i < 4; i++)
	{
		LDAx(0x400 + i * 0x100);
		EORi(0x80);
		STAx(0x400 + i * 0x100);
	}
	INXa();
	BNE(L("copyloop"));



	CLIa();

	LPC("loop")
//	INC(0xD020)

	JMP(L("loop"))

	int cl1[] = {0, 0,  0,  0,  0,  0,  0,  0};
	int cl2[] = {0, 0,  6,  11, 2,  11, 6,  0};
	int cl3[] = {0, 6,  14, 12, 10, 12, 14, 6};
	int cl4[] = {6, 14, 3,  7,  1,  7,  3,  14};

	LPC("ct1");
	write_colortab(cl3, cl4, 8);
/*
	MOV16(L("jumpmod"), 0x314);
	MOV16i(0x314, L("irqrut"));

	MOV8i(0xD01A, 0x01);
	MOV8i(0xD012, 0x20);
	MOV8i(0xD011, 0x9B);
	CLIa();
*/
}

int main(int argc, char **argv)
{
	assemble(genis);

	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&initprg);

	make_prg("maakallo.prg", 0x0801, segs);

	return 0;
}