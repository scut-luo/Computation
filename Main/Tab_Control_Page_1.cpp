#include "Computation.h"
#include "matrix.h"

TCHAR *Methods[]={TEXT("1.Gauss消元法"),
						TEXT("2.三角分解法"),
						TEXT("3.Thomas算法"),
						TEXT("4.平方根法"),
						TEXT("5.Jacobi迭代法"),
						TEXT("6.Gauss-Seidel迭代法"),
						TEXT("7.超松弛迭代法(SOR)"),
						TEXT("8.矩阵特征值计算"),
						0
};


static WNDPROC		OldFilesListProc;
//函数说明
VOID TabPage_1_FillComboBox(HWND hwndComboBox);
BOOL TabPage_1_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_1_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_1_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);			//Edit Control控件消息处理函数


BOOL CALLBACK TabPage_Proc_1(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_1_OnInitDialog);		//消息WM_INITDIALOG分流
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_1_OnContextMenu);	//消息WM_CONTEXTMENU分流
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_1_OnCommand);			//消息WM_COMMAND分流
	}
	return FALSE;
}

BOOL TabPage_1_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	TabPage_1_FillComboBox(GetDlgItem(hDlg,ComboBoxID));
	OldFilesListProc = (WNDPROC) SetWindowLong (GetDlgItem(hDlg,FileListID), GWL_WNDPROC,
		(LPARAM) FileListProc) ;

	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[0]),TEXT("0"));
	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[1]),TEXT("0.0001"));
	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[2]),TEXT("1"));

	//限定输入字符个数
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[0]),10);
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[2]),10);
	Edit_LimitText(GetDlgItem(hDlg,Edit_Dimension),5);


	return TRUE;
}

BOOL TabPage_1_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

BOOL TabPage_1_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	ErrNo err;
	static TCHAR szFilename[MAX_PATH],szTitleName[MAX_PATH];		//必须要static,不然出错 
	TCHAR szBuffer[MAX_PATH];
	static int iSelectIndex = 0;
	Matrix_Comp matrix(hDlg);

	switch(LOWORD(wParam))
	{
		//开始计算
	case IDC_MATRIX_START:
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
				if(err = matrix.GetStart_File(FilePath,nCount))
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
			else	//文件列表中没有文件
			{
				iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));
				if(matrix.GetStart(iSelectIndex))
					return FALSE;
			}
		}
		return TRUE;

		// 保存数据框的数据到文件当中
	case MATRIX_FILE_SAVE:
		if(FileSaveDlg(hDlg,szFilename,szTitleName))
		{
			if(FileWrite(GetDlgItem(hDlg,EditControlID),szFilename))
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

		// 将文件保存到文件列表中
	case MATRIX_FILE_OPEN:
		if(FileOpenDlg(hDlg,szFilename,szTitleName))
		{
			SendMessage(GetDlgItem(hDlg,FileListID),LB_ADDSTRING,0,(LPARAM)szFilename);
		}
		return TRUE;

		//右键菜单
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,EditControlID),TEXT(""));
		return TRUE;

	case RC_HELP:
		{
			TCHAR szBuffer[] = TEXT("1-4号的算法只需输入增广矩阵数据和维数(系数矩阵维数)\n5-8号的算法要求输入\"最大迭代次数\"、\"迭代精度\"或\"松弛因子\",同时输入增广矩阵数据和维数(系数矩阵维数),8号算法只需输入方阵数据");
			MessageBox(hDlg,szBuffer,TEXT("帮助"),MB_OK);
		}
		return TRUE;

	case MATRIX_FILE_HELP:
		{
			TCHAR szBuffer[] = TEXT("算法编号为0-7,依次对应下拉框的算法。\n0 -- Gauss消元法 ... 7 -- 特征值计算\n输入格式(参数逗号','分开):\n\t矩阵行数,矩阵列数,算法编号,最大迭代次数,迭代精度,松弛因子\n\t矩阵数据\n(三对角矩阵输入特殊,系数矩阵的每一条对角线为一行数据,增广矩阵最后列为一行)");
			MessageBox(hDlg,szBuffer,TEXT("帮助"),MB_OK);
		}
	}
	return FALSE;
}

VOID TabPage_1_FillComboBox(HWND hwndComboBox)
{
	TCHAR	**pVarName;

	pVarName = Methods;

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
