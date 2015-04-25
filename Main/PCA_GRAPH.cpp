/************************************************************************/
/* PCA����ͼ�λ�                                                        */
/************************************************************************/

#include "Computation.h"
#include "PCA.h"


extern CChartWnd chartWnd;			//CChart��
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

//��ͼ
ErrNo PCA_DrawGraph(HWND hParent,double **data,LinkList *List_Names,int n_samples,int n_index,int graph_type /* = 0 */,bool draw_all /* = true */,int *sample_num /* = NULL */,int count /* = 0 */)
{
	HWND hDlgGraph;
	HINSTANCE hInstance;

	//Ҫ�õ������ݸ���һ��
	graph.hwndParent = hParent;
	graph.data = data;
	graph.List_Names = List_Names;
	graph.n_samples = n_samples;
	graph.n_index = n_index;
	graph.graph_type = graph_type;
	graph.draw_all = draw_all;
	graph.sample_num = sample_num;
	graph.count = count;

	//������ģ̬�Ի���
	hInstance = GetModuleHandle(NULL);
	hDlgGraph = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_PCA_GRAPH),hParent,PCA_DrawGraph_Proc);
	ShowWindow(hDlgGraph,SW_SHOW);		//����Ҫ��䣬��Ȼ�Ի�����ʾ������
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
	chartWnd.GetChart()->SetUseLegend(true);	//����ͼ��
	chartWnd.GetChart()->SetTitle(TEXT(""));

	switch(graph.graph_type)
	{
	case HARMONIC_GRAPH:		//��������ͼ
		{
			double pX[201],pY[201];
			double x,y;				
			double interval;			//���
			double section_size;		//�����С
			int nPoint;					//������
			int nCount;
			int n,c,pos;					//��������,ûɶ��������
			LNode *r;					//ÿ���ڵ�

			nCount = 0;
			nPoint = 200;
			section_size = 2*PI;
			x = (-1) * PI;
			interval = section_size / nPoint;

			if(graph.draw_all)
			{
				for(int i=0;i<graph.n_samples;i++)  //ÿ������
				{
					x = (-1) * PI;
					c = 0;
					for(int p=0;p<201;p++)
					{
						y = 0.0;
						n = 1;
						if(graph.n_index % 2)
						{
							y = graph.data[i][0] / sqrt(2.0);	//��һ������
							for(int k=1;k<graph.n_index;k=k+2)	//ע�������kҪ��1��ʼ
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
				//����ͼ������
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
				for(int i=0;i<graph.count;i++)  //ÿ������
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
							y = graph.data[pos][0] / sqrt(2.0);	//��һ������
							for(int k=1;k<graph.n_index;k=k+2)	//ע�������kҪ��1��ʼ
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

				//����ͼ������
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
				double *pX,*pY;			//����ֵ����
				double x;			
				double interval,start_point;
				int pos;

				if(!(pX = new double [graph.n_index]))
					return FALSE;
				if(!(pY = new double [graph.n_index]))
					return FALSE;
				
				start_point = 50;
				interval = 100;	//���

				if(graph.draw_all)	//ȫ�����ݶ���ʾ
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

					//����ͼ������
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
				else		//����������ʾ
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

					//����ͼ������
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
	DestroyWindow(hDlg);		//��ģ̬�Ի���ر�
	return TRUE;
}


