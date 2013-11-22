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

}