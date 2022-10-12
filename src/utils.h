#pragma once

namespace hashembler
{
static const uint8_t getbyte(value_t value, int num)
{
	return (value >> (num * 8)) & 0xFF;
}

static const uint8_t LB(value_t value)
{
	return (value >> (0)) & 0xFF;
}

static const uint8_t HB(value_t value)
{
	return (value >> (8)) & 0xFF;
}

void static_hack()
{
	getbyte(0,0);
	LB(0);
	HB(0);
}


static char encoder_scr(char byte) {
		if((byte >= 'a') && (byte <= 'z'))
				return((char)(byte - 96));      // shift lowercase down
		if((byte >= 'A') && (byte <= 'Z'))
				return((char)(byte - 64));      // shift uppercase down
		if((byte >= '0') && (byte <= '9'))
				return((char)(byte));      // numbers
		if((byte >= '[') && (byte <= '_'))
				return((char)(byte - 64));      // shift [\]^_ down
		if(byte == '`')
				return(64);     // shift ` down
		if(byte == '@')
				return(0);      // shift @ down
		return(byte);
}

static char encoder_scr2(char byte) {
		if((byte >= 'a') && (byte <= 'z'))
				return((char)(byte - 96));      // shift lowercase down
		if((byte >= 'A') && (byte <= 'Z'))
				return((char)(byte));      // dontshift uppercase down
		if((byte >= '0') && (byte <= '9'))
				return((char)(byte));      // numbers
		if((byte >= '[') && (byte <= '_'))
				return((char)(byte - 64));      // shift [\]^_ down
		if(byte == '`')
				return(64);     // shift ` down
		if(byte > 0x20 && byte <= 0x3f)
				return(byte);
		if(byte == '@')
				return(0);      // shift @ down
		return(byte);
}

}