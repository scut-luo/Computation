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

//使用GDI+
#include<GdiPlus.h>
using namespace Gdiplus;  
#pragma comment(lib,"gdiplus")

#include<vector>
using std::vector;

//使用CChart
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

//类型定义
typedef  unsigned int uint;
typedef	 unsigned int ErrNo;
typedef  INT_PTR (CALLBACK  *Tab_Control_Page_Proc)(HWND,UINT,WPARAM,LPARAM);
typedef	 Tab_Control_Page_Proc Page_Proc;

//TabCtrl定义
typedef struct TabControl
{
	HWND		hTab;									//Tab Control 的句柄
	HWND		*hTabPages;								//Tab Page 的句柄
	Page_Proc	*page_proc;								//每个对话框消息处理函数
	int			tabPageCount;							//选项卡的个数
	LPTSTR		*tabNames;								//每个选项卡的名字
	LPTSTR		*dlgNames;								//每个选项卡对应对话框的ID
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



//主对话框消息处理函数
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

//画图 对话框
ErrNo DrawGraph(HWND hDlg,TCHAR *expression,TCHAR variable_symbol,double Left_Interval = -10.0,double Right_Interval = 10.0,double Step_Size = 0.1);

//文件处理
void FileInitialize(HWND hwnd);
BOOL FileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName);
BOOL FileSaveDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName);
BOOL FileWrite(HWND hwndEdit,PTSTR pstrFileName);
int DetectCodeType(PCTSTR pstrFileName);		//判断文件编码类型


//类定义
//异常输出
class Exception
{
public:
	Exception();
	//n_values:可变参数的个数
	void    ErrorReport(HWND hDlg,ErrNo ErrorNumber,const int n_values = 0,...);					//错误输出
	PCTSTR	FormatError(const ErrNo ErrorNumber);
};

//输入数据判断
class  Character_Judegement
{
public:
	bool Judge_Double(const TCHAR *szBuffer) const;
};

//表达式转换
class Expression
{
#define     E				2.718281828459						//e的值							

private:
	double	eps;
	//一维情形
	TCHAR	variable_symbol;									//自变量符号

public:
	Expression();

public:
	int	  Deal_String(TCHAR *expression);										//处理接受到的字符串
	bool  InOp(TCHAR ch);														//判断ch是否为运算符
	int	  Left_Pri(TCHAR op);													//获得左运算符优先级
	int	  Rigth_Pri(TCHAR op);													//获得右运算符优先级
	int	  Precede_Op(TCHAR op1,TCHAR op2);										//比较运算符的优先级 

public:
	//一维情形
	void  ChangeVariableSymbol(TCHAR new_variable_symbol);						//修改变量符号
	ErrNo Trans_Exp(TCHAR *expression,TCHAR *postexp);							//表达式转换
	ErrNo Character_Filter(TCHAR *expression);									//判断表达式是否非法字符 还有括号是否双双配对
	ErrNo CompValue(const TCHAR *postexp,double x,double *result);				//给定一个x值,根据表达式计算值
	
public:
	//多维情形
	ErrNo Character_Filter(TCHAR *expression,const TCHAR *variable_symbol_array,const int numofvariable);		//判断表达式是否非法字符 还有括号是否双双配对
	ErrNo Trans_Exp(TCHAR *expression,TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable);
	ErrNo CompValue(const TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable,double *values,double *result);	
};

//矩阵算法
class Matrix_Algorithm
{
private:
	double	eps;
public:
	Matrix_Algorithm();
	void	ConvertMatrix(double **Augmented_Matrix,const int k,const int dimension);

	//Gauss列主元消元法 Augmented_Matrix:增广矩阵数据 result:计算结果保存 determinant:矩阵行列式值 dimension:系数矩阵维数 
	ErrNo	Gauss(double **Augmented_Matrix,double *result,double &determinant,const int dimension);

	// Gauss列主元消元法
	// 不修改原始矩阵matrix
	// matrix:存储增广矩阵(Nx(N+1))
	ErrNo	Gauss_NOC(double **matrix,double *result,double &determinant,const int dimension);

	//LU分解法 Augmented_Matrix:增广矩阵数据 result:计算结果保存 dimension:系数矩阵维数
	ErrNo	LU(double **Augmented_Matrix,double *result,const int dimension);

	// LU分解法求解方程组
	// 不修改原始矩阵matrix
	// matrix:存储增广矩阵( Nx(N+1) )
	ErrNo	LU_NOC(double **matrix,double *result,const int dimension);

	//追赶法 Augmented_Matrix:增广矩阵数据 result:计算结果保存 dimension:系数矩阵维数
	ErrNo	Thomas(double **Tridiagonal_Matrix,double *result,const int dimension);

	// 追赶法
	// 不修改原始矩阵matrix
	ErrNo	Thomas_NOC(double **matrix,double *result,const int dimension);

	//平凡根法 Augmented_Matrix:增广矩阵数据 result:计算结果保存 dimension:系数矩阵维数
	ErrNo	Cholesky(double **Augmented_Matrix,double *result,const int dimension);

	//平方根法
	//不修改原始矩阵matrix
	ErrNo	Cholesky_NOC(double **matrx,double *result,const int dimension);

	//Jacobi迭代法 Augmented_Matrix:增广矩阵数据 iteration_vector:初始x值(向量) result:计算结果保存
	//iterations:最大迭代次数(如果达不到该次数则该值被修改为实际迭代次数) dimension:系数矩阵维数 precision:迭代精度
	ErrNo	Jacobi(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision);

	//Gauss-Seidel迭代法 Augmented_Matrix:增广矩阵数据 iteration_vector:初始x值(向量) result:计算结果保存
	//iterations:最大迭代次数(如果达不到该次数则该值被修改为实际迭代次数) dimension:系数矩阵维数 precision:迭代精度
	ErrNo	Gauss_Seidel(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,int &r_interations,double &r_precision);

	//SOR迭代法 Augmented_Matrix:增广矩阵数据 iteration_vector:初始x值(向量) result:计算结果保存
	//iterations:最大迭代次数(如果达不到该次数则该值被修改为实际迭代次数) dimension:系数矩阵维数 precision:迭代精度 relaxationfactor:松弛因子(0-2)
	ErrNo   SOR(double **Augmented_Matrix,const double *iteration_vector,double *result,const int dimension,const int iterations,const double precision,const double relaxationfactor,int &r_interations,double &r_precision);

	// 矩阵特征值计算(QR算法) 注：未加速 (未完善)
	// matrix:所求矩阵 eigenvalues:特征值 eigenvectors:特征向量 nDimension:方阵维数 iterations:迭代次数
	ErrNo	QR(double **matrix,double *eigenvalues,double **eigenvectors,const int nDimension,int iterations = 11);
	
	// 矩阵特征值计算(QR算法) 注：加速
	// matrix:所求矩阵 eigenvalues:特征值 eigenvectors:特征向量 nDimension:方阵维数
	ErrNo	QR_Accelerate(double **matrix,double *eigenvalues,const int nDimension,int iterations = 11,const double precision = 1e-6);

	// Householder变换
	// matrix:已知矩阵 matrix_hess:求出的Hessenberg矩阵 matrix_house:求出的Householder矩阵 nDimension:matrix的维数(方阵)
	ErrNo	Hess(double **matrix,double **matrix_hess,double **matrix_house,int nDimension);

	// QR分解 matrix_hess:Hessenberg矩阵 nDimension:矩阵维数
	ErrNo	QR_Decomposition(double **matrix_hess,double **matrix_Q,double **matrix_R,int nDimension);

	// 计算矩阵的特征向量（已知矩阵特征值）
	// matrix:矩阵数据 eigenvectors:特征向量(每一行为一特征值对应的特征向量) eigenvalues:特征值(已知) dimension:矩阵维数(方阵)
	ErrNo	matrix_eigenvectors(double **matrix,double **eigenvectors,double *eigenvalues,const int dimension);

	//原点平移的反幂法(求某个特征值的特征向量)
	ErrNo	InvPowerMeth(double **matrix,double *eigenvector,const double eigenvalue,const int dimension,const double precision = 1e-6,const int iteration = 50);

	//符号函数
	double	sign(double x);
	// 计算向量的 2-范数
	double	sqrt_vector(const double *vector_m,const int n);
	// 计算点积
	double	dot_product(const double *vector_m,const int n);
	// 计算向量中绝对值最大的一个
	double	max_vector(double *vector_t,const int n);
	// 计算向量中绝对值最大的一个的值的下标(从0开始)
	  int	max_vector_index(double *vector_t,const int n);
	//将矩阵变成单位矩阵
	void	create_identity_matrix(double **matrix,int nDimension);	
	//一般矩阵乘法 A = B * C
	bool	multiply_matrix(double **pmatrix_a,double **pmatrix_b,double **pmatrix_c,int row_a,int col_a,int row_b,int col_b,int row_c,int col_c);		
	// 方阵乘法(左乘) matrix_dest = matrix_dest * matrix_src
	bool	multiply_matrix_left(double **pdest_matrix,double **psrc_matrix,int dim_dest,int dim_src);
	// 方阵乘法(右乘) matrix_dest = matrix_src * matrix_dest
	bool	multiply_matrix_right(double **psrc_matrix,double **pdest_matrix,int dim_src,int dim_dest);
	// 矩阵的行范数
	double	matrix_norm_r(double **matrix,const int nDimension);

	//快速排序算法(小到大)
	void	QuickSort_STL(double *vector,int s,int t);
	//快速排序算法(大到小)
	void	QuickSort_LTS(double *vector,int s,int t);
};

//计算方程根算法
class Equation_ALgorithm : public Expression
{
private: 
	double	eps;

public:
	Equation_ALgorithm(TCHAR variable_symbol = TEXT('x'));
	//不动点迭代法 expression:表达式 result:计算结果 max_iterations:迭代最大次数(若在未达到最大次数时达到迭代精度,则该值被修改为以达到的迭代次数) 
	//precision:迭代精度(迭代达到最大次数时,若符合精度,值不变,若不符合精度,值修改为所达到的精度) initial_value:初始值 
	ErrNo   Fixed_Point_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);

	//牛顿迭代法 expression:表达式 result:计算结果 max_iterations:迭代最大次数(若在未达到最大次数时达到迭代精度,则该值被修改为以达到的迭代次数) 
	//precision:迭代精度(迭代达到最大次数时,若符合精度,值不变,若不符合精度,值修改为所达到的精度) initial_value:初始值 
	ErrNo	Newton_Iteration(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);	

	//Aitken加速方案 expression:表达式 result:计算结果 max_iterations:迭代最大次数(若在未达到最大次数时达到迭代精度,则该值被修改为以达到的迭代次数) 
	//precision:迭代精度(迭代达到最大次数时,若符合精度,值不变,若不符合精度,值修改为所达到的精度) initial_value:初始值
	ErrNo	Aitken(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);	

	//Steffensen迭代法 expression:表达式 result:计算结果 max_iterations:迭代最大次数(若在未达到最大次数时达到迭代精度,则该值被修改为以达到的迭代次数) 
	//precision:迭代精度(迭代达到最大次数时,若符合精度,值不变,若不符合精度,值修改为所达到的精度) initial_value:初始值
	ErrNo	Steffensen(TCHAR *expression,double &result,int &max_iterations,double &precision,const double initial_value);																								
};

//插值算法
class Interpolation_Algorithm : public Matrix_Algorithm
{
private:
	double	eps;
public:
	Interpolation_Algorithm();
	//Lagrange插值
	ErrNo Lagrange(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//Newton插值 
	ErrNo Newton(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//Hermite插值
	ErrNo Hermite(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//分段线性插值
	ErrNo Piecewise_Linear_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//分段Hermite插值
	ErrNo Piecewise_Hermite_Interpolation(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//样条插值-三弯矩法(边界条件：区间两端点的一阶导数已知)
	//dpoints:插值点 compvalue:要计算x的值 degree:次数n(插值点个数n+1 ) eps:判断除数是否近似等于零 result:结果输出
	ErrNo	Piecewise_Spline_Interpolation_DF(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
	//样条插值-三弯矩法（边界条件：区间两端点的二阶导数已知)
	//dpoints:插值点 compvalue:要计算x的值 degree:次数n(插值点个数n+1 ) eps:判断除数是否近似等于零 result:结果输出
	ErrNo	Piecewise_Spline_Interpolation_DDF(const DPOINT *dpoints,const double compvalue,const int degree,double &result);
};

//拟合算法
class Fitting_Algorithm:public Matrix_Algorithm,Expression
{
private:
	double eps;
public:
	// 最小二乘拟合法
	ErrNo Least_Square_Fitting(PTSTR *polynomial,double *parameter,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps);
	// 基于正交多项式拟合
	ErrNo Polynominal_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const int degree,const double eps);
	// 指数模型的最小二乘拟合 s(x) = a*exp(b*x)
	ErrNo Exponential_Fitting(double *parameter,const DPOINT *dPoints_F,const int nNumofPoints,const double eps);

public:
	// 计算正交多项式在某点x的值 k:第k个多项式 k=0,1,2,...
	// dPoints_F:拟合点 n:拟合点个数 x:要计算的x值 eps:判断除数是否为0
	// 遇到除数为0,返回INF(INF = DBL_MAX)
	double Polynominal_Recursion(const DPOINT *dPoints_F,const int n,const int k,const double x,const double eps);
};

//微分计算方法
class Differential_Algorithm : public Expression
{
private:
	double eps;

public:
	Differential_Algorithm();

public:
	// Taylor公式的数值微分公式
	// 向前差商
	// result:计算结果 expression:存储表达式f(x)的字符串(以x为变量符号) x:要计算的值 interval:间隔
	ErrNo Forward_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor公式的数值微分公式
	// 向后差商
	// result:计算结果 expression:存储表达式f(x)的字符串(以x为变量符号) x:要计算的值 interval:间隔
	ErrNo Backward_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor公式的数值微分公式
	// 中心差商
	// result:计算结果 expression:存储表达式f(x)的字符串(以x为变量符号) x:要计算的值 interval:间隔
	ErrNo Central_Difference(double &result,const TCHAR* expression,const double x,const double interval);
	// Taylor公式的数值微分公式
	// 二阶中心差商
	// result:计算结果 expression:存储表达式f(x)的字符串(以x为变量符号) x:要计算的值 interval:间隔
	ErrNo Second_Order_Central_Difference(double &result,const TCHAR* expression,const double x,const double interval);
};

//积分计算方法
class Integral_Algorithm : public Expression
{
private:
	int		max_times;
	double	eps;
public:
	Integral_Algorithm();
	int		pow_int(int x,int n);
	//Romberg算法
	ErrNo		Romberg(TCHAR *expression,const double precision,const double left_interval,const double right_interval,double &result,double &precision_a,int &times);
	//Gauss-Legendre求积公式(5点式）
	ErrNo		Gauss_Legendre(TCHAR *expression,double &result);
	//Gauss-Chebyshev求积公式(5点式)
	ErrNo		Gauss_Chebyshev(TCHAR *expression,double &result);
};

//常微分方程数值解法
class ODE_Algorithm : public Expression
{
private:
	double	eps;
public:
	ODE_Algorithm();
public:
	//Euler方法（显式单步法） expression:未转换的原始表达式 results:2xn的数组 initial_value:初始值 step_size:步长 left_interval:区间左端的值 right_interval:区间右端的值 variable_symbol_array:变量符号数组
	ErrNo	Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//改进的Euler方法 expression:未转换的原始表达式 results:2xn的数组 initial_value:初始值 step_size:步长 left_interval:区间左端的值 right_interval:区间右端的值 variable_symbol_array:变量符号数组
	ErrNo	Improved_Euler(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//4阶Runge-Kutta方法
	ErrNo	Runge_Kutta(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//4阶Adamas预测-校正格式（简易格式）
	ErrNo	Simple_Adams_PC(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//Adams4步4阶显式公式
	ErrNo	Adams(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//Milne4步4阶显式公式
	ErrNo	Milne(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
	//修正的Milne-Hamming预测-校正格式
	ErrNo	Milne_Hamming(TCHAR *expression,double **results,double initial_value,double step_size,const double left_interval,const double right_interval,const TCHAR *variable_symbol_array);
};

//PCA算法
class PCA_Algorithm : public Matrix_Algorithm
{
private:
	double eps;
public:
	PCA_Algorithm();
	//pricinpal:储存主成分(未申请空间) k:储存主成分个数
	ErrNo PCA_A(double **matrix,const int rows,const int columns,double **&principle,int &k);
	ErrNo PCA_A(double **matrix,const int rows,const int columns,double **matrix_r,double *eigenvalues,double **&principle,int &k);
	//标准化矩阵（在原矩阵的基础上修改）
	ErrNo Standardize_Matrix(double **matrix,const int rows,const int columns);
	//降维 matrix[rows][columns] --> matrix_r[columns][columns]
	ErrNo Relevance_Matrix(double **matrix,double **matrix_r,int rows,int columns);
};

//第二 第三章(方程组计算)
class Matrix_Comp : public Exception
{
public:
	ErrNo	Algorithm_Choice(int iSelectIndex);					//算法选择
	BOOL	CharacterJudgement(LPTSTR szBuffer);				//字符判断(这个判断很特殊)	
	ErrNo	GetStart(int iSelectIndex);							//开始计算
	ErrNo	AllocateMemory(int iSelectIndex);					//申请空间
	//ErrNo	GetMatrix();										//增广矩阵数据获取
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//获得文本输入控件一个数据(整型)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//获得文本输入控件一个数据(整型)
	ErrNo	GetThreeLine();										//获得三对角矩阵的数据
	ErrNo   DestroyPointer();					//销毁指针
	ErrNo	OutPutResult(ErrNo err = 0);										//输出计算结果
	Matrix_Comp(HWND hDlg);										//构造函数

public:
	//带参数
	//获取矩阵数据 pMatrix:存储矩阵数据 nOutput:矩阵最后行在文本框的位置 nRows:矩阵行数 nColumns:矩阵列数
	ErrNo	GetMatrix(const HWND hwndEdit,double **pMatrix,int &nOutput,const int nRows,const int nColumns);
	ErrNo	AllocateMemory(double **&pointer,int nRows,int nColumns);
	void	DestroyPointer(double **&pointer,int nRows);
	void	DestroyPointer(double *&pointer);

public:
	//文件输入数据
	ErrNo	GetStart_File(TCHAR **FilesPath,const int nCount);
	ErrNo	GetStart_File_ANSI(PCTSTR pstrFilePath);	//ANSI文件
	ErrNo	GetStart_File_UTF_LE(PCTSTR pstrFilePath);	//UTF16-LE文件
	ErrNo	StringToNumbericArr(PCTSTR szBuffer,PCTSTR seps,double *n_arr,const int n);	//将一行字符串转为存储double数据的数组
	void	OutPutResult_File();								//文件中获得数据得到计算结果
	//ErrNo	GetMatrix_ANSI(HWND hwndEdit,PCTSTR pstrFileName,double **matrix);
	//ErrNo	GetMatrix_UTF_LE(HWND hwndEdit,PCTSTR pstrFileName,double **matrix);

	//用户界面
private:
	int		iDimension;											//矩阵维数
	int		iOutput;											//计算结果应该输入到的地方 
	int		iIndex;												//算法选择哪个

	//文件获取使用
private:
	int		nRows;												//矩阵行数
	int		nColumns;											//矩阵列数
	int		algorithm;											//算法编号

	//公共
private:
	HWND	hwndDlg;											//对话框句柄
	double	runtime;											//算法计算时间
	double	*iteration_vector;									//迭代初始向量
	double	*result;											//计算结果
	double	**matrix;											//普通矩阵存储
	double  *eigenvalues;										//特征值存储
	double  **eigenvectors;										//特征向量存储
	double  determinant;										//矩阵行列式
	int		iterations;											//迭代次数
	double	precision;											//迭代精度
	int		r_iterations;										//计算过程达到的迭代次数
	double  r_precision;										//计算过程达到的迭代精度
	double  relaxationfactor;									//松弛因子
};

//第四章(一元方程计算)
class Equation : public Exception,Equation_ALgorithm
{
public:
	Equation(HWND hDlg);
	ErrNo	Algorithm_Choice(int algorithm);
	bool	Character_Judgement(const TCHAR *szBuffer);		    //判断输入数据是否有误
	ErrNo	GetStart(int iSelectIndex);
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//获得文本输入控件一个数据(整型)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//获得文本输入控件一个数据(浮点型)
	ErrNo	OutputResult(int iSelectIndex);						//输出结果

public:
	//文件输入数据
	ErrNo	GetStart_File(TCHAR **FilesPath,const int nCount);
	ErrNo	GetStart_File_ANSI(PCTSTR pstrFilePath);	//ANSI文件
	ErrNo	GetStart_File_UTF_LE(PCTSTR pstrFilePath);	//UTF16-LE文件
	void	OutPutResult_File(int algorithm_num);								//文件中获得数据得到计算结果

private:
	HWND	hwndDlg;
	TCHAR	var_symbol;											//变量符号
	TCHAR	var_symbol_save;									//变量符号保存
	TCHAR	formula[MAXINPUT];									//迭代公式
	TCHAR	formula_store[MAXINPUT];							//保存获取的表达式
	double	original_precision;									//保存迭代精度
	double	precision;											//迭代精度
	double  initial_value;										//迭代初始值
	double  result;												//计算结果
	double	runtime;											//计算时间
	int		original_iterations;									//保存最大迭代次数
	int		max_iterations;										//最大迭代次数

private:
	int		algorithm;											//算法编号
};

//第五章和第六章（插值和拟合）
class InterpolationAndFitting : public Exception,Character_Judegement
{
public:
	InterpolationAndFitting();
	InterpolationAndFitting(HWND hwndDlg);

public:
	ErrNo	Algorithm_Choice_I(int iSelectIndex,const DPOINT *dpoints_i,const double compvalue,const int degree,double &result);					//算法选择
	ErrNo	Algorithm_Choice_F(PTSTR *polynomial,double *parameters,int iSelectIndex,const int nNumofPolynomial,const int nNumofPoints,const DPOINT *dPoints_F,const double eps);
	void	DestroyPointer(DPOINT *&dpoints);						//销毁指针
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);				//获得文本输入控件一个数据(整型)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);		//获得文本输入控件一个数据(浮点型)
	ErrNo	GetStart_I(int iSelectIndex);							//计算开始 函数插值
	ErrNo   GetStart_F(int iSelectIndex);							//计算开始 曲线拟合
	ErrNo	GetPoints_I(DPOINT *dPoints,const HWND hListView,const int n,const int SelectIndex);		//获取插值点(n+1个插值点)
	ErrNo	GetPoints_F(DPOINT *dPoints,const HWND hListView,const int SelectIndex);					//获取拟合点
	ErrNo	GetPolynomial_F(PTSTR *polynomial,PTSTR szBuffer,const int nMax,const int nNumofPolynomial);//获取插值多项式	
	void	OutputResult_I(int iSelectIndex,ErrNo err);						//结果输出
	void	OutputResult_F(int iSelectIndex,ErrNo err);

public:
	void	QuickSort(DPOINT *dPoints,int s,int t);					//快速排序
private:
	HWND	hwndDlg;												//对话框句柄
	DPOINT	*dpoints_I;												//插值点 函数插值
	DPOINT  *dPoints_F;												//插值点 函数拟合	
	double	compvalue;												//所求点
	double	result;													//计算结果
	double  eps;													//判断除数是否为零
	double  *parameters;											//拟合计算出的参数
	TCHAR	**polynomial;											//插值多项式
	int		nNumofPolynomial;										//多项式个数
	int		nNumofPoints;
	int		degree;													//次数(正交多项式次数和插值次数)
};

//第七章（微积分计算)
class Calculus : public Exception,Character_Judegement,Integral_Algorithm
{
	//微分计算
private:
	double		value;					//所求点
	double		interval;				//间隔(h)

	//积分计算
private:
	int			times;					//迭代出结果时达到的次数
	double		left_interval;			//左区间
	double		right_interval;			//右区间
	double		precision;				//迭代要求的精度
	double		precision_a;			//迭代达到的精度
	

	//公共
private:
	HWND		hDlg;					//对话框句柄
	double		eps;					//判断除数是否为0
	double		result;					//计算结果
	TCHAR		expression[MAXSIZE];	//获得的表达式
	TCHAR		exp_store[MAXSIZE];		//获得的表达式副本(在表达式计算中，原表达式存储的地方数据被修改)

public:
	Calculus(HWND hwndDlg);	

	//微分计算
public:
	ErrNo		Algorithm_Choice_D(int iSelectIndex);					//微分算法选择
	ErrNo		GetStart_D(int iSelectIndex);							//微分计算
	void		OutputResult_D(int iSelectIndex,ErrNo err = 0);						//计算结果输出（微分）

	//积分计算
public:		
	ErrNo		GetStart_I(int iSelectIndex);							//积分计算
	void		OutputResult_I(int iSelectIndex,ErrNo err = 0);						//计算结果输出（积分）

	//公共函数
public:
	ErrNo		GetLine_Double(HWND hwndEdit,double *fParameter);		//获取输入框中输入的浮点数
};

//第八章(常微分方程计算)
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
	ErrNo	GetStart(int iSelectIndex);							//开始计算	
	ErrNo	GetLine_Int(HWND hwndEdit,int *iParameter);			//获得文本输入控件一个数据(整型)
	ErrNo	GetLine_Double(HWND hwndEdit,double *fParameter);	//获得文本输入控件一个数据(浮点型)
	ErrNo	OutputResults(const int iSelectIndex,ErrNo err = 0);
};

//PCA
class PCA : public PCA_Algorithm,Exception
{
private:
	HWND hwndDlg;
	int nsamples;			// 样本个数	
	int nindexs;			// 指标个数
	int k;					// 主成分个数
	double *eigenvalues;	//特征值
	double **matrix_r;		//相关系数矩阵
	double **matrix_data;  // 储存样本数据
	double **principal;		// 储存主成分数据

private:
	int nOutput;

public:
	PCA(HWND hDlg);
	ErrNo GetStart_Calc();		//开始计算
	ErrNo AllocateMemory(double **&matrix,int rows,int columns);		//申请空间
	void Destroy(double **&matrix,int rows);	//销毁空间
	BOOL CharacterJudgement(LPTSTR szBuffer);
	ErrNo GetMatrix(const HWND hwndEdit,double **pMatrix,const int nRows,const int nColumns);
	ErrNo GetLine_Int(HWND hwndEdit,int *iParameter);
	void OutPutResult();
};