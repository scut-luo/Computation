/*
	΢������ֵ����
*/
#include "Computation.h"
#include "Calculus.h"

TCHAR	*Methods_Differential[] = { TEXT("1.����Taylor��ʽ����ֵ΢�ֹ�ʽ-��ǰ����"),
									TEXT("2.����Taylor��ʽ����ֵ΢�ֹ�ʽ-������"),
									TEXT("3.����Taylor��ʽ����ֵ΢�ֹ�ʽ-���Ĳ���"),
									TEXT("4.����Taylor��ʽ����ֵ΢�ֹ�ʽ-�������Ĳ���"),
									0
};

TCHAR	*Methods_Integral[] = {	TEXT("1.Romberg�㷨"),
								TEXT("2.Gauss-Legendre�����ʽ(5��ʽ)"),
								TEXT("3.Gauss-Chebyshev�����ʽ(5��ʽ)"),
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

	//΢��	
	//�޶������ַ�����
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
		case 0:						//΢�ּ���
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[0]));
			calculus.GetStart_D(iSelectIndex);
			break;
		case 1:						//���ּ���
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[1]));
			calculus.GetStart_I(iSelectIndex);
			break;	
		}
		return TRUE;

		//�Ҽ��˵���Ϣ����
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