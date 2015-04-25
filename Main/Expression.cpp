/*
	类：将中缀表达式转换成后缀表达式

*/

#include "Computation.h"

Expression::Expression()														//构造函数
{
	eps = 1e-6;
	variable_symbol = TEXT('x');
}

int Expression::Deal_String(TCHAR *expression)
{
	int		i,j,iLength,iSize;

	iLength = lstrlen(expression);
	iSize = iLength+1;													//字符数组大小 包括'\0'

	//全体字符变小写
	for(i=0;i<iLength;i++)
	{
		if(expression[i]>=TEXT('A') && expression[i]<=TEXT('Z'))
			expression[i] += 32;
	}

	for(i=0;i<iLength;i++)
	{
		//删除表达式中空格
		if(expression[i] == TEXT(' ') || expression[i] == TEXT('\t') || expression[i] == TEXT('='))		//添加 \t 和 =
		{
			for(j=i+1;j<iSize;j++)
				expression[j-1] = expression[j];						//字符串整体右移 包括'\0' 注意循环是iLength
			iLength--;
			iSize--;
			i--;			//原本没有
			continue;		//原本没有
		}

		//sin->S
		if(expression[i] == TEXT('s'))
		{
			if(i+2<iLength)
			{
				if(expression[i+1] == TEXT('i') && expression[i+2] == TEXT('n'))
				{
					//将sin转换为S
					expression[i] = TEXT('S');
					for(j=i+3;j<iSize;j++)
						expression[j-2] = expression[j];
					iLength -= 2;
					iSize -= 2;
				}
			}
		}

		//cos->C
		if(expression[i] == TEXT('c'))
		{
			if(i+2<iLength)
			{
				if(expression[i+1] == TEXT('o') && expression[i+2] == TEXT('s'))
				{
					//将cos转换为C
					expression[i] = TEXT('C');
					for(j=i+3;j<iSize;j++)
						expression[j-2] = expression[j];
					iLength -= 2;
					iSize -= 2;
				}
			}
		}

		//tan->T
		if(expression[i] == TEXT('t'))
		{
			if(i+2<iLength)
			{
				if(expression[i+1] == TEXT('a') && expression[i+2] == TEXT('n'))
				{
					//将tan转换为T
					expression[i] = TEXT('T');
					for(j=i+3;j<iSize;j++)
						expression[j-2] = expression[j];
					iLength -= 2;
					iSize -= 2;
				}
			}
		}

		//ln->L
		if(expression[i] == TEXT('l') && expression[i+1] == TEXT('n'))
		{
			//将ln转换为L
			expression[i] = TEXT('L');
			for(j=i+2;j<iSize;j++)
				expression[j-1] = expression[j];
			iLength--;
			iSize--;
		}

		//log->G
		if(expression[i] == TEXT('l'))
		{
			if(i+2<iLength)
			{
				if(expression[i+1] == TEXT('o') && expression[i+2] == TEXT('g'))
				{
					//将log转换为G
					expression[i] = TEXT('G');
					for(j=i+3;j<iSize;j++)
						expression[j-2] = expression[j];
					iLength -= 2;
					iSize -= 2;
				}
			}
		}

		//sqrt->R
		if(expression[i] == TEXT('s'))
		{
			if(i+3<iLength)
			{
				if(expression[i+1] == TEXT('q') && expression[i+2] == TEXT('r') && expression[i+3] == TEXT('t'))
				{
					//将sqrt转换为R
					expression[i] = TEXT('R');
					for(j=i+4;j<iSize;j++)
						expression[j-3] = expression[j];
					iLength -= 3;
					iSize -= 3;
				}
			}
		}

		//exp->E e->E
		if(expression[i] == TEXT('e'))
		{
			if(i+2<iLength)
			{
				if(expression[i+1] == TEXT('x') && expression[i+2] == TEXT('p'))
				{
					//将exp转换为E
					expression[i] = TEXT('E');
					expression[i+1] = TEXT('^');
					for(j=i+3;j<iSize;j++)
						expression[j-1] = expression[j];
					iLength -= 1;
					iSize -= 1;
				}
			}
			expression[i] = TEXT('E');							//注意这个替换的位置			
		}

	}

	return 0;
}

int Expression::Left_Pri(TCHAR op)
{
	switch(op)
	{
		case TEXT('='):
			return 0;
		case TEXT('('):
			return 1;
		case TEXT('+'):
		case TEXT('-'):
			return  3;
		case TEXT('*'):
		case TEXT('/'):
			return 5;
		case TEXT('S'):
		case TEXT('C'):
		case TEXT('T'):
		case TEXT('L'):
		case TEXT('G'):
		case TEXT('R'):
		case TEXT('^'):
			return 6;
		case TEXT(')'):
			return 10;
	}
	return -1;
}

int Expression::Rigth_Pri(TCHAR op)
{
	switch(op)
	{
	case TEXT('='):
		return 0;
	case TEXT('('):
		return 10;
	case TEXT('+'):
	case TEXT('-'):
		return  2;
	case TEXT('*'):
	case TEXT('/'):
		return 4;
	case TEXT('S'):
	case TEXT('C'):
	case TEXT('T'):
	case TEXT('L'):
	case TEXT('G'):
	case TEXT('R'):
	case TEXT('^'):
		return 7;
	case TEXT(')'):
		return 1;
	}
	return -1;
}

int Expression::Precede_Op(TCHAR op1,TCHAR op2)
{
	if(Left_Pri(op1) == Rigth_Pri(op2))
		return 0;
	else if(Left_Pri(op1) < Rigth_Pri(op2))
		return -1;
	else
		return 1;
}

bool Expression::InOp(TCHAR ch)
{
	int	  iNumOfOp;
	const TCHAR	operators_array[] = {TEXT('='),TEXT('('),TEXT(')'),TEXT('+'),TEXT('-'),TEXT('*'),TEXT('/'),TEXT('S'), \
		TEXT('C'),TEXT('T'),TEXT('L'),TEXT('G'),TEXT('R'),TEXT('^')};

	iNumOfOp = sizeof(operators_array) / sizeof(operators_array[0]);

	for(int i=0;i<iNumOfOp;i++)
	{
		if(ch == operators_array[i])
			return true;
	}
	return false;
}

//一维情形


void Expression::ChangeVariableSymbol(TCHAR new_variable_symbol)
{
	variable_symbol = new_variable_symbol;
}

//将中缀表达式转换为后缀表达式
ErrNo Expression::Trans_Exp(TCHAR *expression,TCHAR *postexp)
{
	TCHAR		buffer[MAXSIZE];
	ErrNo		err;
	int			i = 0;
	int			position;

	TCHAR		data[MAXSIZE];
	int			top = 0;

	top = -1;
	top++;
	data[top] = TEXT('=');

	if(err = Character_Filter(expression))
		return err;

	while(*expression != TEXT('\0'))
	{
		position = 0;
		if(!InOp(*expression))
		{
			if(*expression == variable_symbol)
			{
				postexp[i++] = variable_symbol;
				expression++;
				postexp[i++] = TEXT('#');
				continue;
			}
			else if(*expression == TEXT('E'))
			{
				swprintf(buffer,MAXSIZE,TEXT("%f"),E);
				for(int k=0;k<lstrlen(buffer);k++)
				{
					postexp[i++] = buffer[k];
				}
				postexp[i++] = TEXT('#');
				expression++;
				continue;
			}

			if(*expression == TEXT('.') && position == 0)
			{
				postexp[i++] = TEXT('0');
				postexp[i++] = TEXT('.');
				expression++;
				position++;
			}
			while((*expression>=TEXT('0') && *expression<=TEXT('9')) || (*expression == TEXT('.')))
			{
				postexp[i++] = *expression;
				expression++;
			}
			postexp[i++] = TEXT('#');
		}
		else
		{
			switch(Precede_Op(data[top],*expression))
			{
			case -1:
				top++;data[top] = *expression;
				expression++;
				break;

			case 0:
				top--;
				expression++;
				break;

			case 1:
				postexp[i++] = data[top];
				top--;
				break;
			}
		}
	}
	while(data[top] != TEXT('='))
	{
		postexp[i++] = data[top];
		top--;
	}
	postexp[i] = TEXT('\0');
	return 0;
}

ErrNo Expression::Character_Filter(TCHAR *expression)
{
	TCHAR		op_stack[MAXSIZE];
	int			iLength,iTop;

	iLength = lstrlen(expression);

	iTop = -1;
	
	for(int i=0;i<iLength;i++)
	{
		if(!InOp(expression[i]))
		{
			if( expression[i] >=TEXT('0') && expression[i] <=TEXT('9'))
				continue;
			else if(expression[i] == TEXT('E'))
				continue;
			else if( expression[i] == '.' )
				continue;
			if( expression[i] != variable_symbol)
				return INCORRECT_DATA;
		}
		else if(expression[i] == TEXT('('))
		{
			iTop++;
			op_stack[iTop] = TEXT('(');
		}
		else if(expression[i] == TEXT(')'))
		{
			if(iTop == -1)
				return BRACKET_NOT_MATCH;				//括号不匹配
			iTop--;
		}
	}

	if(iTop != -1)										//括号不匹配
		return BRACKET_NOT_MATCH;

	return 0;
}

ErrNo Expression::CompValue(const TCHAR *postexp,double x,double *result)
{
	TCHAR	buffer[MAXSIZE],*pstr = NULL;
	double  d,a,b,c;
	double	data[MAXSIZE];
	int		top,positon=0;

	top = -1;
	while(*postexp != TEXT('\0'))
	{
		switch(*postexp)
		{
		case TEXT('+'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b= data[top];
			top--;
			c = a+b;
			top++;
			data[top] = c;
			break;
			
		case TEXT('-'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = b-a;
			top++;
			data[top] = c;
			break;

		case TEXT('*'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = a*b;
			top++;
			data[top] = c;
			break;

		case TEXT('/'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			if(fabs(a) > eps)
			{
				c = b/a;
				top++;
				data[top] = c;
			}
			else
				return DIVIDE_BY_ZERO;				//除数为0
			break;

		case TEXT('S'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = sin(a);
			top++;
			data[top] = c;
			break;

		case TEXT('C'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = cos(a);
			top++;
			data[top] = c;
			break;

		case TEXT('T'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = tan(a);
			top++;
			data[top] = c;
			break;

		case TEXT('L'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = log(a);
			top++;
			data[top] = c;
			break;

		case TEXT('G'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = log10(a);
			top++;
			data[top] = c;
			break;

		case TEXT('R'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = sqrt(a);
			top++;
			data[top] = c;
			break;
			/*
		case TEXT('E'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = exp(a);
			top++;
			data[top] = c;
			break;
			*/

		case TEXT('^'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = pow(b,a);
			top++;
			data[top] = c;
			break;

		default:
			d = 0.0;
			positon = 0;
			if(*postexp == variable_symbol)						//遇到变量
			{
				top++;
				data[top] = x;
				postexp++;
				break;
			}
			//处理小数
			while(*postexp != TEXT('#'))
			{
				if(positon >= MAXSIZE)
					return WRONG_EXPRESSION;
				buffer[positon] = *postexp;
				positon++;
				postexp++;
			}
			buffer[positon] = TEXT('\0');
			d = wcstod(buffer,&pstr);
			top++;
			data[top] = d;
			break;
		}
		postexp++;
	}

	if(top != 0)
		return WRONG_EXPRESSION;

	*result = data[top];
	return 0;
}

//多维情形
ErrNo Expression::Character_Filter(TCHAR *expression,const TCHAR *variable_symbol_array,const int numofvariable)
{
	TCHAR		op_stack[MAXSIZE];
	int			iLength,iTop;
	int			k;

	iLength = lstrlen(expression);

	iTop = -1;

	for(int i=0;i<iLength;i++)
	{
		if(!InOp(expression[i]))
		{
			if( expression[i] >=TEXT('0') && expression[i] <=TEXT('9'))
				continue;
			else if(expression[i] == TEXT('E'))
				continue;
			else if( expression[i] == '.' )
				continue;
			for(k=0;k<numofvariable;k++)
			{
				if(expression[i] == variable_symbol_array[k])
					break;
			}
			if(k== numofvariable)
				return INCORRECT_DATA;
		}
		else if(expression[i] == TEXT('('))
		{
			iTop++;
			op_stack[iTop] = TEXT('(');
		}
		else if(expression[i] == TEXT(')'))
		{
			if(iTop == -1)
				return BRACKET_NOT_MATCH;				//括号不匹配
			iTop--;
		}
	}

	if(iTop != -1)										//括号不匹配
		return BRACKET_NOT_MATCH;

	return 0;
}

ErrNo Expression::Trans_Exp(TCHAR *expression,TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable)
{
	TCHAR		buffer[MAXSIZE];
	ErrNo		err;
	int			i = 0,k;
	int			position;

	TCHAR		data[MAXSIZE];
	int			top = 0;

	top = -1;
	top++;
	data[top] = TEXT('=');


	if(err = Character_Filter(expression,variable_symbol_array,numofvariable))
		return err;

	while(*expression != TEXT('\0'))
	{
		position = 0;
		if(!InOp(*expression))
		{
			for(k=0;k<numofvariable;k++)
			{
				if(*expression == variable_symbol_array[k])
				{
					postexp[i++] = variable_symbol_array[k];
					expression++;
					postexp[i++] = TEXT('#');
					break;
				}
			}
			if(k<numofvariable)
				continue;

			if(*expression == TEXT('E'))
			{
				swprintf(buffer,MAXSIZE,TEXT("%f"),E);
				for(int k=0;k<lstrlen(buffer);k++)
				{
					postexp[i++] = buffer[k];
				}
				postexp[i++] = TEXT('#');
				expression++;
				continue;
			}

			if(*expression == TEXT('.') && position == 0)
			{
				postexp[i++] = TEXT('0');
				postexp[i++] = TEXT('.');
				expression++;
				position++;
			}
			while((*expression>=TEXT('0') && *expression<=TEXT('9')) || (*expression == TEXT('.')))
			{
				postexp[i++] = *expression;
				expression++;
			}
			postexp[i++] = TEXT('#');
		}
		else
		{
			switch(Precede_Op(data[top],*expression))
			{
			case -1:
				top++;data[top] = *expression;
				expression++;
				break;

			case 0:
				top--;
				expression++;
				break;

			case 1:
				postexp[i++] = data[top];
				top--;
				break;
			}
		}
	}
	while(data[top] != TEXT('='))
	{
		postexp[i++] = data[top];
		top--;
	}
	postexp[i] = TEXT('\0');
	return 0;
}

ErrNo Expression::CompValue(const TCHAR *postexp,const TCHAR *variable_symbol_array,const int numofvariable,double *values,double *result)
{
	TCHAR	buffer[MAXSIZE],*pstr = NULL;
	double  d,a,b,c;
	double	data[MAXSIZE];
	int		top,positon=0,i;


	top = -1;
	while(*postexp != TEXT('\0'))
	{
		switch(*postexp)
		{
		case TEXT('+'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b= data[top];
			top--;
			c = a+b;
			top++;
			data[top] = c;
			break;
			
		case TEXT('-'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = b-a;
			top++;
			data[top] = c;
			break;

		case TEXT('*'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = a*b;
			top++;
			data[top] = c;
			break;

		case TEXT('/'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			if(fabs(a) > eps)
			{
				c = b/a;
				top++;
				data[top] = c;
			}
			else
				return DIVIDE_BY_ZERO;				//除数为0
			break;

		case TEXT('S'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = sin(a);
			top++;
			data[top] = c;
			break;

		case TEXT('C'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = cos(a);
			top++;
			data[top] = c;
			break;

		case TEXT('T'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = tan(a);
			top++;
			data[top] = c;
			break;

		case TEXT('L'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = log(a);
			top++;
			data[top] = c;
			break;

		case TEXT('G'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = log10(a);
			top++;
			data[top] = c;
			break;

		case TEXT('R'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			if(a<0)
				return NEGATIVE_NUMBER;
			c = sqrt(a);
			top++;
			data[top] = c;
			break;
			/*
		case TEXT('E'):
			if(top<0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			c = exp(a);
			top++;
			data[top] = c;
			break;
			*/

		case TEXT('^'):
			if(top<=0)
				return WRONG_EXPRESSION;
			a = data[top];
			top--;
			b = data[top];
			top--;
			c = pow(b,a);
			top++;
			data[top] = c;
			break;

		default:
			d = 0.0;
			positon = 0;
			for(i=0;i<numofvariable;i++)
			{
				if(*postexp == variable_symbol_array[i])						//遇到变量
				{
					top++;
					data[top] = values[i];
					postexp++;
					break;
				}
			}
			if(i<numofvariable)
				break;
			//处理小数
			while(*postexp != TEXT('#'))
			{
				if(positon >= MAXSIZE)
					return WRONG_EXPRESSION;
				buffer[positon] = *postexp;
				positon++;
				postexp++;
			}
			buffer[positon] = TEXT('\0');
			d = wcstod(buffer,&pstr);
			top++;
			data[top] = d;
			break;
		}
		postexp++;
	}

	if(top != 0)
		return WRONG_EXPRESSION;

	*result = data[top];
	return 0;
}