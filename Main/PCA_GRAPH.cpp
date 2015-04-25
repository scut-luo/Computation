/************************************************************************/
/* PCA数据图形化                                                        */
/************************************************************************/

#include "Computation.h"
#include "PCA.h"


extern CChartWnd chartWnd;			//CChart类
struct PCA_GraphData 
{
	HWND hwndParent;
	double **data;
	LinkList *List_Names;
	int n_samples;
	int n_index;
	int graph_type;
	bool draw_all;
	int *sample_num;
	int count;
};
static PCA_GraphData graph;

//画图
ErrNo PCA_DrawGraph(HWND hParent,double **data,LinkList *List_Names,int n_samples,int n_index,int graph_type /* = 0 */,bool draw_all /* = true */,int *sample_num /* = NULL */,int count /* = 0 */)
{
	HWND hDlgGraph;
	HINSTANCE hInstance;

	//要用到的数据复制一份
	graph.hwndParent = hParent;
	graph.data = data;
	graph.List_Names = List_Names;
	graph.n_samples = n_samples;
	graph.n_index = n_index;
	graph.graph_type = graph_type;
	graph.draw_all = draw_all;
	graph.sample_num = sample_num;
	graph.count = count;

	//创建非模态对话框
	hInstance = GetModuleHandle(NULL);
	hDlgGraph = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_PCA_GRAPH),hParent,PCA_DrawGraph_Proc);
	ShowWindow(hDlgGraph,SW_SHOW);		//必须要这句，不然对话框显示不出来
	return 0;
}

BOOL CALLBACK PCA_DrawGraph_Proc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_DLGMSG(hwnd,WM_INITDIALOG,wParam,lParam,PCA_DrawGraph_OnInitDialog);
		HANDLE_DLGMSG(hwnd,WM_CLOSE,wParam,lParam,PCA_DrawGraph_OnClose);
		//HANDLE_DLGMSG(hwnd,WM_COMMAND,wParam,lParam,PCA_DrawGraph_OnCommand);
		HANDLE_DLGMSG(hwnd,WM_PAINT,wParam,lParam,PCA_DrawGraph_OnPaint);
	}
	return FALSE;
}

BOOL PCA_DrawGraph_OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

BOOL PCA_DrawGraph_OnPaint(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	chartWnd.Attach(hDlg,PICID,kTypeXY);
	chartWnd.GetChart()->SetUseLegend(true);	//调出图例
	chartWnd.GetChart()->SetTitle(TEXT(""));

	switch(graph.graph_type)
	{
	case HARMONIC_GRAPH:		//调和曲线图
		{
			double pX[201],pY[201];
			double x,y;				
			double interval;			//间隔
			double section_size;		//区间大小
			int nPoint;					//描点个数
			int nCount;
			int n,c,pos;					//记数所用,没啥特殊意义
			LNode *r;					//每个节点

			nCount = 0;
			nPoint = 200;
			section_size = 2*PI;
			x = (-1) * PI;
			interval = section_size / nPoint;

			if(graph.draw_all)
			{
				for(int i=0;i<graph.n_samples;i++)  //每个样本
				{
					x = (-1) * PI;
					c = 0;
					for(int p=0;p<201;p++)
					{
						y = 0.0;
						n = 1;
						if(graph.n_index % 2)
						{
							y = graph.data[i][0] / sqrt(2.0);	//第一个参数
							for(int k=1;k<graph.n_index;k=k+2)	//注意这里的k要从1开始
							{
								y += graph.data[i][k] * sin(n*x);
								y += graph.data[i][k+1] * cos(n*x);
								n++;
							}
						}
						else
						{
							y = graph.data[i][0] / sqrt(2.0) ;
							for(int k=1;k<graph.n_index-1;k=k+2)
							{
								y += graph.data[i][k] * sin(n*x);
								y += graph.data[i][k+1] * cos(n*x);
								n++;
							}
							y += graph.data[i][graph.n_index-1] * sin(n*x);
						}

						pX[p] = x;
						pY[p] = y;

						x += interval;
					}

					chartWnd.GetChart()->AddCurve(pX,pY,201);
				}
				//设置图例名称
				if(graph.List_Names != NULL)
				{
					r = graph.List_Names->next;
					while(r != NULL)
					{
						chartWnd.GetChart()->SetDataTitle(r->data,r->num-1);
						r = r->next;
					}
				}
			}
			else
			{
				for(int i=0;i<graph.count;i++)  //每个样本
				{

					pos = graph.sample_num[i] - 1;

					x = (-1) * PI;
					c = 0;
					for(int p=0;p<201;p++)
					{
						y = 0.0;
						n = 1;
						if(graph.n_index % 2)
						{
							y = graph.data[pos][0] / sqrt(2.0);	//第一个参数
							for(int k=1;k<graph.n_index;k=k+2)	//注意这里的k要从1开始
							{
								y += graph.data[pos][k] * sin(n*x);
								y += graph.data[pos][k+1] * cos(n*x);
								n++;
							}
						}
						else
						{
							y = graph.data[pos][0] / sqrt(2.0) ;
							for(int k=1;k<graph.n_index-1;k=k+2)
							{
								y += graph.data[pos][k] * sin(n*x);
								y += graph.data[pos][k+1] * cos(n*x);
								n++;
							}
							y += graph.data[pos][graph.n_index-1] * sin(n*x);
						}

						pX[p] = x;
						pY[p] = y;

						x += interval;
					}

					chartWnd.GetChart()->AddCurve(pX,pY,201);
				}

				//设置图例名称
				if(graph.List_Names != NULL)
				{
					n = 0;
					r = graph.List_Names->next;
					while(r != NULL)
					{
						if(r->num == graph.sample_num[n])
						{
							chartWnd.GetChart()->SetDataTitle(r->data,n);
							n++;
						}
						r = r->next;
					}
				}

				/*
				n = 0;
				if(graph.List_Names != NULL)
				{
					r = graph.List_Names->next;
					for(int i=0;i<graph.count;i++)
					{
						if(r != NULL)
						{
							if(r->num == graph.sample_num[i])
							{
								chartWnd.GetChart()->SetDataTitle(r->data,n);
								n++;
							}
						}
						else
							break;
						r = r->next;
					}
				}
				*/
			}


		}
		break;

		case PROFILE_GRAPH:
			{
				LNode *r;
				double *pX,*pY;			//坐标值保存
				double x;			
				double interval,start_point;
				int pos;

				if(!(pX = new double [graph.n_index]))
					return FALSE;
				if(!(pY = new double [graph.n_index]))
					return FALSE;
				
				start_point = 50;
				interval = 100;	//间隔

				if(graph.draw_all)	//全部数据都显示
				{
					for(int i=0;i<graph.n_samples;i++)
					{
						x = start_point;
						for(int j=0;j<graph.n_index;j++)
						{
							pX[j] = x + j*interval;
							pY[j] = graph.data[i][j];
						}
						chartWnd.GetChart()->AddCurve(pX,pY,graph.n_index);
					}

					//设置图例名称
					if(graph.List_Names != NULL)
					{
						r = graph.List_Names->next;
						while(r != NULL)
						{
							chartWnd.GetChart()->SetDataTitle(r->data,r->num-1);
							r = r->next;
						}
					}
				}
				else		//部分数据显示
				{
					for(int i=0;i<graph.count;i++)
					{
						x = start_point;
						pos = graph.sample_num[i] - 1;
						for(int j=0;j<graph.n_index;j++)
						{
							pX[j] = x + j*interval;
							pY[j] = graph.data[pos][j];
						}
						chartWnd.GetChart()->AddCurve(pX,pY,graph.n_index);
					}

					//设置图例名称
					if(graph.List_Names != NULL)
					{
						pos = 0;
						r = graph.List_Names->next;
						while(r != NULL)
						{
							if(r->num == graph.sample_num[pos])
							{
								chartWnd.GetChart()->SetDataTitle(r->data,pos);
								pos++;
							}
							r = r->next;
						}
					}
				}

				delete [] pX;
				delete [] pY;
			}
			break;
	}
	return TRUE;
}

BOOL PCA_DrawGraph_OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{

	return FALSE;
}

BOOL PCA_DrawGraph_OnClose(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	chartWnd.GetChart()->ClearAllData();
	chartWnd.Detach();
	DestroyWindow(hDlg);		//非模态对话框关闭
	return TRUE;
}


