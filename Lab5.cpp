#include <stdint.h> // Exact-width integer types
#include <iostream>
#include <fstream>

using namespace std;

#define TIME_QUANTUM 1
#define LENGTH_OF_DESCRIPTOR 36

streamoff file_length() //taken directly from http://www.cplusplus.com/doc/tutorial/files/
{
	streampos begin, end;
	ifstream myfile ("processes.pro", ios::binary);
	begin = myfile.tellg();
	myfile.seekg (0, ios::end);
  	end = myfile.tellg();
  	myfile.close();
  	return end - begin;
}
int main()
{
	//check if file is a valid length
	streamoff number_of_bytes = file_length();
	if (number_of_bytes % LENGTH_OF_DESCRIPTOR != 0)
	{
		cout << "file is missing information\n";
		return 0;
	}
	//variable declaration
	char state[21];
	uint32_t process_id, program_counter, program_limit, open_files;
	uint_fast16_t number_of_processes, number_of_files;

	//variable initialization
	number_of_processes = number_of_bytes / LENGTH_OF_DESCRIPTOR;

	cout << number_of_bytes << " bytes\n";
	cout << number_of_processes << " processes\n";

	fstream binaryfile ("processes.pro", ios::binary);
	if (binaryfile.is_open())
	{
		//get state of current process
		cout << "it worked!";
		 binaryfile.close();
	}
	else cout << "unable to open file\n";
	return 0
}