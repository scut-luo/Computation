/*
	函数插值
*/
#include "Computation.h"
#include "IAF.h"
			 

InterpolationAndFitting::InterpolationAndFitting(HWND hwndDlg)
{
	eps = 1e-6;
	this->hwndDlg = hwndDlg;
}

ErrNo InterpolationAndFitting::Algorithm_Choice_I(int iSelectIndex,const DPOINT *dpoints_I,const double compvalue,const int degree,double &result)
{
	Interpolation_Algorithm	interpolation;
	ErrNo err;

	switch(iSelectIndex)
	{
	case LAGRANGE_INTERPOLATION_INDEX:							//Lagrange插值
		if(err = interpolation.Lagrange(dpoints_I,compvalue,degree,result))
			return err;
		break;

	case  NEWTON_INTERPOLATION_INDEX:							//Newton插值
		if(err = interpolation.Newton(dpoints_I,compvalue,degree,result))
			return err;
		break;

	case HERMITE_INTERPOLATION_INDEX:							//Hermite插值
		if(err = interpolation.Hermite(dpoints_I,compvalue,degree,result))
			return err;
		break;

	case PIECEWISE_LINEAR_INTERPOLATION:
		if(err = interpolation.Piecewise_Linear_Interpolation(dpoints_I,compvalue,degree,result))
			return err;
		break;

	case PIECEWISE_HERMITE_INTERPOLATION:
		if(err = interpolation.Piecewise_Hermite_Interpolation(dpoints_I,compvalue,degree,result))
			return err;
		break;

	case PIECEWISE_SPLINE_INTERPOLATION_DF:
		if(err = interpolation.Piecewise_Spline_Interpolation_DF(dpoints_I,compvalue,degree,result))
			return err;
		break;
	}
	return 0;
}

ErrNo InterpolationAndFitting::Algorithm_Choice_F(PTSTR *polynomial,double *parameters,int iSelectIndex,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps)
{

	ErrNo err;
	Fitting_Algorithm fitting;

	switch(iSelectIndex)
	{
		//线性最小二乘拟合
	case LEAST_SQUARE_FITTING_INDEX:
		if(err = fitting.Least_Square_Fitting(polynomial,parameters,nNumofPolynomial,nNumofPoints,dPoints_F,eps))
			return err;
		break;
	
		//正交多项式
	case POLYNOMINAL_FITTING_INDEX:
		if(err = fitting.Polynominal_Fitting(parameters,dPoints_F,nNumofPoints,degree,eps))
			return err;
		break;

		//指数模型的最小二乘拟合
	case EXPONENTIAL_FITTING_INDEX:
		if(err = fitting.Exponential_Fitting(parameters,dPoints_F,nNumofPoints,eps))
			return err;
		break;

	}
	return 0;
}

//插值(开始）
ErrNo InterpolationAndFitting::GetStart_I(int iSelectIndex)
{
	ErrNo	err;
	//Interpolation_Algorithm interpolation;
	double	eps = 1e-6;

	//获取所求点
	if( err = GetLine_Double(GetDlgItem(hwndDlg,EditControlID_I[5]),&compvalue))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}

	//获取次数
	if( err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID_I[4]),&degree))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}

	//申请空间
	if(!(dpoints_I = new DPOINT [degree+1]))
	{
		ErrorReport(hwndDlg,ALLOCATE_MEMORY_FAIL);
		return ALLOCATE_MEMORY_FAIL;
	}
	
	//获取插值点
	if(err = GetPoints_I(dpoints_I,GetDlgItem(hwndDlg,ListViewID_I),degree,iSelectIndex))
	{
		ErrorReport(hwndDlg,err);
		DestroyPointer(dpoints_I);
		return err;
	}

	//算法选择
	err = Algorithm_Choice_I(iSelectIndex,dpoints_I,compvalue,degree,result);

	OutputResult_I(iSelectIndex,err);
	DestroyPointer(dpoints_I);									//销毁指针
	return 0;
}

//拟合（开始）
ErrNo InterpolationAndFitting::GetStart_F(int iSelectIndex)
{
	TCHAR	szBuffer[MAXINPUT];
	PCTSTR  pszPos = NULL;
	HWND	hListView = NULL,hEdit = NULL;
	int		nNum;
	ErrNo	err;
	
	hListView = GetDlgItem(hwndDlg,ListViewID_F);
	hEdit = GetDlgItem(hwndDlg,EditControlID_F[2]);

	//判断输入多少行数据
	nNumofPoints = ListView_GetItemCount(hListView);
	if(!nNumofPoints)
	{
		MessageBox(hwndDlg,TEXT("没有输入拟合数据"),TEXT("错误"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}
	
	//拟合点储存
	if(!(dPoints_F = new DPOINT [nNumofPoints]))
	{
		ErrorReport(hwndDlg,ALLOCATE_MEMORY_FAIL);
		return ALLOCATE_MEMORY_FAIL;
	}
	//获得拟合点
	if(err = GetPoints_F(dPoints_F,hListView,iSelectIndex))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}

	switch(iSelectIndex)
	{
		// 正交多项式拟合
	case POLYNOMINAL_FITTING_INDEX:
		// 获取正交多项式次数
		if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID_F[3]),&degree))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		nNum = degree;
		//申请空间（储存拟合计算的参数）注意：k次多项式，系数有k+1个
		if(!(parameters = new double [nNum+1]))
		{
			ErrorReport(hwndDlg,ALLOCATE_MEMORY_FAIL);
			return ALLOCATE_MEMORY_FAIL;
		}

		break;

		// 指数模型拟合
	case EXPONENTIAL_FITTING_INDEX:
		nNum = 2;
		//申请空间（储存拟合计算的参数）
		if(! (parameters = new double [nNum]) )
		{
			ErrorReport(hwndDlg,ALLOCATE_MEMORY_FAIL);
			return ALLOCATE_MEMORY_FAIL;
		}
		break;
	default:
		//判断是否输入拟合多项式
		if(! Edit_GetText(hEdit,szBuffer,MAXINPUT))
		{
			MessageBox(hwndDlg,TEXT("未输入拟合多项式"),TEXT("错误"),MB_ICONERROR);
			return NO_DATA_INPUT;
		}

		//获取有多少个多项式（根据逗号决定）
		nNum = 0;
		pszPos = szBuffer;
		while(*pszPos != TEXT('\0'))
		{
			if(*pszPos == TEXT(','))
				nNum++;
			pszPos++;
		}
		if(nNum == 0)
		{
			MessageBox(hwndDlg,TEXT("拟合多项式输入错误"),TEXT("错误"),MB_ICONERROR);
			return INCORRECT_DATA;
		}
		nNum++;
		nNumofPolynomial = nNum;

		//申请空间保存插值多项式（二维字符数组）
		if(! (polynomial = new TCHAR* [nNum]) )
			return ALLOCATE_MEMORY_FAIL;
		for(int i=0;i<nNum;i++)
		{
			if(! (polynomial[i] = new TCHAR [MAXSIZE]) )
				return ALLOCATE_MEMORY_FAIL;
		}

		//获得多项式
		if(err = GetPolynomial_F(polynomial,szBuffer,MAXINPUT,nNum))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}

		//申请空间（储存拟合计算的参数）
		if(! (parameters = new double [nNum]) )
		{
			ErrorReport(hwndDlg,ALLOCATE_MEMORY_FAIL);
			return ALLOCATE_MEMORY_FAIL;
		}
		break;
	}


	//算法选择
	err = Algorithm_Choice_F(polynomial,parameters,iSelectIndex,nNum,nNumofPoints,dPoints_F,eps);

	OutputResult_F(iSelectIndex,err);

	//销毁空间
	DestroyPointer(dPoints_F);
	delete [] parameters;
	switch(iSelectIndex)
	{
	case POLYNOMINAL_FITTING_INDEX:
	case EXPONENTIAL_FITTING_INDEX:
		break;
	default:
		for(int i=0;i<nNum;i++)
		{
			delete [] polynomial[i];
		}
		delete [] polynomial;
		break;
	}
	
	return 0;
}

ErrNo InterpolationAndFitting::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

ErrNo InterpolationAndFitting::GetLine_Double(HWND hwndEdit,double *fParameter)
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

ErrNo InterpolationAndFitting::GetPoints_I(DPOINT *dPoints,HWND hListView,int n,int iSelectIndex)
{
	int		nNum,iLength;
	TCHAR	szBuffer[MAXINPUT],*stopstring;
	double	fNum;
	LVITEM	item;

	nNum = ListView_GetItemCount(hListView);
	if(nNum != n+1)
		return NOT_MATCH_DEGREE;

	//n+1个插值点
	for(int i=0;i<nNum;i++)
	{
		item.pszText = szBuffer;
		item.cchTextMax = MAXINPUT;
		for(int j=0;j<4;j++)
		{
			item.iSubItem = j;
			iLength = SendMessage(hListView,LVM_GETITEMTEXT,i,(LPARAM)&item);
			if(iLength)
			{
				fNum = wcstod(szBuffer,&stopstring);
				switch(j)
				{
				case 0:
					dPoints[i].x = fNum;
					break;
				case 1:
					dPoints[i].fx = fNum;
					break;
				case 2:
					dPoints[i].df = fNum;
					break;
				case 3:
					dPoints[i].ddf = fNum;
					break;
				}
			}
		}

	}

	QuickSort(dPoints,0,n);					//x值从小到大

	switch(iSelectIndex)
	{
	case HERMITE_INTERPOLATION_INDEX:					//Hermite插值
	case PIECEWISE_HERMITE_INTERPOLATION:				//分段Hermite插值	
		for(int i=0;i<nNum;i++)
		{
			if(dPoints[i].df == DBL_MAX)
				return NOT_MATCH;
		}
		break;
		
	case PIECEWISE_SPLINE_INTERPOLATION_DF:				//三次样条插值(已知两端点的一阶导数)
		if(dPoints[0].df == DBL_MAX || dPoints[n].df == DBL_MAX)
			return NOT_MATCH;
		break;
	case PIECEWISE_SPLINE_INTERPOLATION_DDF:
		if(dPoints[0].ddf == DBL_MAX || dPoints[n].ddf == DBL_MAX)
			return NOT_MATCH;
		break;
	}
	return 0;

}

ErrNo InterpolationAndFitting::GetPoints_F(DPOINT *dPoints,const HWND hListView,const int SelectIndex)
{
	LVITEM	item;
	double	fNum;
	int		nNum,iLength;
	TCHAR	szBuffer[MAXINPUT],*stopstring;

	nNum = ListView_GetItemCount(hListView);

	for(int i=0;i<nNum;i++)
	{
		item.pszText = szBuffer;
		item.cchTextMax = MAXINPUT;
		for(int i=0;i<nNum;i++)
		{
			for(int j=0;j<2;j++)
			{
				item.iSubItem = j;
				iLength = SendMessage(hListView,LVM_GETITEMTEXT,i,(LPARAM)&item);
				if(iLength)
				{
					fNum = wcstod(szBuffer,&stopstring);
					switch(j)
					{
					case 0:
						dPoints[i].x = fNum;
						break;
					case 1:
						dPoints[i].fx = fNum;
						break;
					}
				}
				else
					return NO_DATA_INPUT;
			}
		}
	}
	return 0;
}

ErrNo InterpolationAndFitting::GetPolynomial_F(PTSTR *polynomial,PTSTR szBuffer,const int nMax,const int nNumofPolynomial)
{
	TCHAR	seps[] = TEXT(",");					//分割字符
	TCHAR	*pstr,*pnext_token = NULL;
	int		nCount,iLength;

	//分割字符串
	nCount = 0;
	pstr = wcstok_s(szBuffer,seps,&pnext_token);
	while(pstr != NULL)
	{
		iLength = _tcslen(pstr) > (size_t)nMax ? nMax : _tcslen(pstr);
		for(int i=0;i<=iLength;i++)
		{
			polynomial[nCount][i] = pstr[i];
		}
		pstr = wcstok_s(NULL,seps,&pnext_token);
		nCount++;
	}

	if(nNumofPolynomial != nCount)
		return NOT_MATCH;
	return 0;
}

void InterpolationAndFitting::DestroyPointer(DPOINT *&dpoints)
{
	delete [] dpoints;
	dpoints = NULL;
}

void InterpolationAndFitting::OutputResult_I(int iSelectIndex,ErrNo err)
{
	PCTSTR		pErrInfo;
	TCHAR		szBuffer[MAXSIZE];
	HWND		hwndEdit = GetDlgItem(hwndDlg,OutPut_ResultID);

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case LAGRANGE_INTERPOLATION_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Lagrange插值法 \r\n"));
		break;
	case NEWTON_INTERPOLATION_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Newton插值法 \r\n"));
		break;
	case HERMITE_INTERPOLATION_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Hermite插值法 \r\n"));
		break;
	case PIECEWISE_LINEAR_INTERPOLATION:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 分段线性插值 \r\n"));
		break;
	case PIECEWISE_HERMITE_INTERPOLATION:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 分段Hermite插值 \r\n"));
		break;
	case PIECEWISE_SPLINE_INTERPOLATION_DF:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 分段样条插值(已知两端点的一阶导数) \r\n"));
		break;
	case PIECEWISE_SPLINE_INTERPOLATION_DDF:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 分段样条插值(已知两端点的二阶导数) \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID：%d\r\n错误信息：%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		swprintf(szBuffer,MAXSIZE,TEXT("所求点:x=%f  次数:%d  计算结果:%f \r\n"),compvalue,degree,result);
		Edit_ReplaceSel(hwndEdit,szBuffer);
		Edit_ReplaceSel(hwndEdit,TEXT("插值点: \r\n"));
		for(int i=0;i<=degree;i++)
		{
			if(dpoints_I[i].df != DBL_MAX)
				swprintf(szBuffer,MAXSIZE,TEXT("x=%f  f(x)=%f  f'(x)=%f \r\n"),dpoints_I[i].x,dpoints_I[i].fx,dpoints_I[i].df);
			else if(dpoints_I[i].ddf != DBL_MAX)
				swprintf(szBuffer,MAXSIZE,TEXT("x=%f  f(x)=%f  f''(x)=%f \r\n"),dpoints_I[i].x,dpoints_I[i].fx,dpoints_I[i].ddf);
			else
				swprintf(szBuffer,MAXSIZE,TEXT("x=%f  f(x)=%f \r\n"),dpoints_I[i].x,dpoints_I[i].fx);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
	}
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++\r\n"));

}

void InterpolationAndFitting::OutputResult_F(int iSelectIndex,ErrNo err)
{
	PCTSTR		pErrInfo;
	TCHAR		szBuffer[MAXSIZE];
	HWND		hwndEdit = GetDlgItem(hwndDlg,OutPut_ResultID);

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case LEAST_SQUARE_FITTING_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 线性最小二乘拟合 \r\n"));
		break;

	case POLYNOMINAL_FITTING_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 正交多项式拟合 \r\n"));
		break;

	case EXPONENTIAL_FITTING_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 指数模型的最小二乘解 \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID：%d\r\n错误信息：%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		switch(iSelectIndex)
		{
		case LEAST_SQUARE_FITTING_INDEX:
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("拟合曲线: s(x)="));
			Edit_ReplaceSel(hwndEdit,szBuffer);
			for(int i=0;i<nNumofPolynomial;i++)
			{
				if(i != nNumofPolynomial-1)
					_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%f*%s+"),parameters[i],polynomial[i]);
				else
					_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%f*%s"),parameters[i],polynomial[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			//输出拟合数据
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n计算数据\r\n"));
			for(int i=0;i<nNumofPoints;i++)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%10f\tf%d=%10f\r\n"),i+1,dPoints_F[i].x,i+1,dPoints_F[i].fx);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			break;
		
		case POLYNOMINAL_FITTING_INDEX:
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("拟合曲线参数:"));
			Edit_ReplaceSel(hwndEdit,szBuffer);
			for(int i=0;i<=degree;i++)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("a%d=%f  "),i,parameters[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			//输出拟合数据
			Edit_ReplaceSel(hwndEdit,TEXT("计算数据\r\n"));
			for(int i=0;i<nNumofPoints;i++)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%10f\tf%d=%10f\r\n"),i,dPoints_F[i].x,i,dPoints_F[i].fx);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			break;
			
		case EXPONENTIAL_FITTING_INDEX:
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("拟合曲线: s(x)="));
			Edit_ReplaceSel(hwndEdit,szBuffer);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%f*e^(%fx)\r\n"),parameters[0],parameters[1]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
			//输出拟合数据
			Edit_ReplaceSel(hwndEdit,TEXT("计算数据\r\n"));
			for(int i=0;i<nNumofPoints;i++)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%10f\tf%d=%10f\r\n"),i,dPoints_F[i].x,i,dPoints_F[i].fx);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			break;
			
		}
	}
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++\r\n"));
}

void InterpolationAndFitting::QuickSort(DPOINT *dPoints,int s,int t)
{
	int			i =s,j =t;
	DPOINT		ptemp;

	if(s<t)
	{
		ptemp = dPoints[s];
		while(i!=j)
		{
			while(j>i && dpoints_I[j].x >= ptemp.x)
				j--;
			dPoints[i] = dPoints[j];
			while(i<j && dPoints[i].x <= ptemp.x)
				i++;
			dPoints[j] = dPoints[i];
		}
		dPoints[i] = ptemp;
		QuickSort(dPoints,s,i-1);
		QuickSort(dPoints,i+1,t);
	}
}



