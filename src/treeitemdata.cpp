/******************************************
*** Definition of class clCanvas
***
*** Body File
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <wx/wx.h>

//#include "mywx.h"
#include "exceptiontree.h"
#include "treeitemdata.h"

/*
** Constructor and Destructor
*/
clTreeItemData::clTreeItemData(int theItemID, enItemType theItemType)
{
  itemID   = theItemID;
  itemType = theItemType;
}

clTreeItemData::clTreeItemData(int theItemID, int theItemIndex, enItemType theItemType)
{
  itemID   = theItemID;
  itemIndex= theItemIndex;
  itemType = theItemType;
}

clTreeItemData::~clTreeItemData(void)
{
}

/*
** Set and Get
*/
void clTreeItemData::SetItemType(const enItemType theItemType)
{
  itemType = theItemType;
}

enItemType clTreeItemData::GetItemType(void) const
{
  return(itemType);
}

void clTreeItemData::SetItemID(const int theItemID)
{
  itemID = theItemID;
}

int clTreeItemData::GetItemID(void) const
{
  return(itemID);
}

void clTreeItemData::SetItemIndex(const int theItemIndex)
{
  itemIndex = theItemIndex;
}

int clTreeItemData::GetItemIndex(void) const
{
  return(itemIndex);
}

