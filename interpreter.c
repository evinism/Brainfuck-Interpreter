#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEPTH_LIMIT 1000

int main(int argC, char** argV){
	int programLength = 100;//Initial Vals.
	int memLength = 100;
	char *program = malloc(programLength);
	int endOfProgram;
	char *memory = malloc(memLength);
	//Initialize memory
	int i;
	for( i = 0; i < memLength; i++)
		memory[i] = 0;
	int pc = 0;//Program counter
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
				free(program);
				free(memory);
				return 1;
			}
			programLength *= 2;
		}
		program[pc] = c;//Use program counter to insert the instructions into the array;
		pc++;
	}
	endOfProgram = pc;
	pc = 0;//Now that we're done, reset, yo.
	int mempos = 0;
	int depth = 0;
	int bracket[DEPTH_LIMIT];
	while( pc<endOfProgram ){
		switch( program[pc] ){
			case '>':
				++mempos;
				if( mempos >= memLength ){
					memory = realloc(memory, 2*memLength);//Same thing for memory.
					if( memory==NULL ){
						printf("Program: Can't allocate new memory!\n");
						free(program);
						free(memory);
						fclose(fp);
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
        if( memory[mempos] == 0 ){
          //Forward miniscan
          int tempDepth = depth + 1;
          while( tempDepth > depth ){
            pc++;
            switch( program[pc] ){
              case '[':
                tempDepth++;
                break;
              case ']':
                tempDepth--; 
                break;
            }
          }
          //End forward miniscan
        }else{
          depth++;
          if( depth > DEPTH_LIMIT ){
            printf("Warning: Depth Limit Exceeded\n");
          }
          bracket[depth] = pc;
        }
				break;
			case ']':
				if(memory[mempos] == 0){
					if(depth > 0){
						depth--;
					}else{
						printf("Error: Unmatched Bracket at PC %d\n", pc);
						free(program);
						free(memory);
						fclose(fp);
						return 1;
					}
				}else{
					pc = bracket[depth];
				}
				break;
			default: //Ignore all other characters
				break;
		}
		pc++;
	}
	free(program);
	free(memory);
	fclose(fp);
	if( depth > 0){
		printf("Warning: Unmatched [ bracket at EOF\n");
	}
	return 0;
}
