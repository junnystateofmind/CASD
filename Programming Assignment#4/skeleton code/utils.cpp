/*
Copyright (c) 2019, Seung Won Choi

modified by Wonseok Kang
*/


#include "utils.h"

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
	int numOfZ = z_coords.size();
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
	int nb_row = offset_table.size();
	int nb_col = offset_table[0].size();
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



	shared_ptr<Vertex> start = ptr_edge->ptr_p1;
	shared_ptr<Vertex> end = ptr_edge->ptr_p2;

	// step 1 : check if each vertex of main edge has more than three connections.
	bool isPossible = (start->ptr_next.size() >= 3 && end->ptr_next.size() >= 3);

	if (isPossible) {
		// step 2 : check there are two faces
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

	int nb_next_1 = ptr_vertex_1->ptr_next.size();
	int nb_next_2 = ptr_vertex_2->ptr_next.size();
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