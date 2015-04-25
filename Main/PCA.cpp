#include "Computation.h"
#include "PCA.h"

const int EditControlID[] = {PCA_DATA,N_SAMPLE,N_INDEX};

PCA::PCA(HWND hDlg)
{
	hwndDlg = hDlg;
}

// ��ʼ���㣨���ļ���
ErrNo PCA::GetStart_Calc()
{
	ErrNo err;

	// ��ȡ��������
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[1]),&nsamples))
		return err;

	// ��ȡָ�����
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[2]),&nindexs))
		return err;

	// ����ռ�
	if(err = AllocateMemory(matrix_data,nsamples,nindexs))		//������������
		return err;
	if(err = AllocateMemory(matrix_r,nindexs,nindexs))
		return err;
	if(!(eigenvalues = new double [nindexs]))
		return ALLOCATE_MEMORY_FAIL;


	if(err = GetMatrix(GetDlgItem(hwndDlg,EditControlID[0]),matrix_data,nsamples,nindexs))
		return err;

	//if(err = PCA_A(matrix_data,nsamples,nindexs,principal,k))
	//	return err;
	if(err = PCA_A(matrix_data,nsamples,nindexs,matrix_r,eigenvalues,principal,k))
		return err;

	//������
	OutPutResult();

	Destroy(matrix_data,nsamples);
	Destroy(principal,k);
	return 0;
}

ErrNo PCA::AllocateMemory(double **&matrix,int rows,int columns)
{
	if(!(matrix = new double* [rows]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<rows;i++)
	{
		if(!(matrix[i] = new double [columns]))
			return ALLOCATE_MEMORY_FAIL;
	}
	return 0;
}

void PCA::Destroy(double **&matrix,int rows)
{
	for(int i=0;i<rows;i++)
		delete [] matrix[i];
	delete [] matrix;
	matrix = NULL;
}

BOOL  PCA::CharacterJudgement(LPTSTR szBuffer)
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

ErrNo PCA::GetMatrix(const HWND hwndEdit,double **pMatrix,const int nRows,const int nColumns)
{
	TCHAR	szBuffer_t[MAXSIZE];
	bool	data;
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

	nOutput = iStart+nRows;

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

ErrNo PCA::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

void PCA::OutPutResult()
{
	int iCharacterIndex,iLength;
	TCHAR szBuffer[MAXSIZE];
	HWND hEdit = GetDlgItem(hwndDlg,OutPut_ResultID);

	iCharacterIndex = Edit_LineIndex(hEdit,nOutput);
	if(iCharacterIndex == -1)
	{
		iLength = Edit_GetTextLength(hEdit);
		Edit_SetSel(hEdit,iLength,iLength);
		Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	}
	else
		Edit_SetSel(hEdit,iCharacterIndex,iCharacterIndex);

	Edit_ReplaceSel(hEdit,TEXT("������\r\n"));
	_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("��������:%d  ָ�����:%d\r\n"),nsamples,nindexs);
	Edit_ReplaceSel(hEdit,szBuffer);
	Edit_ReplaceSel(hEdit,TEXT("��ؾ���Ϊ:\r\n"));
	for(int i=0;i<nindexs;i++)
	{
		for(int j=0;j<nindexs;j++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%10f\t"),matrix_r[i][j]);
			Edit_ReplaceSel(hEdit,szBuffer);
		}
		Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	}
	Edit_ReplaceSel(hEdit,TEXT("����ֵΪ(�Ӵ�С):\r\n"));
	for(int i=0;i<nindexs;i++)
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("a%d=%f  "),i+1,eigenvalues[i]);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("���ɷָ���:%d\r\n"),k);
	Edit_ReplaceSel(hEdit,szBuffer);
	for(int i=0;i<k;i++)
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("��%d���ɷ�: "),i);
		Edit_ReplaceSel(hEdit,szBuffer);
		for(int j=0;j<nindexs;j++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%f  "),principal[i][j]);
			Edit_ReplaceSel(hEdit,szBuffer);
		}
		Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	}
	Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	Edit_ReplaceSel(hEdit,TEXT("++++++++++++++++++++"));
}