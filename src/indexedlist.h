/**************************************************************
*** Header file for declaration of template class tpIndexedList
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef INDEXED_LIST_H
#define INDEXED_LIST_H

#include <string.h>

#include "globals.h"
#include "exception.h"
#include "list.h"

template <class LTYPE>
class tpIndexedList : public tpList<LTYPE>
{
private:
  stItem<LTYPE> **indexArray; // Array of pointers to stItem<LTYPE>
  void            BuildIndexArray(void);
  stItem<LTYPE>*  GetPointer(const int) const;

public:
  tpIndexedList(void);
  tpIndexedList(const tpIndexedList<LTYPE>&);
  ~tpIndexedList(void);

  int    AddItem(const LTYPE&);
  int    InsertItem(const int, const LTYPE&);
  int    DeleteItem(const int);
};

/**************************************************************
*** Body file for declaration of template class tpIndexedList
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <iostream>
#include <stdio.h>

#include "exception.h"
//#include "list.h"
//#include "indexedlist.h"

/*
** ---------------
** Private Methods
** ---------------
*/

template <class LTYPE>
stItem<LTYPE>* tpIndexedList<LTYPE>::GetPointer(const int position) const
{
  // Check for valid position
  if(position<0 || position>this->GetNumberItems()-1)
  {
    char message[MESLEN];
    sprintf(message, "Invalid position.\n  POS = %d\n  MAX = %d", position, this->GetNumberItems()-1);
    throw clException("tpIndexedList", "GetPointer", message);
  }

  return(indexArray[position]);
}

template <class LTYPE>
void tpIndexedList<LTYPE>::BuildIndexArray(void)
{
  if(indexArray)
  {
    delete [] indexArray;
    indexArray = NULL;
  }

  indexArray = new stItem<LTYPE> *[this->GetNumberItems()];
  indexArray[0] = this->GetHead();
  for(int i=1; i<this->GetNumberItems(); i++)
  {
    indexArray[i] = indexArray[i-1]->next;
  }
}

/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructor and destructor
*/
template <class LTYPE>
tpIndexedList<LTYPE>::tpIndexedList(void) : tpList<LTYPE>()
{
  indexArray = NULL;
}

template <class LTYPE>
tpIndexedList<LTYPE>::tpIndexedList(const tpIndexedList<LTYPE> &source) : tpList<LTYPE>(source)
{
  indexArray = NULL;
}

template <class LTYPE>
tpIndexedList<LTYPE>::~tpIndexedList(void)
{
  while(this->GetNumberItems()>0) DeleteItem(this->GetNumberItems()-1);

  if(indexArray) delete [] indexArray;

  // cerr << "DESTROYING LIST" << endl;
}

/*
** Methods for item manipulation
*/
template <class LTYPE>
int tpIndexedList<LTYPE>::AddItem(const LTYPE &object)
{
  tpList<LTYPE>::AddItem(object);
  BuildIndexArray();

  return(this->GetNumberItems()-1);
}

template <class LTYPE>
int tpIndexedList<LTYPE>::InsertItem(const int position, const LTYPE &object)
{
  tpList<LTYPE>::InsertItem(position, object);
  BuildIndexArray();

  return(position);
}

template <class LTYPE>
int tpIndexedList<LTYPE>::DeleteItem(const int position)
{
  tpList<LTYPE>::DeleteItem(position);
  BuildIndexArray();

  return(this->GetNumberItems());
}

//template <class LTYPE>
//int tpIndexedList<LTYPE>::AddItem(const LTYPE &object)
//{
//  int oldSize = this->GetNumberItems();
//
//  // Add object to list
//  stItem<LTYPE>* itemPointer = tpList<LTYPE>::AddItem2(object);
//
//  // Create new indexArray, copy contents from old indexArray
//  stItem<LTYPE> **newArray = new stItem<LTYPE> *[oldSize+1];
//  if(!newArray)
//  {
//    throw clException("tpIndexedList", "AddItem", "Could not allocate memory");
//  }
//
//  void* pointer = memcpy(newArray, indexArray, oldSize);
//
//  // Add new item to new indexArray
//  newArray[oldSize] = itemPointer;
//
//  // Delete old indexArray
//  if(indexArray)
//  {
//    delete [] indexArray;
//    indexArray = NULL;
//  }
//
//  // Redirect indexArray to newArray
//  indexArray = newArray;
//
//  return(this->GetNumberItems()-1);
//}

//template <class LTYPE>
//int tpIndexedList<LTYPE>::InsertItem(const int position, const LTYPE &object)
//{
//printf("7\n");
//  int oldSize = this->GetNumberItems();
//
//  // Insert item object in list
//printf("8\n");
//  stItem<LTYPE>* itemPointer = tpList<LTYPE>::InsertItem2(position, object);
//
//  // Create new indexArray
//printf("9\n");
//  stItem<LTYPE> **newArray = new stItem<LTYPE> *[oldSize+1];
//  if(!newArray)
//  {
//    throw clException("tpIndexedList", "InsertItem", "Could not allocate memory");
//  }
//
//  // Copy items till (not including) position from old indexArray to new indexArray
//printf("10\n");
//  memcpy(newArray, indexArray, position);
//
//  // Insert new item in new indexArray on position
//printf("11\n");
//  newArray[position] = itemPointer;
//
//  // Copy items position and upwards from old indexArray to new indexArray
//printf("12\n");
//  memcpy(newArray[position+1], indexArray[position], oldSize-position);
//
//  // Delete old indexArray
//printf("13\n");
//  if(indexArray)
//  {
//    delete [] indexArray;
//    indexArray = NULL;
//  }
//
//  // Redirect indexArray to newArray
//  indexArray = newArray;
//
////  BuildIndexArray();
//
//  return(position);
//}

//template <class LTYPE>
//int tpIndexedList<LTYPE>::DeleteItem(const int position)
//{
//printf("14\n");
//  int oldSize = this->GetNumberItems();
//
//  // Delete object from list
//printf("15\n");
//  tpList<LTYPE>::DeleteItem(position);
//
//  // Create new indexArray
//printf("16\n");
//  stItem<LTYPE> **newArray = new stItem<LTYPE> *[oldSize-1];
//  if(!newArray)
//  {
//    throw clException("tpIndexedList", "DeleteItem", "Could not allocate memory");
//  }
//
//  // Copy items till (not including) position from old indexArray to new indexArray
//printf("17\n");
//  memcpy(newArray, indexArray, position);
//
//  // Copy items position+1 and upwards from old indexArray to new indexArray
//printf("18\n");
//  memcpy(newArray[position], indexArray[position+1], oldSize-(position+1));
//
//  // Delete old indexArray
//printf("19\n");
//  if(indexArray)
//  {
//    delete [] indexArray;
//    indexArray = NULL;
//  }
//
//  // Redirect indexArray to newArray
//  indexArray = newArray;
//
////  BuildIndexArray();
//
//  return(this->GetNumberItems());
//}

#endif
