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
	iterations = 0;							//迭代次数
	relaxationfactor = 1;
	precision = 0.0001;						//迭代精度
	determinant = 1.0f;
	
	iteration_vector = NULL;
	result = NULL;
	matrix = NULL;
	eigenvectors = NULL;
	eigenvectors = NULL;

	result = NULL;
}									//初始化

ErrNo Matrix_Comp::GetStart(int iSelectIndex)							//计算开始
{
	PCTSTR pErrInfo;
	TCHAR ErrInfo[MAXSIZE];			//详细的错误信息
	HWND hwndEdit;
	ErrNo	err;

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);

	iIndex = iSelectIndex;
	determinant = 1.0f;

	//获取矩阵维数
	if( GetLine_Int(GetDlgItem(hwndDlg,Edit_Dimension),&iDimension))
	{
		MessageBox(hwndDlg,TEXT("未输入矩阵维数"),TEXT("错误"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}

	switch(iSelectIndex)						 //迭代算法参数获取
	{
	case JACOBI_INDEX:
	case GAUSS_SEIDEL_INDEX:
	case SOR_INDEX:
		if( GetLine_Int(GetDlgItem(hwndDlg,ITERATIONID[0]),&iterations) || GetLine_Double(GetDlgItem(hwndDlg,ITERATIONID[1]),&precision) )
		{
			MessageBox(hwndDlg,TEXT("迭代参数输入有误"),TEXT("错误"),MB_ICONERROR);
			return INCORRECT_DATA;
		}
		if(iSelectIndex == SOR_INDEX)
		{
			if( GetLine_Double(GetDlgItem(hwndDlg,ITERATIONID[2]),&relaxationfactor) )
			{
				MessageBox(hwndDlg,TEXT("松弛因子输入有误"),TEXT("错误"),MB_ICONERROR);
				return INCORRECT_DATA;
			}
			
			if(relaxationfactor <=0 || relaxationfactor >=2)
			{
				MessageBox(hwndDlg,TEXT("松弛因子输入有误"),TEXT("错误"),MB_ICONERROR);
				return INCORRECT_DATA;
			}
		}
		break;
	}

	if(err = AllocateMemory(iSelectIndex))						//申请内存空间
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("错误ID:%d\r\n%s"),err,pErrInfo);
		MessageBox(hwndDlg,ErrInfo,TEXT("错误"),MB_ICONERROR);
		return err;
	}

	// 矩阵数据获取
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

	if(err = OutPutResult(err))						//输出计算结果
		ErrorReport(hwndDlg,err);

	DestroyPointer();					//销毁指针
	return 0;
}

ErrNo Matrix_Comp::Algorithm_Choice(int iSelectIndex)
{
	Matrix_Algorithm	matrix_algorithm;
	ErrNo				err;
	LARGE_INTEGER		nFreq,nBefore,nAfter;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);

	switch(iSelectIndex)				//调用哪个算法
	{
	case GAUSS_INDEX:								//高斯列主元消元法
		if(err=matrix_algorithm.Gauss_NOC(matrix,result,determinant,iDimension))
			return err;
		break;	

	case LU_INDEX:								//LU分解法
		if(err=matrix_algorithm.LU_NOC(matrix,result,iDimension))
			return err;
		break;

	case THOMAS_INDEX:
		if(err=matrix_algorithm.Thomas_NOC(matrix,result,iDimension))				//追赶法(Thomas)
			return err;
		break;

	case CHOLESKY_INDEX:								//平凡根法(Cholesky)
		if(err=matrix_algorithm.Cholesky_NOC(matrix,result,iDimension))
			return err;
		break;

	case JACOBI_INDEX:								//Jacobi迭代
		if(err=matrix_algorithm.Jacobi(matrix,iteration_vector,result,iDimension,iterations,precision,r_iterations,r_precision))
			return err;
		break;

	case GAUSS_SEIDEL_INDEX:								//Gauss-Seidel迭代
		if(err=matrix_algorithm.Gauss_Seidel(matrix,iteration_vector,result,iDimension,iterations,precision,r_iterations,r_precision))
			return err;
		break;

	case SOR_INDEX:								//超松弛迭代(SOR)
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
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Gauss消元法 \r\n"));
		break;
	case LU_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 三角分解法 \r\n"));
		break;
	case THOMAS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 追赶法(Thomas) \r\n"));
		break;
	case CHOLESKY_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 平方根法(Cholesky) \r\n"));
		break;
	case JACOBI_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Jacobi迭代法 \r\n"));
		break;
	case GAUSS_SEIDEL_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Gauss-Seidel迭代法 \r\n"));
		break;
	case SOR_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 超松弛迭代法(SOR) \r\n"));
		break;
	case EIGENVALUES_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 特征值计算(加速单步QR算法) \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("错误ID：%d\r\n错误信息：%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer_t);
	}
	else
	{
		if(iIndex != EIGENVALUES_INDEX)
		{
			Edit_ReplaceSel(hwndEdit,TEXT("方程组解为: "));
			for(int i=0;i<iDimension;i++)
			{
				swprintf(szBuffer,nMax,TEXT("x%d=%f "),i+1,result[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
			if(iIndex == GAUSS_INDEX)
			{
				swprintf(szBuffer,nMax,TEXT("矩阵的行列式为：%f\r\n"),determinant);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			else if(iIndex >=JACOBI_INDEX)
			{
				_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("最大迭代次数为: %d  迭代精度为：%f\r\n"),iterations,precision);
				Edit_ReplaceSel(hwndEdit,szBuffer_t);
				_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("迭代结束后迭代次数为：%d  达到的迭代精度为：%f\r\n"),r_iterations,r_precision);
				Edit_ReplaceSel(hwndEdit,szBuffer_t);
				if(iIndex == SOR_INDEX)
				{
					_sntprintf_s(szBuffer_t,_countof(szBuffer_t),MAXSIZE,TEXT("松弛因子为: %f\r\n"),relaxationfactor);
					Edit_ReplaceSel(hwndEdit,szBuffer_t);
				}
			}
		}
		else
		{
			Edit_ReplaceSel(hwndEdit,TEXT("方程特征值为: "));
			for(int i=0;i<iDimension;i++)
			{
				swprintf(szBuffer,nMax,TEXT("x%d=%f "),i+1,eigenvalues[i]);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
			Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
		}
		
		swprintf(szBuffer,nMax,TEXT("算法计算时间: %f 毫秒\r\n"),runtime);
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
ErrNo Matrix_Comp::GetMatrix()									  //增广矩阵数据获取
{
	TCHAR	seps[] = TEXT(" \t");
	TCHAR	*ptemp,*pstr,*pnext_token = NULL,*szBuffer = NULL;
	HWND	hwndEdit;
	int		iLine,iLength,iStart,iCount;
	int		iMax = iDimension*20;							//缺憾 获取数据还是有限定的 每行字符个数不能超过iMax个

	hwndEdit = GetDlgItem(hwndDlg,EditControlID);
	szBuffer = new TCHAR[iMax];

	iLine = Edit_GetLineCount(hwndEdit);
	if(iLine == 1)											 //判断是否没有输入数据
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
		if(iLength)				//找出第一个有数据的行
		{
			iStart = i;
			break;
		}
	}

	iOutput = iStart+iDimension;								//输出结果所在行 在矩阵的下一行（行号从0开始）

	for(int i=iStart,k=0;i<iStart+iDimension;i++,k++)			//获取每行数据
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
			if(iCount>iDimension)								//判断每行输入数据个数是否超出限定矩阵维数
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

	iOutput = iStart +4;							//输出结果所在行 在矩阵的下一行（行号从0开始）	

	for(int i=iStart,k=0;i<iStart+4;i++,k++)		//获取每行数据
	{
		iCount = 0;
		iLength = Edit_GetLine(hwndEdit,i,szbuffer,iMax-1);			//注意参数iMax-1
		szbuffer[iLength] = TEXT('\0');
		//字符串分割
		pstr = wcstok_s(szbuffer,seps,&pnext_token);
		while(pstr!=NULL)
		{
			if(*pstr == TEXT('+'))									//判断是否有分割行
				return WRONG_MATRIX;
			else if(!CharacterJudgement(pstr))						//判断是否有非法字符输入
				return INCORRECT_DATA;

			if(iCount>=iDimension)									//判断每行输入数据个数是否超出限定矩阵维数
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

	//申请空间,保存计算结果
	if(!(result = new double[iDimension]))
	{
		return ALLOCATE_MEMORY_FAIL;
	}

	switch(iSelectIndex)
	{
	case GAUSS_INDEX:
	case LU_INDEX:
	case CHOLESKY_INDEX:
		if(err = AllocateMemory(matrix,iDimension,iDimension+1))		//增广矩阵
			return err;
		break;

	case THOMAS_INDEX:
		if(err = AllocateMemory(matrix,4,iDimension))					//三对角矩阵
			return err;
		break;

	case JACOBI_INDEX:
	case GAUSS_SEIDEL_INDEX:
	case SOR_INDEX:
		if(err = AllocateMemory(matrix,iDimension,iDimension+1))
			return err;

		if(!(iteration_vector = new double[iDimension]))		//迭代初始向量
			return ALLOCATE_MEMORY_FAIL;
		for(int i=0;i<iDimension;i++)								//初始化s
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
	if(iIndex == THOMAS_INDEX)		//删除三对角矩阵
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

//带参数函数
ErrNo Matrix_Comp::GetMatrix(const HWND hwndEdit,double **pMatrix,int &nOutput,const int nRows,const int nColumns)
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

	nOutput = iStart+nRows;								//输出结果所在行 在矩阵的下一行（行号从0开始）

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


 //文件输入数据
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


		default:		//打不开文件
			{
				TCHAR ErrInfo[MAXSIZE+MAX_PATH];
				PCTSTR pErrInfo;						//根据错误代码获得的错误消息
				int nMax;

				nMax = MAX_PATH + MAXSIZE;
				
				pErrInfo = FormatError(CAN_NOT_OPEN_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n错误信息:%s\r\n文件路径:%s\r\n补充信息:文件编码不识别"),CAN_NOT_OPEN_FILE,pErrInfo,FilesPath[i]);
				MessageBox(hwndDlg,ErrInfo,TEXT("错误"),MB_ICONERROR);
			}
			return CAN_NOT_OPEN_FILE;
		}
	}
	return 0;
}

ErrNo Matrix_Comp::GetStart_File_ANSI(PCTSTR pstrFilePath)
{
	TCHAR seps[] = TEXT(" \t,"),seps_m[] = TEXT(" \t");		//分割符
	TCHAR *ptemp,*pstr,*pnext_token = NULL;
	int nCount;		//参数个数

	TCHAR ErrInfo[MAXSIZE+MAX_PATH];
	PCTSTR pErrInfo;						//根据错误代码获得的错误消息
	ErrNo err;
	HWND hEdit;
	bool data;								//判断是否有数据
	int nLine,nLine_Pre;					//读取数据读到第几行
	int nLength,ntemp;	
	FILE *fp;
	int nMax;								//ErrInfo数组的大小

	//每行数据存储
	TCHAR *szBuffer;						//获取每行数据
	char  szBuffer_t[4096];					//获取每行数据
	int	  nLenOfWideChar;

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID);
	_tfopen_s(&fp,pstrFilePath,TEXT("r"));		//只读模式打开文件

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
		
		//转换字符
		nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
		szBuffer = new TCHAR [nLenOfWideChar];
		MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

		//判断是否在某一行有数据
		nLength = _tcslen(szBuffer);
		data = false;
		for(int i=0;i<nLength;i++)
		{
			if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
			{
				data = true;	//有数据
				break;
			}
		}
		if(!data)		//找不到数据
		{
			continue;
		}

		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//获取算法参数
		nCount = 0;
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr != NULL)
		{
			if(!CharacterJudgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				
				delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//获取矩阵行数
				nRows = _wtoi(pstr);
				break;

			case 1:		//获取矩阵列数
				nColumns = _wtoi(pstr);
				break;

			case 2:		//获取算法编号
				algorithm = _wtoi(pstr);
				break;

			case 3:		//最大迭代次数
				iterations = _wtoi(pstr);
				break;

			case 4:		//获取迭代精度
				precision = wcstod(pstr,&ptemp);
				break;

			case 5:		//获取松弛因子
				relaxationfactor = wcstod(pstr,&ptemp);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 3)	//参数太少(至少三个）
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n%s\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;	//删除转换后的字符存储地方(动态申请)

			fclose(fp);
			return NOT_ENOUGH_PARA;
		}
		//判断参数是否符合算法要求
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
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID：%d\r\n文件:%s 行数%d\r\n错误信息:%s\r\n"),NOT_MATCH,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;	//删除转换后的字符存储地方(动态申请)

			fclose(fp);
			return NOT_MATCH;
		}

		delete [] szBuffer;	//删除转换后的字符存储地方(动态申请),由于前面申请过，后面要用因此要释放

		//获取矩阵数据
		//申请空间
		if(algorithm == THOMAS_INDEX)
			iDimension = nColumns;
		else
			iDimension = nRows;
		if(err = AllocateMemory(algorithm))
		{
			pErrInfo = FormatError(ALLOCATE_MEMORY_FAIL);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n错误信息:%s\r\n"),ALLOCATE_MEMORY_FAIL,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return ALLOCATE_MEMORY_FAIL;
		}

		//获取矩阵数据
		//找到有数据的一行
		nLine_Pre = nLine;
		data = false;
		while(1)
		{
			nLine++;
			//if(_fgetts(szBuffer,4096,fp) == NULL)		
			if(fgets(szBuffer_t,4096,fp) == NULL)
			{
				if(feof(fp))		//达到文件尾
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n矩阵数据与要求的矩阵维数不匹配\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数：%d\r\n读取文件内容出错\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			else		//如果有数据,判断是否有数字数据
			{

				//转换字符
				nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
				szBuffer = new TCHAR [nLenOfWideChar];
				MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
					{
						data = true;	//有数据
						break;
					}
				}
				if(!data)		//找不到数据
				{
					delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

					continue;
				}
				break;
			}
		}
		nLength = _tcslen(szBuffer);
		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//逐行读取矩阵数据
		if(algorithm == THOMAS_INDEX)		//三对角矩阵获取特殊处理
			ntemp = nColumns -1;
		else
			ntemp = nColumns;

		if(err = StringToNumbericArr(szBuffer,seps_m,matrix[0],ntemp))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
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
				if(feof(fp))		//达到文件尾
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n矩阵数据与要求的矩阵维数不匹配\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数：%d\r\n读取文件内容出错\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}

			//转换字符
			nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
			szBuffer = new TCHAR [nLenOfWideChar];
			MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

			nLength = _tcslen(szBuffer);
			if(szBuffer[nLength-1] == TEXT('\n'))
				szBuffer[nLength-1] = TEXT('\0');

			if(algorithm == THOMAS_INDEX)		//三对角矩阵特殊处理
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
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);

				delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

				fclose(fp);
				return err;
			}

			delete [] szBuffer;			//删除转换后的字符存储地方(动态申请)
		}


		//使用算法
		if(err = Algorithm_Choice(algorithm))
		{
			Edit_ReplaceSel(hEdit,TEXT("\r\n计算矩阵\r\n"));
			for(int i=0;i<nRows;i++)
			{
				for(int j=0;j<nColumns;j++)
				{
					if(algorithm == THOMAS_INDEX)	//三对角矩阵特殊处理
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
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s\r\n错误信息:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//输出计算结果
		OutPutResult_File();
	}

	fclose(fp);
	return 0;
	return 0;
}

ErrNo Matrix_Comp::GetStart_File_UTF_LE(PCTSTR pstrFilePath)
{
	TCHAR seps[] = TEXT(" \t,"),seps_m[] = TEXT(" \t");		//分割符
	TCHAR *ptemp,*pstr,*pnext_token = NULL;
	int nCount;		//参数个数

	TCHAR ErrInfo[MAXSIZE+MAX_PATH];
	PCTSTR pErrInfo;						//根据错误代码获得的错误消息
	ErrNo err;
	HWND hEdit;
	bool data;								//判断是否有数据
	int nLine,nLine_Pre;					//读取数据读到第几行
	int nLength,ntemp;	
	TCHAR szBuffer[4096];					//获取计算参数
	FILE *fp;
	int nMax;								//ErrInfo数组的大小

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID);
	_tfopen_s(&fp,pstrFilePath,TEXT("r,ccs=UNICODE"));		//只读模式打开文件

	if(fp == NULL)
		return CAN_NOT_OPEN_FILE;

	nLine = 0;
	while(!feof(fp))
	{
		nLine++;
		if(_fgetts(szBuffer,4096,fp) == NULL)
			break;
		
		//判断是否在某一行有数据
		nLength = _tcslen(szBuffer);
		data = false;
		for(int i=0;i<nLength;i++)
		{
			if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
			{
				data = true;	//有数据
				break;
			}
		}
		if(!data)		//找不到数据
		{
			continue;
		}

		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//获取算法参数
		nCount = 0;
		pstr = wcstok_s(szBuffer,seps,&pnext_token);
		while(pstr != NULL)
		{
			if(!CharacterJudgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//获取矩阵行数
				nRows = _wtoi(pstr);
				break;
				
			case 1:		//获取矩阵列数
				nColumns = _wtoi(pstr);
				break;

			case 2:		//获取算法编号
				algorithm = _wtoi(pstr);
				break;

			case 3:		//最大迭代次数
				iterations = _wtoi(pstr);
				break;

			case 4:		//获取迭代精度
				precision = wcstod(pstr,&ptemp);
				break;

			case 5:		//获取松弛因子
				relaxationfactor = wcstod(pstr,&ptemp);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 3)	//参数太少(至少三个）
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n%s\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_ENOUGH_PARA;
		}
		//判断参数是否符合算法要求
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
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID：%d\r\n文件:%s 行数%d\r\n错误信息:%s\r\n"),NOT_MATCH,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_MATCH;
		}


		//获取矩阵数据
		//申请空间
		if(algorithm == THOMAS_INDEX)
			iDimension = nColumns;
		else
			iDimension = nRows;
		if(err = AllocateMemory(algorithm))
		{
			pErrInfo = FormatError(ALLOCATE_MEMORY_FAIL);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n错误信息:%s\r\n"),ALLOCATE_MEMORY_FAIL,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return ALLOCATE_MEMORY_FAIL;
		}

		//获取矩阵数据
		//找到有数据的一行
		nLine_Pre = nLine;
		data = false;
		while(1)
		{
			nLine++;
			if(_fgetts(szBuffer,4096,fp) == NULL)		
			{
				if(feof(fp))		//达到文件尾
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n矩阵数据与要求的矩阵维数不匹配\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数：%d\r\n读取文件内容出错\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			else		//如果有数据,判断是否有数字数据
			{
				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if((szBuffer[i] >= TEXT('0') && szBuffer[i] <= TEXT('9')) || szBuffer[i] == TEXT('.'))
					{
						data = true;	//有数据
						break;
					}
				}
				if(!data)		//找不到数据
				{
					continue;
				}
				break;
			}
		}
		nLength = _tcslen(szBuffer);
		if(szBuffer[nLength-1] == TEXT('\n'))
			szBuffer[nLength-1] = TEXT('\0');
		//逐行读取矩阵数据
		if(algorithm == THOMAS_INDEX)		//三对角矩阵获取特殊处理
			ntemp = nColumns -1;
		else
			ntemp = nColumns;

		if(err = StringToNumbericArr(szBuffer,seps_m,matrix[0],ntemp))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		for(int i=1;i<nRows;i++)
		{
			nLine++;
			if(_fgetts(szBuffer,4096,fp) == NULL)
			{
				if(feof(fp))		//达到文件尾
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n矩阵数据与要求的矩阵维数不匹配\r\n"),WRONG_MATRIX,pstrFilePath,nLine_Pre);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return WRONG_MATRIX;
				}
				else
				{
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数：%d\r\n读取文件内容出错\r\n"),READ_FILE_ERR,pstrFilePath,nLine);
					Edit_ReplaceSel(hEdit,ErrInfo);
					fclose(fp);
					return READ_FILE_ERR;
				}
			}
			nLength = _tcslen(szBuffer);
			if(szBuffer[nLength-1] == TEXT('\n'))
				szBuffer[nLength-1] = TEXT('\0');

			if(algorithm == THOMAS_INDEX)		//三对角矩阵特殊处理
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
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),err,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return err;
			}
		}

		
		//使用算法
		if(err = Algorithm_Choice(algorithm))
		{
			Edit_ReplaceSel(hEdit,TEXT("\r\n计算矩阵\r\n"));
			for(int i=0;i<nRows;i++)
			{
				for(int j=0;j<nColumns;j++)
				{
					if(algorithm == THOMAS_INDEX)	//三对角矩阵特殊处理
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
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s\r\n错误信息:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//输出计算结果
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
		pstr_t[i] = szBuffer[i];		//复制字符串

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

	//输出矩阵
	//申请空间
	Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
	for(int i=0;i<nRows;i++)
	{
		for(int j=0;j<nColumns;j++)
		{
			if(algorithm == THOMAS_INDEX)	//三对角矩阵特殊处理
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
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Gauss消元法 \r\n"));
		break;
	case LU_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 三角分解法 \r\n"));
		break;
	case THOMAS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 追赶法(Thomas) \r\n"));
		break;
	case CHOLESKY_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 平方根法(Cholesky) \r\n"));
		break;
	case JACOBI_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Jacobi迭代法 \r\n"));
		break;
	case GAUSS_SEIDEL_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: Gauss-Seidel迭代法 \r\n"));
		break;
	case SOR_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 超松弛迭代法(SOR) \r\n"));
		break;

	case EIGENVALUES_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("所选算法: 特征值计算 \r\n"));
		break;
	}

	if(algorithm != EIGENVALUES_INDEX)
	{
		Edit_ReplaceSel(hwndEdit,TEXT("方程组解为: "));
		for(int i=0;i<iDimension;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%f "),i+1,result[i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
		Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
		if(algorithm == GAUSS_INDEX)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("矩阵的行列式为：%f\r\n"),determinant);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
		else if(algorithm >=JACOBI_INDEX)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("最大迭代次数为: %d  迭代精度为：%f\r\n"),iterations,precision);
			Edit_ReplaceSel(hwndEdit,szBuffer);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("迭代结束后迭代次数为：%d  达到的迭代精度为：%f\r\n"),r_iterations,r_precision);
			Edit_ReplaceSel(hwndEdit,szBuffer);
			if(algorithm == SOR_INDEX)
			{
				_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("松弛因子为: %f\r\n"),relaxationfactor);
				Edit_ReplaceSel(hwndEdit,szBuffer);
			}
		}
	}
	else
	{
		Edit_ReplaceSel(hwndEdit,TEXT("方程特征值为: "));
		for(int i=0;i<iDimension;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x%d=%f "),i+1,eigenvalues[i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
	}
	Edit_ReplaceSel(hwndEdit,TEXT("\r\n"));
}