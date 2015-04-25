#pragma once

//消息分流器
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

//宏定义
#define	  MAXINPUT			256						//字符数组最大大小
#define	  MAXSIZE			256
#define	  NUMOFPOINT		500						//描点的数量
#define	  PI				3.1415926				//PI值
#define	  INF				DBL_MAX					//无穷大

//错误代码
//必须弹窗的错误1-100
#define	  NO_DATA_INPUT				1				//没有输入数据
#define	  INCORRECT_DATA			2				//输入数据非法
#define	  WRONG_MATRIX				3				//输入矩阵有误
#define	  WRONG_EXPRESSION			4				//表达式错误
#define   NOT_MATCH_DEGREE			5				//输入数据个数与次数不匹配
#define	  NOT_MATCH					6				//输入数据个数有误
#define	  CAN_NOT_OPEN_FILE			7				//打不开文件
#define	  INCORRECT_DATA_FILE		8				//输入数据非法(文件)
#define	  READ_FILE_ERR				9				//获取文件内容出错
#define	  NOT_ENOUGH_PARA			10				//参数太少
#define	  NOT_MATCH_ALGORITHM		11				//没有匹配的算法

//在输出框输出的错误 101开始
#define	  ALLOCATE_MEMORY_FAIL		101				//申请内存失败
#define   DIVIDE_BY_ZERO			102				//除数为零
#define	  NEGATIVE_NUMBER			103				//开根时根号中数为负数或对负数求对数
#define	  BRACKET_NOT_MATCH			104				//括号不配对
#define	  NOT_REQUIRED_SOLUTION     105				//迭代未获得达到要求的解
#define   DATA_OVERFLOW				106				//计算出现数据溢出
#define	  ARRAY_OVERFLOW			107				//数组溢出
#define   DIVERGENCY				108				//迭代不收敛
#define	  NO_RESULT					109				//计算不出结果
#define	  NOT_IN_INTERVAL			110				//不在计算区间内
#define	  SMALL_INTERVAL			111				//区间过小




//算法在ComboBox的Index	（第二第三章）
#define	  GAUSS_INDEX				0
#define   LU_INDEX					1
#define	  THOMAS_INDEX				2
#define	  CHOLESKY_INDEX			3
#define	  JACOBI_INDEX				4
#define	  GAUSS_SEIDEL_INDEX		5
#define	  SOR_INDEX					6
#define	  EIGENVALUES_INDEX			7

//算法在ComboBox的Index	（第四章）
#define	  FIXED_POINT_ITERATION_INDEX			0			//不动点迭代法
#define	  NEWTON_ITERATION_INDEX				1			//Newton迭代法
#define	  AITKEN_INDEX							2			//Aitken加速方案
#define	  STEFFENSEN_INDEX						3			//Steffensen迭代法

//算法在ComboBox的Index	（第五第六章）
#define   LAGRANGE_INTERPOLATION_INDEX			0           //Lagrange插值
#define	  NEWTON_INTERPOLATION_INDEX			1		    //Newton插值
#define   HERMITE_INTERPOLATION_INDEX			2			//Hermite插值
#define	  PIECEWISE_LINEAR_INTERPOLATION		3			//分段线性插值
#define	  PIECEWISE_HERMITE_INTERPOLATION		4			//分段Hermite插值
#define	  PIECEWISE_SPLINE_INTERPOLATION_DF		5			//分段样条插值(已知两端点的一阶导数)
#define	  PIECEWISE_SPLINE_INTERPOLATION_DDF	6			//分段样条插值(已知两端点的二阶导数)

#define   LEAST_SQUARE_FITTING_INDEX			0			//线性最小二乘拟合
#define	  POLYNOMINAL_FITTING_INDEX				1			//正交多项式拟合
#define	  EXPONENTIAL_FITTING_INDEX				2			//指数模型

//算法在ComboBox的Index（第七章 微积分计算）
//微分计算
#define	  FORWARD_DIFFERENCE_INDEX				0			//Taylor计算微分 向前差分
#define	  BACKFORWARD_DIFFERENCE_INDEX			1			//Taylor计算微分 向后差分
#define   CENTRAL_DIFFERENCE_DIFFERENCE_INDEX	2			//Taylor计算微分 中心差分
#define   SO_CENTRAL_DIFFERENCE_DIFFERENCE_INDEX 3			//Taylor计算微分 二阶中心差分


//积分计算
#define	  ROMBERG_INDEX							0			//Romberg算法
#define	  GAUSS_LEGENDRE_INDEX					1			//Gauss-Lagendre求积公式
#define	  GAUSS_CHEBYSHEV_INDEX					2			//Gauss-Chebyshev求积公式

//算法在ComboBox的Index（第八章）
#define	  EULER_INDEX							0			//Euler方法（显式单步法）
#define	  IMPROVED_EULER_INDEX					1			//改进的Euler方法
#define	  RUNGE_KUTTA_INDEX						2			//4阶Runge-Kutta方法
#define	  SIMPLE_ADAMS_PC_INDEX					3			//4阶Adams预测-校正格式(简易格式)
#define	  MILNE_HAMMING_INDEX					4			//修正的Milne-Hamming预测-校正格式
#define	  ADAMS_INDEX							5			//Adams4步4阶显式公式
#define	  MILNE_INDEX							6			//Milne4步4阶显式公式



enum
{
	CODE_TYPE_UNICODE_LE,		//UTF-16LE
	CODE_TYPE_UNICODE_BE,		//UTF-16BE
	CODE_TYPE_UTF8,				//UFT-8
	CODE_TYPE_ANSI				//ANSI
};

