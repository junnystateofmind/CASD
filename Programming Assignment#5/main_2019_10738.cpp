//
//  main.cpp
//  Progmramming Assignment #4
//
//  Created by Minjun Jang on 2023/05/22.
//

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "utils_2019_10738.h"
#include "Visualization.h"
#include "winged_edge_2019_10738.h"

using namespace std;
using namespace Eigen;


void faces_to_mesh(vector<shared_ptr<Face>> ptr_faces){
    for(int i = 0; i < ptr_faces.size(); i++){
        if(ptr_faces[i]->vertices.size()==3){ // 삼각형
            mesh.push_back({vertex_to_vec3(ptr_faces[i]->vertices[0]), vertex_to_vec3(ptr_faces[i]->vertices[1]), vertex_to_vec3(ptr_faces[i]->vertices[2])});
        }
        else if(ptr_faces[i]->vertices.size()==4){ // 사각형
            mesh.push_back({vertex_to_vec3(ptr_faces[i]->vertices[0]), vertex_to_vec3(ptr_faces[i]->vertices[1]), vertex_to_vec3(ptr_faces[i]->vertices[2])});
            mesh.push_back({vertex_to_vec3(ptr_faces[i]->vertices[2]), vertex_to_vec3(ptr_faces[i]->vertices[3]), vertex_to_vec3(ptr_faces[i]->vertices[0])});
        }
    }
}

void printTable(vector<vector<shared_ptr<Vertex>>> offset_table){
    for(int i = 0; i < offset_table.size(); i++){
        for(int j = 0; j < offset_table[i].size(); j++){
            if(offset_table[i][j]->valid){
                cout << "Vertex at [" << i << "][" << j << "]: " << offset_table[i][j]->x << ", " << offset_table[i][j]->y << ", " << offset_table[i][j]->z << '\n';
            }
            else{
                cout << "Invalid vertex at [" << i << "][" << j << "]" << '\n';
            }
        }
    }
}
void printWedge(const vector<shared_ptr<W_Edge>> ptr_winged_edges){
//    for(int i = 0; i < ptr_winged_edges.size(); i++){
//        cout << i <<"번째 winged edge :" << "(" <<ptr_winged_edges[i]->ptr_start->x << ", "<< ptr_winged_edges[i]->ptr_start->y << ", "<< ptr_winged_edges[i]->ptr_start->z << ") " << "("<< ptr_winged_edges[i]->ptr_end->x << ", "<< ptr_winged_edges[i]->ptr_end->y << ", "<< ptr_winged_edges[i]->ptr_end->z << ")"<< endl;
//    }
    cout << "Number of Winged Edge: " << ptr_winged_edges.size() << endl;
}
void printFace(const vector<shared_ptr<Face>> ptr_faces){
    cout << "Number of Face: " << ptr_faces.size() << endl;
}
void printMesh(const vector<vector<Vector3d>>& mesh) {
    cout << "Number of mesh: " << mesh.size() << endl;
    for (int i = 0; i < mesh.size(); i++) {
        cout << "mesh " << i << ": ";
        for (const auto& vertex : mesh[i]) {
            cout << "(" << vertex.x() << ", " << vertex.y() << ", " << vertex.z() << ") ";
        }
        cout << endl;
    }
}
void printControlpoints_Face(const vector<shared_ptr<Face>> ptr_faces){
    for(shared_ptr<Face> ptr_face : ptr_faces){
        for(int i = 0; i < ptr_face->controlpoints.size(); i++){
            cout << i+1 << "번째 control point : x = " << ptr_face->controlpoints[i].x() << ", y = " << ptr_face->controlpoints[i].y() << ", z = " << ptr_face->controlpoints[i].z() << endl;
        }
    }
}
void build_triangle_mesh(shared_ptr<Face> ptr_face){
    int step_size = 5;
    double step = 1.0/step_size;
    vector<vector<Vector3d>> triangle_points;
    cout << "이 triangle face의 꼭짓점은 " << endl;
    for(shared_ptr<Vertex> vertex : ptr_face->vertices){
        cout << vertex->x << ", " << vertex->y << ", " << vertex->z << endl;
    }
    for(int i = 0; i <= step_size; i++){
        vector<Vector3d> temp;
        for(int j = 0; j <= i; j++){
            // 이 행에서 u, v의 합은 (1/step_size) * i, triangle points에 삼각형 모양으로 데이터 입력
//                        00
//                    10      11
//                20      21      22
//            30      31      32      33
            temp.push_back(deCasteljau_triangle(ptr_face, step*(i-j), step*j));
            cout << i << ", " << j << " : " ;
            cout << deCasteljau_triangle(ptr_face, step*(i-j), step*j).x() << ", ";
            cout << deCasteljau_triangle(ptr_face, step*(i-j), step*j).y() << ", ";
            cout << deCasteljau_triangle(ptr_face, step*(i-j), step*j).z() << ", " << endl;
        }
        triangle_points.push_back(temp);
    }
    //  n층의 피라미드 형식으로 pos 찾기 완료
    for(int i = 0; i < step_size; i++){
        for(int j = 0; j < triangle_points[i].size(); j++){
            vector<Vector3d> temp;
            temp.push_back(triangle_points[i][j]);
            temp.push_back(triangle_points[i+1][j+1]);
            temp.push_back(triangle_points[i+1][j]);
            mesh.push_back(temp);
            // 정방향 삼각형 mesh 입력, 시계방향
        }
    }
    for(int i = 1; i < step_size; i++){
        for(int j = 0; j < triangle_points[i].size()-1; j++){
            vector<Vector3d> temp;
            temp.push_back(triangle_points[i][j]);
            temp.push_back(triangle_points[i][j+1]);
            temp.push_back(triangle_points[i+1][j+1]);
            mesh.push_back(temp);
            // 역삼각형 mesh 입력, 시계방향
        }
    }
}

void build_rectangle_mesh(shared_ptr<Face> ptr_face){
    int step_size = 5;
    double step = 1.0/step_size;
    vector<vector<Vector3d>> rectangle_points;
    for(int i = 0; i <= step_size; i++){
        vector<Vector3d> temp;
        for(int j = 0; j <= step_size; j++){
            
            // 바둑판 모양
//            00 01 02 03
//            10 11 12 13
//            20 21 22 23
//            30 31 32 33
            temp.push_back(deCasteljau_rectangle(ptr_face, i*step, j*step));
//            cout << i << ", " << j << " : " ;
//            cout << deCasteljau_rectangle(ptr_face, i*step, j*step).x() << ", ";
//            cout << deCasteljau_rectangle(ptr_face, i*step, j*step).y() << ", ";
//            cout << deCasteljau_rectangle(ptr_face, i*step, j*step).z() << ", " << endl;
        }
        rectangle_points.push_back(temp);
    }
//    for(int i = 0; i < rectangle_points.size(); i++){
//        for(int j = 0; j < rectangle_points[i].size(); j++){
//            cout << i << ", " << j << "의 rectangle_points ";
//            cout << rectangle_points[i][j].x() << ", " << rectangle_points[i][j].y() << ", " << rectangle_points[i][j].z() << endl;
//        }
//    }
    
    for(int i = 0; i < step_size; i++){
        for(int j = 0; j < step_size; j++){
//            vector<Vector3d> temp;
//            temp.push_back(rectangle_points[i][j]);
//            temp.push_back(rectangle_points[i][j+1]);
//            temp.push_back(rectangle_points[i+1][j+1]);
//            mesh.push_back(temp);
            mesh.push_back({rectangle_points[i][j], rectangle_points[i][j+1], rectangle_points[i+1][j+1]});
            mesh.push_back({rectangle_points[i+1][j+1], rectangle_points[i+1][j], rectangle_points[i][j]});
//            vector<Vector3d> temp2;
//            temp2.push_back(rectangle_points[i+1][j+1]);
//            temp2.push_back(rectangle_points[i][j+1]);
//            temp2.push_back(rectangle_points[i][j]);
//            mesh.push_back(temp2);
            // 바둑판의 좌상귀? 잡듯이 점 선택해서 시계방향
        }
    }
}

void build_coons_mesh(vector<shared_ptr<Face>> ptr_faces){
    for(shared_ptr<Face> ptr_face : ptr_faces){
        if(ptr_face->vertices.size() == 3){ //삼각형
            build_triangle_mesh(ptr_face);
        }
        else if(ptr_face->vertices.size() == 4){ // 사각형
            build_rectangle_mesh(ptr_face);
        }
    }
}

void readfile(){
    vector<vector<shared_ptr<Vertex>>> offset_table;
    vector<shared_ptr<Edge>> ptr_edges;
    vector<shared_ptr<W_Edge>> ptr_winged_edges;
    vector<shared_ptr<Face>> ptr_faces;
    string offset_path = "/Users/minjun/Desktop/Progmramming Assignment #5/Programming Assignment#5/Programming Assignment#5/offset_table_rev.txt";
    readOffsetTable(offset_path, offset_table); // offset_table 읽어옴
    buildAllEdges(offset_table, ptr_edges); // offset_table을  바탕으로 edge 형성, 이를 vector type으로 저장
    for (int i = 0; i < ptr_edges.size(); i++){ // ptr_edge를 통해, edge 기준으로 연결된 winged_edge 형성
        buildWEdge(ptr_edges[i], ptr_winged_edges);
    }
//    printWedge(ptr_winged_edges);
    buildAllFace(ptr_winged_edges, ptr_faces);
    buildCoonFaces(ptr_faces);
//    printFace(ptr_faces);
//    faces_to_mesh(ptr_faces);
//    printControlpoints_Face(ptr_faces);
    build_coons_mesh(ptr_faces);

    printMesh(mesh);
}

int main(int argc, char* argv[]){
    
    readfile();
    Visualization(argc, argv);
    return 0;
}


