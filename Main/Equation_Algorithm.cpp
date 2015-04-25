/*
	近似计算方程根算法
*/

#include "Computation.h"

Equation_ALgorithm::Equation_ALgorithm(TCHAR variable_symbol)
{
	ChangeVariableSymbol(variable_symbol);
	eps = 1e-15;				//提高计算精度
}

ErrNo Equation_ALgorithm::Fixed_Point_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value)
{
	TCHAR	postexpression[MAXSIZE];
	int		i = 0;
	ErrNo	err;
	double	x_next,x_pre;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexpression))
	{
		return err;
	}

	x_pre = initial_value;
	for(i=1;i<=max_iterations;i++)
	{
		if(err = CompValue(postexpression,x_pre,&x_next))
			return err;
		if(fabs(x_next) > DBL_MAX)
			return DATA_OVERFLOW;
		if(fabs(x_next - x_pre) < precision)
		{
			max_iterations = i;
			result = x_next;
			return 0;
		}

		if(i == max_iterations)   //若迭代次数达到最大时,精度判断
		{
			if(fabs(x_next - x_pre) >= precision)
				precision = fabs(x_next - x_pre);
		}
		x_pre = x_next;
	}

	result = x_pre;
	return 0;
}

ErrNo Equation_ALgorithm::Newton_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value)
{
	TCHAR	postexpression[MAXSIZE];
	int		i = 0;
	ErrNo	err;
	double	x_next,x_pre;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexpression))
	{
		return err;
	}

	x_pre = initial_value;
	for(i=1;i<=max_iterations;i++)
	{
		if(err = CompValue(postexpression,x_pre,&x_next))
			return err;
		if(fabs(x_next - x_pre) < precision)
		{
			max_iterations = i;
			result = x_next;
			return 0;
		}

		if(i == max_iterations)
		{
			if(fabs(x_next - x_pre) >= precision)
				precision = fabs(x_next - x_pre);
		}
		x_pre = x_next;
	}
	result = x_pre;
	return 0;
}

ErrNo Equation_ALgorithm::Aitken(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value)
{
	TCHAR	postexpression[MAXSIZE];
	ErrNo	err;
	double	x_pre,x_mid,x_next,sum;

	x_mid = x_next = x_pre = initial_value;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexpression))
	{
		return err;
	}

	for(int k=1;k<=max_iterations;k++)
	{
		if(err = CompValue(postexpression,x_pre,&x_mid))				//计算X(k+1)
			return err;
		if(fabs(x_mid) > DBL_MAX)
			return DATA_OVERFLOW;

		if(err = CompValue(postexpression,x_mid,&x_next))			//计算X(k+2)
			return err;
		if(fabs(x_next) > DBL_MAX)
			return DATA_OVERFLOW;

		sum = x_next-2*x_mid+x_pre;
		if(fabs(sum) < eps)
			return DIVIDE_BY_ZERO;

		x_next = x_next-(x_next-x_mid)*(x_next-x_mid)/sum;

		if(fabs(x_next) > DBL_MAX)
			return DATA_OVERFLOW;

		if(fabs(x_next-x_pre) < precision)
		{
			max_iterations = k;
			result = x_next;
			return 0;
		}

		if(k == max_iterations)
		{
			if(fabs(x_next - x_pre) > precision)
			{
				precision = fabs(x_next - x_pre);
			}
		}
		x_pre = x_next;
	}

	result = x_next;
	return 0;
}

ErrNo Equation_ALgorithm::Steffensen(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value)
{
	TCHAR	postexpression[MAXSIZE];
	ErrNo	err;
	double	x_pre,x_mid,x_next,sum;

	x_pre = x_mid =x_pre = initial_value;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexpression))
	{
		return err;
	}

	for(int k=1;k<=max_iterations;k++)
	{
		if(err = CompValue(postexpression,x_pre,&x_mid))				//计算X(k+1)
			return err;
		if(fabs(x_mid) > DBL_MAX)
			return DATA_OVERFLOW;

		if(err = CompValue(postexpression,x_mid,&x_next))			//计算X(k+2)
			return err;
		if(fabs(x_next) > DBL_MAX)
			return DATA_OVERFLOW;

		sum = x_next-2*x_mid+x_pre;
		if(fabs(sum) < eps)
			return DIVIDE_BY_ZERO;

		x_next = x_pre-(x_mid-x_pre)*(x_mid-x_pre)/sum;

		if(fabs(x_next) > DBL_MAX)
			return DATA_OVERFLOW;

		if(fabs(x_next-x_pre) < precision)
		{
			max_iterations = k;
			result = x_next;
			return 0;
		}

		if(k == max_iterations)
		{
			if(fabs(x_next-x_pre) < precision)
			{
				precision = fabs(x_next - x_pre);
			}
		}
		x_pre = x_next;
	}

	result = x_next;
	return 0;
}