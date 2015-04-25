#include "Computation.h"
#include "PCA.h"

const int EditControlID[] = {PCA_DATA,N_SAMPLE,N_INDEX};

PCA::PCA(HWND hDlg)
{
	hwndDlg = hDlg;
}

// 开始计算（非文件）
ErrNo PCA::GetStart_Calc()
{
	ErrNo err;

	// 获取样本个数
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[1]),&nsamples))
		return err;

	// 获取指标个数
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[2]),&nindexs))
		return err;

	// 申请空间
	if(err = AllocateMemory(matrix_data,nsamples,nindexs))		//保存样本数据
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

	//输出结果
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

	nOutput = iStart+nRows;

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

	Edit_ReplaceSel(hEdit,TEXT("计算结果\r\n"));
	_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("样本个数:%d  指标个数:%d\r\n"),nsamples,nindexs);
	Edit_ReplaceSel(hEdit,szBuffer);
	Edit_ReplaceSel(hEdit,TEXT("相关矩阵为:\r\n"));
	for(int i=0;i<nindexs;i++)
	{
		for(int j=0;j<nindexs;j++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%10f\t"),matrix_r[i][j]);
			Edit_ReplaceSel(hEdit,szBuffer);
		}
		Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	}
	Edit_ReplaceSel(hEdit,TEXT("特征值为(从大到小):\r\n"));
	for(int i=0;i<nindexs;i++)
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("a%d=%f  "),i+1,eigenvalues[i]);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	Edit_ReplaceSel(hEdit,TEXT("\r\n"));
	_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("主成分个数:%d\r\n"),k);
	Edit_ReplaceSel(hEdit,szBuffer);
	for(int i=0;i<k;i++)
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("第%d主成分: "),i);
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