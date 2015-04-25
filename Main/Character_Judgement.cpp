#include "Computation.h"

//判断输入是否为小数
bool Character_Judegement::Judge_Double(const TCHAR *szBuffer) const
{
	int		iLength = 0;

	iLength = lstrlen(szBuffer);

	for(int i=0;i<iLength;i++)
	{
		if(szBuffer[i] == TEXT('.'))
		{
			if(i==0 || i==iLength -1)
				return FALSE;
		}
		else if(szBuffer[i] == TEXT('-'))
		{
			if(i!=0)
				return FALSE;
		}
		else if( (szBuffer[i]<TEXT('0')) || (szBuffer[i]>TEXT('9')) )
		{
			return FALSE;
		}
	}

	return TRUE;
}