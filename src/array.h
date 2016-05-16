/**********************************************************
*** Header file for declaration of template class tpArray
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef ARRAY_H
#define ARRAY_H

template <class ATYPE>
class tpArray
{
private:
  int    staticArray;
  int    arraySize;
  ATYPE  *items;

  int    SetItemStatic(const int, ATYPE);
  int    SetItemDynamic(const int, ATYPE);

public:
  tpArray(void);
  tpArray(int);
  ~tpArray(void);

  int   IsStatic(void) const;
  int   GetArraySize(void) const;

  int   SetItem(const int, ATYPE);
  ATYPE GetItem(const int) const;

  int   AddItem(ATYPE);

  ATYPE operator()(const int) const;
};

/**********************************************************
*** Body file for declaration of template class tpArray
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <stdio.h>

#include "exceptiontree.h"

/*
** ---------------
** Private Methods
** ---------------
*/

template <class ATYPE>
int tpArray<ATYPE>::SetItemStatic(const int position, ATYPE item)
{
  if(!staticArray)
  {
    throw clExceptionTree("tpArray", "SetItemStatic", "Not a static array.");
  }

  if(position>=arraySize)
  {
    throw clExceptionTree("tpArray", "SetItemStatic", "Array overflow.");
  }

  items[position] = item;

  return(position);
}

template <class ATYPE>
int tpArray<ATYPE>::SetItemDynamic(const int position, ATYPE item)
{
  if(staticArray)
  {
    throw clExceptionTree("tpArray", "SetItemDynamic", "Not a dynamic array.");
  }

  if(position>=arraySize)
  {
    // Create a new array
    ATYPE *newArray = new ATYPE[position+1];
    if(!newArray)
    {
      throw clExceptionTree("tpArray", "SetItemDynamic", "Could not allocate memory.");
    }

    // Copy the old array to the new array
    for(int i=0; i<arraySize; i++)
    {
      newArray[i] = items[i];
    }

    /*
    ** ATTENTION: Due to the unknown type of the items (ATYPE), the items
    **            between arraySize and position-1 can not be reset.
    **            It is assumed that the void-constructor of the items fulfills
    **            this task.
    */

    // Set the new item
    newArray[position] = item;

    // Set the new arraySize
    arraySize = position + 1;

    // Delete the old array
    if(items) delete [] items;

    // Redirect the pointer
    items = newArray;
  }
  else
  {
    items[position] = item;
  }

  return(position);
}


/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructor and destructor
*/
template <class ATYPE>
tpArray<ATYPE>::tpArray(void)
{
  // Creates a DYNAMIC stack
  staticArray = 0;
  arraySize   = 0;
  items       = NULL;
}

template <class ATYPE>
tpArray<ATYPE>::tpArray(int size)
{
  // Creates a STATIC stack
  staticArray = 1;
  arraySize   = size;
  items       = new ATYPE[size];
}

template <class ATYPE>
tpArray<ATYPE>::~tpArray(void)
{
  if(items) delete [] items;

  // cerr << "DESTROYING ARRAY" << endl;
}

/*
** Query Methods
*/
template <class ATYPE>
int tpArray<ATYPE>::IsStatic(void) const
{
  return(staticArray);
}

template <class ATYPE>
int tpArray<ATYPE>::GetArraySize(void) const
{
  return(arraySize);
}

/*
** Set and Get Methods
*/
template <class ATYPE>
int tpArray<ATYPE>::SetItem(const int position, ATYPE item)
{
  int ret;

  if(staticArray)
  {
    ret = SetItemStatic(position, item);
  }
  else
  {
    ret = SetItemDynamic(position, item);
  }

  return(ret);
}

template <class ATYPE>
int tpArray<ATYPE>::AddItem(ATYPE item)
{
  int ret;
  int position = arraySize;

  if(staticArray)
  {
    throw clExceptionTree("tpArray", "AddItem", "Can not ADD to static array.");
  }
  else
  {
    ret = SetItemDynamic(position, item);
  }

  return(ret);
}

template <class ATYPE>
ATYPE tpArray<ATYPE>::GetItem(const int position) const
{
  if(position<0 || position>=arraySize)
  {
    char message[MESLEN];
    sprintf(message, "Requested position beyond array boundaries.\n  ENTRY = %d\n", position);
    throw clExceptionTree("tpArray", "GetItem", message);
  }

  return(items[position]);
}

template <class ATYPE>
ATYPE tpArray<ATYPE>::operator()(const int position) const
{
  return(GetItem(position));
}


#endif
