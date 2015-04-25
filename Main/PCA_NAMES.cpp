/************************************************************************/
/* �����������                                                         */
/************************************************************************/

#include "Computation.h"
#include "PCA.h"

int NumOfNames = 0;			//��ȡ�����������Ƶĸ���(�����ʼֵΪ0)
TCHAR **Array_Names = NULL;	//�洢�������Ƶ�����
LinkList *List_Names;


static WNDPROC OldListViewProc;
static int nItem;
static int nSubItem;

static LRESULT CALLBACK ListViewProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//�����������
BOOL CALLBACK PCA_AddNames_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,PCA_AddNames_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_CLOSE,wParam,lParam,PCA_AddNames_OnClose);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,PCA_AddNames_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_NOTIFY,wParam,lParam,PCA_AddNames_OnNotify);
	}
	return FALSE;
}

BOOL PCA_AddNames_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	int i;
	LNode *s;
	HWND hListView,hEditControl;
	LVCOLUMN vcl;
	LVITEM vitem;
	TCHAR szBuffer[MAXSIZE];
	static bool initial = false;			//�ж�List_Names�Ƿ��ʼ��(����Ϊstatic)

	hListView = GetDlgItem(hDlg,ListControlID_NAMES);
	hEditControl = GetDlgItem(hDlg,EditControlID_NAMES);
	vcl.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	vcl.pszText = TEXT("���");
	vcl.cx = 60;
	vcl.iSubItem = 0;
	ListView_InsertColumn(hListView,0,&vcl);


	vcl.pszText = TEXT("��������");
	vcl.cx = 100;
	vcl.iSubItem = 1;
	ListView_InsertColumn(hListView,1,&vcl);

	ListView_SetExtendedListViewStyle(hListView,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//�����ַ���������
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_NAMES),10);
	Edit_LimitText(GetDlgItem(hDlg,Edit_NAME),10);

	//�����������������
	if(NumOfNames != 0)
	{
		s = List_Names->next;
		i = 1;
		//������������
		while(s != NULL)
		{
			vitem.mask = LVIF_TEXT;

			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%d"),i);
			vitem.iItem = ListView_GetItemCount(hListView);
			vitem.iSubItem = 0;
			vitem.pszText = szBuffer;
			ListView_InsertItem(hListView,&vitem);

			vitem.iSubItem = 1;
			vitem.pszText = s->data;
			ListView_SetItem(hListView,&vitem);

			s = s->next;
			i++;
		}
	}
	else
	{
		NumOfNames = 0;

		if(!initial)		//List_Namesδ��ʼ��
		{
			//��ʼ��������
			List_Names = new LinkList;
			List_Names->next = NULL;
			List_Names->tail = NULL;
			initial = true;		//��ʼ���ɹ�
		}

	}

	OldListViewProc = (WNDPROC) SetWindowLong (GetDlgItem(hDlg,ListControlID_NAMES), GWL_WNDPROC,
		(LPARAM) ListViewProc ) ;
	return TRUE;
}

BOOL PCA_AddNames_OnNotify(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	HWND				hEdit;
	hEdit = GetDlgItem(hDlg,Edit_NAME);

	switch(((LPNMHDR)lParam)->code)
	{
	case NM_DBLCLK:
		{
			TCHAR				szBuffer[MAXINPUT];
			LPNMITEMACTIVATE	lpnmitem;
			HWND				hListView;
			POINT				pt;
			RECT				rect,rect_l;

			hListView = GetDlgItem(hDlg,ListControlID_NAMES);
			lpnmitem = (LPNMITEMACTIVATE)lParam;
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
		}
		return TRUE;

	case NM_CLICK:
		ShowWindow(hEdit,SW_HIDE);
		return TRUE;
	}
	return FALSE;
}

BOOL PCA_AddNames_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case PCA_NAMES_INPUT:
		{
			HWND hListView = GetDlgItem(hDlg, ListControlID_NAMES);
			HWND hEditControl = GetDlgItem(hDlg,EditControlID_NAMES);
			LVITEM vitem;
			TCHAR szBuffer[MAXSIZE],szBuffer_N[MAXSIZE];

			vitem.mask = LVIF_TEXT;
			
			//��ȡ���������
			if(!Edit_GetText(hEditControl,szBuffer,MAXSIZE))
				return FALSE;

			NumOfNames++;
			_sntprintf_s(szBuffer_N,_countof(szBuffer_N),MAXSIZE,TEXT("%d"),NumOfNames);
			vitem.iItem = ListView_GetItemCount(hListView);
			vitem.iSubItem = 0;
			vitem.pszText = szBuffer_N;
			ListView_InsertItem(hListView,&vitem);

			vitem.iSubItem = 1;
			vitem.pszText = szBuffer;
			ListView_SetItem(hListView,&vitem);
			
		}
		return TRUE;
	}

	//List View�༭��ʧȥ����
	switch(HIWORD(wParam))
	{
	case EN_KILLFOCUS:
		{
			TCHAR szBuffer[MAXINPUT];
			if(LOWORD(wParam) == Edit_NAME)						//�༭�� EditControlʧȥ����
			{
				Edit_GetText(GetDlgItem(hDlg,Edit_NAME),szBuffer,MAXINPUT);
				ListView_SetItemText(GetDlgItem(hDlg,ListControlID_NAMES),nItem,nSubItem,szBuffer);
			}
			ShowWindow(GetDlgItem(hDlg,Edit_NAME),SW_HIDE);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL PCA_AddNames_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	//�����������������
	//ʹ������洢

	HWND hListView = GetDlgItem(hDlg,ListControlID_NAMES);
	int nCount,k;
	LNode *s,*r,*pre;
	
	nCount = ListView_GetItemCount(hListView);
	NumOfNames = nCount;

	if(List_Names->next == NULL)		//һ������Ҳû��
	{
		//ʹ��β�巨�������
		//����ͷ�ڵ��������ڵ㲻һ��,��û�д洢�κ�һ����������ʱ,���⴦��
		k = 0;
		if(List_Names->tail == NULL)
		{
			s = new LNode;
			ListView_GetItemText(hListView,0,1,s->data,_countof(s->data));		//��õ�һ�еڶ�������
			s->num = 1;		//���Ϊ1
			s->next = NULL;
			
			List_Names->next = s;
			List_Names->tail = s;
			
			k = 1;
		}
		
		r = List_Names->tail;
		for(int i=k;i<nCount;i++)
		{
			s = new LNode;
			ListView_GetItemText(hListView,i,1,s->data,_countof(s->data));		//�������
			s->num = i+1;
			r->next = s;
			r = s;
		}
		r->next = NULL;
		List_Names->tail = r;
	}
	else	//����������
	{
		pre = r = List_Names->next;
		for(int i=0;i<nCount;i++)
		{
			if(r == NULL)		//�����ڽڵ�
			{
				s = new LNode;
				ListView_GetItemText(hListView,i,1,s->data,_countof(s->data));		//�������
				s->num = i+1;
				pre->next = s;
				pre = s;
				r = NULL;
			}
			else				//�Ѿ����ڽڵ�
			{
				ListView_GetItemText(hListView,i,1,r->data,_countof(r->data));
				r->num = i+1;
				pre = r;
				r = r->next;
			}
		}
	
		if(r == NULL)		//��ʾ���µĽڵ����
		{
			pre->next = NULL;
			List_Names->tail = pre;
		}
		else				//û���µĽڵ����,ɾ�������Ѵ��ڲ��õĽڵ�
		{
			if(nCount == 0)	//Ҫɾ��ȫ������
			{
				r = s = List_Names->next;
				while(s != NULL)
				{
					s = r->next;
					free(r);
					r = s;
				}
				List_Names->next = List_Names->tail = NULL;
			}
			else
			{
				List_Names->tail = pre;
				
				if(pre->next != NULL)
				{
					pre = pre->next;
					r = pre->next;
					
					while(r != NULL)
					{
						delete r;
						pre = r;
						r = pre->next;
					}
					delete pre;
				}
				List_Names->tail->next = NULL;
			}
		}

	}
	
	EndDialog(hDlg,0);
	return TRUE;
}

static LRESULT CALLBACK ListViewProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{		
	int		iPosition;
	int		*iSelectArray = NULL;
	int		nCount = 0;
	TCHAR	szBuffer[MAXSIZE];

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

		//���±��
		nCount = ListView_GetItemCount(hwnd);
		for(int i=0;i<nCount;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%d"),i+1);
			ListView_SetItemText(hwnd,i,0,szBuffer);
		}

		break;
	}
	return CallWindowProc(OldListViewProc,hwnd,message,wParam,lParam);
}
