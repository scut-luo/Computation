#include "Computation.h"

#define	ESM_POKECODEANDLOOKUP (WM_USER+100)

const int	ICO_ID = IDI_ICON;
const int	TAB_ID = IDC_TAB;
const TCHAR szAppName[]=TEXT("��ֵ����");
HINSTANCE		hInst;
TABCTRL			TabCtrl;			//Struct 

VOID Set_Dlg_Ico(HWND hWnd, int idi);			//����ͼ��
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
	LPTSTR tabnames[]={TEXT("������"),TEXT("һԪ�������"),TEXT("������ֵ���"),TEXT("΢���ּ���"),TEXT("��΢�ַ�����ֵ��"),TEXT("���ɷַ���"),0};         //��ҳ��Ŀ����
	LPTSTR dlgnames[]={MAKEINTRESOURCE(TAB_CONTROL_PAGE_1),		 //ÿҳ��Ӧ�Ի����ID
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_2),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_3),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_4),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_5),
					   MAKEINTRESOURCE(TAB_CONTROL_PAGE_6),0};
	Page_Proc tab_control_page_proc[]={TabPage_Proc_1,TabPage_Proc_2,TabPage_Proc_3,TabPage_Proc_4,TabPage_Proc_5,TabPage_Proc_6};

	cx_scr = GetSystemMetrics(SM_CXSCREEN);
	cy_scr = GetSystemMetrics(SM_CYSCREEN);
	
	LoadLibrary(TEXT("Riched20.dll"));  //Ϊ�ڶԻ���ʹ��Rich Edit 2.0���������DLL��
	
	GetWindowRect(hDlg,&rect);
	MoveWindow(hDlg,(cx_scr-rect.right)/2,(cy_scr-rect.bottom)/2,rect.right-rect.left,rect.bottom-rect.top,TRUE);
	Set_Dlg_Ico(hDlg,ICO_ID);									 //����ͼ��

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
		i=SendDlgItemMessage(hDlg,TAB_ID,TCM_GETCURSEL,0,0);								//ͨ�� TCM_GETCURSEL ��Ϣ��ȡ��ǰ��ѡ��Tabҳ
		MoveWindow(TabCtrl.hTabPages[i],rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);			//��Pageiҳ���ص�Ŀ��λ��
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
	PCTSTR title = TEXT("PCA��ͼ");

	if(hwndDlg = FindWindow(NULL,title))
	{
		SendMessage(hwndDlg,WM_CLOSE,0,0);
		return FALSE;
	}
	return TRUE;
}