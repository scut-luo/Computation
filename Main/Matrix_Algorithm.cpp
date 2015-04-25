/*
	矩阵算法
*/

#include "Computation.h"

Matrix_Algorithm::Matrix_Algorithm()
{
	eps = 1e-6;
}

//选取列主元
void Matrix_Algorithm::ConvertMatrix(double **Augmented_Matrix,const int k,const int dimension)
{
	double	temp;
	double	max = Augmented_Matrix[k][k];
	int		convert = k ;

	for(int i=k;i<dimension;i++)
	{
		if(fabs(Augmented_Matrix[i][k]) > fabs(max))
		{
			convert = i;
			max = Augmented_Matrix[convert][i];
		}
	}
	for(int j=k;j<dimension+1;j++)
	{
		temp = Augmented_Matrix[k][j];
		Augmented_Matrix[k][j] = Augmented_Matrix[convert][j];
		Augmented_Matrix[convert][j] = temp;
	}
}

//Gauss列主元消元法
ErrNo Matrix_Algorithm::Gauss(double **Augmented_Matrix,double *result,double &determinant,const int dimension)
{
	double	L,oldvalue;

	determinant = 1.0f;

	for(int k=0;k<dimension-1;k++)
	{
		oldvalue = Augmented_Matrix[k][k];
		ConvertMatrix(Augmented_Matrix,k,dimension);
		if(fabs(Augmented_Matrix[k][k]) < eps)			//矩阵奇异  
		{
			return DIVIDE_BY_ZERO;
		}
		if(Augmented_Matrix[k][k] != oldvalue)
		{
			determinant *=(-1);
		}
		for(int i=k+1;i<dimension;i++)
		{
			if(fabs(Augmented_Matrix[k][k]) < eps)
				return DIVIDE_BY_ZERO;
			L = Augmented_Matrix[i][k]/Augmented_Matrix[k][k];
			for(int j=k;j<dimension+1;j++)
			{
				Augmented_Matrix[i][j] -= Augmented_Matrix[k][j]*L;

			}
		}
		determinant *= Augmented_Matrix[k][k];
	}
	determinant *= Augmented_Matrix[dimension-1][dimension-1];

	//回代 得出结果
	double sum;
	if(fabs(Augmented_Matrix[dimension-1][dimension-1]) < eps)
		return DIVIDE_BY_ZERO;
	result[dimension-1] = Augmented_Matrix[dimension-1][dimension]/Augmented_Matrix[dimension-1][dimension-1];
	for(int i=dimension-2;i>=0;i--)
	{
		sum = 0.0f;
		for(int j=i+1;j<dimension;j++)
			sum += Augmented_Matrix[i][j]*result[j];
		if(fabs(Augmented_Matrix[i][i]) < eps)
			return DIVIDE_BY_ZERO;
		result[i] = (Augmented_Matrix[i][dimension]-sum)/Augmented_Matrix[i][i];
	}

	return 0;
}

//Gauss列主元消元法(不修改原始数据)
ErrNo Matrix_Algorithm::Gauss_NOC(double **matrix,double *result,double &determinant,const int dimension)
{
	ErrNo  err;
	double **Augmented_Matrix = NULL;

	// 申请空间
	if(!(Augmented_Matrix = new double* [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		// 多一列
		if(!(Augmented_Matrix[i] = new double [dimension+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension+1;j++)
		{
			Augmented_Matrix[i][j] = matrix[i][j];
		}
	}

	determinant = 1.0f;

	if(err = Gauss(Augmented_Matrix,result,determinant,dimension))
	{
		// 销毁空间
		for(int i=0;i<dimension;i++)
			delete [] Augmented_Matrix[i];
		delete [] Augmented_Matrix;

		return err;
	}

	// 销毁空间
	for(int i=0;i<dimension;i++)
		delete [] Augmented_Matrix[i];
	delete [] Augmented_Matrix;

	return 0;
}

//LU分解法
ErrNo Matrix_Algorithm::LU(double **Augmented_Matrix,double *result,const int dimension)
{
	double	sum = 0.0;

	for(int k=0;k<dimension;k++)
	{
		//计算U矩阵元素
		for(int j=k;j<dimension;j++)
		{
			sum = 0.0;
			for(int r=0;r<k;r++)
				sum += Augmented_Matrix[k][r]*Augmented_Matrix[r][j];
			Augmented_Matrix[k][j] -= sum;
		}

		//计算L矩阵元素
		for(int i=k+1;i<dimension;i++)
		{
			sum = 0.0;
			for(int r=0;r<k;r++)
				sum += Augmented_Matrix[i][r]*Augmented_Matrix[r][k];
			if(fabs(Augmented_Matrix[k][k]) < eps)
				return DIVIDE_BY_ZERO;
			Augmented_Matrix[i][k] = (Augmented_Matrix[i][k] - sum)/Augmented_Matrix[k][k];
		}
	}

	//解下三角方程
	for(int i=1;i<dimension;i++)
	{
		sum = 0.0;
		for(int r=0;r<i;r++)
			sum += Augmented_Matrix[i][r]*Augmented_Matrix[r][dimension];
		Augmented_Matrix[i][dimension] -= sum;
	}

	//解上三角方程
	if(fabs(Augmented_Matrix[dimension-1][dimension-1])< eps)
		return DIVIDE_BY_ZERO;
	result[dimension-1] = Augmented_Matrix[dimension-1][dimension]/Augmented_Matrix[dimension-1][dimension-1];
	for(int i=dimension-2;i>=0;i--)
	{
		sum = 0.0;
		for(int j=i+1;j<dimension;j++)
			sum += Augmented_Matrix[i][j]*result[j];
		if(fabs(Augmented_Matrix[i][i])< eps)
			return DIVIDE_BY_ZERO;
		result[i] = (Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
	}

	return 0;
}

// LU分解法（不修改原始矩阵）
ErrNo Matrix_Algorithm::LU_NOC(double **matrix,double *result,const int dimension)
{
	double **Augmented_Matrix = NULL;
	double	sum = 0.0;

	// 申请空间
	if(!(Augmented_Matrix = new double* [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		// 多一列
		if(!(Augmented_Matrix[i] = new double [dimension+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension+1;j++)
		{
			Augmented_Matrix[i][j] = matrix[i][j];
		}
	}

	for(int k=0;k<dimension;k++)
	{
		//计算U矩阵元素
		for(int j=k;j<dimension;j++)
		{
			sum = 0.0;
			for(int r=0;r<k;r++)
				sum += Augmented_Matrix[k][r]*Augmented_Matrix[r][j];
			Augmented_Matrix[k][j] -= sum;
		}

		//计算L矩阵元素
		for(int i=k+1;i<dimension;i++)
		{
			sum = 0.0;
			for(int r=0;r<k;r++)
				sum += Augmented_Matrix[i][r]*Augmented_Matrix[r][k];
			if(fabs(Augmented_Matrix[k][k]) < eps)
				return DIVIDE_BY_ZERO;
			Augmented_Matrix[i][k] = (Augmented_Matrix[i][k] - sum)/Augmented_Matrix[k][k];
		}
	}

	//解下三角方程
	for(int i=1;i<dimension;i++)
	{
		sum = 0.0;
		for(int r=0;r<i;r++)
			sum += Augmented_Matrix[i][r]*Augmented_Matrix[r][dimension];
		Augmented_Matrix[i][dimension] -= sum;
	}

	//解上三角方程
	if(fabs(Augmented_Matrix[dimension-1][dimension-1])< eps)
		return DIVIDE_BY_ZERO;
	result[dimension-1] = Augmented_Matrix[dimension-1][dimension]/Augmented_Matrix[dimension-1][dimension-1];
	for(int i=dimension-2;i>=0;i--)
	{
		sum = 0.0;
		for(int j=i+1;j<dimension;j++)
			sum += Augmented_Matrix[i][j]*result[j];
		if(fabs(Augmented_Matrix[i][i])< eps)
			return DIVIDE_BY_ZERO;
		result[i] = (Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
	}

	// 销毁空间
	for(int i=0;i<dimension;i++)
		delete [] Augmented_Matrix[i];
	delete [] Augmented_Matrix;

	return 0;
}

//追赶法(Thomas算法)
ErrNo Matrix_Algorithm::Thomas(double **Tridiagonal_Matrix,double *result,const int dimension)
{
	//L和U矩阵在原矩阵中
	for(int i=1;i<dimension;i++)
	{
		if(fabs(Tridiagonal_Matrix[1][i-1]) < eps)
			return DIVIDE_BY_ZERO;
		Tridiagonal_Matrix[0][i-1] /= Tridiagonal_Matrix[1][i-1];								//计算L矩阵
		Tridiagonal_Matrix[1][i] -= Tridiagonal_Matrix[0][i-1]*Tridiagonal_Matrix[2][i-1];		//计算U矩阵
	}

	//求解Ly=d
	result[0] = Tridiagonal_Matrix[3][0];
	for(int i=1;i<dimension;i++)
		result[i] = Tridiagonal_Matrix[3][i]-Tridiagonal_Matrix[0][i-1]*result[i-1];

	//求解Ux=y
	result[dimension-1] /= Tridiagonal_Matrix[1][dimension-1];
	for(int i=dimension-2;i>=0;i--)
	{

		if(fabs(Tridiagonal_Matrix[1][i]) < eps)
			return DIVIDE_BY_ZERO;
		result[i] = (result[i]-Tridiagonal_Matrix[2][i]*result[i+1])/Tridiagonal_Matrix[1][i];
	}

	return 0;
}

//追赶法(Thomas算法) 注：不修改原始矩阵
ErrNo Matrix_Algorithm::Thomas_NOC(double **matrix,double *result,const int dimension)
{
	ErrNo err;

	double **Tridiagonal_Matrix = NULL;

	// 申请空间
	if(!(Tridiagonal_Matrix = new double* [4]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		// 多一列
		if(!(Tridiagonal_Matrix[i] = new double [dimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<dimension;j++)
		{
			Tridiagonal_Matrix[i][j] = matrix[i][j];
		}
	}

	if(err = Thomas(Tridiagonal_Matrix,result,dimension))
	{
		for(int i=0;i<4;i++)
			delete [] Tridiagonal_Matrix[i];
		delete [] Tridiagonal_Matrix;
		return err;
	}


	for(int i=0;i<4;i++)
		delete [] Tridiagonal_Matrix[i];
	delete [] Tridiagonal_Matrix;
	return 0;
}

//平方根法(Cholesky算法)
ErrNo Matrix_Algorithm::Cholesky(double **Augmented_Matrix,double *result,const int dimension)
{
	int			i,j,k;
	double		sum = 0.0;
	double		ftemp;

	for(j=0;j<dimension;j++)
	{
		sum = 0.0;
		for(k=0;k<=j-1;k++)
			sum += Augmented_Matrix[j][k]*Augmented_Matrix[j][k];
		ftemp = Augmented_Matrix[j][j]-sum;
		if(ftemp<0)														//判断开根数是否为负数
			return NEGATIVE_NUMBER;
		Augmented_Matrix[j][j] = sqrt(ftemp);
		for(i=j+1;i<dimension;i++)
		{
			sum = 0.0;
			for(k=0;k<=j-1;k++)
				sum += Augmented_Matrix[i][k]*Augmented_Matrix[j][k];
			if(fabs(Augmented_Matrix[j][j]) < eps)
				return DIVIDE_BY_ZERO;
			Augmented_Matrix[i][j] = (Augmented_Matrix[i][j] - sum)/Augmented_Matrix[j][j];
			Augmented_Matrix[j][i] = Augmented_Matrix[i][j];
		}
	}

	if(fabs(Augmented_Matrix[0][0]) < eps)
		return DIVIDE_BY_ZERO;
	result[0] = Augmented_Matrix[0][dimension]/Augmented_Matrix[0][0];
	for(i=1;i<dimension;i++)
	{
		sum = 0.0;
		for(j=0;j<=i-1;j++)
			sum += Augmented_Matrix[i][j]*result[j];
		if(fabs(Augmented_Matrix[i][i]) < eps)
			return DIVIDE_BY_ZERO;
		result[i] = (Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
	}

	if(fabs(Augmented_Matrix[dimension-1][dimension-1]) < eps)
		return DIVIDE_BY_ZERO;
	result[dimension-1] = result[dimension-1]/Augmented_Matrix[dimension-1][dimension-1];
	for(i=dimension-2;i>=0;i--)
	{
		sum = 0.0;
		for(j=i+1;j<dimension;j++)
			sum += Augmented_Matrix[i][j]*result[j];
		if(fabs(Augmented_Matrix[i][i]) < eps)
			return DIVIDE_BY_ZERO;
		result[i] = (result[i] - sum)/Augmented_Matrix[i][i];
	}

	return 0;
}

ErrNo Matrix_Algorithm::Cholesky_NOC(double **matrix,double *result,const int dimension)
{
	ErrNo err;
	double **Augmented_Matrix = NULL;

	// 申请空间
	if(!(Augmented_Matrix = new double* [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		// 多一列 增广矩阵
		if(!(Augmented_Matrix[i] = new double [dimension+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension+1;j++)
		{
			Augmented_Matrix[i][j] = matrix[i][j];
		}
	}

	if(err = Cholesky(matrix,result,dimension))
	{
		// 销毁空间
		for(int i=0;i<dimension;i++)
			delete [] Augmented_Matrix[i];
		delete [] Augmented_Matrix;
		return err;
	}

	// 销毁空间
	for(int i=0;i<dimension;i++)
		delete [] Augmented_Matrix[i];
	delete [] Augmented_Matrix;

	return 0;
}

//Jacobi迭代法
ErrNo Matrix_Algorithm::Jacobi(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision)
{
	int			k=0;
	double		sum,epsilon;
	double     *x_later,*x_pre;

	if(!(x_later = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(x_pre = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;

	for(int i=0;i<dimension;i++)
		x_pre[i] = iteration_vector[i];

	r_precision = fabs(precision);
	r_interations = iterations;

	for(k=1;k<=iterations;k++)
	{
		for(int i=0;i<dimension;i++)
		{
			sum = 0.0;
			for(int j=0;j<dimension;j++)
			{
				if(j!=i)
				{
					sum += Augmented_Matrix[i][j] * x_pre[j];
				}
			}
			if(fabs(Augmented_Matrix[i][i]) < eps)
			{
				delete[] x_pre;
				delete[] x_later;
				return DIVIDE_BY_ZERO;
			}
			x_later[i] = (Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
		}

		epsilon = 0.0;
		for(int i=0;i<dimension;i++)							//取得两个算的值的最大误差
		{
			if(epsilon < fabs(x_later[i]-x_pre[i]))
				epsilon = fabs(x_later[i]-x_pre[i]);
		}

		r_precision = fabs(epsilon);

		for(int i=0;i<dimension;i++)
			x_pre[i] = x_later[i];

		if(fabs(epsilon) < fabs(precision))									//判断是否达到迭代精度
		{
			r_precision = fabs(epsilon);
			break;
		}
	}

	if(k<=iterations)
		r_interations = k;

	for(int i=0;i<dimension;i++)
		result[i] = x_pre[i];

	delete[] x_pre;
	delete[] x_later;
	return 0;
}

//Gauss-Seidel迭代法
ErrNo Matrix_Algorithm::Gauss_Seidel(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision)
{
	int				k;
	double			*x;							
	double			epsilon,epsilon_t,sum;

	if(!(x = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;

	r_precision = fabs(precision);
	r_interations = iterations;
	
	for(int i=0;i<dimension;i++)
	{
		x[i] = iteration_vector[i];
	}

	for(k=1;k<=iterations;k++)
	{
		epsilon = 0.0;
		for(int i=0;i<dimension;i++)
		{
			sum = 0.0;
			for(int j=0;j<dimension;j++)
				sum += Augmented_Matrix[i][j]*x[j];

			if(fabs(Augmented_Matrix[i][i]) < eps)
			{
				delete[] x;
				return DIVIDE_BY_ZERO;
			}

			epsilon_t = (Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
			if(fabs(epsilon_t) > fabs(epsilon))
				epsilon = fabs(epsilon_t);

			x[i] = x[i] + epsilon_t;
		}

		r_precision = fabs(epsilon);

		if(fabs(epsilon) < fabs(precision))
		{
			r_precision = fabs(epsilon);
			break;
		}
	}

	if(k<=iterations)
		r_interations = k;

	for(int i=0;i<dimension;i++)
	{
		result[i] = x[i];
	}

	delete[] x;
	return 0;
}

//SOR(超松弛因子法)
ErrNo Matrix_Algorithm::SOR(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,const double relaxationfactor,int &r_interations,double &r_precision)
{
	int				k;
	double			*x;							
	double			epsilon,epsilon_t,sum;

	r_precision = fabs(precision);
	r_interations = iterations;

	if(!(x = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;

	for(int i=0;i<dimension;i++)
	{
		x[i] = iteration_vector[i];
	}

	for(k=1;k<=iterations;k++)
	{
		epsilon = 0.0;
		for(int i=0;i<dimension;i++)
		{
			sum = 0.0;
			for(int j=0;j<dimension;j++)
				sum += Augmented_Matrix[i][j]*x[j];

			if(fabs(Augmented_Matrix[i][i]) < eps)
			{
				delete[] x;
				return DIVIDE_BY_ZERO;
			}

			epsilon_t = relaxationfactor*(Augmented_Matrix[i][dimension] - sum)/Augmented_Matrix[i][i];
			if(fabs(epsilon_t) > fabs(epsilon))
				epsilon = fabs(epsilon_t);

			x[i] = x[i] + epsilon_t;
		}

		r_precision = fabs(epsilon);

		if(fabs(epsilon) < fabs(precision))
		{
			r_precision = fabs(epsilon);
			break;
		}
	}

	if(k<=iterations)
		r_interations = k;

	for(int i=0;i<dimension;i++)
	{
		result[i] = x[i];
	}

	delete[] x;
	return 0;
}

//QR(单步特征值计算) 注：未加速（未完善）
ErrNo Matrix_Algorithm::QR(double **matrix,double *eigenvalues,double **eigenvectors,const int nDimension,int iterations)
{
	ErrNo err;
	double **matrix_hess = NULL;
	double **matrix_house = NULL;
	double **matrix_Q = NULL;
	double **matrix_R = NULL;

	if(!(matrix_hess = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_house = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_hess[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
		if(!(matrix_house[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 计算原矩阵的Hessenberg矩阵和Householder矩阵
	if(err = Hess(matrix,matrix_hess,matrix_house,nDimension))
		return err;

	// 删除matrix_house存储空间 节约空间
	for(int i=0;i<nDimension;i++)
	{
		delete [] matrix_house[i];
	}
	delete [] matrix_house;
	

	// QR分解
	// 申请空间保存Q和R矩阵
	if(!(matrix_Q = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_R = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_Q[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
		if(!(matrix_R[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}
	// 迭代QR分解
	iterations = 50;
	for(int k=0;k<iterations;k++)
	{
		// QR分解
		if(err = QR_Decomposition(matrix_hess,matrix_Q,matrix_R,nDimension))
			return err;
		if(!(multiply_matrix(matrix_hess,matrix_R,matrix_Q,nDimension,nDimension,nDimension,nDimension,nDimension,nDimension)))
			return NO_RESULT;
	}


	// 删除申请的空间
	for(int i=0;i<nDimension;i++)
	{
		delete [] matrix_hess[i];
		delete [] matrix_Q[i];
		delete [] matrix_R[i];
	}
	delete [] matrix_hess;
	delete [] matrix_Q;
	delete [] matrix_R;
	return 0;
}

//QR(单步特征值计算) 注：加速
ErrNo Matrix_Algorithm::QR_Accelerate(double **matrix,double *eigenvalues,const int nDimension,int iterations,const double precision)
{
	ErrNo err;
	double **matrix_hess = NULL;
	double **matrix_house = NULL;
	double **matrix_Q = NULL;
	double **matrix_R = NULL;

	if(!(matrix_hess = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_house = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_hess[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
		if(!(matrix_house[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 计算原矩阵的Hessenberg矩阵和Householder矩阵
	if(err = Hess(matrix,matrix_hess,matrix_house,nDimension))
		return err;

	// 删除matrix_house存储空间 节约空间
	for(int i=0;i<nDimension;i++)
	{
		delete [] matrix_house[i];
	}
	delete [] matrix_house;


	// QR分解
	// 申请空间保存Q和R矩阵
	if(!(matrix_Q = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_R = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_Q[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
		if(!(matrix_R[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 带平移的QR分解
	double matrix_norm,temp_u;			// 矩阵的行范数
	int n = nDimension;
	int k = 0;

	matrix_norm = matrix_norm_r(matrix_hess,nDimension);
	while(n >= 2)
	{
		// 迭代
		for(k=0;k<iterations;k++)
		{
			temp_u = matrix_hess[n-1][n-1];
			// 对角线元素平移
			for(int i=0;i<n;i++)
			{
				matrix_hess[i][i] -= temp_u;
			}
			// QR分解
			if(err = QR_Decomposition(matrix_hess,matrix_Q,matrix_R,n))
				return err;
			if(!(multiply_matrix(matrix_hess,matrix_R,matrix_Q,n,n,n,n,n,n)))
				return NO_RESULT;

			// 对角线元素平移
			for(int i=0;i<n;i++)
			{
				matrix_hess[i][i] += temp_u;
			}

			// 判断是否达到精度
			if(fabs(matrix_hess[n-1][n-2]) < fabs(precision * matrix_norm))
			{
				eigenvalues[n-1] = matrix_hess[n-1][n-1];
				break;
			}
		}

		if(k == iterations)
		{
			if(fabs(matrix_hess[n-1][n-2]) >= fabs(precision * matrix_norm))
			{
				return DIVERGENCY;
			}
		}

		n--;
	}

	eigenvalues[0] = matrix_hess[0][0];

	// 删除申请的空间
	for(int i=0;i<nDimension;i++)
	{
		delete [] matrix_hess[i];
		delete [] matrix_Q[i];
		delete [] matrix_R[i];
	}
	delete [] matrix_hess;
	delete [] matrix_Q;
	delete [] matrix_R;
	return 0;
	return 0;
}

ErrNo Matrix_Algorithm::Hess(double **matrix,double **matrix_hess,double **matrix_house,int nDimension)
{
	// 存储每步构造出的Householder矩阵
	double **matrix_temp = NULL;
	double *vector_m = NULL;
	double temp_a,temp_b;

	// 申请空间
	if(!(vector_m = new double [nDimension-1]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_temp = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_temp[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// Hessenberg矩阵赋初值
	for(int i=0;i<nDimension;i++)
	{
		for(int j=0;j<nDimension;j++)
		{
			matrix_hess[i][j] = matrix[i][j];
		}
	}

	// matrix_house储存单位矩阵
	create_identity_matrix(matrix_house,nDimension);

	// 计算 n-2 个Householder矩阵
	for(int k=0;k<nDimension-2;k++)
	{
		create_identity_matrix(matrix_temp,nDimension);
		// 构造向量v
		// 从矩阵获得列向量
		for(int i=0;i<nDimension-k-1;i++)
			vector_m[i] = matrix_hess[i+k+1][k];

		temp_a = sign(matrix_hess[k+1][k]);
		temp_b = temp_a * sqrt_vector(vector_m,nDimension-k-1);
		vector_m[0] += temp_b;

		// 这里是关键
		if(fabs(temp_b - matrix_hess[k+1][k]) > eps && fabs(temp_b) > eps)
		{
			//Householder变换
			temp_a = temp_b * vector_m[0];
			if(temp_a < eps)
				return DIVIDE_BY_ZERO;
			temp_a = 1/temp_a;
			for(int i=k+1;i<nDimension;i++)
			{
				for(int j=k+1;j<nDimension;j++)
				{
					matrix_temp[i][j] -= temp_a*vector_m[i-k-1]*vector_m[j-k-1];
				}
			}
		}

		//计算Householder矩阵
		if(!multiply_matrix_right(matrix_temp,matrix_house,nDimension,nDimension))
			return NO_RESULT;
		
		//计算Hessenberg矩阵
		if(!multiply_matrix_left(matrix_hess,matrix_temp,nDimension,nDimension))
			return NO_RESULT;
		if(!multiply_matrix_right(matrix_temp,matrix_hess,nDimension,nDimension))
			return NO_RESULT;
	}

	delete [] vector_m;
	for(int i=0;i<nDimension;i++)
		delete [] matrix_temp[i];
	delete [] matrix_temp;

	return 0;
}

ErrNo Matrix_Algorithm::QR_Decomposition(double **matrix_hess,double **matrix_Q,double **matrix_R,int nDimension)
{
	//  储存计算出的Givens变换矩阵
	double **matrix_temp = NULL;
	double temp,temp_cos,temp_sin;

	if(!(matrix_temp = new double* [nDimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nDimension;i++)
	{
		if(!(matrix_temp[i] = new double [nDimension]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// matrix_Q 存储单位矩阵
	create_identity_matrix(matrix_Q,nDimension);
	// matrix_R 存储Hessenberg矩阵
	for(int i=0;i<nDimension;i++)
	{
		for(int j=0;j<nDimension;j++)
		{
			matrix_R[i][j] = matrix_hess[i][j];
		}
	}

	for(int k=0;k<nDimension-1;k++)
	{
		// 每次计算前 matrix_temp 储存单位矩阵
		create_identity_matrix(matrix_temp,nDimension);

		// 计算r值
		if(fabs(matrix_R[k][k]) < eps && fabs(matrix_R[k+1][k]) < eps)
			;
		else
		{
			temp = sqrt(matrix_R[k][k] * matrix_R[k][k] + matrix_R[k+1][k] * matrix_R[k+1][k]);
			// 计算Givens矩阵中的sin cos值
			temp_cos = matrix_R[k][k] / temp;
			temp_sin = matrix_R[k+1][k] / temp;

			// 构造Givens矩阵
			matrix_temp[k][k] = temp_cos;
			matrix_temp[k][k+1] = temp_sin;
			matrix_temp[k+1][k] = -1*temp_sin;
			matrix_temp[k+1][k+1] = temp_cos;
		}

		// 对Hessenberg矩阵进行Givens变换
		if(!multiply_matrix_right(matrix_temp,matrix_R,nDimension,nDimension))
			return NO_RESULT;


		// 将矩阵matrix_temp转置
		temp = matrix_temp[k][k+1];
		matrix_temp[k][k+1] = matrix_temp[k+1][k];
		matrix_temp[k+1][k] = temp;

		// 乘矩阵Q(matrix_Q)
		if(!multiply_matrix_left(matrix_Q,matrix_temp,nDimension,nDimension))
			return NO_RESULT;
	}


	// 删除临时申请的空间
	for(int i=0;i<nDimension;i++)
		delete [] matrix_temp[i];
	delete [] matrix_temp;

	return 0;

}

ErrNo Matrix_Algorithm::matrix_eigenvectors(double **matrix,double **eigenvectors,double *eigenvalues,const int dimension)
{
	// matrix_t:保存构造的矩阵( (M-tI)x = 0 )
	double **matrix_t = NULL;

	// 申请空间
	if(!(matrix_t = new double* [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		if(!(matrix_t[i] = new double [dimension]))
			return ALLOCATE_MEMORY_FAIL;
	}



	// 销毁空间
	for(int i=0;i<dimension;i++)
		delete [] matrix_t[i];
	delete [] matrix_t;
	return 0;
}

ErrNo Matrix_Algorithm::InvPowerMeth(double **matrix,double *eigenvector,const double eigenvalue,const int dimension,const double precision /* = 1e-6 */,const int iteration /* = 50 */)
{
	int m;
	ErrNo err;
	double temp;
	double c_before,c_later;
	int index = 0;
	double **matrix_t = NULL;
	double *vector_x = NULL;
	double *vector_y = NULL; 

	// 申请空间
	if(!(vector_x = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(vector_y = new double [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	if(!(matrix_t = new double* [dimension]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<dimension;i++)
	{
		// 为了构造增广矩阵
		if(!(matrix_t[i] = new double [dimension+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 初始化vector_x
	for(int i=0;i<dimension;i++)
		vector_x[i] = 1.0;
	// 初始化matrix_t
	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension;j++)
		{
			if(i==j)
				matrix_t[i][j] = matrix[i][j] - eigenvalue;
			else
				matrix_t[i][j] = matrix[i][j];
		}
	}
	// 初始化c值
	c_before = c_later = 1.0;

	// 进行迭代
	for(m=0;m<iteration;m++)
	{
		// 构造增广矩阵
		for(int i=0;i<dimension;i++)
		{
			matrix_t[i][dimension] = vector_x[i];
		}

		// LU分解法计算方程组
		if(err = LU_NOC(matrix_t,vector_y,dimension))
			return err;

		index = max_vector_index(vector_y,dimension);
		c_later = temp = vector_y[index];
		if(fabs(temp) < eps)
			return DIVIDE_BY_ZERO;

		for(int i=0;i<dimension;i++)
			vector_x[i] = vector_y[i] / temp;
		if(fabs(c_later - c_before) < eps)
			break;
		c_before = c_later;
	}
	
	for(int i=0;i<dimension;i++)
	{
		eigenvector[i] = vector_x[i];
	}

	// 标准化
	temp = dot_product(eigenvector,dimension);
	if(fabs(temp) < eps)
		return DIVIDE_BY_ZERO;
	if(temp < 0)
		return NEGATIVE_NUMBER;
	temp = sqrt(temp);
	for(int i=0;i<dimension;i++)
	{
		eigenvector[i] = eigenvector[i] / temp;
	}



	// 删除空间
	delete [] vector_x;
	delete [] vector_y;
	for(int i=0;i<dimension;i++)
		delete [] matrix_t[i];
	delete [] matrix_t;
	return 0;
}

double Matrix_Algorithm::sign(double x)
{
	if(x >= 0.0)
		return 1.0;
	else
		return -1.0;
}

double Matrix_Algorithm::sqrt_vector(const double *vector_m,const int n)
{
	double sum = 0.0;

	for(int i=0;i<n;i++)
	{
		sum += vector_m[i] * vector_m[i];
	}
	
	return sqrt(sum);
}

double Matrix_Algorithm::dot_product(const double *vector_m,const int n)
{
	double sum = 0.0;

	for(int i=0;i<n;i++)
	{
		sum += vector_m[i] * vector_m[i];
	}

	return sum;
}

double Matrix_Algorithm::max_vector(double *vector_t,const int n)
{
	double max;

	max = fabs(vector_t[0]);

	for(int i=0;i<n;i++)
	{
		if(fabs(vector_t[i]) > max)
			max = fabs(vector_t[i]);
	}
	return max;
}

int Matrix_Algorithm::max_vector_index(double *vector_t,const int n)
{
	int index;

	index = 0;

	for(int i=0;i<n;i++)
	{
		if(fabs(vector_t[i]) > fabs(vector_t[index]))
			index = i;
	}
	return index;
}

void Matrix_Algorithm::create_identity_matrix(double **matrix,int nDimension)
{
	for(int i=0;i<nDimension;i++)
	{
		for(int j=0;j<nDimension;j++)
		{
			if(i == j)
				matrix[i][j] = 1.0;
			else
				matrix[i][j] = 0.0;
		}
	}
}

bool Matrix_Algorithm::multiply_matrix(double **pmatrix_a,double **pmatrix_b,double **pmatrix_c,int row_a,int col_a,int row_b,int col_b,int row_c,int col_c)
{
	double sum;

	if((row_a != row_b) || (col_a != col_c))
		return false;
	else if(col_b != row_c)
		return false;

	for(int i=0;i<row_a;i++)
	{
		for(int j=0;j<col_a;j++)
		{
			sum = 0.0;

			for(int k=0;k<col_b;k++)
			{
				sum += pmatrix_b[i][k] * pmatrix_c[k][j];
			}

			pmatrix_a[i][j] = sum;
		}
	}

	return true;

}

bool Matrix_Algorithm::multiply_matrix_left(double **pdest_matrix,double **psrc_matrix,int dim_dest,int dim_src)
{
	double sum;
	double *matrix_vector;

	if(dim_dest != dim_src)
		return false;

	// 判断是否为方阵
	if(!(matrix_vector = new double [dim_dest]))
		return false;

	for(int i=0;i<dim_dest;i++)
	{
		// 保存要相乘的那一行
		for(int k=0;k<dim_dest;k++)
			matrix_vector[k] = pdest_matrix[i][k];

		for(int j=0;j<dim_dest;j++)
		{
			sum = 0.0;

			// 矩阵乘法
			for(int k=0;k<dim_dest;k++)
			{
				sum += matrix_vector[k] * psrc_matrix[k][j];
			}

			pdest_matrix[i][j] = sum;
		}
	}

	delete [] matrix_vector;
	matrix_vector = NULL;
	return true;
}

bool Matrix_Algorithm::multiply_matrix_right(double **psrc_matrix,double **pdest_matrix,int dim_src,int dim_dest)
{
	double **pmatrix_temp = NULL;

	if(dim_dest != dim_src)
		return false;

	if(!(pmatrix_temp = new double* [dim_src]))
		return false;
	for(int i=0;i<dim_dest;i++)
	{
		if(!(pmatrix_temp[i] = new double [dim_src]))
			return false;
	}

	for(int i=0;i<dim_src;i++)
	{
		for(int j=0;j<dim_src;j++)
		{
			pmatrix_temp[i][j] = psrc_matrix[i][j];
		}
	}

	multiply_matrix_left(pmatrix_temp,pdest_matrix,dim_src,dim_dest);

	for(int i=0;i<dim_src;i++)
	{
		for(int j=0;j<dim_src;j++)
		{
			pdest_matrix[i][j] = pmatrix_temp[i][j];
		}
	}

	for(int i=0;i<dim_src;i++)
		delete [] pmatrix_temp[i];
	delete [] pmatrix_temp;
	return true;
}

double Matrix_Algorithm::matrix_norm_r(double **matrix,const int nDimension)
{
	double sum,max;

	sum = max = 0.0;

	for(int i=0;i<nDimension;i++)
	{
		sum = 0.0;
		for(int j=0;j<nDimension;j++)
		{
			sum += fabs(matrix[i][j]);
		}
		if(sum > max)
			max = sum;
	}
	return max;
}

void Matrix_Algorithm::QuickSort_STL(double *vector,int s,int t)
{
	int i=s,j=t;

	double temp;
	if(s<t)
	{
		temp = vector[s];
		while(i!=j)
		{
			while(j>i && vector[j] >= temp)
				j--;
			vector[i] = vector[j];
			while(i<j && vector[i] <= temp)
				i++;
			vector[j] = vector[i];
		}
		vector[i] = temp;
		QuickSort_STL(vector,s,i-1);
		QuickSort_STL(vector,i+1,t);
	}
}

void Matrix_Algorithm::QuickSort_LTS(double *vector,int s,int t)
{
	int i=s,j=t;

	double temp;
	if(s<t)
	{
		temp = vector[s];
		while(i!=j)
		{
			while(j>i && vector[j] <= temp)
				j--;
			vector[i] = vector[j];
			while(i<j && vector[i] >= temp)
				i++;
			vector[j] = vector[i];
		}
		vector[i] = temp;
		QuickSort_LTS(vector,s,i-1);
		QuickSort_LTS(vector,i+1,t);
	}
}

