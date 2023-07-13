// H264PlayerDlg.h : header file
//

#if !defined(AFX_H264PLAYERDLG_H__EB529B2A_63F1_4F56_B5DA_3D69C01BC37B__INCLUDED_)
#define AFX_H264PLAYERDLG_H__EB529B2A_63F1_4F56_B5DA_3D69C01BC37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CH264PlayerDlg dialog

#include "Decode.h"
#include "Picture.h"
#include "PotoProcessor.h"
#include "H264PlayerDlg.h"
#include "ImageShow.h"

class CH264PlayerDlg : public CDialog
{
// Construction
public:
	CH264PlayerDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL PreTranslateMessage(MSG* pMsg);    // �ػ������Ϣ

	void FullScreen();                      // ȫ��
	void ExitFullScreen();					// �˳�ȫ��
	static DWORD DecodeThread(void *pDlg);  // �����߳�
	void Display(CDC* pDC);                 // ��ʾ��Ƶͼ��
	CRect OnResetRect(CRect* rcScreen,DWORD bmpWidth, DWORD bmpHeight);  //ʹ��Ƶͼ��ԭ�ߴ����������ʾ
	void ResourceReleaseAndReconvertSet();   // �ͷ���Դ����ԭĳЩ��־������

// Dialog Data
	//{{AFX_DATA(CH264PlayerDlg)
	enum { IDD = IDD_H264PLAYER_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CH264PlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	
	WINDOWPLACEMENT m_OldWndPlacement;
	CRect m_FullScreenRect;
	BOOL m_bFullScreen;

	CButton	m_stop;
	CButton	m_cutPic;
	CButton	m_play;
	CProgressCtrl	m_playProgress;
	//CImageShow	m_show;
	CStatic	m_show;

	CDecode m_decode;  //������
	CPicture m_bmpImage; //��Ƶͼ��
	CDC* pDCShow ;
	CRect rectShow;
	char* m_inFilePath;
	HANDLE hDecodeThread;
	DWORD dwThreadID;
    BOOL m_bDecoding;
	BOOL m_bIsEndThread;
	BOOL m_isOver;
	BOOL m_firstOpen;    //��һ�δ򿪴��ڣ�����OnSize()�п���m_OriginalSize
	BOOL m_OriginalSize; //�Ƿ�ԭ�ߴ������ʾ
	BOOL m_bIsFullScreen; //�Ƿ����ȫ��ģʽ
	BOOL m_bIsSuspend;   //��ͣ

	//ͼ����
	CPotoProcessor* m_potoProcessor;
	BOOL m_bIsOpenPotoProcessor;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CH264PlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPlay();
	afx_msg void OnFileOpen();
	afx_msg void OnStop();
	afx_msg void OnViewFullscrren();
	afx_msg void OnViewOriginalsize();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCutPicture();
	afx_msg void OnAboutbox();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_H264PLAYERDLG_H__EB529B2A_63F1_4F56_B5DA_3D69C01BC37B__INCLUDED_)
