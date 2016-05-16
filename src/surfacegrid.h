/******************************************************************
*** Header file for declaration of class clSurfaceGrid
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef SURFACEGRID_H
#define SURFACEGRID_H

#include <string>

#include "globals.h"
#include "grid.h"
#include "matrix.h"
#include "node3d.h"
#include "surface.h"
#include "triangle.h"
#include "unstructured2d.h"

enum enGridType         {gtStructured, gtUnstructured, gtUserDefined};
enum enDistributionType {dtUniform, dtNonUniform, dtUserDefined};

class clSurfaceGrid : public tpGrid<clNode3D, clTriangle>
{
private:
  void               CreateGrid(void);
//  void               CreateStructured(const dMatrix&, const dMatrix&, clSurface&);
  void               InsertGrid(clSurface*, clUnstructured2D*);
  void               ParseGTSGrid(const char*);
  void               ParseOBJGrid(const char*);

public:
  clSurfaceGrid(void);
  clSurfaceGrid(const int, string = "noName");
  clSurfaceGrid(const clSurfaceGrid&);
  ~clSurfaceGrid(void);

  clSurfaceGrid&     Assign(const clSurfaceGrid&);
  clSurfaceGrid&     operator=(const clSurfaceGrid&);

  double             GetMinX(void) const;
  double             GetMaxX(void) const;
  double             GetMinY(void) const;
  double             GetMaxY(void) const;
  double             GetMinZ(void) const;
  double             GetMaxZ(void) const;

  void               CreateUnstructured(clSurface*, const int, const int, const enDistributionType);
  void               CreateUnstructured(clGeometry*, const int, const int, const enDistributionType);

  void               Distribute(const enDistributionType, const int, const int, dMatrix&, dMatrix&) const;
  void               MergeGrid(clSurface*, clUnstructured2D*);

  void               DumpGrid(void);
  void               DumpGrid(const std::string);
  void               DumpGrid(const char*);

  void               WriteGnuplotGrid(void);
  void               WriteGnuplotGrid(const std::string);
  void               WriteGnuplotGrid(const char*);

  void               WriteLegacyVTKGrid(void);
  void               WriteLegacyVTKGrid(const std::string);
  void               WriteLegacyVTKGrid(const char*);

  void               ReadGTSGrid(const std::string);
  void               ReadGTSGrid(const char*);

  void               ReadOBJGrid(const std::string);
  void               ReadOBJGrid(const char*);
};

#endif
