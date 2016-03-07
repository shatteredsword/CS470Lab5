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
void get_status(uint_fast16_t number_of_processes, uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	char buff[21];
	int seeker = process * LENGTH_OF_DESCRIPTOR;
		fseek(filepointer, seeker, SEEK_SET);
		fread(buff, 4, 1, filepointer);
		printf("%s\n", buff);
	fclose(filepointer);
}


/*void round_robin(uint_fast16_t number_of_processes)
{
	uint_fast16_t remaining_processes;
	remaining_processes = number_of_processes;

	while (remaining_processes > 0)
	{
		//get status of process


		//skip process if terminated
		if (state != "terminated"){

			//do stuff

			//increment program counter
			program_counter += TIME_QUANTUM;
			if (program_counter >= program_limit)
			{
				state == "terminated";
				remaining_processes--;
				//update state

			}
		}
	}
}*/
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
	//close the file
	fclose(binaryfile);

	//check if file is a valid length
	long int number_of_bytes = file_length();
	if (number_of_bytes % LENGTH_OF_DESCRIPTOR != 0 || number_of_bytes == 0)
	{
		printf("%s\n", "file is missing information" );
		return 0;
	}

	//variable declaration
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_processes, number_of_files, remaining_processes, process;

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;
	//start a count of remaining processes
	remaining_processes = number_of_processes;

	//print total number of processes
	printf("file conatins %li processes\n", number_of_processes );
	//print total number of files
	total_files(number_of_processes);

	//get status of first process
	process = 0;
	get_status(number_of_processes, process);

}