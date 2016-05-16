/****************************************
*** Declaration of class clCanvas
***
*** Header File
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef TREEITEMDATA_H
#define TREEITEMDATA_H

#include <wx/treectrl.h>

enum enItemType {itDummy, itRoot, itGeometry, itSurface, itPatch, itPoint, itSurfaceGrid, itElement2D, itFace2D, itNode2D, itCircumCentre2D, itCentroid2D, itVolumeGrid, itElement3D, itFace3D, itNode3D, itCircumCentre3D, itCentroid3D, itNeighbour, itID, itValue};

class clTreeItemData: public wxTreeItemData
{
private:
  enItemType itemType;
  int        itemID;
  int        itemIndex;

public:
  clTreeItemData(const int, const enItemType);
  clTreeItemData(const int, const int, const enItemType);
  ~clTreeItemData(void);

  void       SetItemType(const enItemType);
  enItemType GetItemType(void) const;
  void       SetItemID(const int);
  int        GetItemID(void) const;
  void       SetItemIndex(const int);
  int        GetItemIndex(void) const;
};

#endif

