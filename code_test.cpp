/*
* 
* README
* 
* If you have any questions, or something does not work as expected - please let us know!
* 
* To build and run the project, click either the hollow green play button to build without debug
* or the Local Windows Debugger to build with the ability to debug your program.
* 
* This is a small interactive command line application that uses a simple text API to set
* and get some values. Once the program is running, simply enter the commands as detailed below.
* 
* The API is of the format:
* "(GET|SET) <parameter> [<param value 1>]\n" 
* 
* This API currently only has one command:
* SET SYS_FRATE N
* Where N is a valid frame rate number that the API can accept
* 
* For example:
* SET SYS_FRATE 50
* will set the system frame rate to 50
* 
* GET SYS_FRATE
* will print out the current frame rate
* 
* SET SYS_FRATE 49
* will return an error number
* 
* 
* 
* There are a few tasks to complete in order, please do as many as you can in around 1-2 hours maximum.
* Once you are done, please zip up the project folder and send it back to us! Thanks!
* 
* 1) There are some 'magic numbers' used in the array allocations and memset functions below. Change this 
*    to be safer and more programmer-friendly
* 
* 2) Improve the feedback of the 'error' from when an invalid frame rate is entered
* 
* 3) Add a valid frame rate of '48' to the system (so that 'SET SYS_FRATE 48' will work)
* 
* 4) In the 'vfc_sys_if_cfg' struct there is a video_enabled flag. Add an API command to be able to GET this value
* 
* 5) Add a SET command to set the video_enabled flag to 0 or 1
* 
*/

#include <iostream>
#include <code_test.h>
#include <cmd_handle.h>


int main()
{
	/* Main configuration struct*/
	vfc_sys_if_cfg* cfg;

	cfg = (vfc_sys_if_cfg*)malloc(sizeof(*cfg));
	/* Clear out entire struct */
	memset(cfg, 0, sizeof(*cfg));

	/* Process and wait for user input commands */
	while (1)
	{
		char buffer[128];
		char command[64];
		int bytes_read = 0;
		int err = 0;

		memset(buffer, 0, 128);
		memset(command, 0, 64);

		fgets(buffer, 128, stdin);

		for (int i = 0; i < 128; i++)
		{
			if (buffer[i] == '\n')
			{
				bytes_read = i;
				buffer[i] = '\0';
				break;
			}
		}

		err = get_buffer_command(buffer, command, 64);
		if (err != 0)
			printf("error: %d\n", err);

		err = process_input_cmd(cfg, command, buffer, bytes_read);
		if (err != 0)
			printf("error: %d\n", err);

	}
}

