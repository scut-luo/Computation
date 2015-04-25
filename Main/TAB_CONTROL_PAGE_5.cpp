/*
	��΢�ַ��̼���
*/
#include "Computation.h"
#include "ODE.h"

TCHAR	*Methods_ODE[] = { TEXT("1.Euler����"),
						   TEXT("2.�Ľ���Euler����"),
						   TEXT("3.4��Runge-Kutta����"),
						   TEXT("4.4��AdamsԤ��-У����ʽ(���׸�ʽ)"),
						   TEXT("5.������Milne-HammingԤ��-У����ʽ"),
						   TEXT("6.Adams4��4����ʽ��ʽ"),
						   TEXT("7.Milne4��4����ʽ��ʽ"),
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
				MessageBox(hDlg,TEXT("���ݱ���ɹ�"),TEXT("�ļ�����"),MB_OK);
				return TRUE;
			}
			else
			{
				swprintf(szBuffer,MAX_PATH,TEXT("Could not write fils as %s"),szTitleName);
				MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);
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

	//��ȡ����һ�ʱ������
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	//��ȡ�ͻ�����С
	GetClientRect(hDlg,&rect);
	//����Ļ����תΪ�ͻ�������
	ScreenToClient(hDlg,&pt);
	//�жϵ��Ƿ�λ�ڿͻ�������
	if(PtInRect(&rect,pt))
	{

		//���ز˵���Դ
		hroot = LoadMenu((HINSTANCE)GetWindowLongPtr(hDlg,GWLP_HINSTANCE),MAKEINTRESOURCE(RC_MENU));

		if(hroot)
		{
			//��ȡ��һ�������˵�
			hpop = GetSubMenu(hroot,0);

			ClientToScreen(hDlg,&pt);

			//�����������ĳЩ�˵����Ƿ񱻽���

			//��ʾ��ݲ˵�
			TrackPopupMenu(hpop,
				TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
				pt.x,
				pt.y,
				0,
				hDlg,
				NULL);
			//���ٲ˵���Դ
			DestroyMenu(hroot);
		}
	}
	else
	{
		//��ʾϵͳ�˵����ڱ�������
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