#pragma once

enum
{
	HARMONIC_GRAPH,
	PROFILE_GRAPH
};

//链表类型
struct LNode
{
	TCHAR data[MAXSIZE];		//存储样本名称（样本名称最大字符数为256）
	int num;					//标识每个样本名称的序号
	LNode *next;
};

typedef struct LHead
{
	LNode *next;	//指向下一个LNode
	LNode *tail;	//指向最后一个LNode
} LinkList;


BOOL TabPage_6_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_6_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);
VOID TabPage_6_FillComboBox(HWND hwndComboBox,TCHAR **strings);

BOOL CALLBACK PCA_AddNames_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL PCA_AddNames_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_AddNames_OnNotify(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_AddNames_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_AddNames_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam);

ErrNo PCA_DrawGraph(HWND hParent,double **data,LinkList *List_Names,int n_samples,int n_index,int graph_type = 0,bool draw_all = true,int *sample_num = NULL,int count = 0);
BOOL CALLBACK PCA_DrawGraph_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL PCA_DrawGraph_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_DrawGraph_OnPaint(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL TabPage_6_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_DrawGraph_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL PCA_DrawGraph_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam);

//资源ID
const int OutPut_ResultID = PCA_DATA;
//样本名称输入
const int ListControlID_NAMES = PCA_NAMES_LIST;
const int EditControlID_NAMES = PCA_NAMES;
const int Edit_NAME = PCA_NAME_EDIT;

//画图
const int PICID = PCA_PIC;