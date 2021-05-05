#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;

int main()
{
	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;


	// transpose with eigen library
	std::cout << "traspose" << std::endl;
	Matrix<int, 2, 2> a;
	a << 1, 2, 3, 4;
	std::cout << "Here is the matrix a:\n" << a << std::endl;
	std::cout << "Here is the matrix a^T\n" << a.transpose() << std::endl;
	//a = a.transpose();
	//std::cout << "Here is the matrix a = a.transpose()\n" << a << std::endl;

	Matrix2i b;
	b = a.transpose();
	std::cout << "Here is the matrix b = a.transpose()\n" << b << std::endl;

	a.transposeInPlace();
	std::cout << "Here is the matrix a.transposeInPlace()\n" << a << std::endl;

	// Matrix-Matrix and matrix-vector multiplication
	Matrix2d mat;
	mat << 1, 2, 3, 4;

	Vector2d u(-1, 1), v(2, 0);

	std::cout << "Here is mat*mat:\n" << mat * mat << std::endl;
	std::cout << "Here is mat*u:\n" << mat * u << std::endl;
	std::cout << "Here is u^T*mat:\n" << u.transpose() * mat << std::endl;

	mat << mat * mat;
	std::cout << "Here is mat*mat:\n" << mat << std::endl;


	// Dot product and cross product
	Vector3d mm(1, 2, 3);
	Vector3d nn(0, 1, 2);
	std::cout << "Dot product:" << mm.dot(nn) << std::endl;

	double dp = mm.adjoint() * nn;
	std::cout << "Dot product via a matrix product:" << dp << std::endl;
	std::cout << "Cross product:\n" << mm.cross(nn) << std::endl;

	// basic arithmetic reduction operation
	std::cout << "Here is mat.sum():         " << mat.sum() << std::endl;
	std::cout << "Here is mat.prod():        " << mat.prod() << std::endl;
	std::cout << "Here is mat.mean():        " << mat.mean() << std::endl;
	std::cout << "Here is mat.maxCoeff():    " << mat.maxCoeff() << std::endl;
	std::cout << "Here is mat.minCoeff():    " << mat.minCoeff() << std::endl;
	std::cout << "Here is mat.trace():       " << mat.trace() << std::endl;


	Matrix3f mn = Matrix3f::Random();
	std::ptrdiff_t i, j;
	float minOfM = mn.minCoeff(&i, &j);
	std::cout << "Here is the matrix mn:\n" << mn << std::endl;
	std::cout << "Its minimum coefficient (" << minOfM << ")" <<
		"is at position (" << i << " , " << j << ")" << std::endl;

	return 0;
}
