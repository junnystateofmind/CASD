#pragma once

#include <cstdarg>
#include <GL/glut.h>
#include <iostream>
#include <Eigen/Dense>
#include "utils.h"
#include "winged_edge.h"

extern vector<vector<Vector3d>> mesh; // n x [3 x Vertex], ���������� ����ȭ�� n���� �ﰢ�� ����

void Visualization(int argc, char* argv[]);