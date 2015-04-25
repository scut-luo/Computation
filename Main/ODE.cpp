#include "Computation.h"
#include "ODE.h"

ODE::ODE(HWND hwndDlg)
{
	eps = 1e-6;
	hDlg = hwndDlg;
}

ErrNo ODE::GetStart(int iSelectIndex)
{
	ErrNo	err;
	TCHAR	variable_symbol_array[2];
	TCHAR	szBuffer[MAXSIZE];
	double	x;
	int		iLength;

	variable_symbol_array[0] = TEXT('x');
	variable_symbol_array[1] = TEXT('y');

	//获取表达式
	if(!(iLength = Edit_GetText(GetDlgItem(hDlg,EditControlID[0]),szBuffer,MAXSIZE-1)))
	{
		ErrorReport(hDlg,NO_DATA_INPUT);
		return NO_DATA_INPUT;
	}
	for(int i=0;i<=lstrlen(szBuffer);i++)
		exp_store[i] = szBuffer[i];

	//获取区间参数
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[1]),&left_interval))			//区间左端
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[2]),&right_interval))			//区间右端
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(left_interval >= right_interval || fabs(right_interval - left_interval)< eps)
	{
		MessageBox(hDlg,TEXT("区间输入有误"),TEXT("错误"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//获取初始值
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[3]),&initial_value))
	{
		ErrorReport(hDlg,err);
		return err;
	}

	//获取步长
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[4]),&step_size))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(step_size < 0 || fabs(step_size) < eps)
	{
		MessageBox(hDlg,TEXT("步长输入有误"),TEXT("错误"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//获取在指定区间要计算的点的个数
	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(abs(num_x) == 0)
	{
		MessageBox(hDlg,TEXT("区间设置太短"),TEXT("错误"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//申请空间
	if(!(results = new double* [2]))
	{
		ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
		return ALLOCATE_MEMORY_FAIL;
	}
	for(int i=0;i<2;i++)
	{
		if(!(results[i] = new double [num_x+1]))
		{
			ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
			return ALLOCATE_MEMORY_FAIL;
		}
	}

	err  = 0;
	switch(iSelectIndex)
	{
	case EULER_INDEX:
		err = Euler(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case IMPROVED_EULER_INDEX:
		err = Improved_Euler(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case RUNGE_KUTTA_INDEX:
		err = Runge_Kutta(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case SIMPLE_ADAMS_PC_INDEX:
		err = Simple_Adams_PC(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case MILNE_HAMMING_INDEX:
		err = Milne_Hamming(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case ADAMS_INDEX:
		err = Adams(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case MILNE_INDEX:
		err = Milne(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;
	}

	OutputResults(iSelectIndex,err);

	for(int i=0;i<2;i++)
		delete [] results[i];
	delete [] results;
	return 0;
}

ErrNo ODE::GetLine_Double(HWND hwndEdit,double *fParameter)
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

ErrNo ODE::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

ErrNo ODE::OutputResults(const int iSelectIndex,ErrNo err)
{
	PCTSTR	pErrInfo;
	HWND	hwndEdit = GetDlgItem(hDlg,OutputResultID);
	TCHAR	szBuffer[MAXSIZE];

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case EULER_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Euler方法(显式单步法) \r\n"));
		break;
	case IMPROVED_EULER_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 改进的Euler方法 \r\n"));
		break;
	case RUNGE_KUTTA_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 4阶Runge-Kutta方法 \r\n"));
		break;
	case MILNE_HAMMING_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 修正的Milne-Hamming预测-校正格式 \r\n"));
		break;
	case SIMPLE_ADAMS_PC_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 4阶Adams预测-校正格式(简易格式) \r\n"));
		break;
	case ADAMS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Adams4步4阶显式公式 \r\n"));
		break;
	case MILNE_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Milne4步4阶显式公式 \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("所求常微分方程为: y'=%s   y(%f)=%f \r\n"),exp_store,left_interval,initial_value);
		//swprintf(szBuffer,MAXSIZE,TEXT("所求常微分方程为: y'=%s   y(%f)=%f \r\n"),exp_store,left_interval,initial_value);
		Edit_ReplaceSel(hwndEdit,szBuffer);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("计算参数为: 区间为[%f,%f] 步长为%f \r\n"),left_interval,right_interval,step_size);
		//swprintf(szBuffer,MAXSIZE,TEXT("计算参数为: 区间为[%f,%f] 步长为%f \r\n"),left_interval,right_interval,step_size);
		Edit_ReplaceSel(hwndEdit,szBuffer);
		for(int i=0;i<=num_x;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x=%f  y=%f \r\n"),results[0][i],results[1][i]);
			//swprintf(szBuffer,MAXSIZE,TEXT("x=%f  y=%f \r\n"),results[0][i],results[1][i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
	}	
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));
	return 0;
}