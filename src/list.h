/**********************************************************
*** Header file for declaration of template class tpList
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef LIST_H
#define LIST_H

#include "globals.h"
#include "exception.h"

template <class LTYPE>
struct stItem
{
  stItem<LTYPE> *next;
  stItem<LTYPE> *prev;
  LTYPE          object;
};

template <class LTYPE>
class tpList
{
private:
  int                    numItems;
  stItem<LTYPE>         *head;
  stItem<LTYPE>         *tail;

  virtual stItem<LTYPE> *GetPointer(const int) const;

protected:
  stItem<LTYPE> *GetHead(void) const;
  stItem<LTYPE> *GetTail(void) const;

  stItem<LTYPE> *AddItem2(const LTYPE&);
  stItem<LTYPE> *InsertItem2(const int, const LTYPE&);

public:
  tpList(void);
  tpList(const tpList<LTYPE>&);
  virtual ~tpList(void);

  int    GetNumberItems(void) const;
  LTYPE* GetItemPointer(const int) const;
  LTYPE  GetItem(const int) const;
  int    GetIndex(const LTYPE) const;

  virtual int AddItem(const LTYPE&);
  virtual int InsertItem(const int, const LTYPE&);
  virtual int DeleteItem(const int);

  void   Assign(const tpList<LTYPE>&);

  LTYPE* operator[](const int) const;
  LTYPE  operator()(const int) const;
  void   operator=(const tpList<LTYPE>&);
};

/**********************************************************
*** Body file for declaration of template class tpList
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <iostream>
#include <stdio.h>

#include "exception.h"
//#include "list.h"

/*
** ---------------
** Private Methods
** ---------------
*/

template <class LTYPE>
stItem<LTYPE>* tpList<LTYPE>::GetPointer(const int position) const
{
  stItem<LTYPE> *current;// = head;
  int            pos;//     = 0;

  // Check for valid position
  if(position<0 || position>numItems-1)
  {
    char message[MESLEN];
    sprintf(message, "Invalid position.\n  POS = %d\n  MAX = %d", position, numItems-1);
    throw clException("tpList", "GetPointer", message);
  }
  else
  {
    if(position<(numItems-position))
    {
      // Search from beginning
      current = head;
      pos     = 0;

      while(pos++ < position) current = current->next;
    }
    else
    {
      // Search from end
      current = tail;
      pos     = numItems-1;

      while(pos-- > position) current = current->prev;
    }
  }

  return(current);
}

/*
** --------------
** Protected Methods
** --------------
*/

template <class LTYPE>
inline
stItem<LTYPE> *tpList<LTYPE>::GetHead(void) const
{
  return(head);
}

template <class LTYPE>
inline
stItem<LTYPE> *tpList<LTYPE>::GetTail(void) const
{
  return(tail);
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
tpList<LTYPE>::tpList(void)
{
  numItems = 0;
  head     = NULL;
  tail     = NULL;
}

template <class LTYPE>
tpList<LTYPE>::tpList(const tpList<LTYPE> &source)
{
  numItems = 0;
  head     = NULL;
  tail     = NULL;

  Assign(source);
}

template <class LTYPE>
tpList<LTYPE>::~tpList(void)
{
  while(numItems>0) DeleteItem(numItems-1);

  // cerr << "DESTROYING LIST" << endl;
}

/*
** Query Methods
*/
template <class LTYPE>
inline
int tpList<LTYPE>::GetNumberItems(void) const
{
  return(numItems);
}

template <class LTYPE>
LTYPE* tpList<LTYPE>::GetItemPointer(const int position) const
{
  stItem<LTYPE>* returnValue = GetPointer(position);
  return(&(returnValue->object));
}

template <class LTYPE>
LTYPE tpList<LTYPE>::GetItem(const int position) const
{
  return(GetPointer(position)->object);
}

template <class LTYPE>
int tpList<LTYPE>::GetIndex(const LTYPE item) const
{
  int  index = 0;
  bool found = false;

  while(index<numItems && !found)
  {
    found = (GetItem(index)==item);
    index++;
  }

  if(!found)
  {
    index=-1;
  }

  return(--index);
}

/*
** Methods for item manipulation
*/
template <class LTYPE>
int tpList<LTYPE>::AddItem(const LTYPE &object)
{
  stItem<LTYPE> *item = new stItem<LTYPE>;

  // Check memory
  if(!item)
  {
    throw clException("tpList", "AddItem", "Could not allocate memory.");
  }

  // Update head
  if (numItems==0)
  {
    head = item;
  }

  // Update item fields
  item->object = object;
  item->prev   = tail;
  item->next   = NULL;

  // Update tail
  tail = item;

  // Update previous item
  if (item->prev) item->prev->next = item;

  // Update number of items
  numItems++;

  return(numItems-1);
}

template <class LTYPE>
stItem<LTYPE>* tpList<LTYPE>::AddItem2(const LTYPE &object)
{
  stItem<LTYPE> *item = new stItem<LTYPE>;

  // Check memory
  if(!item)
  {
    throw clException("tpList", "AddItem", "Could not allocate memory.");
  }

  // Update head
  if (numItems==0)
  {
    head = item;
  }

  // Update item fields
  item->object = object;
  item->prev   = tail;
  item->next   = NULL;

  // Update tail
  tail = item;

  // Update previous item
  if (item->prev) item->prev->next = item;

  // Update number of items
  numItems++;

  return(item);
}

template <class LTYPE>
int tpList<LTYPE>::InsertItem(const int position, const LTYPE &object)
{
  // Search for correct item
  stItem<LTYPE> *current = GetPointer(position);

  // Create a new item
  stItem<LTYPE> *item = new stItem<LTYPE>;

  // Check memory
  if(!item)
  {
    throw clException("tpList", "InsertItem", "Could not allocate memory.");
  }

  // Update item fields
  item->object = object;
  item->prev   = current->prev;
  item->next   = current;

  // Update previous and next items
  if (item->prev) item->prev->next = item;
  if (item->next) item->next->prev = item;

  // Update head
  if (position==0)            head = item;

  // Update number of items
  numItems++;

  return(position);
}

template <class LTYPE>
stItem<LTYPE>* tpList<LTYPE>::InsertItem2(const int position, const LTYPE &object)
{
  // Search for correct item
  stItem<LTYPE> *current = GetPointer(position);

  // Create a new item
  stItem<LTYPE> *item = new stItem<LTYPE>;

  // Check memory
  if(!item)
  {
    throw clException("tpList", "InsertItem", "Could not allocate memory.");
  }

  // Update item fields
  item->object = object;
  item->prev   = current->prev;
  item->next   = current;

  // Update previous and next items
  if (item->prev) item->prev->next = item;
  if (item->next) item->next->prev = item;

  // Update head
  if (position==0)            head = item;

  // Update number of items
  numItems++;

  return(item);
}

template <class LTYPE>
int tpList<LTYPE>::DeleteItem(const int position)
{
  // Search for correct item
  stItem<LTYPE> *current = GetPointer(position);

  // Update the previous and next items
  if (current->prev) current->prev->next = current->next;
  if (current->next) current->next->prev = current->prev;

  // Update head and tail
  if (position==0)          head = current->next;
  if (position==numItems-1) tail = current->prev;

  // Delete current item
  delete current;

  // Update the number of items
  numItems--;

  return(numItems);
}

/*
** Assign method
*/
template <class LTYPE>
void tpList<LTYPE>::Assign(const tpList<LTYPE> &source)
{
  // Delete all items if any
  for (int i=0; i<numItems; i++)
  {
    DeleteItem(i);
  }

  // Copy all items from source
  for (int j=0; j<source.GetNumberItems(); j++)
  {
    AddItem(source.GetItem(j));
  }
}

/*
** Operator methods
*/
template <class LTYPE>
LTYPE* tpList<LTYPE>::operator[](const int position) const
{
  return(GetItemPointer(position));
}

template <class LTYPE>
LTYPE tpList<LTYPE>::operator()(const int position) const
{
  return(GetItem(position));
}

template <class LTYPE>
void tpList<LTYPE>::operator=(const tpList<LTYPE>& source)
{
  Assign(source);
}

#endif
