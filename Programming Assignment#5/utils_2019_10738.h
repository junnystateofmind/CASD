/*
Copyright (c) 2019, Seung Won Choi

modified by Wonseok Kang
*/


#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "winged_edge_2019_10738.h"
#include <memory>
#include <cmath>
#include <Eigen/Dense>

using namespace std;


//Function reading a line from text and storing it in a vector.
vector<double> readOneLine(
	string line);

//Function reading data from an offset table and storing it in a matrix.
void readOffsetTable(
	string file_name,
	vector<vector<shared_ptr<Vertex>>>& offset_table);

//Function checking whether a vertex is valid or not.
bool isVertexValid(
	shared_ptr<Vertex> ptr_vertex);

//Function building edges in local area.
void buildLocalEdge(
	shared_ptr<Vertex> ptr_target,
	shared_ptr<Vertex> ptr_adj_1,
	shared_ptr<Vertex> ptr_adj_2,
	shared_ptr<Vertex> ptr_corner,
	vector<shared_ptr<Edge>>& ptrs_edge);

//Function building edges in whole area.
void buildAllEdges(
	vector<vector<shared_ptr<Vertex>>>& offset_table,
	vector<shared_ptr<Edge>>& ptrs_edge);
//vector<Edge>& edges);

//Function adding connection between two vertices.
void addConnection(
	shared_ptr<Vertex>& ptr_p1,
	shared_ptr<Vertex>& ptr_p2);

//Function building winged edge classes.
void buildWEdge(
	shared_ptr<Edge> ptr_edge,
	vector<shared_ptr<W_Edge>>& ptrs_wedge);

//Function checking if two vertices are same or not.
bool isSameVertex(
	shared_ptr<Vertex> ptr_vertex_1,
	shared_ptr<Vertex> ptr_vertex_2);

//Function checing if two vertices having connection to each other.
bool isConnected(
	shared_ptr<Vertex> ptr_vertex_1,
	shared_ptr<Vertex> ptr_vertex_2);

//Function checing if two vertices having another connection to each other.
vector<vector<int>> isConnectedPathThrough(
	shared_ptr<Vertex> ptr_vertex_1,
	shared_ptr<Vertex> ptr_vertex_2);

Eigen::Vector3d centroid_triangle(shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c);

Eigen::Vector3d centroid_rectangle(shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c, shared_ptr<Vertex> d);

Eigen::Vector3d vertex_to_vec3(shared_ptr<Vertex> vertex);

//W_Edge를 통해 Face class를 build하는 메서드
void buildAllFace(vector<shared_ptr<W_Edge>>& ptr_wedges,vector<shared_ptr<Face>>& ptr_faces);

void buildCoonFaces(vector<shared_ptr<Face>>& ptr_faces);

void triangle_coons_patch(shared_ptr<Face>& ptr_face);

void rectangular_coons_patch(shared_ptr<Face>& ptr_face);

Vector3d calculate_near_controlpoint(shared_ptr<Vertex> near, shared_ptr<Vertex> far);

Vector3d deCasteljau_triangle(shared_ptr<Face> face, double u, double v);

Vector3d deCasteljau_for_triangle(Vector3d cp0, Vector3d cp1, Vector3d cp2, double u, double v);

Vector3d deCasteljau_rectangle(shared_ptr<Face> face, double u, double v);

Vector3d deCasteljau(Vector3d cp0, Vector3d cp1, Vector3d cp2, Vector3d cp3, double t);

double calAngle(
	double x1,
	double z1,
	double x2,
	double z2);
