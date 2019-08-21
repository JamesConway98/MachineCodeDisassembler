/**************************************************************************
 * Assessment Title: Assessed Coursework Exercise #3
 *
 * Number of Submitted C Files: 1
 *
 * Date: 9/11/2017
 *
 *
 * Author:
 *      Rokas Aidukas,      Reg no: 201644296
 *      James Conway,       Reg no: 201603292
 *      Claudia Horvath,    Reg no: 201630190
 *
 *
 * Personal Statement: I confirm that this submission is all my own work.
 *
 *          (Signed)_________Claudia Horvath___Rokas Aidukas____James Conway_________________
 *
 *
 *
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   Defining constants for the length of our Words (16 bits), Opcodes (4 bits), Memory addresses and Operands (12 bits)
*/
#define WORD_LEN 16
#define OPCODE_LEN 4
#define OPERAND_LEN 12

/*
   Declaring and initialising registers
*/
char AC     [WORD_LEN] = "0000000000000000"; // ACCUMULATOR
char MAR    [OPERAND_LEN]  = "000000000000"; // MEMORY ADDRESS REGISTER
char MBR    [WORD_LEN] = "0000000000000000"; // MEMORY BUFFER REGISTER
char PC     [OPERAND_LEN]  = "000000000000"; // PROGRAM COUNTER
char IR     [WORD_LEN] = "0000000000000000"; // INSTRUCTION REGISTER
char inReg  [WORD_LEN] = "0000000000000000"; // INPUT REGISTER
char outReg [WORD_LEN] = "0000000000000000"; // OUTPUT REGISTER
char temp   [OPERAND_LEN] = "000000000000";  // TEMPORARY REGISTER
char mainMem [4096][16]; 		     // MAIN MEMORY
int halt = 0;

/*
   Function declarations
*/

int fetch();
int getOpcode(char *binArray);
int getOperandU(char *binArray);
int getOperandS(char *binArray);
void copyIRtoMAR(void);
void copyMARtoMBR(void);
void increasePC(void);
void loadAC(void);
void storeAC(void);
void subtX(void);
void subti(void);
void addX(void);
void addi(void);
void inputAC (void);
void outputAC (void);
void skip(void);
void jump(void);
void jumpG(void);
void jumpL(void);
void jumpNZ(void);
void hlt (void);
void decToBinU(int dec, char *binArray);
int binToDecU(char binArray[]);
void decToBinS(int dec, char *binArray);
int binToDecS(char binArray[]);
void displayMemory(void);
void assemblyDisplay(void);
void loadDefault(void);
void readConsole(void);
void readFile(void);
void printRegisters(void);



/*
   Runs the program 
*/
int main(int argc, char const *argv[]) {
    memset(mainMem, '0', 65536);
    if (argc == 2){
	if(!strcmp(argv[1],"-f"))
		readFile();
	else if (!strcmp(argv[1], "-d"))
		loadDefault();
	else if (!strcmp(argv[1], "-c"))
		readConsole();
    }

    else if (argc > 2)
	printf("Too many arguments supplied.\n");	
    else 
	printf("One argument expected");
    assemblyDisplay();
    while(!halt){
    	fetch();
    }
    hlt();
    return 0;
}

/*
   Decodes and fetches the instructions to run the program
*/
int fetch(){
	for (int i = 0; i < OPERAND_LEN; i++){
		MAR[i] = PC[i];
	}
	for (int i = 0; i < 16; i++){
		IR[i] = mainMem[binToDecU(MAR)][i];
	}
	increasePC();
	copyIRtoMAR();
	copyMARtoMBR();

	switch (getOpcode(IR)){

		case 10:
			loadAC();
			break;
		case 11:
			storeAC();
			break;
		case 111:
			skip();
			break;
		case 1000:
			subtX();
			break;
		case 1110:
			subti();
			break;
		case 1001:
			addX();
			break;
		case 1011:
			addi();
			break;
		case 1111:
			jump();
			break;
		case 100:
			inputAC();
			break;
		case 101:
			outputAC();
			break;
		case 0:
			hlt();
			break;
		case 1100:
			jumpG();
			break;
		case 1101:
			jumpL();
			break;
		case 1010:
			jumpNZ();
			break;
	}
}

/*
   Functions used for the fetch-decode-execute cycle
*/

/*
   Returns first (most significant to least significant) 4 bits of IR that determine the instruction
   parameter: pointer to an array of characters
*/
int getOpcode(char *binArray){
	char opcode[5];
	for (int i = 0; i < 4; i++){
		opcode[i] = binArray[i];
	}
	int n = atoi(opcode);
	return n;
}


/*
   Returns last 12 bits of IR that determine the operand
   parameter: pointer to an array of characters
*/
int getOperandU(char *binArray){
	char operand[12];
	for (int i = 4; i < WORD_LEN; i++){
		operand[i-4] = binArray[i];
	}
	 return binToDecU(operand);
}

/*
   Copies contents of IR to MAR
*/
void copyIRtoMAR(void){
	for (int i = 4; i<WORD_LEN; i++){
		MAR[i-4] = IR[i];
	}
}

/*
   Copies contents of MAR to MBR
*/
void copyMARtoMBR(void){
	int n = binToDecU(MAR);
	for(int i = 0; i <WORD_LEN; i++){
		MBR[i] = mainMem[n][i];
	}
}

/*
   Increases the Program Counter by 1
*/
void increasePC(void){
	int x = binToDecU(PC);
	x++;
	decToBinU(x, PC);
}

/*
   Instruction Set architecture functions
*/

/*
   Loads the Accumulator from MBR
*/
void loadAC(void){
	for (int i = 0; i <WORD_LEN; i++){
		AC[i] = MBR[i];
	}
}


/*
   Stores the Accumulator in the address MAR holds
*/
void storeAC(void){
	for (int i = 0; i<WORD_LEN; i++){
		mainMem[binToDecU(MAR)][i] = AC[i];
	}
}

/*
   Subtracts MBR from the Accumulator
*/
void subtX(void){
    int decAC = binToDecS(AC);
    int decMBR = binToDecS(MBR);
    int ans = 0;
    ans = decAC - decMBR;
    decToBinS(ans, AC);
}

/*
    Subtracts a constant that user inputs
*/
void subti(void){
    int decAC = binToDecS(AC);
    int ans = 0;
    int input;
    printf("Please enter a decimal number to subtract from the AC\n");
    scanf("%d", &input);
    ans = decAC - input;
    decToBinS(ans, AC);
}


/*
   Adds MBR to the Accumulator
*/
void addX(void){
	int x = binToDecS(AC);
	x += binToDecS(MBR);
    	decToBinS(x, AC);
}

/*
    Adds a constant the user inputs
*/
void addi(void){
	int x = binToDecS(AC);
	int input;
        printf("Please enter a decimal number to add to the AC\n");
	scanf("%d", &input);
	x += input;
        decToBinS(x, AC);
}

/*
   Prompts the user to enter a value for the Accumulator in binary
*/
void inputAC (void) {
	printf("Please input a value for the Accumulator (in binary)\n");
	scanf("%16s", AC);
}

/*
   Outputs the value of the Accumulator
*/
void outputAC (void) {
	printf("Accumulator is: %.16s\n", AC);
}

/*
   Skips an instruction
*/
void skip(void){
    if (!binToDecS(AC))
    	increasePC();
}

/*
  Loads the contents of MAR into PC 
*/
void jump(void){
	for (int i = 0; i < OPERAND_LEN; i++){
		PC[i] = MAR[i];
	}
}

/*
   Jumps to addres in MAR if the Accumulator is greater than 0
*/
void jumpG(void){
    if (binToDecS(AC) > 0){
    	jump();
    }
}

/*
   Jumps to address in MAR if the Accumulator is less than 0
*/
void jumpL(void){
    if (binToDecS(AC) < 0){
    	jump();
    }
}

/*
   Jumps to address in MAR if the Accumulator doesn't equal 0
*/
void jumpNZ(void){
    int x = 0;
    x = binToDecS(AC);
    if (x != 0) {
        jump();
    }
}


/*
   Terminates the program
*/
void hlt (void) {
	printf("Program has halted.\n");
	displayMemory();
	outputAC();	
	halt = 1;
}
/*
   Auxiliary C functions
*/

/*
   Converts decimal to unsigned binary character array
   parameters: decimal integer, pointer to binary array
*/
void decToBinU(int dec, char *binArray){
    memset(binArray, '0', OPERAND_LEN);
    int r = 0;
    int i = OPERAND_LEN;
    if (dec > 0 && dec <= 1<<(OPERAND_LEN-1)){
            while (dec != 0) {
                i--;
                r = dec % 2;
                binArray[i] = r + '0';
                dec = dec / 2;
	   }
    }
}

/*
   Converts unsigned binary character array to decimal integer
   parameters: binary character array
*/
int binToDecU(char binArray[]){
    int mul = 1;
    int total = 0;
    for (int i = OPERAND_LEN-1; i >= 0; i--){
        total += (binArray[i] - '0')*mul;
        mul *= 2;
    }
    return total;
}

/*
   Converts decimal to signed binary character array
   parameters: decimal integer, pointer to binary array
*/
void decToBinS(int dec, char *binArray){
    memset(binArray, '0', WORD_LEN);
    int r = 0;
    int i = WORD_LEN;
    if (dec <= (1<<(i-1)-1) && dec >= (-1<<(i-1))){
        // Conversion if it's positive.
        if (dec >= 0) {
            while (dec != 0) {
                i--;
                r = dec % 2;
                binArray[i] = r + '0';
                dec = dec / 2;
            }
        // Conversion if it's negative.
        } else {
            binArray[0] = '1';
	    int temp = -(1 << (WORD_LEN-1));
	    for (int i = 1; i < WORD_LEN; i++) {
	        if (temp + (1 << (WORD_LEN -1 - i)) <= dec){
		    temp += (1 << (WORD_LEN -1 - i));
	            binArray[i] = '1';
		} else {
	            binArray[i] = '0';
	        }
	    }
        }
    } else { // Out of bounds error check.
            printf("%s\n", "Overflow, decimal out of range!");
	    halt = 1;
    }
}

/*
   Converts signed binary character array to decimal integer
   parameters: binary character array
*/
int binToDecS(char binArray[]){
    int mul = 1;
    int total = 0;
    int negTemp = 0;
    for (int i = WORD_LEN-1; i > 0; i--){
        if (binArray[i] == '0') {
            mul *= 2;
        }
        if (binArray[i] == '1') {
            total += mul;
            mul *= 2;
        }
    }
    if (binArray[0] == '1'){
        negTemp = -mul;
        total += negTemp;
    }
    return total;
}


/*
   Displays Main Memory contents and its memory location
*/
void displayMemory(void){
	int x = 0;
	for (int i = 0; i < 4096; i+=4){
		printf("%d\t= %.16s\t%d\t= %.16s\t%d\t= %.16s\t%d\t= %.16s\n", i, mainMem[i], i+1, mainMem[i+1], i+2, mainMem[i+2], i+3, mainMem[i+3]);
	}
}

/*
   Displays program in Assembly Language
*/
void assemblyDisplay(void){
	int i = 0;
	do{
	switch (getOpcode(mainMem[i])){
		
		case 10:
			printf("%.4d: LDA [%d] : Load the contents from Memory Location [%d] into AC\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i])); 
			break;
		case 11:
			printf("%.4d: STA [%d] : Store the contents of AC into Memory Location [%d]\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 111:
			printf("%.4d: SKP \t : Skips the next intruction if AC = 0\n", i);
			break;
		case 1000:
			printf("%.4d: SUB [%d] : Subtracts the value in Memory Location [%d] from AC and stores the result in AC\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 1110:
			printf("%.4d: SBI \t : Subtracts a decimal number from the AC input by the user \n", i);
			break;
		case 1001:
			printf("%.4d: ADD [%d] : Adds the value in Memory Location [%d] with AC and stores the result in AC\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 1011:
			printf("%.4d: ADI \t : Adds a decimal number input by the user to the AC\n", i);
			break;
		case 1111:
			printf("%.4d: JMP [%d] : Jumps to memory location [%d]\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 100:
			printf("%.4d: INP \t : Inputs a value into AC\n", i);
			break;
		case 101:
			printf("%.4d: OUT \t : Outputs the value of the AC\n", i);
			break;
		case 0:
			printf("%.4d: HLT \t : Halts the program\n", i);
			break;
		case 1100:
			printf("%.4d: JMG [%d] : Jumps to memory location [%d] if the AC is greater than 0\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 1101:
			printf("%.4d: JML [%d] : Jumps to memory location [%d] if the AC is less than 0\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
		case 1010:
			printf("%.4d: JNZ [%d] : Jumps to memory location [%d] if the AC not 0\n", i, getOperandU(mainMem[i]), getOperandU(mainMem[i]));
			break;
	}
	i++;
	} while(getOpcode(mainMem[i-1]) != 0);
}

/*
   Fills Main memory with default content
*/
void loadDefault(void){
    strcpy(mainMem[0], "0100000000000000"); //Input into AC
    strcpy(mainMem[1], "1101000000000000"); //Jump to Memory Location[0] if the input is less than 0
    strcpy(mainMem[2], "0011111111111111"); //Store AC into Main Memory[4095]
    strcpy(mainMem[3], "0101000000000000"); //Output the AC
    strcpy(mainMem[4], "1110000000000000"); //Subtract from the AC a decimal number inputed by the user
    strcpy(mainMem[5], "0111000000000000"); //Skip the next instruction if the AC is  0
    strcpy(mainMem[6], "1111000000000011"); // Jump to Main Memory [3]
    strcpy(mainMem[7], "1011000000000000"); // Add a decimal integer that user inputs to the AC
    strcpy(mainMem[8], "1000111111111111"); // Subtract from AC the value in Memory Location [4095]
    strcpy(mainMem[9], "0000000000000000"); //Halt
    memset(mainMem[10], '0', 16);
}

/*
   Prompts user to type a program in binary
*/
void readConsole(void) {
   char line[WORD_LEN];
   int i = 0;
   scanf("%16s", line);
   while (strcmp(line,"exit") && i < 4001){
	for(int x = 0; x < WORD_LEN; x++){
		mainMem[i][x] = line[x];
	}
	scanf("%16s", line);
	i++;
   }	
}

/*
   Reads contents of file and fills Main Memory
*/
void readFile(void) {
   FILE *fr;
   char line[WORD_LEN];
   fr = fopen("ACE3.txt", "r");
   int i = 0;
   while (fscanf(fr, "%16s%*[\n]", line) != EOF && i < 4001){
	for(int x = 0; x < WORD_LEN; x++){
		mainMem[i][x] = line[x];
	}
	i++;
   }	
   fclose(fr);
}

void printRegisters(void){
	printf("IR = %.16s\n", IR);
	printf("AC = %.16s\n", AC);
	printf("MAR1 = ");
	for(int i = 0; i < OPERAND_LEN; i++){
		printf("|%c", MAR[i]);
	}
	printf("MAR = %.12s\n", MAR);
	printf("MBR = %.16s\n", MBR);
	printf("PC = %.12s\n", PC);
}