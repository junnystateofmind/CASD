    #include <iostream>
    #include <Eigen/Dense>
    #include <fstream>

    using namespace std;
    using namespace Eigen;

    class BezierCurve {
    public:
        BezierCurve() {};
        BezierCurve(const Vector2d& C0, const Vector2d& P1, const Vector2d& P2, const Vector2d& C3)
        {   C0_ = C0;
            C3_ = C3;
            findControlPoints(P1, P2);
        }
        BezierCurve(BezierCurve& frontBezier, const Vector2d& final_point){
            connectBezierCurve(frontBezier, final_point);
        }
        BezierCurve(const Matrix<double, 4,2>& point)
        {   C0_ = point.row(0);
            C3_ = point.row(3);
            findControlPoints(point.row(1), point.row(2));
        }
        Vector2d getControlPoint(int index) const { // control point 출력 메서드
            switch (index) {
                case 0:
                    return C0_;
                case 1:
                    return C1_;
                case 2:
                    return C2_;
                case 3:
                    return C3_;
                default:
                    throw std::out_of_range("Invalid control point index");
            }
        }
        Vector2d getBezierPoint(double t) {
            return Bezier(t, C0_, C1_, C2_, C3_);
        };
        
    private:
        Vector2d C0_;
        Vector2d C1_;
        Vector2d C2_;
        Vector2d C3_;
        double chord_length;
        // control point 벡터로 선언
        Vector2d Bezier(double t, const Vector2d& C0, const Vector2d& C1, const Vector2d& C2, const Vector2d& C3) {
            double oneMinusT = 1.0 - t;
            double oneMinusTSquared = oneMinusT * oneMinusT;
            double tSquared = t * t;
            return (C0 * oneMinusTSquared * oneMinusT) + (C1 * 3 * oneMinusTSquared * t) + (C2 * 3 * oneMinusT * tSquared) + (C3 * t * tSquared);
        }// Bezier 구현
        
        void findControlPoints(const Vector2d& P1, const Vector2d& P2) {
            double t1 = 1.0 / 3.0;
            double t2 = 2.0 / 3.0;

            Matrix2d M;
            M << (3 * t1 * (1 - t1) * (1 - t1)), (3 * t1 * t1 * (1 - t1)),
                  (3 * t2 * (1 - t2) * (1 - t2)), (3 * t2 * t2 * (1 - t2));

            Vector2d b1 = P1 - C0_ * (1 - t1) * (1 - t1) * (1 - t1) - C3_ * t1 * t1 * t1;
            Vector2d b2 = P2 - C0_ * (1 - t2) * (1 - t2) * (1 - t2) - C3_ * t2 * t2 * t2;

            Matrix<double, 2, 2> B;
            B.col(0) << b1(0), b2(0);
            B.col(1) << b1(1), b2(1);

            Matrix<double, 2, 2> controlPoints = M.inverse() * B;

            C1_ = controlPoints.row(0);
            C2_ = controlPoints.row(1);
            
            chord_length = (C3_-C0_).norm();//chord_length도 constructor 내부에 포함하기 위해 넣음
        }
        
        void connectBezierCurve(BezierCurve& frontBezier, const Vector2d& final_point){
            C0_ = frontBezier.C3_; // C0 condition
            C3_ = final_point; // 끝 점 설정
            chord_length = (final_point-C0_).norm(); // l2 설정
            double scale_factor = chord_length / frontBezier.chord_length; // chord_length 이용
            C1_ = scale_factor*2*frontBezier.C3_ - frontBezier.C2_;
            C2_ = scale_factor*4*frontBezier.C3_ - 4*frontBezier.C2_ + frontBezier.C1_;
        }


    };

    
    class drawing{
        public :
            void print_Bezier_Curve1(Vector<BezierCurve, 16>& hanguel, ofstream& outfile){
                for(int i = 0; i<16; i++){
                    outfile << "newpath" << endl;
                    outfile << hanguel(i).getControlPoint(0).x()/3 << " " << (1500 - hanguel(i).getControlPoint(0).y())/3 << " moveto" << endl;
                    for(int j = 0; j < 10; j++){
                        outfile << hanguel(i).getBezierPoint(j/10.0).x()/3 << " " << (1500 - hanguel(i).getBezierPoint(j/10.0).y())/3 << " lineto" << endl;
                    }
                    outfile << hanguel(i).getControlPoint(3).x()/3 << " " << (1500 - hanguel(i).getControlPoint(3).y())/3 << " lineto" << endl;
                    outfile << "stroke" << endl;
                }
            }

            void print_Bezier_Curve2(Vector<BezierCurve, 7>& hanguel, ofstream& outfile){
                for(int i = 0; i<7; i++){
                    outfile << "newpath" << endl;
                    outfile << hanguel(i).getControlPoint(0).x()/3 << " " << (1500 - hanguel(i).getControlPoint(0).y())/3 << " moveto" << endl;
                    for(int j = 0; j < 10; j++){
                        outfile << hanguel(i).getBezierPoint(j/10.0).x()/3 << " " << (1500 - hanguel(i).getBezierPoint(j/10.0).y())/3 << " lineto" << endl;
                    }
                    outfile << hanguel(i).getControlPoint(3).x()/3 << " " << (1500 - hanguel(i).getControlPoint(3).y())/3 << " lineto" << endl;
                    outfile << "stroke" << endl;
                }
            }
            void print_Bezier_Curve3(Vector<BezierCurve, 6>& hanguel, ofstream& outfile){
                for(int i = 0; i<6; i++){
                    outfile << "newpath" << endl;
                    outfile << hanguel(i).getControlPoint(0).x()/3 << " " << (1500 - hanguel(i).getControlPoint(0).y())/3 << " moveto" << endl;
                    for(int j = 0; j < 10; j++){
                        outfile << hanguel(i).getBezierPoint(j/10.0).x()/3 << " " << (1500 - hanguel(i).getBezierPoint(j/10.0).y())/3 << " lineto" << endl;
                    }
                    outfile << hanguel(i).getControlPoint(3).x()/3 << " " << (1500 - hanguel(i).getControlPoint(3).y())/3 << " lineto" << endl;
                    outfile << "stroke" << endl;
                }
            }
            void visualize_triangle_1(Vector<BezierCurve, 16>& hanguel, ofstream& outfile){
                //0과 15, 10과 9 poc_to_poc_edge 로 연결
                poc_to_poc_edge_first(hanguel, 0, 15, outfile);
                poc_to_poc_edge_first(hanguel, 10, 9, outfile);
                //1,4 4,13 6,12 8,11 poc_to_poc로 연결;
                poc_to_poc_first(hanguel, 1, 14, outfile);
                poc_to_poc_first(hanguel, 4, 13, outfile);
                poc_to_poc_first(hanguel, 6, 12, outfile);
                poc_to_poc_first(hanguel, 8, 11, outfile);
                //2,14 3,14 5,13 7,12 poc_to_point로 연결;
                poc_to_point_first(hanguel, 2, 14, outfile);
                poc_to_point_first(hanguel, 3, 14, outfile);
                poc_to_point_first(hanguel, 5, 13, outfile);
                poc_to_point_first(hanguel, 7, 12, outfile);
            }
            void poc_to_poc_edge_first(Vector<BezierCurve, 16>& hanguel, int a, int b, ofstream& outfile){
                outfile << "newpath" << endl;
                outfile << hanguel(a).getControlPoint(0).x()/3 << " " << (1500 - hanguel(a).getControlPoint(0).y())/3 << " moveto" << endl;
                for(int i = 0; i < 10; i++){
                    outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
                    outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
                }
                outfile <<hanguel(a).getBezierPoint(1).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(1).y())/3 << " lineto" << endl;
                outfile <<hanguel(b).getBezierPoint(0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(0).y())/3 << " lineto" << endl;
                outfile << "stroke" << endl;
            }
            void poc_to_poc_first(Vector<BezierCurve, 16>& hanguel, int a, int b, ofstream& outfile){
                outfile << "newpath" << endl;
                outfile << hanguel(a).getBezierPoint(0).x()/3 << " " << (1500-hanguel(a).getBezierPoint(0).y())/3 << " moveto" << endl;
                for(int i = 0; i < 10; i++){
                    outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500-hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
                    outfile << hanguel(a).getBezierPoint((i+1)/10.0).x()/3 << " " << (1500-hanguel(a).getBezierPoint((i+1)/10.0).y())/3 << " lineto" << endl;
                }
                outfile << "stroke" << endl;
            }
            void poc_to_point_first(Vector<BezierCurve, 16>& hanguel, int a, int b, ofstream& outfile){
                outfile << "newpath" << endl;
                for(int i = 0; i <11; i++){
                    outfile << hanguel(b).getControlPoint(0).x()/3 << " " << (1500 - hanguel(b).getControlPoint(0).y())/3 << " moveto" << endl;
                    outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
                }
                outfile << "stroke" << endl;
            }
        void visualize_triangle_2(Vector<BezierCurve, 7>& hanguel, ofstream& outfile){
            //0과 6, 4과 3 poc_to_poc_edge 로 연결
            poc_to_poc_edge_middle(hanguel, 0, 6, outfile);
            poc_to_poc_edge_middle(hanguel, 4, 3, outfile);
            //2,5 poc_to_poc로 연결;
            poc_to_poc_middle(hanguel, 2, 5, outfile);
            //1, 6 poc_to_point로 연결;
            poc_to_point_middle(hanguel, 1, 6, outfile);
        }
        void poc_to_poc_edge_middle(Vector<BezierCurve, 7>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            outfile << hanguel(a).getControlPoint(0).x()/3 << " " << (1500 - hanguel(a).getControlPoint(0).y())/3 << " moveto" << endl;
            for(int i = 0; i < 10; i++){
                outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
                outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
            }
            outfile <<hanguel(a).getBezierPoint(1).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(1).y())/3 << " lineto" << endl;
            outfile <<hanguel(b).getBezierPoint(0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(0).y())/3 << " lineto" << endl;
            outfile << "stroke" << endl;
        }
        void poc_to_poc_middle(Vector<BezierCurve, 7>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            outfile << hanguel(a).getBezierPoint(0).x()/3 << " " << (1500-hanguel(a).getBezierPoint(0).y())/3 << " moveto" << endl;
            for(int i = 0; i < 10; i++){
                outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500-hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
                outfile << hanguel(a).getBezierPoint((i+1)/10.0).x()/3 << " " << (1500-hanguel(a).getBezierPoint((i+1)/10.0).y())/3 << " lineto" << endl;
            }
            outfile << "stroke" << endl;
        }
        void poc_to_point_middle(Vector<BezierCurve, 7>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            for(int i = 0; i <11; i++){
                outfile << hanguel(b).getControlPoint(0).x()/3 << " " << (1500 - hanguel(b).getControlPoint(0).y())/3 << " moveto" << endl;
                outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
            }
            outfile << "stroke" << endl;
        }
        void visualize_triangle_3(Vector<BezierCurve, 6>& hanguel, ofstream& outfile){
            //0과 5, 4과 3 poc_to_poc_edge 로 연결
            poc_to_poc_edge_last(hanguel, 0, 5, outfile);
            poc_to_poc_edge_last(hanguel, 4, 3, outfile);
            //1,5 2,5 poc_to_point로 연결;
            poc_to_point_last(hanguel, 1, 5, outfile);
            poc_to_point_last(hanguel, 2, 5, outfile);
            
        }
        void poc_to_poc_edge_last(Vector<BezierCurve, 6>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            outfile << hanguel(a).getControlPoint(0).x()/3 << " " << (1500 - hanguel(a).getControlPoint(0).y())/3 << " moveto" << endl;
            for(int i = 0; i < 10; i++){
                outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
                outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
            }
            outfile <<hanguel(a).getBezierPoint(1).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(1).y())/3 << " lineto" << endl;
            outfile <<hanguel(b).getBezierPoint(0).x()/3 << " " << (1500 - hanguel(b).getBezierPoint(0).y())/3 << " lineto" << endl;
            outfile << "stroke" << endl;
        }
        void poc_to_poc_last(Vector<BezierCurve, 6>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            outfile << hanguel(a).getBezierPoint(0).x()/3 << " " << (1500-hanguel(a).getBezierPoint(0).y())/3 << " moveto" << endl;
            for(int i = 0; i < 10; i++){
                outfile << hanguel(b).getBezierPoint(1-i/10.0).x()/3 << " " << (1500-hanguel(b).getBezierPoint(1-i/10.0).y())/3 << " lineto" << endl;
                outfile << hanguel(a).getBezierPoint((i+1)/10.0).x()/3 << " " << (1500-hanguel(a).getBezierPoint((i+1)/10.0).y())/3 << " lineto" << endl;
            }
            outfile << "stroke" << endl;
        }
        void poc_to_point_last(Vector<BezierCurve, 6>& hanguel, int a, int b, ofstream& outfile){
            outfile << "newpath" << endl;
            for(int i = 0; i <11; i++){
                outfile << hanguel(b).getControlPoint(0).x()/3 << " " << (1500 - hanguel(b).getControlPoint(0).y())/3 << " moveto" << endl;
                outfile << hanguel(a).getBezierPoint(i/10.0).x()/3 << " " << (1500 - hanguel(a).getBezierPoint(i/10.0).y())/3 << " lineto" << endl;
            }
            outfile << "stroke" << endl;
        }
    };
    

    int main() {
        // 초성 'ㅁ' 구현
        Vector<BezierCurve,16> first_hanguel;
        
        Matrix<double, 4, 2> first_hanguel_0;
        Matrix<double, 4, 2> first_hanguel_1;
        Matrix<double, 4, 2> first_hanguel_2;
        Matrix<double, 4, 2> first_hanguel_3;
        Matrix<double, 4, 2> first_hanguel_4;
        Matrix<double, 4, 2> first_hanguel_5;
        Matrix<double, 4, 2> first_hanguel_6;
        Matrix<double, 4, 2> first_hanguel_7;
        Matrix<double, 4, 2> first_hanguel_8;
        Matrix<double, 4, 2> first_hanguel_9;
        Matrix<double, 4, 2> first_hanguel_10;
        Matrix<double, 4, 2> first_hanguel_11;
        Matrix<double, 4, 2> first_hanguel_12;
        Matrix<double, 4, 2> first_hanguel_13;
        Matrix<double, 4, 2> first_hanguel_14;
        Matrix<double, 4, 2> first_hanguel_15;
        first_hanguel_0 <<  451, 325,
                            485, 313,
                            521, 306,
                            557, 298;
        first_hanguel_1 <<  557, 298,
                            571, 289,
                            585, 279,
                            600, 276;
        first_hanguel_2 <<  600, 276,
                            630, 288,
                            653, 306,
                            668, 330;
        first_hanguel_3 <<  668, 330,
                            662, 344,
                            657, 355,
                            650, 368;
        first_hanguel_4 <<  650, 368,
                            648, 403,
                            640, 439,
                            631, 476;
        first_hanguel_5 <<  631, 476,
                            650, 511,
                            619, 537,
                            572, 544;
        first_hanguel_6 <<  572, 544,
                            541, 553,
                            506, 555,
                            473, 546;
        first_hanguel_7 <<  473, 546,
                            467, 562,
                            458, 576,
                            443, 586;
        first_hanguel_8 <<  443, 586,
                            409, 511,
                            398, 447,
                            387, 396;
        first_hanguel_9 <<  387, 396,
                            375, 369,
                            357, 347,
                            348, 324;
        first_hanguel_10 << 348, 324,
                            393, 311,
                            439, 332,
                            471, 369;
        first_hanguel_11 << 471, 369,
                            468, 414,
                            466, 457,
                            469, 508;
        first_hanguel_12 << 469, 508,
                            515, 498,
                            554, 487,
                            588, 475;
        first_hanguel_13 << 588, 475,
                            582, 432,
                            577, 389,
                            571, 350;
        first_hanguel_14 << 571, 350,
                            554, 355,
                            533, 362,
                            514, 367;
        first_hanguel_15 << 514, 367,
                            489, 359,
                            469, 344,
                            451, 325;
        first_hanguel(0) = BezierCurve(first_hanguel_0);
        first_hanguel(1) = BezierCurve(first_hanguel_1);
        first_hanguel(2) = BezierCurve(first_hanguel_2);
        first_hanguel(3) = BezierCurve(first_hanguel_3);
        first_hanguel(4) = BezierCurve(first_hanguel_4);
        first_hanguel(5) = BezierCurve(first_hanguel_5);
        first_hanguel(6) = BezierCurve(first_hanguel_6);
        first_hanguel(7) = BezierCurve(first_hanguel_7);
        first_hanguel(8) = BezierCurve(first_hanguel_8);
        first_hanguel(9) = BezierCurve(first_hanguel_9);
        first_hanguel(10) = BezierCurve(first_hanguel_10);
        first_hanguel(11) = BezierCurve(first_hanguel_11);
        first_hanguel(12) = BezierCurve(first_hanguel_12);
        first_hanguel(13) = BezierCurve(first_hanguel_13);
        first_hanguel(14) = BezierCurve(first_hanguel_14);
        first_hanguel(15) = BezierCurve(first_hanguel_15);
        
        // 중성 'ㅣ' 구현
        Vector<BezierCurve,7> middle_hanguel;
        Matrix<double, 4, 2> middle_hanguel_0;
        Matrix<double, 4, 2> middle_hanguel_1;
        Matrix<double, 4, 2> middle_hanguel_2;
        Matrix<double, 4, 2> middle_hanguel_3;
        Matrix<double, 4, 2> middle_hanguel_4;
        Matrix<double, 4, 2> middle_hanguel_5;
        Matrix<double, 4, 2> middle_hanguel_6;
        middle_hanguel_0 << 709, 160,
                            757, 150,
                            812, 171,
                            854, 198;
        middle_hanguel_1 << 854, 198,
                            866, 212,
                            869, 228,
                            866, 247;
        middle_hanguel_2 << 866, 247,
                            867, 311,
                            867, 377,
                            867, 441;
        middle_hanguel_3 << 867, 441,
                            858, 542,
                            836, 632,
                            790, 713;
        middle_hanguel_4 << 790, 713,
                            801, 649,
                            791, 590,
                            783, 528;
        middle_hanguel_5 << 783, 528,
                            781, 432,
                            782, 340,
                            780, 258;
        middle_hanguel_6 << 780, 258,
                            761, 220,
                            737, 190,
                            709, 160;
        middle_hanguel(0) = BezierCurve(middle_hanguel_0);
        middle_hanguel(1) = BezierCurve(middle_hanguel_1);
        middle_hanguel(2) = BezierCurve(middle_hanguel_2);
        middle_hanguel(3) = BezierCurve(middle_hanguel_3);
        middle_hanguel(4) = BezierCurve(middle_hanguel_4);
        middle_hanguel(5) = BezierCurve(middle_hanguel_5);
        middle_hanguel(6) = BezierCurve(middle_hanguel_6);
        // 종성 'ㄴ' 구현
        Vector<BezierCurve,6> last_hanguel;
        Matrix<double, 4, 2> last_hanguel_0;
        Matrix<double, 4, 2> last_hanguel_1;
        Matrix<double, 4, 2> last_hanguel_2;
        Matrix<double, 4, 2> last_hanguel_3;
        Matrix<double, 4, 2> last_hanguel_4;
        Matrix<double, 4, 2> last_hanguel_5;
        last_hanguel_0 <<   516, 667,
                            564, 655,
                            613, 674,
                            640, 715;
        last_hanguel_1 <<   640, 715,
                            640, 744,
                            640, 779,
                            657, 801;
        last_hanguel_2 <<   657, 801,
                            701, 799,
                            757, 788,
                            810, 777;
        last_hanguel_3 <<   810, 777,
                            851, 798,
                            833, 839,
                            793, 858;
        last_hanguel_4 <<   793, 858,
                            737, 873,
                            671, 870,
                            624, 839;
        last_hanguel_5 <<   624, 839,
                            588, 779,
                            546, 717,
                            516, 667;
        last_hanguel(0) = BezierCurve(last_hanguel_0);
        last_hanguel(1) = BezierCurve(last_hanguel_1);
        last_hanguel(2) = BezierCurve(last_hanguel_2);
        last_hanguel(3) = BezierCurve(last_hanguel_3);
        last_hanguel(4) = BezierCurve(last_hanguel_4);
        last_hanguel(5) = BezierCurve(last_hanguel_5);
        //파일 출력
        ofstream outFile("/Users/minjun/Desktop/2023-1/컴선설/Programming Assigment#2/Output_PA2.ps");
        

        if (!outFile) {
            cout << "Output_PA2 출력 불가." << endl;
            return 1;
        }
        outFile << "%!" << endl;
        outFile << "<< /PageSize [1500 1500] >> setpagedevice" << endl;
        outFile << "1 setlinewidth" << endl;
        outFile << "0 0 0 setrgbcolor" << endl;
        drawing d;
        d.print_Bezier_Curve1(first_hanguel, outFile);
        d.print_Bezier_Curve2(middle_hanguel, outFile);
        d.print_Bezier_Curve3(last_hanguel, outFile);
        outFile << "0 0 255 setrgbcolor" << endl;
        d.visualize_triangle_1(first_hanguel, outFile);
        d.visualize_triangle_2(middle_hanguel, outFile);
        d.visualize_triangle_3(last_hanguel, outFile);
        outFile << "showpage"<<endl;
        outFile.close();
        
        
        return 0;
    }
