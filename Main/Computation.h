#pragma once

#include "Chart.h"
#include<tchar.h>
#include<float.h>
#include<math.h>
#include<Windows.h>
#include<WindowsX.h>
#include<time.h>
#include<stdarg.h>
#include<CommCtrl.h>
#include "resource.h"

#include "define.h"

//ʹ��GDI+
#include<GdiPlus.h>
using namespace Gdiplus;  
#pragma comment(lib,"gdiplus")

#include<vector>
using std::vector;

//ʹ��CChart
#include "Chart.h"
#ifdef _DEBUG
#if defined(_UNICODE) || defined(UNICODE)
#pragma comment(lib, "PlotDll_ud.lib")
#else
#pragma comment(lib, "PlotDll_d.lib")
#endif
#else
#if defined(_UNICODE) || defined(UNICODE)
#pragma comment(lib, "PlotDll_u.lib")
#else
#pragma comment(lib, "PlotDll.lib")
#endif
#endif

//���Ͷ���
typedef  unsigned int uint;
typedef	 unsigned int ErrNo;
typedef  INT_PTR (CALLBACK  *Tab_Control_Page_Proc)(HWND,UINT,WPARAM,LPARAM);
typedef	 Tab_Control_Page_Proc Page_Proc;

//TabCtrl����
typedef struct TabControl
{
	HWND		hTab;									//Tab Control �ľ��
	HWND		*hTabPages;								//Tab Page �ľ��
	Page_Proc	*page_proc;								//ÿ���Ի�����Ϣ������
	int			tabPageCount;							//ѡ��ĸ���
	LPTSTR		*tabNames;								//ÿ��ѡ�������
	LPTSTR		*dlgNames;								//ÿ��ѡ���Ӧ�Ի����ID
	RECT		rect;
} TABCTRL,*LPTABCTRL;

struct DPOINT
{
	double	x;
	double	fx;				//f(x)
	double	df;				//f'(x)
	double	ddf;			//f''(x)

	DPOINT()
	{
		x	= DBL_MAX;
		fx	= DBL_MAX;
		df	= DBL_MAX;
		ddf = DBL_MAX;
	};
};



//���Ի�����Ϣ������
INT_PTR WINAPI Dlg_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
BOOL Dlg_OnNOTIFY(HWND hDlg, WPARAM wParam,LPARAM lParam);
BOOL Dlg_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam);
BOOL Dlg_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam);

//Tab Control
VOID New_TabControl(LPTABCTRL lptc,HWND hTab,LPTSTR *tabNames,LPTSTR *dlgNames,Page_Proc *page_proc);

BOOL CALLBACK TabPage_Proc_1(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK TabPage_Proc_2(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK TabPage_Proc_3(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK TabPage_Proc_4(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK TabPage_Proc_5(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK TabPage_Proc_6(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);

//��ͼ �Ի���
ErrNo DrawGraph(HWND hDlg,TCHAR *expression,TCHAR variable_symbol,double Left_Interval = -10.0,double Right_Interval = 10.0,double Step_Size = 0.1);

//�ļ�����
void FileInitialize(HWND hwnd);
BOOL FileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName);
BOOL FileSaveDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName);
BOOL FileWrite(HWND hwndEdit,PTSTR pstrFileName);
int DetectCodeType(PCTSTR pstrFileName);		//�ж��ļ���������


//�ඨ��
//�쳣���
class Exception
{
public:
	Exception();
	//n_values:�ɱ�����ĸ���
	void    ErrorReport(HWND hDlg,ErrNo ErrorNumber,const int n_values = 0,...);					//�������
	PCTSTR	FormatError(const ErrNo ErrorNumber);
};

//���������ж�
class  Character_Judegement
{
public:
	bool Judge_Double(const TCHAR *szBuffer) const;
};

//���ʽת��
class Expression
{
#define     E				2.718281828459						//e��ֵ							

private:
	double	eps;
	//һά����
	TCHAR	variable_symbol;									//�Ա�������

public:
	Expression();

public:
	int	  Deal_String(TCHAR *expression);										//������ܵ����ַ���
	bool  InOp(TCHAR ch);														//�ж�ch�Ƿ�Ϊ�����
	int	  Left_Pri(TCHAR op);													//�������������ȼ�
	int	  Rigth_Pri(TCHAR op);													//�������������ȼ�
	int	  Precede_Op(TCHAR op1,TCHAR op2);										//�Ƚ�����������ȼ� 

public:
	//һά����
	void  ChangeVariableSymbol(TCHAR new_variable_symbol);						//�޸ı�������
	ErrNo Trans_Exp(TCHAR *expression,TCHAR *postexp);							//���ʽת��
	ErrNo Character_Filter(TCHAR *expression);									//�жϱ��ʽ�Ƿ�Ƿ��ַ� ���������Ƿ�˫˫���
	ErrNo CompValue(const TCHAR *postexp,double x,double *result);				//����һ��xֵ,���ݱ��ʽ����ֵ
	
public:
	//��ά����
	ErrNo Character_Filter(TCHAR *expression,const TCHAR *variable_symbol_array,const int numofvariable);		//�жϱ��ʽ�Ƿ�Ƿ��ַ� ���������Ƿ�˫˫���
	ErrNo Trans_Exp(TCHAR *expression,TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable);
	ErrNo CompValue(const TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable,double *values,double *result);	
};

//�����㷨
class Matrix_Algorithm
{
private:
	double	eps;
public:
	Matrix_Algorithm();
	void	ConvertMatrix(double **Augmented_Matrix,const int k,const int dimension);

	//Gauss����Ԫ��Ԫ�� Augmented_Matrix:����������� result:���������� determinant:��������ʽֵ dimension:ϵ������ά�� 
	ErrNo	Gauss(double **Augmented_Matrix,double *result,double &determinant,const int dimension);

	// Gauss����Ԫ��Ԫ��
	// ���޸�ԭʼ����matrix
	// matrix:�洢�������(Nx(N+1))
	ErrNo	Gauss_NOC(double **matrix,double *result,double &determinant,const int dimension);

	//LU�ֽⷨ Augmented_Matrix:����������� result:���������� dimension:ϵ������ά��
	ErrNo	LU(double **Augmented_Matrix,double *result,const int dimension);

	// LU�ֽⷨ��ⷽ����
	// ���޸�ԭʼ����matrix
	// matrix:�洢�������( Nx(N+1) )
	ErrNo	LU_NOC(double **matrix,double *result,const int dimension);

	//׷�Ϸ� Augmented_Matrix:����������� result:���������� dimension:ϵ������ά��
	ErrNo	Thomas(double **Tridiagonal_Matrix,double *result,const int dimension);

	// ׷�Ϸ�
	// ���޸�ԭʼ����matrix
	ErrNo	Thomas_NOC(double **matrix,double *result,const int dimension);

	//ƽ������ Augmented_Matrix:����������� result:���������� dimension:ϵ������ά��
	ErrNo	Cholesky(double **Augmented_Matrix,double *result,const int dimension);

	//ƽ������
	//���޸�ԭʼ����matrix
	ErrNo	Cholesky_NOC(double **matrx,double *result,const int dimension);

	//Jacobi������ Augmented_Matrix:����������� iteration_vector:��ʼxֵ(����) result:����������
	//iterations:����������(����ﲻ���ô������ֵ���޸�Ϊʵ�ʵ�������) dimension:ϵ������ά�� precision:��������
	ErrNo	Jacobi(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision);

	//Gauss-Seidel������ Augmented_Matrix:����������� iteration_vector:��ʼxֵ(����) result:����������
	//iterations:����������(����ﲻ���ô������ֵ���޸�Ϊʵ�ʵ�������) dimension:ϵ������ά�� precision:��������
	ErrNo	Gauss_Seidel(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision);

	//SOR������ Augmented_Matrix:����������� iteration_vector:��ʼxֵ(����) result:����������
	//iterations:����������(����ﲻ���ô������ֵ���޸�Ϊʵ�ʵ�������) dimension:ϵ������ά�� precision:�������� relaxationfactor:�ɳ�����(0-2)
	ErrNo   SOR(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,const double relaxationfactor,int &r_interations,double &r_precision);

	// ��������ֵ����(QR�㷨) ע��δ���� (δ����)
	// matrix:������� eigenvalues:����ֵ eigenvectors:�������� nDimension:����ά�� iterations:��������
	ErrNo	QR(double **matrix,double *eigenvalues,double **eigenvectors,const int nDimension,int iterations = 11);
	
	// ��������ֵ����(QR�㷨) ע������
	// matrix:������� eigenvalues:����ֵ eigenvectors:�������� nDimension:����ά��
	ErrNo	QR_Accelerate(double **matrix,double *eigenvalues,const int nDimension,int iterations = 11,const double precision = 1e-6);

	// Householder�任
	// matrix:��֪���� matrix_hess:�����Hessenberg���� matrix_house:�����Householder���� nDimension:matrix��ά��(����)
	ErrNo	Hess(double **matrix,double **matrix_hess,double **matrix_house,int nDimension);

	// QR�ֽ� matrix_hess:Hessenberg���� nDimension:����ά��
	ErrNo	QR_Decomposition(double **matrix_hess,double **matrix_Q,double **matrix_R,int nDimension);

	// ��������������������֪��������ֵ��
	// matrix:�������� eigenvectors:��������(ÿһ��Ϊһ����ֵ��Ӧ����������) eigenvalues:����ֵ(��֪) dimension:����ά��(����)
	ErrNo	matrix_eigenvectors(double **matrix,double **eigenvectors,double *eigenvalues,const int dimension);

	//ԭ��ƽ�Ƶķ��ݷ�(��ĳ������ֵ����������)
	ErrNo	InvPowerMeth(double **matrix,double *eigenvector,const double eigenvalue,const int dimension,const double precision = 1e-6,const int iteration = 50);

	//���ź���
	double	sign(double x);
	// ���������� 2-����
	double	sqrt_vector(const double *vector_m,const int n);
	// ������
	double	dot_product(const double *vector_m,const int n);
	// ���������о���ֵ����һ��
	double	max_vector(double *vector_t,const int n);
	// ���������о���ֵ����һ����ֵ���±�(��0��ʼ)
	  int	max_vector_index(double *vector_t,const int n);
	//�������ɵ�λ����
	void	create_identity_matrix(double **matrix,int nDimension);	
	//һ�����˷� A = B * C
	bool	multiply_matrix(double **pmatrix_a,double **pmatrix_b,double **pmatrix_c,int row_a,int col_a,int row_b,int col_b,int row_c,int col_c);		
	// ����˷�(���) matrix_dest = matrix_dest * matrix_src
	bool	multiply_matrix_left(double **pdest_matrix,double **psrc_matrix,int dim_dest,int dim_src);
	// ����˷�(�ҳ�) matrix_dest = matrix_src * matrix_dest
	bool	multiply_matrix_right(double **psrc_matrix,double **pdest_matrix,int dim_src,int dim_dest);
	// ������з���
	double	matrix_norm_r(double **matrix,const int nDimension);

	//���������㷨(С����)
	void	QuickSort_STL(double *vector,int s,int t);
	//���������㷨(��С)
	void	QuickSort_LTS(double *vector,int s,int t);
};

//���㷽�̸��㷨
class Equation_ALgorithm : public Expression
{
private: 
	double	eps;

public:
	Equation_ALgorithm(TCHAR variable_symbol = TEXT('x'));
	//����������� expression:���ʽ result:������ max_iterations:����������(����δ�ﵽ������ʱ�ﵽ��������,���ֵ���޸�Ϊ�Դﵽ�ĵ�������) 
	//precision:��������(�����ﵽ������ʱ,�����Ͼ���,ֵ����,�������Ͼ���,ֵ�޸�Ϊ���ﵽ�ľ���) initial_value:��ʼֵ 
	ErrNo   Fixed_Point_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);

	//ţ�ٵ����� expression:���ʽ result:������ max_iterations:����������(����δ�ﵽ������ʱ�ﵽ��������,���ֵ���޸�Ϊ�Դﵽ�ĵ�������) 
	//precision:��������(�����ﵽ������ʱ,�����Ͼ���,ֵ����,�������Ͼ���,ֵ�޸�Ϊ���ﵽ�ľ���) initial_value:��ʼֵ 
	ErrNo	Newton_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);	

	//Aitken���ٷ��� expression:���ʽ result:������ max_iterations:����������(����δ�ﵽ������ʱ�ﵽ��������,���ֵ���޸�Ϊ�Դﵽ�ĵ�������) 
	//precision:��������(�����ﵽ������ʱ,�����Ͼ���,ֵ����,�������Ͼ���,ֵ�޸�Ϊ���ﵽ�ľ���) initial_value:��ʼֵ
	ErrNo	Aitken(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);	

	//Steffensen������ expression:���ʽ result:������ max_iterations:����������(����δ�ﵽ������ʱ�ﵽ��������,���ֵ���޸�Ϊ�Դﵽ�ĵ�������) 
	//precision:��������(�����ﵽ������ʱ,�����Ͼ���,ֵ����,�������Ͼ���,ֵ�޸�Ϊ���ﵽ�ľ���) initial_value:��ʼֵ
	ErrNo	Steffensen(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);																								
};

//��ֵ�㷨
class Interpolation_Algorithm : public Matrix_Algorithm
{
private:
	double	eps;
public:
	Interpolation_Algorithm();
	//Lagrange��ֵ
	ErrNo Lagrange(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//Newton��ֵ 
	ErrNo Newton(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//Hermite��ֵ
	ErrNo Hermite(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//�ֶ����Բ�ֵ
	ErrNo Piecewise_Linear_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//�ֶ�Hermite��ֵ
	ErrNo Piecewise_Hermite_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//������ֵ-����ط�(�߽��������������˵��һ�׵�����֪)
	//dpoints:��ֵ�� compvalue:Ҫ����x��ֵ degree:����n(��ֵ�����n+1 ) eps:�жϳ����Ƿ���Ƶ����� result:������
	ErrNo	Piecewise_Spline_Interpolation_DF(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//������ֵ-����ط����߽��������������˵�Ķ��׵�����֪)
	//dpoints:��ֵ�� compvalue:Ҫ����x��ֵ degree:����n(��ֵ�����n+1 ) eps:�жϳ����Ƿ���Ƶ����� result:������
	ErrNo	Piecewise_Spline_Interpolation_DDF(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
};

//����㷨
class Fitting_Algorithm:public Matrix_Algorithm,Expression
{
private:
	double eps;
public:
	// ��С������Ϸ�
	ErrNo Least_Square_Fitting(PTSTR *polynomial,double *parameter,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps);
	// ������������ʽ���
	ErrNo Polynominal_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const int degree,const double eps);
	// ָ��ģ�͵���С������� s(x) = a*exp(b*x)
	ErrNo Exponential_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const double eps);

public:
	// ������������ʽ��ĳ��x��ֵ k:��k������ʽ k=0,1,2,...
	// dPoints_F:��ϵ� n:��ϵ���� x:Ҫ�����xֵ eps:�жϳ����Ƿ�Ϊ0
	// ��������Ϊ0,����INF(INF = DBL_MAX)
	double Polynominal_Recursion(const DPOINT *dPoints_F,const int n,const int k,const double x,const double eps);
};

//΢�ּ��㷽��
class Differential_Algorithm : public Expression
{
private:
	double eps;

public:
	Differential_Algorithm();

public:
	// Taylor��ʽ����ֵ΢�ֹ�ʽ
	// ��ǰ����
	// result:������ expression:�洢���ʽf(x)���ַ���(��xΪ��������) x:Ҫ�����ֵ interval:���
	ErrNo Forward_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor��ʽ����ֵ΢�ֹ�ʽ
	// ������
	// result:������ expression:�洢���ʽf(x)���ַ���(��xΪ��������) x:Ҫ�����ֵ interval:���
	ErrNo Backward_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor��ʽ����ֵ΢�ֹ�ʽ
	// ���Ĳ���
	// result:������ expression:�洢���ʽf(x)���ַ���(��xΪ��������) x:Ҫ�����ֵ interval:���
	ErrNo Central_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor��ʽ����ֵ΢�ֹ�ʽ
	// �������Ĳ���
	// result:������ expression:�洢���ʽf(x)���ַ���(��xΪ��������) x:Ҫ�����ֵ interval:���
	ErrNo Second_Order_Central_Difference(double &result,const TCHAR* expression,const double x,const double interval);
};

//���ּ��㷽��
class Integral_Algorithm : public Expression
{
private:
	int		max_times;
	double	eps;
public:
	Integral_Algorithm();
	int		pow_int(int x,int n);
	//Romberg�㷨
	ErrNo		Romberg(TCHAR *expression,const double precision,const double left_interval,const double right_interval,double &result,double &precision_a,int &times);
	//Gauss-Legendre�����ʽ(5��ʽ��
	ErrNo		Gauss_Legendre(TCHAR *expression,double &result);
	//Gauss-Chebyshev�����ʽ(5��ʽ)
	ErrNo		Gauss_Chebyshev(TCHAR *expression,double &result);
};

//��΢�ַ�����ֵ�ⷨ
class ODE_Algorithm : public Expression
{
private:
	double	eps;
public:
	ODE_Algorithm();
public:
	//Euler��������ʽ�������� expression:δת����ԭʼ���ʽ results:2xn������ initial_value:��ʼֵ step_size:���� left_interval:������˵�ֵ right_interval:�����Ҷ˵�ֵ variable_symbol_array:������������
	ErrNo	Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//�Ľ���Euler���� expression:δת����ԭʼ���ʽ results:2xn������ initial_value:��ʼֵ step_size:���� left_interval:������˵�ֵ right_interval:�����Ҷ˵�ֵ variable_symbol_array:������������
	ErrNo	Improved_Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//4��Runge-Kutta����
	ErrNo	Runge_Kutta(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//4��AdamasԤ��-У����ʽ�����׸�ʽ��
	ErrNo	Simple_Adams_PC(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//Adams4��4����ʽ��ʽ
	ErrNo	Adams(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//Milne4��4����ʽ��ʽ
	ErrNo	Milne(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//������Milne-HammingԤ��-У����ʽ
	ErrNo	Milne_Hamming(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
};

//PCA�㷨
class PCA_Algorithm : public Matrix_Algorithm
{
private:
	double eps;
public:
	PCA_Algorithm();
	//pricinpal:�������ɷ�(δ����ռ�) k:�������ɷָ���
	ErrNo PCA_A(double **matrix,const int rows,const int columns,double **&principle,int &k);
	ErrNo PCA_A(double **matrix,const int rows,const int columns,double **matrix_r,double *eigenvalues,double **&principle,int &k);
	//��׼��������ԭ����Ļ������޸ģ�
	ErrNo Standardize_Matrix(double **matrix,const int rows,const int columns);
	//��ά matrix[rows][columns] --> matrix_r[columns][columns]
	ErrNo Relevance_Matrix(double **matrix,double **matrix_r,int rows,int columns);
};

//�ڶ� ������(���������)
class Matrix_Comp : public Exception
{
public:
	ErrNo	Algorithm_Choice(int iSelectIndex);					//�㷨ѡ��
	BOOL	CharacterJudgement(LPTSTR szBuffer);				//�ַ��ж�(����жϺ�����)	
	ErrNo	GetStart(int iSelectIndex);							//��ʼ����
	ErrNo	AllocateMemory(int iSelectIndex);					//����ռ�
	//ErrNo	GetMatrix();										//����������ݻ�ȡ
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//����ı�����ؼ�һ������(����)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//����ı�����ؼ�һ������(����)
	ErrNo	GetThreeLine();										//������ԽǾ��������
	ErrNo   DestroyPointer();					//����ָ��
	ErrNo	OutPutResult(ErrNo err = 0);										//���������
	Matrix_Comp(HWND hDlg);										//���캯��

public:
	//������
	//��ȡ�������� pMatrix:�洢�������� nOutput:������������ı����λ�� nRows:�������� nColumns:��������
	ErrNo	GetMatrix(const HWND hwndEdit,double **pMatrix,int &nOutput,const int nRows,const int nColumns);
	ErrNo	AllocateMemory(double **&pointer,int nRows,int nColumns);
	void	DestroyPointer(double **&pointer,int nRows);
	void	DestroyPointer(double *&pointer);

public:
	//�ļ���������
	ErrNo	GetStart_File(TCHAR **FilesPath,const int nCount);
	ErrNo	GetStart_File_ANSI(PCTSTR pstrFilePath);	//ANSI�ļ�
	ErrNo	GetStart_File_UTF_LE(PCTSTR pstrFilePath);	//UTF16-LE�ļ�
	ErrNo	StringToNumbericArr(PCTSTR szBuffer,PCTSTR seps,double *n_arr,const int n);	//��һ���ַ���תΪ�洢double���ݵ�����
	void	OutPutResult_File();								//�ļ��л�����ݵõ�������
	//ErrNo	GetMatrix_ANSI(HWND hwndEdit,PCTSTR pstrFileName,double **matrix);
	//ErrNo	GetMatrix_UTF_LE(HWND hwndEdit,PCTSTR pstrFileName,double **matrix);

	//�û�����
private:
	int		iDimension;											//����ά��
	int		iOutput;											//������Ӧ�����뵽�ĵط� 
	int		iIndex;												//�㷨ѡ���ĸ�

	//�ļ���ȡʹ��
private:
	int		nRows;												//��������
	int		nColumns;											//��������
	int		algorithm;											//�㷨���

	//����
private:
	HWND	hwndDlg;											//�Ի�����
	double	runtime;											//�㷨����ʱ��
	double	*iteration_vector;									//������ʼ����
	double	*result;											//������
	double	**matrix;											//��ͨ����洢
	double  *eigenvalues;										//����ֵ�洢
	double  **eigenvectors;										//���������洢
	double  determinant;										//��������ʽ
	int		iterations;											//��������
	double	precision;											//��������
	int		r_iterations;										//������̴ﵽ�ĵ�������
	double  r_precision;										//������̴ﵽ�ĵ�������
	double  relaxationfactor;									//�ɳ�����
};

//������(һԪ���̼���)
class Equation : public Exception,Equation_ALgorithm
{
public:
	Equation(HWND hDlg);
	ErrNo	Algorithm_Choice(int algorithm);
	bool	Character_Judgement(const TCHAR *szBuffer);		    //�ж����������Ƿ�����
	ErrNo	GetStart(int iSelectIndex);
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//����ı�����ؼ�һ������(����)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//����ı�����ؼ�һ������(������)
	ErrNo	OutputResult(int iSelectIndex);						//������

public:
	//�ļ���������
	ErrNo	GetStart_File(TCHAR **FilesPath,const int nCount);
	ErrNo	GetStart_File_ANSI(PCTSTR pstrFilePath);	//ANSI�ļ�
	ErrNo	GetStart_File_UTF_LE(PCTSTR pstrFilePath);	//UTF16-LE�ļ�
	void	OutPutResult_File(int algorithm_num);								//�ļ��л�����ݵõ�������

private:
	HWND	hwndDlg;
	TCHAR	var_symbol;											//��������
	TCHAR	var_symbol_save;									//�������ű���
	TCHAR	formula[MAXINPUT];									//������ʽ
	TCHAR	formula_store[MAXINPUT];							//�����ȡ�ı��ʽ
	double	original_precision;									//�����������
	double	precision;											//��������
	double  initial_value;										//������ʼֵ
	double  result;												//������
	double	runtime;											//����ʱ��
	int		original_iterations;									//��������������
	int		max_iterations;										//����������

private:
	int		algorithm;											//�㷨���
};

//�����º͵����£���ֵ����ϣ�
class InterpolationAndFitting : public Exception,Character_Judegement
{
public:
	InterpolationAndFitting();
	InterpolationAndFitting(HWND hwndDlg);

public:
	ErrNo	Algorithm_Choice_I(int iSelectIndex,const DPOINT *dpoints_i,const double compvalue,const int degree,double &result);					//�㷨ѡ��
	ErrNo	Algorithm_Choice_F(PTSTR *polynomial,double *parameters,int iSelectIndex,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps);
	void	DestroyPointer(DPOINT *&dpoints);						//����ָ��
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);				//����ı�����ؼ�һ������(����)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);		//����ı�����ؼ�һ������(������)
	ErrNo	GetStart_I(int iSelectIndex);							//���㿪ʼ ������ֵ
	ErrNo   GetStart_F(int iSelectIndex);							//���㿪ʼ �������
	ErrNo	GetPoints_I(DPOINT *dPoints,const HWND hListView,const int n,const int SelectIndex);		//��ȡ��ֵ��(n+1����ֵ��)
	ErrNo	GetPoints_F(DPOINT *dPoints,const HWND hListView,const int SelectIndex);					//��ȡ��ϵ�
	ErrNo	GetPolynomial_F(PTSTR *polynomial,PTSTR szBuffer,const int nMax,const int nNumofPolynomial);//��ȡ��ֵ����ʽ	
	void	OutputResult_I(int iSelectIndex,ErrNo err);						//������
	void	OutputResult_F(int iSelectIndex,ErrNo err);

public:
	void	QuickSort(DPOINT *dPoints,int s,int t);					//��������
private:
	HWND	hwndDlg;												//�Ի�����
	DPOINT	*dpoints_I;												//��ֵ�� ������ֵ
	DPOINT  *dPoints_F;												//��ֵ�� �������	
	double	compvalue;												//�����
	double	result;													//������
	double  eps;													//�жϳ����Ƿ�Ϊ��
	double  *parameters;											//��ϼ�����Ĳ���
	TCHAR	**polynomial;											//��ֵ����ʽ
	int		nNumofPolynomial;										//����ʽ����
	int		nNumofPoints;
	int		degree;													//����(��������ʽ�����Ͳ�ֵ����)
};

//�����£�΢���ּ���)
class Calculus : public Exception,Character_Judegement,Integral_Algorithm
{
	//΢�ּ���
private:
	double		value;					//�����
	double		interval;				//���(h)

	//���ּ���
private:
	int			times;					//���������ʱ�ﵽ�Ĵ���
	double		left_interval;			//������
	double		right_interval;			//������
	double		precision;				//����Ҫ��ľ���
	double		precision_a;			//�����ﵽ�ľ���
	

	//����
private:
	HWND		hDlg;					//�Ի�����
	double		eps;					//�жϳ����Ƿ�Ϊ0
	double		result;					//������
	TCHAR		expression[MAXSIZE];	//��õı��ʽ
	TCHAR		exp_store[MAXSIZE];		//��õı��ʽ����(�ڱ��ʽ�����У�ԭ���ʽ�洢�ĵط����ݱ��޸�)

public:
	Calculus(HWND hwndDlg);	

	//΢�ּ���
public:
	ErrNo		Algorithm_Choice_D(int iSelectIndex);					//΢���㷨ѡ��
	ErrNo		GetStart_D(int iSelectIndex);							//΢�ּ���
	void		OutputResult_D(int iSelectIndex,ErrNo err = 0);						//�����������΢�֣�

	//���ּ���
public:		
	ErrNo		GetStart_I(int iSelectIndex);							//���ּ���
	void		OutputResult_I(int iSelectIndex,ErrNo err = 0);						//��������������֣�

	//��������
public:
	ErrNo		GetLine_Double(HWND hwndEdit,double *fParameter);		//��ȡ�����������ĸ�����
};

//�ڰ���(��΢�ַ��̼���)
class ODE : public Exception,Character_Judegement,ODE_Algorithm
{
private:
	double	eps;
	double  **results;
	double	left_interval;
	double	right_interval;
	double	initial_value;
	double	step_size;
	int		num_x;
	TCHAR	exp_store[MAXSIZE];
	HWND	hDlg;
public:
	ODE(HWND hwndDlg);
	ErrNo	GetStart(int iSelectIndex);							//��ʼ����	
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//����ı�����ؼ�һ������(����)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//����ı�����ؼ�һ������(������)
	ErrNo	OutputResults(const int iSelectIndex,ErrNo err = 0);
};

//PCA
class PCA : public PCA_Algorithm,Exception
{
private:
	HWND hwndDlg;
	int nsamples;			// ��������	
	int nindexs;			// ָ�����
	int k;					// ���ɷָ���
	double *eigenvalues;	//����ֵ
	double **matrix_r;		//���ϵ������
	double **matrix_data;  // ������������
	double **principal;		// �������ɷ�����

private:
	int nOutput;

public:
	PCA(HWND hDlg);
	ErrNo GetStart_Calc();		//��ʼ����
	ErrNo AllocateMemory(double **&matrix,int rows,int columns);		//����ռ�
	void Destroy(double **&matrix,int rows);	//���ٿռ�
	BOOL CharacterJudgement(LPTSTR szBuffer);
	ErrNo GetMatrix(const HWND hwndEdit,double **pMatrix,const int nRows,const int nColumns);
	ErrNo GetLine_Int(HWND hwndEdit,int *iParameter);
	void OutPutResult();
};