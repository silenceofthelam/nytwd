#include "nytwd.h"
#include "dbg.h"

char *construct_response(int code)
{
	
	FILE *web_page = fopen("index.html", "r");
	fseek(web_page, 0, SEEK_END);
	long fsize = ftell(web_page);
	fseek(web_page, 0, SEEK_SET);

	char head[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";

	long int buffer_size = fsize + strlen(head) + 256;


	char *response = malloc(buffer_size);
	check_mem(response);

	long int position = 0;

	strncpy(&(response[position]), head, strlen(head));
	position += strlen(head);

	fread(&(response[position]), 1, fsize, web_page);
	
	fclose(web_page);

	change_vars(response, buffer_size, position);

	return response;

error:
	if(response) free(response);
	return NULL;

}


int change_vars(char *buffer, long int buffer_size, long int position)
{

      int i;
      for(i = 0; i < strlen(buffer); i++)
	{
	  if((buffer[i] == '#') && (buffer[i+1] == '#'))
	    {

	      int replace = i;

	      i++;
	      i++;

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

		   strncpy(&(var[j]), &(buffer[i]), 1);
		}
	      
	      debug("Found variable in index.html at position %d: %s", replace, var);

	      switch(strlen(var))
		{
		case 11:
		  if((strcmp(var, "CONNECTIONS"))== 0)
		    {
		      char connections[12];
		      snprintf(connections, 12, "%d", CUR_CONNECTIONS);
		      long cur_buffer_len = strlen(buffer);
		      memmove(&(buffer[replace + strlen(connections)]), &(buffer[replace + 14]), strlen(buffer) - i);
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
