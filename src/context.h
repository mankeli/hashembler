#pragma once
namespace hashembler
{

class _ctx_t
{
public:

	string m_name;
	int m_pass;

	vector<bool> m_zp;
	list<variable_c> m_variables;

	void reserve(value_t pos)
	{
		m_zp[pos] = true;
	}

	value_t alloc(string name, int size)
	{
		int i, j;
		int zppos = -1;

		for (i = 0; i < (0x100-size);i++)
		{
			if (!m_zp[i])
			{
				bool failed = false;
				for (j = 0; j < size; j++)
				{
					if (m_zp[i+j])
						failed = true;
				}
				if (!failed)
				{
					zppos = i;
					break;
				}
			}
		}

		if (zppos < 0)
		{
			printf("zeropage allocation failed. no more space.\n");
		}
		else
		{
			for (j = 0; j < size; j++)
			{
				m_zp[zppos + j] = true;
			}
		}

		printf("zp variable '%s' allocated at 0x%04x\n", name.c_str(), zppos);

		return zppos;
	}

	value_t &get_variable(string name, int size)
	{
		for (auto &it: m_variables)
		{
			if (it.m_name == name)
			{
				if (it.m_size != size)
				{
					printf("WARNING: variable '%s' size mismatch.\n", name.c_str());
				}
				return it.m_value;
			}
		}

		value_t initval = 0xDEADFACE;
		if (size > 0)
			initval = alloc(name, size);
		else
		{
			// TODO: make some "debug" flag which enables this line
			 printf("size 0 variable undefined: '%s'\n", name.c_str());
			g_pass_vars_left++;
		}

		m_variables.push_back(variable_c(name, initval, size));
		return m_variables.back().m_value;
	}
};


class _ctx_handler_t
{
	list<_ctx_t> m_ctxlist;
	list<_ctx_t *> m_ctxstack;
	/*
	stack<_ctx_t *> m_ctxstack;
		stacklistiin tulee pino allokoiduista konteksteista. lahinna viittauksia varten mutta ehka nimetyt kontekstit riittavat
		myos taytyy olla mekanismi jolla sisakkaiset kontekstit varaavat muistiosoitteita ja nimia, sitten taytyy ehka teha staattiset osoitteet
		seka viittaukset labeleihin kontekstien sisalla

		ja linkkaus on tietenkin taysin ratkaisematon pulma viela

	*/
public:

	_ctx_t *ctxcreated(string name)
	{
		//m_ctxlist.push_back(ctx);
//		printf("ctx created '%s'\n", name.c_str());

		_ctx_t *ctxptr = NULL;

		for (auto &it: m_ctxlist)
		{
			if (it.m_name == name)
			{
				_ctx_t *actctx = &it;

//				printf("exists: '%s' was created in pass %i\n", actctx->m_name.c_str(), actctx->m_pass);

				ctxptr = actctx;
			}
		}

		if (!ctxptr)
		{
			_ctx_t newctx;
			newctx.m_name = name;
			newctx.m_pass = g_pass;
			if (m_ctxstack.size() == 0)
				newctx.m_zp.resize(0x100, false);
			else
				newctx.m_zp = m_ctxstack.back()->m_zp;
			m_ctxlist.push_back(newctx);

			ctxptr = &(m_ctxlist.back());
		}

		m_ctxstack.push_back(ctxptr);

//		printf("ctxstack size is now: %lu\n", m_ctxstack.size());

		return ctxptr;
	}

	void ctxdestroyed(string name)
	{
		_ctx_t *ctxptr;
		ctxptr = m_ctxstack.back();

		if (name == ctxptr->m_name)
		{
			m_ctxstack.pop_back();
		}
		else
		{
			printf("ctx destry '%s'. current was: '%s'\n", name.c_str(), ctxptr->m_name.c_str());
			stopassembly("context mismatch");
		}

//		printf("ctxstack size is now: %lu\n", m_ctxstack.size());
	}

	void reserve(value_t pos)
	{
		_ctx_t *ctxptr;
		ctxptr = m_ctxstack.back();
		ctxptr->reserve(pos);
	}

	value_t &get_variable(string name, int size)
	{
		_ctx_t *ctxptr;
		/*
			/twister/startpos
			../twister/startpos

			twister.startpos
		*/
//		printf("considering: '%s'\n", name.c_str());
		size_t pos = name.find_first_of("/", 0);
//		printf("pos of /: %i\n", pos);

		if (pos == -1) // local variable
		{
			ctxptr = m_ctxstack.back();
			return ctxptr->get_variable(name, size);
		}
		else if (pos == 0)
		{
			ctxptr = m_ctxstack.front();
			return ctxptr->get_variable(name.c_str() + 1, size);
		}

		// fail
		return ctxptr->get_variable(name, size);
	}


	value_t &get_variable_va(const char *format, int size, ...)
	{
		char tmpname[1024];
   		va_list args;
   		va_start(args, size);
   		int r = vsprintf(tmpname, format, args);
   		va_end(args);

		return get_variable(tmpname, size);
	}


};

_ctx_handler_t _ctx_handler;

class _ctxhelper_t
{
	string m_name;
	_ctx_t *m_ctx;

public:
	_ctxhelper_t(string name)
	{
		m_name = name;
		m_ctx = _ctx_handler.ctxcreated(m_name);
//		printf("helper: yey! my ctx is at %p. '%s'\n", m_ctx, m_ctx->m_name.c_str());
	}

	~_ctxhelper_t()
	{
		_ctx_handler.ctxdestroyed(m_name);
	}

};

};

#define CTX(_name) _ctxhelper_t __ctx(_name);
#define ZP(_name) (_ctx_handler.get_variable(_name, 1))
#define ZPS(_name, _size) (_ctx_handler.get_variable(_name, _size))
//#define L(_name) (_ctx_handler.get_variable(_name, 0))
#define L(_name, ...) (_ctx_handler.get_variable_va(_name, 0,  ##__VA_ARGS__))
#define RESERVE(_pos) { _ctx_handler.reserve(_pos); }

//#define LPC(_n) { L(_n) = PC(); }
#define LPC(_n, ...) { L(_n, ##__VA_ARGS__) = PC(); }

