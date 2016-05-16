/*********************************************************
*** Header file for declaration of class clGeometryWriter
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef GEOMETRY_WRITER_H
#define GEOMETRY_WRITER_H

#include "geometry.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

enum enFormat {foGnu, foTec};

class clGeometryWriter
{
private:
  char  *fileName;
  int   fileNameLength;

  void  WriteTec(const clGeometry&);
  void  WriteGnu(const clGeometry&);

public:
  clGeometryWriter(const char*, enFormat, clGeometry&);
  //clGeometryWriter(const char*, clBoundaryGrid&);
  //clGeometryWriter(const char*, clUnstructured3D&);
  ~clGeometryWriter(void);
};

#endif

