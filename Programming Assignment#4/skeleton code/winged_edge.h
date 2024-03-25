/*
Copyright (c) 2019, Seung Won Choi

modified by Wonseok Kang
*/

#pragma once
#include <vector>
#include <memory>

using namespace std;

class Vertex
{
public:
	Vertex() {}
	Vertex(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	~Vertex() {};

	bool valid = false;

	void setValid(bool valid) {
		this->valid = valid;
	}
	void setCoord(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int findConnection(shared_ptr<Vertex> ptr_target) {
		int nb_connection = ptr_next.size();
		int idx = -1;
		for (int i = 0; i < nb_connection; i++) {
			if (ptr_next[i] == ptr_target)
				idx = i;
		}

		if (idx >= 0)
			return idx;
		else
			return -1;
	}


	double x = 0;
	double y = 0;
	double z = 0;

	vector<shared_ptr<Vertex>> ptr_next;
};

class Edge
{
public:
	Edge() {}
	Edge(shared_ptr<Vertex> ptr_p1, shared_ptr<Vertex> ptr_p2) {
		this->ptr_p1 = ptr_p1;
		this->ptr_p2 = ptr_p2;
	}
	~Edge() {};
	shared_ptr<Vertex> ptr_p1, ptr_p2;
};

class Face
{
public:
	Face() {};
	Face(vector<shared_ptr<Vertex>> vertices)
	{
		this->vertices = vertices;
	}
	Face(vector<shared_ptr<Vertex>> vertices, vector<shared_ptr<Edge>> edges) {
		this->vertices = vertices;
		this->edges = edges;
	}
	~Face() {};

	bool rendered = false;
	vector<shared_ptr<Vertex>> vertices;
	vector<shared_ptr<Edge>> edges;

	void addVertex(shared_ptr<Vertex> vertex) {
		this->vertices.push_back(vertex);
	}

	void addEdge(shared_ptr<Edge> edge) {
		this->edges.push_back(edge);
	}
};

class W_Edge
{
public:
	W_Edge() {};
	~W_Edge() {};

	shared_ptr<Vertex> ptr_start, ptr_end;

	//Face face_left, face_right;
	shared_ptr<Vertex> ptr_lp_start, ptr_lp_end, ptr_ln_start, ptr_ln_end;
	shared_ptr<Vertex> ptr_rp_start, ptr_rp_end, ptr_rn_start, ptr_rn_end;


	void setEdge(shared_ptr<Vertex> ptr_start, shared_ptr<Vertex> ptr_end) {
		this->ptr_start = ptr_start;
		this->ptr_end = ptr_end;
	}

	void setLeftWing(
		shared_ptr<Vertex> ptr_lp_start,
		shared_ptr<Vertex> ptr_lp_end,
		shared_ptr<Vertex> ptr_ln_start,
		shared_ptr<Vertex> ptr_ln_end) {

		this->ptr_lp_start = ptr_lp_start;
		this->ptr_lp_end = ptr_lp_end;
		this->ptr_ln_start = ptr_ln_start;
		this->ptr_ln_end = ptr_ln_end;
	}

	void setRightWing(
		shared_ptr<Vertex> ptr_rp_start,
		shared_ptr<Vertex> ptr_rp_end,
		shared_ptr<Vertex> ptr_rn_start,
		shared_ptr<Vertex> ptr_rn_end) {

		this->ptr_rp_start = ptr_rp_start;
		this->ptr_rp_end = ptr_rp_end;
		this->ptr_rn_start = ptr_rn_start;
		this->ptr_rn_end = ptr_rn_end;
	}
};
