/* 错误 */
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
	_sntprintf_s(ErrInfo,_countof(ErrInfo),MAXSIZE,TEXT("错误ID:%d\r\n%s"),ErrorNumber,pErrInfo);
	MessageBox(hDlg,ErrInfo,TEXT("错误"),MB_ICONERROR);
	/*
	switch(ErrorNumber)
	{
	case  ALLOCATE_MEMORY_FAIL:
		MessageBox(hDlg,TEXT("申请内存失败"),TEXT("错误"),MB_ICONERROR);
		break;
	case NO_DATA_INPUT:
		MessageBox(hDlg,TEXT("未输入完整数据"),TEXT("错误"),MB_ICONERROR);
		break;
	case INCORRECT_DATA:
		MessageBox(hDlg,TEXT("输入数据不符合标准"),TEXT("错误"),MB_ICONERROR);
		break;
	case WRONG_MATRIX:
		MessageBox(hDlg,TEXT("输入矩阵有误"),TEXT("错误"),MB_ICONERROR);
		break;
	case DIVIDE_BY_ZERO:
		MessageBox(hDlg,TEXT("计算过程中遇到除数为零"),TEXT("错误"),MB_ICONERROR);
		break;
	case NEGATIVE_NUMBER:
		MessageBox(hDlg,TEXT("对负数开根或求对数"),TEXT("错误"),MB_ICONERROR);
		break;
	case BRACKET_NOT_MATCH:
		MessageBox(hDlg,TEXT("表达式括号不匹配"),TEXT("错误"),MB_ICONERROR);
		break;
	case WRONG_EXPRESSION:
		MessageBox(hDlg,TEXT("表达式错误"),TEXT("错误"),MB_ICONERROR);
		break;
	case NOT_REQUIRED_SOLUTION:
		MessageBox(hDlg,TEXT("迭代未达到精度要求"),TEXT("错误"),MB_ICONERROR);
		break;
	case DATA_OVERFLOW:
		MessageBox(hDlg,TEXT("数据溢出"),TEXT("错误"),MB_ICONERROR);
		break;
	case ARRAY_OVERFLOW:
		MessageBox(hDlg,TEXT("程序运作过程中出现数组溢出"),TEXT("错误"),MB_ICONERROR);
		break;
	case NOT_MATCH_DEGREE:
		MessageBox(hDlg,TEXT("输入数据个数与次数不匹配"),TEXT("错误"),MB_ICONERROR);
		break;
	case NOT_MATCH:
		MessageBox(hDlg,TEXT("输入数据个数有误"),TEXT("错误"),MB_ICONERROR);
		break;
	case NOT_IN_INTERVAL:
		MessageBox(hDlg,TEXT("不在计算区间内"),TEXT("错误"),MB_ICONERROR);
		break;
	case SMALL_INTERVAL:
		MessageBox(hDlg,TEXT("计算区间过小"),TEXT("错误"),MB_ICONERROR);
		break;
	case NO_RESULT:
		MessageBox(hDlg,TEXT("计算不出结果"),TEXT("错误"),MB_ICONERROR);
		break;
	case DIVERGENCY:
		MessageBox(hDlg,TEXT("迭代不收敛"),TEXT("错误"),MB_ICONERROR);
		break;

	case CAN_NOT_OPEN_FILE:
		{
			TCHAR szBuffer[MAX_PATH+50],*pstrFileName;
			va_list var_arg;

			va_start(var_arg,n_values);

			pstrFileName = va_arg(var_arg,TCHAR *);
			_sntprintf_s(szBuffer,_countof(szBuffer),MAX_PATH,TEXT("错误ID:%d\r\n不能打开文件:%s"),CAN_NOT_OPEN_FILE,pstrFileName);
			MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);

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
			_sntprintf_s(szBuffer,_countof(szBuffer),MAX_PATH,TEXT("错误ID:%d 输入数据非法!\r\n文件:%s \r\n行数:%d"),INCORRECT_DATA_FILE,pstrFileName,nLine);
			MessageBox(hDlg,szBuffer,TEXT("错误"),MB_ICONERROR);

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