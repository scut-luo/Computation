/* ���� */
#include "Computation.h"
#include "exception.h"

Exception::Exception()
{

}

void Exception::ErrorReport(HWND hDlg,ErrNo ErrorNumber,const int n_values,...)
{
	PCTSTR pErrInfo;
	TCHAR ErrInfo[MAXSIZE];

	if(hDlg == NULL)
		return;

	pErrInfo = FormatError(ErrorNumber);
	_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("����ID:%d\r\n%s"),ErrorNumber,pErrInfo);
	MessageBox(hDlg,ErrInfo,TEXT("����"),MB_ICONERROR);
	/*
	switch(ErrorNumber)
	{
	case  ALLOCATE_MEMORY_FAIL:
		MessageBox(hDlg,TEXT("�����ڴ�ʧ��"),TEXT("����"),MB_ICONERROR);
		break;
	case NO_DATA_INPUT:
		MessageBox(hDlg,TEXT("δ������������"),TEXT("����"),MB_ICONERROR);
		break;
	case INCORRECT_DATA:
		MessageBox(hDlg,TEXT("�������ݲ����ϱ�׼"),TEXT("����"),MB_ICONERROR);
		break;
	case WRONG_MATRIX:
		MessageBox(hDlg,TEXT("�����������"),TEXT("����"),MB_ICONERROR);
		break;
	case DIVIDE_BY_ZERO:
		MessageBox(hDlg,TEXT("�����������������Ϊ��"),TEXT("����"),MB_ICONERROR);
		break;
	case NEGATIVE_NUMBER:
		MessageBox(hDlg,TEXT("�Ը��������������"),TEXT("����"),MB_ICONERROR);
		break;
	case BRACKET_NOT_MATCH:
		MessageBox(hDlg,TEXT("���ʽ���Ų�ƥ��"),TEXT("����"),MB_ICONERROR);
		break;
	case WRONG_EXPRESSION:
		MessageBox(hDlg,TEXT("���ʽ����"),TEXT("����"),MB_ICONERROR);
		break;
	case NOT_REQUIRED_SOLUTION:
		MessageBox(hDlg,TEXT("����δ�ﵽ����Ҫ��"),TEXT("����"),MB_ICONERROR);
		break;
	case DATA_OVERFLOW:
		MessageBox(hDlg,TEXT("�������"),TEXT("����"),MB_ICONERROR);
		break;
	case ARRAY_OVERFLOW:
		MessageBox(hDlg,TEXT("�������������г����������"),TEXT("����"),MB_ICONERROR);
		break;
	case NOT_MATCH_DEGREE:
		MessageBox(hDlg,TEXT("�������ݸ����������ƥ��"),TEXT("����"),MB_ICONERROR);
		break;
	case NOT_MATCH:
		MessageBox(hDlg,TEXT("�������ݸ�������"),TEXT("����"),MB_ICONERROR);
		break;
	case NOT_IN_INTERVAL:
		MessageBox(hDlg,TEXT("���ڼ���������"),TEXT("����"),MB_ICONERROR);
		break;
	case SMALL_INTERVAL:
		MessageBox(hDlg,TEXT("���������С"),TEXT("����"),MB_ICONERROR);
		break;
	case NO_RESULT:
		MessageBox(hDlg,TEXT("���㲻�����"),TEXT("����"),MB_ICONERROR);
		break;
	case DIVERGENCY:
		MessageBox(hDlg,TEXT("����������"),TEXT("����"),MB_ICONERROR);
		break;

	case CAN_NOT_OPEN_FILE:
		{
			TCHAR szBuffer[MAX_PATH+50],*pstrFileName;
			va_list var_arg;

			va_start(var_arg,n_values);

			pstrFileName = va_arg(var_arg,TCHAR *);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAX_PATH,TEXT("����ID:%d\r\n���ܴ��ļ�:%s"),CAN_NOT_OPEN_FILE,pstrFileName);
			MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);

			va_end(var_arg);
		}
		break;
	case INCORRECT_DATA_FILE:
		{
			va_list var_arg;
			int nLine;
			const TCHAR *pstrFileName;
			TCHAR szBuffer[MAX_PATH+50];

			va_start(var_arg,n_values);

			pstrFileName = va_arg(var_arg,const TCHAR *);
			nLine = va_arg(var_arg,int);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAX_PATH,TEXT("����ID:%d �������ݷǷ�!\r\n�ļ�:%s \r\n����:%d"),INCORRECT_DATA_FILE,pstrFileName,nLine);
			MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);

			va_end(var_arg);
		}
		break;
	}
	*/
}

PCTSTR Exception::FormatError(const ErrNo ErrorNumber)
{
	int nCount;

	nCount = _countof(FormatErr);

	for(int i=0;i<nCount;i++)
	{
		if(ErrorNumber == FormatErr[i].ErrNum)
			return FormatErr[i].ErrDescription;
	}
	return NULL;
}