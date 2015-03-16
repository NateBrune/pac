#!/usr/bin/c
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
using namespace std;

int shellcode(std::string file){
	FILE *sf; //executable file
	FILE *f; //shellcode.h
	int i, c;
	char *arr_name;
	sf = fopen(file.c_str(), "rb");
	if (sf == NULL) {
		fprintf(stderr, "fopen(%s) Failed. Could'nt open file.", file.c_str());
		return 1;
	}
	arr_name = "shellcode";
	f = fopen("shellcode.h","w");
	fprintf(f, "unsigned char %s[] = {", arr_name);
	for (i=0;;i++) { //grab the shellcode and drop it in
		if ((c = fgetc(sf)) == EOF) break; //shellcode.h
		if (i != 0) fprintf(f,",");
		if ((i % 12) == 0) fprintf(f,"\n\t");
		fprintf(f,"0x%.2X", (unsigned char)c);
	}
	fprintf(f,"\n\t};\n");
	fprintf(f,"unsigned int size = %i;\n", i);
	fclose(sf);
	fclose(f);
	return 0;
}

int main(int argc, char *argv[]){
	shellcode(argv[1]);
}
