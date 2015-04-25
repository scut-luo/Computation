#include "Computation.h"

#define	ESM_POKECODEANDLOOKUP (WM_USER+100)

const int	ICO_ID = IDI_ICON;
const int	TAB_ID = IDC_TAB;
const TCHAR szAppName[]=TEXT("数值计算");
HINSTANCE		hInst;
TABCTRL			TabCtrl;			//Struct 

VOID Set_Dlg_Ico(HWND hWnd, int idi);			//设置图标
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	HWND			hwnd=NULL;

	hInst=hInstance;
	hwnd=FindWindow(TEXT("#32770"),szAppName);
	if(IsWindow(hwnd))
	{
	}
	else
	{
		DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_MAIN),NULL,Dlg_Proc,(LPARAM)iCmdShow);
	}
	return 0;
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_DLGMSG(hwnd,WM_CLOSE,wParam,lParam,Dlg_OnClose);
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,Dlg_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_NOTIFY,wParam,lParam,Dlg_OnNOTIFY);
	case ESM_POKECODEANDLOOKUP:
		SetForegroundWindow(hwnd);
		break;
	}
	return FALSE;
}

BOOL Dlg_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	RECT   rect;
	int	   cx_scr,cy_scr;
	LPTSTR tabnames[]={TEXT("方程组"),TEXT("一元方程求根"),TEXT("函数插值拟合"),TEXT("微积分计算"),TEXT("常微分方程数值解"),TEXT("主成分分析"),0};         //分页项目名字
	LPTSTR dlgnames[]={MAKEINTRESOURCE(TAB_CONTROL_PAGE_1),		 //每页对应对话框的ID
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_2),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_3),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_4),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_5),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_6),0};
	Page_Proc tab_control_page_proc[]={TabPage_Proc_1,TabPage_Proc_2,TabPage_Proc_3,TabPage_Proc_4,TabPage_Proc_5,TabPage_Proc_6};

	cx_scr = GetSystemMetrics(SM_CXSCREEN);
	cy_scr = GetSystemMetrics(SM_CYSCREEN);
	
	LoadLibrary(TEXT("Riched20.dll"));  //为在对话框使用Rich Edit 2.0（必须加载DLL）
	
	GetWindowRect(hDlg,&rect);
	MoveWindow(hDlg,(cx_scr-rect.right)/2,(cy_scr-rect.bottom)/2,rect.right-rect.left,rect.bottom-rect.top,TRUE);
	Set_Dlg_Ico(hDlg,ICO_ID);									 //设置图标

	New_TabControl(&TabCtrl,GetDlgItem(hDlg,TAB_ID),tabnames,dlgnames,tab_control_page_proc);

	return TRUE;
}

BOOL Dlg_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	EndDialog(hDlg,0);
	return TRUE;
}

BOOL Dlg_OnNOTIFY(HWND hDlg, WPARAM wParam,LPARAM lParam)
{
	RECT	rect;
	int		i,j;

	rect = TabCtrl.rect;


	if(((LPNMHDR) lParam)->code == TCN_SELCHANGE)
	{
		i=SendDlgItemMessage(hDlg,TAB_ID,TCM_GETCURSEL,0,0);								//通过 TCM_GETCURSEL 消息获取当前被选的Tab页
		MoveWindow(TabCtrl.hTabPages[i],rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);			//将Pagei页加载到目标位置
		for(j=0;j<TabCtrl.tabPageCount;j++)
		{
			if(j==i)
			{
				ShowWindow(TabCtrl.hTabPages[j],TRUE);
				continue;
			}
			ShowWindow(TabCtrl.hTabPages[j],FALSE);
		}
	}
	else if(((LPNMHDR) lParam)->code == TCN_SELCHANGING)
	{
		i=SendDlgItemMessage(hDlg,TAB_ID,TCM_GETCURSEL,0,0);
		EnumChildWindows(TabCtrl.hTabPages[i],EnumChildProc,0);
	}
	return TRUE;
}

VOID Set_Dlg_Ico(HWND hWnd, int idi) 
{
	SendMessage(hWnd, WM_SETICON, ICON_BIG,  (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
}

BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
{
	HWND hwndDlg = NULL;
	PCTSTR title = TEXT("PCA画图");

	if(hwndDlg = FindWindow(NULL,title))
	{
		SendMessage(hwndDlg,WM_CLOSE,0,0);
		return FALSE;
	}
	return TRUE;
}