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
void total_files(uint_fast16_t number_of_processes)
{
	int buff[5];
	int total = 0;
	int seeker = 32;
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");

	//sum up the "# of open files" category from the binary file
	for (int i=0; i < number_of_processes; i++)
	{
		fseek(filepointer, seeker, SEEK_SET);
		fread(buff, 4, 1, filepointer);
		seeker += LENGTH_OF_DESCRIPTOR;
		total += buff[0];
	}
	fclose(filepointer);
	printf("file contains %i files\n", total);
}
void round_robin(uint_fast16_t number_of_processes)
{

}
int main()
{
	//check if binary file can be opened
	FILE *binaryfile;
	binaryfile = fopen("processes.pro", "r+b");
	if (binaryfile == NULL)
	{
		printf("%s\n", "unable to open file" );
		return 0;
	}

	//check if file is a valid length
	long int number_of_bytes = file_length();
	if (number_of_bytes % LENGTH_OF_DESCRIPTOR != 0 || number_of_bytes == 0)
	{
		printf("%s\n", "file is missing information" );
		return 0;
	}

	//variable declaration
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_processes, number_of_files, remaining_processes;
	

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;
	//start a count of remaining processes
	remaining_processes = number_of_processes;

	//print total number of processes
	printf("file conatins %li processes\n", number_of_processes );
	//print total number of files
	total_files(number_of_processes);

	//close the file
	fclose(binaryfile);
}