#include "Computation.h"
#include "ODE.h"

ODE::ODE(HWND hwndDlg)
{
	eps = 1e-6;
	hDlg = hwndDlg;
}

ErrNo ODE::GetStart(int iSelectIndex)
{
	ErrNo	err;
	TCHAR	variable_symbol_array[2];
	TCHAR	szBuffer[MAXSIZE];
	double	x;
	int		iLength;

	variable_symbol_array[0] = TEXT('x');
	variable_symbol_array[1] = TEXT('y');

	//��ȡ���ʽ
	if(!(iLength = Edit_GetText(GetDlgItem(hDlg,EditControlID[0]),szBuffer,MAXSIZE-1)))
	{
		ErrorReport(hDlg,NO_DATA_INPUT);
		return NO_DATA_INPUT;
	}
	for(int i=0;i<=lstrlen(szBuffer);i++)
		exp_store[i] = szBuffer[i];

	//��ȡ�������
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[1]),&left_interval))			//�������
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[2]),&right_interval))			//�����Ҷ�
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(left_interval >= right_interval || fabs(right_interval - left_interval)< eps)
	{
		MessageBox(hDlg,TEXT("������������"),TEXT("����"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//��ȡ��ʼֵ
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[3]),&initial_value))
	{
		ErrorReport(hDlg,err);
		return err;
	}

	//��ȡ����
	if(err = GetLine_Double(GetDlgItem(hDlg,EditControlID[4]),&step_size))
	{
		ErrorReport(hDlg,err);
		return err;
	}
	if(step_size < 0 || fabs(step_size) < eps)
	{
		MessageBox(hDlg,TEXT("������������"),TEXT("����"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//��ȡ��ָ������Ҫ����ĵ�ĸ���
	num_x = 0;
	x = left_interval;
	while(1)
	{
		x += step_size;
		if(x > right_interval && fabs(x-right_interval)>eps)
			break;
		num_x++;
	}
	if(abs(num_x) == 0)
	{
		MessageBox(hDlg,TEXT("��������̫��"),TEXT("����"),MB_ICONERROR);
		return INCORRECT_DATA;
	}

	//����ռ�
	if(!(results = new double* [2]))
	{
		ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
		return ALLOCATE_MEMORY_FAIL;
	}
	for(int i=0;i<2;i++)
	{
		if(!(results[i] = new double [num_x+1]))
		{
			ErrorReport(hDlg,ALLOCATE_MEMORY_FAIL);
			return ALLOCATE_MEMORY_FAIL;
		}
	}

	err  = 0;
	switch(iSelectIndex)
	{
	case EULER_INDEX:
		err = Euler(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case IMPROVED_EULER_INDEX:
		err = Improved_Euler(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case RUNGE_KUTTA_INDEX:
		err = Runge_Kutta(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case SIMPLE_ADAMS_PC_INDEX:
		err = Simple_Adams_PC(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case MILNE_HAMMING_INDEX:
		err = Milne_Hamming(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case ADAMS_INDEX:
		err = Adams(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;

	case MILNE_INDEX:
		err = Milne(szBuffer,results,initial_value,step_size,left_interval,right_interval,variable_symbol_array);
		break;
	}

	OutputResults(iSelectIndex,err);

	for(int i=0;i<2;i++)
		delete [] results[i];
	delete [] results;
	return 0;
}

ErrNo ODE::GetLine_Double(HWND hwndEdit,double *fParameter)
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

ErrNo ODE::GetLine_Int(HWND hwndEdit,int *iParameter)
{
	int			iLength;
	TCHAR		Buffer[MAXINPUT];

	iLength = Edit_GetText(hwndEdit,Buffer,MAXINPUT-1);
	if(!iLength)
		return NO_DATA_INPUT;

	*iParameter = _wtoi(Buffer);

	return 0;
}

ErrNo ODE::OutputResults(const int iSelectIndex,ErrNo err)
{
	PCTSTR	pErrInfo;
	HWND	hwndEdit = GetDlgItem(hDlg,OutputResultID);
	TCHAR	szBuffer[MAXSIZE];

	Edit_SetSel(hwndEdit,0,0);
	switch(iSelectIndex)
	{
	case EULER_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Euler����(��ʽ������) \r\n"));
		break;
	case IMPROVED_EULER_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: �Ľ���Euler���� \r\n"));
		break;
	case RUNGE_KUTTA_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: 4��Runge-Kutta���� \r\n"));
		break;
	case MILNE_HAMMING_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: ������Milne-HammingԤ��-У����ʽ \r\n"));
		break;
	case SIMPLE_ADAMS_PC_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: 4��AdamsԤ��-У����ʽ(���׸�ʽ) \r\n"));
		break;
	case ADAMS_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Adams4��4����ʽ��ʽ \r\n"));
		break;
	case MILNE_INDEX:
		Edit_ReplaceSel(hwndEdit,TEXT("ѡ����㷨: Milne4��4����ʽ��ʽ \r\n"));
		break;
	}

	if(err)
	{
		pErrInfo = FormatError(err);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����ID:%d\r\n������Ϣ:%s\r\n"),err,pErrInfo);
		Edit_ReplaceSel(hwndEdit,szBuffer);
	}
	else
	{
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("����΢�ַ���Ϊ: y'=%s   y(%f)=%f \r\n"),exp_store,left_interval,initial_value);
		//swprintf(szBuffer,MAXSIZE,TEXT("����΢�ַ���Ϊ: y'=%s   y(%f)=%f \r\n"),exp_store,left_interval,initial_value);
		Edit_ReplaceSel(hwndEdit,szBuffer);
		_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("�������Ϊ: ����Ϊ[%f,%f] ����Ϊ%f \r\n"),left_interval,right_interval,step_size);
		//swprintf(szBuffer,MAXSIZE,TEXT("�������Ϊ: ����Ϊ[%f,%f] ����Ϊ%f \r\n"),left_interval,right_interval,step_size);
		Edit_ReplaceSel(hwndEdit,szBuffer);
		for(int i=0;i<=num_x;i++)
		{
			_sntprintf_s(szBuffer,_countof(szBuffer),MAXSIZE,TEXT("x=%f  y=%f \r\n"),results[0][i],results[1][i]);
			//swprintf(szBuffer,MAXSIZE,TEXT("x=%f  y=%f \r\n"),results[0][i],results[1][i]);
			Edit_ReplaceSel(hwndEdit,szBuffer);
		}
	}	
	Edit_ReplaceSel(hwndEdit,TEXT("++++++++++++++++++++++++++++++++++++++\r\n"));
	return 0;
}