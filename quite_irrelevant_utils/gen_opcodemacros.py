import sys
import fileinput

sizedict = {'imp':1, 'zpy':2, 'zpx':2, 'imm':2, 'abx':3, 'ind':3, 'abs':3, 'rel':2, 'aby':3, 'izx':2, 'izy':2, 'zp':2}
modelist = ['imp', 'zpy', 'zpx', 'imm', 'abx', 'ind', 'abs', 'rel', 'aby', 'izx', 'izy', 'zp']
op_modes = []

opcodes = set()

for line in fileinput.input():
	graham = line[:17].split()
	visu = line[18:].split()
#	print graham
#	print visu
#	print

	opnum = graham[0]

	if (graham[1][:1] == "*"):
		opname = graham[1][1:].lower()
	else:
		opname = graham[1].lower()

	if (len(graham) == 4):
		addrmodename = graham[2].lower()
	else:
		addrmodename = 'imp'

	addrmode = modelist.index(addrmodename)

	opcodes.add(opname)

	# print("{{ 0x{0}, \"{1}\", \"{2}\", {3} }}, ".format(opnum, opname, addrmodename, addrmode))
	# print("#define {0}.{1} (_val) {{ __current_seg->add_statement(\"{0}\", \"{1}\", _val); }} ".format(opname, addrmodename, opname, addrmodename))
	if (addrmodename == 'imp'):
		print("#define {0}a() {{ __current_seg->add_statement(\"{1}\", \"imp\", 0); }}".format(opname.upper(), opname))
	elif (addrmodename == 'imm'):
		print("#define {0}i(_val) {{ __current_seg->add_statement(\"{1}\", \"imm\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'rel'):
		print("#define {0}(_val) {{ __current_seg->add_statement(\"{1}\", \"rel\", _val); }}".format(opname.upper(), opname))


	elif (addrmodename == 'zp'):
		print("#define {0}z(_val) {{ __current_seg->add_statement(\"{1}\", \"zp\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'zpx'):
		print("#define {0}zx(_val) {{ __current_seg->add_statement(\"{1}\", \"zpx\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'zpy'):
		print("#define {0}zy(_val) {{ __current_seg->add_statement(\"{1}\", \"zpy\", _val); }}".format(opname.upper(), opname))

	elif (addrmodename == 'izx'):
		print("#define {0}izx(_val) {{ __current_seg->add_statement(\"{1}\", \"izx\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'izy'):
		print("#define {0}izy(_val) {{ __current_seg->add_statement(\"{1}\", \"izy\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'ind'):
		print("#define {0}i(_val) {{ __current_seg->add_statement(\"{1}\", \"ind\", _val); }}".format(opname.upper(), opname))

	elif (addrmodename == 'abs'):
		print("#define {0}(_val) {{ __current_seg->add_statement(\"{1}\", \"abs\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'aby'):
		print("#define {0}y(_val) {{ __current_seg->add_statement(\"{1}\", \"aby\", _val); }}".format(opname.upper(), opname))
	elif (addrmodename == 'abx'):
		print("#define {0}x(_val) {{ __current_seg->add_statement(\"{1}\", \"abx\", _val); }}".format(opname.upper(), opname))
	else:
		print("ERROR {0}".format(addrmodename))
		sys.exit()

	pass
