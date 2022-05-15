User guide for assembly code:

Formats

F1:
OPCODE SRC1 SRC2 DEST
Available instructions
	ADD
	SUB
	XOR
	OR


F2: 
	JMP INSTRUCRION
	OPCODE PREDICADE SRC1 SRC2 ADDRESS
	AVAILABLE PREDICADES
		eq  : == 
		neq : != 
		lt  : <
		leq : <=
		gt  : >
		geq : >=
	Label format:
	Labels is written only by one word and starting with "label" word
	For jumping in ADDRESS section is written available(allready defined label name)
	Example: jmp geq %r1 $10 label_name(jump to label_name if reg 1 >= 10)
F3:	
	MOVE AND NOT
	OPCODE SRC1 DEST
		example mov $50 %r1(move 50 to reg 1)	
