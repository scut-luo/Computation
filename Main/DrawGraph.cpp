/*
	在指定区域画图
*/
#include "Computation.h"

static ErrNo GetLine_Double(HWND hwndEdit,double *fParameter);
static bool Judge_Double(const TCHAR *szBuffer);

BOOL  CALLBACK DrawGraph_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
static BOOL  DrawGraph_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
static BOOL DrawGraph_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);
static BOOL  DrawGraph_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL  DrawGraph_OnPaint(HWND hDlg,WPARAM wParam,LPARAM lParam);

const	double		eps = 1e-6;
const	int			maxsize = 255;				//表达式数组最大大小
CChartWnd	chartWnd; 
static	TCHAR		store_expression[maxsize];
static	TCHAR		post_expression[maxsize];
static	TCHAR		original_expression[maxsize];
static	TCHAR		var_name;
static	double		L_Interval,R_Interval,Steps;			//画图参数
static	Expression  trans_exp;
const	int	EditControl_ID[] = {IDC_LEFT_INTERVAL ,IDC_RIGHT_INTERVAL};


ErrNo DrawGraph(HWND hDlg,TCHAR *expression,TCHAR variable_symbol,double Left_Interval ,double Right_Interval ,double Step_Size )
{
	Exception	exception;
	HINSTANCE	hInstance;
	ErrNo		err;
	int			iLength;

	hInstance = GetModuleHandle(NULL);

	var_name = variable_symbol;
	L_Interval = Left_Interval;
	R_Interval = Right_Interval;
	Steps = Step_Size;

	trans_exp.ChangeVariableSymbol(variable_symbol);
	iLength = lstrlen(expression);
	if(iLength+1 > maxsize)
		return ARRAY_OVERFLOW;

	for(int i=0;i<iLength+1;i++)
	{
		store_expression[i] = original_expression[i] = expression[i];
	}

	//表达式处理

	trans_exp.Deal_String(original_expression);
	if(err = trans_exp.Trans_Exp(original_expression,post_expression))
	{
		exception.ErrorReport(hDlg,err);
		return err;
	}
	
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_GRAPH),hDlg,(DLGPROC)DrawGraph_Proc);

	return 0;
}

BOOL CALLBACK DrawGraph_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,DrawGraph_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_PAINT,wParam,lParam,DrawGraph_OnPaint);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,DrawGraph_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_CLOSE,wParam,lParam,DrawGraph_OnClose);
	}
	return FALSE;
}

BOOL DrawGraph_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	Edit_SetText(GetDlgItem(hDlg,EditControl_ID[0]),TEXT("-10.0"));
	Edit_SetText(GetDlgItem(hDlg,EditControl_ID[1]),TEXT("10.0"));

	//限定输入字符个数
	Edit_LimitText(GetDlgItem(hDlg,EditControl_ID[0]),10);
	Edit_LimitText(GetDlgItem(hDlg,EditControl_ID[1]),10);
	return TRUE;
}

BOOL DrawGraph_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	int					iNums;
	ErrNo				err;
	Exception			exception;

	switch(LOWORD(wParam))
	{
	case IDC_REDRAW:
		if(err = GetLine_Double(GetDlgItem(hDlg,EditControl_ID[0]),&L_Interval))
		{
			exception.ErrorReport(hDlg,err);
			return FALSE;
		}
		if(err = GetLine_Double(GetDlgItem(hDlg,EditControl_ID[1]),&R_Interval))
		{
			exception.ErrorReport(hDlg,err);
			return FALSE;
		}
		if(L_Interval >= R_Interval)
		{
			MessageBox(hDlg,TEXT("区间输入有误"),TEXT("错误"),MB_ICONERROR);
			return FALSE;
		}


		chartWnd.GetChart()->ClearAllData();
		chartWnd.Detach();
		chartWnd.ReDraw();
		SendMessage(hDlg,WM_PAINT,0,0);
		return TRUE;

	case IDC_RESET_GRAPH:
		iNums = sizeof(EditControl_ID) / sizeof(EditControl_ID[0]);
		for(int i=0;i<iNums;i++)
		{
			Edit_SetText(GetDlgItem(hDlg,EditControl_ID[i]),TEXT(""));
		}
		return TRUE;
	}
	return FALSE;
}

BOOL  DrawGraph_OnPaint(HWND hDlg,WPARAM wParam,LPARAM lParam)
{

	ErrNo		err;
	TCHAR		szBuffer[maxsize+20];
	Exception	exception;
	double		x ,y,*pX,*pY,ftemp;
	int			iCount = 0,iMax;

	chartWnd.Attach(hDlg,IDC_PIC,kTypeXY);							//"粘"窗口
	chartWnd.GetChart()->SetUseLegend(false);						//不使用图例
	//chartWnd.GetChart()->SetGridLine(true, true);  
	//chartWnd.GetChart()->SetBkgndColor(RGB(224, 224, 224)); 

	
	iMax = 1;
	ftemp = L_Interval;
	while(1)
	{
		ftemp += Steps;
		if(fabs(R_Interval - ftemp)< eps || ftemp>R_Interval)
			break;
		iMax++;
	}
	if(iMax == 1)
		return FALSE;

	pX = new double [iMax];
	pY = new double [iMax];

	x = L_Interval;
	y = 0.0;
	iCount = 0;
	for(int i=0;i<iMax;i++)
	{
		if(err = trans_exp.CompValue(post_expression,x,&y))
		{
			if(err == WRONG_EXPRESSION)
			{
				exception.ErrorReport(hDlg,WRONG_EXPRESSION);
				return FALSE;
			}
			chartWnd.GetChart()->AddCurve(pX,pY,iCount);
			iCount = 0;
			x += Steps;
			continue;
		}
		pX[iCount] = x;
		pY[iCount] = y;
		x += Steps;
		iCount++;
	}
	chartWnd.GetChart()->AddCurve(pX,pY,iCount);

	delete [] pX;
	delete [] pY;

	wsprintf(szBuffer,TEXT("表达式为: f(%c)=%s"),var_name,store_expression);
	chartWnd.GetChart()->SetTitle(szBuffer);

	return TRUE;
}

BOOL DrawGraph_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	chartWnd.GetChart()->ClearAllData();
	chartWnd.Detach();
	memset((TCHAR *)post_expression,0,maxsize);
	memset((TCHAR *)original_expression,0,maxsize);
	memset((TCHAR *)store_expression,0,maxsize);
	EndDialog(hDlg,0);
	return TRUE;
}

static ErrNo GetLine_Double(HWND hwndEdit,double *fParameter)
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

//判断输入是否为小数
static bool Judge_Double(const TCHAR *szBuffer)
{
	int		iLength = 0;

	iLength = lstrlen(szBuffer);

	for(int i=0;i<iLength;i++)
	{
		if(szBuffer[i] == TEXT('.'))
		{
			if(i==0 || i==iLength -1)
				return FALSE;
		}
		else if(szBuffer[i] == TEXT('-'))
		{
			if(i!=0)
				return FALSE;
		}
		else if( (szBuffer[i]<TEXT('0')) || (szBuffer[i]>TEXT('9')) )
		{
			return FALSE;
		}
	}

	return TRUE;
}