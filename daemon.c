#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static char * getJSON_ElementString( char * element_name, char * string);

int main(int argc, char* argv[])
{
	FILE *fp_log = NULL;
	FILE *fp_commissioning = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	char buffer[500];
	char * state_string = "\"state\"";
	char * state_string_AP = "\"AP\"";
	char * state_string_client = "\"client\"";
	char * token_string = "\"token\"";
	char * ssid_string = "\"ssid\"";
	char * pass_phrase_string = "\"pass_phrase\"";
	char * security_mode_string = "\"security_mode\"";

	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0)
	{
		printf("fork failed!\n");
		// Return failure in exit status
		exit(1);
	}

	// PARENT PROCESS. Need to kill it.
	if (process_id > 0)
	{
		printf("process_id of child process %d \n", process_id);
		// return success in exit status
		exit(0);
	}

	//unmask the file mode
	umask(0);

	//set new session
	sid = setsid();
	if(sid < 0)
	{
		// Return failure
		exit(1);
	}
	// Change the current working directory to root.
	//chdir("/");


	// Open a log file in write mode.
	fp_log = fopen ("Loggers.txt", "w+");
	fp_commissioning = fopen ("hellothere.txt", "r+");
	fseek(fp_commissioning, SEEK_SET, 0);

	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	fread(buffer, 1, 500, fp_commissioning);
	fprintf(fp_log, buffer);


//	printf("Here is our state element [%s]\r\n", getJSON_ElementString(state_string, buffer));
//	printf("Here is our token element [%s]\r\n", getJSON_ElementString(token_string, buffer));
//	printf("Here is our ssid element [%s]\r\n", getJSON_ElementString(ssid_string, buffer));
//	printf("Here is our pass_phrase element [%s]\r\n", getJSON_ElementString(pass_phrase_string, buffer));
//	printf("Here is our security_mode element [%s]\r\n", getJSON_ElementString(security_mode_string, buffer));

	char * current_state = NULL;

	while (1)
	{
		//Dont block context switches, let the process sleep for some time
		sleep(1);
		//fprintf(fp_log, "Logging info...\n");
		//fflush(fp_log);
		fp_commissioning = fopen ("hellothere.txt", "r+");
		fseek(fp_commissioning, SEEK_SET, 0);
		fread(buffer, 1, 500, fp_commissioning);
		fclose(fp_commissioning);

		current_state = getJSON_ElementString(state_string, buffer);
		if(0 == strncmp(current_state, state_string_client, strlen(state_string_client)))
		{
			fprintf(fp_log, "~~~~~~~~~ State changed to client mode!!~~~~~~~~~~~~~~~\n");
		}

		// Implement and call some function that does core work for this daemon.
	}

	fclose(fp_log);
	fclose(fp_commissioning);
	return (0);
}

/**
 *
 * @param element_name
 * @return
 */
static char * getJSON_ElementString( char * element_name, char * string)
{
	char * return_pointer;
	static char return_string[500];
	char buffer[500];
	unsigned char counter = 0;

	strncpy(buffer, string, 500);

	return_pointer = strtok (buffer,":,} ");

	while ((return_pointer != NULL) && (counter != 1))
	{
		return_pointer = strtok (NULL,":,} ");
		if(0 == strncmp(element_name, return_pointer, strlen(element_name)))
		{
			counter = 1;

		}
	}

	return_pointer = strtok (NULL,":,} ");
	memset(return_string, 0x00, 500);
	strncpy(return_string, return_pointer, strlen(return_pointer));

	return(return_string);
}
