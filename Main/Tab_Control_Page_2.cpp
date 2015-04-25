#include "Computation.h"
#include "equation.h"

TCHAR	*Methods_FX[] = { TEXT("1.���������"),
					   TEXT("2.Newton����"),
					   TEXT("3.Aitken���ٷ���"),
					   TEXT("4.Steffensen����"),
					   0
};

static WNDPROC	OldFilesListProc;

BOOL TabPage_2_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_2_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);
VOID TabPage_2_FillComboBox(HWND hwndComboBox);
BOOL TabPage_2_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam);

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK TabPage_Proc_2(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_2_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_2_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_2_OnContextMenu);
	}
	return FALSE;
}

BOOL TabPage_2_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	OldFilesListProc = (WNDPROC) SetWindowLong (GetDlgItem(hDlg,FileListID), GWL_WNDPROC,
		(LPARAM) FileListProc) ;

	TabPage_2_FillComboBox(GetDlgItem(hDlg,ComboBoxID));

	Edit_SetText(GetDlgItem(hDlg,InitialValueID[0]),TEXT("x"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[1]),TEXT("0"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[2]),TEXT("0.000001"));
	Edit_SetText(GetDlgItem(hDlg,InitialValueID[3]),TEXT("0.0"));

	//�޶������ַ�����
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[0]),1);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[2]),10);
	Edit_LimitText(GetDlgItem(hDlg,InitialValueID[3]),10);

	Edit_LimitText(GetDlgItem(hDlg,IDC_ITERATIVE_FORMULA),MAXINPUT);
	Edit_LimitText(GetDlgItem(hDlg,IDC_FUNCTION_EXP),MAXINPUT);

	return TRUE;
}

BOOL TabPage_2_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	Exception			exception;
	TCHAR				ErrInfo[MAXSIZE];
	PCTSTR				pErrInfo;

	ErrNo				err;
	int					iLength;
	TCHAR				szFunExp[MAXINPUT],szBuffer[MAXINPUT];
	static TCHAR		szFilename[MAX_PATH],szTitleName[MAX_PATH];		//����Ҫstatic,��Ȼ���� 

	int					iSelectIndex = 0;
	Equation			equation(hDlg);

	switch(LOWORD(wParam))
	{
	case IDC_EQUATION_START:
		{
			HWND hFileList;
			TCHAR **FilePath;
			int nCount;

			hFileList = GetDlgItem(hDlg,FileListID);
			//�ж��Ƿ����ļ���ӵ��ļ��б���,������ȼ����ļ��е�����
			nCount = SendMessage(hFileList,LB_GETCOUNT,0,0);
			if(nCount >= 1)	//������ļ���ӵ��ļ��б���
			{
				//����ռ䱣���ļ�·��
				if(!(FilePath = new TCHAR* [nCount]))
					return FALSE;
				for(int i=0;i<nCount;i++)
				{
					if(!(FilePath[i] = new TCHAR [MAX_PATH]))
						return FALSE;
				}

				//��ȡ�ļ��б�
				for(int i=0;i<nCount;i++)
				{
					SendMessage(hFileList,LB_GETTEXT,i,(LPARAM)FilePath[i]);
				}

				//���ļ���ȡ���ݿ�ʼ����
				if(err = equation.GetStart_File(FilePath,nCount))
				{
					for(int i=0;i<nCount;i++)
						delete [] FilePath[i];
					delete [] FilePath;
					return FALSE;
				}

				//ɾ������ռ�
				for(int i=0;i<nCount;i++)
					delete [] FilePath[i];
				delete [] FilePath;
			}
			else
			{
				iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));
				equation.GetStart(iSelectIndex);									//���㿪ʼ
			}
		}
		return TRUE;

	case IDC_FORMULA_HELP:
		MessageBox(hDlg,TEXT("��ʽ�п�������\"+ - * \\ sin cos tan ln log ^ e\",����Ҫ��ȫƥ�� \n��ʽ���Գ���sin3����sin(3)д��,��������\"ln4x\"������д���Ǵ����,Ӧд��ln(4*x)[xΪ��������],��ʽ�г��ֵı�������Ҫ���������趨��һ��,������ִ���"),TEXT("����"),MB_OK);
		return TRUE;

	case IDC_GRAPH:
		if(!(iLength = Edit_GetText(GetDlgItem(hDlg,FunExpID),szFunExp,MAXINPUT-1)))
		{
			MessageBox(hDlg,TEXT("δ������Ҫ���ĺ������ʽ"),TEXT("����"),MB_ICONERROR);
			return FALSE;
		}
		if(!(iLength = Edit_GetText(GetDlgItem(hDlg,InitialValueID[0]),szBuffer,MAXINPUT-1)))							//��ȡ��������
		{
			MessageBox(hDlg,TEXT("δ�����������"),TEXT("����"),MB_ICONERROR);
			return FALSE;
		}

		//�ж��Ƿ�������ȷ�ַ�
		if(!(( szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z')) || ( szBuffer[0]>=TEXT('a') && szBuffer[0]<=TEXT('z'))))
		{
			pErrInfo = exception.FormatError(INCORRECT_DATA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n������Ϣ:���������������"),INCORRECT_DATA,pErrInfo);
			MessageBox(hDlg,ErrInfo,TEXT("����"),MB_ICONERROR);
			return INCORRECT_DATA;
		}

		//��д��Сд
		if(szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z'))
			szBuffer[0] += 32;
		DrawGraph(hDlg,szFunExp,szBuffer[0]);
		return TRUE;

		//�������������
	case EQ_SAVE:
		if(FileSaveDlg(hDlg,szFilename,szTitleName))
		{
			if(FileWrite(GetDlgItem(hDlg,EditControlID[6]),szFilename))
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

	case EQ_OPEN:
		if(FileOpenDlg(hDlg,szFilename,szTitleName))
		{
			SendMessage(GetDlgItem(hDlg,FileListID),LB_ADDSTRING,0,(LPARAM)szFilename);
		}
		return TRUE;

		//�Ҽ��˵�
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,EditControlID[6]),TEXT(""));
		return TRUE;

	case RC_HELP:
		{
			TCHAR szBuffer[1024] = TEXT("��������ʽʱ,����ֻ��һ��,Ĭ�Ϸ���Ϊ'x',��������ʽ֮ǰҪȷ����������Ҫ�����õ�һ�¡�");
			MessageBox(hDlg,szBuffer,TEXT("����"),MB_OK);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL TabPage_2_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

VOID TabPage_2_FillComboBox(HWND hwndComboBox)
{
	TCHAR	**pVarName;

	pVarName = Methods_FX;

	while(*pVarName)
	{
		SendMessage(hwndComboBox,CB_ADDSTRING,0,(LPARAM)*pVarName);
		pVarName++;
	}
	SendMessage(hwndComboBox,CB_SETCURSEL,0,0);
}

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	return CallWindowProc(OldFilesListProc,hwnd,message,wParam,lParam);
}
