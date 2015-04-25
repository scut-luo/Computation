/*
	常微分方程的数值解法
*/
#include "Computation.h"

ODE_Algorithm::ODE_Algorithm()
{
	eps = 1e-6;
}

ErrNo ODE_Algorithm::Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	int		k;
	double  x,y,result;
	double  values[2];
	TCHAR	postexp[MAXSIZE];
	ErrNo	err;


	results[0][0] = values[0] = left_interval;
	results[1][0] = values[1] = initial_value;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	k = 1;
	x = left_interval;
	while(1)
	{
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result))
			return err;
		y = values[1] + step_size * result;
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		values[0] = x;
		values[1] = y;

		results[0][k] = x;
		results[1][k] = y;
		k++;
	}

	return 0;
}

ErrNo ODE_Algorithm::Improved_Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	int		k;
	double  x,y,result,result_s,y_temp;
	double  values[2];
	TCHAR	postexp[MAXSIZE];
	ErrNo	err;


	results[0][0] = values[0] = left_interval;
	results[1][0] = values[1] = initial_value;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	k = 1;
	x = left_interval;
	while(1)
	{
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result))
			return err;
		y = values[1] + step_size * result;
		y_temp = values[1];
		x += step_size;

		if(x > right_interval && fabs(x-right_interval)>eps)
			break;

		values[0] = x;
		values[1] = y;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_s))
			return err;
		y = y_temp + step_size /2 * (result + result_s) ;
		values[0] = x;
		values[1] = y;

		results[0][k] = x;
		results[1][k] = y;
		k++;
	}

	return 0;
}

ErrNo ODE_Algorithm::Runge_Kutta(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	int		k;
	double  x,y,x_temp,y_temp,result_i,result_j,result_k,result_l;
	double  values[2];
	TCHAR	postexp[MAXSIZE];
	ErrNo	err;


	results[0][0] = values[0] = left_interval;
	results[1][0] = values[1] = initial_value;
	

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	k = 1;
	x = left_interval;
	while(1)
	{
		x_temp = values[0];
		y_temp = values[1];

		//计算K1
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_i))
			return err;
		
		//计算K2
		values[0] = x_temp + step_size /2;
		values[1] = y_temp + result_i * step_size /2;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算K3
		values[0] = x_temp + step_size /2;
		values[1] = y_temp + result_j * step_size /2;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算K4
		values[0] = x_temp + step_size;
		values[1] = y_temp + result_k * step_size;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		//计算y
		y = y_temp + (result_i+ result_j*2 + result_k*2 + result_l) * step_size / 6;
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		values[0] = x;
		values[1] = y;

		results[0][k] = x;
		results[1][k] = y;
		k++;
	}

	return 0;
}

ErrNo ODE_Algorithm::Simple_Adams_PC(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	TCHAR	exp_temp[MAXSIZE],postexp[MAXSIZE];
	ErrNo	err;
	double	x,y,result_i,result_j,result_k,result_l,result_m;
	double  values[2];
	int		num_x,k;

	for(int i=0;i<=lstrlen(expression);i++)
		exp_temp[i] = expression[i];

	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(num_x < 3)
		return SMALL_INTERVAL;

	//获取y0,y1,y2,y3
	if(err = Runge_Kutta(exp_temp,results,initial_value,step_size,left_interval,left_interval+3*step_size,variable_symbol_array))
		return err;

	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	x = left_interval + 3*step_size;
	k = 3;
	while(1)
	{	
		//计算f(n-3)
		values[0] = results[0][k-3];
		values[1] = results[1][k-3];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_i))
			return err;

		//计算f(n-2)
		values[0] = results[0][k-2];
		values[1] = results[1][k-2];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算f(n-1)
		values[0] = results[0][k-1];
		values[1] = results[1][k-1];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算f(n)
		values[0] = results[0][k];
		values[1] = results[1][k];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		y = results[1][k] + (55*result_l - 59*result_k + 37*result_j - 9*result_i)*step_size/24;
		
		//计算y(n+1)
		x += step_size;
		values[0] = x;
		values[1] = y;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_m))
			return err;
		y = results[1][k] + (9*result_m + 19*result_l -5*result_k + result_j)*step_size/24;
		k++;
		results[0][k] = x;
		results[1][k] = y;
	}
	return 0;
}

ErrNo ODE_Algorithm::Milne_Hamming(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	TCHAR	exp_temp[MAXSIZE],postexp[MAXSIZE];
	ErrNo	err;
	double	x,y,y_j,y_k,y_l,y_m;
	double	result_j,result_k,result_l,result_m;
	double  values[2];
	int		num_x,k;

	for(int i=0;i<=lstrlen(expression);i++)
		exp_temp[i] = expression[i];

	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(num_x < 4)
		return SMALL_INTERVAL;

	//获取y0,y1,y2,y3,y4（使用Milne4步4阶显式公式）
	if(err = Milne(exp_temp,results,initial_value,step_size,left_interval,left_interval+4*step_size,variable_symbol_array))
		return err;


	//表达式处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	//n从4开始
	x = left_interval + 4*step_size;
	k = 4;
	while(1)
	{	
		//计算f(n-2)
		values[0] = results[0][k-3];
		values[1] = results[1][k-3];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算f(n-1)
		values[0] = results[0][k-2];
		values[1] = results[1][k-2];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算f(n)
		values[0] = results[0][k-1];
		values[1] = results[1][k-1];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		y_j = results[1][k-4] + (2*result_l - result_k + 2*result_j)*4*step_size/3;

		//////////第一步 预测
		//计算f(n-2)
		values[0] = results[0][k-2];
		values[1] = results[1][k-2];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算f(n-1)
		values[0] = results[0][k-1];
		values[1] = results[1][k-1];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算f(n)
		values[0] = results[0][k];
		values[1] = results[1][k];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		y_k = results[1][k-3] + (2*result_l - result_k + 2*result_j)*4*step_size/3;

		//////////第二步 修正
		y_l = y_k + (results[1][k] - y_j)*112/121;

		//////////第三步 校正
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;

		values[0] = x;
		values[1] = y_l;
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_m))
			return err;
		y_m = (9*results[1][k] - results[1][k-2])*1/8 + (result_m + 2*result_l - result_k)*3*step_size/8;

		/////////第四步 修正
		y = y_m - (y_m - y_k)*9/121;

		k++;
		results[0][k] = x;
		results[1][k] = y;
	}
	return 0;
}

ErrNo ODE_Algorithm::Adams(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	TCHAR	exp_temp[MAXSIZE],postexp[MAXSIZE];
	ErrNo	err;
	double	values[2];
	double	x,y,result_i,result_j,result_k,result_l;
	int		num_x,k;

	for(int i=0;i<=lstrlen(expression);i++)
		exp_temp[i] = expression[i];

	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(num_x < 3)
		return SMALL_INTERVAL;

	//获取y0,y1,y2,y3 使用4阶Runge-Kutta方法
	if(err = Runge_Kutta(exp_temp,results,initial_value,step_size,left_interval,left_interval+3*step_size,variable_symbol_array))
		return err;

	//表达处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	x = left_interval + 3*step_size;
	k = 3;
	while(1)
	{
		//计算f(n-3)
		values[0] = results[0][k-3];
		values[1] = results[1][k-3];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_i))
			return err;

		//计算f(n-2)
		values[0] = results[0][k-2];
		values[1] = results[1][k-2];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算f(n-1)
		values[0] = results[0][k-1];
		values[1] = results[1][k-1];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算f(n)
		values[0] = results[0][k];
		values[1] = results[1][k];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		y = results[1][k] + (55*result_l - 59*result_k + 37*result_j - 9*result_i)*step_size/24;
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		k++;
		results[0][k] = x;
		results[1][k] = y;
	}
	return 0;
}

ErrNo ODE_Algorithm::Milne(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array)
{
	TCHAR	exp_temp[MAXSIZE],postexp[MAXSIZE];
	ErrNo	err;
	double	values[2];
	double	x,y,result_j,result_k,result_l;
	int		num_x,k;

	for(int i=0;i<=lstrlen(expression);i++)
		exp_temp[i] = expression[i];

	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(num_x < 3)
		return SMALL_INTERVAL;

	//获取y0,y1,y2 y3使用4阶Runge-Kutta方法
	if(err = Runge_Kutta(exp_temp,results,initial_value,step_size,left_interval,left_interval+3*step_size,variable_symbol_array))
		return err;

	//表达处理
	Deal_String(expression);
	if(err = Trans_Exp(expression,postexp,variable_symbol_array,2))
		return err;

	x = left_interval + 3*step_size;
	k = 3;
	while(1)
	{
		//计算f(n-2)
		values[0] = results[0][k-2];
		values[1] = results[1][k-2];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_j))
			return err;

		//计算f(n-1)
		values[0] = results[0][k-1];
		values[1] = results[1][k-1];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_k))
			return err;

		//计算f(n)
		values[0] = results[0][k];
		values[1] = results[1][k];
		if(err = CompValue(postexp,variable_symbol_array,2,values,&result_l))
			return err;

		y = results[1][k-3] + (2*result_l - result_k + 2*result_j)*step_size*4/3;
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		k++;
		results[0][k] = x;
		results[1][k] = y;
	}
	return 0;
	return 0;
}

