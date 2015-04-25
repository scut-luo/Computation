/*
	主成分分析
*/

#include "Computation.h"
#include "PCA.h"


TCHAR	*Graph_Type[] = { TEXT("1.调和曲线图"),
						  TEXT("2.轮廓图"),0};

const int EditControlID[] = {PCA_DATA,SAMPLE_NUM,N_SAMPLE,N_INDEX};
const int ComboBoxID = PCA_GRAPH_TYPE;
const int CheckBoxID = ALL_CHECK;

extern HWND hwndGrph;		//画图所在的非模态对话框句柄
extern LinkList *List_Names;

//static int n_samples,n_index;
//double **data = NULL;

static ErrNo GetLine_Int(HWND hwndEdit,int &nParameter);
static ErrNo GetMatrix(const HWND hwndEdit,double **pMatrix,const int nRows,const int nColumns);
static ErrNo StringToNumbericArr(PCTSTR szBuffer,PCTSTR seps,int *n_arr,const int n);
static void QuickSort(int *n_arr,int s,int t);

BOOL CALLBACK TabPage_Proc_6(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_6_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_6_OnContextMenu);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_6_OnCommand);
	}
	return FALSE;
}

BOOL TabPage_6_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	TabPage_6_FillComboBox(GetDlgItem(hDlg,ComboBoxID),Graph_Type);

	// 限定输入字符个数
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[1]),255);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[2]),5);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[3]),5);

	Button_SetCheck(GetDlgItem(hDlg,CheckBoxID),BST_CHECKED);
	Edit_SetReadOnly(GetDlgItem(hDlg,EditControlID[1]),TRUE);

	Edit_SetText(GetDlgItem(hDlg,EditControlID[0]),TEXT("数据输入"));
	return TRUE;
}

BOOL TabPage_6_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	HWND hEdit;

	hEdit = GetDlgItem(hDlg,EditControlID[0]);

	switch(LOWORD(wParam))
	{
		//开始计算（无文本）
	case PCA_START:
		{
			Exception excption;
			ErrNo err;

			PCA pca(hDlg);
			if(err = pca.GetStart_Calc())
			{
				excption.ErrorReport(hDlg,err);
				return FALSE;
			}
		}
		return TRUE;

		// 清除样本数据
	case PCA_DATA_CLEAR:
		Edit_SetText(hEdit,TEXT(""));
		break;

	case PCA_DRAW:
		{
			PCTSTR pErrInfo;

			int n_samples,n_index;
			double **data = NULL;

			TCHAR seps[] = TEXT(" ,\t");
			TCHAR szBuffer[MAXSIZE];
			int graph_type,*sample_num,ncount = 0,nLength;
			Exception excption;
			ErrNo err;

			//获取样本个数
			if(err = GetLine_Int(GetDlgItem(hDlg,EditControlID[2]),n_samples))
			{
				excption.ErrorReport(hDlg,err);
				return err;
			}
			//获取指标个数
			if(err = GetLine_Int(GetDlgItem(hDlg,EditControlID[3]),n_index))
			{
				excption.ErrorReport(hDlg,err);
				return err;
			}
			//获取画图类型
			graph_type = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));

			

			//获取样本数据
			//申请空间
			if(!(data = new double* [n_samples]))
			{
				excption.ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
				return ALLOCATE_MEMORY_FAIL;
			}
			for(int i=0;i<n_samples;i++)
			{
				if(!(data[i] = new double [n_index]))
				{
					excption.ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
					return ALLOCATE_MEMORY_FAIL;
				}
			}
			if(err = GetMatrix(GetDlgItem(hDlg,EditControlID[0]),data,n_samples,n_index))
			{
				excption.ErrorReport(hDlg,err);
				return err;
			}

			//判断是否要将全部样本画在图上
			if(Button_GetCheck(GetDlgItem(hDlg,CheckBoxID)) == BST_CHECKED)		//全部都画
			{
				if(err = PCA_DrawGraph(hDlg,data,List_Names,n_samples,n_index,graph_type))
				{
					excption.ErrorReport(hDlg,err);
					return err;
				}
				
			}
			else			//部分画
			{
				//获取要画图的样本数据编号
				Edit_GetText(GetDlgItem(hDlg,EditControlID[1]),szBuffer,MAXSIZE);

				ncount = 0;
				//判断有多少个样本数据编号
				nLength = _tcslen(szBuffer);
				for(int i=0;i<nLength;i++)
				{
					if(szBuffer[i] == TEXT(','))
						ncount++;
				}
				ncount++;

				//申请空间保存要画图的样本标号
				if(!(sample_num = new int [ncount]))
				{
					excption.ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
					return ALLOCATE_MEMORY_FAIL;
				}
				if(err = StringToNumbericArr(szBuffer,seps,sample_num,ncount))
				{
					excption.ErrorReport(hDlg,err);
					return err;
				}
				//判断样本编号是否有误
				for(int i=0;i<ncount;i++)
				{
					if(sample_num[i] > n_samples)
					{
						pErrInfo = excption.FormatError(INCORRECT_DATA);
						_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("错误ID:%d\r\n错误信息:%s\r\n补充信息:%s"),INCORRECT_DATA,pErrInfo,TEXT("样本编号超过样本个数"));
						MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);
						return INCORRECT_DATA;
					}
				}

				QuickSort(sample_num,0,ncount-1);		//从小到大

				if(err = PCA_DrawGraph(hDlg,data,List_Names,n_samples,n_index,graph_type,false,sample_num,ncount))
				{
					excption.ErrorReport(hDlg,err);
					return err;
				}

			}
		}
		return TRUE;

	case PCA_NAME:
		{
			HINSTANCE hInstance;
			//创建模态对话框
			hInstance = GetModuleHandle(NULL);
			DialogBox(hInstance,MAKEINTRESOURCE(IDD_PCA_NAME),hDlg,PCA_AddNames_Proc);
		}
		return TRUE;

	case ALL_CHECK:			//要画图的样本数据要全部画在图上
		{
			if(Button_GetCheck(GetDlgItem(hDlg,CheckBoxID)) == BST_CHECKED)
				Edit_SetReadOnly(GetDlgItem(hDlg,EditControlID[1]),TRUE);
			else
				Edit_SetReadOnly(GetDlgItem(hDlg,EditControlID[1]),FALSE);
		}
		return TRUE;

	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,OutPut_ResultID),TEXT(""));
		return TRUE;

	case RC_HELP:
		{

		}
		return TRUE;
	}
	return FALSE;
}

BOOL TabPage_6_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

VOID TabPage_6_FillComboBox(HWND hwndComboBox,TCHAR **strings)
{
	TCHAR  **pVarName;

	pVarName = strings;

	while(*pVarName)
	{
		SendMessage(hwndComboBox,CB_ADDSTRING,0,(LPARAM)*pVarName);
		pVarName++;
	}
	SendMessage(hwndComboBox,CB_SETCURSEL,0,0);
}

static ErrNo GetLine_Int(HWND hwndEdit,int &nParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	nParameter = _wtoi(Buffer);

	return 0;
}

static BOOL  CharacterJudgement(LPTSTR szBuffer)
{
	int	iLength;

	iLength = lstrlen(szBuffer);
	for(int i=0;i<iLength;i++)
	{
		if(szBuffer[i] == TEXT('.'))
		{
			if(i==0 || i==iLength-1)
				return FALSE;
		}
		else if(szBuffer[i] == TEXT('-'))
		{
			if(i!=0)
				return FALSE;
		}
		else if(szBuffer[i] == TEXT('\t'))
			continue;
		else if( (szBuffer[i]<TEXT('0')) || (szBuffer[i]>TEXT('9')) )
			return FALSE;
	}
	return TRUE;
}

static ErrNo GetMatrix(const HWND hwndEdit,double **pMatrix,const int nRows,const int nColumns)
{
	TCHAR	szBuffer_t[MAXSIZE];
	bool	data = false;
	TCHAR	seps[] = TEXT(" \t");
	TCHAR	*ptemp,*pstr,*pnext_token = NULL,*szBuffer = NULL;
	int		iLine,iLength,iStart,iCount;
	int		iMax = nColumns*20;							//缺憾 获取数据还是有限定的 每行字符个数不能超过iMax个

	szBuffer = new TCHAR[iMax];		//存储每一行获得的字符

	iLine = Edit_GetLineCount(hwndEdit);
	if(iLine == 1)											 //判断是否没有输入数据
	{
		if(!(iLength = Edit_GetText(hwndEdit,szBuffer,iMax)))
		{
			return NO_DATA_INPUT;
		}
	}
	else if(iLine<nRows)
	{
		return WRONG_MATRIX;
	}

	data = false;
	for(int i=0;i<iLine;i++)
	{
		iLength = Edit_GetLine(hwndEdit,i,szBuffer_t,MAXSIZE);
		if(iLength)				//找出第一个有数据的行
		{
			for(int j=0;j<iLength;j++)
			{
				if((szBuffer_t[j] >= TEXT('0') && szBuffer_t[j] <= TEXT('9')) || szBuffer_t[j] == TEXT('.'))
				{
					iStart = i;
					data = true;
					break;
				}
			}
			if(data)
				break;
		}
	}

	for(int i=iStart,k=0;i<iStart+nRows;i++,k++)			//获取每行数据
	{
		iCount = 0;
		iLength = Edit_GetLine(hwndEdit,i,szBuffer,iMax-1);
		szBuffer[iLength]=TEXT('\0');
		//字符串分割
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr!=NULL)
		{
			if(*pstr == TEXT('+'))								//判断是否有分割行							
				return WRONG_MATRIX;	
			else if(!CharacterJudgement(pstr))					//判断是否非法字符输入
				return INCORRECT_DATA;
			if(iCount >= nColumns)								//判断每行输入数据个数是否超出限定矩阵维数
				return WRONG_MATRIX;
			pMatrix[k][iCount] = wcstod(pstr,&ptemp);
			pstr = wcstok_s(NULL,seps,&pnext_token);
			iCount++;
		}
		if(iCount != nColumns)
			return WRONG_MATRIX;
	}

	delete szBuffer;
	szBuffer = NULL;
	return 0;
}

static ErrNo StringToNumbericArr(PCTSTR szBuffer,PCTSTR seps,int *n_arr,const int n)
{
	TCHAR *pstr_t;
	int nLength,nCount;
	TCHAR *pstr,*pnext_token = NULL;

	nLength = _tcslen(szBuffer);
	if(!(pstr_t = new TCHAR [nLength+1]))
		return ALLOCATE_MEMORY_FAIL;

	for(int i=0;i<=nLength;i++)
		pstr_t[i] = szBuffer[i];		//复制字符串

	nCount = 0;
	pstr = wcstok_s(pstr_t,seps,&pnext_token);
	while(pstr != NULL)
	{
		if(!CharacterJudgement(pstr))
			return INCORRECT_DATA;
		if(nCount > n)
			return NOT_MATCH;
		//n_arr[nCount] = wcstod(pstr,&ptemp);
		n_arr[nCount] = _wtoi(pstr);
		pstr = wcstok_s(NULL,seps,&pnext_token);
		nCount++;
	}
	if(nCount != n)
		return NOT_MATCH;

	delete [] pstr_t;
	return 0;
}

//快速排序
static void QuickSort(int *n_arr,int s,int t)
{
	int			i =s,j =t;
	int			ptemp;

	if(s<t)
	{
		ptemp = n_arr[s];
		while(i!=j)
		{
			while(j>i && n_arr[j] >= ptemp)
				j--;
			n_arr[i] = n_arr[j];
			while(i<j && n_arr[i] <= ptemp)
				i++;
			n_arr[j] = n_arr[i];
		}
		n_arr[i] = ptemp;
		QuickSort(n_arr,s,i-1);
		QuickSort(n_arr,i+1,t);
	}
}