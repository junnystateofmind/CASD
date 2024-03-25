#pragma once

#include <cstdarg>
#include <GL/glut.h>
#include <iostream>
#include <Eigen/Dense>
#include "utils.h"
#include "winged_edge.h"

extern vector<vector<Vector3d>> mesh; // n x [3 x Vertex], 최종적으로 가시화할 n개의 삼각형 저장

void Visualization(int argc, char* argv[]);