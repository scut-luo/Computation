#include "Computation.h"
#include "Calculus.h"

Calculus::Calculus(HWND hwndDlg)
{
	eps = 1e-6;
	hDlg = hwndDlg;
}

//微分计算
ErrNo Calculus::Algorithm_Choice_D(int iSelectIndex)
{
	ErrNo err;
	Differential_Algorithm diff;

	switch(iSelectIndex)
	{
		//向前差分
	case FORWARD_DIFFERENCE_INDEX:
		if(err = diff.Forward_Difference(result,expression,value,interval))
			return err;
		break;
		
		//向后差分
	case BACKFORWARD_DIFFERENCE_INDEX:
		if(err = diff.Backward_Difference(result,expression,value,interval))
			return err;
		break;

		//中心差分
	case CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		if(err = diff.Central_Difference(result,expression,value,interval))
			return err;
		break;

		//二阶中心差分
	case SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		if(err = diff.Second_Order_Central_Difference(result,expression,value,interval))
			return err;
		break;
	}
	return 0;
}

ErrNo Calculus::GetStart_D(int iSelectIndex)
{
	PCTSTR pErrInfo;
	TCHAR szBuffer[MAXSIZE];
	int nLength;
	ErrNo err;

	//获取间隔大小
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID_D[0]),&interval))
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n补充信息:输入间隔大小数据错误"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);
		return err;
	}
	//获取所求点
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID_D[1]),&value))
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n补充信息:输入所求点数据错误"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);
		return err;
	}

	//获取函数表达式
	if(!(nLength = Edit_GetText(GetDlgItem(hDlg,EditControlID_D[2]),expression,MAXSIZE)))
	{
		err = NO_DATA_INPUT;
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n补充信息:未输入微分计算所需的表达式"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);
		return err;
	}

	//复制一份函数表达式
	nLength = _tcslen(expression) + 1;
	for(int i=0;i<nLength;i++)
		exp_store[i] = expression[i];

	//算法选择
	err = Algorithm_Choice_D(iSelectIndex);

	//输出结果
	OutputResult_D(iSelectIndex,err);
	return 0;
}

void Calculus::OutputResult_D(int iSelectIndex,ErrNo err)
{
	PCTSTR	pErrInfo;
	HWND hEdit = GetDlgItem(hDlg,EditOutputID);
	TCHAR szBuffer[MAXSIZE];

	Edit_SetSel(hEdit,0,0);
	switch(iSelectIndex)
	{
	case FORWARD_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("选择的算法: 基于Taylor公式的数值微分公式-向前差商 \r\n"));
		break;

	case BACKFORWARD_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("选择的算法: 基于Taylor公式的数值微分公式-向后差商 \r\n"));
		break;

	case CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("选择的算法: 基于Taylor公式的数值微分公式-中心差商 \r\n"));
		break;

	case SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("选择的算法: 基于Taylor公式的数值微分公式-二阶中心差商 \r\n"));
		break;
	}

	_stprintf_s(szBuffer,MAXSIZE,TEXT("函数表达式为：y=%s\r\n"),exp_store);
	Edit_ReplaceSel(hEdit,szBuffer);
	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	else
	{
		
		if(iSelectIndex == SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX)
			_stprintf_s(szBuffer,MAXSIZE,TEXT("所求点：%f  间隔：%f  函数在该点的二阶导数为：%f\r\n"),value,interval,result);
		else
			_stprintf_s(szBuffer,MAXSIZE,TEXT("所求点：%f  间隔：%f  函数在该点的一阶导数为：%f\r\n"),value,interval,result);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	Edit_ReplaceSel(hEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));
}


//积分计算
ErrNo Calculus::GetStart_I(int iSelectIndex)
{
	ErrNo			err;
	int				iLength;

	ChangeVariableSymbol(TEXT('x'));				//使用默认的变量符号

	//获取函数表达式
	if(! (iLength = Edit_GetText(GetDlgItem(hDlg,EditControlID[0]),expression,MAXINPUT)))
	{
		MessageBox(hDlg,TEXT("未输入表达式"),TEXT("错误"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}
	iLength = lstrlen(expression);
	for(int i=0;i<=iLength;i++)
	{
		exp_store[i] = expression[i];
	}


	//获取区间大小
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[1]),&left_interval))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[2]),&right_interval))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(iSelectIndex == GAUSS_LEGENDRE_INDEX || iSelectIndex == GAUSS_CHEBYSHEV_INDEX)
	{
		if(fabs(left_interval+1)>eps || fabs(right_interval-1)>eps)
		{
			MessageBox(hDlg,TEXT("区间输入错误，应输入范围[-1,1]"),TEXT("错误"),MB_ICONERROR);
			return NOT_MATCH;
		}
	}

	//获取计算精度
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[3]),&precision))
	{
		if(iSelectIndex == GAUSS_LEGENDRE_INDEX || iSelectIndex == GAUSS_CHEBYSHEV_INDEX)
			;
		else
		{
			ErrorReport(hDlg,err);
			return err;
		}
	}
	precision_a = precision;

	switch(iSelectIndex)
	{
	case ROMBERG_INDEX:
		err = Romberg(expression,precision,left_interval,right_interval,result,precision_a,times);
		break;

	case GAUSS_LEGENDRE_INDEX:
		err = Gauss_Legendre(expression,result);
		break;
		
	case GAUSS_CHEBYSHEV_INDEX:
		err = Gauss_Chebyshev(expression,result);
		break;
	}

	OutputResult_I(iSelectIndex,err);
	return 0;
}

void Calculus::OutputResult_I(int iSelectIndex,ErrNo err)
{
	PCTSTR		pErrInfo;
	HWND		hwndEdit = GetDlgItem(hDlg,EditOutputID);
	TCHAR		szBuffer[MAXSIZE];

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case ROMBERG_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Romberg算法 \r\n"));
		break;
	case GAUSS_LEGENDRE_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Gauss-Legendre求积公式(5点式) \r\n"));
		break;
	case GAUSS_CHEBYSHEV_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Gauss-Chebyshev求积公式(5点式) \r\n"));
		break;
	}


	swprintf(szBuffer,MAXSIZE,TEXT("所求函数为：%s\r\n"),exp_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	
	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		if(iSelectIndex == ROMBERG_INDEX)
		{
			if(precision_a == precision)
				swprintf(szBuffer,MAXSIZE,TEXT("积分值：%f  精度：%f  区间：[%f,%f] \r\n"),result,precision,left_interval,right_interval);
			else
				swprintf(szBuffer,MAXSIZE,TEXT("积分值：%f  精度：%f  已达到精度：%f  区间：[%f,%f] \r\n"),result,precision,precision_a,left_interval,right_interval);
		}
		else
		{
			swprintf(szBuffer,MAXSIZE,TEXT("积分值：%f  区间：[%f,%f] \r\n"),result,left_interval,right_interval);
		}
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));

}

//公共函数
ErrNo Calculus::GetLine_Double(HWND hwndEdit,double *fParameter)
{
	TCHAR		*ptemp;
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	if(!Judge_Double(Buffer))
		return INCORRECT_DATA;

	*fParameter = wcstod(Buffer,&ptemp);
	return 0;
}