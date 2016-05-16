/********************************************************
*** Header file for declaration of class clExceptionTree
***
*** J.L. Klaufus - Haarlem 2000
**/

#ifndef EXCEPTION_TREE_H
#define EXCEPTION_TREE_H

#include <string>

#include "globals.h"
#include "exception.h"
#include "list.h"

class clExceptionTree : public clException
{
protected:
  typedef struct sMethodNameStruct
  {
    std::string methodName;
  } stMethodNameStruct;

  tpList<stMethodNameStruct> methodTree;

  void   PrintMethodTree(void) const;
  std::string PrintMethodTree(std::string) const;

public:
  clExceptionTree(const char*, const char*, const char*);
  clExceptionTree(const std::string, const std::string, const std::string);

  void        AddMethodToTree(const std::string);
  void        PrintError(void) const;
  std::string PrintError(std::string) const;
};

#endif
