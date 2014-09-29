#pragma once

typedef uint32_t value_t;

// 6502:lla oikeastaan vain yksi datatype
class variable_c
{
public:
	string m_name;
	value_t m_value;

	variable_c(string name, value_t value)
	{
		m_name = name;
		m_value = value;
	}
};