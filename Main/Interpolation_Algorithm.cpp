/*
	插值算法
*/
#include "Computation.h"

Interpolation_Algorithm::Interpolation_Algorithm()
{
	eps = 1e-6;
}

ErrNo Interpolation_Algorithm::Lagrange(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	double		whole,multiply = 0.0;

	result = 0.0;

	for(int i=0;i<=degree;i++)
	{
		whole = 1.0;
		for(int j=0;j<=degree;j++)
		{
			if(j!=i)
			{
				if( fabs(dpoints[i].x - dpoints[j].x) < eps)
					return DIVIDE_BY_ZERO;
				whole *= (compvalue - dpoints[j].x) / (dpoints[i].x - dpoints[j].x) ;
			}
		}
		result += whole * dpoints[i].fx;
	}
	return 0;
}

ErrNo Interpolation_Algorithm::Newton(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	double		**Matrix = NULL;

	result = 0.0;
	//申请空间存储均差
	if(!(Matrix = new double* [degree+1]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}
	for(int i=0;i<=degree;i++)
	{
		if(!(Matrix[i] = new double [degree+1]))
			return ALLOCATE_MEMORY_FAIL;
	}

	//矩阵第一列存储f(x)的值
	for(int i=0;i<=degree;i++)
	{
		Matrix[i][0] = dpoints[i].fx;
	}

	//计算均差 下三角矩阵
	for(int j=1;j<=degree;j++)
	{
		for(int i=j;i<=degree;i++)
		{
			if( fabs(dpoints[i-j].x - dpoints[i].x) < eps)
				return DIVIDE_BY_ZERO;
			Matrix[i][j] = (Matrix[i-1][j-1] - Matrix[i][j-1]) / (dpoints[i-j].x - dpoints[i].x);
		}
	}

	//Newton插值
	result = Matrix[degree][degree];
	for(int i=degree-1;i>=0;i--)
	{
		result = Matrix[i][i] + (compvalue - dpoints[i].x)*result;
	}

	return 0;
}

ErrNo Interpolation_Algorithm::Hermite(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	double		differ = 0.0;
	double		whole = 0.0,whole_l = 0.0;
	result = 0.0;

	for(int i=0;i<=degree;i++)    //n+1个插值点
	{
		//求解l(x)
		//求解 1/(x(i)-x(j))
		whole = 1.0;
		whole_l = 0.0;
		for(int j=0;j<=degree;j++)
		{
			if(j!=i)
			{
				differ = dpoints[i].x - dpoints[j].x ;
				if(fabs(differ) < eps)
					return DIVIDE_BY_ZERO;
				whole *= (compvalue - dpoints[j].x) / differ;
				whole_l += 1 / differ;

			}
		}

		result += (1-2*(compvalue-dpoints[i].x)*whole_l)*whole*whole*dpoints[i].fx + (compvalue-dpoints[i].x)*whole*whole*dpoints[i].df;	
	}

	return 0;
}

ErrNo Interpolation_Algorithm::Piecewise_Linear_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	double		differ = 0.0;

	result = 0.0;

	//由所求点求f(x)值
	if(compvalue < dpoints[0].x || compvalue > dpoints[degree].x)				//超出所求区间
		return NOT_IN_INTERVAL;

	for(int i=0;i<degree;i++)
	{
		if(compvalue == dpoints[i].x)
		{
			result = dpoints[i].fx;
			break;
		}
		else if(compvalue > dpoints[i].x && compvalue <= dpoints[i+1].x)
		{
			differ = dpoints[i].x - dpoints[i+1].x ;
			if(fabs(differ) < eps)
				return DIVIDE_BY_ZERO;
			result = ((compvalue - dpoints[i+1].x) / differ) * dpoints[i].fx - ((compvalue - dpoints[i].x) / differ) * dpoints[i+1].fx;
			break;
		}
	}

	return 0;
}

ErrNo Interpolation_Algorithm::Piecewise_Hermite_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	double		Mutl_1,Mutl_2;
	double		differ = 0.0;

	result = 0.0;

	//由所求点求f(x)值
	if(compvalue < dpoints[0].x || compvalue > dpoints[degree].x)				//超出所求区间
		return NOT_IN_INTERVAL;

	for(int i=0;i<degree;i++)
	{
		if(compvalue == dpoints[i].x)
		{
			result = dpoints[i].fx;
			break;
		}
		else if(compvalue > dpoints[i].x && compvalue <= dpoints[i+1].x)
		{
			differ = dpoints[i].x - dpoints[i+1].x ;
			if(fabs(differ) < eps)
				return DIVIDE_BY_ZERO;
			Mutl_1 = (compvalue - dpoints[i+1].x) / differ;
			Mutl_2 = -1*(compvalue - dpoints[i].x) / differ;
			result = ((1+2*Mutl_2)* dpoints[i].fx + (compvalue - dpoints[i].x)*dpoints[i].df)*Mutl_1*Mutl_1 + \
				((1+2*Mutl_1)* dpoints[i+1].fx + (compvalue - dpoints[i+1].x)*dpoints[i+1].df)*Mutl_2*Mutl_2;
			break;
		}
	}
	return 0;
}

ErrNo Interpolation_Algorithm::Piecewise_Spline_Interpolation_DF(const DPOINT *dpoints,const double compvalue,const int degree,double &result)
{
	ErrNo				err;
	double				differ = 0.0 ,differ2 = 0.0;
	double				**Tridiagonal_Matrix = NULL,*step_vector = NULL,*para_vector = NULL;

	//申请矩阵空间
	if(!(Tridiagonal_Matrix = new double* [4]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}
	for(int i=0;i<4;i++)
	{
		Tridiagonal_Matrix[i] = new double[degree+1];
		if(!Tridiagonal_Matrix)
		{
			return ALLOCATE_MEMORY_FAIL;
		}
	}

	//申请保存每个区间长度的大小
	if(!(step_vector = new double [degree]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}

	//申请空间保存系数
	if(!(para_vector = new double [degree+1]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}

	//计算每个区间的大小
	for(int i=0;i<=degree-1;i++)
	{
		step_vector[i] = dpoints[i+1].x - dpoints[i].x;
		if(fabs(step_vector[i]) < eps)
			return DIVIDE_BY_ZERO;
	}

	//三对角矩阵的数据
	for(int i=0;i<=degree;i++)
	{
		Tridiagonal_Matrix[1][i] = 2.0;
	}
	Tridiagonal_Matrix[0][degree-1] = 1.0;
	Tridiagonal_Matrix[2][0] = 1.0;

	differ = dpoints[1].x - dpoints[0].x;
	if(fabs(differ) < eps)
		return DIVIDE_BY_ZERO;
	Tridiagonal_Matrix[3][0] = 6*(dpoints[1].fx - dpoints[0].fx - dpoints[0].df) / differ; 

	differ = dpoints[degree].x - dpoints[degree-1].x;
	if(fabs(differ) < eps)
		return	DIVIDE_BY_ZERO;
	Tridiagonal_Matrix[3][degree] = 6*(dpoints[degree].df - (dpoints[degree].fx - dpoints[degree-1].fx)) / differ; 
	
	for(int i=0;i<=degree-2;i++)
	{
		Tridiagonal_Matrix[0][i] = step_vector[i] / (step_vector[i] + step_vector[i+1]);
	}
	for(int i=1;i<=degree-1;i++)
	{
		Tridiagonal_Matrix[2][i] = step_vector[i] / (step_vector[i-1] + step_vector[i]);
	}
	for(int i=1;i<=degree-1;i++)
	{
		Tridiagonal_Matrix[3][i] = 6*( (dpoints[i+1].fx - dpoints[i].fx) / step_vector[i] - (dpoints[i].fx - dpoints[i-1].fx) / step_vector[i-1] ) / (step_vector[i-1] + step_vector[i]);
	}

	if( err = Thomas(Tridiagonal_Matrix,para_vector,degree+1))
		return err;

	//由所求点求f(x)值
	if(compvalue < dpoints[0].x || compvalue > dpoints[degree].x)				//超出所求区间
		return NOT_IN_INTERVAL;
	for(int i=0;i<degree;i++)
	{
		if(compvalue == dpoints[i].x)
		{
			result = dpoints[i].fx;
			break;
		}
		else if (compvalue > dpoints[i].x && compvalue <= dpoints[i+1].x)
		{
			differ = dpoints[i+1].x - compvalue;
			differ2 = compvalue - dpoints[i].x;
			result = para_vector[i]*differ*differ*differ/(6*step_vector[i]) + para_vector[i+1]*differ2*differ2*differ2/(6*step_vector[i]) + \
				(dpoints[i].fx - para_vector[i]*step_vector[i]*step_vector[i]/6)*differ/step_vector[i] + (dpoints[i+1].fx - para_vector[i+1]*step_vector[i]*step_vector[i]/6)*differ2/step_vector[i] ;
			break;
		}
	}

	//销毁指针
	for(int i=0;i<4;i++)
		delete [] Tridiagonal_Matrix[i];
	delete [] Tridiagonal_Matrix;
	Tridiagonal_Matrix = NULL;
	delete [] step_vector;
	step_vector = NULL;
	return 0;
}
