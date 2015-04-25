/************************************************************************/
/* 添加样本名称                                                         */
/************************************************************************/

#include "Computation.h"
#include "PCA.h"

int NumOfNames = 0;			//获取输入样本名称的个数(必须初始值为0)
TCHAR **Array_Names = NULL;	//存储样本名称的数组
LinkList *List_Names;


static WNDPROC OldListViewProc;
static int nItem;
static int nSubItem;

static LRESULT CALLBACK ListViewProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//添加样本名字
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
	static bool initial = false;			//判断List_Names是否初始化(必须为static)

	hListView = GetDlgItem(hDlg,ListControlID_NAMES);
	hEditControl = GetDlgItem(hDlg,EditControlID_NAMES);
	vcl.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	vcl.pszText = TEXT("序号");
	vcl.cx = 60;
	vcl.iSubItem = 0;
	ListView_InsertColumn(hListView,0,&vcl);


	vcl.pszText = TEXT("样本名称");
	vcl.cx = 100;
	vcl.iSubItem = 1;
	ListView_InsertColumn(hListView,1,&vcl);

	ListView_SetExtendedListViewStyle(hListView,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//输入字符个数限制
	Edit_LimitText(GetDlgItem(hDlg,EditControlID_NAMES),10);
	Edit_LimitText(GetDlgItem(hDlg,Edit_NAME),10);

	//如果已输入样本名称
	if(NumOfNames != 0)
	{
		s = List_Names->next;
		i = 1;
		//输出链表的数据
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

		if(!initial)		//List_Names未初始化
		{
			//初始化单链表
			List_Names = new LinkList;
			List_Names->next = NULL;
			List_Names->tail = NULL;
			initial = true;		//初始化成功
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
				Edit_SetText(hEdit,szBuffer);		//设置Edit Control内容
				Edit_SetSel(hEdit,0,-1);			//选中全部字体
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
			
			//获取输入的内容
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

	//List View编辑框失去焦点
	switch(HIWORD(wParam))
	{
	case EN_KILLFOCUS:
		{
			TCHAR szBuffer[MAXINPUT];
			if(LOWORD(wParam) == Edit_NAME)						//编辑后 EditControl失去焦点
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
	//保存输入的样本名称
	//使用链表存储

	HWND hListView = GetDlgItem(hDlg,ListControlID_NAMES);
	int nCount,k;
	LNode *s,*r,*pre;
	
	nCount = ListView_GetItemCount(hListView);
	NumOfNames = nCount;

	if(List_Names->next == NULL)		//一个数据也没有
	{
		//使用尾插法添加数据
		//由于头节点与其他节点不一致,当没有存储任何一个样本名称时,特殊处理
		k = 0;
		if(List_Names->tail == NULL)
		{
			s = new LNode;
			ListView_GetItemText(hListView,0,1,s->data,_countof(s->data));		//获得第一行第二个数据
			s->num = 1;		//序号为1
			s->next = NULL;
			
			List_Names->next = s;
			List_Names->tail = s;
			
			k = 1;
		}
		
		r = List_Names->tail;
		for(int i=k;i<nCount;i++)
		{
			s = new LNode;
			ListView_GetItemText(hListView,i,1,s->data,_countof(s->data));		//获得数据
			s->num = i+1;
			r->next = s;
			r = s;
		}
		r->next = NULL;
		List_Names->tail = r;
	}
	else	//链表有数据
	{
		pre = r = List_Names->next;
		for(int i=0;i<nCount;i++)
		{
			if(r == NULL)		//不存在节点
			{
				s = new LNode;
				ListView_GetItemText(hListView,i,1,s->data,_countof(s->data));		//获得数据
				s->num = i+1;
				pre->next = s;
				pre = s;
				r = NULL;
			}
			else				//已经存在节点
			{
				ListView_GetItemText(hListView,i,1,r->data,_countof(r->data));
				r->num = i+1;
				pre = r;
				r = r->next;
			}
		}
	
		if(r == NULL)		//表示有新的节点加入
		{
			pre->next = NULL;
			List_Names->tail = pre;
		}
		else				//没有新的节点加入,删除后面已存在不用的节点
		{
			if(nCount == 0)	//要删除全部数据
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

		//从新标号
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
