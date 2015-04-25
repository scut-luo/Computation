/*
	微积分数值计算
*/
#include "Computation.h"
#include "Calculus.h"

TCHAR	*Methods_Differential[] = { TEXT("1.基于Taylor公式的数值微分公式-向前差商"),
									TEXT("2.基于Taylor公式的数值微分公式-向后差商"),
									TEXT("3.基于Taylor公式的数值微分公式-中心差商"),
									TEXT("4.基于Taylor公式的数值微分公式-二阶中心差商"),
									0
};

TCHAR	*Methods_Integral[] = {	TEXT("1.Romberg算法"),
								TEXT("2.Gauss-Legendre求积公式(5点式)"),
								TEXT("3.Gauss-Chebyshev求积公式(5点式)"),
								0				
};

BOOL TabPage_4_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_4_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_4_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);
VOID TabPage_4_FillComboBox(HWND hwndComboBox,TCHAR **Methods);

BOOL CALLBACK TabPage_Proc_4(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_4_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_4_OnContextMenu);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_4_OnCommand);
	}
	return FALSE;
}

BOOL TabPage_4_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	TabPage_4_FillComboBox(GetDlgItem(hDlg,ComboBoxID[0]),Methods_Differential);
	TabPage_4_FillComboBox(GetDlgItem(hDlg,ComboBoxID[1]),Methods_Integral);

	SendMessage(GetDlgItem(hDlg,RadioID[1]),BM_SETCHECK,BST_CHECKED,0);

	//微分	
	//限定输入字符个数
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_D[0]),10);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_D[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_D[2]),MAXSIZE);

	return TRUE;
}

BOOL TabPage_4_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

BOOL TabPage_4_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	int			iCount,iSelectRadio,iSelectIndex;
	Calculus	calculus(hDlg);

	switch(LOWORD(wParam))
	{
	case IDC_CALCULUS_START:
		iCount = sizeof(RadioID) / sizeof(RadioID[0]);
		for(int i=0;i<iCount;i++)
		{
			if(SendMessage(GetDlgItem(hDlg,RadioID[i]),BM_GETCHECK,0,0) == BST_CHECKED)
			{
				iSelectRadio = i;
				break;
			}
		}
		switch(iSelectRadio)
		{
		case 0:						//微分计算
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[0]));
			calculus.GetStart_D(iSelectIndex);
			break;
		case 1:						//积分计算
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[1]));
			calculus.GetStart_I(iSelectIndex);
			break;	
		}
		return TRUE;

		//右键菜单消息处理
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,EditOutputID),TEXT(""));
		return TRUE;
	}
	return FALSE;
}

VOID TabPage_4_FillComboBox(HWND hwndComboBox,TCHAR **Methods)
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