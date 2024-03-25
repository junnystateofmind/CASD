#pragma once

#include <cstdarg>
#include <GLUT/GLUT.h>
#include <iostream>
#include <Eigen/Dense>
#include "utils_2019_10738.h"
#include "winged_edge_2019_10738.h"
#include <vector>

using namespace Eigen;

extern vector<vector<Vector3d>> mesh; // n x [3 x Vertex], 최종적으로 가시화할 n개의 삼각형 저장

void Visualization(int argc, char* argv[]);
