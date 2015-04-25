/*
	积分计算方法
*/
#include "Computation.h"

Integral_Algorithm::Integral_Algorithm()
{
	max_times = 50;
	eps = 1e-6;
}

int Integral_Algorithm::pow_int(int x,int n)
{
	int		multiply = 1;
	for(int i=1;i<=n;i++)
	{
		multiply *= x;
	}

	if(multiply > INT_MAX)
		return INT_MAX;
	return multiply;
}

ErrNo Integral_Algorithm::Romberg(TCHAR *expression,const double precision,const double left_interval,const double right_interval,double &result,double &precision_a,int &times)
{
	//v_F：储存T(k-1）,v_N：储存T(k)
	TCHAR			postexp[MAXSIZE];
	vector<double>	v_F,v_N;
	ErrNo			err;
	int				k,multiply;
	double			sum,calc_result,temp;
	
	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp))
		return err;

	//计算T0(0)
	sum = 0.0;
	if(err = CompValue(postexp,left_interval,&calc_result))
		return err;
	sum += calc_result;
	if(err = CompValue(postexp,right_interval,&calc_result))
		return err;
	sum = (sum + calc_result)/2;
	v_F.push_back(sum);

	k = 1;
	while(1)
	{
		sum = 0.0;
		multiply = 1;

		multiply = pow_int(2,k-1);
		//计算T0(k)
		for(int i=1;i<=multiply;i++)
		{
			temp = left_interval + (2*i-1)*(right_interval-left_interval) / (2*multiply);
			if(err = CompValue(postexp,temp,&calc_result))
				return err;
			sum += calc_result;
		}
		sum = v_F[0]/2 + sum * (right_interval - left_interval) / (2*multiply);
		v_N.push_back(sum);

		//计算Tm(k)
		for(int m=1;m<=k;m++)
		{
			multiply = pow_int(4,m);
			sum = multiply * v_N[m-1] / (multiply-1) - v_F[m-1] / (multiply-1);
			v_N.push_back(sum);
		}
		
		if(fabs(v_N[v_N.size()-1] - v_F[v_F.size()-1]) < precision)
		{
			result = v_N[v_N.size()-1];
			break;
		}
		else if(k >= max_times)
		{
			result = v_N[v_N.size()-1];
			precision_a = fabs(v_N[v_N.size()-1] - v_F[v_F.size()-1]);
			times = k;
		}

		v_F = v_N;
		v_N.clear();
		k++;
	}
	return 0;
}

ErrNo Integral_Algorithm::Gauss_Legendre(TCHAR *expression,double &result)
{
	double			result_t;
	ErrNo			err;
	TCHAR			postexp[MAXSIZE];

	//处理表达式
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp))
	{
		return err;
	}

	result = 0.0;

	if(err = CompValue(postexp,0.9061793,&result_t))
		return err;
	result += result_t*0.2369269;
	if(err = CompValue(postexp,-0.9061793,&result_t))
		return err;
	result += result_t*0.2369269;


	if(err = CompValue(postexp,0.5384693,&result_t))
		return err;
	result += result_t*0.4786287;
	if(err = CompValue(postexp,-0.5384693,&result_t))
		return err;
	result += result_t*0.4786287;

	if(err = CompValue(postexp,0.0000000,&result_t))
		return err;
	result += 0.5688889*result_t;

	return 0;
}

ErrNo Integral_Algorithm::Gauss_Chebyshev(TCHAR *expression,double &result)
{
	ErrNo		err;
	double		result_t;
	TCHAR		postexp[MAXSIZE];

	//处理表达式
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp))
		return err;

	result = 0.0;
	if(err = CompValue(postexp,0.5877853,&result_t))
		return err;
	result += PI*result_t/5;
	if(err = CompValue(postexp,-0.5877853,&result_t))
		return err;
	result += PI*result_t/5;

	if(err = CompValue(postexp,0.9510565,&result_t))
		return err;
	result += PI*result_t/5;
	if(err = CompValue(postexp,-0.9510565,&result_t))
		return err;
	result += PI*result_t/5;

	if(err = CompValue(postexp,0.0000000,&result_t))
		return err;
	result += PI*result_t/5;
	return 0;
}