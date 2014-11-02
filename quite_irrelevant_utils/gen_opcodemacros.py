import sys
import fileinput

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
		continue
	else:
		opname = graham[1].lower()

	if (len(graham) == 4):
		addrmodename = graham[2].lower()
	else:
		if (opname == 'ror'):
			addrmodename = "impa"
		elif (opname == 'asl'):
			addrmodename = "impa"
		elif (opname == 'lsr'):
			addrmodename = "impa"
		elif (opname == 'rol'):
			addrmodename = "impa"
		else:
			addrmodename = "imp"


	opcodes.add(opname)

	opnameup = opname.upper()

	# print("{{ 0x{0}, \"{1}\", \"{2}\", {3} }}, ".format(opnum, opname, addrmodename, addrmode))
	# print("#define {0}.{1} (_val) {{{ __current_seg->add_statement(\"{0}\", \"{1}\", _val); }}} ".format(opname, addrmodename, opname, addrmodename))

	if (addrmodename == 'imp'):
		print("#define {0}() {{ __current_seg->add_op(0x{1}, 0, 0); }}".format(opnameup, opnum))
		#print("#define {0} {{ __current_seg->add_op(0x{1}, 0, 0); }}".format(opnameup, opnum))
	elif (addrmodename == 'impa'):
		print("#define {0}a() {{ __current_seg->add_op(0x{1}, 0, 0); }}".format(opnameup, opnum))

	elif (addrmodename == 'imm'):
		print("#define {0}i(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))
	elif (addrmodename == 'zp'):
		print("#define {0}z(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))
	elif (addrmodename == 'zpx'):
		print("#define {0}zx(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))
	elif (addrmodename == 'zpy'):
		print("#define {0}zy(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))
	elif (addrmodename == 'izx'):
		print("#define {0}izx(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))
	elif (addrmodename == 'izy'):
		print("#define {0}izy(_val) {{ __current_seg->add_op(0x{1}, _val, 1); }}".format(opnameup, opnum))

	elif (addrmodename == 'ind'):
		print("#define {0}i(_val) {{ __current_seg->add_op(0x{1}, _val, 2); }}".format(opnameup, opnum))
	elif (addrmodename == 'abs'):
		print("#define {0}(_val) {{ __current_seg->add_op(0x{1}, _val, 2); }}".format(opnameup, opnum))
	elif (addrmodename == 'abx'):
		print("#define {0}x(_val) {{ __current_seg->add_op(0x{1}, _val, 2); }}".format(opnameup, opnum))
	elif (addrmodename == 'aby'):
		print("#define {0}y(_val) {{ __current_seg->add_op(0x{1}, _val, 2); }}".format(opnameup, opnum))


	elif (addrmodename == 'rel'):
		print("#define {0}(_val) {{ __current_seg->add_op(0x{1}, _val, 3); }}".format(opnameup, opnum))

	else:
		print("ERROR {0}".format(addrmodename))
		sys.exit()

	pass
