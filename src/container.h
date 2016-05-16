/*************************************************************
*** Header file for declaration of template class tpContainer
***
*** J.L. Klaufus - Haarlem 2000
**/

#ifndef CONTAINER_H
#define CONTAINER_H

template <class CTYPE>
class tpContainer
{
private:
  CTYPE              object;
  int                numBranches;
  tpContainer<CTYPE> *branches[];

public:
  tpContainer(void);
  tpContainer(const tpContainer<CTYPE>&);
  ~tpContainer(void);

  void                SetObject(CTYPE);
  void                SetObject(CTYPE&);
  CTYPE               GetObject(void) const;
  CTYPE*              GetObjectPointer(void) const;

  int                 GetNumberBranches(void) const;
  int                 AddBranch(const tpContainer<CTYPE>*);
  int                 InsertBranch(const tpContainer<CTYPE>*, const int);
  int                 DeleteBranch(const int);
  tpContainer<CTYPE>* GetBranch(const int) const;

  void                Assign(const tpContainer<CTYPE>&);

  tpContainer<CTYPE>* operator()(const int) const;
  void                operator=(const tpContainer<CTYPE>&);
};

/************************************************************
*** Body file for declaration of template class tpContainer
***
*** J.L. Klaufus - Haarlem 2000
**/

#include <iostream>
#include <stdio.h>

#include "container.h"
#include "exception.h"

/*
** ---------------
** Private Methods
** ---------------
*/


/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructor and destructor
*/
template <class CTYPE>
tpContainer<CTYPE>::tpContainer(void)
{
  numBranches = 0;
  **branches = NULL;
}

template <class CTYPE>
tpContainer<CTYPE>::tpContainer(const tpContainer<CTYPE> &source)
{
  numBranches = 0;
  **branches = NULL;

  Assign(source);
}

template <class CTYPE>
tpContainer<CTYPE>::~tpContainer(void)
{
  if(branches)
  {
    delete [] branches;
  }

  // cerr << "DESTROYING LIST" << endl;
}

/*
** Object Methods
*/
template <class CTYPE>
void tpContainer<CTYPE>::SetObject(CTYPE source)
{
  object = source;
}

template <class CTYPE>
void tpContainer<CTYPE>::SetObject(CTYPE &source)
{
  object = source;
}

template <class CTYPE>
CTYPE tpContainer<CTYPE>::GetObject(void) const
{
  return(object);
}

template <class CTYPE>
CTYPE* tpContainer<CTYPE>::GetObjectPointer(void) const
{
  return(&object);
}

/*
** Branch Methods
*/
template <class CTYPE>
int tpContainer<CTYPE>::GetNumberBranches(void) const
{
  return(numBranches);
}

template <class CTYPE>
int tpContainer<CTYPE>::AddBranch(const tpContainer<CTYPE> *theBranch)
{
  int num_branches=InsertBranch(&(*theBranch), numBranches);

  return(numBranches);
}

template <class CTYPE>
int tpContainer<CTYPE>::InsertBranch(const tpContainer<CTYPE> *theBranch, const int entry)
{
  if(entry<0 || entry=numBranches)
  {
    char message[MESLEN];
    sprintf(message, "Invalid entry specified.\n    ENTRY = %d", entry);
    throw clException("tpContainer", "InsertBranch", message);
  }

  // Alocate memory for new branch array
  tpContainer<CTYPE> *newBranches = new tpContainer<CTYPE>[numBranches+1];

  // Copy old branch array into new array
  for(int i=0; i<numBranches+1; i++)
  {
    if(i==entry)
    {
      i++;
    }
    else
    {
      newBranches[i] = branches[i];
    }
  }

  // Add new branch
  newBranches[entry] = theBranch;

  // Delete old branch array
  delete [] branches;

  // Redirect branches
  branches = newBranches;

  // Update number of branches
  numBranches++;

  return(numBranches);
}

template <class CTYPE>
int tpContainer<CTYPE>::DeleteBranch(const int entry)
{
  if(entry<0 || entry>=numBranches)
  {
    char message[MESLEN];
    sprintf(message, "Invalid entry specified.\n    ENTRY = %d", entry);
    throw clException("tpContainer", "DeleteBranch", message);
  }

  // Alocate memory for new branch array
  tpContainer<CTYPE> *newBranches = new tpContainer<CTYPE>[numBranches-1];

  // Copy old branch array into new array
  int j=0;
  for(int i=0; i<numBranches; i++)
  {
    if(i!=entry)
    {
      newBranches[j] = branches[i];
      j++;
    }
  }

  // Delete old branch array
  delete [] branches;

  // Redirect branches
  branches = newBranches;

  // Update number of branches
  numBranches--;

  return(numBranches);
}

template <class CTYPE>
tpContainer<CTYPE>* tpContainer<CTYPE>::GetBranch(const int entry) const
{
  if(entry<0 || entry>=numBranches)
  {
    char message[MESLEN];
    sprintf(message, "Invalid entry specified.\n    ENTRY = %d", entry);
    throw clException("tpContainer", "GetBranch", message);
  }

  return(branches[entry]);
}

/*
** Assign method
*/
template <class CTYPE>
void tpContainer<CTYPE>::Assign(const tpContainer<CTYPE> &source)
{
  // Delete all branches if any
  for (int i=0; i<numBranches; i++)
  {
    DeleteBranch(i);
  }

  // Copy all branches from source
  for (int j=0; j<source.GetNumberBranches(); j++)
  {
    AddBranch(source.GetItem(j));
  }

  // Copy the actual object
  SetObject(source.Getobject());
}

/*
** Operator methods
*/
template <class CTYPE>
tpContainer<CTYPE>* tpContainer<CTYPE>::operator()(const int position) const
{
  return(GetBranch(position));
}

template <class CTYPE>
void tpContainer<CTYPE>::operator=(const tpContainer<CTYPE>& source)
{
  Assign(source);
}

#endif

