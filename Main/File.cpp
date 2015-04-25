/*
	文件处理
*/
#include "Computation.h"

static OPENFILENAME ofn;

//判断文件类型
int DetectCodeType(PCTSTR pstrFileName)
{
	int codeType;
	char buffer[10];			//由于要对比的是单字节存储的字符串
	FILE *fp;
	
	_tfopen_s(&fp,pstrFileName,TEXT("rb"));

	if(fp == NULL)
		return CAN_NOT_OPEN_FILE;

	fread(buffer,1,3,fp); //读取文件前面的三个字节

	//判断前两个字节
	if(memcmp("\xFE\xFF",buffer,2) == 0)
		codeType = CODE_TYPE_UNICODE_BE;
	else if(memcmp("\xFF\xFE",buffer,2) == 0)
		codeType = CODE_TYPE_UNICODE_LE;
	else if(memcmp("\xEF\xBB",buffer,2) == 0)
		codeType = CODE_TYPE_UTF8;
	else			//找不到BOM头,默认为ANSI编码
		codeType = CODE_TYPE_ANSI;

	fclose(fp);
	return codeType;
}

void FileInitialize(HWND hwnd)
{
	static TCHAR szFilter[] = TEXT("Text Files(*.txt)\0*.txt\0") \
							  TEXT("All Files(*.*)\0*.*\0\0");

	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= hwnd;
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= szFilter;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 0;
	ofn.lpstrFile			= NULL;
	ofn.nMaxFile			= MAX_PATH;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= MAX_PATH;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= NULL;
	ofn.Flags				= 0;
	ofn.nFileOffset			= 0;
	ofn.nFileExtension		= 0;
	ofn.lpstrDefExt			= TEXT("txt");
	ofn.lCustData			= 0L;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;
}

BOOL FileSaveDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName)
{
	ofn.hwndOwner		= hwnd;
	ofn.lpstrFile		= pstrFileName;
	ofn.lpstrFileTitle	= pstrTitileName;
	ofn.Flags			= OFN_OVERWRITEPROMPT;
	return GetSaveFileName(&ofn);
}

BOOL FileOpenDlg(HWND hwnd,PTSTR pstrFileName,PTSTR pstrTitileName)
{
	ofn.hwndOwner		= hwnd;
	ofn.lpstrFile		= pstrFileName;
	ofn.lpstrFileTitle	= pstrTitileName;
	ofn.Flags			= OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	return GetOpenFileName(&ofn);
}

BOOL FileRead(HWND hwndEdit,PTSTR pstrFileName)
{
	//BYTE	bySwap;
	//DWORD	dwBytesRead;
	return TRUE;
}

BOOL FileWrite(HWND hwndEdit,PTSTR pstrFileName)
{
	DWORD		dwBytesWritten;
	HANDLE		hFile;
	int			iLength;
	PTSTR		pstrBuffer;
	WORD		wByteOrderMark = 0xFEFF;

	//Open the file,creating it if necessary
	if(INVALID_HANDLE_VALUE == (hFile = CreateFile(pstrFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL)))
		return FALSE;

	//Get th number of characters in the edit control and allocate memory for them
	iLength = Edit_GetTextLength(hwndEdit);
	pstrBuffer = (PTSTR) malloc ((iLength + 1) * sizeof(TCHAR));

	if(!pstrBuffer)				//Fail to allocate memory 
	{
		CloseHandle(hFile);
		return FALSE;
	}

	//If the edit control will return Unicode text,write the byte order mark to the file
#ifdef UNICODE
	WriteFile(hFile,&wByteOrderMark,2,&dwBytesWritten,NULL);
#endif

	//Get the edit buffer and write that out to the file.
	Edit_GetText(hwndEdit,pstrBuffer,iLength+1);
	WriteFile(hFile,pstrBuffer,iLength*sizeof(TCHAR),&dwBytesWritten,NULL);

	if((iLength * sizeof(TCHAR)) != (int)dwBytesWritten)
	{
		CloseHandle(hFile);
		free(pstrBuffer);
		return FALSE;
	}

	CloseHandle(hFile);
	free(pstrBuffer);
	return TRUE;
}

