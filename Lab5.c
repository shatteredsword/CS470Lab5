#include <stdint.h> // Exact-width integer types
#include <stdio.h>//file operations
#include <string.h>//used for string operations

#define TIME_QUANTUM 1
#define LENGTH_OF_DESCRIPTOR 36
#define BUFFSIZE 4

uint_fast16_t number_of_processes, remaining_processes;
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
	printf("file contains %i files\n", total);
}
char get_status(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	char buff[21];
	int seeker = process * LENGTH_OF_DESCRIPTOR;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 20, 1, filepointer);
	printf("%s\n", buff);
	fclose(filepointer);
}
uint32_t get_process_id(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	uint32_t buff[5];
	int seeker = process * LENGTH_OF_DESCRIPTOR + 20;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 4, 1, filepointer);
	printf("%i\n", buff[0]);
	fclose(filepointer);
	return buff[0];
}
uint32_t get_program_counter(uint_fast16_t process)
{
	FILE *filepointer;
	filepointer = fopen("processes.pro", "rb");
	uint32_t buff[5];
	int seeker = process * LENGTH_OF_DESCRIPTOR + 24;
	fseek(filepointer, seeker, SEEK_SET);
	fread(buff, 4, 1, filepointer);
	fclose(filepointer);
	return buff[0];
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
void set_status(uint_fast16_t process, char status[21])
{
	char buffer[BUFFSIZE + 1];
	FILE *filepointer, *bufferfile;

	//copy contents of processes.pro to buffer.bin using a buffer
	filepointer = fopen("processes.pro", "rb");
	bufferfile = fopen("buffer.bin", "ab");
	for(long int i=0; i < number_of_bytes / BUFFSIZE; i++)
	{
		fread(buffer, BUFFSIZE, 1, filepointer);
		fwrite(buffer, BUFFSIZE, 1, bufferfile);
	}
	fclose(filepointer);
	fclose(bufferfile);

	//delete contents of processes.pro 
	filepointer = fopen("processes.pro", "wb");
	fclose(filepointer);

	//copy up to status back to processes.pro
	filepointer = fopen("processes.pro", "ab");
	bufferfile = fopen("buffer.bin", "rb");

	//leave i intact for next loop
	long int i = 0;
	while (i < process * LENGTH_OF_DESCRIPTOR / BUFFSIZE)
	{
		fread(buffer, BUFFSIZE, 1, bufferfile);
		fwrite(buffer, BUFFSIZE, 1, filepointer);
		i++;
	}
	//append status to processes.pro
	fwrite(status, 20, 1, filepointer);

	//move file pointer in buffer.bin to after the status
	fseek(bufferfile, 20, SEEK_CUR);
	i += 5;

	//append remaining contents of buffer.bin (minus the old status) to processes.pro

	while (i < number_of_bytes / BUFFSIZE)
	{
		fread(buffer, BUFFSIZE, 1, bufferfile);
		fwrite(buffer, BUFFSIZE, 1, filepointer);
		i++;
	}
	fclose(filepointer);
	fclose(bufferfile);

	//delete contents of buffer.bin
	bufferfile = fopen("buffer.bin", "wb");
	fclose(bufferfile);
}
void set_program_counter(uint_fast16_t process, uint32_t program_counter[5])
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
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_files, process;
	char status[21];

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;
	
	/*
	set_status(0, "terminated");
	*/

	//start a count of remaining processes
	
	/*
	remaining_processes = get_remaining_processes();
	*/
	remaining_processes = number_of_processes;

	//print total number of processes
	printf("file conatins %li processes\n", number_of_processes);

	printf("%li processes are remaining\n", remaining_processes);
	//print total number of files
	total_files();
	char temp[21] = "terminated";
	set_status(4, temp);


	/*while (remaining_processes > 0)
	{
		//loop for all processes
		for (uint_fast16_t i = 0; i < number_of_processes; i++)

			status = get_status(i);
			if (status != "terminated" && status != "blocked"){
				process_id = get_process_id(i);
				program_counter = get_program_counter(i);
				program_limit = get_program_limit(i);
				program_counter += TIME_QUANTUM;
				if (program_counter >= program_limit)
				{
					program_counter = program_limit;
					status = "terminated";
					remaining_processes--;
				}
				set_status(i, status);
				set_program_counter(i, program_counter);
				printf("proess %i is %s and is at %i of %i\n", process_id, status, program_counter, program_limit);

			}
		}
	}*/
}