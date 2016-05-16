/****************************************************
*** Header file for declaration of class clGeometryReader
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef READER_H
#define READER_H

#include <fstream>

#include "geometry.h"
#include "matrix.h"
#include "patch.h"
#include "surface.h"

using namespace std;

#ifdef  LINE_SIZE
#undef  LINE_SIZE
#endif

#define LINE_SIZE 500

#define UNKNOWN_SPEC  -1
#define POINT_SPEC    1
#define PATCH_SPEC    2
#define SURFACE_SPEC  3
#define GEOMETRY_SPEC 4
#define END_SPEC      99

typedef tpMatrix<double> dMatrix;

class clGeometryReader
{
private:
  char  *fileName;
  int    fileNameLength;

  int   GetBlockSpec(const char*) const;

  void  ReadFile(clGeometry&) const;
  void  ReadGeometry(fstream&, clGeometry&) const;
  void  ReadSurface(fstream&, clSurface&) const;
  void  ReadPoints(fstream&, dMatrix&, dMatrix&, dMatrix&) const;
  void  ReadPatch(fstream&, clPatch&) const;

public:
  clGeometryReader(const char*, clGeometry&);
  ~clGeometryReader(void);
};

#endif

