#include <Eigen/Dense>
#include <Eigen/QR>    

Eigen::MatrixXf calcs(float x, float y, float z, float roll, float pitch, float yaw)
{
	Eigen::Vector3f T1 = {1, -1, -1};
	Eigen::Vector3f T2 = {1, 1, -1};
	Eigen::Vector3f T3 = {1, -1, 1};
	Eigen::Vector3f T4 = {1, 1, -1};
	Eigen::Vector3f T5 = {-1, -1, -1};
	Eigen::Vector3f T6 = {-1, 1, -1};
	Eigen::Vector3f T7 = {-1, -1, 1};
	Eigen::Vector3f T8 = {-1, 1, -1};

	Eigen::Vector3f D1 = {1, 1, 1};
	Eigen::Vector3f D2 = {1, -1, 1};
	Eigen::Vector3f D3 = {1, 1, -1};
	Eigen::Vector3f D4 = {1, -1, -1};
	Eigen::Vector3f D5 = {-1, 1, 1};
	Eigen::Vector3f D6 = {-1, -1, 1};
	Eigen::Vector3f D7 = {-1, 1, -1};
	Eigen::Vector3f D8 = {-1, -1, -1};

	Eigen::Vector3f M1 = D1.cross(T1);
	Eigen::Vector3f M2 = D2.cross(T2);
	Eigen::Vector3f M3 = D3.cross(T3);
	Eigen::Vector3f M4 = D4.cross(T4);
	Eigen::Vector3f M5 = D5.cross(T5);
	Eigen::Vector3f M6 = D6.cross(T6);
	Eigen::Vector3f M7 = D7.cross(T7);
	Eigen::Vector3f M8 = D8.cross(T8);

	Eigen::MatrixXf C1(6, 1);
	C1 << T1, M1; 
	Eigen::MatrixXf C2(6, 1);
	C2 << T2, M2; 
	Eigen::MatrixXf C3(6, 1);
	C3 << T3, M3; 
	Eigen::MatrixXf C4(6, 1);
	C4 << T4, M4; 
	Eigen::MatrixXf C5(6, 1);
	C5 << T5, M5; 
	Eigen::MatrixXf C6(6, 1);
	C6 << T6, M6; 
	Eigen::MatrixXf C7(6, 1);
	C7 << T7, M7; 
	Eigen::MatrixXf C8(6, 1);
	C8 << T8, M8; 

	Eigen::MatrixXf R(6, 8);
	R.col(0) = C1;
	R.col(1) = C2;
	R.col(2) = C3;
	R.col(3) = C4;
	R.col(4) = C5;
	R.col(5) = C6;
	R.col(6) = C7;
	R.col(7) = C8;

	Eigen::MatrixXf I = R.completeOrthogonalDecomposition().pseudoInverse();
	Eigen::MatrixXf b(6, 1);
	b << x, y, z, roll, pitch, yaw;
	
	return I * b;
}
