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
#include "winged_edge.h"
#include <memory>
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


double calAngle(
	double x1,
	double z1,
	double x2,
	double z2);