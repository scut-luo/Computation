/*
	΢�ּ��㷽��
*/
#include "Computation.h"


Differential_Algorithm::Differential_Algorithm()
{
	eps = 1e-6;
}

//����Taylor��ʽ����ֵ΢�ֹ�ʽ ��ǰ���
ErrNo Differential_Algorithm::Forward_Difference(double &result,const TCHAR* expression,const double x,const double interval)
{
	double temp_a,temp_b;
	ErrNo err;
	int nLength;
	TCHAR szBuffer[MAXSIZE],postexp[MAXSIZE];			// �����ַ���

	nLength = _tcslen(expression);
	nLength++;

	for(int i=0;i<nLength;i++)
		szBuffer[i] = expression[i];
	
	//���ʽ����
	Deal_String(szBuffer);
	if(err = Trans_Exp(szBuffer,postexp))  // ����ȫ postexp�����С�޶������ֵ
		return err;

	// ����f(x+h)
	if(err = CompValue(postexp,(x+interval),&temp_a))
		return err;
	// ����f(x)
	if(err = CompValue(postexp,x,&temp_b))
		return err;
	if(fabs(interval) < eps)
		return DIVIDE_BY_ZERO;

	result = (temp_a - temp_b) / interval;

	return 0;
}

//����Taylor��ʽ����ֵ΢�ֹ�ʽ �����
ErrNo Differential_Algorithm::Backward_Difference(double &result,const TCHAR* expression,const double x,const double interval)
{
	double temp_a,temp_b;
	ErrNo err;
	int nLength;
	TCHAR szBuffer[MAXSIZE],postexp[MAXSIZE];			// �����ַ���

	nLength = _tcslen(expression);
	nLength++;

	for(int i=0;i<nLength;i++)
		szBuffer[i] = expression[i];

	//���ʽ����
	Deal_String(szBuffer);
	if(err = Trans_Exp(szBuffer,postexp))  // ����ȫ postexp�����С�޶������ֵ
		return err;

	// ����f(x-h)
	if(err = CompValue(postexp,(x-interval),&temp_a))
		return err;
	// ����f(x)
	if(err = CompValue(postexp,x,&temp_b))
		return err;
	if(fabs(interval) < eps)
		return DIVIDE_BY_ZERO;

	result = (temp_b - temp_a) / interval;

	return 0;
}

//����Taylor��ʽ����ֵ΢�ֹ�ʽ ���Ĳ��
ErrNo Differential_Algorithm::Central_Difference(double &result,const TCHAR* expression,const double x,const double interval)
{
	double temp_a,temp_b;
	ErrNo err;
	int nLength;
	TCHAR szBuffer[MAXSIZE],postexp[MAXSIZE];			// �����ַ���

	nLength = _tcslen(expression);
	nLength++;

	for(int i=0;i<nLength;i++)
		szBuffer[i] = expression[i];

	//���ʽ����
	Deal_String(szBuffer);
	if(err = Trans_Exp(szBuffer,postexp))  // ����ȫ postexp�����С�޶������ֵ
		return err;

	// ����f(x+h)
	if(err = CompValue(postexp,(x+interval),&temp_a))
		return err;
	// ����f(x-h)
	if(err = CompValue(postexp,(x-interval),&temp_b))
		return err;
	if(fabs(interval) < eps)
		return DIVIDE_BY_ZERO;

	result = (temp_a - temp_b) / (2*interval);

	return 0;
}

//����Taylor��ʽ����ֵ΢�ֹ�ʽ �������Ĳ��
ErrNo Differential_Algorithm::Second_Order_Central_Difference(double &result,const TCHAR* expression,const double x,const double interval)
{
	double temp_a,temp_b,temp_c,temp;
	ErrNo err;
	int nLength;
	TCHAR szBuffer[MAXSIZE],postexp[MAXSIZE];			// �����ַ���

	nLength = _tcslen(expression);
	nLength++;

	for(int i=0;i<nLength;i++)
		szBuffer[i] = expression[i];

	//���ʽ����
	Deal_String(szBuffer);
	if(err = Trans_Exp(szBuffer,postexp))  // ����ȫ postexp�����С�޶������ֵ
		return err;

	// ����f(x+h)
	if(err = CompValue(postexp,(x+interval),&temp_a))
		return err;
	// ����f(x)
	if(err = CompValue(postexp,x,&temp_b))
		return err;
	// ����f(x-h)
	if(err = CompValue(postexp,(x-interval),&temp_c))
		return err;
	temp = interval * interval;
	if(fabs(temp) < eps)
		return DIVIDE_BY_ZERO;

	result = (temp_a - 2*temp_b + temp_c) / temp;

	return 0;
}
