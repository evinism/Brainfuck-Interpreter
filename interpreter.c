#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argC, char** argV){
	char *program = malloc(100);
	int programLength = 100;
	int endOfProgram;
	char *memory = malloc(100);
	int memLength = 100;
	//Initialize memory
	int i;
	for( i = 0; i < memLength; i++)
		memory[i] = 0;
	int pc = 0;//Program counter!
	if( argC != 2 ){
		printf("Usage: %s [name of file]\n", argV[0]);
		return 0;
	}
	FILE *fp = fopen(argV[1],"r");
	if( !fp ){
		printf("Could not open specified file: %s\n", argV[1]);
		return 0;
	}
	char c;
	while((c = fgetc(fp))!=EOF){
		if( pc >= programLength ){
			program = realloc(program, 2*programLength);//We want a small number of reallocs, so increasing by constant size doesn't make sense
			if( program==NULL ){
				printf("Loading Program: Can't allocate new memory!\n");
				return 1;
			}
			programLength *= 2;
		}
		program[pc] = c;//Use program counter to insert the thing into the array;
		pc++;
	}
	endOfProgram = pc;
	pc = 0;//Now that we're done, reset, yo.
	int mempos = 0;
	int nestedLoops = 0;
	while( pc<endOfProgram ){
		switch( program[pc] ){
			case '>':
				++mempos;
				if( mempos >= memLength ){
					memory = realloc(memory, 2*memLength);//Same thing for memory.
					if( memory==NULL ){
						printf("Program: Can't allocate new memory!\n");
						return 1;
					}
					for( i = memLength; i < memLength*2; i++)
						memory[i] = 0;
					memLength *= 2;
				}
				break;
			case '<':
				--mempos;
				break;
			case '+':
				++memory[mempos];
				break;
			case '-':
				--memory[mempos];
				break;
			case '.':
				printf("%c",memory[mempos]);
				break;
			case ',':
				memory[mempos] = getchar();
				break;
			case '[':
				break;
			case ']':
				if(memory[mempos] == 0)
					break;
				while(1){
					--pc;
					if( pc < 0 ){
						printf("Error: ] has no opening bracket.\n");
						return 0;
					}
					if( program[pc] == '['){
						if( nestedLoops == 0 )
							break;
						else
							nestedLoops--;
					}else if(program[pc] == ']'){
						nestedLoops++;
					}
				}
				break;
			default: //Ignore all ofther characters
				break;
		}
		pc++;
	}
	free(program);
	free(memory);
	fclose(fp);
}