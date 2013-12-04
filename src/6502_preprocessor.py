import fileinput
import re
import sys
from pprint import pprint

modelist = ['imp', 'imm', 'zp', 'izx', 'izy', 'zpx', 'zpy', 'rel', 'abs', 'abx', 'aby', 'ind' ]
modecodes = {'and': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'bvs': {'rel'}, 'sec': {'imp'}, 'rol': {'abx', 'imp', 'abs', 'zp', 'zpx'}, 'pla': {'imp'}, 'anc': {'imm'}, 'rti': {'imp'}, 'arr': {'imm'}, 'rra': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'bvc': {'rel'}, 'sax': {'izx', 'zpy', 'abs', 'zp'}, 'lsr': {'abx', 'imp', 'abs', 'zp', 'zpx'}, 'rts': {'imp'}, 'inx': {'imp'}, 'ror': {'abx', 'imp', 'abs', 'zp', 'zpx'}, 'ldx': {'zpy', 'zp', 'abs', 'imm', 'aby'}, 'alr': {'imm'}, 'ahx': {'izy', 'aby'}, 'sei': {'imp'}, 'iny': {'imp'}, 'inc': {'abx', 'zpx', 'abs', 'zp'}, 'cli': {'imp'}, 'beq': {'rel'}, 'cpy': {'zp', 'abs', 'imm'}, 'cld': {'imp'}, 'txs': {'imp'}, 'tas': {'aby'}, 'clc': {'imp'}, 'adc': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'tsx': {'imp'}, 'xaa': {'imm'}, 'clv': {'imp'}, 'asl': {'abx', 'imp', 'abs', 'zp', 'zpx'}, 'jmp': {'ind', 'abs'}, 'bne': {'rel'}, 'ldy': {'abx', 'zp', 'abs', 'imm', 'zpx'}, 'axs': {'imm'}, 'plp': {'imp'}, 'tax': {'imp'}, 'pha': {'imp'}, 'bmi': {'rel'}, 'rla': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'tya': {'imp'}, 'tay': {'imp'}, 'sbc': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'lax': {'zpy', 'imm', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'txa': {'imp'}, 'jsr': {'abs'}, 'kil': {'imp'}, 'bit': {'abs', 'zp'}, 'php': {'imp'}, 'nop': {'zpx', 'imm', 'imp', 'abs', 'abx', 'zp'}, 'dcp': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'ora': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'dex': {'imp'}, 'dey': {'imp'}, 'dec': {'abx', 'zpx', 'abs', 'zp'}, 'sed': {'imp'}, 'sta': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'sre': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'shx': {'aby'}, 'shy': {'abx'}, 'bpl': {'rel'}, 'bcc': {'rel'}, 'cpx': {'zp', 'abs', 'imm'}, 'eor': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'lda': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'slo': {'zpx', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'las': {'aby'}, 'isc': {'zpx', 'imp', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'brk': {'imp'}, 'cmp': {'zpx', 'imm', 'abx', 'abs', 'aby', 'izx', 'izy', 'zp'}, 'stx': {'zpy', 'abs', 'zp'}, 'sty': {'zpx', 'abs', 'zp'}, 'bcs': {'rel'}}

allcodes = set(['and', 'bvs', 'sec', 'rol', 'pla', 'anc', 'rti', 'arr', 'rra', 'bvc', 'sax', 'lsr', 'rts', 'inx', 'ror', 'ldx', 'alr', 'ahx', 'sei', 'iny', 'inc', 'cli', 'beq', 'cpy', 'cld', 'txs', 'tas', 'clc', 'adc', 'tsx', 'xaa', 'clv', 'asl', 'jmp', 'bne', 'ldy', 'axs', 'plp', 'tax', 'pha', 'bmi', 'rla', 'tya', 'tay', 'sbc', 'lax', 'txa', 'jsr', 'kil', 'bit', 'php', 'nop', 'dcp', 'ora', 'dex', 'dey', 'dec', 'sed', 'sta', 'sre', 'shx', 'shy', 'bpl', 'bcc', 'cpx', 'eor', 'lda', 'slo', 'las', 'isc', 'brk', 'cmp', 'stx', 'sty', 'bcs'])

for line in fileinput.input():

	line = re.match("^\s*(.*?)\s*(//.*)?$", line).group(1)

	is_this_label = False
	if re.match("^^§[\w]+\:$", line):
		is_this_label = True

	line = re.sub('§([\w]+)\.([\w]+)', '\g<1>.get_variable("\g<2>")', line)
	line = re.sub('§([\w]+)', '__current_seg->get_variable("\g<1>")', line)
	line = re.sub('§([\w]+)\.\*', '\g<1>.get_pos()', line)
	line = re.sub('§\*', '__current_seg->get_pos()', line)

	if (is_this_label):
		line=line[:-1]+" = __current_seg->get_pos();"

	# forgive me: i didn't bother to invent proper regexes. this will do for now

	is_this_op = True
	is_this_bug = False


	op = ""
	imm = ""
	operand = ""
	zp = ""
	ptr = False
	idx = ""

	# find out indexing parameter, and separate it
	idx_m = re.match("^(.+),\s*([xy])$", line)
	if idx_m :
		idx = idx_m.group(2)
		line2 = idx_m.group(1)
	else:
		line2 = line

	# separate op and operand
	rest_m = re.match('^(([a-zA-Z]{3})(\.z)?)(\s+(.*))?$', line2)
	if rest_m:
		op = rest_m.group(2).lower()
		if rest_m.group(3) :
			zp = rest_m.group(3)
		if (rest_m.group(5)):
			operand = rest_m.group(5)
	else:
		is_this_op = False

	# check if operand is valid and if it's immediate mode
	if (len(operand) > 2):
		if (operand[0] == '#') :
			imm = '#'
			operand = operand[1:]
		else:
			imm = ''
	elif (len(operand) == 1):
		is_this_op = False


	# check if operand is a ptr and if it's (indirect,x)

	if (len(operand) > 0):
		ptr_m = re.match("^\((.+)\)$", operand)
		if (ptr_m):
			ptr = True
			xind_m = re.match("(.*),x$", ptr_m.group(1))
			if (xind_m):
				operand = xind_m.group(1)
				idx = "x"
			else:
				operand = ptr_m.group(1)
				idx = "y"

	# decide addrmode from these parameters

	if (len(operand) == 0):
		addrmode = "imp"
	elif (len(imm) > 0):
		addrmode = "imm"
	elif (len(zp) > 0 and ptr == False):
		if (idx == 'x'):
			addrmode = "zpx"
		elif (idx == 'y'):
			addrmode = "zpy"
		else:
			addrmode = "zp"
	elif (len(zp) > 0 and ptr == True):
		if (idx == 'x'):
			addrmode = "izx"
		elif (idx == 'y'):
			addrmode = "izy"
		else:
			addrmode = "izp" # this addressing mode doesn't exists

	elif (len(zp) == 0 and ptr == False):
		if (idx == 'x'):
			addrmode = "abx"
		elif (idx == 'y'):
			addrmode = "aby"
		elif (op == "bpl" or op == "bmi" or op == "bvc" or op == "bvs" or op == "bcc" or op == "bcs" or op == "bne" or op == "beq"):
			addrmode = "rel"
		else:
			addrmode = "abs"
	elif (len(zp) == 0 and ptr == True):
		if (idx == 'x'):
			addrmode = "NOA" # doesn't exists
		elif (idx == 'y'):
			addrmode = "NOA" # doesn't exist
		elif (op == "jmp"):
			addrmode = "ind"

	# check if op actually even exists
	if not (op in modecodes):
		is_this_op = False
	else:
		if not (addrmode in modecodes[op]) :
			is_this_bug = True

#modelist = ['imp', 'imm', 'zp', 'izx', 'izy', 'zpx', 'zpy', 'rel', 'abs', 'abx', 'aby', 'ind' ]

	if (is_this_label):
		print(line)
	elif (is_this_bug):
		print("#error \"", end="")
		print(line, "----> ", end="")
		print([is_this_op, op, zp, ptr, idx, imm, operand, addrmode], end="")
		print(modecodes[op], "\"", end="")
	elif (is_this_op):
		if (len(operand) == 0):
			operand = "0"
		print('__current_seg->add_statement("' + op + '", "' + addrmode + '", ' + operand + ');')
	else:
		print(line)
	#pprint(modes)

