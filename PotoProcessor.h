#if !defined(AFX_POTOPROCESSOR_H__73AA95B8_A3F8_4562_BC26_33740DDDEBB0__INCLUDED_)
#define AFX_POTOPROCESSOR_H__73AA95B8_A3F8_4562_BC26_33740DDDEBB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PotoProcessor.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CPotoProcessor dialog
#include "cvheader.h"
//#include "cv.h"
//#include "highgui.h"

#include "ImageShow.h"
#include "OpenCVPotoProcessor.h"

class CPotoProcessor : public CDialog
{
// Construction
public:
	BOOL m_bThrshold;
	BOOL m_bSthengthen;
	BOOL m_bFacedDetection;
	BOOL m_bSmooth;
	BOOL m_bCanny;
	BOOL m_bLaplace;
	BOOL m_bExpend;
	BOOL m_bErod;
	BOOL m_bPrySegment;
	BOOL m_bBlur;
	BOOL m_bGaussian;
	BOOL m_bMedian;
	BOOL m_bBright;
	BOOL m_bContrast;

	int smoothtype;
	BOOL m_bIsProcessed;   //图片是否已经经过处理
	CDialogBar m_controlBar;
	CToolBar m_TBar;
	CPotoProcessor(CWnd* pParent = NULL);   // standard constructor
	~CPotoProcessor();
    CWnd* m_pParentDlg;
	CDC* pDCPotoShow;

	IplImage* m_image;     //用于处理和显示
	IplImage* m_orImage;   //原图
	DWORD m_width;
	DWORD m_height;
	
	CRect m_rect;

	COpenCVPotoProcessor cvProcessor;

	void InitUserInterface(int width,int height);
	void DrawPicToHDC(IplImage *img, UINT ID, CRect rect);
	CRect OnResetRect(CRect *rcScreen, DWORD picWidth, DWORD picHeight);
	bool GetBmpFromIpl(IplImage* pIpl, BYTE* &pBmp, DWORD& size); //从IPLIMAGE对象中生成BMP的图像文件流

	IplImage* GetIplFromBmp(BYTE* pBmp, DWORD size); //从BMP图像文件数据流中生成IPLIMAGE对象
	void LoadImage(LPCTSTR lpszPathName);
// Dialog Data
	//{{AFX_DATA(CPotoProcessor)
	enum { IDD = IDD_POTOPROCESSOR };
	CSliderCtrl	m_sliderLightness;
	CSliderCtrl	m_sliderExpend;
	CSliderCtrl	m_sliderContast;
	CSliderCtrl	m_sliderBrightness;
	CSliderCtrl	m_sliderPyrSegment2;
	CSliderCtrl	m_sliderPyrSegment1;
	CSliderCtrl	m_sliderLaplace;
	CSliderCtrl	m_smoothSlider2;
	CSliderCtrl	m_smoothSlider1;
	CSliderCtrl	m_sliderCanny;
	CImageShow	m_potoShow;
	int		m_smooth_para1;
	int		m_smooth_para2;
	int		m_editThresh;
	int		m_apertureSize;  //扩展 laplace 核的大小，必须是 1, 3, 5 或 7
	int		m_pyrSegment1;
	int		m_pyrSegment2;
	int		m_editBrightness;
	int		m_editContrast;
	int		m_editLightness;
	int		m_editTimes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPotoProcessor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPotoProcessor)
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnFileOpen();
	afx_msg void OnRadioBlur();
	afx_msg void OnRadioGaussian();
	afx_msg void OnRadioMedian();
	afx_msg void OnCustomdrawSliderPara1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderPara2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioCanny();
	afx_msg void OnCustomdrawSliderCanny(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderLaplace(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderPyrsegment1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderPyrsegment2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioPyrsegment();
	afx_msg void OnBtnSthengthen();
	afx_msg void OnBtnRecoverorimage();
	afx_msg void OnBtnThrshold();
	afx_msg void OnBtnFacedetection();
	afx_msg void OnCustomdrawSliderBrightness(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderContrast(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderLightness(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioErode();
	afx_msg void OnRadioExpend();
	afx_msg void OnRadioLaplace();
	afx_msg void OnRadioBrightness();
	afx_msg void OnCustomdrawSliderExpend(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioContrast();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POTOPROCESSOR_H__73AA95B8_A3F8_4562_BC26_33740DDDEBB0__INCLUDED_)
