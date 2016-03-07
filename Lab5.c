#include <stdint.h> // Exact-width integer types
#include <stdio.h>//file operations
#include <string.h>//used for string operations
#include <stdlib.h>//for manual memory allocation

#define TIME_QUANTUM 1
#define LENGTH_OF_DESCRIPTOR 36

int main()
{
	FILE *filepointer;
	uint_fast16_t number_of_processes;
	long int number_of_bytes = 0;
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

	filepointer = fopen("processes.pro", "r+b");

	//check if file is a valid length
	fseek(filepointer, 0, SEEK_END);
	number_of_bytes = ftell(filepointer);
	if (number_of_bytes % LENGTH_OF_DESCRIPTOR != 0 || number_of_bytes == 0)
	{
		printf("%s\n", "file is missing information" );
		printf("%li bytes\n", number_of_bytes);
		return 0;
	}

	//variable declaration
	uint32_t open_files;
	uint_fast16_t number_of_files, process, remaining_processes;
	char status[21];

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;
	remaining_processes = number_of_processes;

	//print total number of processes
	printf("file conatins %li processes\n", number_of_processes);

	//printf("%li processes are remaining\n", remaining_processes);
	
	//print total number of files
	int buff[5];
	int total = 0;
	for (int i=0; i < number_of_processes; i++)//sum up the "# of open files" category from the binary file
	{
		fseek(filepointer, i * LENGTH_OF_DESCRIPTOR + 32, SEEK_SET);
		fread(buff, 4, 1, filepointer);
		total += buff[0];
	}
	printf("file contains %i files\n\n", total);

	while (remaining_processes > 0)
	{
		
		//loop for all processes
		for (uint_fast16_t i = 0; i < number_of_processes; i++)
		{
			const char *terminated = "terminated";
			//get status
			char *buff = (char *) malloc(sizeof(char) * 21);
			fseek(filepointer, i * LENGTH_OF_DESCRIPTOR, SEEK_SET);
			fread(buff, 20, 1, filepointer);
			const char *status = buff;
			if (strcmp(status, terminated) != 0){
				uint32_t process_id, program_counter, program_limit;

				//get process id
				uint32_t buff[5];
				fread(buff, 4, 1, filepointer);
				process_id = buff[0];

				//get program_counter
				fread(buff, 4, 1, filepointer);
				program_counter = buff[0];

				//get program_limit
				fread(buff, 4, 1, filepointer);
				program_limit = buff[0];

				program_counter += TIME_QUANTUM;
				if (program_counter >= program_limit)
				{
					program_counter = program_limit;
					remaining_processes--;
					status = "terminated";
				}
				else
				{
					status = "ready";
				}

				//set status
				fseek(filepointer, i * LENGTH_OF_DESCRIPTOR, SEEK_SET);
				fwrite(status, 20, 1, filepointer);

				//set program counter
				uint32_t *ptr = &program_counter;
				fseek(filepointer, i * LENGTH_OF_DESCRIPTOR + 24, SEEK_SET);
				fwrite(ptr, 4, 1, filepointer);

				//print status of process to screen
				printf("process %i is %s and is at %i of %i\n", process_id, status, program_counter, program_limit);
			}
		}
		printf("%s\n\n", "PCB loop completed" );
	}
	printf("%s\n", "all processes terminated" );
	fclose(filepointer);
}