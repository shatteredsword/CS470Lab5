#include <stdint.h> // Exact-width integer types
#include <stdio.h>//file operations

#define TIME_QUANTUM 1
#define LENGTH_OF_DESCRIPTOR 36

 long int file_length()
{
	long int length;
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	fseek(filepointer, 0L, SEEK_END);
	length = ftell(filepointer);
	fclose(filepointer);
	return length;
}
void total_files()
{
	int buff[5];
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	//figure out total number of files
	fseek(filepointer, 32L, SEEK_SET);
	fread(buff, 4, 1, filepointer);
	fclose(filepointer);
	printf("file contains %i\n files", buff[0]);
}





int main()
{
	//check if file is a valid length
	long int number_of_bytes = file_length();
	if (number_of_bytes % LENGTH_OF_DESCRIPTOR != 0)
	{
		printf("%s\n", "file is missing information" );
		return 0;
	}

	//variable declaration
	char state[20];
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_processes, number_of_files;
	FILE *binaryfile;

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;

	printf("file contains %li bytes\n", number_of_bytes);
	printf("file conatins %li processes\n", number_of_processes );

	binaryfile = fopen("processes.pro", "r+b");
	if (binaryfile == NULL)
	{
		
		printf("%s\n", "unable to open file" );
		return 0;
	}
	//print number of processes
	total_files();
	//get state of current process
	
	fclose(binaryfile);
}