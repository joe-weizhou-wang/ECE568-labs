#include <stdio.h>
#include "ip_parser.h"

int main(int argc, char * argv[])
{
	FILE *fileptr = fopen(argv[1], "rb");   
 
	// write your code here
	if (!fileptr) {
		fprintf(stderr, "Failed to open file\n");
		exit(1);
	}

	if (fseek(fileptr, 0L, SEEK_END) != 0) {
		fprintf(stderr, "Failed to point to end of file\n");
		exit(1);
	}
	long int size = ftell(fileptr);
	if (size == -1L) {
		fprintf(stderr, "Failed to get file size\n");
		exit(1);
	}
	if (fseek(fileptr, 0L, SEEK_SET) != 0) {
		fprintf(stderr, "Failed to point to start of file\n");
		exit(1);
	}
	char *buffer = (char*)malloc(size);
	if (!buffer) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(1);
	}
	if (fread(buffer, 1, size, fileptr) != size) {
		fprintf(stderr, "Failed to read file\n");
		exit(1);
	}
	int return_code = ip_parse(buffer, size);
	if (return_code != SUCCESS) {
		fprintf(stderr, "%s\n", GET_ERROR_STRING(return_code));
	}
	fclose(fileptr);
	free(buffer);
	return 0;
}

