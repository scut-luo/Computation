#include "Computation.h"
#include "Calculus.h"

Calculus::Calculus(HWND hwndDlg)
{
	eps = 1e-6;
	hDlg = hwndDlg;
}

//΢�ּ���
ErrNo Calculus::Algorithm_Choice_D(int iSelectIndex)
{
	ErrNo err;
	Differential_Algorithm diff;

	switch(iSelectIndex)
	{
		//��ǰ���
	case FORWARD_DIFFERENCE_INDEX:
		if(err = diff.Forward_Difference(result,expression,value,interval))
			return err;
		break;
		
		//�����
	case BACKFORWARD_DIFFERENCE_INDEX:
		if(err = diff.Backward_Difference(result,expression,value,interval))
			return err;
		break;

		//���Ĳ��
	case CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		if(err = diff.Central_Difference(result,expression,value,interval))
			return err;
		break;

		//�������Ĳ��
	case SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		if(err = diff.Second_Order_Central_Difference(result,expression,value,interval))
			return err;
		break;
	}
	return 0;
}

ErrNo Calculus::GetStart_D(int iSelectIndex)
{
	PCTSTR pErrInfo;
	TCHAR szBuffer[MAXSIZE];
	int nLength;
	ErrNo err;

	//��ȡ�����С
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID_D[0]),&interval))
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n������Ϣ:��������С���ݴ���"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);
		return err;
	}
	//��ȡ�����
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID_D[1]),&value))
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n������Ϣ:������������ݴ���"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);
		return err;
	}

	//��ȡ�������ʽ
	if(!(nLength = Edit_GetText(GetDlgItem(hDlg,EditControlID_D[2]),expression,MAXSIZE)))
	{
		err = NO_DATA_INPUT;
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n������Ϣ:δ����΢�ּ�������ı��ʽ"),err,pErrInfo);
		MessageBox(hDlg,szBuffer,TEXT("����"),MB_ICONERROR);
		return err;
	}

	//����һ�ݺ������ʽ
	nLength = _tcslen(expression) + 1;
	for(int i=0;i<nLength;i++)
		exp_store[i] = expression[i];

	//�㷨ѡ��
	err = Algorithm_Choice_D(iSelectIndex);

	//������
	OutputResult_D(iSelectIndex,err);
	return 0;
}

void Calculus::OutputResult_D(int iSelectIndex,ErrNo err)
{
	PCTSTR	pErrInfo;
	HWND hEdit = GetDlgItem(hDlg,EditOutputID);
	TCHAR szBuffer[MAXSIZE];

	Edit_SetSel(hEdit,0,0);
	switch(iSelectIndex)
	{
	case FORWARD_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("ѡ����㷨: ����Taylor��ʽ����ֵ΢�ֹ�ʽ-��ǰ���� \r\n"));
		break;

	case BACKFORWARD_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("ѡ����㷨: ����Taylor��ʽ����ֵ΢�ֹ�ʽ-������ \r\n"));
		break;

	case CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("ѡ����㷨: ����Taylor��ʽ����ֵ΢�ֹ�ʽ-���Ĳ��� \r\n"));
		break;

	case SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX:
		Edit_ReplaceSel(hEdit,TEXT("ѡ����㷨: ����Taylor��ʽ����ֵ΢�ֹ�ʽ-�������Ĳ��� \r\n"));
		break;
	}

	_stprintf_s(szBuffer,MAXSIZE,TEXT("�������ʽΪ��y=%s\r\n"),exp_store);
	Edit_ReplaceSel(hEdit,szBuffer);
	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	else
	{
		
		if(iSelectIndex == SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX)
			_stprintf_s(szBuffer,MAXSIZE,TEXT("����㣺%f  �����%f  �����ڸõ�Ķ��׵���Ϊ��%f\r\n"),value,interval,result);
		else
			_stprintf_s(szBuffer,MAXSIZE,TEXT("����㣺%f  �����%f  �����ڸõ��һ�׵���Ϊ��%f\r\n"),value,interval,result);
		Edit_ReplaceSel(hEdit,szBuffer);
	}
	Edit_ReplaceSel(hEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));
}


//���ּ���
ErrNo Calculus::GetStart_I(int iSelectIndex)
{
	ErrNo			err;
	int				iLength;

	ChangeVariableSymbol(TEXT('x'));				//ʹ��Ĭ�ϵı�������

	//��ȡ�������ʽ
	if(! (iLength = Edit_GetText(GetDlgItem(hDlg,EditControlID[0]),expression,MAXINPUT)))
	{
		MessageBox(hDlg,TEXT("δ������ʽ"),TEXT("����"),MB_ICONERROR);
		return NO_DATA_INPUT;
	}
	iLength = lstrlen(expression);
	for(int i=0;i<=iLength;i++)
	{
		exp_store[i] = expression[i];
	}


	//��ȡ�����С
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[1]),&left_interval))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[2]),&right_interval))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(iSelectIndex == GAUSS_LEGENDRE_INDEX || iSelectIndex == GAUSS_CHEBYSHEV_INDEX)
	{
		if(fabs(left_interval+1)>eps || fabs(right_interval-1)>eps)
		{
			MessageBox(hDlg,TEXT("�����������Ӧ���뷶Χ[-1,1]"),TEXT("����"),MB_ICONERROR);
			return NOT_MATCH;
		}
	}

	//��ȡ���㾫��
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[3]),&precision))
	{
		if(iSelectIndex == GAUSS_LEGENDRE_INDEX || iSelectIndex == GAUSS_CHEBYSHEV_INDEX)
			;
		else
		{
			ErrorReport(hDlg,err);
			return err;
		}
	}
	precision_a = precision;

	switch(iSelectIndex)
	{
	case ROMBERG_INDEX:
		err = Romberg(expression,precision,left_interval,right_interval,result,precision_a,times);
		break;

	case GAUSS_LEGENDRE_INDEX:
		err = Gauss_Legendre(expression,result);
		break;
		
	case GAUSS_CHEBYSHEV_INDEX:
		err = Gauss_Chebyshev(expression,result);
		break;
	}

	OutputResult_I(iSelectIndex,err);
	return 0;
}

void Calculus::OutputResult_I(int iSelectIndex,ErrNo err)
{
	PCTSTR		pErrInfo;
	HWND		hwndEdit = GetDlgItem(hDlg,EditOutputID);
	TCHAR		szBuffer[MAXSIZE];

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case ROMBERG_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Romberg�㷨 \r\n"));
		break;
	case GAUSS_LEGENDRE_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Gauss-Legendre�����ʽ(5��ʽ) \r\n"));
		break;
	case GAUSS_CHEBYSHEV_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Gauss-Chebyshev�����ʽ(5��ʽ) \r\n"));
		break;
	}


	swprintf(szBuffer,MAXSIZE,TEXT("������Ϊ��%s\r\n"),exp_store);
	Edit_ReplaceSel(hwndEdit,szBuffer);
	
	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		if(iSelectIndex == ROMBERG_INDEX)
		{
			if(precision_a == precision)
				swprintf(szBuffer,MAXSIZE,TEXT("����ֵ��%f  ���ȣ�%f  ���䣺[%f,%f] \r\n"),result,precision,left_interval,right_interval);
			else
				swprintf(szBuffer,MAXSIZE,TEXT("����ֵ��%f  ���ȣ�%f  �Ѵﵽ���ȣ�%f  ���䣺[%f,%f] \r\n"),result,precision,precision_a,left_interval,right_interval);
		}
		else
		{
			swprintf(szBuffer,MAXSIZE,TEXT("����ֵ��%f  ���䣺[%f,%f] \r\n"),result,left_interval,right_interval);
		}
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));

}

//��������
ErrNo Calculus::GetLine_Double(HWND hwndEdit,double *fParameter)
{
	TCHAR		*ptemp;
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	if(!Judge_Double(Buffer))
		return INCORRECT_DATA;

	*fParameter = wcstod(Buffer,&ptemp);
	return 0;
}