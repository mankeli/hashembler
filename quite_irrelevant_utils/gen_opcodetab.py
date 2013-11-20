import fileinput

sizedict = {'imp':1, 'zpy':2, 'zpx':2, 'imm':2, 'abx':3, 'ind':3, 'abs':3, 'rel':2, 'aby':3, 'izx':2, 'izy':2, 'zp':2}
modelist = ['imp', 'zpy', 'zpx', 'imm', 'abx', 'ind', 'abs', 'rel', 'aby', 'izx', 'izy', 'zp']
op_modes = []

opcodes = set()

print("struct instruction_s { int num; char name[4]; char addrmodename[4]; int addrmode; };")

print("instruction_s instructions[] = {")
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

	print("{{ 0x{0}, \"{1}\", \"{2}\", {3} }}, ".format(opnum, opname, addrmodename, addrmode))

	pass

print("};")
print("int modesizes[] = {")
for i in modelist:
	print("{0},".format(sizedict[i]))
print("};")

print("//",	sorted(list(opcodes)))