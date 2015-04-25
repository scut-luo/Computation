/*
	���ɷַ���
*/

#include "Computation.h"
#include "PCA.h"


TCHAR	*Graph_Type[] = { TEXT("1.��������ͼ"),
						  TEXT("2.����ͼ"),0};

const int EditControlID[] = {PCA_DATA,SAMPLE_NUM,N_SAMPLE,N_INDEX};
const int ComboBoxID = PCA_GRAPH_TYPE;
const int CheckBoxID = ALL_CHECK;

extern HWND hwndGrph;		//��ͼ���ڵķ�ģ̬�Ի�����
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

	// �޶������ַ�����
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[1]),255);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[2]),5);
	Edit_LimitText(GetDlgItem(hDlg,EditControlID[3]),5);

	Button_SetCheck(GetDlgItem(hDlg,CheckBoxID),BST_CHECKED);
	Edit_SetReadOnly(GetDlgItem(hDlg,EditControlID[1]),TRUE);

	Edit_SetText(GetDlgItem(hDlg,EditControlID[0]),TEXT("��������"));
	return TRUE;
}

BOOL TabPage_6_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	HWND hEdit;

	hEdit = GetDlgItem(hDlg,EditControlID[0]);

	switch(LOWORD(wParam))
	{
		//��ʼ���㣨���ı���
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

		// �����������
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

			//��ȡ��������
			if(err = GetLine_Int(GetDlgItem(hDlg,EditControlID[2]),n_samples))
			{
				excption.ErrorReport(hDlg,err);
				return err;
			}
			//��ȡָ�����
			if(err = GetLine_Int(GetDlgItem(hDlg,EditControlID[3]),n_index))
			{
				excption.ErrorReport(hDlg,err);
				return err;
			}
			//��ȡ��ͼ����
			graph_type = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));

			

			//��ȡ��������
			//����ռ�
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

			//�ж��Ƿ�Ҫ��ȫ����������ͼ��
			if(Button_GetCheck(GetDlgItem(hDlg,CheckBoxID)) == BST_CHECKED)		//ȫ������
			{
				if(err = PCA_DrawGraph(hDlg,data,List_Names,n_samples,n_index,graph_type))
				{
					excption.ErrorReport(hDlg,err);
					return err;
				}
				
			}
			else			//���ֻ�
			{
				//��ȡҪ��ͼ���������ݱ��
				Edit_GetText(GetDlgItem(hDlg,EditControlID[1]),szBuffer,MAXSIZE);

				ncount = 0;
				//�ж��ж��ٸ��������ݱ��
				nLength = _tcslen(szBuffer);
				for(int i=0;i<nLength;i++)
				{
					if(szBuffer[i] == TEXT(','))
						ncount++;
				}
				ncount++;

				//����ռ䱣��Ҫ��ͼ���������
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
				//�ж���������Ƿ�����
				for(int i=0;i<ncount;i++)
				{
					if(sample_num[i] > n_samples)
					{
						pErrInfo = excption.FormatError(INCORRECT_DATA);
						_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n������Ϣ:%s"),INCORRECT_DATA,pErrInfo,TEXT("������ų�����������"));
						MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);
						return INCORRECT_DATA;
					}
				}

				QuickSort(sample_num,0,ncount-1);		//��С����

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
			//����ģ̬�Ի���
			hInstance = GetModuleHandle(NULL);
			DialogBox(hInstance,MAKEINTRESOURCE(IDD_PCA_NAME),hDlg,PCA_AddNames_Proc);
		}
		return TRUE;

	case ALL_CHECK:			//Ҫ��ͼ����������Ҫȫ������ͼ��
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
	int		iMax = nColumns*20;							//ȱ�� ��ȡ���ݻ������޶��� ÿ���ַ��������ܳ���iMax��

	szBuffer = new TCHAR[iMax];		//�洢ÿһ�л�õ��ַ�

	iLine = Edit_GetLineCount(hwndEdit);
	if(iLine == 1)											 //�ж��Ƿ�û����������
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
		if(iLength)				//�ҳ���һ�������ݵ���
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

	for(int i=iStart,k=0;i<iStart+nRows;i++,k++)			//��ȡÿ������
	{
		iCount = 0;
		iLength = Edit_GetLine(hwndEdit,i,szBuffer,iMax-1);
		szBuffer[iLength]=TEXT('\0');
		//�ַ����ָ�
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr!=NULL)
		{
			if(*pstr == TEXT('+'))								//�ж��Ƿ��зָ���							
				return WRONG_MATRIX;	
			else if(!CharacterJudgement(pstr))					//�ж��Ƿ�Ƿ��ַ�����
				return INCORRECT_DATA;
			if(iCount >= nColumns)								//�ж�ÿ���������ݸ����Ƿ񳬳��޶�����ά��
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
		pstr_t[i] = szBuffer[i];		//�����ַ���

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

//��������
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