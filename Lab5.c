#include <stdint.h> // Exact-width integer types

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
	char state[21];
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_processes, number_of_files;
	FILE *binaryfile;

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;

	printf("%s bytes\n", number_of_bytes);
	printf("%s processes\n", number_of_processes );

	binaryfile = fopen("processes.pro", "r+b")
	if (binaryfile == NULL)
	{
		
		printf("%s\n", "unable to open file" );
		return 0
	}
	//get state of current process
	printf("%s\n", "it worked!");
	fclose(binaryfile);
}