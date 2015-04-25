/*
	һԪһ�η������
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

	QueryPerformanceFrequency(&nFreq);					//����ʱ��
	QueryPerformanceCounter(&nBefore);

	switch(algorithm)
	{
	case FIXED_POINT_ITERATION_INDEX:							//�����������
		if(err = Fixed_Point_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case  NEWTON_INTERPOLATION_INDEX:						    //Newton������
		if(err = Newton_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case AITKEN_INDEX:										    //Aitken���ٷ���
		if(err = Aitken(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	case STEFFENSEN_INDEX:							//Steffensen������
		if(err = Steffensen(formula,result,max_iterations,precision,initial_value))
		{
			return err;
		}
		break;

	default:		//û��ƥ����㷨
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

	if(!(iLength = Edit_GetText(GetDlgItem(hwndDlg,EditControlID[0]),szBuffer,MAXINPUT-1)))							//��ȡ��������
	{
		MessageBox(hwndDlg,TEXT("û�������������"),TEXT("����"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}
	if(!(( szBuffer[0]>=TEXT('A') && szBuffer[0]<=TEXT('Z')) || ( szBuffer[0]>=TEXT('a') && szBuffer[0]<=TEXT('z'))))
	{
		ErrorReport(hwndDlg,INCORRECT_DATA);
		return INCORRECT_DATA;
	}

	var_symbol_save = var_symbol = szBuffer[0];

	//��д��Сд
	if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
		var_symbol += 32;

	equation_algorithm.ChangeVariableSymbol(var_symbol);



	//��ȡ������ʽ
	if(!(iLength = Edit_GetText(GetDlgItem(hwndDlg,EditControlID[2]),szBuffer,MAXINPUT-1)))
	{
		ErrorReport(hwndDlg,NO_DATA_INPUT);
		return NO_DATA_INPUT;
	}
	for(int i=0;i<lstrlen(szBuffer)+1;i++)
		formula[i] = formula_store[i] = szBuffer[i];

	//��ȡ����������
	if(err = GetLine_Int(GetDlgItem(hwndDlg,EditControlID[3]),&max_iterations))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}
	original_iterations = max_iterations;

	//��ȡ��������
	if(err = GetLine_Double(GetDlgItem(hwndDlg,EditControlID[4]),&precision))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}
	original_precision = precision;

	//��ȡ��ʼֵ
	if(err = GetLine_Double(GetDlgItem(hwndDlg,EditControlID[5]),&initial_value))
	{
		ErrorReport(hwndDlg,err);
		return err;
	}


	QueryPerformanceFrequency(&nFreq);					//����ʱ��
	QueryPerformanceCounter(&nBefore);

	switch(iSelectIndex)
	{
	case FIXED_POINT_ITERATION_INDEX:							//�����������
		if(err = equation_algorithm.Fixed_Point_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;
		
	case  NEWTON_INTERPOLATION_INDEX:						    //Newton������
		if(err = equation_algorithm.Newton_Iteration(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;

	case AITKEN_INDEX:										    //Aitken���ٷ���
		if(err = equation_algorithm.Aitken(formula,result,max_iterations,precision,initial_value))
		{
			ErrorReport(hwndDlg,err);
			return err;
		}
		break;
		
	case STEFFENSEN_INDEX:							//Steffensen������
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
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: ����������㷨 \r\n"));
		break;
	case  1:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Newton�����㷨 \r\n"));
		break;
	case  2:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Aitken�����㷨 \r\n"));
		break;
	case  3:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Steffensen�����㷨 \r\n"));
		break;
	}
	wsprintf(szBuffer,TEXT("������ʽ��f(%c)=%s \r\n"),var_symbol_save,formula_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("���̽��ƽ�: %f \r\n"),result);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("��������: %d  ����ﵽ�ĵ�������: %.10f\r\n"),max_iterations,precision);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("��ʼ��������:  ����������: %d  ��������: %.10f  ��ʼֵ: %f \r\n"),original_iterations,original_precision,initial_value);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("����ʱ��: %f ����\r\n"),runtime);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++\r\n"));
	return 0;
}

//���ļ���ȡ���ݿ�ʼ����
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

ErrNo Equation::GetStart_File_UTF_LE(PCTSTR pstrFilePath)
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
	int nLength;	
	TCHAR szBuffer[4096];					//��ȡ�������
	FILE *fp;
	int nMax;								//ErrInfo����Ĵ�С

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID[6]);
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
			if(nCount == 4)		//�жϱ��������Ƿ�����
			{
				//�ж��Ƿ�Ϊ��ĸ
				//�ж��Ƿ�������ȷ�ַ�
				if(pstr[0] == TEXT('E') || pstr[0] == TEXT('e'))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s  ����:%d\r\n������Ϣ:%s\r\n������Ϣ:��������e��E��Ϊ��������"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);
					return INCORRECT_DATA;
				}
				else if(!(( pstr[0]>=TEXT('A') && pstr[0]<=TEXT('Z')) || ( pstr[0]>=TEXT('a') && pstr[0]<=TEXT('z'))))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s  ����:%d\r\n������Ϣ:%s\r\n������Ϣ:���������������"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);
					return INCORRECT_DATA;
				}
			}
			else if(!Character_Judgement(pstr))
			{				
				pErrInfo = FormatError(INCORRECT_DATA_FILE);
				_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:%s\r\n"),INCORRECT_DATA_FILE,pstrFilePath,nLine,pErrInfo);
				Edit_ReplaceSel(hEdit,ErrInfo);
				fclose(fp);
				return INCORRECT_DATA_FILE;
			}
			switch(nCount)
			{
			case 0:		//��ȡ�㷨���
				algorithm = _wtoi(pstr);
				break;

			case 1:		//��ȡ����������
				original_iterations = max_iterations = _wtoi(pstr);
				break;

			case 2:		//��ȡ��������
				original_precision = precision = _tcstod(pstr,&ptemp);
				break;

			case 3:		//������ʼֵ
				initial_value = _tcstod(pstr,&ptemp);
				break;

			case 4:		//��ȡ��������
				var_symbol_save = var_symbol = pstr[0];

				//��д��Сд
				if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
					var_symbol += 32;		

				ChangeVariableSymbol(var_symbol);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 4)	//����̫��(����4����
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n%s\r\n������Ϣ:����̫��\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return NOT_ENOUGH_PARA;
		}

		
		//��ȡ������ʽ
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
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:δ���������ʽ\r\n"),NO_DATA_INPUT,pstrFilePath,nLine_Pre);
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
			else		//���������,�ж��Ƿ����
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
		
		//��ȡ���ʽ����
		for(int i=0;i<=nLength;i++)
			formula_store[i] = formula[i] = szBuffer[i];

		
		//ʹ���㷨
		
		if(err = Algorithm_Choice(algorithm))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s\r\n������Ϣ:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//���������
		OutPutResult_File(algorithm);
	}

	fclose(fp);
	return 0;
}

ErrNo Equation::GetStart_File_ANSI(PCTSTR pstrFilePath)
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
	int nLength;	
	FILE *fp;
	int nMax;								//ErrInfo����Ĵ�С

	//ÿ�����ݴ洢
	TCHAR *szBuffer;					//��ȡÿ������
	char  szBuffer_t[4096];
	int	  nLenOfWideChar;

	nMax = MAXSIZE + MAX_PATH;
	hEdit = GetDlgItem(hwndDlg,EditControlID[6]);
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
			if(nCount == 4)		//�жϱ��������Ƿ�����
			{
				//�ж��Ƿ�Ϊ��ĸ
				//�ж��Ƿ�������ȷ�ַ�
				if(pstr[0] == TEXT('E') || pstr[0] == TEXT('e'))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s  ����:%d\r\n������Ϣ:%s\r\n������Ϣ:��������e��E��Ϊ��������"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);

					delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

					fclose(fp);
					return INCORRECT_DATA;
				}
				else if(!(( pstr[0]>=TEXT('A') && pstr[0]<=TEXT('Z')) || ( pstr[0]>=TEXT('a') && pstr[0]<=TEXT('z'))))
				{
					pErrInfo = FormatError(INCORRECT_DATA);
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s  ����:%d\r\n������Ϣ:%s\r\n������Ϣ:���������������"),INCORRECT_DATA,pstrFilePath,nLine,pErrInfo);
					Edit_ReplaceSel(hEdit,ErrInfo);

					delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

					fclose(fp);
					return INCORRECT_DATA;
				}
			}
			else if(!Character_Judgement(pstr))
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
			case 0:		//��ȡ�㷨���
				algorithm = _wtoi(pstr);
				break;

			case 1:		//��ȡ����������
				original_iterations = max_iterations = _wtoi(pstr);
				break;

			case 2:		//��ȡ��������
				original_precision = precision = _tcstod(pstr,&ptemp);
				break;

			case 3:		//������ʼֵ
				initial_value = _tcstod(pstr,&ptemp);
				break;

			case 4:		//��ȡ��������
				var_symbol_save = var_symbol = pstr[0];

				//��д��Сд
				if(var_symbol>=TEXT('A') && var_symbol<=TEXT('Z'))
					var_symbol += 32;		

				ChangeVariableSymbol(var_symbol);
				break;
			}
			pstr = wcstok_s(NULL,seps,&pnext_token);
			nCount++;
		}

		if(nCount < 4)	//����̫��(����4����
		{
			pErrInfo = FormatError(NOT_ENOUGH_PARA);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("\r\n����ID:%d\r\n�ļ�:%s ����:%d\r\n%s\r\n������Ϣ:����̫��\r\n"),NOT_ENOUGH_PARA,pstrFilePath,nLine,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);

			delete [] szBuffer;		//ɾ��ת������ַ��洢�ط�(��̬����)

			fclose(fp);
			return NOT_ENOUGH_PARA;
		}


		//��ȡ������ʽ
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
					_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s ����:%d\r\n������Ϣ:δ���������ʽ\r\n"),NO_DATA_INPUT,pstrFilePath,nLine_Pre);
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
			else		//���������,�ж��Ƿ����
			{

				//ת���ַ�
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


		//��ȡ���ʽ����
		for(int i=0;i<=nLength;i++)
			formula_store[i] = formula[i] = szBuffer[i];

		delete [] szBuffer;	//ɾ��ת������ַ��洢�ط�(��̬����)
		//ʹ���㷨
		if(err = Algorithm_Choice(algorithm))
		{
			pErrInfo = FormatError(err);
			_sntprintf_s(ErrInfo,_countof(ErrInfo),nMax,TEXT("����ID:%d\r\n�ļ�:%s\r\n������Ϣ:%s\r\n"),err,pstrFilePath,pErrInfo);
			Edit_ReplaceSel(hEdit,ErrInfo);
			fclose(fp);
			return err;
		}
		//���������
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
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: ����������㷨 \r\n"));
		break;
	case  1:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Newton�����㷨 \r\n"));
		break;
	case  2:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Aitken�����㷨 \r\n"));
		break;
	case  3:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Steffensen�����㷨 \r\n"));
		break;
	}
	wsprintf(szBuffer,TEXT("������ʽ��f(%c)=%s \r\n"),var_symbol_save,formula_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("���̽��ƽ�: %f \r\n"),result);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("��������: %d  ����ﵽ�ĵ�������: %.10f\r\n"),max_iterations,precision);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("��ʼ��������:  ����������: %d  ��������: %.10f  ��ʼֵ: %f \r\n"),original_iterations,original_precision,initial_value);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	swprintf(szBuffer,MAXINPUT,TEXT("����ʱ��: %f ����\r\n"),runtime);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++\r\n"));
	return;
}
