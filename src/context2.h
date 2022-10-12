#pragma once
	#include <unordered_map>
namespace hashembler
{

	struct setvalue_t
	{
		value_t m_value;
		int m_createpass;
		int m_accesspass;
	};

	std::unordered_map<string, setvalue_t> vars;
	value_t label(const char *format, int size, ...)
	{
		char tmpname[1024];
		va_list args;
		va_start(args, size);
		int r = vsprintf(tmpname, format, args);
		va_end(args);

		string name = string(tmpname);

		if (vars.count(name) == 0)
		{
			printf("can't find var %s\n", name.c_str());
			g_pass_vars_left++;
			return 0xDEADFACE;
		}

		vars[name].m_accesspass = g_pass;

		return vars[name].m_value;
	}

	void makelabel(const char *format, int size, ...)
	{
		char tmpname[1024];
		va_list args;
		va_start(args, size);
		int r = vsprintf(tmpname, format, args);
		va_end(args);

		string name = string(tmpname);

		if (vars.count(name) > 0)
		{
			if (vars[name].m_createpass == g_pass)
			{
				printf("label/variable '%s' already created on this pass (%d)..\n", name.c_str(), g_pass);
				exit(1);
			}
			else if (vars[name].m_accesspass == g_pass)
			{
				//printf("label/variable '%s' already accessed on this pass (%d)..\n", name.c_str(), g_pass);

				if (vars[name].m_value != PC())
					g_pass_vars_left++;
			}
		}


		vars[name].m_value = PC();
		vars[name].m_createpass = g_pass;
	}


	#define L(_name, ...) ( label(_name, 0,  ##__VA_ARGS__) )
	#define LPC(_n, ...) { makelabel(_n, 0, ##__VA_ARGS__); }

	#define CTX(_x)
	#define RESERVE(_x)
}