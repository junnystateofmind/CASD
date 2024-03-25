//
//  BezierCurve.cpp
//  Programming Assignment#3
//
//  Created by Minjun on 2023/05/08.
//

#include "BezierCurve.hpp"
#include <iostream>
#include <Eigen/dense>

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
    BezierCurve(BezierCurve& frontBezier, const Vector2d& final_point){ // C2 continuity
        Continuity2_BezierCurve(frontBezier, final_point);
    }
    BezierCurve(BezierCurve& frontBezier, const Vector2d& third_point, const Vector2d& final_point){ // C1 continuity
        Continuity1_BezierCurve(frontBezier, third_point, final_point);
    }
    BezierCurve(BezierCurve& frontBezier, const Vector2d& P1, const Vector2d& P2, const Vector2d& C3){ // C0 continuity
        C0_ = frontBezier.C3_;
        C3_ = C3;
        findControlPoints(P1, P2);
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
    
    void Continuity2_BezierCurve(BezierCurve& frontBezier, const Vector2d& final_point){// C2 continuity로 BezierCurve 연결
        C0_ = frontBezier.C3_; // C0 condition
        C3_ = final_point; // 끝 점 설정
        chord_length = (final_point-C0_).norm(); // l2 설정
        double scale_factor = chord_length / frontBezier.chord_length; // chord_length 이용
        C1_ = (scale_factor+1)*frontBezier.C3_ - scale_factor*frontBezier.C2_; // C1 condition,(C3-C2)/l1 =(C5-C4)/l2
        C2_ = 2*C1_-C0_+scale_factor*scale_factor*(frontBezier.C3_-2*frontBezier.C2_+frontBezier.C1_);
    }
    
    void Continuity1_BezierCurve(BezierCurve& frontBezier, const Vector2d& third_point, const Vector2d& final_point){
        C0_ = frontBezier.C3_; // C0 condition
        C3_ = final_point; // 끝 점 설정
        chord_length = (final_point - C0_).norm();
        double scale_factor = chord_length / frontBezier.chord_length; // chord_length 이용
        C1_ = (scale_factor+1)*frontBezier.C3_ - scale_factor*frontBezier.C2_; // C1 condition,(C3-C2)/l1 =(C5-C4)/l2
        C2_ = (27*third_point - C0_ - 6*C1_-8*C3_)/12; // third point t=2/3일때 가정 삽입
    }
    
};
