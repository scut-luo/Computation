#include "Computation.h"
#include "matrix.h"

TCHAR *Methods[]={TEXT("1.Gauss��Ԫ��"),
						TEXT("2.���Ƿֽⷨ"),
						TEXT("3.Thomas�㷨"),
						TEXT("4.ƽ������"),
						TEXT("5.Jacobi������"),
						TEXT("6.Gauss-Seidel������"),
						TEXT("7.���ɳڵ�����(SOR)"),
						TEXT("8.��������ֵ����"),
						0
};


static WNDPROC		OldFilesListProc;
//����˵��
VOID TabPage_1_FillComboBox(HWND hwndComboBox);
BOOL TabPage_1_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_1_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParm);
BOOL TabPage_1_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm);

static LRESULT CALLBACK FileListProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);			//Edit Control�ؼ���Ϣ������


BOOL CALLBACK TabPage_Proc_1(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,TabPage_1_OnInitDialog);		//��ϢWM_INITDIALOG����
		HANDLE_DLGMSG(hwnd,WM_CONTEXTMENU,wParam,lParam,TabPage_1_OnContextMenu);	//��ϢWM_CONTEXTMENU����
		HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,TabPage_1_OnCommand);			//��ϢWM_COMMAND����
	}
	return FALSE;
}

BOOL TabPage_1_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	TabPage_1_FillComboBox(GetDlgItem(hDlg,ComboBoxID));
	OldFilesListProc = (WNDPROC) SetWindowLong (GetDlgItem(hDlg,FileListID), GWL_WNDPROC,
		(LPARAM) FileListProc) ;

	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[0]),TEXT("0"));
	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[1]),TEXT("0.0001"));
	Edit_SetText(GetDlgItem(hDlg,ITERATIONID[2]),TEXT("1"));

	//�޶������ַ�����
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[0]),10);
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[1]),10);
	Edit_LimitText(GetDlgItem(hDlg,ITERATIONID[2]),10);
	Edit_LimitText(GetDlgItem(hDlg,Edit_Dimension),5);


	return TRUE;
}

BOOL TabPage_1_OnContextMenu(HWND hDlg,WPARAM wParam,LPARAM lParam)
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

BOOL TabPage_1_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParm)
{
	ErrNo err;
	static TCHAR szFilename[MAX_PATH],szTitleName[MAX_PATH];		//����Ҫstatic,��Ȼ���� 
	TCHAR szBuffer[MAX_PATH];
	static int iSelectIndex = 0;
	Matrix_Comp matrix(hDlg);

	switch(LOWORD(wParam))
	{
		//��ʼ����
	case IDC_MATRIX_START:
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
				if(err = matrix.GetStart_File(FilePath,nCount))
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
			else	//�ļ��б���û���ļ�
			{
				iSelectIndex = ComboBox_GetCurSel(GetDlgItem(hDlg,ComboBoxID));
				if(matrix.GetStart(iSelectIndex))
					return FALSE;
			}
		}
		return TRUE;

		// �������ݿ�����ݵ��ļ�����
	case MATRIX_FILE_SAVE:
		if(FileSaveDlg(hDlg,szFilename,szTitleName))
		{
			if(FileWrite(GetDlgItem(hDlg,EditControlID),szFilename))
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

		// ���ļ����浽�ļ��б���
	case MATRIX_FILE_OPEN:
		if(FileOpenDlg(hDlg,szFilename,szTitleName))
		{
			SendMessage(GetDlgItem(hDlg,FileListID),LB_ADDSTRING,0,(LPARAM)szFilename);
		}
		return TRUE;

		//�Ҽ��˵�
	case RC_CLEAR:
		Edit_SetText(GetDlgItem(hDlg,EditControlID),TEXT(""));
		return TRUE;

	case RC_HELP:
		{
			TCHAR szBuffer[] = TEXT("1-4�ŵ��㷨ֻ����������������ݺ�ά��(ϵ������ά��)\n5-8�ŵ��㷨Ҫ������\"����������\"��\"��������\"��\"�ɳ�����\",ͬʱ��������������ݺ�ά��(ϵ������ά��),8���㷨ֻ�����뷽������");
			MessageBox(hDlg,szBuffer,TEXT("����"),MB_OK);
		}
		return TRUE;

	case MATRIX_FILE_HELP:
		{
			TCHAR szBuffer[] = TEXT("�㷨���Ϊ0-7,���ζ�Ӧ��������㷨��\n0 -- Gauss��Ԫ�� ... 7 -- ����ֵ����\n�����ʽ(��������','�ֿ�):\n\t��������,��������,�㷨���,����������,��������,�ɳ�����\n\t��������\n(���ԽǾ�����������,ϵ�������ÿһ���Խ���Ϊһ������,������������Ϊһ��)");
			MessageBox(hDlg,szBuffer,TEXT("����"),MB_OK);
		}
	}
	return FALSE;
}

VOID TabPage_1_FillComboBox(HWND hwndComboBox)
{
	TCHAR	**pVarName;

	pVarName = Methods;

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
