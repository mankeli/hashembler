import fileinput
from pprint import pprint
from collections import defaultdict

allcodes = defaultdict(set)

codeset = set()

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

	allcodes[opname].add(addrmodename)
	codeset.add(opname)
	pass

print(allcodes)

print(codeset)
