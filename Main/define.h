#pragma once

//��Ϣ������
#define HANDLE_DLGMSG(hwnd,message,wParam,lParam,fn)	\
	case (message): return HANDLE_DLG_##message((hwnd),(wParam),(lParam),(fn))
#define HANDLE_DLG_WM_NOTIFY(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))
#define HANDLE_DLG_WM_INITDIALOG(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))
#define HANDLE_DLG_WM_CLOSE(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))
#define HANDLE_DLG_WM_COMMAND(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))
#define HANDLE_DLG_WM_PAINT(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))
#define HANDLE_DLG_WM_CONTEXTMENU(hwnd,wParam,lParam,fn)		\
	(fn)((hwnd),(wParam),(lParam))

//�궨��
#define	  MAXINPUT			256						//�ַ���������С
#define	  MAXSIZE			256
#define	  NUMOFPOINT		500						//��������
#define	  PI				3.1415926				//PIֵ
#define	  INF				DBL_MAX					//�����

//�������
//���뵯���Ĵ���1-100
#define	  NO_DATA_INPUT				1				//û����������
#define	  INCORRECT_DATA			2				//�������ݷǷ�
#define	  WRONG_MATRIX				3				//�����������
#define	  WRONG_EXPRESSION			4				//���ʽ����
#define   NOT_MATCH_DEGREE			5				//�������ݸ����������ƥ��
#define	  NOT_MATCH					6				//�������ݸ�������
#define	  CAN_NOT_OPEN_FILE			7				//�򲻿��ļ�
#define	  INCORRECT_DATA_FILE		8				//�������ݷǷ�(�ļ�)
#define	  READ_FILE_ERR				9				//��ȡ�ļ����ݳ���
#define	  NOT_ENOUGH_PARA			10				//����̫��
#define	  NOT_MATCH_ALGORITHM		11				//û��ƥ����㷨

//�����������Ĵ��� 101��ʼ
#define	  ALLOCATE_MEMORY_FAIL		101				//�����ڴ�ʧ��
#define   DIVIDE_BY_ZERO			102				//����Ϊ��
#define	  NEGATIVE_NUMBER			103				//����ʱ��������Ϊ������Ը��������
#define	  BRACKET_NOT_MATCH			104				//���Ų����
#define	  NOT_REQUIRED_SOLUTION     105				//����δ��ôﵽҪ��Ľ�
#define   DATA_OVERFLOW				106				//��������������
#define	  ARRAY_OVERFLOW			107				//�������
#define   DIVERGENCY				108				//����������
#define	  NO_RESULT					109				//���㲻�����
#define	  NOT_IN_INTERVAL			110				//���ڼ���������
#define	  SMALL_INTERVAL			111				//�����С




//�㷨��ComboBox��Index	���ڶ������£�
#define	  GAUSS_INDEX				0
#define   LU_INDEX					1
#define	  THOMAS_INDEX				2
#define	  CHOLESKY_INDEX			3
#define	  JACOBI_INDEX				4
#define	  GAUSS_SEIDEL_INDEX		5
#define	  SOR_INDEX					6
#define	  EIGENVALUES_INDEX			7

//�㷨��ComboBox��Index	�������£�
#define	  FIXED_POINT_ITERATION_INDEX			0			//�����������
#define	  NEWTON_ITERATION_INDEX				1			//Newton������
#define	  AITKEN_INDEX							2			//Aitken���ٷ���
#define	  STEFFENSEN_INDEX						3			//Steffensen������

//�㷨��ComboBox��Index	����������£�
#define   LAGRANGE_INTERPOLATION_INDEX			0           //Lagrange��ֵ
#define	  NEWTON_INTERPOLATION_INDEX			1		    //Newton��ֵ
#define   HERMITE_INTERPOLATION_INDEX			2			//Hermite��ֵ
#define	  PIECEWISE_LINEAR_INTERPOLATION		3			//�ֶ����Բ�ֵ
#define	  PIECEWISE_HERMITE_INTERPOLATION		4			//�ֶ�Hermite��ֵ
#define	  PIECEWISE_SPLINE_INTERPOLATION_DF		5			//�ֶ�������ֵ(��֪���˵��һ�׵���)
#define	  PIECEWISE_SPLINE_INTERPOLATION_DDF	6			//�ֶ�������ֵ(��֪���˵�Ķ��׵���)

#define   LEAST_SQUARE_FITTING_INDEX			0			//������С�������
#define	  POLYNOMINAL_FITTING_INDEX				1			//��������ʽ���
#define	  EXPONENTIAL_FITTING_INDEX				2			//ָ��ģ��

//�㷨��ComboBox��Index�������� ΢���ּ��㣩
//΢�ּ���
#define	  FORWARD_DIFFERENCE_INDEX				0			//Taylor����΢�� ��ǰ���
#define	  BACKFORWARD_DIFFERENCE_INDEX			1			//Taylor����΢�� �����
#define   CENTRAL_DIFFERENCE_DIFFERENCE_INDEX	2			//Taylor����΢�� ���Ĳ��
#define   SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX 3			//Taylor����΢�� �������Ĳ��


//���ּ���
#define	  ROMBERG_INDEX							0			//Romberg�㷨
#define	  GAUSS_LEGENDRE_INDEX					1			//Gauss-Lagendre�����ʽ
#define	  GAUSS_CHEBYSHEV_INDEX					2			//Gauss-Chebyshev�����ʽ

//�㷨��ComboBox��Index���ڰ��£�
#define	  EULER_INDEX							0			//Euler��������ʽ��������
#define	  IMPROVED_EULER_INDEX					1			//�Ľ���Euler����
#define	  RUNGE_KUTTA_INDEX						2			//4��Runge-Kutta����
#define	  SIMPLE_ADAMS_PC_INDEX					3			//4��AdamsԤ��-У����ʽ(���׸�ʽ)
#define	  MILNE_HAMMING_INDEX					4			//������Milne-HammingԤ��-У����ʽ
#define	  ADAMS_INDEX							5			//Adams4��4����ʽ��ʽ
#define	  MILNE_INDEX							6			//Milne4��4����ʽ��ʽ



enum
{
	CODE_TYPE_UNICODE_LE,		//UTF-16LE
	CODE_TYPE_UNICODE_BE,		//UTF-16BE
	CODE_TYPE_UTF8,				//UFT-8
	CODE_TYPE_ANSI				//ANSI
};

