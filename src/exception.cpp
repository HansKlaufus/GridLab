/***************************************************
*** Body file for definition of class clException
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <iostream>
#include <string>
#include <string.h>

#include "exception.h"

using namespace std;

clException::clException(void)
{
  className    = "Unknown";
  methodName   = "Unknown";
  errorMessage = "None";
}

clException::clException(const char *clName, const char *mtName, const char *mes)
{
  className    = string(clName);
  methodName   = string(mtName);
  errorMessage = string(mes);
}

clException::clException(const string clName, const string mtName, const string mes)
{
  className    = clName;
  methodName   = mtName;
  errorMessage = mes;
}

void clException::PrintError(void) const
{
  cerr << endl;
  cerr << "**************************************************" << endl;
  cerr << "ERROR" << endl;
  cerr << "  CLASS  : " << className << endl;
  cerr << "  METHOD : " << methodName << endl;
  cerr << "  MESSAGE: " << errorMessage << endl;
  cerr << "**************************************************" << endl;
  cerr << endl;
}

