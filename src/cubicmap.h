#include "raylib.h"
#include <stdlib.h>           // Required for: free()

#ifndef CUBICMAP_H
#define CUBICMAP_H

Model LoadModelFromMesh2(Mesh mesh);

Mesh GenMeshCubicmap2(Image cubicmap, Vector3 cubeSize);

#endif
