/*
	一元一次方程求解
*/
#include "Computation.h"
#include "equation.h"

Equation::Equation(HWND hDlg)
{
	hwndDlg = hDlg;
}

ErrNo Equation::Algorithm_Choice(int algorithm)
{
	ErrNo				err;
	LARGE_INTEGER		nFreq,nBefore,nAfter;
	Equation_ALgorithm	equation_algorithm;

	QueryPerformanceFrequency(&nFreq);					//计算时间
	QueryPerformanceCounter(&nBefore);

	switch(algorithm)
	{
	case FIXED_POINT_ITERATION_INDEX:							//不动点迭代法
		if(err = Fixed_Point_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case  NEWTON_INTERPOLATION_INDEX:						    //Newton迭代法
		if(err = Newton_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case AITKEN_INDEX:										    //Aitken加速方案
		if(err = Aitken(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case STEFFENSEN_INDEX:							//Steffensen迭代法
		if(err = Steffensen(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	default:		//没有匹配的算法
		return NOT_MATCH_ALGORITHM;
	}

	QueryPerformanceCounter(&nAfter);
	runtime = (nAfter.QuadPart - nBefore.QuadPart)*1000 / (double)nFreq.QuadPart;
	return 0;
}

ErrNo Equation::GetStart(int iSelectIndex)
{	
	TCHAR				szBuffer[MAXINPUT];
	int					iLength = 0;
	ErrNo				err;
	LARGE_INTEGER		nFreq,nBefore,nAfter;
	Equation_ALgorithm	equation_algorithm;

	if(!(iLength = Edit_GetText(GetDlgItem(hwndDlg,EditControlID[0]),szBuffer,MAXINPUT-1)))							//获取变量符号
	{
		MessageBox(hwndDlg,TEXT("没有输入变量符号"),TEXT("错误"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}
	if(!(( szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z')) || ( szBuffer[0]>=TEXT('a') && szBuffer[0]<=TEXT('z'))))
	{
		ErrorReport(hwndDlg,INCORRECT_DATA);
		return INCORRECT_DATA;
	}

	var_symbol_save = var_symbol = szBuffer[0];

	//大写变小写
	if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
		var_symbol += 32;

	equation_algorithm.ChangeVariableSymbol(var_symbol);



	//获取迭代公式
	if(!(iLength = Edit_GetText(GetDlgItem(hwndDlg,EditControlID[2]),szBuffer,MAXINPUT-1)))
	{
		ErrorReport(hwndDlg,NO_DATA_INPUT);
		return NO_DATA_INPUT;
	}
	for(int i=0;i<lstrlen(szBuffer)+1;i++)
		formula[i] = formula_store[i] = szBuffer[i];

	//获取最大迭代次数
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[3]),&max_iterations))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}
	original_iterations = max_iterations;

	//获取迭代精度
	if(err = GetLine_Double(GetDlgItem(hwndDlg,EditControlID[4]),&precision))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}
	original_precision = precision;

	//获取初始值
	if(err = GetLine_Double(GetDlgItem(hwndDlg,EditControlID[5]),&initial_value))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}


	QueryPerformanceFrequency(&nFreq);					//计算时间
	QueryPerformanceCounter(&nBefore);

	switch(iSelectIndex)
	{
	case FIXED_POINT_ITERATION_INDEX:							//不动点迭代法
		if(err = equation_algorithm.Fixed_Point_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;
		
	case  NEWTON_INTERPOLATION_INDEX:						    //Newton迭代法
		if(err = equation_algorithm.Newton_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;

	case AITKEN_INDEX:										    //Aitken加速方案
		if(err = equation_algorithm.Aitken(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;
		
	case STEFFENSEN_INDEX:							//Steffensen迭代法
		if(err = equation_algorithm.Steffensen(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;
	}

	QueryPerformanceCounter(&nAfter);
	runtime = (nAfter.QuadPart - nBefore.QuadPart)*1000 / (double)nFreq.QuadPart;

	if(err = OutputResult(iSelectIndex))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}
	return 0;
}

ErrNo Equation::GetLine_Double(HWND hwndEdit,double *fParameter)
{
	TCHAR		*ptemp;
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	if(!Character_Judgement(Buffer))
		return INCORRECT_DATA;

	*fParameter = wcstod(Buffer,&ptemp);
	return 0;
}

ErrNo Equation::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

bool Equation::Character_Judgement(const TCHAR *szBuffer)
{
	int		iLength = 0;

	iLength = lstrlen(szBuffer);

	for(int i=0;i<iLength;i++)
	{
		if(szBuffer[i] == TEXT('.'))
		{
			if(i==0 || i==iLength -1)
				return FALSE;
		}
		else if(szBuffer[i] == TEXT('-'))
		{
			if(i!=0)
				return FALSE;
		}
		else if( (szBuffer[i]<TEXT('0')) || (szBuffer[i]>TEXT('9')) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

ErrNo Equation::OutputResult(int iSelecIndex)
{
	TCHAR		szBuffer[MAXINPUT];
	HWND		hwndEdit = GetDlgItem(hwndDlg,EditControlID[6]);

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelecIndex)
	{
	case 0:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 不动点迭代算法 \r\n"));
		break;
	case  1:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Newton迭代算法 \r\n"));
		break;
	case  2:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Aitken加速算法 \r\n"));
		break;
	case  3:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Steffensen迭代算法 \r\n"));
		break;
	}
	wsprintf(szBuffer,TEXT("迭代公式：f(%c)=%s \r\n"),var_symbol_save,formula_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("方程近似解: %f \r\n"),result);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("迭代次数: %d  计算达到的迭代精度: %.10f\r\n"),max_iterations,precision);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("初始迭代参数:  最大迭代次数: %d  迭代精度: %.10f  初始值: %f \r\n"),original_iterations,original_precision,initial_value);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("计算时间: %f 毫秒\r\n"),runtime);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++\r\n"));
	return 0;
}

//从文件获取数据开始计算
ErrNo Equation::GetStart_File(TCHAR **FilesPath,const int nCount)
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

ErrNo Equation::GetStart_File_UTF_LE(PCTSTR pstrFilePath)
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
	int nLength;	
	TCHAR szBuffer[4096];					//获取计算参数
	FILE *fp;
	int nMax;								//ErrInfo数组的大小

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID[6]);
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
			if(nCount == 4)		//判断变量符号是否输入
			{
				//判断是否为字母
				//判断是否输入正确字符
				if(pstr[0] == TEXT('E') || pstr[0] == TEXT('e'))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s  行数:%d\r\n错误信息:%s\r\n补充信息:不能输入e或E作为变量符号"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);
					return INCORRECT_DATA;
				}
				else if(!(( pstr[0]>=TEXT('A') && pstr[0]<=TEXT('Z')) || ( pstr[0]>=TEXT('a') && pstr[0]<=TEXT('z'))))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s  行数:%d\r\n错误信息:%s\r\n补充信息:变量符号输入错误"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);
					return INCORRECT_DATA;
				}
			}
			else if(!Character_Judgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//获取算法编号
				algorithm = _wtoi(pstr);
				break;

			case 1:		//获取最大迭代次数
				original_iterations = max_iterations = _wtoi(pstr);
				break;

			case 2:		//获取迭代精度
				original_precision = precision = _tcstod(pstr,&ptemp);
				break;

			case 3:		//迭代初始值
				initial_value = _tcstod(pstr,&ptemp);
				break;

			case 4:		//获取变量符号
				var_symbol_save = var_symbol = pstr[0];

				//大写变小写
				if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
					var_symbol += 32;		

				ChangeVariableSymbol(var_symbol);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 4)	//参数太少(至少4个）
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n%s\r\n补充信息:参数太少\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_ENOUGH_PARA;
		}

		
		//获取迭代公式
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
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:未输入迭代公式\r\n"),NO_DATA_INPUT,pstrFilePath,nLine_Pre);
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
			else		//如果有数据,判断是否空行
			{
				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if(szBuffer[i] == TEXT(' ') || szBuffer[i] == TEXT('\t') || szBuffer[i] == TEXT('\n'))
						continue;
					else
					{
						data = true;
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
		
		//获取表达式数据
		for(int i=0;i<=nLength;i++)
			formula_store[i] = formula[i] = szBuffer[i];

		
		//使用算法
		
		if(err = Algorithm_Choice(algorithm))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s\r\n错误信息:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//输出计算结果
		OutPutResult_File(algorithm);
	}

	fclose(fp);
	return 0;
}

ErrNo Equation::GetStart_File_ANSI(PCTSTR pstrFilePath)
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
	int nLength;	
	FILE *fp;
	int nMax;								//ErrInfo数组的大小

	//每行数据存储
	TCHAR *szBuffer;					//获取每行数据
	char  szBuffer_t[4096];
	int	  nLenOfWideChar;

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID[6]);
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
			if(nCount == 4)		//判断变量符号是否输入
			{
				//判断是否为字母
				//判断是否输入正确字符
				if(pstr[0] == TEXT('E') || pstr[0] == TEXT('e'))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s  行数:%d\r\n错误信息:%s\r\n补充信息:不能输入e或E作为变量符号"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);

					delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

					fclose(fp);
					return INCORRECT_DATA;
				}
				else if(!(( pstr[0]>=TEXT('A') && pstr[0]<=TEXT('Z')) || ( pstr[0]>=TEXT('a') && pstr[0]<=TEXT('z'))))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s  行数:%d\r\n错误信息:%s\r\n补充信息:变量符号输入错误"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);

					delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

					fclose(fp);
					return INCORRECT_DATA;
				}
			}
			else if(!Character_Judgement(pstr))
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
			case 0:		//获取算法编号
				algorithm = _wtoi(pstr);
				break;

			case 1:		//获取最大迭代次数
				original_iterations = max_iterations = _wtoi(pstr);
				break;

			case 2:		//获取迭代精度
				original_precision = precision = _tcstod(pstr,&ptemp);
				break;

			case 3:		//迭代初始值
				initial_value = _tcstod(pstr,&ptemp);
				break;

			case 4:		//获取变量符号
				var_symbol_save = var_symbol = pstr[0];

				//大写变小写
				if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
					var_symbol += 32;		

				ChangeVariableSymbol(var_symbol);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 4)	//参数太少(至少4个）
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n错误ID:%d\r\n文件:%s 行数:%d\r\n%s\r\n补充信息:参数太少\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;		//删除转换后的字符存储地方(动态申请)

			fclose(fp);
			return NOT_ENOUGH_PARA;
		}


		//获取迭代公式
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
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s 行数:%d\r\n错误信息:未输入迭代公式\r\n"),NO_DATA_INPUT,pstrFilePath,nLine_Pre);
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
			else		//如果有数据,判断是否空行
			{

				//转换字符
				nLenOfWideChar = MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,NULL,0);
				szBuffer = new TCHAR [nLenOfWideChar];
				MultiByteToWideChar(CP_ACP,0,szBuffer_t,-1,szBuffer,nLenOfWideChar);

				nLength = _tcslen(szBuffer);
				data = false;
				for(int i=0;i<nLength;i++)
				{
					if(szBuffer[i] == TEXT(' ') || szBuffer[i] == TEXT('\t') || szBuffer[i] == TEXT('\n'))
						continue;
					else
					{
						data = true;
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


		//获取表达式数据
		for(int i=0;i<=nLength;i++)
			formula_store[i] = formula[i] = szBuffer[i];

		delete [] szBuffer;	//删除转换后的字符存储地方(动态申请)
		//使用算法
		if(err = Algorithm_Choice(algorithm))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("错误ID:%d\r\n文件:%s\r\n错误信息:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//输出计算结果
		OutPutResult_File(algorithm);
	}

	fclose(fp);
	return 0;
}

void Equation::OutPutResult_File(int algorithm_num)
{
	TCHAR		szBuffer[MAXINPUT];
	HWND		hwndEdit = GetDlgItem(hwndDlg,EditControlID[6]);

	switch(algorithm_num)
	{
	case 0:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: 不动点迭代算法 \r\n"));
		break;
	case  1:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Newton迭代算法 \r\n"));
		break;
	case  2:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Aitken加速算法 \r\n"));
		break;
	case  3:
		Edit_ReplaceSel(hwndEdit,TEXT("选择的算法: Steffensen迭代算法 \r\n"));
		break;
	}
	wsprintf(szBuffer,TEXT("迭代公式：f(%c)=%s \r\n"),var_symbol_save,formula_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("方程近似解: %f \r\n"),result);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("迭代次数: %d  计算达到的迭代精度: %.10f\r\n"),max_iterations,precision);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("初始迭代参数:  最大迭代次数: %d  迭代精度: %.10f  初始值: %f \r\n"),original_iterations,original_precision,initial_value);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("计算时间: %f 毫秒\r\n"),runtime);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++\r\n"));
	return;
}
