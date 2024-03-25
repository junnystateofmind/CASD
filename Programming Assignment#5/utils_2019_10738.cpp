/*
Copyright (c) 2019, Seung Won Choi

modified by Wonseok Kang
modified by Minjun Jang
*/


#include "utils_2019_10738.h"
#include <Eigen/Dense>
#include <unordered_set>

using namespace Eigen;

//The function that reads a line from text and stores it in a vector.
vector<double> readOneLine(string line)
{
    istringstream is(line);
    double tempVal;
    vector<double> tempVec;

    while (is >> tempVal)
        tempVec.push_back(tempVal);

    return tempVec;
}

void readOffsetTable(string file_name, vector<vector<shared_ptr<Vertex>>>& offset_table)
{
    ifstream file;
    file.open(file_name);
    vector<double> x_coords, y_coords, z_coords;
    string tempLine;
    vector<double> tempVec;

    //first row - z coordinates
    getline(file, tempLine);
    tempVec = readOneLine(tempLine);
    z_coords = tempVec;
    int numOfZ = static_cast<int>(z_coords.size());
    int numOfX = 0;

    int temp = 0;
    vector<vector<double>> tempY_coords;

    //other rows - x and y coordinates
    while (!file.eof())
    {
        temp++;
        getline(file, tempLine);
        if (tempLine.size() != 0)
        {
            tempVec = readOneLine(tempLine);
            x_coords.push_back(tempVec.front());
            tempVec.erase(tempVec.begin());
            y_coords = tempVec;
            tempY_coords.push_back(y_coords);
            numOfX++;
        }
    }

    vector<shared_ptr<Vertex>> ptr_tempvec_padding;



    for (int i = 0; i <= numOfZ + 1; i++) {
        shared_ptr<Vertex> temp_point_padding(new Vertex(0, -1, 0));
        ptr_tempvec_padding.push_back(temp_point_padding);
    }

    offset_table.push_back(ptr_tempvec_padding);

    //make matrix form
    for (int row = 0; row < numOfX; row++)
    {
        //vector<Vertex> tempVec;
        vector<shared_ptr<Vertex>> temp_vec;


        shared_ptr<Vertex> temp_point_padding(new Vertex(0, -1, 0));
        temp_vec.push_back(temp_point_padding);

        for (int column = 0; column < numOfZ; column++)
        {
            shared_ptr<Vertex> temp_point(new Vertex());
            temp_point->x = x_coords[row];
            //tempPoint.z = z_coords[column];
            temp_point->z = z_coords[column];
            vector<double> tempY_vector_org = tempY_coords[row];
            vector<double> tempY_vector_lower;
            vector<double> tempY_vector_upper;
            if (row == 0)
                tempY_vector_lower = tempY_coords[row + 1];

            else if (row == numOfX - 1)
                tempY_vector_upper = tempY_coords[row - 1];

            else
            {
                tempY_vector_upper = tempY_coords[row - 1];
                tempY_vector_lower = tempY_coords[row + 1];
            }

            if (tempY_vector_org[column] == 0)
            {
                int valid = 0;

                if (row == 0 && tempY_vector_lower[column] != 0)
                    valid = 1;

                else if (row == numOfX - 1 && tempY_vector_upper[column] != 0)
                    valid = 1;

                else if (row != 0 && row != numOfX - 1 && (tempY_vector_lower[column] != 0 || tempY_vector_upper[column] != 0))
                    valid = 1;

                else
                    valid = 0;

                if (column == 0)
                {
                    if (valid == 0 && tempY_vector_org[column] == 0 && tempY_vector_org[column + 1] == 0) {
                        //tempPoint.y = -1;
                        temp_point->y = -1;
                    }

                    else {
                        //tempPoint.y = 0;
                        temp_point->y = 0;
                    }
                }

                else if (column < numOfZ - 1)
                {
                    double sumOf_threeY = tempY_vector_org[column] + tempY_vector_org[column - 1] + tempY_vector_org[column + 1];
                    if (valid == 0 && sumOf_threeY == 0) {
                        //tempPoint.y = -1;
                        temp_point->y = -1;
                    }
                    else {
                        //tempPoint.y = 0;
                        temp_point->y = 0;
                    }
                }

                else if (column == numOfZ - 1)
                {
                    if (valid == 0 && tempY_vector_org[column - 1] == 0) {
                        //tempPoint.y = -1;
                        temp_point->y = -1;
                    }
                    else {
                        //tempPoint.y = 0;
                        temp_point->y = 0;
                    }
                }
            }
            else
                temp_point->y = tempY_vector_org[column] / 1000; //convert unit of y coordinate from mm to m

            //tempVec.push_back(tempPoint);
            temp_vec.push_back(temp_point);
        }
        shared_ptr<Vertex> temp_point_padding2(new Vertex(0, -1, 0));
        //tempVec.push_back(tempPoint_padding);
        temp_vec.push_back(temp_point_padding2);
        //offset_table.push_back(tempVec);
        offset_table.push_back(temp_vec);
    }

    //offset_table.push_back(tempVec_padding);
    vector<shared_ptr<Vertex>> ptr_tempvec_padding2;



    for (int i = 0; i <= numOfZ + 1; i++) {
        shared_ptr<Vertex> temp_point_padding(new Vertex(0, -1, 0));
        ptr_tempvec_padding2.push_back(temp_point_padding);
    }
    offset_table.push_back(ptr_tempvec_padding2);
    file.close();
}

bool isVertexValid(
    shared_ptr<Vertex> ptr_vertex) {
    return ptr_vertex->valid;
}

void addConnection(
    shared_ptr<Vertex>& ptr_p1,
    shared_ptr<Vertex>& ptr_p2) {

    if (ptr_p1->findConnection(ptr_p2) < 0 && ptr_p2->findConnection(ptr_p1) < 0) {
        ptr_p1->ptr_next.push_back(ptr_p2);
        ptr_p2->ptr_next.push_back(ptr_p1);
    }

}

void buildLocalEdge(
    shared_ptr<Vertex> ptr_target,
    shared_ptr<Vertex> ptr_adj_1,
    shared_ptr<Vertex> ptr_adj_2,
    shared_ptr<Vertex> ptr_corner,
    vector<shared_ptr<Edge>>& ptrs_edge) {

    // build adjacent edge


    // case 1~4

    bool case_1 = (isVertexValid(ptr_adj_1) && !isVertexValid(ptr_adj_2) && isVertexValid(ptr_corner));
    bool case_2 = (isVertexValid(ptr_adj_1) && isVertexValid(ptr_adj_2) && !isVertexValid(ptr_corner));
    bool case_3 = (!isVertexValid(ptr_adj_1) && isVertexValid(ptr_adj_2) && isVertexValid(ptr_corner));
    //bool case_4 = (isVertexValid(adj_1) && isVertexValid(adj_2) && isVertexValid(corner));

    if (case_1) {
        if ((ptr_target->findConnection(ptr_corner)) < 0) {
            shared_ptr<Edge> corner_edge(new Edge(ptr_target, ptr_corner));
            addConnection(ptr_target, ptr_corner);
            ptrs_edge.push_back(corner_edge);
        }
    }

    if (case_2) {
        if ((ptr_adj_1->findConnection(ptr_adj_2)) < 0) {
            shared_ptr<Edge> corner_edge(new Edge(ptr_adj_1, ptr_adj_2));
            addConnection(ptr_adj_1, ptr_adj_2);
            ptrs_edge.push_back(corner_edge);
        }
    }

    if (case_3) {
        if ((ptr_target->findConnection(ptr_corner)) < 0) {
            shared_ptr<Edge> corner_edge(new Edge(ptr_target, ptr_corner));
            addConnection(ptr_target, ptr_corner);
            ptrs_edge.push_back(corner_edge);
        }
    }
}

void buildAllEdges(
    vector<vector<shared_ptr<Vertex>>>& offset_table,
    vector<shared_ptr<Edge>>& ptrs_edge) {
    int nb_row = static_cast<int>(offset_table.size());
    int nb_col = static_cast<int>(offset_table[0].size());
    for (int i = 0; i < nb_row; i++) {
        for (int j = 0; j < nb_col; j++) {
            double valid = (offset_table[i][j]->y);
            if (offset_table[i][j]->y >= 0)
                offset_table[i][j]->setValid(true);
            else
                offset_table[i][j]->y = 0.0;
        }
    }
    // padding = 1

    for (int i = 1; i < nb_row - 1; i++) {
        for (int j = 1; j < nb_col - 1; j++) {

            //Vertex& target = offset_table[i][j];

            if (isVertexValid(offset_table[i][j])) {

                /*Vertex& left_upper = offset_table[i - 1][j - 1];
                Vertex& left = offset_table[i][j - 1];
                Vertex& left_under = offset_table[i + 1][j - 1];
                Vertex& under = offset_table[i + 1][j];
                Vertex& right_under = offset_table[i + 1][j + 1];
                Vertex& right = offset_table[i][j + 1];
                Vertex& right_upper = offset_table[i - 1][j + 1];
                Vertex& upper = offset_table[i - 1][j];
                */





                if (isVertexValid(offset_table[i - 1][j])) {
                    if ((offset_table[i][j]->findConnection(offset_table[i - 1][j])) < 0) {
                        shared_ptr<Edge> ptr_upper_edge(new Edge(offset_table[i][j], offset_table[i - 1][j]));
                        addConnection(offset_table[i][j], offset_table[i - 1][j]);
                        ptrs_edge.push_back(ptr_upper_edge);
                    }
                }

                if (isVertexValid(offset_table[i][j - 1])) {
                    if ((offset_table[i][j]->findConnection(offset_table[i][j - 1])) < 0) {
                        shared_ptr<Edge> ptr_left_edge(new Edge(offset_table[i][j], offset_table[i][j - 1]));
                        addConnection(offset_table[i][j], offset_table[i][j - 1]);
                        ptrs_edge.push_back(ptr_left_edge);
                    }
                }

                if (isVertexValid(offset_table[i + 1][j])) {
                    if ((offset_table[i][j]->findConnection(offset_table[i + 1][j])) < 0) {
                        shared_ptr<Edge> under_edge(new Edge(offset_table[i][j], offset_table[i + 1][j]));
                        addConnection(offset_table[i][j], offset_table[i + 1][j]);
                        ptrs_edge.push_back(under_edge);
                    }
                }

                if (isVertexValid(offset_table[i][j + 1])) {
                    if ((offset_table[i][j]->findConnection(offset_table[i][j + 1])) < 0) {
                        shared_ptr<Edge> right_edge(new Edge(offset_table[i][j], offset_table[i][j + 1]));
                        addConnection(offset_table[i][j], offset_table[i][j + 1]);
                        ptrs_edge.push_back(right_edge);
                    }
                }


                //// Corners

                //bool isNeedCorner = (i % 2 == 1 && j % 2 == 1);
                bool isNeedCorner = true;
                if (isNeedCorner) {
                    // Left Upper Rectangle Search
                    buildLocalEdge(offset_table[i][j], offset_table[i][j - 1], offset_table[i - 1][j], offset_table[i - 1][j - 1], ptrs_edge);

                    // Left Under Rectangle Search
                    buildLocalEdge(offset_table[i][j], offset_table[i][j - 1], offset_table[i + 1][j], offset_table[i + 1][j - 1], ptrs_edge);

                    // Right Under Rectangle Search
                    buildLocalEdge(offset_table[i][j], offset_table[i + 1][j], offset_table[i][j + 1], offset_table[i + 1][j + 1], ptrs_edge);

                    // RIght Upper Rectangle Search
                    buildLocalEdge(offset_table[i][j], offset_table[i][j + 1], offset_table[i - 1][j], offset_table[i - 1][j + 1], ptrs_edge);
                }


            }

        }
    }
}

void buildWEdge(
    shared_ptr<Edge> ptr_edge,
    vector<shared_ptr<W_Edge>>& ptrs_wedge) {

    shared_ptr<Vertex> start;
    shared_ptr<Vertex> end;
    if(ptr_edge->ptr_p1->y <= ptr_edge->ptr_p2->y){
        start = ptr_edge->ptr_p1;
        end = ptr_edge->ptr_p2;
    }
    else{
        start = ptr_edge->ptr_p2;
        end = ptr_edge->ptr_p1;
    }

    // step 1 : check if each vertex of main edge has more than three connections.
    bool isPossible = (start->ptr_next.size() >= 3 && end->ptr_next.size() >= 3);
    if (isPossible) {
        // step 2 : check there are two faces
        for (auto& w_edge : ptrs_wedge) {
            if ((w_edge->ptr_lp_start == end && w_edge->ptr_lp_end == start) ||
                (w_edge->ptr_rp_start == end && w_edge->ptr_rp_end == start)) {
                // a winged edge with swapped vertices already exists, so return
                return;
            }
        }
        vector<vector<int>> faces = isConnectedPathThrough(start, end);
        bool areThereTwoFaces = (faces.size() == 2);

        if (areThereTwoFaces) {
            // step 3 : build wings

            shared_ptr<Vertex> target_1 = end->ptr_next[faces[0][0]];
            shared_ptr<Vertex> target_2 = end->ptr_next[faces[1][0]];

            double angle_1 = calAngle(end->x, end->z, target_1->x, target_1->z);
            double angle_2 = calAngle(end->x, end->z, target_2->x, target_2->z);

            shared_ptr<W_Edge> w_edge(new W_Edge());
            w_edge->setEdge(start, end);

            shared_ptr<Vertex> target_left;
            shared_ptr<Vertex> target_right;

            vector<int> face_left;
            vector<int> face_right;


            // Left = bigger one
            if (angle_1 > angle_2) {
                target_left = target_1;
                target_right = target_2;

                face_left = faces[0];
                face_right = faces[1];
            }

            else {
                target_left = target_2;
                target_right = target_1;

                face_left = faces[1];
                face_right = faces[0];
            }

            //// Set Left Wing

            // if triangle
            // left prev = target_1 -> end
            // left next = start -> target_1

            if (face_left.size() == 1)
                w_edge->setLeftWing(target_left, end, start, target_left);


            // if rectangular
            // left prev = target_1 -> end
            // left next = end->ptr_next[faces[0][0]]->ptr_next[faces[0][1]] -> start

            else
                w_edge->setLeftWing(target_left, end, start, end->ptr_next[face_left[0]]->ptr_next[face_left[1]]);


            //// Set Right Wing

            // if triangle
            // right prev = target_2 -> start
            // right next = end -> target_2
            if (face_right.size() == 1)
                w_edge->setRightWing(target_right, start, end, target_right);

            // if rectangular
            // right prev = end->prt_next[faces[1][0]]->ptr_next[faces[0][1]] -> start
            // right next = end -> target_2

            else
                w_edge->setRightWing(end->ptr_next[face_right[0]]->ptr_next[face_right[1]], start, end, target_right);

            ptrs_wedge.push_back(w_edge);

                                                                                    
        }

    }

}

bool isSameVertex(
    shared_ptr<Vertex> ptr_vertex_1,
    shared_ptr<Vertex> ptr_vertex_2) {
    return (ptr_vertex_1 == ptr_vertex_2);
}

bool isConnected(
    shared_ptr<Vertex> ptr_vertex_1,
    shared_ptr<Vertex> ptr_vertex_2) {

    return (ptr_vertex_1->findConnection(ptr_vertex_2) >= 0 && ptr_vertex_2->findConnection(ptr_vertex_1) >= 0);
}

vector<vector<int>> isConnectedPathThrough(
    shared_ptr<Vertex> ptr_vertex_1,
    shared_ptr<Vertex> ptr_vertex_2) {

    vector<vector<int>> faces;

    int nb_next_1 = static_cast<int>(ptr_vertex_1->ptr_next.size());
    int nb_next_2 = static_cast<int>(ptr_vertex_2->ptr_next.size());
    int nb_tri = 0;
    int nb_rectangle = 0;
    // Search Triangle
    for (int i = 0; i < nb_next_1; i++) {
        for (int j = 0; j < nb_next_2; j++) {
            if (ptr_vertex_1->ptr_next[i] == ptr_vertex_2->ptr_next[j] && ptr_vertex_2->ptr_next[j] != ptr_vertex_1 && ptr_vertex_1->ptr_next[i] != ptr_vertex_2) {

                // target point : end
                vector<int> triangle;
                triangle.push_back(j);
                faces.push_back(triangle);

            }
        }
    }

    for (int i = 0; i < nb_next_1; i++) {
        for (int j = 0; j < nb_next_2; j++) {
            if (!(ptr_vertex_1->ptr_next[i] == ptr_vertex_2) && !(ptr_vertex_2->ptr_next[j] == ptr_vertex_1)) {
                if (isConnected(ptr_vertex_1->ptr_next[i], ptr_vertex_2->ptr_next[j])) {
                    
                    // target point : end
                    int idx = ptr_vertex_2->ptr_next[j]->findConnection(ptr_vertex_1->ptr_next[i]);
                    vector<int> rectangle;
                    rectangle.push_back(j);
                    rectangle.push_back(idx);
                    rectangle.push_back(i);
                    faces.push_back(rectangle);
                }
            }
        }
    }

    return faces;
}

double calAngle(double x1, double z1, double x2, double z2) {
    double dot = x1 * x2 + z1 * z2 + 0.000001;
    double det = x1 * z2 - x2 * z1 + 0.000001;
    double angle = atan2(det, dot);
    return angle;
}

Vector3d centroid_triangle(shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c){
    double x_coord = (a->x+b->x+c->x)/3.0;
    double y_coord = (a->y+b->y+c->y)/3.0;
    double z_coord = (a->z+b->z+c->z)/3.0;
    return Vector3d({x_coord,y_coord, z_coord});
}

Vector3d centroid_rectangle(shared_ptr<Vertex> a, shared_ptr<Vertex> b, shared_ptr<Vertex> c, shared_ptr<Vertex> d){
    double x_coord = (a->x+b->x+c->x+d->x)/4.0;
    double y_coord = (a->y+b->y+c->y+d->y)/4.0;
    double z_coord = (a->z+b->z+c->z+d->z)/4.0;
    return Vector3d({x_coord,y_coord, z_coord});
}

Vector3d vertex_to_vec3(shared_ptr<Vertex> vertex){
    return Vector3d(vertex->x, vertex->y, vertex->z);
}

void setFace(shared_ptr<Face>& face){
    Vector3d vec1 = vertex_to_vec3(face->vertices[1]) - vertex_to_vec3(face->vertices[0]);
    Vector3d vec2 = vertex_to_vec3(face->vertices[2]) - vertex_to_vec3(face->vertices[0]);
    Vector3d normal = vec1.cross(vec2); // 법선벡터 설정
    Vector3d centroid;
    if(face->vertices.size() == 3){
        centroid = centroid_triangle(face->vertices[0], face->vertices[1], face->vertices[2]);
    }else if(face->vertices.size() == 4){
        centroid = centroid_rectangle(face->vertices[0], face->vertices[1], face->vertices[2], face->vertices[3]);
    }
    Vector3d centroid_projection;
    centroid_projection << 0, centroid[1] , 0;
    if(normal.dot(centroid_projection)<0){
        if(face->vertices.size() == 3){
            shared_ptr<Vertex> temp = face->vertices[1];
            face->vertices[1] = face->vertices[2];
            face->vertices[2] = temp;
        }else{
            shared_ptr<Vertex> temp = face->vertices[1];
            face->vertices[1] = face->vertices[3];
            face->vertices[3] = temp;
        }
    }
}

void buildAllFace(vector<shared_ptr<W_Edge>>& ptr_wedges, vector<shared_ptr<Face>>& ptr_faces){
    vector<Vector3d> find_overlap;
    Vector3d epsilon;
    epsilon << 0.0001, 0.0001, 0.0001;
    for (int i = 0; i < ptr_wedges.size(); i++){
        if (isSameVertex(ptr_wedges[i]->ptr_lp_start, ptr_wedges[i]->ptr_ln_end) == true) { // 삼각형 face의 경우
            vector<shared_ptr<Vertex>> points;
            points.push_back(ptr_wedges[i]->ptr_lp_start);
            points.push_back(ptr_wedges[i]->ptr_end);
            points.push_back(ptr_wedges[i]->ptr_start);
            Vector3d overlap = centroid_triangle(ptr_wedges[i]->ptr_lp_start, ptr_wedges[i]->ptr_end, ptr_wedges[i]->ptr_start);
            bool isOverlapFound = false;
            for (const auto& existingOverlap : find_overlap) {
                if ((overlap - existingOverlap).norm() < epsilon.norm()) {
                    isOverlapFound = true;
                    break;
                }
            }
            if (isOverlapFound) {
                continue;
            } else {
                find_overlap.push_back(overlap);
                shared_ptr<Face> addFace = make_shared<Face>(points);
                setFace(addFace);
                ptr_faces.push_back(addFace);
            }
        } else {
            vector<shared_ptr<Vertex>> points;
            points.push_back(ptr_wedges[i]->ptr_lp_start);
            points.push_back(ptr_wedges[i]->ptr_end);
            points.push_back(ptr_wedges[i]->ptr_start);
            points.push_back(ptr_wedges[i]->ptr_ln_end);
            Vector3d overlap = centroid_rectangle(ptr_wedges[i]->ptr_lp_start, ptr_wedges[i]->ptr_end, ptr_wedges[i]->ptr_start, ptr_wedges[i]->ptr_ln_end);
            bool isOverlapFound = false;
            for (const auto& existingOverlap : find_overlap) {
                if ((overlap - existingOverlap).norm() < epsilon.norm()) {
                    isOverlapFound = true;
                    break;
                }
            }
            if (isOverlapFound) {
                continue;
            } else {
                find_overlap.push_back(overlap);
                shared_ptr<Face> addFace = make_shared<Face>(points);
                setFace(addFace);
                ptr_faces.push_back(addFace);
            }
        }
        if (isSameVertex(ptr_wedges[i]->ptr_rp_start, ptr_wedges[i]->ptr_rp_end) == true) { // 삼각형 face의 경우
            vector<shared_ptr<Vertex>> points;
            points.push_back(ptr_wedges[i]->ptr_rp_start);
            points.push_back(ptr_wedges[i]->ptr_start);
            points.push_back(ptr_wedges[i]->ptr_end);
            Vector3d overlap = centroid_triangle(ptr_wedges[i]->ptr_rp_start, ptr_wedges[i]->ptr_start, ptr_wedges[i]->ptr_end);
            bool isOverlapFound = false;
            for (const auto& existingOverlap : find_overlap) {
                if ((overlap - existingOverlap).norm() < epsilon.norm()) {
                    isOverlapFound = true;
                    break;
                }
            }
            if (isOverlapFound) {
                continue;
            } else {
                find_overlap.push_back(overlap);
                shared_ptr<Face> addFace = make_shared<Face>(points);
                setFace(addFace);
                ptr_faces.push_back(addFace);
            }
        }
        else{
            vector<shared_ptr<Vertex>> points;
            points.push_back(ptr_wedges[i]->ptr_rp_start);
            points.push_back(ptr_wedges[i]->ptr_start);
            points.push_back(ptr_wedges[i]->ptr_end);
            points.push_back(ptr_wedges[i]->ptr_rn_end);
            Vector3d overlap = centroid_rectangle(ptr_wedges[i]->ptr_rp_start, ptr_wedges[i]->ptr_start, ptr_wedges[i]->ptr_end, ptr_wedges[i]->ptr_rn_end);
            bool isOverlapFound = false;
            for (const auto& existingOverlap : find_overlap) {
                if ((overlap - existingOverlap).norm() < epsilon.norm()) {
                    isOverlapFound = true;
                    break;
                }
            }
            if (isOverlapFound) {
                continue;
            } else {
                find_overlap.push_back(overlap);
                shared_ptr<Face> addFace = make_shared<Face>(points);
                setFace(addFace);
                ptr_faces.push_back(addFace);
            }
        }
    }
}

void buildCoonFaces(vector<shared_ptr<Face>>& ptr_faces){
    for(shared_ptr<Face> ptr_face : ptr_faces){
        if(ptr_face->vertices.size() == 3){ // triangle Coons-patch
            ptr_face->allocate_normal_to_vertex();
        }
        else if(ptr_face->vertices.size() == 4){ // rectangular Coons-patch
            ptr_face->allocate_normal_to_vertex();
        }
    }
    for(shared_ptr<Face> ptr_face : ptr_faces){
        if(ptr_face->vertices.size() == 3){ // triangle Coons-patch
            triangle_coons_patch(ptr_face);
        }
        else if(ptr_face->vertices.size() == 4){ // rectangular Coons-patch
            rectangular_coons_patch(ptr_face);
        }
    }
}

void triangle_coons_patch(shared_ptr<Face>& ptr_face){
    for(shared_ptr<Vertex> inner_vertex : ptr_face->vertices){ // vertices의 normal vector 계산
        inner_vertex->calculate_ponit_normal();
    }
    vector<Vector3d> temp;
    temp.push_back(Vector3d(ptr_face->vertices[0]->x, ptr_face->vertices[0]->y, ptr_face->vertices[0]->z)); // 첫 번째 control point는 face의 첫 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[0], ptr_face->vertices[1]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[1], ptr_face->vertices[0]));
    temp.push_back(Vector3d(ptr_face->vertices[1]->x, ptr_face->vertices[1]->y, ptr_face->vertices[1]->z)); // 4번째 control point는 face의 두 번째 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[1], ptr_face->vertices[2]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[2], ptr_face->vertices[1]));
    temp.push_back(Vector3d(ptr_face->vertices[2]->x, ptr_face->vertices[2]->y, ptr_face->vertices[2]->z)); // 7번째 control point는 face의 세 번째 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[2], ptr_face->vertices[0]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[0], ptr_face->vertices[2]));
    // 외곽의 control point 계산 완료
    
    Vector3d middle_controlpoint = (1.0/2)*((1.0/2) * (temp[1]+temp[2]+temp[4]+temp[5]+temp[7]+temp[8]) - (1.0/3) * (temp[0]+temp[3]+temp[6]));
    // 내부 control point 계산 완료
    
    ptr_face->controlpoints.push_back(temp[0]);
    ptr_face->controlpoints.push_back(temp[8]);
    ptr_face->controlpoints.push_back(temp[1]);
    ptr_face->controlpoints.push_back(temp[7]);
    ptr_face->controlpoints.push_back(middle_controlpoint);
    ptr_face->controlpoints.push_back(temp[2]);
    ptr_face->controlpoints.push_back(temp[6]);
    ptr_face->controlpoints.push_back(temp[5]);
    ptr_face->controlpoints.push_back(temp[4]);
    ptr_face->controlpoints.push_back(temp[3]);
    
//                0
//            1       2
//        3       4       5
//    6       7       8       9
    
}

void rectangular_coons_patch(shared_ptr<Face>& ptr_face){
    for(shared_ptr<Vertex> inner_vertex : ptr_face->vertices){ // vertices의 normal vector 계산
        inner_vertex->calculate_ponit_normal();
    }
    vector<Vector3d> temp;
    temp.push_back(Vector3d(ptr_face->vertices[0]->x, ptr_face->vertices[0]->y, ptr_face->vertices[0]->z)); // 첫 번째 control point는 face의 첫 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[0], ptr_face->vertices[1]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[1], ptr_face->vertices[0]));
    temp.push_back(Vector3d(ptr_face->vertices[1]->x, ptr_face->vertices[1]->y, ptr_face->vertices[1]->z)); // 4번째 control point는 face의 두 번째 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[1], ptr_face->vertices[2]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[2], ptr_face->vertices[1]));
    temp.push_back(Vector3d(ptr_face->vertices[2]->x, ptr_face->vertices[2]->y, ptr_face->vertices[2]->z)); // 7번째 control point는 face의 세 번째 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[2], ptr_face->vertices[3]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[3], ptr_face->vertices[2]));
    temp.push_back(Vector3d(ptr_face->vertices[3]->x, ptr_face->vertices[3]->y, ptr_face->vertices[3]->z)); // 10번째 control point는 face의 네 번째 vertex
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[3], ptr_face->vertices[0]));
    temp.push_back(calculate_near_controlpoint(ptr_face->vertices[0], ptr_face->vertices[3]));
    // 외곽의 control point 계산 완료
    
    Vector3d middle_controlpoint11 = (1.0/3)*(2*temp[1]+temp[8]+2*temp[11]+temp[4])-(1.0/9)*(4*temp[0]+2*temp[3]+2*temp[9]+temp[6]);
    Vector3d middle_controlpoint12 = (1.0/3)*(2*temp[2]+temp[7]+2*temp[4]+temp[11])-(1.0/9)*(4*temp[3]+2*temp[6]+2*temp[0]+temp[9]);
    Vector3d middle_controlpoint22 = (1.0/3)*(2*temp[7]+temp[2]+2*temp[5]+temp[10])-(1.0/9)*(4*temp[6]+2*temp[9]+2*temp[3]+temp[0]);
    Vector3d middle_controlpoint21 = (1.0/3)*(2*temp[8]+temp[1]+2*temp[10]+temp[5])-(1.0/9)*(4*temp[9]+2*temp[0]+2*temp[6]+temp[3]);
    // 내부 control point 계산 완료
    
    ptr_face->controlpoints.push_back(temp[0]);
    ptr_face->controlpoints.push_back(temp[11]);
    ptr_face->controlpoints.push_back(temp[10]);
    ptr_face->controlpoints.push_back(temp[9]);
    ptr_face->controlpoints.push_back(temp[1]);
    ptr_face->controlpoints.push_back(middle_controlpoint11);
    ptr_face->controlpoints.push_back(middle_controlpoint12);
    ptr_face->controlpoints.push_back(temp[8]);
    ptr_face->controlpoints.push_back(temp[2]);
    ptr_face->controlpoints.push_back(middle_controlpoint21);
    ptr_face->controlpoints.push_back(middle_controlpoint22);
    ptr_face->controlpoints.push_back(temp[7]);
    ptr_face->controlpoints.push_back(temp[3]);
    ptr_face->controlpoints.push_back(temp[4]);
    ptr_face->controlpoints.push_back(temp[5]);
    ptr_face->controlpoints.push_back(temp[6]);
    
//    30-31-32-33       12-13-14-15
//    20-21-22-23       8-9-10-11
//    10-11-12-13       4-5-6-7
//    00-01-02-03       0-1-2-3
//    이 순서로 입력해줌
    
}

Vector3d calculate_near_controlpoint(shared_ptr<Vertex> near, shared_ptr<Vertex> far){
    Vector3d projected_point = 1.0/3.0 * (vertex_to_vec3(far) - vertex_to_vec3(near));
    Vector3d point_normal_nomalized = near->point_normal.normalized(); // point normal의 단위벡터화
    Vector3d reproject_projected_point = projected_point - point_normal_nomalized.dot(projected_point) * point_normal_nomalized; // point normal을 법선으로 갖는 평면에 정사영 때린 벡터
    double multiply = projected_point.norm()/reproject_projected_point.norm(); // 배율 구하고
    Vector3d controlpoint = vertex_to_vec3(near) + multiply * multiply * reproject_projected_point; // 시컨트 두번 때림
    cout << "x : " << controlpoint.x() << ", y : " << controlpoint.y() << ", z = " << controlpoint.z() << endl;
    return controlpoint;
}

Vector3d deCasteljau_triangle(shared_ptr<Face> face, double u, double v){
    // 첫번째 과정
    Vector3d first = deCasteljau_for_triangle(face->controlpoints[0], face->controlpoints[1], face->controlpoints[2], u, v);
    Vector3d second = deCasteljau_for_triangle(face->controlpoints[1], face->controlpoints[3], face->controlpoints[4], u, v);
    Vector3d third = deCasteljau_for_triangle(face->controlpoints[2], face->controlpoints[4], face->controlpoints[5], u, v);
    Vector3d fourth = deCasteljau_for_triangle(face->controlpoints[3], face->controlpoints[6], face->controlpoints[7], u, v);
    Vector3d fifth = deCasteljau_for_triangle(face->controlpoints[4], face->controlpoints[7], face->controlpoints[8], u, v);
    Vector3d sixth = deCasteljau_for_triangle(face->controlpoints[5], face->controlpoints[8], face->controlpoints[9], u, v);
    // 두번쩨 과정
    Vector3d second_first = deCasteljau_for_triangle(first, second, third, u, v);
    Vector3d second_second = deCasteljau_for_triangle(second, fourth, fifth, u, v);
    Vector3d second_third = deCasteljau_for_triangle(third, fifth, sixth, u, v);
    // 마지막
    return deCasteljau_for_triangle(second_first, second_second, second_third, u, v);
}

Vector3d deCasteljau_for_triangle(Vector3d cp0, Vector3d cp1, Vector3d cp2, double u, double v){
    return (1.0-u-v) * cp0 + u * cp1 + v * cp2;
}

Vector3d deCasteljau_rectangle(shared_ptr<Face> face, double u, double v){
    Vector3d first = deCasteljau(face->controlpoints[0], face->controlpoints[1], face->controlpoints[2], face->controlpoints[3], u);
    Vector3d second = deCasteljau(face->controlpoints[4], face->controlpoints[5], face->controlpoints[6], face->controlpoints[7], u);
    Vector3d third = deCasteljau(face->controlpoints[8], face->controlpoints[9], face->controlpoints[10], face->controlpoints[11], u);
    Vector3d fourth = deCasteljau(face->controlpoints[12], face->controlpoints[13], face->controlpoints[14], face->controlpoints[15], u);
    
    return deCasteljau(first, second, third, fourth, v);
}

Vector3d deCasteljau(Vector3d cp0, Vector3d cp1, Vector3d cp2, Vector3d cp3, double t){
    Vector3d a = cp0 + t * (cp1 - cp0);
    Vector3d b = cp1 + t * (cp2 - cp1);
    Vector3d c = cp2 + t * (cp3 - cp2);

    Vector3d d = a + t * (b - a);
    Vector3d e = b + t * (c - b);

    Vector3d pointOnCurve = d + t * (e - d);

    return pointOnCurve;
}

//void build_triangle_mesh(shared_ptr<Face> ptr_face){
//    int step_size = 10;
//    vector<vector<Vector3d>> triangle_points;
//    for(int i = 0; i <= step_size; i++){
//        for(int j = 0; j <= i; j++){
//            // 이 행에서 u, v의 합은 (1/step_size) * i;
//            triangle_points[i].push_back(deCasteljau_triangle(ptr_face, i*(1/step_size)*(i-j), i*(1/step_size)*j));
//        }
//    }
//    //  n층의 피라미드 형식으로 pos 찾기 완료
//    for(int i = 0; i < step_size; i++){
//        for(int j = 0; j < triangle_points[i].size(); j++){
//            
//        }
//    }
//}

