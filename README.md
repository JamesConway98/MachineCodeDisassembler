
- Claudia
  - CPU REGISTERS:
    - AC: (Accumulator), which holds data values that the CPU needs to process.
    - MAR: (Memory Address Register), which holds the memory address of the data being referenced.
  - MACHINE INSTRUCTIONS:
    - **0100 InputAC** - Input a value from the keyboard into AC. 
    - **0101 OutputAC** - Output the value in AC to the display. 
    - **0000 Halt** - Terminate the program.
  - C FUNCTIONS:
    - Load some default content into the memory. 
    - Display the memory contents on the screen. Your function must also display the respective memory locations.
    
- James
  - CPU REGISTERS:
    - IR (Instruction Register), which holds the next instruction to be executed.
  - MACHINE INSTRUCTIONS:
    - **1000 Subt X** - Subtract the contents of address X from AC and store the result in AC.
    - **1001 Add X** - Add the contents of address X to AC and store the result in AC. 
    - **1111 Jump X** - Load the value of X into PC. 
  - C FUNCTIONS:
    - Convert an integer from decimal representation to binary. 
    - Convert an integer from binary representation to decimal. 
    
- Rokas
  - CPU REGISTERS:
    - InREG (Input Register), which holds data from the input device. 
    - OutREG (Output Register), which holds data for the output device.
  - MACHINE INSTRUCTIONS:
    - **1100 BGT** - Jump if greater than.
    - **1101 BLT** - Jump if less than.
    - **1010 BNZ** - Jump if not zero.
  - C FUNCTIONS:
    - Convert the memory contents to assembly language and display the assembly language on the screen. Your function must display each assembly instruction on a new line.

### General ACE3 Computer Conventions

##### Basic Characteristics
-	Binary,	two’s	complement	representation
-	Stored	program,	fixed	word	length	
-	Word	(not	byte)	addressable	
-	4K	words	of	main	memory	(this	implies	12	bits	per	address)	
-	16-bit	instructions,	4	for	the	opcode	and	12	for	the	operand
-	A	16-bit	accumulator	(AC)	
-	A	16-bit	instruction	register	(IR)	
-	A	16-bit	memory	buffer	register	(MBR)	
-	A	12-bit	program	counter	(PC)	
-	A	12-bit	memory	address	register	(MAR)	
-	An input register	
-	An output register

##### Primary Memory
4K	words	of	main	memory.
Each	cell	can	hold	machine	instructions	or	data	values.	

##### Instruction Format
Each instruction is a 16-bit number:

- The	Operation	Code/opcode	(the	most	significant	4	bits) specifies	what	to	do.	All	instructions	
have	an	opcode.
- The	Operand (the	least	significant	12	bits) provides	additional	data	for	the	opcode	to	operate	
on.	Typically,	 the	operand	is	a	memory	address	where	data	 to	be	operated	on	is	stored	or	
where	the	next	instruction	should	be	read,	or	it	is	an	immediate	(constant)	numerical	value	to	
be	operated	on.

In Laymans terms, all of the machine instructions that we're writing have the 4-bit OPCODE specified for it, all of us have to keep to the given values. Any instructions that have an X next to them means the instruction has some numerical value to work with.
