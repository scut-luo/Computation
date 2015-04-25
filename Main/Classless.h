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

#ifndef __CLASSLESS_H_122333444455555__
#define __CLASSLESS_H_122333444455555__

#include <windows.h>

//���ڶ�̬���ӿ��ʱ��ע�͵�#else�еĵڶ���
//���ھ�̬�����Դ��ʱ��ע�͵�#else�еĵ�һ��
#ifdef PLOTDLL_EXPORTS
#	define PLOTDLL_API __declspec(dllexport)
#else
#	define PLOTDLL_API __declspec(dllimport)
//#	define PLOTDLL_API
#endif

namespace CChart_Classless
{
/////////////////////////////////////////////////////////////////////////////
// ���º�������CChartWnd
	// �ı���ͼ����
	PLOTDLL_API bool	SetChartType(int nType);

	// ճ���ڣ��������ʽ
	PLOTDLL_API bool	Attach(HWND hWnd);
	// ճ���ڣ�ͬʱ���û�ͼ����
	PLOTDLL_API bool	Attach(HWND hWnd, int nType);
	// ճ���ڣ�ͬʱ���û�ͼ���ͣ������ƻ�ͼ��
	PLOTDLL_API bool	Attach(HWND hWnd, int nType, RECT rtClient);
	// ճ���ڣ�ճ���Ի�����, �����ؼ����
	PLOTDLL_API bool	Attach(HWND hDlg, HWND hCtrl, int nType);
	// ճ���ڣ�ճ���Ի����ϣ������ؼ�ID
	PLOTDLL_API bool	Attach(HWND hDlg, UINT nCtrlID, int nType);
	// ж����
	PLOTDLL_API bool	Detach();
	// ������������
	HWND	CreatePopupWnd(HWND hWndParent, int nType, TCHAR *wndTitle, int x=0, int y=0, int cx=640, int cy=480, DWORD dwStyle=0);
	// �ػ�
	PLOTDLL_API void	ReDraw();

/////////////////////////////////////////////////////////////////////////////
// ���º�������CChart
	//��ʼ��GDI+
	PLOTDLL_API void	InitGdiPlus();
	//ж��GDI+
	PLOTDLL_API void	FreeGdiPlus();
	// ���öԻ����Ҽ��˵�ʹ�õ�����
	PLOTDLL_API	void	SetLangurage(int nLang);
	// ���������Ƿ����������ƶ�
	PLOTDLL_API	void	SetOscilloMenus(bool bMoveable);
	// �������ݴ���˵��Ƿ���ʾ
	PLOTDLL_API	void	SetDataProcessing(bool process);
	// ����ͼ�񵽼�����
	PLOTDLL_API	void	CopyImageToClipBoard(HWND hWnd);
	// ����ͼ���ļ�
	PLOTDLL_API	void	SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt);
	// ��ӡͼ��
	PLOTDLL_API	void	PrintImage(HWND hWnd);

	// �����Ƿ����ù�����ʾ
	PLOTDLL_API	bool	IsEnableToolTip();
	// �����Ƿ����ù�����ʾ
	PLOTDLL_API	void	SetEnableToolTip(bool enable);
	// ���ù�����ʾ�ַ�������
	PLOTDLL_API	void	SetToolTipType(int type);
	
	// �����Ƿ���Ӧ��������Ϣ
	PLOTDLL_API bool		IsReact();
	// �����Ƿ���Ӧ��������Ϣ
	PLOTDLL_API void		SetReact(bool react);
	// �����Ƿ����ƻ�ͼ��
	PLOTDLL_API bool		IsRectConfined();
	// �����Ƿ����ƻ�ͼ��
	PLOTDLL_API void		SetRectConfined(bool confine);
	// ����˫�����Ƿ�ʹ��
	PLOTDLL_API bool		IsBuffered();
	// ����˫�����Ƿ�ʹ��
	PLOTDLL_API void		SetBuffered(bool buffered);
	// ���û�ͼ��ƫ��
	PLOTDLL_API void		SetOffsetClient(int offsetX, int offsetY);

	// �����Ҽ��˵������˵�����
	PLOTDLL_API void		SetPluginMenu(void (*fcnAddPluginMenu)( void *plot, HMENU addMenu ), int (*fcnInterpretPluginMenu)( void *plot, HWND hWnd, int result ), void *pPara);
	// ���ػ���ʾȱʡ�Ҽ��˵�����ϲ���˵����ܿ�ʵ���Ҽ��˵�ȫ���Զ���
	PLOTDLL_API void		SetDefMenus(bool bShow);
	// �û��Զ����ͼ����
	PLOTDLL_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara);
	// �û��Զ����ͼ����
	PLOTDLL_API void		SetUserDrawingFunc(void	(*fcnUserDrawing)( HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex);
	// �û��Զ�������ƶ������
	PLOTDLL_API void		SetPostMouseMoveFunc(bool	(*fcnPostMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para ), void *pPara);


	// ������ߣ�ǰ������������������
	// ���ĸ�������Ҫ���ڷ�����ͼ���ֲ���ͼ������X����ͼ����ʾ��ͼ��š�����������ͬ
	// ����ֵ�����ߵ�ID�ţ�ע�ⲻ����ţ�����-1��ʾ�������ʧ�ܡ�����������ͬ
	PLOTDLL_API int			AddCurve(double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ������ߣ�����X����Ϊʱ�䣬���ַ�����ʽ����
	//�ַ���ӦΪ"20130528234428"����������ʱ���룬��14λ���֣��������
	PLOTDLL_API int			AddCurve(TCHAR **pStrTime, double *pY, int nLen, int nPlotIndex=0);
	// ��ӿ�����
	PLOTDLL_API int			AddCurve(int nPlotIndex=0);
	// ������������
	PLOTDLL_API int			UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// ����ID�Ÿ�����������
	PLOTDLL_API int			UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// �����ߵ�DataID���Index
	PLOTDLL_API int			GetIndex(int dataID, int nPlotIndex=0);
	// �����ߵ�Index���DataID
	PLOTDLL_API int			GetDataID(int nIndex, int nPlotIndex=0);
	// ��ӱ�ͼ��һ��ֵ
	PLOTDLL_API int			AddPie(double value);
	// ��ӱ�ͼ��һ��ֵ��ͬʱ���øÿ���ı���
	PLOTDLL_API int			AddPie(double value, TCHAR* title);
	// �����ͼ��һ������
	PLOTDLL_API int			AddStems(double *pData, int nLen);
	// �����ͼ��һ�����У�ͬʱ���ø����еı���
	PLOTDLL_API int			AddStems(double *pData, int nLen, TCHAR* title);
	// ���2ά���ߵ�һ�����ݵ�
	// nDataIndex��ʾ���ߵ����,
	// nDataIndex������ڵ�ǰ���ߵ�������������һ�����ߣ����С��0���ߴ��ڵ�ǰ���������������ʧ��
	PLOTDLL_API int			AddPoint2D(double x, double y, int nDataIndex=0, int nPlotIndex=0);
	// ������ݵ㣬����X����Ϊʱ�䣬���ַ�����ʽ����
	//�ַ���ӦΪ"20130528234428"����������ʱ���룬��14λ���֣��������
	PLOTDLL_API int			AddPoint2D(TCHAR *strTime, double y, int nDataIndex=0, int nPlotIndex=0);
	// �����ά���ߵ�һ�����ݵ�
	PLOTDLL_API int			AddPoint3D(double x, double y, double z, int nDataIndex=0);
	// ���һ����ά����
	PLOTDLL_API int			AddCurve(double *pX, double *pY, double *pZ, int nLen);	
	// ���ó����������ڵȸ���ͼ����ͼ����ά����ͼ
	PLOTDLL_API void		SetFieldFcn(double (*_pFieldFcn) (double, double));
	// ��ó�����ָ�룬���ڵȸ���ͼ����ͼ����ά����ͼ
	PLOTDLL_API double		(*GetFieldFcn( ))( double, double );
	// ���������������
	PLOTDLL_API void		ClearAllData();
	// �������ͼȫ����������
	PLOTDLL_API void		ClearPlotData(int nPlotIndex=0);
	// ���������������
	PLOTDLL_API void		ClearSingleData(int nDataIndex, int nPlotIndex=0);
	// ��յ�����������
	PLOTDLL_API void		EmptySingleData(int nDataIndex, int nPlotIndex=0);
	// Ϊ����Ԥ���ڴ�ռ䡣��ҪĿ���Ǽӿ��ٶ�
	// CChart�ڲ����ݲ���vector��������ݵ���࣬�������ݵ�����ӣ�CChart�����ϵ�Ϊvector���·����ڴ棬Ӱ���ٶ�
	// �������ʵ�����Ԥ�����ڴ棬���һ���Գɹ������ڴ�
	PLOTDLL_API void		SetReservedDataLength(int nLen, int nPlotIndex=0);
	// ������ߵ����ݵ���
	PLOTDLL_API int			GetDataPointsCount(int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ������ݵ�
	PLOTDLL_API int			RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// ȥ�������ϵ�һ�����ݵ�
	PLOTDLL_API int			RemoveFirstPoint(int nDataIndex, int nPlotIndex=0);
	// ȥ�����������һ�����ݵ�
	PLOTDLL_API int			RemoveLastPoint(int nDataIndex, int nPlotIndex=0);

	// ����һ��ͼ�㣬���ڷֲ���ͼ������X����ͼ
	PLOTDLL_API int			AddLayer();
	// ������ͼ���������ڷ�����ͼ������mode��ʾ����ģʽ��nRows��ʾ������nCols��ʾ����
	// ����ģʽ�μӱ��ļ�ͷ����ע�͵���enum�����Խ��ע�ͣ�����ֱ��ʹ����Щenum�ˡ�
	PLOTDLL_API void		ResizePlots(int mode, int nRows, int nCols);
	// ����ͼ�����������ڷֲ���ͼ������X����ͼ
	PLOTDLL_API void		ResizePlots(int nLayers);
	
	// ���ͼ�����
	PLOTDLL_API const TCHAR*		GetTitle();
	// �����ͼ���⣬�����ڷ�����ͼ���ֲ���ͼ������X����ͼ
	PLOTDLL_API const TCHAR*		GetTitle(int nPlotIndex);
	// ����ͼ�����
	PLOTDLL_API void		SetTitle(const TCHAR* title);
	// ������ͼ����
	PLOTDLL_API void		SetTitle(const TCHAR* title, int nPlotIndex);
	// �ֲ���ͼ��ʾ������
	PLOTDLL_API void		SetShowParentTitle(bool set);

	// �����ͼ����
	PLOTDLL_API int			GetSubPlotsCount();

	// ����������ߵı���,nDataIndex��ʾ���ߵ����
	PLOTDLL_API const TCHAR*		GetDataTitle(int nDataIndex, int nPlotIndex=0);
	// �������ݱ���
	PLOTDLL_API void		SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	PLOTDLL_API void		SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	PLOTDLL_API void		SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex=0);
	// ����������ɫ
	PLOTDLL_API void		SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �������ߵĻ��Ʒ�ʽ
	PLOTDLL_API void		SetPlotType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ���ʾ
	PLOTDLL_API void		SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ����Ƿ����
	PLOTDLL_API void		SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��ǵ���״
	PLOTDLL_API void		SetMarkerType(int nType, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ�Ĵ�С
	PLOTDLL_API void		SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ���Ƶ�ʣ�0��ʾ����ǣ���n��ʾÿ��n������һ�Σ���n��ʾһ�����n����
	PLOTDLL_API void		SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// �����������ݵ��һ������
	PLOTDLL_API void		SetMarkFirstPoint(bool bMark, int nPlotIndex=0);
	// �����������ݵ����һ������
	PLOTDLL_API void		SetMarkLastPoint(bool bMark, int nPlotIndex=0);
	// ��������ɢ����ʾģʽ
	PLOTDLL_API void		SetScatter(int nDataIndex, int nPlotIndex=0);
	// �������������ɫ
	PLOTDLL_API void		SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex=0);
	// �����������ݰ���ʾ
	PLOTDLL_API void		SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex=0);
	// ����ѹ����ʾģʽ����Ҫ���ڴ�������ʱ����ʾ
	// ѹ����ʾģʽ�У�����������ݵ�����Ļ����ʾʱ��X��ͬ����ѹ����һ�����ݵ㣬ֵȡƽ����
	PLOTDLL_API void		SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex=0);
	// �������ݰ�ֱ��ͼģʽ��ʾ
	PLOTDLL_API void		SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex=0);

	// �ƶ�����
	PLOTDLL_API bool		MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex=0);
	// ������ߵ�ˮƽƫ��
	PLOTDLL_API double		GetDataOffsetX(int nDataIndex, int nPlotIndex=0);
	// �������ߵ�ˮƽƫ��
	PLOTDLL_API void		SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex=0);
	// ������ߵ���ֱƫ��
	PLOTDLL_API double		GetDataOffsetY(int nDataIndex, int nPlotIndex=0);
	// �������ߵ���ֱƫ��
	PLOTDLL_API void		SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex=0);
	// ����X����λ����������ģ��ʾ����
	PLOTDLL_API void		SetXUnit(TCHAR *unit, int nDataIndex);
	// ����Y����λ����������ģ��ʾ����
	PLOTDLL_API void		SetYUnit(TCHAR *unit, int nDataIndex);
	// ��ͼ���н�����ʾY����ı����ߣ���������ģ��ʾ����
	PLOTDLL_API void		SetShowYSCaleOnly(bool only);
	// ���ù��󶨵�����
	PLOTDLL_API void		SetHoldCursorToCurve(bool hold, int nPlotIndex=0);
	// ��ù��󶨵��������
	PLOTDLL_API int			GetIndexToHoldCursor(int nPlotIndex=0);
	// ��ʾˮƽ����ߣ���������ģ��ʾ����
	PLOTDLL_API void		SetShowHCursor(bool show, int nPlotIndex=0);
	// ��ʾ��ֱ����ߣ���������ģ��ʾ����
	PLOTDLL_API void		SetShowVCursor(bool show, int nPlotIndex=0);
	// ���ˮƽ���ֵ����������ģ��ʾ����
	PLOTDLL_API double		GetCursorX(int nPlotIndex=0);
	// ����ˮƽ���ֵ����������ģ��ʾ����
	PLOTDLL_API void		SetCursorX( double cursor, int nPlotIndex=0);
	// �����ֱ���ֵ����������ģ��ʾ����
	PLOTDLL_API double		GetCursorY(int nPlotIndex=0);
	// ������ֱ���ֵ����������ģ��ʾ����
	PLOTDLL_API void		SetCursorY( double cursor, int nPlotIndex=0);
	// ��ʾˮƽѡ��������������ģ��ʾ����
	PLOTDLL_API void		SetShowHSel(bool show, int nPlotIndex=0);
	// ��ʾ��ֱѡ��������������ģ��ʾ����
	PLOTDLL_API void		SetShowVSel(bool show, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API double		GetHSelLower(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API void		SetHSelLower(int val, int nPlotIndex=0);
	// ���ˮƽѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API double		GetHSelUpper(int nPlotIndex=0);
	// ����ˮƽѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API void		SetHSelUpper(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API double		GetVSelLower(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API void		SetVSelLower(int val, int nPlotIndex=0);
	// �����ֱѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API double		GetVSelUpper(int nPlotIndex=0);
	// ������ֱѡ���������ޣ���������ģ��ʾ����
	PLOTDLL_API void		SetVSelUpper(int val, int nPlotIndex=0);
	// ���ˮƽѡ�����Ŀ�ȣ���������ģ��ʾ����
	PLOTDLL_API double		GetHSelWidth(int nPlotIndex=0);
	// �����ֱѡ�����Ŀ�ȣ���������ģ��ʾ����
	PLOTDLL_API double		GetVSelWidth(int nPlotIndex=0);
	
	// ���������ᣬ�������λ��location�����ļ�ͷ����ע�͵���enum
	PLOTDLL_API void		AddAxis(int location, int nPlotIndex=0);
	// ������ͼ���������
	PLOTDLL_API void		SetAxisTitle(const TCHAR* title, int location, int nPlotIndex);
	// �������������
	PLOTDLL_API void		SetAxisTitle(const TCHAR* title, int location);
	// ������ͼʱ��������
	PLOTDLL_API void		SetAxisToTime(bool bTime, int location, int nPlotIndex);
	// ����ʱ��������
	PLOTDLL_API void		SetAxisToTime(bool bTime, int location);
	// �����������Ƿ���ʾ�̶�ֵ
	PLOTDLL_API void		SetAxisLabelShow(bool bShow, int location);
	// ������ͼ�������Ƿ���ʾ�̶�ֵ
	PLOTDLL_API void		SetAxisLabelShow(bool bShow, int location, int nPlotIndex);
	// ������ͼ�����ḡ�����
	PLOTDLL_API void		SetAxisFloatTicks(bool flt, int nPlotIndex);
	// ���������ḡ�����
	PLOTDLL_API void		SetAxisFloatTicks(bool flt);
	// �����������Ƿ���ʾ
	PLOTDLL_API void		SetAxisShow(bool bShow, int location);
	// ������ͼ�������Ƿ���ʾ
	PLOTDLL_API void		SetAxisShow(bool bShow, int location, int nPlotIndex);
	//�������Ƿ���ʾ
	PLOTDLL_API bool		IsAxisShow(int location);
	// ��ͼ�������Ƿ���ʾ
	PLOTDLL_API bool		IsAxisShow(int location, int nPlotIndex);

	// ����Ƿ�ʹ��ͼ��
	PLOTDLL_API bool		IsUseLegend(int nPlotIndex=0);
	// �����Ƿ�ʹ��ͼ��
	PLOTDLL_API void		SetUseLegend(bool bUse, int nPlotIndex=0);
	// ����ͼ�����������
	PLOTDLL_API void		SetLegendMaxRows(int nRows, int nPlotIndex=0);
	// ����ÿ����ʾ��ͼ����
	PLOTDLL_API void		SetLegendMaxCols(int nCols, int nPlotIndex=0);

	// ���������᷶Χ
	PLOTDLL_API void		SetPlotRange(double xRange[2], double yRange[2]);
	PLOTDLL_API void		SetXRange(double low, double high, int nPlotIndex);
	PLOTDLL_API void		SetXRange(double low, double high);
	PLOTDLL_API void		SetYRange(double low, double high, int nPlotIndex);
	PLOTDLL_API void		SetYRange(double low, double high);
	// ���������᷶Χ�Զ�����
	PLOTDLL_API void		SetXAutoRange(bool bAuto);
	PLOTDLL_API void		SetYAutoRange(bool bAuto);
	PLOTDLL_API void		SetXAutoRange(bool bAuto, int nPlotIndex);
	PLOTDLL_API void		SetYAutoRange(bool bAuto, int nPlotIndex);
	// ���������᷶Χ�����ݷ�Χ��ͬ������������չ
	PLOTDLL_API void		SetExactXRange(bool bExact, int nPlotIndex=0);
	PLOTDLL_API void		SetExactYRange(bool bExact, int nPlotIndex=0);

	// ����������ȳ���ֻ��kTypeXY��Ч
	PLOTDLL_API void		SetEqualXYAxis(bool bEqual);
	// ����������ȷ�Χ��ֻ��kTypeXY��Ч
	PLOTDLL_API void		SetEqualXYRange(bool bEqual);
	// ����������ȱ����ߣ�ֻ��kTypeXY��Ч
	PLOTDLL_API void		SetEqualXYScale(bool bEqual);

	// ����ͼ��߿���ʾ
	PLOTDLL_API void		SetEdgeShow(bool bShow);
	// ����ͼ��߿���ʾ
	PLOTDLL_API void		SetEdgeRoundConor(bool bRound);
	// ����ͼ��߿���ɫ
	PLOTDLL_API void		SetEdgeColor(COLORREF color);
	// ����ͼ��߿���
	PLOTDLL_API void		SetEdgeWidth(int width);

	// ���ñ�ͼ�������ı���
	PLOTDLL_API void		SetPieTitle(const TCHAR* title, int which);
	// ���ñ�ͼ����������ɫ
	PLOTDLL_API void		SetPieColor(COLORREF color, int which);

	// ������ͼ�������ӵı�ǩ
	PLOTDLL_API void		SetStemLabel(const TCHAR* label, int which);
	// ������ͼ�������еı���
	PLOTDLL_API void		SetStemTitle(const TCHAR* title, int nDataIndex);
	// ������ͼ����������ռ����
	PLOTDLL_API void		SetStemRatio(double ratio);

	// ���õȸ���ͼ�ȸ��ߵ�����
	PLOTDLL_API void		SetContourLineNum(int num);
	// ���õȸ���ͼ�ȸ��߻��Ƶľ��ȣ�nԽ�󣬾���Խ�ߣ�һ��5�Ϳ�����
	PLOTDLL_API void		SetContourPrecision(int n);

	// ������ͼ����ʼ��ɫ
	PLOTDLL_API void		SetContourMapColor1(COLORREF cr);
	// ������ͼ��������ɫ
	PLOTDLL_API void		SetContourMapColor2(COLORREF cr);

	// ���ñ�����������һ����ͼ��
	// ���ڹ���X����ͼ
	PLOTDLL_API void		SetGridBindLayer(int nPlotIndex);
	// �����ͼ��������
	PLOTDLL_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex);
	// ���������
	PLOTDLL_API bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV);
	// ������ͼ�����ߵĻ���
	PLOTDLL_API void		SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex);
	// ���������ߵĻ���
	PLOTDLL_API void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false);
	// ��ȡ��ͼ������ɫ
	PLOTDLL_API COLORREF	GetBkgndColor(int nPlotIndex);
	// ��ȡ������ɫ
	PLOTDLL_API COLORREF	GetBkgndColor();
	// ������ͼ������ɫ
	PLOTDLL_API void		SetBkgndColor(COLORREF color, int nPlotIndex);
	// ���ñ�����ɫ
	PLOTDLL_API void		SetBkgndColor(COLORREF color);

	// ������ͼ���ݵ��ļ�
	PLOTDLL_API bool		WriteToFile(TCHAR *pathName, int nPlotIndex);
	// ������ͼĳ�������ݵ��ļ�
	PLOTDLL_API bool		WriteToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex);

	// ����ͼ����������ģʽ
	PLOTDLL_API void		SetZoomMode(bool zoom);
	// ����ͼ����������ʱX���������ϵ��
	PLOTDLL_API void		SetZoomFactorX(double fact);
	// ����ͼ����������ʱY���������ϵ��
	PLOTDLL_API void		SetZoomFactorY(double fact);
	// ����ͼ����������ʱ�������������ϵ��
	PLOTDLL_API void		SetZoomFactor(double fact);
	// ����ͼ����������ʱ���������ģ����ĵĴ��ż����ļ���ʼ��ע�͵���enum
	PLOTDLL_API void		SetZoomCenterMode(int center);

	// ������ά���ߵ�����
	PLOTDLL_API void		SetShowXYMajorGrid( bool show );
	PLOTDLL_API void		SetShowYZMajorGrid( bool show );
	PLOTDLL_API void		SetShowZXMajorGrid( bool show );
	PLOTDLL_API void		SetShowXYMinorGrid( bool show );
	PLOTDLL_API void		SetShowYZMinorGrid( bool show );
	PLOTDLL_API void		SetShowZXMinorGrid( bool show );
	PLOTDLL_API void		SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX);

	// �ڷֲ���ͼ�ͷ�����ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	PLOTDLL_API void		SetLRAxis();
	// �ڷֲ���ͼ�У����õ�һ����ʾ�������ᣬ�ڶ�����ʾ��������
	PLOTDLL_API void		SetTBAxis();
	// �ײ㺯������ȡ��ͼ�Ļ�ͼ����
	PLOTDLL_API RECT		GetPlotRect(int nPlotIndex);
	// �ײ㺯������ȡ��ͼ����
	PLOTDLL_API RECT		GetPlotRect();
	// �ײ㺯������ȡ������ķ�Χ
	PLOTDLL_API void		GetPlotRange(double xRange[2], double yRange[2], int nPlotIndex=0);

	// ���ö�ʱ������
	PLOTDLL_API void		SetTimerFunc(bool (*pFcnTimer)(void *pChart, void *pVoid), void *pPara);
	// ���ö�ʱ����ʱ��������λΪms
	PLOTDLL_API void		SetTimerInterval(int interval);
	// ���ö�ʱ���ľ��ȣ���λΪms��ֻ�Ը߾��ȶ�ʱ��������
	PLOTDLL_API void		SetTimerResolution(int res);
	
	// ������ʱ��
	// �߾��ȶ�ʱ�����ö�ý�嶨ʱ�����;��ȶ�ʱ������WM_TIMER��Ϣ
	PLOTDLL_API void		StartTimer(HWND hWnd, bool bHighRes);
	// ֹͣ��ʱ��
	PLOTDLL_API void		StopTimer(HWND hWnd);
	
	// ��ö�ʱ������ָ��
	PLOTDLL_API bool		(*GetTimerFunc( ))( void *pChart, void *pVoid );
	// ��ö�ʱ�������������
	PLOTDLL_API void		*GetTimerPara();
}

#ifdef PLOTDLL_API
#	undef PLOTDLL_API
#endif //PLOTDLL_API

#endif //__CLASSLESS_H_122333444455555__
