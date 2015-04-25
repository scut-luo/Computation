/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2013 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

#ifndef __CHART_H_122333444455555__
#define __CHART_H_122333444455555__

#include <windows.h>
#include <tchar.h>

//���ڶ�̬���ӿ��ʱ��ע�͵�#else�еĵڶ���
//���ھ�̬�����Դ��ʱ��ע�͵�#else�еĵ�һ��
#ifdef PLOTDLL_EXPORTS
#	define PLOTDLL_API __declspec(dllexport)
#else
#	define PLOTDLL_API __declspec(dllimport)
//#	define PLOTDLL_API
#endif

// CChart��װ��֧�ֵĻ�ͼ����
enum
{
	kTypeXY,							//0 ����ͼ
	kTypePie,							//1 ��ͼ
	kTypeStem,							//2 ��ͼ
	kTypeOscillo,						//3 ģ��ʾ����ͼ
	kTypeContourLine,					//4 �ȸ���ͼ
	kTypeContourMap,					//5 �ȸ���ͼ

	kTypeSingleLayerCount,				// ������ͼ��Ŀ

	kTypeSplit = kTypeSingleLayerCount,	//6 ������ͼ
	kTypeShareX,						//7 ����X����ͼ
	kTypeLayered,						//8 �ֲ���ͼ
	
	kType2DCount,						// ��ά��ͼ��Ŀ

	kType3DLine = kType2DCount,			//9 3ά����ͼ
	kType3DSurface,						//10 3ά����ͼ

	kTypeCount							//CChart��֧�ֵ���ͼ������
};

//typedef double (*FieldFunction)(double, int);

/*

// ���߻��Ʒ�ʽ
enum
{
	kXYPlotScatter = 0,		//0 ɢ��ͼ
	kXYPlotConnect = 1,		//1 ����ͼ
	kXYPlotStepHV = 2,		//2 ̨��ͼ��ˮƽ����ֱ
	kXYPlotStepVH = 3,		//3 ̨��ͼ����ֱ��ˮƽ
	kXYPlotStepHVH = 4,		//4 ̨��ͼˮƽ��ֱˮƽ
	kXYPlotStepVHV = 5,		//5 ̨��ͼ��ֱˮƽ��ֱ
	kXYPlotBezier = 6,		//6 ����������ͼ
  
	kPlotTypeCount
};


// ���ݱ�ǵ���״
enum
{
	kXYMarkerNone = 0,			//0 �ޱ��
	kXYMarkerCircle = 1,		//1 ԲȦ���
	kXYMarkerSquareUpright = 2,	//2 ��������
	kXYMarkerSquareOblique = 3,	//3 б������
	kXYMarkerTriangleLeft = 4,	//4 ����������
	kXYMarkerTriangleRight = 5,	//5 ����������
	kXYMarkerTriangleUp = 6,	//6 ��������
	kXYMarkerTriangleDown = 7,	//7 ��������
	kXYMarkerX = 8,				//8 бʮ��
	kXYMarkerCross = 9,			//9 ��ʮ��
	kXYMarkerBar = 10,			//10��ɫ��
	kXYMarkerDot = 11,			//11��
		
	kMarkerTypeCount
};

 // �������ķ�ʽ
enum
{
	kDataFillClosed = 0,			//0 ������
	kDataFillFromBottomAxis = 1,	//1 �������
	kDataFillFromTopAxis = 2,		//2 �������
	kDataFillFromLeftAxis = 3,      //3 �������
	kDataFillFromRightAxis = 4,		//4 �������
  
	kDataFillModeCount
};
// ���ݰ���ʾ��ʽ
enum
{
	kDataBarBaseBottom,		//0 �ӵײ���ʼ
	kDataBarBaseTop,		//1 �Ӷ�����ʼ
	kDataBarBaseLeft,		//2 ���󲿿�ʼ
	kDataBarBaseRight,		//3 ���Ҳ���ʼ
		
	kDataBarBaseCount
};

// ������ͼ�ķ���ģʽ
// ���ȡ��ע�ͣ�����ֱ���ô��ţ������ô��룬����2��ʾ��һ�Ҷ���������
enum
{
	kSplitNot=0,		//0 ������
	kSplitNM=1,			//1 ���зָ�
	kSplit3L1R2=2,		//2 ��1��2
	kSplit3L2R1=3,		//3 ��2��1
	kSplit3T1B2=4,		//4 ��1��2
	kSplit3T2B1=5,		//5 ��2��1
		
	kSplitModeCount
};

// ������λ��
// ���ȡ��ע�ͣ�����ֱ���ô��ţ������ô���
enum
{
	kLocationLeft = 0,	//0 ����
	kLocationBottom,	//1 ����
	kLocationRight,		//2 ����
	kLocationTop,		//3 ����
	kLocationCenterVL,	//4 ������ֱƫ��
	kLocationCenterVR,	//5 ������ֱƫ��
	kLocationCenterHB,	//6 ����ˮƽƫ��
	kLocationCenterHT,	//7 ����ˮƽƫ��
  
	kLocationTDX,		//8 ��άX��
	kLocationTDY,		//9 ��άY��
	kLocationTDZ,		//10��άZ��
	
	kLocationCount
};

// ͼ����������ʱ����������
// ���ȡ��ע�ͣ�����ֱ���ô��ţ������ô���
enum
{
	kZoomCenterLT,		//0 ����
	kZoomCenterLB,		//1 ����
	kZoomCenterRT,		//2 ����
	kZoomCenterRB,		//3 ����
	kZoomCenterCT,		//4 ����
	kZoomCenterARB,		//5 ����㣬��Ҫ�������λ��
  
	kZoomCenterCount
};

 // how to show the mouse tool tip
 enum
{
	kDesCoords,
	kDesXY,
	kDesElements,
	kDesXAndYAll,
		
	kDesCount
};

// �Ի����Ҽ��˵�ʹ�õ�����
enum
{
	kLangEnglish,		//0 Ӣ��
	kLangChinese,		//1 ����
	kLangCount
};
*/

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CChart

class	PLOTDLL_API	CChart
{
public:	
	// CChart�ڲ����ݽṹ
	struct CChartPara;
	// ����ڲ����ݵ�ָ��
	CChartPara *GetPara();
protected:
	// �ڲ�����
	CChartPara	*m_pPara;
	// �ͷ��ڲ���Դ
	void		Release();	

public:
	CChart();
	virtual	~CChart();

public:
	// ��ʼ��GDI+
	static	void	InitGdiPlus();
	// �ͷ�GDI+
	static	void	FreeGdiPlus();
	
	// ���öԻ����Ҽ��˵�ʹ�õ�����
	static	void	SetLangurage(int nLang);

	// �����Ƿ���ʾʾ����ģʽ�Ĳ˵��ͶԻ���
	static	void	SetOscilloMenus(bool bOscillo);
	// �������ݴ���˵��Ƿ���ʾ
	static	void	SetDataProcessing(bool process);

	// ����ͼ�񵽼�����
	static	void	CopyImageToClipBoard(HWND hWnd);
	// ����ͼ���ļ�
	static	void	SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt);
	// ��ӡͼ��
	static	void	PrintImage(HWND hWnd);

	// �����Ƿ����ù�����ʾ
	static	bool	IsEnableToolTip();
	// �����Ƿ����ù�����ʾ
	static	void	SetEnableToolTip(bool enable);
	// ���÷�����ͼ��ͼ��
	static	bool			ResizeSplit(CChart *pChart, int nSubPlots);

public:
	// ������ʾ״̬�£������Ƿ���ʾ������ĺ���
	void		SetAccompanyHLine(bool bDraw);
	// ������ʾ״̬�£������Ƿ���ʾ�����������
	void		SetAccompanyVLine(bool bDraw);

	// �����ͼ����Ƿ��ڷ�Χ��
	bool		CheckSubPlotIndex(int nPlotIndex);

public:
	// ��ù�����ʾ�ַ���
	TCHAR		*GetDescriptionByMousePos(HDC hDC, POINT point);
	// ���ù�����ʾ�ַ�������
	void		SetToolTipType(int type);

public:
	// �����Ƿ���Ӧ��������Ϣ
	bool		IsReact();
	// �����Ƿ���Ӧ��������Ϣ
	void		SetReact(bool react);
	// �����Ƿ���Ӧ����ƶ���Ϣ
	void		SetNoMouseMove(bool nomm);
	// �����Ƿ����ƻ�ͼ��
	bool		IsRectConfined();
	// �����Ƿ����ƻ�ͼ��
	void		SetRectConfined(bool confine);
	// ����˫�����Ƿ�ʹ��
	bool		IsBuffered();
	// ����˫�����Ƿ�ʹ��
	void		SetBuffered(bool buffered);
	// ���û�ͼ��ƫ��
	void		SetOffsetClient(int offsetX, int offsetY);

public:
	// �Դ��ھ����ͼ
	virtual	void		OnDraw(HWND hWnd);
	// ���豸�����Ļ�ͼ
	virtual	void		OnDraw(HDC hDC);
	// �ڴ�����ѡ�������ͼ
	virtual	void		OnDraw(HDC hDC, RECT destRect);
	// ���豸�����Ķ�Ӧ�Ĵ����ϣ�ѡ�������ͼ
	virtual	void		OnDraw(HWND hWnd, RECT destRect);

public:
	// ��������������Ϣ
	virtual	bool		OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseWheel( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	virtual	bool		OnKeyDown( HWND hWnd, UINT key );

	// ��Ӧ��Ϣ�������ػ�
	virtual	bool		OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// ��Ӧ��Ϣ���ػ�
	virtual	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �����Ҽ��˵������˵�����
	void		SetPluginMenu(void (*fcnAddPluginMenu)( void *plot, HMENU addMenu ), int (*fcnInterpretPluginMenu)( void *plot, HWND hWnd, int result ), void *pPara);
	// ���ػ���ʾȱʡ�Ҽ��˵�����ϲ���˵����ܿ�ʵ���Ҽ��˵�ȫ���Զ���
	void		SetDefMenus(bool bShow);
	// �û��Զ����ͼ����
	void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara);
	// �û��Զ����ͼ����
	void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex);
	// �û��Զ�������ƶ������
	void		SetPostMouseMoveFunc(bool	(*fcnPostMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);

public:
	// ��ȡ�ڲ�Plotָ��
	void		*GetPlot();
	// ��ȡͼ�����ͱ���
	int			GetType();
	// ����ͼ�����ͱ���
	bool		SetType(int nType);
	// ����ͼ�����ͱ��룬�����û�ͼ���ڣ���Ҫ������ά��ͼ����ά��ͼ������hWnd
	bool		SetType(int nType, HWND hWnd);
	
	// ������������
	bool		SetConfineRect(RECT ConfineRect);
	// ��ȡ��������
	RECT		GetConfinedRect();

	// �����ڲ��Ƿ�������
	bool		IsEmpty();

public:	
	// ������ߣ�ǰ������������������
	// ���ĸ�������Ҫ���ڷ�����ͼ���ֲ���ͼ������X����ͼ����ʾ��ͼ��š�����������ͬ
	// ����ֵ�����ߵ�ID�ţ�ע�ⲻ����ţ�����-1��ʾ�������ʧ�ܡ�����������ͬ
	int			AddCurve(double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ������ߣ�����X����Ϊʱ�䣬���ַ�����ʽ����
	//�ַ���ӦΪ"20130528234428"����������ʱ���룬��14λ���֣��������
	int			AddCurve(TCHAR **pStrTime, double *pY, int nLen, int nPlotIndex=0);
	// ��ӿ�����
	int			AddCurve(int nPlotIndex=0);
	// ������������
	int			UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ����ID�Ÿ�����������
	int			UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// �����ߵ�DataID���Index
	int			GetIndex(int dataID, int nPlotIndex=0);
	// �����ߵ�Index���DataID
	int			GetDataID(int nIndex, int nPlotIndex=0);
	// �������ߵ�dataID��������;���������콣����������Ҫ������
	void		SetDataID(int newID, int nIndex, int nPlotIndex=0);
	// ��ӱ�ͼ��һ��ֵ
	int			AddPie(double value);
	// ��ӱ�ͼ��һ��ֵ��ͬʱ���øÿ���ı���
	int			AddPie(double value, TCHAR* title);
	// �����ͼ��һ������
	int			AddStems(double *pData, int nLen);
	// �����ͼ��һ�����У�ͬʱ���ø����еı���
	int			AddStems(double *pData, int nLen, TCHAR* title);
	// ���2ά���ߵ�һ�����ݵ�
	// nDataIndex��ʾ���ߵ����,
	// nDataIndex������ڵ�ǰ���ߵ�������������һ�����ߣ����С��0���ߴ��ڵ�ǰ���������������ʧ��
	int			AddPoint2D(double x, double y, int nDataIndex=0, int nPlotIndex=0);
	// ������ݵ㣬����X����Ϊʱ�䣬���ַ�����ʽ����
	//�ַ���ӦΪ"20130528234428"����������ʱ���룬��14λ���֣��������
	int			AddPoint2D(TCHAR *strTime, double y, int nDataIndex=0, int nPlotIndex=0);
	// �����ά���ߵ�һ�����ݵ�
	int			AddPoint3D(double x, double y, double z, int nDataIndex=0);
	// ���һ����ά����
	int			AddCurve(double *pX, double *pY, double *pZ, int nLen);	
	// ���ó����������ڵȸ���ͼ����ͼ����ά����ͼ
	void		SetFieldFcn(double (*_pFieldFcn) (double, double));
	// ��ó�����ָ�룬���ڵȸ���ͼ����ͼ����ά����ͼ
	double		(*GetFieldFcn( ))( double, double );

	// ���������������
	void		ClearAllData();
	// �������ͼȫ����������
	void		ClearPlotData(int nPlotIndex=0);
	// ���������������
	void		ClearSingleData(int nDataIndex, int nPlotIndex=0);
	// ��յ�����������
	void		EmptySingleData(int nDataIndex, int nPlotIndex=0);
	// Ϊ����Ԥ���ڴ�ռ䡣��ҪĿ���Ǽӿ��ٶ�
	// CChart�ڲ����ݲ���vector��������ݵ���࣬�������ݵ�����ӣ�CChart�����ϵ�Ϊvector���·����ڴ棬Ӱ���ٶ�
	// �������ʵ�����Ԥ�����ڴ棬���һ���Գɹ������ڴ�
	void		SetReservedDataLength(int nLen, int nPlotIndex=0);
	// ������ߵ����ݵ���
	int			GetDataPointsCount(int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ������ݵ�
	int			RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ�һ�����ݵ�
	int			RemoveFirstPoint(int nDataIndex, int nPlotIndex=0);
	// ȥ�����������һ�����ݵ�
	int			RemoveLastPoint(int nDataIndex, int nPlotIndex=0);

public:
	// ����һ��ͼ�㣬���ڷֲ���ͼ������X����ͼ
	int			AddLayer();
	// ������ͼ���������ڷ�����ͼ������mode��ʾ����ģʽ��nRows��ʾ������nCols��ʾ����
	// ����ģʽ�μӱ��ļ�ͷ����ע�͵���enum�����Խ��ע�ͣ�����ֱ��ʹ����Щenum�ˡ�
	void		ResizePlots(int mode, int nRows, int nCols);
	// ����ͼ�����������ڷֲ���ͼ������X����ͼ
	void		ResizePlots(int nLayers);
	
public:
	// ���ͼ�����
	TCHAR*		GetTitle();
	// �����ͼ���⣬�����ڷ�����ͼ���ֲ���ͼ������X����ͼ
	TCHAR*		GetTitle(int nPlotIndex);
	// ����ͼ�����
	void		SetTitle(const TCHAR* title);
	// ������ͼ����
	void		SetTitle(const TCHAR* title, int nPlotIndex);
	// �ֲ���ͼ��ʾ������
	void		SetShowParentTitle(bool set);

	// �����ͼ����
	int			GetSubPlotsCount();
	// ����������������ڶ����ͼΪ��ͼ��������
	int			GetDataSetCount(int nPlotIndex=0);
	// ����������ߵı���,nDataIndex��ʾ���ߵ����
	const TCHAR*		GetDataTitle(int nDataIndex, int nPlotIndex=0);
	// �������ݱ���
	void		SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	void		SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	void		SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	void		SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �������ߵĻ��Ʒ�ʽ
	void		SetPlotType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ���ʾ
	void		SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ����
	void		SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��ǵ���״
	void		SetMarkerType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ�Ĵ�С
	void		SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���Ƶ�ʣ�0��ʾ����ǣ���n��ʾÿ��n������һ�Σ���n��ʾһ�����n����
	void		SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��һ������
	void		SetMarkFirstPoint(bool bMark, int nPlotIndex=0);
	// �����������ݵ����һ������
	void		SetMarkLastPoint(bool bMark, int nPlotIndex=0);
	// ��������ɢ����ʾģʽ
	void		SetScatter(int nDataIndex, int nPlotIndex=0);
	// �������������ɫ
	void		SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex=0);
	// �����������ݰ���ʾ
	void		SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex=0);
	// ����ѹ����ʾģʽ����Ҫ���ڴ�������ʱ����ʾ
	// ѹ����ʾģʽ�У�����������ݵ�����Ļ����ʾʱ��X��ͬ����ѹ����һ�����ݵ㣬ֵȡƽ����
	void		SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex=0);
	// �������ݰ�ֱ��ͼģʽ��ʾ
	void		SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex=0);

	// �ƶ�����
	bool		MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex=0);
	// ������ߵ�ˮƽƫ��
	double		GetDataOffsetX(int nDataIndex, int nPlotIndex=0);
	// �������ߵ�ˮƽƫ��
	void		SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex=0);
	// ������ߵ���ֱƫ��
	double		GetDataOffsetY(int nDataIndex, int nPlotIndex=0);
	// �������ߵ���ֱƫ��
	void		SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex=0);
	// ����X����λ����������ģ��ʾ����
	void		SetXUnit(TCHAR *unit, int nDataIndex);
	// ����Y����λ����������ģ��ʾ����
	void		SetYUnit(TCHAR *unit, int nDataIndex);
	// ��ͼ���н�����ʾY����ı����ߣ���������ģ��ʾ����
	void		SetShowYSCaleOnly(bool only);
	// ���ù��󶨵�����
	void		SetHoldCursorToCurve(bool hold, int nPlotIndex=0);
	// ��ù��󶨵��������
	int			GetIndexToHoldCursor(int nPlotIndex=0);
	// ��ʾˮƽ����ߣ���������ģ��ʾ����
	void		SetShowHCursor(bool show, int nPlotIndex=0);
	// ��ʾ��ֱ����ߣ���������ģ��ʾ����
	void		SetShowVCursor(bool show, int nPlotIndex=0);
	// ���ˮƽ���ֵ����������ģ��ʾ����
	double		GetCursorX(int nPlotIndex=0);
	// ����ˮƽ���ֵ����������ģ��ʾ����
	void		SetCursorX( double cursor, int nPlotIndex=0);
	// �����ֱ���ֵ����������ģ��ʾ����
	double		GetCursorY(int nPlotIndex=0);
	// ������ֱ���ֵ����������ģ��ʾ����
	void		SetCursorY( double cursor, int nPlotIndex=0);
	// ��ʾˮƽѡ��������������ģ��ʾ����
	void		SetShowHSel(bool show, int nPlotIndex=0);
	// ��ʾ��ֱѡ��������������ģ��ʾ����
	void		SetShowVSel(bool show, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	double		GetHSelLower(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	void		SetHSelLower(int val, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	double		GetHSelUpper(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	void		SetHSelUpper(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	double		GetVSelLower(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	void		SetVSelLower(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	double		GetVSelUpper(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	void		SetVSelUpper(int val, int nPlotIndex=0);
	// ���ˮƽѡ�����Ŀ�ȣ���������ģ��ʾ����
	double		GetHSelWidth(int nPlotIndex=0);
	// �����ֱѡ�����Ŀ�ȣ���������ģ��ʾ����
	double		GetVSelWidth(int nPlotIndex=0);
	
	// ���������ᣬ�������λ��location�����ļ�ͷ����ע�͵���enum
	void		AddAxis(int location, int nPlotIndex=0);
	// ������ͼ���������
	void		SetAxisTitle(const TCHAR* title, int location, int nPlotIndex);
	// �������������
	void		SetAxisTitle(const TCHAR* title, int location);
	// ������ͼʱ��������
	void		SetAxisToTime(bool bTime, int location, int nPlotIndex);
	// ����ʱ��������
	void		SetAxisToTime(bool bTime, int location);
	// �����������Ƿ���ʾ�̶�ֵ
	void		SetAxisLabelShow(bool bShow, int location);
	// ������ͼ�������Ƿ���ʾ�̶�ֵ
	void		SetAxisLabelShow(bool bShow, int location, int nPlotIndex);
	// ������ͼ�����ḡ�����
	void		SetAxisFloatTicks(bool flt, int nPlotIndex);
	// ���������ḡ�����
	void		SetAxisFloatTicks(bool flt);
	// �����������Ƿ���ʾ
	void		SetAxisShow(bool bShow, int location);
	// ������ͼ�������Ƿ���ʾ
	void		SetAxisShow(bool bShow, int location, int nPlotIndex);
	//�������Ƿ���ʾ
	bool		IsAxisShow(int location);
	// ��ͼ�������Ƿ���ʾ
	bool		IsAxisShow(int location, int nPlotIndex);

	// ����Ƿ�ʹ��ͼ��
	bool		IsUseLegend(int nPlotIndex=0);
	// �����Ƿ�ʹ��ͼ��
	void		SetUseLegend(bool bUse, int nPlotIndex=0);
	// ����ͼ�����������
	void		SetLegendMaxRows(int nRows, int nPlotIndex=0);
	// ����ÿ����ʾ��ͼ����
	void		SetLegendMaxCols(int nCols, int nPlotIndex=0);

	// ���������᷶Χ
	void		SetPlotRange(double xRange[2], double yRange[2]);
	void		SetXRange(double low, double high, int nPlotIndex);
	void		SetXRange(double low, double high);
	void		SetYRange(double low, double high, int nPlotIndex);
	void		SetYRange(double low, double high);
	// ���������᷶Χ�Զ�����
	void		SetXAutoRange(bool bAuto);
	void		SetYAutoRange(bool bAuto);
	void		SetXAutoRange(bool bAuto, int nPlotIndex);
	void		SetYAutoRange(bool bAuto, int nPlotIndex);
	// ���������᷶Χ�����ݷ�Χ��ͬ������������չ
	void		SetExactXRange(bool bExact, int nPlotIndex=0);
	void		SetExactYRange(bool bExact, int nPlotIndex=0);

	// ����������ȳ���ֻ��kTypeXY��Ч
	void		SetEqualXYAxis(bool bEqual);
	// ����������ȷ�Χ��ֻ��kTypeXY��Ч
	void		SetEqualXYRange(bool bEqual);
	// ����������ȱ����ߣ�ֻ��kTypeXY��Ч
	void		SetEqualXYScale(bool bEqual);

	// ����ͼ��߿���ʾ
	void		SetEdgeShow(bool bShow);
	// ����ͼ��߿���ʾ
	void		SetEdgeRoundConor(bool bRound);
	// ����ͼ��߿���ɫ
	void		SetEdgeColor(COLORREF color);
	// ����ͼ��߿���
	void		SetEdgeWidth(int width);

public:
	// ���ñ�ͼ�������ı���
	void		SetPieTitle(const TCHAR* title, int which);
	// ���ñ�ͼ����������ɫ
	void		SetPieColor(COLORREF color, int which);

public:
	// ������ͼ�������ӵı�ǩ
	void		SetStemLabel(const TCHAR* label, int which);
	// ������ͼ�������еı���
	void		SetStemTitle(const TCHAR* title, int nDataIndex);
	// ������ͼ����������ռ����
	void		SetStemRatio(double ratio);

public:
	// ���õȸ���ͼ�ȸ��ߵ�����
	void		SetContourLineNum(int num);
	// ���õȸ���ͼ�ȸ��߻��Ƶľ��ȣ�nԽ�󣬾���Խ�ߣ�һ��5�Ϳ�����
	void		SetContourPrecision(int n);

	// ������ͼ����ʼ��ɫ
	void		SetContourMapColor1(COLORREF cr);
	// ������ͼ��������ɫ
	void		SetContourMapColor2(COLORREF cr);

public:
	// ���ñ�����������һ����ͼ�󶨣����ڹ���X����ͼ
	void		SetGridBindLayer(int nPlotIndex);
	// �����ͼ��������
	bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex);
	// ���������
	bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV);
	// ������ͼ�����ߵĻ���
	void		SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex);
	// ���������ߵĻ���
	void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false);
	// ��ȡ��ͼ������ɫ
	COLORREF	GetBkgndColor(int nPlotIndex);
	// ��ȡ������ɫ
	COLORREF	GetBkgndColor();
	// ������ͼ������ɫ
	void		SetBkgndColor(COLORREF color, int nPlotIndex);
	// ���ñ�����ɫ
	void		SetBkgndColor(COLORREF color);

public:
	// ������ͼ���ݵ��ļ�
	bool		WriteToFile(TCHAR *pathName, int nPlotIndex);
	// ������ͼĳ�������ݵ��ļ�
	bool		WriteToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex);

public:
	// ����ͼ����������ģʽ
	void		SetZoomMode(bool zoom);
	// ����ͼ����������ʱX���������ϵ��
	void		SetZoomFactorX(double fact);
	// ����ͼ����������ʱY���������ϵ��
	void		SetZoomFactorY(double fact);
	// ����ͼ����������ʱ�������������ϵ��
	void		SetZoomFactor(double fact);
	// ����ͼ����������ʱ���������ģ����ĵĴ��ż����ļ���ʼ��ע�͵���enum
	void		SetZoomCenterMode(int center);
protected:
	// �ڲ����ú���
	RECT		GetZoomedRect(RECT destRect);

public:
	// ������ά���ߵ�����
	void		SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX);
	// ����XY��Ļ������
	void		SetShowXYMajorGrid( bool show );
	// ����YZ��Ļ������
	void		SetShowYZMajorGrid( bool show );
	// ����ZX��Ļ������
	void		SetShowZXMajorGrid( bool show );
	// ����XY��Ļ������
	void		SetShowXYMinorGrid( bool show );
	// ����YZ��Ļ������
	void		SetShowYZMinorGrid( bool show );
	// ����ZX��Ļ������
	void		SetShowZXMinorGrid( bool show );
	

public:
	// �ڷֲ���ͼ�ͷ�����ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	void		SetLRAxis();
	// �ڷֲ���ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	void		SetTBAxis();
	// �ײ㺯������ȡ��ͼ�Ļ�ͼ����
	RECT		GetPlotRect(int nPlotIndex);
	// �ײ㺯������ȡ��ͼ����
	RECT		GetPlotRect();
	// �ײ㺯������ȡ������ķ�Χ
	void		GetPlotRange(double xRange[2], double yRange[2], int nPlotIndex=0);

public:
	// ���ö�ʱ������
	void		SetTimerFunc(bool (*pFcnTimer)(void *pChart, void *pVoid), void *pPara);
	// ���ö�ʱ����ʱ��������λΪms
	void		SetTimerInterval(int interval);
	// ���ö�ʱ���ľ��ȣ���λΪms��ֻ�Ը߾��ȶ�ʱ��������
	void		SetTimerResolution(int res);
	
	// ������ʱ��
	// �߾��ȶ�ʱ�����ö�ý�嶨ʱ�����;��ȶ�ʱ������WM_TIMER��Ϣ
	void		StartTimer(HWND hWnd, bool bHighRes);
	// ֹͣ��ʱ��
	void		StopTimer(HWND hWnd);

	// ��ö�ʱ������ָ��
	bool		(*GetTimerFunc( ))( void *pChart, void *pVoid );
	// ��ö�ʱ�������������
	void		*GetTimerPara();
};

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CChartWnd

// �����࣬��װ����Ϣ������
class	PLOTDLL_API	CChartWnd
{
public:
	// CChartWnd�ڲ����õ����ݽṹ
	struct	CChartWndPara;
	// ����ڲ����ݵ�ָ��
	CChartWndPara *GetPara();
protected:
	// �ڲ�����
	CChartWndPara *m_pPara;
	
	// �ͷ��ڲ���Դ
	void	Release();

public:
	CChartWnd();
	virtual	~CChartWnd();

public:
	// ��ȡCChart���ָ�룬�Ե����亯��
	CChart	*GetChart();
	// ����CChart�������Ҫ�������໯
	void	SetChart(CChart *pChart);
	// �ı���ͼ����
	virtual	bool	SetChartType(int nType);
	
	// ճ���ڣ��������ʽ
	virtual	bool	Attach(HWND hWnd);
	// ճ���ڣ�ͬʱ���û�ͼ����
	virtual	bool	Attach(HWND hWnd, int nType);
	// ճ���ڣ�ͬʱ���û�ͼ���ͣ������ƻ�ͼ��
	virtual	bool	Attach(HWND hWnd, int nType, RECT rtClient);
	// ճ���ڣ�ճ���Ի�����, �����ؼ����
	virtual	bool	Attach(HWND hDlg, HWND hCtrl, int nType);
	// ճ���ڣ�ճ���Ի����ϣ������ؼ�ID
	virtual	bool	Attach(HWND hDlg, UINT nCtrlID, int nType);
	// ж����
	bool	Detach();
	// ������������
	HWND	CreatePopupWnd(HWND hWndParent, int nType, TCHAR *wndTitle, int x=0, int y=0, int cx=640, int cy=480, DWORD dwStyle=0);

	// �ػ�
	void	ReDraw();
};

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CCommWnd

const int nBaudRates = 15;
const int aBaudRates[]=
{
	110,
		300,
		600,
		1200,
		2400,
		4800,
		9600,
		14400,
		19200,
		38400,
		56000,
		57600,
		115200,
		128000,
		256000
};

const struct Parity
{
	BYTE btParity;
	TCHAR sParity[24];
}Paritys[] = 
{
	{NOPARITY    ,_T("NO")},
	{ODDPARITY   ,_T("ODD")},
	{EVENPARITY  ,_T("EVEN")},
	{MARKPARITY  ,_T("MARK")},
	{SPACEPARITY ,_T("SPACE")}
};

const BYTE ByteSizes[] = {4, 5, 6, 7, 8};

const struct StopBit
{
	BYTE btStopBit;
	TCHAR sStopBit[24];
}StopBits[] = 
{
	{ONESTOPBIT   ,_T("ONESTOPBIT")},
	{ONE5STOPBITS ,_T("ONE5STOPBITS")},
	{TWOSTOPBITS  ,_T("TWOSTOPBITS")},
};

enum
{
	NoFlowControl,
		CtsRtsFlowControl,
		CtsDtrFlowControl,
		DsrRtsFlowControl,
		DsrDtrFlowControl,
		XonXoffFlowControl
};
const struct FlowControl
{
	BYTE btFlowCtrl;
	TCHAR sFlowCtrl[24];
}FlowControls[] =
{
	{NoFlowControl	,_T("No")},
	{CtsRtsFlowControl	,_T("CtsRts")},
	{CtsDtrFlowControl	,_T("CtsDtr")},
	{DsrRtsFlowControl	,_T("DsrRts")},
	{DsrDtrFlowControl	,_T("DsrDtr")},
	{XonXoffFlowControl	,_T("XonXoff")},
};

// ��������������
class	PLOTDLL_API	CCommWnd : public CChartWnd
{
public:
	// CCommWnd�ڲ����õ����ݽṹ
	struct	CCommWndPara;
	// ����ڲ����ݵ�ָ��
	CCommWndPara *GetPara();
protected:
	// �ڲ�����
	CCommWndPara *m_pPara;
	// �ͷ��ڲ���Դ
	void	Release();
public:
	CCommWnd();
	virtual	~CCommWnd();

	// ������ͼ����
	bool	SetChartType(int nType);

public:
	// ��ȡ���ڲ���
	bool	GetCommPara(int nChannelIndex, DWORD &dwPort, DWORD &dwBaudRate, BYTE &btParity, BYTE &btByteSize, BYTE &btStopBits, BYTE &btFlowCtrl, bool &bOpen);
	// ��ȡָ�����ڲ���
	bool	GetCommPara(DWORD dwPort, DWORD &dwBaudRate, BYTE &btParity, BYTE &btByteSize, BYTE &btStopBits, BYTE &btFlowCtrl, bool &bOpen);
	// ֱ�Ӵ򿪴���
	/*! \param[in] dwPort ������� 1~1024 \param[in] dwBaudRate ������ \param[in] btParity  ��żУ�� \param[in] btByteSize ����λ��	\param[in] btStopBits ֹͣλ��	*/
	//! ֱ�Ӵ򿪴��� ȱʡ 9600, n, 8, 1 \sa bool Open(DWORD dwPort, LPCTSTR szSetStr)
	bool	OpenCommPort(DWORD dwPort, TCHAR *strTitle = _T(""), DWORD dwBaudRate = 9600, BYTE btParity = NOPARITY, BYTE btByteSize = 8, BYTE btStopBits = ONESTOPBIT, BYTE btFlowCtrl = NoFlowControl);
	// �ر�ָ���˿ڴ��ڣ���ɾ�������Դ(���ߵ�)
	void	CloseCommPort(DWORD dwPort);
	// �ر�ָ��ͨ���Ĵ��ڣ���ɾ�������Դ(���ߵ�)
	void	CloseCommCh(int nChannelIndex);
	// �ر�����ͨ�����ڣ���ɾ�������Դ(���ߵ�)
	void	CloseComms();
	// ����Ԥ�����ڵĶԻ��򣬸���bOpenȷ���Ƿ��
	bool	ShowCommDlg(bool bOpen);
	// ׼��һ�����ڣ�������
	bool	PrepareCommPort(DWORD dwPort, TCHAR *strTitle = _T(""), DWORD dwBaudRate = 9600, BYTE btParity = NOPARITY, BYTE btByteSize = 8, BYTE btStopBits = ONESTOPBIT, BYTE btFlowCtrl = NoFlowControl);
	// �޸�ָ���˿ڵĴ��ڲ���
	bool	ModifyComm(DWORD dwPort, TCHAR *strTitle = _T(""), DWORD dwBaudRate = 9600, BYTE btParity = NOPARITY, BYTE btByteSize = 8, BYTE btStopBits = ONESTOPBIT, BYTE btFlowCtrl = NoFlowControl);
	// �޸Ĵ���������ָ��λ�õĴ��ڲ���
	bool	ModifyComm(int nChannelIndex, DWORD dwPort, TCHAR *strTitle = _T(""), DWORD dwBaudRate = 9600, BYTE btParity = NOPARITY, BYTE btByteSize = 8, BYTE btStopBits = ONESTOPBIT, BYTE btFlowCtrl = NoFlowControl);
	// ����ǰԤ���õĴ���
	bool	StartComms();
	// ֹͣȫ������
	void	StopComms();
	// ���ö˿ڽ��յ�������
	void	ResetComms();
	// ������������ݲ������˿�
	void	Trig();
	// ��ȡͨ������
	int		GetChannels();
	// ��ȡ��Ч�˿�����
	int		GetValidPorts();
	// ��ȡ��Ч�˿ں�
	int		GetValidPorts(int nChannelIndex);
	// ���ʱ����
	double	GetInterval();
	// ����ʱ����
	void	SetInterval(double fInterval);
	// �Ƿ���Ԥ���õĶ˿���������
	bool	IsAnyRunning();
	// �Ƿ���Ԥ���õĶ˿ڲ�������
	bool	IsAnyPausing();
	// ��ȡÿͨ��������
	int		GetMaxChPoints();
	// ����ÿͨ��������
	void	SetMaxChPoints(int nPts);
	// ͨ��������Ƿ����
	bool	IsMaxChReset();
	// ����ͨ��������Ƿ����
	void	SetMaxChReset(bool	reset);
	// ����ͨ������
	bool	WriteToFile(TCHAR *pathName, int nChannel);
};

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CDBaseWnd

// ���ݿ�����������
class	PLOTDLL_API	CDBaseWnd : public CChartWnd
{
public:
	// CDBaseWnd�ڲ����õ����ݽṹ
	struct	CDBaseWndPara;
	// ����ڲ����ݵ�ָ��
	CDBaseWndPara *GetPara();
protected:
	// �ڲ�����
	CDBaseWndPara *m_pPara;
	// �ͷ��ڲ���Դ
	void	Release();

public:
	CDBaseWnd();
	virtual	~CDBaseWnd();

public:
	int		AddCurve(TCHAR *strXField, TCHAR *strYField, TCHAR *strTableName, TCHAR *strDSN, int nPlotIndex = 0, TCHAR *strUserName = _TEXT(""), TCHAR *strPswd = _TEXT(""));
	int		UpdateCurve(int nDataIndex, int nPlotIndex=0);
	void	ClearSingleData(int nDataIndex, int nPlotIndex=0);
};

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CSocketWnd

// �׽�������������
class	PLOTDLL_API	CSocketWnd : public CChartWnd
{
public:
	// CSocketWnd�ڲ����õ����ݽṹ
	struct	CSocketWndPara;
	// ����ڲ����ݵ�ָ��
	CSocketWndPara *GetPara();
protected:
	// �ڲ�����
	CSocketWndPara *m_pPara;
	// �ͷ��ڲ���Դ
	void	Release();
	
public:
	CSocketWnd();
	virtual	~CSocketWnd();
	
public:
	int		StartCurve(bool bConnected, int nPort, double fDx = 1.0, char *symbol = " ", int nPlotIndex=0);
	void	CloseCurve(int nDataIndex, int nPlotIndex=0);
};

///////////////////////////////////////////////////////////////////////////////////////////
// aliases
typedef	CChart		MyChart;
typedef	CChartWnd	MyChartWnd;

#ifdef PLOTDLL_API
#	undef PLOTDLL_API
#endif //PLOTDLL_API

#endif //__CHART_H_122333444455555__
///////////////////////////////////////////////////////////////////////////////////////////
