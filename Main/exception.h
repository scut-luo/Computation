#pragma once

#include<Windows.h>
#include<tchar.h>
#include "define.h"

struct ErrorInformation
{
	int ErrNum;
	PCTSTR ErrDescription;
};

ErrorInformation FormatErr[] = { NO_DATA_INPUT,		TEXT("未输入完整数据"),
								   INCORRECT_DATA,		TEXT("输入数据不符合标准"),
								   WRONG_MATRIX,		TEXT("输入矩阵有误"),
								   WRONG_EXPRESSION,	TEXT("表达式错误"),
								   NOT_MATCH_DEGREE,	TEXT("输入数据个数与次数不匹配"),
								   NOT_MATCH,			TEXT("输入数据个数有误"),
								   CAN_NOT_OPEN_FILE,	TEXT("不能打开文件"),
								   INCORRECT_DATA_FILE,	TEXT("文件中有非法数据"),
								   READ_FILE_ERR,		TEXT("读取文件数据出错"),
								   NOT_ENOUGH_PARA,		TEXT("输入参数过少"),
								   NOT_MATCH_ALGORITHM,	TEXT("没有匹配的算法"),								

								   ALLOCATE_MEMORY_FAIL,TEXT("申请内存失败"),
								   DIVIDE_BY_ZERO,		TEXT("计算过程中遇到除数为零"),
								   NEGATIVE_NUMBER,		TEXT("对负数开根或求对数"),
								   BRACKET_NOT_MATCH,	TEXT("表达式括号不匹配"),
								   NOT_REQUIRED_SOLUTION,TEXT("迭代未达到精度要求"),
								   DATA_OVERFLOW,		TEXT("数据溢出"),
								   ARRAY_OVERFLOW,		TEXT("程序运作过程中出现数组溢出"),
								   DIVERGENCY,			TEXT("迭代不收敛"),
								   NO_RESULT,			TEXT("计算不出结果"),
								   NOT_IN_INTERVAL,		TEXT("不在计算区间内"),
								   SMALL_INTERVAL,		TEXT("计算区间过小")

};