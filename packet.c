#include "nytwd.h"
#include "dbg.h"

char *construct_response(int code)
{
	
	FILE *web_page = fopen("index.html", "r");
	fseek(web_page, 0, SEEK_END);
	long fsize = ftell(web_page);
	fseek(web_page, 0, SEEK_SET);

	char head[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";

	int buffer_size = fsize + strlen(head) + 256;


	char *response = malloc(buffer_size);
	check_mem(response);

	int position = 0;

	strncpy(&(response[position]), head, strlen(head));
	position += strlen(head);

	char *rtn = read_file(web_page, fsize);

	while(rtn != NULL)
	  {
	    strncpy(&(response[position]), (char *)rtn, strlen((char *)rtn));
	    position += strlen(rtn);

	    rtn = read_file(web_page, fsize);
	  }
	
	fclose(web_page);

	return response;

error:
	if(response) free(response);
	return NULL;

}


char *read_file(FILE *file, long int fsize)
{

  long int fpos = ftell(file);
  int position = 0;
  char *mod_file;
  char *temp;

  mod_file = malloc(fsize - fpos);
  memset(mod_file, '\0', (fsize - fpos));

  temp = malloc(fsize - fpos);
  memset(temp, '\0', (fsize - fpos));

  while((temp = fgets(temp, (fsize - fpos), file)) != NULL)
    {
      int i;
      for(i = 0; i < strlen(temp); i++)
	{
	  if((temp[i] = '#') && (temp[i+1] = '#'))
	    {

	      int replace = i;

	      i++;
	      i++;

	      char var[20];
	      
	      int j;

	      for(j = 0; j < 20; j++, i++)
		{
		  if((temp[i] = '#'))
		    {
		      var[j] = '\0';
		      break;
		    }

		  strncpy(&(var[j]), &(temp[i]), 1);
		    
		}
	      
	      debug("Found variable in index.html: %s", var);

	      switch(strlen(var))
		{
		case 11:
		  if((strcmp(var, "CONNECTIONS"))== 0)
		    {
		      memmove(&(temp[replace]), &(temp[replace + 12]), strlen(temp) - (position + 12));
		      break;
		    }
		  
		  goto NOVAR;
    
		NOVAR:
		default:
		  debug("Variable %s not recognized", var);
		}
	      
	    }
	}

      strncpy(&(mod_file[position]), temp, strlen(temp));

      position += strlen(temp);
    }

  return mod_file;
}
