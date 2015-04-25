/*
	拟合算法
*/
#include "Computation.h"

// 最小二乘拟合法
ErrNo Fitting_Algorithm::Least_Square_Fitting(PTSTR *polynomial,double *parameter,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps)
{
	Matrix_Algorithm matrix_algorithm;
	double comp_i,comp_k,comp_sum,det;
	ErrNo err;
	int	nSize;
	int	iLength;
	TCHAR expression_i[MAXSIZE],expression_k[MAXSIZE],postexp_i[MAXSIZE],postexp_k[MAXSIZE];
	double **Augmented_Matrix = NULL;

	//申请空间保存法方程
	if(! (Augmented_Matrix = new double* [nNumofPolynomial]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}

	nSize = nNumofPolynomial > MAXSIZE ? MAXSIZE : nNumofPolynomial;

	for(int i=0;i<nSize;i++)
	{
		//列数增加一维，增广矩阵
		if(! (Augmented_Matrix[i] = new double [nNumofPolynomial+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	// 计算法方程的系数矩阵
	for(int i=0;i<nSize;i++)
	{
		for(int j=0;j<nSize;j++)
		{
			iLength = _tcslen(polynomial[i]);
			for(int k=0;k<=iLength;k++)
			{
				expression_i[k] = polynomial[i][k];
			}
			iLength = _tcslen(polynomial[j]);
			for(int k=0;k<=iLength;k++)
			{
				expression_k[k] = polynomial[j][k];
			}
			//表达式处理
			Deal_String(expression_i);
			Deal_String(expression_k);
			if(err = Trans_Exp(expression_i,postexp_i))
				return err;
			if(err = Trans_Exp(expression_k,postexp_k))
				return err;

			comp_sum = 0.0f;
			for(int k=0;k<nNumofPoints;k++)
			{
				if(err = CompValue(postexp_i,dPoints_F[k].x,&comp_i))
					return err;
				if(err = CompValue(postexp_k,dPoints_F[k].x,&comp_k))
					return err;
				comp_sum += comp_i * comp_k;
				if(comp_sum >= DBL_MAX)
					return DATA_OVERFLOW;
			}

			Augmented_Matrix[i][j] = comp_sum;
		}
	}

	//计算的法方程的常数项
	for(int i=0;i<nSize;i++)
	{
		iLength = _tcslen(polynomial[i]);
		for(int j=0;j<=iLength;j++)
			expression_i[j] = polynomial[i][j];

		//表达式处理
		Deal_String(expression_i);
		if(err = Trans_Exp(expression_i,postexp_i))
			return err;

		comp_sum = 0.0f;
		for(int k=0;k<nNumofPoints;k++)
		{
			if(err = CompValue(postexp_i,dPoints_F[k].x,&comp_i))
				return err;
			comp_sum += comp_i * dPoints_F[k].fx;
			if(comp_sum >= DBL_MAX)
				return DATA_OVERFLOW;
		}

		Augmented_Matrix[i][nSize] = comp_sum;

	}


	//计算曲线系数
	if(err = matrix_algorithm.Gauss(Augmented_Matrix,parameter,det,nSize))
		return err;
	
	//销毁空间
	for(int i=0;i<nSize;i++)
		delete [] Augmented_Matrix[i];
	delete [] Augmented_Matrix;

	return 0;
}

// 基于正交多项式拟合
ErrNo Fitting_Algorithm::Polynominal_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const int degree,const double eps)
{
	double sum_a,sum_b,temp;

	// 计算曲线表达式系数
	for(int i=0;i<=degree;i++)
	{
		temp = sum_a = sum_b =  0.0;
		for(int j=0;j<nNumofPoints;j++)
		{
			temp = Polynominal_Recursion(dPoints_F,nNumofPoints,i,dPoints_F[j].x,eps);
			if(fabs(temp) == DBL_MAX)
				return DATA_OVERFLOW;
			sum_a += temp * temp;
			sum_b += temp * dPoints_F[j].fx;
		}
		if(sum_a < eps)
			return DIVIDE_BY_ZERO;
		parameter[i] = sum_b / sum_a;
	}

	return 0;
}

// 指数模型的最小二乘拟合
ErrNo Fitting_Algorithm::Exponential_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const double eps)
{
	PTSTR *polynomial;
	DPOINT *dPoints;
	ErrNo err;

	if(!(polynomial = new TCHAR* [2]))
		return ALLOCATE_MEMORY_FAIL;

	if(!(dPoints = new DPOINT [nNumofPoints]))
		return ALLOCATE_MEMORY_FAIL;

	for(int i=0;i<nNumofPoints;i++)
	{
		dPoints[i].x = dPoints_F[i].x;
		if(dPoints_F[i].fx < 0)
			return NEGATIVE_NUMBER;
		dPoints[i].fx = log(dPoints_F[i].fx);
	}

	// 构造多项式
	polynomial[0] = TEXT("1");
	polynomial[1] = TEXT("x");

	// 使用最小二乘法
	if(err = Least_Square_Fitting(polynomial,parameter,2,nNumofPoints,dPoints,eps))
		return err;

	parameter[0] = exp(parameter[0]);

	delete [] dPoints;
	delete [] polynomial;

	return 0;
}


// 计算正交多项式在某点x的值 k:第k个多项式 k=0,1,2,...
double Fitting_Algorithm::Polynominal_Recursion(const DPOINT *dPoints_F,const int n,const int k,const double x,const double eps)
{
	double calc;
	double sum_a,sum_b,sum_c,temp_a,temp_b;

	// k要求为正整数
	if(k < 0)
		return INF;

	if(k == 0)
	{
		return(1.0);
	}
	else if(k == 1)
	{
		calc = 0.0;
		sum_a = 0.0;
		for(int i=0;i<n;i++)
			sum_a += dPoints_F[i].x;
		if(abs(n) < eps)
			return INF;
		sum_a = sum_a / n;
		calc = x - sum_a;
	}
	else
	{
		sum_a = sum_b = sum_c = 0.0;

		for(int i=0;i<n;i++)
		{
			temp_a = Polynominal_Recursion(dPoints_F,n,k-1,dPoints_F[i].x,eps);
			temp_b = Polynominal_Recursion(dPoints_F,n,k-2,dPoints_F[i].x,eps);

			sum_a += temp_a * temp_a;
			sum_b += temp_b * temp_b;
			sum_c += dPoints_F[i].x * temp_a * temp_a;
		}

		temp_a = Polynominal_Recursion(dPoints_F,n,k-1,x,eps);
		temp_b = Polynominal_Recursion(dPoints_F,n,k-2,x,eps);

		if(fabs(sum_a) < eps || fabs(sum_b) < eps)
			return INF;

		calc = (x - sum_c/sum_a) * temp_a - sum_a*temp_b/sum_b;
	}

	return calc;
}