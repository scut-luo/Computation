/*
	常微分方程计算
*/
#include "Computation.h"
#include "ODE.h"

TCHAR	*Methods_ODE[] = { TEXT("1.Euler方法"),
						   TEXT("2.改进的Euler方法"),
						   TEXT("3.4阶Runge-Kutta方法"),
						   TEXT("4.4阶Adams预测-校正格式(简易格式)"),
						   TEXT("5.修正的Milne-Hamming预测-校正格式"),
						   TEXT("6.Adams4步4阶显式公式"),
						   TEXT("7.Milne4步4阶显式公式"),
						   0

};

static WNDPROC		OldListBoxProc;

BOOL TabPage_5_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_5_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_5_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);
VOID TabPage_5_FillComboBox(HWND hwndComboBox,TCHAR **Methods);
static LRESULT CALLBACK ListBoxProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TabPage_Proc_5(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_5_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_5_OnContextMenu);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_5_OnCommand);
	}
	return FALSE;
}

BOOL TabPage_5_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	HWND	FilesListBox = GetDlgItem(hDlg,FilesListBoxID);
	FileInitialize(hDlg);
	TabPage_5_FillComboBox(GetDlgItem(hDlg,ComboBoxID),Methods_ODE);

	OldListBoxProc = (WNDPROC) SetWindowLong (FilesListBox,GWL_WNDPROC,
		(LPARAM) ListBoxProc) ;
	return TRUE;
}

BOOL TabPage_5_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	static	TCHAR	szFilename[MAX_PATH],szTitleName[MAX_PATH];
	TCHAR	szBuffer[MAX_PATH];
	ODE		ode(hDlg);
	int		iSelectIndex;

	switch(LOWORD(wParam))
	{
	case IDC_ODE_START:
		iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,IDC_ODE_METHOD));
		ode.GetStart(iSelectIndex);
		return TRUE;
		
	case IDC_ODE_RESET:
		Edit_SetText(GetDlgItem(hDlg,OutputResultID),TEXT(""));
		return TRUE;

	case IDC_ODE_SAVE:
		if(FileSaveDlg(hDlg,szFilename,szTitleName))
		{
			if(FileWrite(GetDlgItem(hDlg,OutputResultID),szFilename))
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

	case IDC_ODE_OPEN:
		if(FileOpenDlg(hDlg,szFilename,szTitleName))
		{
			SendMessage(GetDlgItem(hDlg,FilesListBoxID),LB_ADDSTRING,0,(LPARAM)szFilename);
		}
		return TRUE;

	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,OutputResultID),TEXT(""));
		return TRUE;
	}
	return FALSE;
}

BOOL TabPage_5_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

VOID TabPage_5_FillComboBox(HWND hwndComboBox,TCHAR **Methods)
{
	TCHAR  **pVarName;

	pVarName = Methods;

	while(*pVarName)
	{
		SendMessage(hwndComboBox,CB_ADDSTRING,0,(LPARAM)*pVarName);
		pVarName++;
	}
	SendMessage(hwndComboBox,CB_SETCURSEL,0,0);
}

static LRESULT CALLBACK ListBoxProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	return CallWindowProc(OldListBoxProc,hwnd,message,wParam,lParam);
}