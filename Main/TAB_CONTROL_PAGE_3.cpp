#include "Computation.h"
#include "IAF.h"

static WNDPROC		OldListViewProc_I;				
static WNDPROC		OldListViewProc_F;				//���

TCHAR	*Methods_Interpolation[] = { TEXT("1.Lagrange��ֵ"),
									 TEXT("2.Newton��ֵ"),
									 TEXT("3.Hermite��ֵ"),
									 TEXT("4.�ֶ����Բ�ֵ"),
									 TEXT("5.�ֶ�����Hermite��ֵ"),
									 TEXT("6.����������ֵ-����ط�(��֪���˵�һ�׵���)"),
									 //TEXT("7.����������ֵ-����ط�(��֪���˵���׵���)"),
									 0
};

TCHAR   *Methods_Fitting[] = { TEXT("1.������С�������"),
								   TEXT("2.��������ʽ���������"),
								   TEXT("3.ָ��ģ����С���˽�"),
								   0								    
};

static int nItem,nSubItem;
static int ListViewIDUsing;

static LRESULT CALLBACK ListViewProc_I (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK ListViewProc_F (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//������
BOOL TabPage_3_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_3_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_3_OnNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_3_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);
VOID TabPage_3_FillComboBox(HWND hwndComboBox,TCHAR **Methods);

BOOL CALLBACK TabPage_Proc_3(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_3_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_3_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_NOTIFY,wParam,lParam,TabPage_3_OnNotify);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_3_OnContextMenu);
	}
	return FALSE;
}

BOOL TabPage_3_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	TabPage_3_FillComboBox(GetDlgItem(hDlg,ComboBoxID[0]),Methods_Interpolation);
	TabPage_3_FillComboBox(GetDlgItem(hDlg,ComboBoxID[1]),Methods_Fitting);

	//��ֵ
	LVCOLUMN	vcl_I;
	HWND		hListView_I = GetDlgItem(hDlg,ListViewID_I);

	OldListViewProc_I = (WNDPROC) SetWindowLong (hListView_I, GWL_WNDPROC,
			(LPARAM) ListViewProc_I) ;
	//�޶������ַ���
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_I[0]),15);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_I[1]),15);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_I[2]),15);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_I[3]),15);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_I[4]),5);

	SendMessage(GetDlgItem(hDlg,RadioID[0]),BM_SETCHECK,BST_CHECKED,0);

	vcl_I.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	vcl_I.pszText = TEXT("x");
	vcl_I.cx = 60;
	vcl_I.iSubItem = 0;
	ListView_InsertColumn(hListView_I,0,&vcl_I);

	vcl_I.pszText = TEXT("f(x)");
	vcl_I.cx = 85;
	vcl_I.iSubItem = 1;
	ListView_InsertColumn(hListView_I,1,&vcl_I);

	vcl_I.pszText = TEXT("f'(x)");
	vcl_I.cx = 85;
	vcl_I.iSubItem = 2;
	ListView_InsertColumn(hListView_I,2,&vcl_I);

	vcl_I.pszText = TEXT("f''(x)");
	vcl_I.cx = 85;
	vcl_I.iSubItem = 3;
	ListView_InsertColumn(hListView_I,3,&vcl_I);

	ListView_SetExtendedListViewStyle(hListView_I,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//���
	HWND	hListView_F;
	LVCOLUMN	vcl_F;
	
	hListView_F = GetDlgItem(hDlg,ListViewID_F);
	OldListViewProc_F = (WNDPROC) SetWindowLong (hListView_F, GWL_WNDPROC,
		(LPARAM) ListViewProc_F) ;

	vcl_F.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	vcl_F.pszText = TEXT("x");
	vcl_F.cx = 80;
	vcl_F.iSubItem = 0;
	ListView_InsertColumn(hListView_F,0,&vcl_F);

	vcl_F.pszText = TEXT("f(x)");
	vcl_F.cx = 155;
	vcl_F.iSubItem = 1;
	ListView_InsertColumn(hListView_F,1,&vcl_F);

	ListView_SetExtendedListViewStyle(hListView_F,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �ַ�����
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_F[0]),10);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_F[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_F[2]),255);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_F[3]),10);
	return TRUE;
}

BOOL TabPage_3_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	int						iItem = 0;
	int						iCount,iSelectRadio,iSelectIndex = 0,iLength = 0;
	int						iInputDF = 0,iInputDDF = 0;
	TCHAR					szBuffer[4][MAXINPUT],szBuffer_t[MAXINPUT];
	HWND					hListView_I,hListView_F;
	LVITEM					vitem;
	Exception				exception;
	Character_Judegement	filter;
	InterpolationAndFitting	iAndF(hDlg);

	switch(LOWORD(wParam))
	{
		//��ֵ
	case IDC_POINT_ADD:
		hListView_I = GetDlgItem(hDlg,ListViewID_I);
		vitem.mask = LVIF_TEXT;

		//��ȡxֵ
		if(! Edit_GetText(GetDlgItem(hDlg,EditControlID_I[0]),szBuffer[0],MAXINPUT-1))
		{
			exception.ErrorReport(hDlg,NO_DATA_INPUT);
			return FALSE;
		}
		if(! filter.Judge_Double(szBuffer[0]))
		{
			exception.ErrorReport(hDlg,INCORRECT_DATA);
			return FALSE;
		}
		

		//��ȡf(x)ֵ
		if(! Edit_GetText(GetDlgItem(hDlg,EditControlID_I[1]),szBuffer[1],MAXINPUT-1))
		{
			exception.ErrorReport(hDlg,NO_DATA_INPUT);
			return FALSE;
		}
		if(! filter.Judge_Double(szBuffer[1]))
		{
			exception.ErrorReport(hDlg,INCORRECT_DATA);
			return FALSE;
		}

		//��ȡf'(x)ֵ �������������
		if( Edit_GetText(GetDlgItem(hDlg,EditControlID_I[2]),szBuffer[2],MAXINPUT-1))
		{
			if(! filter.Judge_Double(szBuffer[2]))
			{
				exception.ErrorReport(hDlg,INCORRECT_DATA);
				return FALSE;
			}
			iInputDF = 1;
			
		}
		
		//��ȡf''(x)ֵ �������������
		if( Edit_GetText(GetDlgItem(hDlg,EditControlID_I[3]),szBuffer[3],MAXINPUT-1))
		{
			if(! filter.Judge_Double(szBuffer[3]))
			{
				exception.ErrorReport(hDlg,INCORRECT_DATA);
				return FALSE;
			}
			iInputDDF = 1;
		}

		iItem = ListView_GetItemCount(hListView_I);
		vitem.iItem = iItem;
		vitem.iSubItem = 0;
		vitem.pszText = szBuffer[0];
		ListView_InsertItem(hListView_I,&vitem);

		vitem.iSubItem = 1;
		vitem.pszText = szBuffer[1];
		ListView_SetItem(hListView_I,&vitem);

		if(iInputDF)
		{
			vitem.iSubItem = 2;
			vitem.pszText = szBuffer[2];
			ListView_SetItem(hListView_I,&vitem);
		}
		if(iInputDDF)
		{
			vitem.iSubItem = 3;
			vitem.pszText = szBuffer[3];
			ListView_SetItem(hListView_I,&vitem);
		}

		//������������
		for(int i=0;i<_countof(EditControlID_I)-2;i++)
			Edit_SetText(GetDlgItem(hDlg,EditControlID_I[i]),TEXT(""));
		break;

	case IDC_POINT_RESET:
		hListView_I = GetDlgItem(hDlg,ListViewID_I);
		ListView_DeleteAllItems(hListView_I);
		break;

		//���
	case FITTING_POINT_ADD:
		hListView_F = GetDlgItem(hDlg,ListViewID_F);
		vitem.mask = LVIF_TEXT;

		//��ȡxֵ
		if(! Edit_GetText(GetDlgItem(hDlg,EditControlID_F[0]),szBuffer[0],MAXINPUT))
		{
			exception.ErrorReport(hDlg,NO_DATA_INPUT);
			return FALSE;
		}
		if(! filter.Judge_Double(szBuffer[0]))
		{
			exception.ErrorReport(hDlg,INCORRECT_DATA);
			return FALSE;
		}


		//��ȡf(x)ֵ
		if(! Edit_GetText(GetDlgItem(hDlg,EditControlID_F[1]),szBuffer[1],MAXINPUT))
		{
			exception.ErrorReport(hDlg,NO_DATA_INPUT);
			return FALSE;
		}
		if(! filter.Judge_Double(szBuffer[1]))
		{
			exception.ErrorReport(hDlg,INCORRECT_DATA);
			return FALSE;
		}

		iItem = ListView_GetItemCount(hListView_F);
		vitem.iItem = iItem;
		vitem.iSubItem = 0;
		vitem.pszText = szBuffer[0];
		ListView_InsertItem(hListView_F,&vitem);

		vitem.iSubItem = 1;
		vitem.pszText = szBuffer[1];
		ListView_SetItem(hListView_F,&vitem);

		for(int i=0;i<2;i++)
		{
			Edit_SetText(GetDlgItem(hDlg,EditControlID_F[i]),TEXT(""));
		}
		break;
		
	case FITTING_POINTS_RESET:
		hListView_F = GetDlgItem(hDlg,ListViewID_F);
		ListView_DeleteAllItems(hListView_F);
		break;

		//������
	case IDC_INT_OVER_START:
		iCount = sizeof(RadioID) / sizeof(RadioID[0]);
		for(int i=0;i<iCount;i++)									//���ѡȡ�ķ���
		{
			if(SendMessage(GetDlgItem(hDlg,RadioID[i]),BM_GETCHECK,0,0) == BST_CHECKED)
			{
				iSelectRadio = i;
				break;
			}
		}
		switch(iSelectRadio)
		{
		case 0:									//ѡ���˺�����ֵ
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[0]));
			iAndF.GetStart_I(iSelectIndex);
			break;

		case 1:									//ѡ�����������
			iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID[1]));
			iAndF.GetStart_F(iSelectIndex);
			break;
		}
		break;

		//�Ҽ��˵�����
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,OutPut_ResultID),TEXT(""));
		break;
	}

	//List View�༭��ʧȥ����
	switch(HIWORD(wParam))
	{
	case EN_KILLFOCUS:
		if(LOWORD(wParam) == LVEditControlID)						//�༭�� EditControlʧȥ����
		{
			Edit_GetText(GetDlgItem(hDlg,LVEditControlID),szBuffer_t,MAXINPUT);
			if(!filter.Judge_Double(szBuffer_t))
			{
				return FALSE;
			}
			switch(ListViewIDUsing)
			{
			case ListViewID_I:
				ListView_SetItemText(GetDlgItem(hDlg,ListViewID_I),nItem,nSubItem,szBuffer_t);
				break;
			case ListViewID_F:
				ListView_SetItemText(GetDlgItem(hDlg,ListViewID_F),nItem,nSubItem,szBuffer_t);
				break;
			}
			ShowWindow(GetDlgItem(hDlg,LVEditControlID),SW_HIDE);
		}
		break;
	}
	return TRUE;
}

BOOL TabPage_3_OnNotify(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	POINT				pt;
	RECT				rect,rect_l;
	HWND				hListView,hEdit;
	TCHAR				szBuffer[MAXINPUT];
	LPNMITEMACTIVATE	lpnmitem;

	hEdit = GetDlgItem(hDlg,LVEditControlID);
	
	switch(((LPNMHDR)lParam)->code)
	{
	case NM_DBLCLK:
		lpnmitem = (LPNMITEMACTIVATE)lParam;
		//�ж�˫�������ĸ�List View�ؼ�
		if(lpnmitem->hdr.idFrom == ListViewID_I)
		{
			ListViewIDUsing = ListViewID_I;
			hListView = GetDlgItem(hDlg,ListViewID_I);
		}
		else
		{
			ListViewIDUsing = ListViewID_F;
			hListView = GetDlgItem(hDlg,ListViewID_F);
		}
		nItem = lpnmitem->iItem;					//Get row
		nSubItem = lpnmitem->iSubItem;				//Get column;
		ListView_GetItemText(hListView,nItem,nSubItem,szBuffer,MAXINPUT);
		ListView_GetSubItemRect(hListView,nItem,nSubItem,LVIR_LABEL,&rect);
		GetWindowRect(hListView,&rect_l);
		pt.x = rect_l.left;
		pt.y = rect_l.top;
		ScreenToClient(hDlg,&pt);

		if(nItem != -1)
		{
			MoveWindow(hEdit,pt.x+rect.left,pt.y+rect.top,rect.right-rect.left,rect.bottom-rect.top+1,NULL);
			ShowWindow(hEdit,SW_SHOW);
			Edit_SetText(hEdit,szBuffer);		//����Edit Control����
			Edit_SetSel(hEdit,0,-1);			//ѡ��ȫ������
			SetFocus(hEdit);
		}
		break;

	case NM_CLICK:
		ShowWindow(hEdit,SW_HIDE);
		break;
	}
	return TRUE;
}

BOOL TabPage_3_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

VOID TabPage_3_FillComboBox(HWND hwndComboBox,TCHAR **Methods)
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

//��ֵ
static LRESULT CALLBACK ListViewProc_I (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{		
	int		iPosition;
	int		*iSelectArray = NULL;
	int		iSelectCount = 0;

	switch(message)
	{
	case WM_RBUTTONDOWN:
		iPosition = ListView_GetItemCount(hwnd) -1 ;
		while(iPosition>=0)
		{
			if(ListView_GetItemState(hwnd,iPosition,LVIS_SELECTED) == LVIS_SELECTED)
			{
				ListView_DeleteItem(hwnd,iPosition);
			}
			iPosition--;
		}
		break;
	}
	return CallWindowProc(OldListViewProc_I,hwnd,message,wParam,lParam);
}

//���
static LRESULT CALLBACK ListViewProc_F (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{		
	int		iPosition;
	int		*iSelectArray = NULL;
	int		iSelectCount = 0;

	switch(message)
	{
	case WM_RBUTTONDOWN:
		iPosition = ListView_GetItemCount(hwnd) -1 ;
		while(iPosition>=0)
		{
			if(ListView_GetItemState(hwnd,iPosition,LVIS_SELECTED) == LVIS_SELECTED)
			{
				ListView_DeleteItem(hwnd,iPosition);
			}
			iPosition--;
		}
		break;
	}
	return CallWindowProc(OldListViewProc_F,hwnd,message,wParam,lParam);
}