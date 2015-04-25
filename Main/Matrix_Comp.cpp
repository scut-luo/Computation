#include "Computation.h"
#include "matrix.h"

//const int   ITERATIONID[] = { IDC_ITERATIONS ,IDC_PRECISION ,IDC_RELAXATIONFACTOR};
//const int	EditControlID = IDC_MATRIX_EDIT;
//const int	Edit_Dimension = IDC_DIMENSIONS;


Matrix_Comp::Matrix_Comp(HWND hDlg)
{
	hwndDlg = hDlg;
	iOutput = 0;
	iDimension = 0;
	iterations = 0;							//��������
	relaxationfactor = 1;
	precision = 0.0001;						//��������
	determinant = 1.0f;
	
	iteration_vector = NULL;
	result = NULL;
	matrix = NULL;
	eigenvectors = NULL;
	eigenvectors = NULL;

	result = NULL;
}									//��ʼ��

ErrNo Matrix_Comp::GetStart(int iSelectIndex)							//���㿪ʼ
{
	PCTSTR pErrInfo;
	TCHAR ErrInfo[MAXSIZE];			//��ϸ�Ĵ�����Ϣ
	HWND hwndEdit;
	ErrNo	err;

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);

	iIndex = iSelectIndex;
	determinant = 1.0f;

	//��ȡ����ά��
	if( GetLine_Int(GetDlgItem(hwndDlg,Edit_Dimension),&iDimension))
	{
		MessageBox(hwndDlg,TEXT("δ�������ά��"),TEXT("����"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}

	switch(iSelectIndex)						 //�����㷨������ȡ
	{
	case JACOBI_INDEX:
	case GAUSS_SEIDEL_INDEX:
	case SOR_INDEX:
		if( GetLine_Int(GetDlgItem(hwndDlg,ITERATIONID[0]),&iterations) || GetLine_Double(GetDlgItem(hwndDlg,ITERATIONID[1]),&precision) )
		{
			MessageBox(hwndDlg,TEXT("����������������"),TEXT("����"),MB_ICONERROR);
			return INCORRECT_DATA;
		}
		if(iSelectIndex == SOR_INDEX)
		{
			if( GetLine_Double(GetDlgItem(hwndDlg,ITERATIONID[2]),&relaxationfactor) )
			{
				MessageBox(hwndDlg,TEXT("�ɳ�������������"),TEXT("����"),MB_ICONERROR);
				return INCORRECT_DATA;
			}
			
			if(relaxationfactor <=0 || relaxationfactor >=2)
			{
				MessageBox(hwndDlg,TEXT("�ɳ�������������"),TEXT("����"),MB_ICONERROR);
				return INCORRECT_DATA;
			}
		}
		break;
	}

	if(err = AllocateMemory(iSelectIndex))						//�����ڴ�ռ�
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("����ID:%d\r\n%s"),err,pErrInfo);
		MessageBox(hwndDlg,ErrInfo,TEXT("����"),MB_ICONERROR);
		return err;
	}

	// �������ݻ�ȡ
	switch(iSelectIndex)
	{
	case THOMAS_INDEX:
		/*
		if(err = GetMatrix(hwndEdit,Tridiagonal_Matrix,iOutput,4,iDimension))
		{
			ErrorReport(hwndDlg,err);
			DestroyPointer();
			return err;
		}
		*/
		if(err = GetThreeLine())
		{
			ErrorReport(hwndDlg,err);
			DestroyPointer();
			return err;
		}
		break;
		
	case EIGENVALUES_INDEX:
		if(err = GetMatrix(hwndEdit,matrix,iOutput,iDimension,iDimension))
		{
			ErrorReport(hwndDlg,err);
			DestroyPointer();
			return(err);
		}
		break;

	default:
		if(err = GetMatrix(hwndEdit,matrix,iOutput,iDimension,iDimension+1))
		{
			ErrorReport(hwndDlg,err);
			DestroyPointer();
			return(err);
		}
		break;
	}

	/*
	if(err = Algorithm_Choice(iSelectIndex))
	{
		ErrorReport(hwndDlg,err);
		DestroyPointer();
		return err;
	}
	*/
	err = Algorithm_Choice(iSelectIndex);

	if(err = OutPutResult(err))						//���������
		ErrorReport(hwndDlg,err);

	DestroyPointer();					//����ָ��
	return 0;
}

ErrNo Matrix_Comp::Algorithm_Choice(int iSelectIndex)
{
	Matrix_Algorithm	matrix_algorithm;
	ErrNo				err;
	LARGE_INTEGER		nFreq,nBefore,nAfter;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);

	switch(iSelectIndex)				//�����ĸ��㷨
	{
	case GAUSS_INDEX:								//��˹����Ԫ��Ԫ��
		if(err=matrix_algorithm.Gauss_NOC(matrix,result,determinant,iDimension))
			return err;
		break;	

	case LU_INDEX:								//LU�ֽⷨ
		if(err=matrix_algorithm.LU_NOC(matrix,result,iDimension))
			return err;
		break;

	case THOMAS_INDEX:
		if(err=matrix_algorithm.Thomas_NOC(matrix,result,iDimension))				//׷�Ϸ�(Thomas)
			return err;
		break;

	case CHOLESKY_INDEX:								//ƽ������(Cholesky)
		if(err=matrix_algorithm.Cholesky_NOC(matrix,result,iDimension))
			return err;
		break;

	case JACOBI_INDEX:								//Jacobi����
		if(err=matrix_algorithm.Jacobi(matrix,iteration_vector,result,iDimension,iterations,precision,r_iterations,r_precision))
			return err;
		break;

	case GAUSS_SEIDEL_INDEX:								//Gauss-Seidel����
		if(err=matrix_algorithm.Gauss_Seidel(matrix,iteration_vector,result,iDimension,iterations,precision,r_iterations,r_precision))
			return err;
		break;

	case SOR_INDEX:								//���ɳڵ���(SOR)
		if(err=matrix_algorithm.SOR(matrix,iteration_vector,result,iDimension,iterations,precision,relaxationfactor,r_iterations,r_precision))
			return err;
		break;
	case EIGENVALUES_INDEX:
		//if(err = matrix_algorithm.QR(matrix,eigenvalues,eigenvectors,iDimension))
		if(err = matrix_algorithm.QR_Accelerate(matrix,eigenvalues,iDimension))
			return err;
		break;

	default:
		return NOT_MATCH_ALGORITHM;
	}

	QueryPerformanceCounter(&nAfter);
	runtime = (nAfter.QuadPart - nBefore.QuadPart) * 1000 / (double)nFreq.QuadPart;

	return 0;
}

ErrNo Matrix_Comp::OutPutResult(ErrNo err)
{
	PCTSTR		pErrInfo;
	TCHAR		szBuffer_t[MAXSIZE];

	TCHAR		*szLine;
	TCHAR		*szBuffer;
	int			nMax,nMax_l,nTime;
	int			iCharacterIndex,iLength=0;
	HWND		hwndEdit;

	nTime = 20;
	nMax = 255;
	nMax_l = (nTime)*(iDimension +1);

	szBuffer = new TCHAR[nMax];
	szLine = new TCHAR[nMax_l];

	if(szBuffer == NULL)
		return ALLOCATE_MEMORY_FAIL;

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);

	iCharacterIndex = Edit_LineIndex(hwndEdit,iOutput);
	if(iCharacterIndex == -1)
	{
		iLength = Edit_GetTextLength(hwndEdit);
		Edit_SetSel(hwndEdit,iLength,iLength);
		Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
	}
	else
		Edit_SetSel(hwndEdit,iCharacterIndex,iCharacterIndex);
	

	switch(iIndex)
	{
	case GAUSS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Gauss��Ԫ�� \r\n"));
		break;
	case LU_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ���Ƿֽⷨ \r\n"));
		break;
	case THOMAS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ׷�Ϸ�(Thomas) \r\n"));
		break;
	case CHOLESKY_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ƽ������(Cholesky) \r\n"));
		break;
	case JACOBI_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Jacobi������ \r\n"));
		break;
	case GAUSS_SEIDEL_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Gauss-Seidel������ \r\n"));
		break;
	case SOR_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ���ɳڵ�����(SOR) \r\n"));
		break;
	case EIGENVALUES_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ����ֵ����(���ٵ���QR�㷨) \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("����ID��%d\r\n������Ϣ��%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer_t);
	}
	else
	{
		if(iIndex != EIGENVALUES_INDEX)
		{
			Edit_ReplaceSel(hwndEdit,TEXT("�������Ϊ: "));
			for(int i=0;i<iDimension;i++)
			{
				swprintf(szBuffer,nMax,TEXT("x%d=%f "),i+1,result[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			if(iIndex == GAUSS_INDEX)
			{
				swprintf(szBuffer,nMax,TEXT("���������ʽΪ��%f\r\n"),determinant);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			else if(iIndex >=JACOBI_INDEX)
			{
				_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("����������Ϊ: %d  ��������Ϊ��%f\r\n"),iterations,precision);
				Edit_ReplaceSel(hwndEdit,szBuffer_t);
				_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("�����������������Ϊ��%d  �ﵽ�ĵ�������Ϊ��%f\r\n"),r_iterations,r_precision);
				Edit_ReplaceSel(hwndEdit,szBuffer_t);
				if(iIndex == SOR_INDEX)
				{
					_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("�ɳ�����Ϊ: %f\r\n"),relaxationfactor);
					Edit_ReplaceSel(hwndEdit,szBuffer_t);
				}
			}
		}
		else
		{
			Edit_ReplaceSel(hwndEdit,TEXT("��������ֵΪ: "));
			for(int i=0;i<iDimension;i++)
			{
				swprintf(szBuffer,nMax,TEXT("x%d=%f "),i+1,eigenvalues[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
		}
		
		swprintf(szBuffer,nMax,TEXT("�㷨����ʱ��: %f ����\r\n"),runtime);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}	
	
	Edit_ReplaceSel(hwndEdit,TEXT("+++++++++++++++++++++++++++++++++++++++\r\n"));
	delete [] szBuffer;
	delete [] szLine;
	szBuffer = NULL;
	szLine = NULL;
	return 0;
}

/*
ErrNo Matrix_Comp::GetMatrix()									  //����������ݻ�ȡ
{
	TCHAR	seps[] = TEXT(" \t");
	TCHAR	*ptemp,*pstr,*pnext_token = NULL,*szBuffer = NULL;
	HWND	hwndEdit;
	int		iLine,iLength,iStart,iCount;
	int		iMax = iDimension*20;							//ȱ�� ��ȡ���ݻ������޶��� ÿ���ַ��������ܳ���iMax��

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);
	szBuffer = new TCHAR[iMax];

	iLine = Edit_GetLineCount(hwndEdit);
	if(iLine == 1)											 //�ж��Ƿ�û����������
	{
		if(!(iLength = Edit_GetText(hwndEdit,szBuffer,iMax)))
		{
			return NO_DATA_INPUT;
		}
	}
	else if(iLine<iDimension)
	{
		return WRONG_MATRIX;
	}

	for(int i=0;i<iLine;i++)
	{
		iLength = Edit_GetLine(hwndEdit,i,szBuffer,iMax);
		if(iLength)				//�ҳ���һ�������ݵ���
		{
			iStart = i;
			break;
		}
	}

	iOutput = iStart+iDimension;								//������������ �ھ������һ�У��кŴ�0��ʼ��

	for(int i=iStart,k=0;i<iStart+iDimension;i++,k++)			//��ȡÿ������
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
			if(iCount>iDimension)								//�ж�ÿ���������ݸ����Ƿ񳬳��޶�����ά��
				return WRONG_MATRIX;
			Augmented_Matrix[k][iCount] = wcstod(pstr,&ptemp);
			pstr = wcstok_s(NULL,seps,&pnext_token);
			iCount++;
		}
		if(iCount != iDimension+1)
			return WRONG_MATRIX;
	}

	delete szBuffer;
	szBuffer = NULL;
	return 0;
}
*/

ErrNo Matrix_Comp::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

ErrNo Matrix_Comp::GetLine_Double(HWND hwndEdit,double *fParameter)
{
	TCHAR		*ptemp;
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	if(!CharacterJudgement(Buffer))
		return INCORRECT_DATA;

	*fParameter = fabs(wcstod(Buffer,&ptemp));
	return 0;
}

ErrNo Matrix_Comp::GetThreeLine()
{
	TCHAR		szBuffer_t[MAXSIZE];
	bool		data = false;
	int			iLine,iLength,iStart,iCount;
	int			iMax = iDimension*20;
	TCHAR		seps[] = TEXT(" \t");
	TCHAR		*ptemp,*pstr,*pnext_token = NULL,*szbuffer = NULL;
	HWND		hwndEdit;

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);
	szbuffer = new TCHAR[iMax];

	iLine = Edit_GetLineCount(hwndEdit);
	if(iLine == 1)
	{
		if(!(iLength = Edit_GetText(hwndEdit,szbuffer,iMax)))
			return NO_DATA_INPUT;
	}
	else if(iLine<iDimension)
		return WRONG_MATRIX;

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

	iOutput = iStart +4;							//������������ �ھ������һ�У��кŴ�0��ʼ��	

	for(int i=iStart,k=0;i<iStart+4;i++,k++)		//��ȡÿ������
	{
		iCount = 0;
		iLength = Edit_GetLine(hwndEdit,i,szbuffer,iMax-1);			//ע�����iMax-1
		szbuffer[iLength] = TEXT('\0');
		//�ַ����ָ�
		pstr = wcstok_s(szbuffer,seps,&pnext_token);
		while(pstr!=NULL)
		{
			if(*pstr == TEXT('+'))									//�ж��Ƿ��зָ���
				return WRONG_MATRIX;
			else if(!CharacterJudgement(pstr))						//�ж��Ƿ��зǷ��ַ�����
				return INCORRECT_DATA;

			if(iCount>=iDimension)									//�ж�ÿ���������ݸ����Ƿ񳬳��޶�����ά��
				return WRONG_MATRIX;

			matrix[k][iCount] = wcstod(pstr,&ptemp);
			pstr = wcstok_s(NULL,seps,&pnext_token);
			iCount++;
		}
		if(k==1 || k==3)
		{
			if(iCount != iDimension)
				return WRONG_MATRIX;
		}
		else
		{
			if(iCount != iDimension-1)
				return WRONG_MATRIX;
		}
	}

	delete szbuffer;
	szbuffer = NULL;
	return 0;
}

BOOL  Matrix_Comp::CharacterJudgement(LPTSTR szBuffer)
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

ErrNo Matrix_Comp::AllocateMemory(int iSelectIndex)
{
	ErrNo err;

	//����ռ�,���������
	if(!(result = new double[iDimension]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}

	switch(iSelectIndex)
	{
	case GAUSS_INDEX:
	case LU_INDEX:
	case CHOLESKY_INDEX:
		if(err = AllocateMemory(matrix,iDimension,iDimension+1))		//�������
			return err;
		break;

	case THOMAS_INDEX:
		if(err = AllocateMemory(matrix,4,iDimension))					//���ԽǾ���
			return err;
		break;

	case JACOBI_INDEX:
	case GAUSS_SEIDEL_INDEX:
	case SOR_INDEX:
		if(err = AllocateMemory(matrix,iDimension,iDimension+1))
			return err;

		if(!(iteration_vector = new double[iDimension]))		//������ʼ����
			return ALLOCATE_MEMORY_FAIL;
		for(int i=0;i<iDimension;i++)								//��ʼ��s
		{
			iteration_vector[i] = 0.0;
		}
		break;

	case EIGENVALUES_INDEX:
		if(err = AllocateMemory(matrix,iDimension,iDimension))
			return err;
		if(!(eigenvalues = new double [iDimension]))
			return ALLOCATE_MEMORY_FAIL;
		if(err = AllocateMemory(eigenvectors,iDimension,iDimension))
			return ALLOCATE_MEMORY_FAIL;
		break;
	}
	return 0;
}

ErrNo Matrix_Comp::DestroyPointer()
{
	if(iIndex == THOMAS_INDEX)		//ɾ�����ԽǾ���
		DestroyPointer(matrix,4);
	else if(iIndex == EIGENVALUES_INDEX)
	{
		DestroyPointer(matrix,iDimension);
		DestroyPointer(eigenvalues);
		DestroyPointer(eigenvectors,iDimension);
	}
	else if(iIndex >= JACOBI_INDEX)
	{
		DestroyPointer(matrix,iDimension);
		DestroyPointer(iteration_vector);
	}
	else
	{
		DestroyPointer(matrix,iDimension);
	}

	DestroyPointer(result);
	return 0;
}

//����������
ErrNo Matrix_Comp::GetMatrix(const HWND hwndEdit,double **pMatrix,int &nOutput,const int nRows,const int nColumns)
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

	nOutput = iStart+nRows;								//������������ �ھ������һ�У��кŴ�0��ʼ��

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

ErrNo Matrix_Comp::AllocateMemory(double **&pointer,int nRows,int nColumns)
{
	if(!(pointer = new double* [nRows]))
		return ALLOCATE_MEMORY_FAIL;
	for(int i=0;i<nRows;i++)
	{
		if(!(pointer[i] = new double [nColumns]))
			return ALLOCATE_MEMORY_FAIL;
	}
	return 0;
}

void Matrix_Comp::DestroyPointer(double **&pointer,int nRows)
{
	for(int i=0;i<nRows;i++)
		delete [] pointer[i];
	delete [] pointer;
	pointer = NULL;
}

void Matrix_Comp::DestroyPointer(double *&pointer)
{
	delete [] pointer;
	pointer = NULL;
}


 //�ļ���������
ErrNo Matrix_Comp::GetStart_File(TCHAR **FilesPath,const int nCount)
{
	ErrNo err;
	for(int i=0;i<nCount;i++)
	{
		switch(DetectCodeType(FilesPath[i]))
		{
		case CODE_TYPE_ANSI:
			if(err = GetStart_File_ANSI(FilesPath[i]))
				return err;
			break;

		case CODE_TYPE_UNICODE_LE:
			if(err = GetStart_File_UTF_LE(FilesPath[i]))
				return err;
			break;


		default:		//�򲻿��ļ�
			{
				TCHAR ErrInfo[MAXSIZE+MAX_PATH];
				PCTSTR pErrInfo;						//���ݴ�������õĴ�����Ϣ
				int nMax;

				nMax = MAX_PATH + MAXSIZE;
				
				pErrInfo = FormatError(CAN_NOT_OPEN_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n������Ϣ:%s\r\n�ļ�·��:%s\r\n������Ϣ:�ļ����벻ʶ��"),CAN_NOT_OPEN_FILE,pErrInfo,FilesPath[i]);
				MessageBox(hwndDlg,ErrInfo,TEXT("����"),MB_ICONERROR);
			}
			return CAN_NOT_OPEN_FILE;
		}
	}
	return 0;
}

ErrNo Matrix_Comp::GetStart_File_ANSI(PCTSTR pstrFilePath)
{
	TCHAR seps[] = TEXT(" \t,"),seps_m[] = TEXT(" \t");		//�ָ��
	TCHAR *ptemp,*pstr,*pnext_token = NULL;
	int nCount;		//��������

	TCHAR ErrInfo[MAXSIZE+MAX_PATH];
	PCTSTR pErrInfo;						//���ݴ�������õĴ�����Ϣ
	ErrNo err;
	HWND hEdit;
	bool data;								//�ж��Ƿ�������
	int nLine,nLine_Pre;					//��ȡ���ݶ����ڼ���
	int nLength,ntemp;	
	FILE *fp;
	int nMax;								//ErrInfo����Ĵ�С

	//ÿ�����ݴ洢
	TCHAR *szBuffer;						//��ȡÿ������
	char  szBuffer_t[4096];					//��ȡÿ������
	int	  nLenOfWideChar;

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID);
	_tfopen_s(&fp,pstrFilePath,TEXT("r"));		//ֻ��ģʽ���ļ�

	if(fp == NULL)
		return CAN_NOT_OPEN_FILE;

	nLine = 0;
	while(!feof(fp))
	{
		nLine++;
		//if(_fgetts(szBuffer,4096,fp) == NULL)
		//	break;
		if(fgets(szBuffer_t,4096,fp) == NULL)
			break;
		
		//ת���ַ�
		nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
		szBuffer = new TCHAR [nLenOfWideChar];
		MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

		//�ж��Ƿ���ĳһ��������
		nLength = _tcslen(szBuffer);
		data = false;
		for(int i=0;i<nLength;i++)
		{
			if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
			{
				data = true;	//������
				break;
			}
		}
		if(!data)		//�Ҳ�������
		{
			continue;
		}

		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//��ȡ�㷨����
		nCount = 0;
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr != NULL)
		{
			if(!CharacterJudgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				
				delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//��ȡ��������
				nRows = _wtoi(pstr);
				break;

			case 1:		//��ȡ��������
				nColumns = _wtoi(pstr);
				break;

			case 2:		//��ȡ�㷨���
				algorithm = _wtoi(pstr);
				break;

			case 3:		//����������
				iterations = _wtoi(pstr);
				break;

			case 4:		//��ȡ��������
				precision = wcstod(pstr,&ptemp);
				break;

			case 5:		//��ȡ�ɳ�����
				relaxationfactor = wcstod(pstr,&ptemp);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 3)	//����̫��(����������
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n%s\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;	//ɾ��ת������ַ��洢�ط�(��̬����)

			fclose(fp);
			return NOT_ENOUGH_PARA;
		}
		//�жϲ����Ƿ�����㷨Ҫ��
		err = 0;
		switch(algorithm)
		{
		case GAUSS_INDEX:
		case LU_INDEX:
		case CHOLESKY_INDEX:
			if(nColumns != nRows+1)
				err = NOT_MATCH;
			break;

		case THOMAS_INDEX:
			if(nRows != 4)
				err = NOT_MATCH;
			break;

		case JACOBI_INDEX:
		case GAUSS_SEIDEL_INDEX:
			if((nCount != 5) || (nColumns != nRows+1))
				err = NOT_MATCH;
			break;

		case SOR_INDEX:
			if((nCount != 6) || (nColumns != nRows+1))
				err = NOT_MATCH;
			break;

		case EIGENVALUES_INDEX:
			if(nRows != nColumns)
				err = NOT_MATCH;
			break;

		default:
			err = NOT_MATCH_ALGORITHM;
		}
		if(err)
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID��%d\r\n�ļ�:%s ����%d\r\n������Ϣ:%s\r\n"),NOT_MATCH,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;	//ɾ��ת������ַ��洢�ط�(��̬����)

			fclose(fp);
			return NOT_MATCH;
		}

		delete [] szBuffer;	//ɾ��ת������ַ��洢�ط�(��̬����),����ǰ�������������Ҫ�����Ҫ�ͷ�

		//��ȡ��������
		//����ռ�
		if(algorithm == THOMAS_INDEX)
			iDimension = nColumns;
		else
			iDimension = nRows;
		if(err = AllocateMemory(algorithm))
		{
			pErrInfo = FormatError(ALLOCATE_MEMORY_FAIL);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n������Ϣ:%s\r\n"),ALLOCATE_MEMORY_FAIL,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return ALLOCATE_MEMORY_FAIL;
		}

		//��ȡ��������
		//�ҵ������ݵ�һ��
		nLine_Pre = nLine;
		data = false;
		while(1)
		{
			nLine++;
			//if(_fgetts(szBuffer,4096,fp) == NULL)		
			if(fgets(szBuffer_t,4096,fp) == NULL)
			{
				if(feof(fp))		//�ﵽ�ļ�β
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n����������Ҫ��ľ���ά����ƥ��\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ������%d\r\n��ȡ�ļ����ݳ���\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			else		//���������,�ж��Ƿ�����������
			{

				//ת���ַ�
				nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
				szBuffer = new TCHAR [nLenOfWideChar];
				MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
					{
						data = true;	//������
						break;
					}
				}
				if(!data)		//�Ҳ�������
				{
					delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

					continue;
				}
				break;
			}
		}
		nLength = _tcslen(szBuffer);
		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//���ж�ȡ��������
		if(algorithm == THOMAS_INDEX)		//���ԽǾ����ȡ���⴦��
			ntemp = nColumns -1;
		else
			ntemp = nColumns;

		if(err = StringToNumbericArr(szBuffer,seps_m,matrix[0],ntemp))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}

		delete [] szBuffer;

		for(int i=1;i<nRows;i++)
		{
			nLine++;
			//if(_fgetts(szBuffer,4096,fp) == NULL)
			if(fgets(szBuffer_t,4096,fp) == NULL)
			{
				if(feof(fp))		//�ﵽ�ļ�β
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n����������Ҫ��ľ���ά����ƥ��\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ������%d\r\n��ȡ�ļ����ݳ���\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}

			//ת���ַ�
			nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
			szBuffer = new TCHAR [nLenOfWideChar];
			MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

			nLength = _tcslen(szBuffer);
			if(szBuffer[nLength-1] == TEXT('\n'))
				szBuffer[nLength-1] = TEXT('\0');

			if(algorithm == THOMAS_INDEX)		//���ԽǾ������⴦��
			{
				if(i == 2)
					ntemp = nColumns -1;
				else
					ntemp = nColumns;
			}
			else
				ntemp = nColumns;

			if(err = StringToNumbericArr(szBuffer,seps_m,matrix[i],ntemp))
			{
				pErrInfo = FormatError(err);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);

				delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

				fclose(fp);
				return err;
			}

			delete [] szBuffer;			//ɾ��ת������ַ��洢�ط�(��̬����)
		}


		//ʹ���㷨
		if(err = Algorithm_Choice(algorithm))
		{
			Edit_ReplaceSel(hEdit,TEXT("\r\n�������\r\n"));
			for(int i=0;i<nRows;i++)
			{
				for(int j=0;j<nColumns;j++)
				{
					if(algorithm == THOMAS_INDEX)	//���ԽǾ������⴦��
					{
						if(i == 0 || i == 2)
						{
							if(j == nColumns-1)
								break;
						}
					}
					_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("%10f\t"),matrix[i][j]);
					Edit_ReplaceSel(hEdit,ErrInfo);
				}
				Edit_ReplaceSel(hEdit,TEXT("\r\n"));
			}
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s\r\n������Ϣ:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//���������
		OutPutResult_File();
	}

	fclose(fp);
	return 0;
	return 0;
}

ErrNo Matrix_Comp::GetStart_File_UTF_LE(PCTSTR pstrFilePath)
{
	TCHAR seps[] = TEXT(" \t,"),seps_m[] = TEXT(" \t");		//�ָ��
	TCHAR *ptemp,*pstr,*pnext_token = NULL;
	int nCount;		//��������

	TCHAR ErrInfo[MAXSIZE+MAX_PATH];
	PCTSTR pErrInfo;						//���ݴ�������õĴ�����Ϣ
	ErrNo err;
	HWND hEdit;
	bool data;								//�ж��Ƿ�������
	int nLine,nLine_Pre;					//��ȡ���ݶ����ڼ���
	int nLength,ntemp;	
	TCHAR szBuffer[4096];					//��ȡ�������
	FILE *fp;
	int nMax;								//ErrInfo����Ĵ�С

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID);
	_tfopen_s(&fp,pstrFilePath,TEXT("r,ccs=UNICODE"));		//ֻ��ģʽ���ļ�

	if(fp == NULL)
		return CAN_NOT_OPEN_FILE;

	nLine = 0;
	while(!feof(fp))
	{
		nLine++;
		if(_fgetts(szBuffer,4096,fp) == NULL)
			break;
		
		//�ж��Ƿ���ĳһ��������
		nLength = _tcslen(szBuffer);
		data = false;
		for(int i=0;i<nLength;i++)
		{
			if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
			{
				data = true;	//������
				break;
			}
		}
		if(!data)		//�Ҳ�������
		{
			continue;
		}

		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//��ȡ�㷨����
		nCount = 0;
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr != NULL)
		{
			if(!CharacterJudgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//��ȡ��������
				nRows = _wtoi(pstr);
				break;
				
			case 1:		//��ȡ��������
				nColumns = _wtoi(pstr);
				break;

			case 2:		//��ȡ�㷨���
				algorithm = _wtoi(pstr);
				break;

			case 3:		//����������
				iterations = _wtoi(pstr);
				break;

			case 4:		//��ȡ��������
				precision = wcstod(pstr,&ptemp);
				break;

			case 5:		//��ȡ�ɳ�����
				relaxationfactor = wcstod(pstr,&ptemp);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 3)	//����̫��(����������
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n%s\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_ENOUGH_PARA;
		}
		//�жϲ����Ƿ�����㷨Ҫ��
		err = 0;
		switch(algorithm)
		{
		case GAUSS_INDEX:
		case LU_INDEX:
		case CHOLESKY_INDEX:
			if(nColumns != nRows+1)
				err = NOT_MATCH;
			break;

		case THOMAS_INDEX:
			if(nRows != 4)
				err = NOT_MATCH;
			break;

		case JACOBI_INDEX:
		case GAUSS_SEIDEL_INDEX:
			if((nCount != 5) || (nColumns != nRows+1))
				err = NOT_MATCH;
			break;

		case SOR_INDEX:
			if((nCount != 6) || (nColumns != nRows+1))
				err = NOT_MATCH;
			break;

		case EIGENVALUES_INDEX:
			if(nRows != nColumns)
				err = NOT_MATCH;
			break;

		default:
			err = NOT_MATCH_ALGORITHM;
		}
		if(err)
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID��%d\r\n�ļ�:%s ����%d\r\n������Ϣ:%s\r\n"),NOT_MATCH,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_MATCH;
		}


		//��ȡ��������
		//����ռ�
		if(algorithm == THOMAS_INDEX)
			iDimension = nColumns;
		else
			iDimension = nRows;
		if(err = AllocateMemory(algorithm))
		{
			pErrInfo = FormatError(ALLOCATE_MEMORY_FAIL);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n������Ϣ:%s\r\n"),ALLOCATE_MEMORY_FAIL,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return ALLOCATE_MEMORY_FAIL;
		}

		//��ȡ��������
		//�ҵ������ݵ�һ��
		nLine_Pre = nLine;
		data = false;
		while(1)
		{
			nLine++;
			if(_fgetts(szBuffer,4096,fp) == NULL)		
			{
				if(feof(fp))		//�ﵽ�ļ�β
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n����������Ҫ��ľ���ά����ƥ��\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ������%d\r\n��ȡ�ļ����ݳ���\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			else		//���������,�ж��Ƿ�����������
			{
				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
					{
						data = true;	//������
						break;
					}
				}
				if(!data)		//�Ҳ�������
				{
					continue;
				}
				break;
			}
		}
		nLength = _tcslen(szBuffer);
		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//���ж�ȡ��������
		if(algorithm == THOMAS_INDEX)		//���ԽǾ����ȡ���⴦��
			ntemp = nColumns -1;
		else
			ntemp = nColumns;

		if(err = StringToNumbericArr(szBuffer,seps_m,matrix[0],ntemp))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		for(int i=1;i<nRows;i++)
		{
			nLine++;
			if(_fgetts(szBuffer,4096,fp) == NULL)
			{
				if(feof(fp))		//�ﵽ�ļ�β
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n����������Ҫ��ľ���ά����ƥ��\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ������%d\r\n��ȡ�ļ����ݳ���\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			nLength = _tcslen(szBuffer);
			if(szBuffer[nLength-1] == TEXT('\n'))
				szBuffer[nLength-1] = TEXT('\0');

			if(algorithm == THOMAS_INDEX)		//���ԽǾ������⴦��
			{
				if(i == 2)
					ntemp = nColumns -1;
				else
					ntemp = nColumns;
			}
			else
				ntemp = nColumns;

			if(err = StringToNumbericArr(szBuffer,seps_m,matrix[i],ntemp))
			{
				pErrInfo = FormatError(err);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return err;
			}
		}

		
		//ʹ���㷨
		if(err = Algorithm_Choice(algorithm))
		{
			Edit_ReplaceSel(hEdit,TEXT("\r\n�������\r\n"));
			for(int i=0;i<nRows;i++)
			{
				for(int j=0;j<nColumns;j++)
				{
					if(algorithm == THOMAS_INDEX)	//���ԽǾ������⴦��
					{
						if(i == 0 || i == 2)
						{
							if(j == nColumns-1)
								break;
						}
					}
					_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("%10f\t"),matrix[i][j]);
					Edit_ReplaceSel(hEdit,ErrInfo);
				}
				Edit_ReplaceSel(hEdit,TEXT("\r\n"));
			}
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s\r\n������Ϣ:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//���������
		OutPutResult_File();
	}

	fclose(fp);
	return 0;
}

ErrNo Matrix_Comp::StringToNumbericArr(PCTSTR szBuffer,PCTSTR seps,double *n_arr,const int n)
{
	TCHAR *pstr_t;
	int nLength,nCount;
	TCHAR *ptemp,*pstr,*pnext_token = NULL;

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
		n_arr[nCount] = wcstod(pstr,&ptemp);
		pstr = wcstok_s(NULL,seps,&pnext_token);
		nCount++;
	}
	if(nCount != n)
		return NOT_MATCH;

	delete [] pstr_t;
	return 0;
}

void Matrix_Comp::OutPutResult_File()
{
	TCHAR szBuffer[MAXSIZE];
	HWND hwndEdit = GetDlgItem(hwndDlg,EditControlID);

	//�������
	//����ռ�
	Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
	for(int i=0;i<nRows;i++)
	{
		for(int j=0;j<nColumns;j++)
		{
			if(algorithm == THOMAS_INDEX)	//���ԽǾ������⴦��
			{
				if(i == 0 || i == 2)
				{
					if(j == nColumns-1)
						break;
				}
			}
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("%10f\t"),matrix[i][j]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
		Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
	}

	switch(algorithm)
	{
	case GAUSS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Gauss��Ԫ�� \r\n"));
		break;
	case LU_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ���Ƿֽⷨ \r\n"));
		break;
	case THOMAS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ׷�Ϸ�(Thomas) \r\n"));
		break;
	case CHOLESKY_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ƽ������(Cholesky) \r\n"));
		break;
	case JACOBI_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Jacobi������ \r\n"));
		break;
	case GAUSS_SEIDEL_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: Gauss-Seidel������ \r\n"));
		break;
	case SOR_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ���ɳڵ�����(SOR) \r\n"));
		break;

	case EIGENVALUES_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("��ѡ�㷨: ����ֵ���� \r\n"));
		break;
	}

	if(algorithm != EIGENVALUES_INDEX)
	{
		Edit_ReplaceSel(hwndEdit,TEXT("�������Ϊ: "));
		for(int i=0;i<iDimension;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%f "),i+1,result[i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
		Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
		if(algorithm == GAUSS_INDEX)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("���������ʽΪ��%f\r\n"),determinant);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
		else if(algorithm >=JACOBI_INDEX)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����������Ϊ: %d  ��������Ϊ��%f\r\n"),iterations,precision);
			Edit_ReplaceSel(hwndEdit,szBuffer);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("�����������������Ϊ��%d  �ﵽ�ĵ�������Ϊ��%f\r\n"),r_iterations,r_precision);
			Edit_ReplaceSel(hwndEdit,szBuffer);
			if(algorithm == SOR_INDEX)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("�ɳ�����Ϊ: %f\r\n"),relaxationfactor);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
		}
	}
	else
	{
		Edit_ReplaceSel(hwndEdit,TEXT("��������ֵΪ: "));
		for(int i=0;i<iDimension;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%f "),i+1,eigenvalues[i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
	}
	Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
}