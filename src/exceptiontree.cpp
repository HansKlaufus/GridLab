/*****************************************************
*** Body file for definition of class clExceptionTree
***
*** J.L. Klaufus - Haarlem 2000
**/

#include <iostream>
#include <sstream>
#include <string.h>

#include "str.h"

#include "exceptiontree.h"

using namespace std;

clExceptionTree::clExceptionTree(const char *clName, const char *mtName, const char *mes)
{
  className    = string(clName);
  methodName   = string(mtName);
  errorMessage = string(mes);

  //clException(clName, mtName, mes);
}

clExceptionTree::clExceptionTree(const string clName, const string mtName, const string mes)
{
  className    = clName;
  methodName   = mtName;
  errorMessage = mes;

  //clException(clName, mtName, mes);
}

void clExceptionTree::PrintMethodTree(void) const
{
  string spaceStr = "  ";
  //char arrowStr[] = "->";

  cerr << "**************************************************" << endl;
  cerr << "CALL SEQUENCE:" << endl;

  int numberCalls=methodTree.GetNumberItems()-1;
  for(int i=numberCalls; i>=0; i--)
  {
    if(i<numberCalls)
    {
      for(int j=i; j<numberCalls; j++)
      {
        cerr << spaceStr;
      }

      //cerr << arrowStr;
    }

    stMethodNameStruct *methodNameStruct=methodTree.GetItemPointer(i);
    cerr << methodNameStruct->methodName << endl;
  }
}

string clExceptionTree::PrintMethodTree(string mes) const
{
  stringstream message;

  char spaceStr[] = "  ";
  //char arrowStr[] = "->";

  message << "**************************************************" << endl;
  message << "CALL SEQUENCE:" << endl;

  int numberCalls=methodTree.GetNumberItems()-1;
  for(int i=numberCalls; i>=0; i--)
  {
    if(i<numberCalls)
    {
      for(int j=i; j<numberCalls; j++)
      {
        message << spaceStr;
      }

      //string = stradd(string, arrowStr);
    }

    stMethodNameStruct *methodNameStruct=methodTree.GetItemPointer(i);
    message << methodNameStruct->methodName << endl;
  }

  mes = message.str();

  return(mes);
}

void clExceptionTree::AddMethodToTree(string methodName)
{
  stMethodNameStruct methodNameStruct;
  methodNameStruct.methodName = methodName;

  methodTree.AddItem(methodNameStruct);
}

void clExceptionTree::PrintError(void) const
{
  cerr << endl;
  cerr << "**************************************************" << endl;
  cerr << "ERROR:" << endl;
  cerr << "  CLASS  : " << className << endl;
  cerr << "  METHOD : " << methodName << endl;
  cerr << "  MESSAGE: " << errorMessage << endl;
  cerr << endl;

  PrintMethodTree();

  cerr << endl;
  cerr << "**************************************************" << endl;
  cerr << endl;
}

string clExceptionTree::PrintError(string mes) const
{
  stringstream message;

  message << endl;
  message << "**************************************************" << endl;
  message << "ERROR:" << endl;

  message << "  CLASS  : "<< className<< endl;
  message << "  METHOD : " << methodName << endl;
  message << "  MESSAGE: " << errorMessage << endl;

  message << PrintMethodTree(message.str()) << endl;
  message << "**************************************************" << endl << endl;

  return(message.str());
}

