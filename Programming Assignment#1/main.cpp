#include <iostream>
#include <cstdio>
#include <Eigen/Dense>


using namespace std;
using namespace Eigen;

class Transformation {
private:
    MatrixXd pre_image_points1;
    MatrixXd post_image_points1;
    MatrixXd pre_image_points2;
    MatrixXd post_image_points2;
    Vector<float, 3> circle_elements;
public:
    Transformation() {
        pre_image_points1 = MatrixXd(3, 2);
        post_image_points1 = MatrixXd(3, 2);
        pre_image_points2 = MatrixXd(4, 2);
        post_image_points2 = MatrixXd(4, 2);
        circle_elements = Vector<float, 3>(3);
    }
    
    MatrixXd get_private_pre1(){
        return pre_image_points1;
    }
    MatrixXd get_private_post1(){
        return post_image_points1;
    }
    MatrixXd get_private_pre2(){
        return pre_image_points2;
    }
    MatrixXd get_private_post2(){
        return post_image_points2;
    }
    Vector<float, 3> get_private_circle_elelments(){
        return circle_elements;
    }
    
    void input1(FILE* fp) {
        char buf[256];
        float num;
        fgets(buf, 100, fp); // P1
        fgets(buf, 100, fp); // "pre-image points"
        
        pre_image_points1 = MatrixXd::Zero(3, 2);
        for (int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++){
                if(fscanf(fp, "%f", &num) == 1){
                    pre_image_points1(i,j) = num;}
            }
        }
        fgets(buf, 100, fp);//행바꿈
        fgets(buf, 100, fp);//행바꿈
        fgets(buf, 100, fp);// "post-image points"
        
        post_image_points1 = MatrixXd::Zero(3, 2);
        for (int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++){
                if(fscanf(fp, "%f", &num) == 1){
                    post_image_points1(i,j) = num;}
            }
        }
        
    }
    
    void input2(FILE* fp) {
        char buf[256];
        float num;
        fgets(buf, 100, fp); // P2
        fgets(buf, 100, fp); // "pre-image points"
        
        pre_image_points2 = MatrixXd::Zero(4, 2);
        for (int i = 0; i < 4; i++) {
            for(int j = 0; j < 2; j++){
                if(fscanf(fp, "%f", &num) == 1){
                    pre_image_points2(i,j) = num;}
            }
        }
        fgets(buf, 100, fp);//행바꿈
        fgets(buf, 100, fp);//행바꿈
        fgets(buf, 100, fp);// "post-image points"
        
        post_image_points2 = MatrixXd::Zero(4, 2);
        for (int i = 0; i < 4; i++) {
            for(int j = 0; j < 2; j++){
                if(fscanf(fp, "%f", &num) == 1){
                    post_image_points2(i,j) = num;}
            }
        }
        
    }
    
    void input3(FILE* fp){
        char buf[256];
        float num;
        fgets(buf, 100, fp); // P3
        fgets(buf, 100, fp); // circle image points (center_x, center_y, radius)
        for (int i = 0; i < 3; i++){
            if(fscanf(fp, "%f", &num) == 1){
                circle_elements(i) = num;}
        }
    }


    void print_pre_post1() {
        cout << "Pre-image points:\n" << pre_image_points1 << endl;
        cout << "Post-image points:\n" << post_image_points1 << endl;
        }
    void print_pre_post2() {
        cout << "Pre-image points:\n" << pre_image_points2 << endl;
        cout << "Post-image points:\n" << post_image_points2 << endl;
        }
    
    
    void print(const MatrixXd &array){
        for(int i = 0; i < array.rows(); i++){
            for(int j = 0; j < array.cols(); j++){
                cout << array(i, j) << " ";
            }
            cout << endl;
        }
    }
    
    MatrixXd Transverse_pre1_3X3() const {
            MatrixXd pre = MatrixXd::Ones(3, 3);
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 2; j++) {
                    pre(j, i) = pre_image_points1(i, j);
                }
            }
            return pre;
        }
    MatrixXd Transverse_post1_3X3() const {
        MatrixXd post = MatrixXd::Ones(3, 3);
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++) {
                post(j, i) = post_image_points1(i, j);
            }
        }
        return post;
    }
    MatrixXd Transverse_pre2_3X4() const {
            MatrixXd pre2 = MatrixXd::Ones(3, 4);
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 2; j++) {
                    pre2(j, i) = pre_image_points2(i, j);
                }
            }
            return pre2;
    }
    MatrixXd Transverse_post2_3X4() const {
            MatrixXd post2 = MatrixXd::Ones(3, 4);
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 2; j++) {
                    post2(j, i) = post_image_points2(i, j);
                }
            }
            return post2;
    }
    
    
    void print_Output1(MatrixXd& affine_pre1) {
          FILE *fp;
          fp = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/OutPut1.ps", "w"); // 파일을 쓰기 모드로 열기
          if (fp == NULL) {
            printf("파일 열기 실패\n");
            return;
          }
          
          // PostScript 코드 작성
        fprintf(fp, "%%!PS\n"); // PostScript 파일 시작
        
        fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
        fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
        fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
        fprintf(fp, "1 0 0 setrgbcolor\n");
        //pre_image_points
        fprintf(fp, "%f %f moveto \n", 50*pre_image_points1(0,0), 50*pre_image_points1(0,1));
        for(int i = 1; i < 3; i++){
            float x_ps = 50*pre_image_points1(i,0);
            float y_ps = 50*pre_image_points1(i,1);
            fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
        }
        fprintf(fp, "closepath\n");
        fprintf(fp, "stroke \n");
        //post_image_points
        fprintf(fp, "newpath\n");
        fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
        fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
        fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
        fprintf(fp, "0 1 0 setrgbcolor\n");// post point 그리기
        fprintf(fp, "%f %f moveto \n", 50*post_image_points1(0,0), 50*post_image_points1(0,1));
        for(int i = 1; i < 3; i++){
            float x_ps = 50*post_image_points1(i,0);
            float y_ps = 50*post_image_points1(i,1);
            fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
        }
        fprintf(fp, "closepath\n");
        fprintf(fp, "stroke \n");
        
        //affine transformation
        fprintf(fp, "newpath\n");
        fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
        fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
        fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
        fprintf(fp, "0 0 1 setrgbcolor\n");// post point 그리기
        fprintf(fp, "%f %f moveto \n", 50*affine_pre1(0,0)+10, 50*affine_pre1(0,1)+10); // 동일하게 겹칠 것을 고려해서 (10,10) 평행이동
        for(int i = 1; i < 3; i++){
            float x_ps = 50*affine_pre1(i,0)+10;
            float y_ps = 50*affine_pre1(i,1)+10;
            fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
        }
        fprintf(fp, "closepath\n");
        fprintf(fp, "stroke \n");
        
        fprintf(fp, "showpage\n"); // 페이지 출력
          
        fclose(fp); // 파일 닫기
        }
    
    void print_Output2(MatrixXd& affine_pre2) {
        FILE *fp;
        fp = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/Output2.ps", "w"); // 파일을 쓰기 모드로 열기
        if (fp == NULL) {
            printf("파일 열기 실패\n");
            return;
        }
            
            // PostScript 코드 작성
          fprintf(fp, "%%!PS\n"); // PostScript 파일 시작
          
          fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
          fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
          fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
          fprintf(fp, "1 0 0 setrgbcolor\n");//pre point 그리기
          fprintf(fp, "%f %f moveto \n", 100*pre_image_points2(0,0), 100*pre_image_points2(0,1));
          for(int i = 1; i < 4; i++){
              float x_ps = 100*pre_image_points2(i,0);
              float y_ps = 100*pre_image_points2(i,1);
              fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
          }
          fprintf(fp, "closepath\n");
          fprintf(fp, "stroke \n");
          
          fprintf(fp, "newpath\n");
          fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
          fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
          fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
          fprintf(fp, "0 1 0 setrgbcolor\n");// post point 그리기
          fprintf(fp, "%f %f moveto \n", 100*post_image_points2(0,0), 100*post_image_points2(0,1));
          for(int i = 1; i < 4; i++){
              float x_ps = 100*post_image_points2(i,0);
              float y_ps = 100*post_image_points2(i,1);
              fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
          }
          fprintf(fp, "closepath\n");
          fprintf(fp, "stroke \n");
        
            fprintf(fp, "newpath\n");
            fprintf(fp, "2 setlinewidth\n"); // 선 굵기 설정
            fprintf(fp, "1 setlinecap\n"); // 끝 점 모양 설정
            fprintf(fp, "1 setlinejoin\n"); // 교차점 처리 모양 설정
            fprintf(fp, "0 0 1 setrgbcolor\n");// post point 그리기
            fprintf(fp, "%f %f moveto \n", 100*affine_pre2(0,0), 100*affine_pre2(0,1));
            for(int i = 1; i < 4; i++){
                float x_ps = 100*affine_pre2(i,0);
                float y_ps = 100*affine_pre2(i,1);
                fprintf(fp, "%f %f lineto \n", x_ps, y_ps);
            }
            fprintf(fp, "closepath\n");
            fprintf(fp, "stroke \n");
        
          fprintf(fp, "showpage\n"); // 페이지 출력
            
          fclose(fp); // 파일 닫기
          }
    void print_Output3() {
          FILE *fp;
          fp = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/OutPut3.ps", "w"); // 파일을 쓰기 모드로 열기
          if (fp == NULL) {
            printf("파일 열기 실패\n");
            return;
          }
          
          // PostScript 코드 작성
        fprintf(fp, "%%!PS\n"); // PostScript 파일 시작
        fprintf(fp, "newpath\n");
        fprintf(fp, "%f %f %f 0 360 arc\n",100*circle_elements(0),100*circle_elements(1),100*circle_elements(2));
        fprintf(fp, "stroke\n");
        fprintf(fp, "%f %f %f 0 360 arc\n",100*circle_elements(0),100*circle_elements(1),100*circle_elements(2));
        fprintf(fp, "closepath\n");
        fprintf(fp, "0 0 0 setrgbcolor\n");
        fprintf(fp, "fill\n");
        fprintf(fp, "showpage\n"); // 페이지 출력
        fclose(fp); // 파일 닫기
        }
};






int main() {
    // 1번
    Transformation IP1;
    FILE* fp1 = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/Input_P1.txt", "r");
    IP1.input1(fp1);
    IP1.print_pre_post1();
    fclose(fp1);
    
    MatrixXd pre1 = IP1.Transverse_pre1_3X3();
    MatrixXd post1 = IP1.Transverse_post1_3X3();
    cout << "pre1 \n";
    IP1.print(pre1);
    cout << "post1 \n";
    IP1.print(post1);
    MatrixXd Affine_IP1 = post1*pre1.inverse();
    cout << "affine 변환 행렬 IP1 : \n";
    MatrixXd Affined_pre1 = (Affine_IP1*pre1).transpose();
    IP1.print(Affine_IP1);
    IP1.print_Output1(Affined_pre1);
    
    // 2번
    Transformation IP2;
    FILE* fp2 = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/Input_P2.txt", "r");
    IP2.input2(fp2);
    IP2.print_pre_post2();
    fclose(fp2);
    MatrixXd pre2 = IP2.Transverse_pre2_3X4();
    MatrixXd post2 = IP2.Transverse_post2_3X4();
    cout << "pre2 \n";
    IP2.print(pre2);
    cout << "post2 \n";
    IP2.print(post2);
    MatrixXd x = IP2.get_private_pre2();
    cout << "x \n";
    IP2.print(x);
    MatrixXd y =IP2.get_private_post2();
    cout << "y \n";
    IP2.print(y);
    MatrixXd AffineIP2 = y.transpose()*pre2.transpose()*(pre2*pre2.transpose()).inverse();
    cout << "affine 변환 행렬 IP2 : \n";
    IP2.print(AffineIP2);
    MatrixXd Affined_pre2 = (AffineIP2*pre2).transpose();
    IP2.print_Output2(Affined_pre2);
    IP2.print(Affined_pre2);
    
    // 3번
    Transformation IP3;
    FILE* fp3 = fopen("/Users/minjun/Desktop/2023-1/컴선설/Programming Assignment#1/Input_P3.txt", "r");
    IP3.input3(fp3);
    IP3.print_Output3();
    
    
    return 0;
}
