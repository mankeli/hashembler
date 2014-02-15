#include "debugf.h"
#include "hashembler.h"

#include "math.h"

using namespace hashembler;

segment_basic_c basicstub;
segment_asm_c initprg;

typedef struct { int r,g,b; } col_s;
col_s c64pal[] = 
{
{0,0,0,},
{255,255,255,},
{136,57,50,},
{103,182,189,},
{139,63,150,},
{85,160,73,},
{64,49,141,},
{191,206,114,},
{139,84,41,},
{87,66,0,},
{184,105,98,},
{80,80,80,},
{120,120,120,},
{148,224,137,},
{120,105,196,},
{159,159,159,},
};

float coldist(int c1, int c2)
{
	float r,g,b;
	r = c64pal[c1].r - c64pal[c2].r;
	g = c64pal[c1].g - c64pal[c2].g;
	b = c64pal[c1].b - c64pal[c2].b;

//	r *= 0.21;
//	g *= 0.71;
//	b *= 0.07;

	r *= 0.2f;
	g *= 0.7f;
	b *= 0.07f;


	return sqrt(r*r+g*g+b*b);
}

void make_colortab()
{
	int i,j;
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			printf("% 4d  ", (int)coldist(i,j));
		}
		printf("\n");
	}

	initprg.align_to_page();
	§colortable:

	int col = 0, col_t = 1;
	int wrapsize = 2;

	for (i = 0; i < 256; i++)
	{
		float mindist = 777.f;
		int mindist_c = col;

		float ref_dist = coldist(col, col_t);

		for (j = 0; j < 16; j++)
		{
			float dist = coldist(col, j);
			float dist_t = coldist(col_t, j);

			if ((mindist - dist) > 10 && dist_t < ref_dist)
			{
				mindist = dist;
				mindist_c = j;
			}
		}

//		printf("%i: %i c%.2f   ", i, mindist_c, mindist);

		if ((i % wrapsize) == 0)
		{
			static int extra = 0;
			col = mindist_c;
			if (col == col_t)
				extra++;
			else
				extra = 0;

			if (extra == 2)
			{
				col_t = 0;
				col = 1;
				extra = 0;

				wrapsize = 6 - (coldist(col, col_t) / 600) * 5;


			}

		}

		//col = i & 1;

		initprg.add_byte(col);

	}
}

void create_nop_delay(int cycles)
{
	int nops = cycles / 2;
	if (cycles & 1)
	{
		nops--;
		bit.z 0xEA
	}

	int i;
	for (i = 0; i < nops; i++)
	{
		nop
	}
}

void genis(int pass)
{
	basicstub.begin(0x801, pass);
	basicstub.add_sys(666, §initprg.hello);
	basicstub.add_end();

	initprg.begin(§basicstub.*, pass);
	
§hello:
	lda #0x0
	sta 0xd011

	sei

§loop:


	inc §* + 4
	ldx #0x0
	ldy #0x0

§bottom:
	bit 0xd011
	bmi §bottom

§effu_loop:
	lda §colortable,x
	sta 0xd020

	create_nop_delay(45);
	inx
	iny
	beq §wait_for_9
	jmp §effu_loop

§wait_for_9:
	bit 0xd011
	bpl §wait_for_9
	lda #0x02
	sta 0xd020


	jmp §loop

	make_colortab();
}

int main()
{
	assemble(genis);

	list<segment_c *> segs;
	segs.push_back(&basicstub);
	segs.push_back(&initprg);

	make_prg("color_test.prg", 0x0801, segs);
}