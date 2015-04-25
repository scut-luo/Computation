#include "Computation.h"

VOID New_TabControl(LPTABCTRL lptc,HWND hTab,LPTSTR *tabNames,LPTSTR *dlgNames,Page_Proc *page_proc)
{
	TCITEM		tie;
	LPTSTR		*ptr;
	int			i;

	//Set Struct TABCTRL 
	lptc->hTab = hTab;
	lptc->tabNames = tabNames;
	lptc->dlgNames = dlgNames;
	lptc->page_proc = page_proc;
	lptc->tabPageCount = 0;

	ptr = dlgNames;
	// Determine number of tab pages to insert based on DlgNames
	while(*ptr++)
	{
		lptc->tabPageCount++;
	}

	//create array based on number of pages
	lptc->hTabPages = (HWND*)malloc(lptc->tabPageCount * sizeof(HWND));

	tie.mask = TCIF_TEXT;
	for(i=0;i<lptc->tabPageCount;i++)
	{
		//Set Tab Name
		tie.pszText = (LPWSTR)lptc->tabNames[i];
		TabCtrl_InsertItem(lptc->hTab,i,&tie);
		// Add page to each tab
		lptc->hTabPages[i] = CreateDialog(GetModuleHandle(NULL),dlgNames[i],lptc->hTab,(DLGPROC)(*page_proc[i]));
	}

	//Get the rect of Tab Control
	GetClientRect(hTab, &lptc->rect); //获取TabControl控件的坐标信息
	lptc->rect.top+=25;
	lptc->rect.left+=4;
	lptc->rect.right-=1;
	lptc->rect.bottom-=1;

	MoveWindow(lptc->hTabPages[0],lptc->rect.left,lptc->rect.top,lptc->rect.right-lptc->rect.left,lptc->rect.bottom-lptc->rect.top,TRUE);   
	//Show first tab
	ShowWindow(lptc->hTabPages[0], SW_SHOW);


}