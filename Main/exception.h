#pragma once

#include<Windows.h>
#include<tchar.h>
#include "define.h"

struct ErrorInformation
{
	int ErrNum;
	PCTSTR ErrDescription;
};

ErrorInformation FormatErr[] = { NO_DATA_INPUT,		TEXT("δ������������"),
								   INCORRECT_DATA,		TEXT("�������ݲ����ϱ�׼"),
								   WRONG_MATRIX,		TEXT("�����������"),
								   WRONG_EXPRESSION,	TEXT("���ʽ����"),
								   NOT_MATCH_DEGREE,	TEXT("�������ݸ����������ƥ��"),
								   NOT_MATCH,			TEXT("�������ݸ�������"),
								   CAN_NOT_OPEN_FILE,	TEXT("���ܴ��ļ�"),
								   INCORRECT_DATA_FILE,	TEXT("�ļ����зǷ�����"),
								   READ_FILE_ERR,		TEXT("��ȡ�ļ����ݳ���"),
								   NOT_ENOUGH_PARA,		TEXT("�����������"),
								   NOT_MATCH_ALGORITHM,	TEXT("û��ƥ����㷨"),								

								   ALLOCATE_MEMORY_FAIL,TEXT("�����ڴ�ʧ��"),
								   DIVIDE_BY_ZERO,		TEXT("�����������������Ϊ��"),
								   NEGATIVE_NUMBER,		TEXT("�Ը��������������"),
								   BRACKET_NOT_MATCH,	TEXT("���ʽ���Ų�ƥ��"),
								   NOT_REQUIRED_SOLUTION,TEXT("����δ�ﵽ����Ҫ��"),
								   DATA_OVERFLOW,		TEXT("�������"),
								   ARRAY_OVERFLOW,		TEXT("�������������г����������"),
								   DIVERGENCY,			TEXT("����������"),
								   NO_RESULT,			TEXT("���㲻�����"),
								   NOT_IN_INTERVAL,		TEXT("���ڼ���������"),
								   SMALL_INTERVAL,		TEXT("���������С")

};