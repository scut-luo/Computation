#include "Computation.h"
#include "equation.h"

TCHAR	*Methods_FX[] = { TEXT("1.不动点迭代"),
					   TEXT("2.Newton迭代"),
					   TEXT("3.Aitken加速方案"),
					   TEXT("4.Steffensen迭代"),
					   0
};

static WNDPROC	OldFilesListProc;

BOOL TabPage_2_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_2_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);
VOID TabPage_2_FillComboBox(HWND hwndComboBox);
BOOL TabPage_2_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TabPage_Proc_2(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_2_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_2_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_2_OnContextMenu);
	}
	return FALSE;
}

BOOL TabPage_2_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	OldFilesListProc = (WNDPROC) SetWindowLong (GetDlgItem(hDlg,FileListID), GWL_WNDPROC,
		(LPARAM) FileListProc) ;

	TabPage_2_FillComboBox(GetDlgItem(hDlg,ComboBoxID));

	Edit_SetText(GetDlgItem(hDlg,InitialValueID[0]),TEXT("x"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[1]),TEXT("0"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[2]),TEXT("0.000001"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[3]),TEXT("0.0"));

	//限定输入字符个数
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[0]),1);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[2]),10);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[3]),10);

	Edit_LimitText(GetDlgItem(hDlg,IDC_ITERATIVE_FORMULA),MAXINPUT);
	Edit_LimitText(GetDlgItem(hDlg,IDC_FUNCTION_EXP),MAXINPUT);

	return TRUE;
}

BOOL TabPage_2_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	Exception			exception;
	TCHAR				ErrInfo[MAXSIZE];
	PCTSTR				pErrInfo;

	ErrNo				err;
	int					iLength;
	TCHAR				szFunExp[MAXINPUT],szBuffer[MAXINPUT];
	static TCHAR		szFilename[MAX_PATH],szTitleName[MAX_PATH];		//必须要static,不然出错 

	int					iSelectIndex = 0;
	Equation			equation(hDlg);

	switch(LOWORD(wParam))
	{
	case IDC_EQUATION_START:
		{
			HWND hFileList;
			TCHAR **FilePath;
			int nCount;

			hFileList = GetDlgItem(hDlg,FileListID);
			//判断是否有文件添加到文件列表中,如果优先计算文件中的数据
			nCount = SendMessage(hFileList,LB_GETCOUNT,0,0);
			if(nCount >= 1)	//如果有文件添加到文件列表中
			{
				//申请空间保存文件路径
				if(!(FilePath = new TCHAR* [nCount]))
					return FALSE;
				for(int i=0;i<nCount;i++)
				{
					if(!(FilePath[i] = new TCHAR [MAX_PATH]))
						return FALSE;
				}

				//获取文件列表
				for(int i=0;i<nCount;i++)
				{
					SendMessage(hFileList,LB_GETTEXT,i,(LPARAM)FilePath[i]);
				}

				//从文件获取数据开始计算
				if(err = equation.GetStart_File(FilePath,nCount))
				{
					for(int i=0;i<nCount;i++)
						delete [] FilePath[i];
					delete [] FilePath;
					return FALSE;
				}

				//删除申请空间
				for(int i=0;i<nCount;i++)
					delete [] FilePath[i];
				delete [] FilePath;
			}
			else
			{
				iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));
				equation.GetStart(iSelectIndex);									//计算开始
			}
		}
		return TRUE;

	case IDC_FORMULA_HELP:
		MessageBox(hDlg,TEXT("公式中可以输入\"+ - * \\ sin cos tan ln log ^ e\",括号要完全匹配 \n公式可以出现sin3或者sin(3)写法,但是类似\"ln4x\"这样的写法是错误的,应写成ln(4*x)[x为变量符号],公式中出现的变量符号要求与你所设定的一致,否则出现错误"),TEXT("帮助"),MB_OK);
		return TRUE;

	case IDC_GRAPH:
		if(!(iLength = Edit_GetText(GetDlgItem(hDlg,FunExpID),szFunExp,MAXINPUT-1)))
		{
			MessageBox(hDlg,TEXT("未输入所要画的函数表达式"),TEXT("错误"),MB_ICONERROR);
			return FALSE;
		}
		if(!(iLength = Edit_GetText(GetDlgItem(hDlg,InitialValueID[0]),szBuffer,MAXINPUT-1)))							//获取变量符号
		{
			MessageBox(hDlg,TEXT("未输入变量符号"),TEXT("错误"),MB_ICONERROR);
			return FALSE;
		}

		//判断是否输入正确字符
		if(!(( szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z')) || ( szBuffer[0]>=TEXT('a') && szBuffer[0]<=TEXT('z'))))
		{
			pErrInfo = exception.FormatError(INCORRECT_DATA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n补充信息:变量符号输入错误"),INCORRECT_DATA,pErrInfo);
			MessageBox(hDlg,ErrInfo,TEXT("错误"),MB_ICONERROR);
			return INCORRECT_DATA;
		}

		//大写变小写
		if(szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z'))
			szBuffer[0] += 32;
		DrawGraph(hDlg,szFunExp,szBuffer[0]);
		return TRUE;

		//保存输出框数据
	case EQ_SAVE:
		if(FileSaveDlg(hDlg,szFilename,szTitleName))
		{
			if(FileWrite(GetDlgItem(hDlg,EditControlID[6]),szFilename))
			{
				MessageBox(hDlg,TEXT("数据保存成功"),TEXT("文件保存"),MB_OK);
				return TRUE;
			}
			else
			{
				swprintf(szBuffer,MAX_PATH,TEXT("Could not write fils as %s"),szTitleName);
				MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);
				return FALSE;
			}
		}
		return TRUE;

	case EQ_OPEN:
		if(FileOpenDlg(hDlg,szFilename,szTitleName))
		{
			SendMessage(GetDlgItem(hDlg,FileListID),LB_ADDSTRING,0,(LPARAM)szFilename);
		}
		return TRUE;

		//右键菜单
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,EditControlID[6]),TEXT(""));
		return TRUE;

	case RC_HELP:
		{
			TCHAR szBuffer[1024] = TEXT("在输入表达式时,变量只有一个,默认符号为'x',在输入表达式之前要确定变量符号要和设置的一致。");
			MessageBox(hDlg,szBuffer,TEXT("帮助"),MB_OK);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL TabPage_2_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	RECT	rect;
	POINT	pt;
	HMENU	hroot,hpop;

	//获取鼠标右击时的坐标
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	//获取客户区大小
	GetClientRect(hDlg,&rect);
	//把屏幕坐标转为客户区坐标
	ScreenToClient(hDlg,&pt);
	//判断点是否位于客户区域内
	if(PtInRect(&rect,pt))
	{

		//加载菜单资源
		hroot = LoadMenu((HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),MAKEINTRESOURCE(RC_MENU));

		if(hroot)
		{
			//获取第一个弹出菜单
			hpop = GetSubMenu(hroot,0);

			ClientToScreen(hDlg,&pt);

			//这里可以设置某些菜单项是否被禁用

			//显示快捷菜单
			TrackPopupMenu(hpop,
				TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
				pt.x,
				pt.y,
				0,
				hDlg,
				NULL);
			//销毁菜单资源
			DestroyMenu(hroot);
		}
	}
	else
	{
		//显示系统菜单（在标题栏）
		return DefWindowProc(hDlg,WM_CONTEXTMENU,wParam,lParam);
	}

	return TRUE;
}

VOID TabPage_2_FillComboBox(HWND hwndComboBox)
{
	TCHAR	**pVarName;

	pVarName = Methods_FX;

	while(*pVarName)
	{
		SendMessage(hwndComboBox,CB_ADDSTRING,0,(LPARAM)*pVarName);
		pVarName++;
	}
	SendMessage(hwndComboBox,CB_SETCURSEL,0,0);
}

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{		
	int		*iSelectArray = NULL;
	int		iSelectCount = 0;

	switch(message)
	{
	case WM_RBUTTONDOWN:
		iSelectCount = SendMessage(hwnd,LB_GETSELCOUNT,0,0);
		if(iSelectCount == LB_ERR)
			return LB_ERR;
		iSelectArray = new int[iSelectCount];
		if( SendMessage(hwnd,LB_GETSELITEMS,iSelectCount,(WPARAM)iSelectArray) == LB_ERR )
			return LB_ERR;
		for(int i=0;i<iSelectCount;i++)
			SendMessage(hwnd,LB_DELETESTRING,iSelectArray[i]-i,0);
		delete [] iSelectArray;
		break;
	}
	return CallWindowProc(OldFilesListProc,hwnd,message,wParam,lParam);
}
