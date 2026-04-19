#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define VERSION "Unstable 1.0.1\n"

/*
 * S80A (Simple 8080 Assembler
 * Version: prototype 2
 * 
 * This is the second version os 8080 assembler, i dont have any idea of what im doing
 * if u are reading this, congratulations for me, my git works
 * im getting crazy because im very tired, i was coding ts at 2am ;-;
 * */
 
uint16_t labels_data[128] = {0};
char labels[128][128];
int lc = 0;

char *exit_name = "a.bin";

int IsNumber(char* s) {
	if(*s == '\0' || s == NULL) return 1;
	while(*s) {
		if((unsigned char)*s < '0' || (unsigned char)*s > '9') return 1;
		s++;
	}
	return 0;
}

void GetParam(FILE *f, char *dest) {
    int index = 0;
    int c;
    
    while((c = fgetc(f)) != EOF && (isspace(c) || c == ','));
    
    if(c != EOF) {
        dest[index++] = c;
        while((c = fgetc(f)) != EOF && c != ',' && !isspace(c)) {
            if(index < 127) dest[index++] = c;
        }
    }
    dest[index] = '\0';
}

int GetRegIndex(char* r) {
    if(strcmp(r, "B") == 0) return 0;
    if(strcmp(r, "C") == 0) return 1;
    if(strcmp(r, "D") == 0) return 2;
    if(strcmp(r, "E") == 0) return 3;
    if(strcmp(r, "H") == 0) return 4;
    if(strcmp(r, "L") == 0) return 5;
    if(strcmp(r, "M") == 0) return 6;
    if(strcmp(r, "A") == 0) return 7;
    return -1; // Error
}

int SearchLabel(char *label) {
	int l = 0;
	while(l < lc) {
		if(strcmp(label, labels[l]) == 0) return labels_data[l];
		l++;
	}
	
	return -1;
}

int main(int argc, char* argv[]) {
	char inst[128] = {0};
	char param[128] = {0};
	char param1[128] = {0};
	int c;
	int index = 0;
	int pc = 0;
	int space = 0;
	
	int jumps[128] = {0};
	int jumps_t[128] = {0};
	char jumps_l[128][128];
	int jc = 0;
	
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i], "-v") == 0) printf(VERSION);
			else if(strcmp(argv[i], "-h") == 0) {
				printf("-v -> Show the assembler version\n");
				printf("-o -> Define the output file name\n");
				printf("-h -> Show all the avaliable flags\n");
			} else if(strcmp(argv[i], "-o") == 0) {
				if(i < argc) {
					exit_name = argv[i+1];
				} else {
					printf("Error> Please put a output file name after '-o'\n");
				}
			} else {
				printf("Unknow flag {%s}\n", argv[i]); 
			}
		}
	}
	
	unsigned char exit_code[65536] = {0};
	
	FILE *as = fopen(argv[1], "r");
	FILE *bin = fopen(exit_name, "wb");
	if(as == NULL || bin == NULL) {
		perror("Error:");
		exit(1);
	}
	
	while((c = fgetc(as)) != EOF) {
		if(isspace(c)) {
			space = 0;
			if(index > 0) {
				inst[index] = '\0';
				if(strlen(inst) > 0 && inst[strlen(inst)-1] == ':') {
					inst[strlen(inst)-1] = '\0';
					labels_data[lc] = pc;
					strcpy(labels[lc++], inst);
				} else if(strcmp(inst, "MVI") == 0) {
					GetParam(as, param);
					GetParam(as, param1);
					if(strcmp(param, "A") == 0) exit_code[pc++] = 0x3E;
					else if(strcmp(param, "B") == 0) exit_code[pc++] = 0x06;
					else if(strcmp(param, "C") == 0) exit_code[pc++] = 0x0E;
					else if(strcmp(param, "D") == 0) exit_code[pc++] = 0x16;
					else if(strcmp(param, "E") == 0) exit_code[pc++] = 0x1E;
					else if(strcmp(param, "H") == 0) exit_code[pc++] = 0x26;
					else if(strcmp(param, "L") == 0) exit_code[pc++] = 0x2E;
					else if(strcmp(param, "M") == 0) exit_code[pc++] = 0x36;
					else {
						printf("Unknow argument {%s}\n", param);
					}
					
					exit_code[pc++] = (unsigned char)strtol(param1, NULL, 0);
				} else if(strcmp(inst, "MOV") == 0) {
					GetParam(as, param);
					GetParam(as, param1);
					
					int dest_idx = GetRegIndex(param);
					int src_idx = GetRegIndex(param1);
					if(dest_idx != -1 && src_idx != -1) {
						exit_code[pc++] = 0x40 + (dest_idx * 8) + src_idx;
					} else printf("Unknow argument {%s}\n", param);
				
				} else if(strcmp(inst, "HLT") == 0) {
					exit_code[pc++] = 0x76;
				} else if(strcmp(inst, "ADD") == 0){
					GetParam(as, param);
					
					if(strcmp(param, "A") == 0) exit_code[pc++] = 0x87;
					else if(strcmp(param, "B") == 0) exit_code[pc++] = 0x80;
					else if(strcmp(param, "C") == 0) exit_code[pc++] = 0x81;
					else if(strcmp(param, "D") == 0) exit_code[pc++] = 0x82;
					else if(strcmp(param, "E") == 0) exit_code[pc++] = 0x83;
					else if(strcmp(param, "H") == 0) exit_code[pc++] = 0x84;
					else if(strcmp(param, "L") == 0) exit_code[pc++] = 0x85;
					else if(strcmp(param, "M") == 0) exit_code[pc++] = 0x86;
					else {
						printf("Unknow argument {%s}\n", param);
					}
				} else if(strcmp(inst, "SUB") == 0){
					GetParam(as, param);
					
					if(strcmp(param, "A") == 0) exit_code[pc++] = 0x97;
					else if(strcmp(param, "B") == 0) exit_code[pc++] = 0x90;
					else if(strcmp(param, "C") == 0) exit_code[pc++] = 0x91;
					else if(strcmp(param, "D") == 0) exit_code[pc++] = 0x92;
					else if(strcmp(param, "E") == 0) exit_code[pc++] = 0x93;
					else if(strcmp(param, "H") == 0) exit_code[pc++] = 0x94;
					else if(strcmp(param, "L") == 0) exit_code[pc++] = 0x95;
					else if(strcmp(param, "M") == 0) exit_code[pc++] = 0x96;
					else {
						printf("Unknow argument {%s}\n", param);
					}
				} else if(strcmp(inst, "JMP") == 0) {
					GetParam(as, param);
					
					jumps[jc] = pc;
					jumps_t[jc] = 0xC3;
					strcpy(jumps_l[jc++], param); 
					pc+= 3;
				} else if(strcmp(inst, "JNZ") == 0) {
					GetParam(as, param);
					
					jumps[jc] = pc;
					jumps_t[jc] = 0xC2;
					strcpy(jumps_l[jc++], param); 
					pc+= 3;
				} else if(strcmp(inst, "JZ") == 0) {
					GetParam(as, param);
					
					jumps[jc] = pc;
					jumps_t[jc] = 0xCA;
					strcpy(jumps_l[jc++], param); 
					pc+= 3;
				} else if(strcmp(inst, "CALL") == 0) {
					GetParam(as, param);
					
					jumps[jc] = pc;
					jumps_t[jc] = 0xCD;
					strcpy(jumps_l[jc++], param); 
					pc+= 3;
				} else {
					printf("Unknow instruction {%s}\n", inst);
				}
				
				index = 0;
			}
		} else {
			if(c == 32 && index == 0) space = 1;
			if(c != 32) {
					space = 0;
					inst[index++] = c;
			}
		}
	}
	
	int prog_size = pc;

	int jc_total = jc;
	jc = 0;
	while(jc < jc_total) {
		pc = jumps[jc];
		int addr = SearchLabel(jumps_l[jc]);
					
		if(addr == -1) {
			printf("Label {%s} not found!\n", jumps_l[jc]);
		} else {
			exit_code[pc++] = jumps_t[jc];
			exit_code[pc++] = addr & 0xFF;
			exit_code[pc++] = (addr >> 8) & 0xFF;
		}
		
		jc++;
	}
	
	fwrite(exit_code, sizeof(unsigned char), prog_size, bin);
	
	fclose(as);
	fclose(bin);
	return 0;
}
