#include "nytwd.h"
#include "dbg.h"

char *read_file(char* filename);
long change_vars(char *line, char *buffer, long int buffer_size);


char *construct_response(int code)
{

  char* response;
  char* file_buffer;

  response = malloc(4096);
  check_mem(response);

	char head[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";


  file_buffer = read_file("index.html");

	strncpy(response, head, strlen(head));
  strncat(response, file_buffer, strlen(file_buffer));

	return response;

error:
	if(response) free(response);
	return NULL;

}


char *read_file(char *filename)
{

  FILE *web_page = fopen(filename, "r");
	check(web_page, "Could not open file");
	long FILE_SIZE = 2048;
	int LINE_SIZE = 128;

	long fsize = 0;

	char *response = malloc(FILE_SIZE);
	check_mem(response);

	char *line = malloc(LINE_SIZE);
	check_mem(line);

	while (fgets(line, sizeof line, web_page) != NULL)
	{
		fsize += change_vars(line, response, fsize);

		if(fsize > (FILE_SIZE - LINE_SIZE))
		{
			FILE_SIZE += 2048;
			response = realloc(response, FILE_SIZE);
			check_mem(response);
		}
	}

  fclose(web_page);

	return response;

error:
  if(web_page)
    fclose(web_page);

  if(response)
    free(response);

  if(line)
    free(line);

  return NULL;

}
		


long change_vars(char *line, char *buffer, long int buffer_size)
{
  long int cur_buffer_len = buffer_size;

      int i;
      for(i = 0; i < strlen(line); i++, buffer_size++)
	{
	  if((line[i] == '#') && (line[i+1] == '#'))
	    {

	      int replace = i;

	      i += 2;

	      char var[20];
	      memset(var, '\0', 20);
	      
	      int j;

	      for(j = 0; j < 20; j++, i++)
		{
		  if((buffer[i] == '#'))
		    {
		      var[j] = '\0';
		      i++;
		      break;
		    }

		   strncpy(&(var[j]), &(line[i]), 1);
		}
	      
	      debug("Found variable in index.html at position %ld: %s", (buffer_size + replace), var);

	      switch(strlen(var))
		{
		case 11:
		  if((strcmp(var, "CONNECTIONS"))== 0)
		    {
		      char connections[12];
		      snprintf(connections, 12, "%d", CUR_CONNECTIONS);
		      strncpy(&(buffer[replace]), connections, strlen(connections));
		      cur_buffer_len += strlen(connections) - 14;
		      buffer[cur_buffer_len] = '\0';
		      break;
		    }
		  
		  goto NOVAR;

		case 6:
		  if((strcmp(var, "UPTIME")) == 0)
		    {
		      char *uptime = get_uptime();
		      long cur_buffer_len = strlen(buffer);
		      memmove(&(buffer[replace + strlen(uptime)]), &(buffer[replace + 9]), strlen(buffer) - i);
		      strncpy(&(buffer[replace]), uptime, strlen(uptime));
		      cur_buffer_len += strlen(uptime) - 9;
		      buffer[cur_buffer_len] = '\0';
		      break;
		     }

		  goto NOVAR;

		NOVAR:
		default:
		  debug("Variable %s not recognized", var);
		}
	    }

	}


  return strlen(buffer);
}
