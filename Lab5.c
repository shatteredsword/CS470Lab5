#include <stdint.h> // Exact-width integer types
#include <stdio.h>//file operations
#include <string.h>//used for string operations
#include <stdlib.h>//for manual memory allocation

#define TIME_QUANTUM 1
#define LENGTH_OF_DESCRIPTOR 36
#define BUFFSIZE 4

uint_fast16_t number_of_processes;
long int number_of_bytes = 0;

 long int file_length()
{
	long int length;
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	fseek(filepointer, 0, SEEK_END);
	length = ftell(filepointer);
	fclose(filepointer);
	return length;
}
void total_files()
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
	printf("file contains %i files\n\n", total);
}
char* get_status(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	char *buff = (char *) malloc(sizeof(char) * 21);
	int seeker = process * LENGTH_OF_DESCRIPTOR;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 20, 1, filepointer);
	//printf("%s\n", buff);
	fclose(filepointer);
	return buff;
}
uint32_t get_process_id(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	uint32_t buff[5];
	int seeker = process * LENGTH_OF_DESCRIPTOR + 20;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 4, 1, filepointer);
	fclose(filepointer);
	return buff[0];
}
uint32_t get_program_counter(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	uint32_t buff;
	uint32_t *ptr;
	ptr = &buff;
	int seeker = process * LENGTH_OF_DESCRIPTOR + 24;
	fseek(filepointer, seeker, SEEK_SET);
	fread(ptr, 4, 1, filepointer);
	fclose(filepointer);
	return buff;
}
uint32_t get_program_limit(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	uint32_t buff[5];
	int seeker = process * LENGTH_OF_DESCRIPTOR + 28;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 4, 1, filepointer);
	fclose(filepointer);
	return buff[0];
}


/*
uint_fast16_t get_remaining_processes()
{
	char buff[21];
	uint_fast16_t remaining_processes;
	int seeker = 32;
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");

	for (int i=0; i < number_of_processes - 1; i++)
	{
		fseek(filepointer, seeker, SEEK_SET);
		fread(buff, 20, 1, filepointer);
		seeker += LENGTH_OF_DESCRIPTOR;
		if(strstr(buff, "terminated") == NULL)
		{
			remaining_processes++;
		}
	}
	fclose(filepointer);
	return remaining_processes;
}
*/


void set_status(uint_fast16_t process, const char status[21])
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "r+b");
	fseek(filepointer, process * LENGTH_OF_DESCRIPTOR, SEEK_SET);
	fwrite(status, 20, 1, filepointer);
	fclose(filepointer);
}

void set_program_counter(uint_fast16_t process, uint32_t program_counter)
{
	FILE *filepointer;
	uint32_t *ptr = &program_counter;
	filepointer = fopen("processes.pro", "r+b");
	fseek(filepointer, process * LENGTH_OF_DESCRIPTOR + 24, SEEK_SET);
	fwrite(ptr, 4, 1, filepointer);
	fclose(filepointer);
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
	//close the file
	fclose(binaryfile);

	//check if file is a valid length
	number_of_bytes = file_length();
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
	total_files();



	/*
	char tempor[21] = "terminated";
	set_status(4, tempor);


	uint32_t temp[5];
	*temp = 4294967295;
	set_program_counter(4, temp);
	*/



	while (remaining_processes > 0)
	{
		
		//loop for all processes
		for (uint_fast16_t i = 0; i < number_of_processes; i++)
		{
			const char *status = get_status(i);
			const char *terminated = "terminated";
			uint32_t program_counter = get_program_counter(i);
			//printf("%s\n", status);
			if (strcmp(status, terminated) != 0){
				uint32_t process_id, program_counter, program_limit;
				process_id = get_process_id(i);
				program_counter = get_program_counter(i);
				program_limit = get_program_limit(i);
				program_counter += TIME_QUANTUM;
				if (program_counter >= program_limit)
				{
					program_counter = program_limit;
					set_program_counter(i, program_counter);
					remaining_processes--;
					status = "terminated";
					set_status(i, status);
				}

				else
				{
					set_program_counter(i, program_counter);
					status = "ready";
					set_status(i, status);
				}
				printf("process %i is %s and is at %i of %i\n", process_id, status, program_counter, program_limit);
			}
		}
		printf("%s\n\n", "PCB loop completed" );
	}
	printf("%s\n", "all processes terminated" );
}