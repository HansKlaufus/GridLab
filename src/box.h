/*****************************************************
*** Header file for declaration of class clBoxCreator
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef BOX_H
#define BOX_H

#define ENLARGE_FACTOR 5

#include "geometry.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

enum enBoxType      {btBox, btCType, btOType};
enum enLimitingAxis {laNone, laAxisX, laAxisY, laAxisZ};
enum enHeading      {hdNone, hdPositiveX, hdPositiveY, hdPositiveZ, hdNegativeX, hdNegativeY, hdNegativeZ};

class clBoxCreator
{
private:
  enBoxType      boxType;
  enLimitingAxis limitingAxis;
  enHeading      heading;

  void CreateBox(clGeometry&, clGeometry&);
  void CreateCType(const clGeometry&, clGeometry&);
  void CreateOType(const clGeometry&, clGeometry&);

public:
  clBoxCreator(clGeometry&, clGeometry&, const enBoxType, const enLimitingAxis, const enHeading);
  ~clBoxCreator(void);
};

#endif
