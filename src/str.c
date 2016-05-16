/*********************************************
*** Body file for string convertion functions
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "str.h"

/*
** Finds the location of a specified character in a string
*/
int chrloc(const char chr, const char *string)
{
  int i;
  int loc;

  loc = -1;
  for(i=0; i<(int)(strlen(&(*string))); i++)
  {
    if (string[i] == chr)
    {
      loc = i;
    }
  }

  return(loc);
}

/*
** Concatenates two strings
*/
char* stradd(const char *string1, const char *string2)
{
  char *dummy = NULL;
  int  length = 0;

  if(string1) length += (int)strlen(string1);
  if(string2) length += (int)strlen(string2);

  // Allocate memory
  dummy = (char*)malloc((length+1)*sizeof(char));
  if(!dummy)
  {
    fprintf(stderr, "ERROR in function stradd: Could not allocate memory.\n");
  }
  else
  {
    if(string1)
    {
      strcpy(dummy, string1);
      if(string2) strcat(dummy, string2);
    }
    else
    {
      if(string2) strcpy(dummy, string2);
    }
    dummy[length] = '\0';
  }

  return(dummy);
}

/*
** Truncates preceding and following spaces from string
*/
char* strtrim(const char *string)
{
  int  i, j;
  int  s, e;
  char *dummy = NULL;

  if(string)
  {
    // Find start point
    i = 0;
    while(i<(int)(strlen(string)) && string[i] == ' ')
    {
      i++;
    }
    s = i;

    // Find end point
    while(i<(int)(strlen(string)) && string[i] != ' ')
    {
      i++;
    }
    e = i;

    // Allocate memory
    dummy = (char*)malloc((e-s+1)*sizeof(char));
    if(!dummy)
    {
      fprintf(stderr, "ERROR in function strtrim: Could not allocate memory.\n");
    }
    else
    {
      // Copy string
      j = 0;
      for(i=s; i<e; i++)
      {
        dummy[j] = string[i];
        j++;
      }

      // Add null-terminator
      dummy[j] = '\0';
    }
  }

  //fprintf(stderr, "TRIM: Orig: [%s] Trimmed: [%s]\n", string, dummy);

  return(dummy);
}

int str2int2(const char *string)
{
  char *dummy = NULL;
  int  error;
  int  i;
  int  length;
  int  negative; // 1 = negative; 0 = positive;
  int  value;

  if(string)
  {
    // Trim the string; automatically allocates memory
    dummy = strtrim(string);

    length = strlen(dummy);
    negative = 0;
    value  = 0;
    error  = 0;
    for(i=0; i<length && error==0 ; i++)
    {
      switch(dummy[i])
      {
        case '+':
          negative = 0;
          break;
        case '-':
          negative = 1;
          break;
        case '0':
          value += (int)(0*pow(10, length-i-1));
          break;
        case '1':
          value += (int)(1*pow(10, length-i-1));
          break;
        case '2':
          value += (int)(2*pow(10, length-i-1));
          break;
        case '3':
          value += (int)(3*pow(10, length-i-1));
          break;
        case '4':
          value += (int)(4*pow(10, length-i-1));
          break;
        case '5':
          value += (int)(5*pow(10, length-i-1));
          break;
        case '6':
          value += (int)(6*pow(10, length-i-1));
          break;
        case '7':
          value += (int)(7*pow(10, length-i-1));
          break;
        case '8':
          value += (int)(8*pow(10, length-i-1));
          break;
        case '9':
          value += (int)(9*pow(10, length-i-1));
          break;
        case '.':
          fprintf(stderr, "ERROR in function str2int: Not an integer value.\n");
          error = 1;
          break;
        default:
          fprintf(stderr, "ERROR in function str2int: Unknown character '%c'.\n", dummy[i]);
          error = 1;
          break;
      }
    }

    if(error == 1)
    {
      value = 0;
    }
    else if(negative == 1)
    {
      value = -value;
    }

    // Free allocated memory
    if (dummy)
      free(dummy);
  }

  //fprintf(stderr, "STR2INT: String: [%s] Value: [%d]\n", string, value);

  return(value);
}

int str2int(const char *string)
{
  char   *dummy = NULL;
  int     error;
  int     i;
  int     length;
  int     negative; // 1 = negative; 0 = positive;
  double  value;

  if(string)
  {
    // Trim the string; automatically allocates memory
    dummy    = strtrim(string);
    length   = strlen(dummy);
    negative = 0;
    value    = 0.0f;
    error    = 0;
    for(i=0; i<length && error==0 ; i++)
    {
      switch(dummy[i])
      {
        case '+':
          negative = 0;
          break;
        case '-':
          negative = 1;
          break;
        case '0':
          value += (0*pow(10, length-i-1));
          break;
        case '1':
          value += (1*pow(10, length-i-1));
          break;
        case '2':
          value += (2*pow(10, length-i-1));
          break;
        case '3':
          value += (3*pow(10, length-i-1));
          break;
        case '4':
          value += (4*pow(10, length-i-1));
          break;
        case '5':
          value += (5*pow(10, length-i-1));
          break;
        case '6':
          value += (6*pow(10, length-i-1));
          break;
        case '7':
          value += (7*pow(10, length-i-1));
          break;
        case '8':
          value += (8*pow(10, length-i-1));
          break;
        case '9':
          value += (9*pow(10, length-i-1));
          break;
        case '.':
          fprintf(stderr, "ERROR in function str2int: Not an integer value.\n");
          error = 1;
          break;
        default:
          fprintf(stderr, "ERROR in function str2int: Unknown character '%c'.\n", dummy[i]);
          error = 1;
          break;
      }
    }

    if(error == 1)
    {
      value = 0.0f;
    }
    else if(negative == 1)
    {
      value = -1.0f*value;
    }

    // Free allocated memory
    if (dummy)
      free(dummy);
  }

  //fprintf(stderr, "STR2INT: String: [%s] Value: [%d]\n", string, value);

  return((int)(value));
}

double str2double(const char *string)
{
  char   *dummy = NULL;
  char   *substr1 = NULL;
  char   *substr2 = NULL;

  int    length;
  int    i;

  int    negative;

  int    e_loc;
  int    d_loc;

  double value;
  double value1;
  double value2;

  if(string)
  {
    // Trim string
    dummy  = strtrim(string);
    length = strlen(dummy);

    // Search for 'e' or 'E'
    e_loc = chrloc('e', dummy);
    if(e_loc==-1)
    {
      e_loc = chrloc('E', dummy);
    }

    if(e_loc!=-1)
    {
      // 'E' symbol found

      // Create 2 substrings
      substr1 = (char*)malloc((e_loc+1)*sizeof(char));
      substr2 = (char*)malloc((length-e_loc)*sizeof(char));

      if(substr1 && substr2)
      {
        // Copy
        for(i=0; i<e_loc; i++)
        {
          substr1[i] = dummy[i];
        }
        substr1[i] = '\0';

        for(i=e_loc+1; i<length; i++)
        {
          substr2[i-e_loc-1] = dummy[i];
        }
        substr2[i-e_loc-1] = '\0';

        // Recursive calls for substrings
        value1 = str2double(substr1);
        value2 = str2double(substr2);

        // Free allocated memory
        if(substr1)
          free(substr1);

        if(substr2)
          free(substr2);

        // Compose final value
        value = value1*pow(10, value2);
      }
      else
      {
        fprintf(stderr, "ERROR in function str2double: Could not allocate memory.\n");
        value = (double) (0.0);
      }
    }
    else
    {
      // 'E' symbol NOT found

      // Search for decimal point
      d_loc = chrloc('.', dummy);

      if(d_loc!=-1)
      {
        // Decimal point found

        // Check for '-' sign
        negative = chrloc('-', dummy) != -1 ? 1 : 0;

        // Create 2 substrings
        substr1 = (char*)malloc((d_loc+1)*sizeof(char));
        substr2 = (char*)malloc((length-d_loc)*sizeof(char));

        if(substr1 && substr2)
        {
          // Copy
          for(i=0; i<d_loc; i++)
          {
            substr1[i] = dummy[i];
          }
          substr1[i] = '\0';

          for(i=d_loc+1; i<length; i++)
          {
            substr2[i-d_loc-1] = dummy[i];
          }
          substr2[i-d_loc-1] = '\0';

          // Create partial values
          value1 = (double)(str2int(substr1));
          value2 = (double)(str2int(substr2));

          // Value2 is value behind decimal point: correct
          for(i=0; i<(int)(strlen(substr2)); i++)
          {
            value2 = value2/10.0;
          }

          // Compose final value
          if(negative==1)
          {
            value = value1-value2;
          }
          else
          {
            value = value1+value2;
          }

          // Free allocated memory
          if(substr1)
            free(substr1);

          if(substr2)
            free(substr2);
        }
        else
        {
          fprintf(stderr, "ERROR in function str2double: Could not allocate memory.\n");
          value = (double) (0.0);
        }
      }
      else
      {
        //Decimal point NOT found
        value = (double) (str2int(dummy));
      }
    }
  }

  // Free allocated memory
  if(dummy)
    free(dummy);

  //fprintf(stderr, "STR2DOUBLE: String: [%s] Value: [%f]\n", string, value);

  return(value);
}

