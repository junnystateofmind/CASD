/*
Copyright (c) 2019, Seung Won Choi

modified by Wonseok Kang
*/

#pragma once
#include <vector>
#include <memory>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

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
    
    vector<Vector3d> setting_point_normal;
    Vector3d point_normal = Vector3d(0.0, 0.0, 0.0);
    
    void calculate_ponit_normal(){
        for (Vector3d normal : this->setting_point_normal){
            this->point_normal + normal;
        }
        this->point_normal = this->point_normal/this->setting_point_normal.size();
    }
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
    vector<Vector3d> controlpoints; // Coons patch 과정을 거치고 난 후의 controlpoints
    Vector3d face_normal = Vector3d(0.0, 0.0, 0.0);

    void addVertex(shared_ptr<Vertex> vertex) {
        this->vertices.push_back(vertex);
    }

    void addEdge(shared_ptr<Edge> edge) {
        this->edges.push_back(edge);
    }
    
    void allocate_normal_to_vertex(){
        if(this->vertices.size() == 3){ // 삼각형의 경우
            Vector3d counterclock_0_point = Vector3d(vertices[1]->x - vertices[0]->x, vertices[1]->y - vertices[0]->y, vertices[1]->z - vertices[0]->z);
            Vector3d clock_0_point = Vector3d(vertices[2]->x - vertices[0]->x, vertices[2]->y - vertices[0]->y, vertices[2]->z - vertices[0]->z);
            this->vertices[0]->setting_point_normal.push_back(counterclock_0_point.cross(clock_0_point));
            
            Vector3d counterclock_1_point = Vector3d(vertices[2]->x - vertices[1]->x, vertices[2]->y - vertices[1]->y, vertices[2]->z - vertices[1]->z);
            Vector3d clock_1_point = Vector3d(vertices[0]->x - vertices[1]->x, vertices[0]->y - vertices[1]->y, vertices[0]->z - vertices[1]->z);
            this->vertices[1]->setting_point_normal.push_back(counterclock_1_point.cross(clock_1_point));
            
            Vector3d counterclock_2_point = Vector3d(vertices[0]->x - vertices[2]->x, vertices[0]->y - vertices[2]->y, vertices[0]->z - vertices[2]->z);
            Vector3d clock_2_point = Vector3d(vertices[1]->x - vertices[2]->x, vertices[1]->y - vertices[2]->y, vertices[1]->z - vertices[2]->z);
            this->vertices[2]->setting_point_normal.push_back(counterclock_2_point.cross(clock_2_point));
        }
        else{ // 사각형 혹은 그 이상의 경우
            for(int i = 0; i < this->vertices.size(); i++){
                if(i == 0){ // 0번 점 혹은 종점
                    Vector3d counterclock_point = Vector3d(vertices[1]->x - vertices[0]->x, vertices[1]->y - vertices[0]->y, vertices[1]->z - vertices[0]->z);
                    Vector3d clock_point = Vector3d(vertices[this->vertices.size()-1]->x - vertices[0]->x, vertices[this->vertices.size()-1]->y - vertices[0]->y, vertices[this->vertices.size()-1]->z - vertices[0]->z);
                    this->vertices[i]->setting_point_normal.push_back(counterclock_point.cross(clock_point));
                }
                else if (i == this->vertices.size()-1){
                    Vector3d counterclock_point = Vector3d(vertices[0]->x - vertices[this->vertices.size()-1]->x, vertices[0]->y - vertices[this->vertices.size()-1]->y, vertices[0]->z - vertices[this->vertices.size()-1]->z);
                    Vector3d clock_point = Vector3d(vertices[this->vertices.size()-2]->x - vertices[this->vertices.size()-1]->x, vertices[this->vertices.size()-2]->y - vertices[this->vertices.size()-1]->y, vertices[this->vertices.size()-2]->z - vertices[this->vertices.size()-1]->z);
                    this->vertices[i]->setting_point_normal.push_back(counterclock_point.cross(clock_point));
                }
                else{
                    Vector3d counterclock_point = Vector3d(vertices[i+1]->x - vertices[i]->x, vertices[i+1]->y - vertices[i]->y, vertices[i+1]->z - vertices[i]->z);
                    Vector3d clock_point = Vector3d(vertices[i-1]->x - vertices[i]->x, vertices[i-1]->y - vertices[i]->y, vertices[i-1]->z - vertices[i]->z);
                    this->vertices[i]->setting_point_normal.push_back(counterclock_point.cross(clock_point));
                }
            }
        }
    }
    
    
    
//    void calculate_face_normal(){
//        Vector3d zero_counter = Vector3d(this->vertices[1]->x - this->vertices[0]->x, this->vertices[1]->y-this->vertices[0]->y, this->vertices[1]->z-this->vertices[0]->z);
//        Vector3d zero_clock = Vector3d(this->vertices[this->vertices.size()-1]->x - this->vertices[0]->x, this->vertices[this->vertices.size()-1]->y-this->vertices[0]->y, this->vertices[this->vertices.size()-1]->z-this->vertices[0]->z);
//        this->face_normal += zero_counter.cross(zero_clock);
//        for(int i = 1; i < this->vertices.size()-1; i++){
//            Vector3d temp1 = Vector3d(this->vertices[i+1]->x - this->vertices[i]->x, this->vertices[i+1]->y-this->vertices[i]->y, this->vertices[i+1]->z-this->vertices[i]->z);
//            Vector3d temp2 = Vector3d(this->vertices[i-1]->x - this->vertices[i]->x, this->vertices[i-1]->y-this->vertices[i]->y, this->vertices[i-1]->z-this->vertices[i]->z);
//            this->face_normal += temp1.cross(temp2);
//        }
//        Vector3d final_counter = Vector3d(this->vertices[0]->x - this->vertices[this->vertices.size()-1]->x, this->vertices[0]->y-this->vertices[this->vertices.size()-1]->y, this->vertices[0]->z-this->vertices[this->vertices.size()-1]->z);
//        Vector3d final_clock =Vector3d(this->vertices[this->vertices.size()-2]->x - this->vertices[this->vertices.size()-1]->x, this->vertices[this->vertices.size()-2]->y-this->vertices[this->vertices.size()-1]->y, this->vertices[this->vertices.size()-2]->z-this->vertices[this->vertices.size()-1]->z);
//        this->face_normal += final_counter.cross(final_clock);
//
//        this->face_normal = this->face_normal/this->vertices.size();
//    }
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
