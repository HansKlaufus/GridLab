/******************************************************
*** Header file for declaration of class clException
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class clException
{
protected:
  std::string className;
  std::string methodName;
  std::string errorMessage;

public:
  clException(void);
  clException(const char*, const char*, const char*);
  clException(const std::string, const std::string, const std::string);
  void PrintError(void) const;
};

#endif
