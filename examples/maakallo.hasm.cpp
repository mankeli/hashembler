#include "debugf.h"
#include "hashembler.h"

#include <math.h>

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

void to_hires(const char *fn)
{
	int w,h,n;
//	unsigned char *data = stbi_load(filename, &w, &h, &n, 0);
}

static float fracf(float val)
{
	return val - floor(val);
}

void write_colortab(int *t1, int *t2, int numcol)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		float intensity = (float)i / 255.f;
		float fnum = intensity * (numcol - 1.01f);
		int num1 = intensity * (numcol - 1.01);
		int num2 = num1 + 1;

		int colmix;
		float dither = 0.5f;
		dither = (rand() & 65535) / 65535.f;
//		printf("%i: %f (%i-%i)\n", i, fracf(fnum), num1, num2);
		if (fracf(fnum) < dither)
			colmix = t1[num1] | (t2[num1] << 4);
		else
			colmix = t1[num2] | (t2[num2] << 4);
		
		B(colmix)
	}
}

int inderp(float pos, int base, int nitems, int mmult)
{
	int i;
	if (pos < 0.f)
		pos = 0.f;
	else if (pos > 1.0f)
		pos = 1.0f;
	float fnum = pos * (nitems - 1.01f);
	int num1 = base + fnum;
	int num2 = num1 + 1;

	float dither = 0.5f;
	dither = (rand() & 65535) / 65535.f;
	if (fracf(fnum) < dither)
		return num1 * mmult;
	else
		return num2 * mmult;
}

value_t cursinusx = 0x13;
value_t cursinusy = 0x14;
value_t cursinusbx = 0x15;
value_t cursinusby = 0x16;

value_t yaccu = 0x17;
value_t xaccu = 0x18;

void makeroto()
{
	int x,y;
//	LDAi(0x8);
	LDAz(cursinusbx);
	CLC();
	ADCi(cursinusby);
	STAz(yaccu);
//	LDAz(0x0a);
//	STAz(0x18);
//	STAz(0x28);

	int ctbuf[40*25];
	for (y = 0; y < 25; y++)
	{
		for (x = 0; x < 40; x++)
		{
			//printf("%i,%i\n",x,y);
			int col1 = -1, col2 = -1, col3 = -1;
			if (y > 0)
				col1 = ctbuf[(y-1) * 40 + x];
			if (x > 0 && y > 0)
				col2 = ctbuf[(y-1) * 40 + (x-1)];
			if (x > 0)
				col3 = ctbuf[y * 40 + (x-1)];

			int col;
			do
			{
				col = rand() & 3;
			} while(col == col1 || col == col2 || col == col3);

			ctbuf[y * 40 + x] = col;
		}
	}


	for (y = 0; y < 25; y++)
	{
#if 1
		EOR(0xd012);
		ANDi(0x07);
		STA(0xd016);

		LDA(0xd018);
		EORi(0x08);
		STA(0xd018);
		//INC(0xd020);
#endif

		LDAz(yaccu);
		CLC();
		ADCz(cursinusy);
		STAz(yaccu);

		for (x = 0; x < 40; x++)
		{
//			CLC()
#if 1
			if ((x % 10) == 9)
			{
				STAz(xaccu); // 3
				EOR(0xd012); // 2
				ANDi(0x07); // 2
				STA(0xd016); // 4
				LDA(0xd018); // 4
				EOR(0x08); // 3
				STA(0xd018); // 4

				//INC(0xd020);
				LDAz(xaccu); // 3
				// total = 25

			}
#endif

			ADCz(cursinusx);

			int ctnum = ctbuf[y * 40 + x];

			float dx = x - 20;
			float dy = (y - 12) * 1.5f;
			int d = sqrt(dx*dx + dy*dy);

			float mug = d / 15.f - 0.4f;

			int cttab = inderp(mug, 0, 4, 1);

			if (cttab >= 0 && cttab <= 2)
			{
				ctnum += (2 - cttab) * 4;
				TAX();
				LDYx(L("cta1") + ctnum * 0x100);
			}
			else
				LDYi(0);

			STY(0x0400+y*40+x)
	//		STY(0xD800+y*40+x)

		}
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

void sixteenbitlut(const char *label, value_t andval, value_t tmppos, value_t target)
{
	value_t sblut = ZPS("sblut", 2);
	ANDi(andval);
	CLC();
	ADCi(HB(L(label)))
	STAz(sblut + 1);
	LDAz(tmppos);
	STAz(sblut + 0)
	LDYi(0);
	LDAizy(sblut)
	STAz(target);

}

void irqroutine()
{
	LPC("irqrut");
	PRINTPOS("irqrut");
	LSR(0xd019);
	//DEC(0xd020);

	value_t angle = ZPS("angle", 2);
	value_t angle2 = ZPS("angle2", 2);
/*	LDAz(angle + 0);
	CLC();
	ADCi(0x01);*/
	LDAz(angle2 + 1)
	EORz(angle2 + 0)
	ANDi(0x03);
	ORAi(0x01);
	CLC();
	ADCz(angle + 0);
	STAz(angle2 + 0);

	STAz(angle + 0);

	LDAz(angle + 1);
	ADCi(0x00);
	STAz(angle + 1);
	PHA();
	PHA();

	LDAz(angle + 1)
	EORz(angle + 0)
	ANDi(0x03);
	ORAi(0x01);
	CLC();
	ADCz(angle2 + 0);
	STAz(angle2 + 0);

	LDAz(angle2 + 1);
	ADCi(0x00);
	STAz(angle2 + 1);


	PHA();

	sixteenbitlut("sinex", 0x03, angle2, cursinusx);
	PLA();
	sixteenbitlut("bsinex", 0x03, angle2, cursinusbx);

	PLA();
	sixteenbitlut("siney", 0x03, angle, cursinusy);
	PLA();
	sixteenbitlut("bsiney", 0x03, angle, cursinusby);

	//DEC(0xd020);

	PRINTPOS("rotostart");

	makeroto();

	PRINTPOS("rotoend");


	//INC(0xd020);
	//INC(0xd020);

	RTI();

	int i;
	#define SINELEN (1024)
	int xsine[SINELEN];
	int ysine[SINELEN];
	int bxsine[SINELEN];
	int bysine[SINELEN];
	for (i = 0; i < SINELEN; i++)
	{
//		xsine[i] = cos(i * (M_PI / 512.f)) * 7;
//		ysine[i] = sin(i * (M_PI / 512.f)) * 5;
		float sx = 12.0f;
		float sy = 12.0f;

		sx += cos(i * (2.0f * M_PI / SINELEN) + 2.0f) * 4.6f;
		sx += cos(i * (-4.0f * M_PI / SINELEN) + 2.5f) * 3.9f;
		sx += cos(i * (-8.0f * M_PI / SINELEN) + 1.7f) * 2.3f;
		sx += cos(i * (16.0f * M_PI / SINELEN) + 2.7f) * 1.3f;
		sx += cos(i * (32.0f * M_PI / SINELEN) + 5.43f) * 3.3f;

		sy += sin(i * (2.0f * M_PI / SINELEN) + 2.0f) * 4.f;
		sy += sin(i * (-4.0f * M_PI / SINELEN) + 2.5f) * 3.9f;
		sy += sin(i * (8.0f * M_PI / SINELEN) + 0.43f) * 2.7f;
		sy += sin(i * (16.0f * M_PI / SINELEN) + 4.43f) * 1.7f;
		sy += sin(i * (32.0f * M_PI / SINELEN) + 5.43f) * 3.3f;

		float px = (round(sy * 3.f + 0.f));
		float py = (round(sx * 3.f + 0.f));



		xsine[i] = px;
		ysine[i] = py;
		bxsine[i] = 256.0f - px * 20;
		bysine[i] = 256.0f - py * 12;
	}

	PAGE
	PRINTPOS("sine alignment");
	value_t tablestart = PC();
	maketable("sinex", xsine, SINELEN);
	maketable("siney", ysine, SINELEN);
	maketable("bsinex", bxsine, SINELEN);
	maketable("bsiney", bysine, SINELEN);
	PRINTPOS("sine end");
	printf("sinetables size: %i bytes\n", PC() - tablestart);

	LPC("keptab")
	for (i = 0; i < 256; i++)
	{
		B((i + 0x28))
	}
}


void genis()
{
	CTX("main")
	RESERVE(0x00);
	RESERVE(0x01);


	basicstub.begin(0x801);
	basicstub.add_sys(666, L("begin"));
	basicstub.add_end();

	initprg.begin(SEGPC(basicstub));
	set_segment(&initprg);

	irqroutine();
	LPC("begin");
	PRINTPOS("pc at begin");

	SEI();
	killirq();
	LDAi(0x35);
	STAz(1);

	MOV16i(0xFFFE, L("irqrut"));

	MOV8i(0xD01A, 0x01);
	MOV8i(0xD012, 0x20);
	MOV8i(0xD011, 0x3B);
//	MOV8i(0xD011, 0x1B);

	MOV8i(0xD018, 0x18);
	MOV8i(0xD020, 0x00);


	LDXi(0);
	LPC("copyloop");
	int i;
	for (i = 0; i < 4; i++)
	{
		LDAx(0x400 + i * 0x100);
		EORi(0x80);
		STAx(0x400 + i * 0x100);
	}
	INX();
	BNE(L("copyloop"));



	CLI();

	printf("pc now: %04X\n", PC());

	LPC("loop")
//	INC(0xD020)

	JMP(L("loop"))

	int cl1[] = {0, 0,  0,  0,  0,  0,  0,  0, 0};
	int cl2[] = {0, 0,  6,  11, 2,  11, 6,  0, 0};
	int cl3[] = {0, 6,  14, 12, 10, 12, 14, 6, 0};
	int cl4[] = {6, 14, 3,  7,  1,  7,  3,  14, 6};

	PAGE
	LPC("cta1");
	write_colortab(cl1, cl2, 9);
	LPC("cta2");
	write_colortab(cl1, cl2, 9);
	LPC("cta3");
	write_colortab(cl1, cl2, 9);
	LPC("cta4");
	write_colortab(cl1, cl2, 9);
	LPC("ctb1");
	write_colortab(cl2, cl3, 9);
	LPC("ctb2");
	write_colortab(cl2, cl3, 9);
	LPC("ctb3");
	write_colortab(cl2, cl3, 9);
	LPC("ctb4");
	write_colortab(cl2, cl3, 9);
	LPC("ctc1");
	write_colortab(cl3, cl4, 9);
	LPC("ctc2");
	write_colortab(cl3, cl4, 9);
	LPC("ctc3");
	write_colortab(cl3, cl4, 9);
	LPC("ctc4");
	write_colortab(cl3, cl4, 9);


/*
	MOV16(L("jumpmod"), 0x314);
	MOV16i(0x314, L("irqrut"));

	MOV8i(0xD01A, 0x01);
	MOV8i(0xD012, 0x20);
	MOV8i(0xD011, 0x9B);
	CLI();
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
