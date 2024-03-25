#pragma once

#include <cstdarg>
#include <GLUT/GLUT.h>
#include <iostream>
#include <Eigen/Dense>
#include "utils_2019_10738.h"
#include "winged_edge_2019_10738.h"
#include <vector>

using namespace Eigen;

extern vector<vector<Vector3d>> mesh; // n x [3 x Vertex], ���������� ����ȭ�� n���� �ﰢ�� ����

void Visualization(int argc, char* argv[]);
