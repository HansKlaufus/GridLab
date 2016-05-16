/**********************************************************
*** Header file for declaration of template class tpStack
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef STACK_H
#define STACK_H

template <class STYPE>
class tpStack
{
private:
  int    stackSize;
  int    top;
  STYPE  *items;

  void   SetTop(int);

  int    PushStatic(STYPE);
  STYPE  PopStatic(void);

  int    PushDynamic(STYPE);
  STYPE  PopDynamic(void);

public:
  tpStack(void);
  tpStack(int);
  ~tpStack(void);

  int   IsEmpty(void) const;
  int   GetTop(void) const;

  int   Push(STYPE);
  STYPE Pop(void);
};

/**********************************************************
*** Body file for declaration of template class tpStack
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <iostream>

#include "exception.h"

/*
** ---------------
** Private Methods
** ---------------
*/

template <class STYPE>
void tpStack<STYPE>::SetTop(int tempTop)
{
  top = tempTop;
}

template <class STYPE>
int tpStack<STYPE>::PushStatic(STYPE item)
{
  if(top == stackSize-1)
  {
    throw clExceptionTree("tpStack", "PushStatic", "Stack overflow.");
  }

  top++;
  items[top] = item;

  return(top);
}

template <class STYPE>
STYPE tpStack<STYPE>::PopStatic(void)
{
  if(top == -1)
  {
    throw clExceptionTree("tpStack", "PopStatic", "Stack underflow.");
  }

  STYPE ret = items[top];
  top--;

  return(ret);
}


template <class STYPE>
int tpStack<STYPE>::PushDynamic(STYPE item)
{
  // Create a new stack
  int   currentSize=top+1;
  STYPE *newStack = new STYPE[currentSize+1];
  if(!newStack)
  {
    throw clExceptionTree("tpStack", "PushDynamic", "Could not allocate memory.");
  }

  // Copy existing stack to new stack
  for(int i=0; i<=top; i++)
  {
    newStack[i] = items[i];
  }

  // Add the new item
  top++;
  newStack[top] = item;

  // Delete the old stack
  if(items)
  {
    delete [] items;
  }

  // Redirect the pointer
  items = newStack;

  return(top);
}

template <class STYPE>
STYPE tpStack<STYPE>::PopDynamic(void)
{
  if(top == -1)
  {
    throw clExceptionTree("tpStack", "PopDynamic", "Stack underflow.");
  }

  // Create a new (smaller) stack
  int   currentSize=top+1;
  STYPE *newStack = new STYPE[currentSize-1];
  if(!newStack)
  {
    throw clExceptionTree("tpStack", "PopDynamic", "Could not allocate memory.");
  }

  // Save the top item and decrease top pointer
  STYPE ret = items[top];
  top--;

  // Copy the existing stack to the new stack
  for(int i=0; i<=top; i++)
  {
    newStack[i] = items[i];
  }

  // Delete the old stack
  if(items)
  {
    delete [] items;
  }

  // redirect the pointer
  items = newStack;

  return(ret);
}

/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructor and destructor
*/
template <class STYPE>
tpStack<STYPE>::tpStack(void)
{
  // Creates a DYNAMIC stack
  stackSize = 0;
  top       = -1;
  items     = NULL;
}

template <class STYPE>
tpStack<STYPE>::tpStack(int size)
{
  // Creates a STATIC stack
  stackSize = size;
  top       = -1;
  items     = new STYPE[stackSize];
}

template <class STYPE>
tpStack<STYPE>::~tpStack(void)
{
  if(items)
  {
    delete [] items;
  }

  // cerr << "DESTROYING STACK" << endl;
}

/*
** Query Methods
*/
template <class STYPE>
int tpStack<STYPE>::IsEmpty(void) const
{
  return(top==-1);
}

template <class STYPE>
int tpStack<STYPE>::GetTop(void) const
{
  // Correction for array starting at zero
  return(top);
}

/*
** Push and Pop Methods
*/
template <class STYPE>
int tpStack<STYPE>::Push(STYPE item)
{
  int ret;

  if(stackSize>0)
  {
    ret = PushStatic(item);
  }
  else
  {
    ret = PushDynamic(item);
  }

  return(ret);
}

template <class STYPE>
STYPE tpStack<STYPE>::Pop(void)
{
  STYPE ret;

  if(stackSize>0)
  {
    ret = PopStatic();
  }
  else
  {
    ret = PopDynamic();
  }

  return(ret);
}

#endif
