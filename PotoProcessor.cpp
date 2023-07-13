// PotoProcessor.cpp : implementation file
//

#include "stdafx.h"
#include "h264player.h"
#include "PotoProcessor.h"
#include "H264PlayerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPotoProcessor dialog


CPotoProcessor::CPotoProcessor(CWnd* pParent /*=NULL*/)
: CDialog(CPotoProcessor::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CPotoProcessor)
	m_smooth_para1 = 0;
	m_smooth_para2 = 0;
	m_editThresh = 0;
	m_apertureSize = 0;
	m_pyrSegment1 = 0;
	m_pyrSegment2 = 0;
	m_editBrightness = 0;
	m_editContrast = 0;
	m_editLightness = 0;
	m_editTimes = 0;
	//}}AFX_DATA_INIT
	m_pParentDlg = pParent;
	m_image = NULL;
	m_orImage = NULL;
	m_bIsProcessed = FALSE;
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;
}

CPotoProcessor::~CPotoProcessor()
{
	if(m_image)
		cvReleaseImage( &m_image );
	if(m_orImage)
		cvReleaseImage( &m_orImage );
	m_image = NULL;
	m_orImage = NULL;
}

void CPotoProcessor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPotoProcessor)
	DDX_Control(pDX, IDC_SLIDER_LIGHTNESS, m_sliderLightness);
	DDX_Control(pDX, IDC_SLIDER_EXPEND, m_sliderExpend);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContast);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_sliderBrightness);
	DDX_Control(pDX, IDC_SLIDER_PYRSEGMENT2, m_sliderPyrSegment2);
	DDX_Control(pDX, IDC_SLIDER_PYRSEGMENT1, m_sliderPyrSegment1);
	DDX_Control(pDX, IDC_SLIDER_LAPLACE, m_sliderLaplace);
	DDX_Control(pDX, IDC_SLIDER_PARA2, m_smoothSlider2);
	DDX_Control(pDX, IDC_SLIDER_PARA1, m_smoothSlider1);
	DDX_Control(pDX, IDC_SLIDER_CANNY, m_sliderCanny);
	DDX_Control(pDX, IDC_POTO_SHOW, m_potoShow);
	DDX_Text(pDX, IDC_EDIT_PARA1, m_smooth_para1);
	DDX_Text(pDX, IDC_EDIT_PARA2, m_smooth_para2);
	DDX_Text(pDX, IDC_EDIT_THRESH, m_editThresh);
	DDX_Text(pDX, IDC_EDIT_APERTUR_SIZE, m_apertureSize);
	DDX_Text(pDX, IDC_EDIT_PYRSEGMENT1, m_pyrSegment1);
	DDX_Text(pDX, IDC_EDIT_PYRSEGMENT2, m_pyrSegment2);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_editBrightness);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_editContrast);
	DDX_Text(pDX, IDC_EDIT_LIGHTNESS, m_editLightness);
	DDX_Text(pDX, IDC_EDIT_TIMES, m_editTimes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPotoProcessor, CDialog)
//{{AFX_MSG_MAP(CPotoProcessor)
ON_WM_SIZE()
ON_WM_PAINT()
ON_COMMAND(IDR_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_RADIO_BLUR, OnRadioBlur)
	ON_BN_CLICKED(IDC_RADIO_GAUSSIAN, OnRadioGaussian)
	ON_BN_CLICKED(IDC_RADIO_MEDIAN, OnRadioMedian)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PARA1, OnCustomdrawSliderPara1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PARA2, OnCustomdrawSliderPara2)
	ON_BN_CLICKED(IDC_RADIO_CANNY, OnRadioCanny)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CANNY, OnCustomdrawSliderCanny)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LAPLACE, OnCustomdrawSliderLaplace)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PYRSEGMENT1, OnCustomdrawSliderPyrsegment1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PYRSEGMENT2, OnCustomdrawSliderPyrsegment2)
	ON_BN_CLICKED(IDC_RADIO_PYRSEGMENT, OnRadioPyrsegment)
	ON_BN_CLICKED(IDC_BTN_STHENGTHEN, OnBtnSthengthen)
	ON_BN_CLICKED(IDC_BTN_RECOVERORIMAGE, OnBtnRecoverorimage)
	ON_BN_CLICKED(IDC_BTN_THRSHOLD, OnBtnThrshold)
	ON_BN_CLICKED(IDC_BTN_FACEDETECTION, OnBtnFacedetection)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BRIGHTNESS, OnCustomdrawSliderBrightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CONTRAST, OnCustomdrawSliderContrast)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LIGHTNESS, OnCustomdrawSliderLightness)
	ON_BN_CLICKED(IDC_RADIO_ERODE, OnRadioErode)
	ON_BN_CLICKED(IDC_RADIO_EXPEND, OnRadioExpend)
	ON_BN_CLICKED(IDC_RADIO_LAPLACE, OnRadioLaplace)
	ON_BN_CLICKED(IDC_RADIO_BRIGHTNESS, OnRadioBrightness)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EXPEND, OnCustomdrawSliderExpend)
	ON_BN_CLICKED(IDC_RADIO_CONTRAST, OnRadioContrast)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPotoProcessor message handlers

void CPotoProcessor::OnCancel() 
{
	// TODO: Add extra cleanup here
	CH264PlayerDlg* pParentDlg=(CH264PlayerDlg*)m_pParentDlg;
	pParentDlg->m_bIsOpenPotoProcessor = FALSE;
	DestroyWindow();
	//	CDialog::OnCancel();
}

void CPotoProcessor::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CPotoProcessor::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(IsWindow(m_TBar.GetSafeHwnd()))
		m_TBar.MoveWindow(0,0,cx,30);

	InitUserInterface(cx,cy);
	Invalidate();
}

BOOL CPotoProcessor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	InitUserInterface(rect.Width(),rect.Height());

		/*	
		/////////////////产生工具栏,并调整对话框上的所有的子窗口位置//////////////////////////////////
		if (!m_TBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_TOP) ||!m_TBar.LoadToolBar(IDR_TOOLBAR))
		{
		TRACE0("Failed to create dialog toolbar\n");
		EndDialog( IDCANCEL );
		}//产生工具栏
		if (!m_controlBar.Create(this,IDD_DIALOGBAR,CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE,IDD_DIALOGBAR))
		{
		TRACE0("Failed to create dialog toolbar\n");
		EndDialog( IDCANCEL );
		}//产生工具栏
		
		  CRect rcClientStart;
		  CRect rcClientNow;
		  GetClientRect(rcClientStart);
		  //取得对话框客户区的矩形区域大小存于rcClientStart当中
		  RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);//上面创建了工具栏所以调整一次工具栏的默认高度就是30(经测量得出的结论),前面已经创建了一个工具栏,并通过RepositionBars进行了调整所以客户区rcClientNow矮了30
		  CPoint ptOffset(rcClientNow.left-rcClientStart.left, rcClientNow.top-rcClientStart.top); 
		  CRect rcChild;      //用来保存子窗口的矩形区域大小                          
		  CWnd* pwndChild = GetWindow(GW_CHILD);//取得CWnd第一个子窗口的指针
		  while (pwndChild)
		  { //循环将所有的子窗口重新调整其位置                             
		  pwndChild->GetWindowRect(rcChild);//取得句柄所对应子窗口的矩形坐标(屏幕坐标)存入rcChild
		  ScreenToClient(rcChild);//将屏幕坐标转化为窗客户坐标
		  rcChild.OffsetRect(ptOffset);//将矩形rcChild按照ptOffset进行偏移计算
		  pwndChild->MoveWindow(rcChild, FALSE);//将子窗口移动到rcChild处
		  pwndChild = pwndChild->GetNextWindow();//取得下一个子窗口的指针
		  }
		  ///////////////下面将对话框大小重整////////////////////////////////////////////////////////
		  CRect rcWindow;
		  GetWindowRect(rcWindow);//取得窗口的屏幕矩形坐标,包含标题栏与边框在内
		  rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
		  rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
		  MoveWindow(rcWindow, FALSE);
		  RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//将控件重排一次,改动了对话框的大小,再调整一次控件的位置,否则控件就不见啦
		  CenterWindow();
	*/
	
	pDCPotoShow = m_potoShow.GetDC();
	m_potoShow.GetClientRect(&m_rect);
	
	m_sliderCanny.SetRange(0,200);//设置数值范围
    //m_threshCanny=0;//设置当前值
    //m_sliderCanny.SetPos(m_editThresh);

	m_smoothSlider1.SetRange(1, 100);         //滚动条的滑动范围是1到100
    m_smoothSlider2.SetRange(1, 100);         //滚动条的滑动范围是1到100
    m_smooth_para1 = 1;                            //初始化第一个参数值为1
    m_smooth_para2 = 1;                            //初始化第二个参数值为1
    smoothtype = CV_BLUR;           //设定默认采用简单平滑方式对图像进行处理

	m_sliderLaplace.SetRange(1,7);
	m_sliderLaplace.SetTicFreq(10);

    m_sliderPyrSegment1.SetRange(1,255);
	m_sliderPyrSegment2.SetRange(1,255);
	m_sliderPyrSegment1.SetPos(255);
	m_sliderPyrSegment2.SetPos(30);

	m_sliderBrightness.SetRange(0,510);
	m_sliderBrightness.SetPos(255);
	m_sliderContast.SetRange(0,510);
	m_sliderContast.SetPos(255);
	m_sliderLightness.SetRange(0,510);
	m_sliderLightness.SetPos(255);

	m_pyrSegment1=28;
	m_pyrSegment2=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPotoProcessor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush *pOldBrush,newBrush;
	m_potoShow.GetClientRect(&m_rect);
	
	newBrush.CreateSolidBrush(RGB(128,128,128));
	pOldBrush = (CBrush*)pDCPotoShow->SelectObject(&newBrush);
	pDCPotoShow->FillRect(&m_rect,&newBrush);
	(CBrush*)pDCPotoShow->SelectObject(pOldBrush);
	
	if(m_image)
		DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
	CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}

void CPotoProcessor::InitUserInterface(int width,int height)
{
	if(IsWindow(m_potoShow.GetSafeHwnd()))    //显示控件（静态图片控件）
		m_potoShow.MoveWindow(10,0,width-250,height-10);

	if(IsWindow(GetDlgItem(IDC_STATIC_COMBOBOX)->GetSafeHwnd()))    //控件的组合框（既包围控件的最大的框）
		GetDlgItem(IDC_STATIC_COMBOBOX)->MoveWindow(width-230,0,220,height-10);

	if(IsWindow(GetDlgItem(IDC_BTN_OPEN)->GetSafeHwnd()))    //打开的按钮
		GetDlgItem(IDC_BTN_OPEN)->MoveWindow(width-220,12,60,35);

	if(IsWindow(GetDlgItem(IDC_BTN_SAVE)->GetSafeHwnd()))    //打开的按钮
		GetDlgItem(IDC_BTN_SAVE)->MoveWindow(width-220,57,60,35);

	if(IsWindow(GetDlgItem(IDC_BTN_RECOVERORIMAGE)->GetSafeHwnd()))    //恢复原图的按钮
		GetDlgItem(IDC_BTN_RECOVERORIMAGE)->MoveWindow(width-150,12,60,35);

	if(IsWindow(GetDlgItem(IDC_BTN_THRSHOLD)->GetSafeHwnd()))    //二值输出的按钮
		GetDlgItem(IDC_BTN_THRSHOLD)->MoveWindow(width-80,12,60,35);

	if(IsWindow(GetDlgItem(IDC_BTN_STHENGTHEN)->GetSafeHwnd()))    //图像增强的按钮
		GetDlgItem(IDC_BTN_STHENGTHEN)->MoveWindow(width-150,57,60,35);

	if(IsWindow(GetDlgItem(IDC_BTN_FACEDETECTION)->GetSafeHwnd()))    //人脸检测的按钮
		GetDlgItem(IDC_BTN_FACEDETECTION)->MoveWindow(width-80,57,60,35);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE1)->GetSafeHwnd()))    //分割线1
		GetDlgItem(IDC_STATIC_LINE1)->MoveWindow(width-230,97,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_BRIGHTNESS)->GetSafeHwnd()))    //亮度单选按钮
		GetDlgItem(IDC_RADIO_BRIGHTNESS)->MoveWindow(width-220,104,45,20);

	if(IsWindow(GetDlgItem(IDC_RADIO_CONTRAST)->GetSafeHwnd()))    //对比单选按钮
		GetDlgItem(IDC_RADIO_CONTRAST)->MoveWindow(width-220,129,45,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LIGHTNESS)->GetSafeHwnd()))    //明暗标签
		GetDlgItem(IDC_STATIC_LIGHTNESS)->MoveWindow(width-202,154,30,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_BRIGHTNESS)->GetSafeHwnd()))    //亮度滑块
		GetDlgItem(IDC_SLIDER_BRIGHTNESS)->MoveWindow(width-170,104,110,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_CONTRAST)->GetSafeHwnd()))    //对比滑块
		GetDlgItem(IDC_SLIDER_CONTRAST)->MoveWindow(width-170,129,110,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_LIGHTNESS)->GetSafeHwnd()))    //明暗滑块
		GetDlgItem(IDC_SLIDER_LIGHTNESS)->MoveWindow(width-170,154,110,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_BRIGHTNESS)->GetSafeHwnd()))    //亮度参数
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->MoveWindow(width-55,104,35,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_CONTRAST)->GetSafeHwnd()))    //对比参数
		GetDlgItem(IDC_EDIT_CONTRAST)->MoveWindow(width-55,129,35,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_LIGHTNESS)->GetSafeHwnd()))    //明暗参数
		GetDlgItem(IDC_EDIT_LIGHTNESS)->MoveWindow(width-55,154,35,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE2)->GetSafeHwnd()))    //分割线2
		GetDlgItem(IDC_STATIC_LINE2)->MoveWindow(width-230,179,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_CANNY)->GetSafeHwnd()))    //显示Canny边缘检测单选按钮
		GetDlgItem(IDC_RADIO_CANNY)->MoveWindow(width-220,184,100,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_CANNY)->GetSafeHwnd()))    //显示Canny边缘检测滑块控件
		GetDlgItem(IDC_SLIDER_CANNY)->MoveWindow(width-220,204,115,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_THRESH)->GetSafeHwnd()))    //Canny边缘检测参数标签
		GetDlgItem(IDC_STATIC_THRESH)->MoveWindow(width-102,204,37,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_THRESH)->GetSafeHwnd()))    //显示Canny边缘检测参数
		GetDlgItem(IDC_EDIT_THRESH)->MoveWindow(width-65,204,45,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE3)->GetSafeHwnd()))    //分割线3
		GetDlgItem(IDC_STATIC_LINE3)->MoveWindow(width-230,229,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_EXPEND)->GetSafeHwnd()))    //膨胀按钮
		GetDlgItem(IDC_RADIO_EXPEND)->MoveWindow(width-220,234,60,20);

	if(IsWindow(GetDlgItem(IDC_RADIO_ERODE)->GetSafeHwnd()))    //形态学腐蚀按钮
		GetDlgItem(IDC_RADIO_ERODE)->MoveWindow(width-120,234,100,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_EXPEND)->GetSafeHwnd()))    //膨胀、腐蚀滑块
		GetDlgItem(IDC_SLIDER_EXPEND)->MoveWindow(width-220,254,115,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_TIMES)->GetSafeHwnd()))    //膨胀、腐蚀参数标签
		GetDlgItem(IDC_STATIC_TIMES)->MoveWindow(width-102,254,30,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_TIMES)->GetSafeHwnd()))    //膨胀、腐蚀参数
		GetDlgItem(IDC_EDIT_TIMES)->MoveWindow(width-65,254,45,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE4)->GetSafeHwnd()))    //分割线4
		GetDlgItem(IDC_STATIC_LINE4)->MoveWindow(width-230,284,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_LAPLACE)->GetSafeHwnd()))    //拉普拉斯锐化单选框
		GetDlgItem(IDC_RADIO_LAPLACE)->MoveWindow(width-220,289,100,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_LAPLACE)->GetSafeHwnd()))    //拉普拉斯锐化滑块
		GetDlgItem(IDC_SLIDER_LAPLACE)->MoveWindow(width-220,309,115,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_APERTUR_SIZE)->GetSafeHwnd()))    //拉普拉斯锐化参数
		GetDlgItem(IDC_EDIT_APERTUR_SIZE)->MoveWindow(width-65,309,45,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE5)->GetSafeHwnd()))    //分割线5
		GetDlgItem(IDC_STATIC_LINE5)->MoveWindow(width-230,334,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_PYRSEGMENT)->GetSafeHwnd()))    //图像分割单选按钮
		GetDlgItem(IDC_RADIO_PYRSEGMENT)->MoveWindow(width-220,339,100,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_PYRSEGMENT1)->GetSafeHwnd()))    //图像分割滑块1
		GetDlgItem(IDC_SLIDER_PYRSEGMENT1)->MoveWindow(width-220,359,115,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_PYRSEGMENT2)->GetSafeHwnd()))    //图像分割滑块2
		GetDlgItem(IDC_SLIDER_PYRSEGMENT2)->MoveWindow(width-220,379,115,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_PYRSEGMENT1)->GetSafeHwnd()))    //图像分割参数1标签
		GetDlgItem(IDC_STATIC_PYRSEGMENT1)->MoveWindow(width-102,359,37,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_PYRSEGMENT2)->GetSafeHwnd()))    //图像分割参数2标签
		GetDlgItem(IDC_STATIC_PYRSEGMENT2)->MoveWindow(width-102,379,37,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_PYRSEGMENT1)->GetSafeHwnd()))    //图像分割参数1
		GetDlgItem(IDC_EDIT_PYRSEGMENT1)->MoveWindow(width-65,354,45,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_PYRSEGMENT2)->GetSafeHwnd()))    //图像分割参数2
		GetDlgItem(IDC_EDIT_PYRSEGMENT2)->MoveWindow(width-65,379,45,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_LINE6)->GetSafeHwnd()))    //分割线6
		GetDlgItem(IDC_STATIC_LINE6)->MoveWindow(width-230,404,220,2);

	if(IsWindow(GetDlgItem(IDC_RADIO_BLUR)->GetSafeHwnd()))    //简单模糊单选按钮
		GetDlgItem(IDC_RADIO_BLUR)->MoveWindow(width-220,409,70,20);

	if(IsWindow(GetDlgItem(IDC_RADIO_GAUSSIAN)->GetSafeHwnd()))    //高斯模糊单选按钮
		GetDlgItem(IDC_RADIO_GAUSSIAN)->MoveWindow(width-220,439,70,20);

	if(IsWindow(GetDlgItem(IDC_RADIO_MEDIAN)->GetSafeHwnd()))    //中值滤波单选按钮
		GetDlgItem(IDC_RADIO_MEDIAN)->MoveWindow(width-220,469,70,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_PARA1)->GetSafeHwnd()))    //模糊滑块1
		GetDlgItem(IDC_SLIDER_PARA1)->MoveWindow(width-150,409,115,20);

	if(IsWindow(GetDlgItem(IDC_STATIC_PARA1)->GetSafeHwnd()))    //模糊参数1标签
		GetDlgItem(IDC_STATIC_PARA1)->MoveWindow(width-102,429,37,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_PARA1)->GetSafeHwnd()))    //模糊参数1
		GetDlgItem(IDC_EDIT_PARA1)->MoveWindow(width-65,429,45,20);

	if(IsWindow(GetDlgItem(IDC_SLIDER_PARA2)->GetSafeHwnd()))    //模糊滑块2
		GetDlgItem(IDC_SLIDER_PARA2)->MoveWindow(width-150,449,115,20);
	
	if(IsWindow(GetDlgItem(IDC_STATIC_PARA2)->GetSafeHwnd()))    //模糊参数2标签
		GetDlgItem(IDC_STATIC_PARA2)->MoveWindow(width-102,469,37,20);

	if(IsWindow(GetDlgItem(IDC_EDIT_PARA2)->GetSafeHwnd()))    //模糊参数2
		GetDlgItem(IDC_EDIT_PARA2)->MoveWindow(width-65,469,45,20);

}


void CPotoProcessor::LoadImage(LPCTSTR lpszPathName)
{
	//m_image.Load(lpszPathName);
}

/** 从IPLIMAGE对象中生成BMP的图像文件流
*/
bool CPotoProcessor::GetBmpFromIpl(IplImage* pIpl, BYTE* &pBmp, DWORD& size)
{
    BITMAPFILEHEADER bfh = {0};
    DWORD dwImageSize = 0;
    DWORD dwBytesRead = 0;
    
    int w = pIpl->width;
    int l = ((w * 24 +31) & ~31) /8;
    int h = pIpl->height;
    dwImageSize    = l * h;
    
    bfh.bfType        = (WORD)'M' << 8 | 'B';            //定义文件类
    bfh.bfOffBits    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);    //定义文件头大小
    bfh.bfSize        = bfh.bfOffBits + dwImageSize;        //文件大小
    
    BITMAPINFOHEADER bih = {0};
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = pIpl->width;
    bih.biHeight = pIpl->height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = BI_RGB;
    bih.biSizeImage    = 0;
    bih.biXPelsPerMeter    = 0;
    bih.biYPelsPerMeter    = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;
    
    size = bfh.bfSize;
    pBmp = new BYTE[bfh.bfSize+1];
    
    memset(pBmp, 0, bfh.bfSize + 1);
    memcpy(pBmp, &bfh, sizeof(BITMAPFILEHEADER));
    memcpy(pBmp+sizeof(BITMAPFILEHEADER), &bih, sizeof(BITMAPINFOHEADER));
    BYTE* p = pBmp+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    memcpy(p, (BYTE*)pIpl->imageData, dwImageSize);
	
    return true;
}

/** 从BMP图像文件数据流中生成IPLIMAGE对象
*/

IplImage* CPotoProcessor::GetIplFromBmp(BYTE* pBmp, DWORD size)
{
    BYTE* p = pBmp;
    BITMAPFILEHEADER fheader;
    memcpy(&fheader, p, sizeof(BITMAPFILEHEADER));
    BITMAPINFOHEADER bmphdr;
    p += sizeof(BITMAPFILEHEADER);
    memcpy(&bmphdr, p, sizeof(BITMAPINFOHEADER));
    int w = bmphdr.biWidth;
    int h = bmphdr.biHeight;
    
    p = pBmp + fheader.bfOffBits;
    
    //reverse(p, w, h);
    IplImage* pIpl = cvCreateImage(cvSize(w,h),8,3);
    memcpy(pIpl->imageData, p, w*h*3*sizeof(char));
    return pIpl;
}

void CPotoProcessor::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString fileName;	
	CFileDialog OpenDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"(*.bmp) |*.bmp| (*.jpg)|*.jpg|",
		NULL);
	if(OpenDlg.DoModal()!=IDOK)
		return;
	//获得文件名
	fileName = OpenDlg.GetPathName();
	if(m_image)
		cvReleaseImage( &m_image );
	if(m_orImage)
		cvReleaseImage( &m_orImage );
	//打开文件，若失败则返回
	if( (m_image = cvLoadImage(fileName,4)) == 0 )
	{
		MessageBox("faile!");
		return;
	}
	//DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
	m_orImage = cvCloneImage(m_image);
	Invalidate();
}

void CPotoProcessor::DrawPicToHDC(IplImage *img, UINT ID,CRect rcScreen) 
{ 
	CRect rect;
	rect=OnResetRect(&rcScreen, img->width, img->height);
	
	CDC *pDC = GetDlgItem(ID)->GetDC(); 
	HDC hDC= pDC->GetSafeHdc();  
	CWnd * pWnd; 
	pWnd=GetDlgItem(ID); 
	//pWnd->SetWindowPos( NULL,0,0,img->width,img->height,SWP_NOZORDER | SWP_NOMOVE ); 
	
	CvvImage cimg; 
	cimg.CopyOf(img,1); 
	cimg.DrawToHDC(hDC,&rect); 
	ReleaseDC(pDC);
} 

//当视频图像尺寸小于显示屏幕时，使视频图像按原尺寸比例居中显示
CRect CPotoProcessor::OnResetRect(CRect *rcScreen, DWORD picWidth, DWORD picHeight)
{
	CRect rect(rcScreen);
	double dWidth = rcScreen->Width();
	double dHeight = rcScreen->Height();
	double dAspectRatio = dWidth/dHeight;
	
	double dPictureWidth = picWidth;
	double dPictureHeight = picHeight;
	double dPictureAspectRatio = dPictureWidth/dPictureHeight;
	
	//If the aspect ratios are the same then the screen rectangle
	// will do, otherwise we need to calculate the new rectangle
	
	if((picWidth<dWidth && picHeight<dHeight)/*||(picWidth>dWidth && picHeight>dHeight)*/)
	{
		rect.SetRect( (dWidth-picWidth)/2,
			(dHeight-picHeight)/2,
			dPictureWidth+(dWidth-picWidth)/2,
			dPictureHeight+(dHeight-picHeight)/2);
		return rect;
	}
	
	if (dPictureAspectRatio > dAspectRatio)
	{
		int nNewHeight = (int)(dWidth/dPictureWidth*dPictureHeight);
		//让图像居中
		int nCenteringFactor= (rcScreen->Height() - nNewHeight) / 2;
		rect.SetRect( 0,nCenteringFactor,(int)dWidth,nNewHeight + nCenteringFactor);
	}
	else if (dPictureAspectRatio < dAspectRatio)
	{
		int nNewWidth =  (int)(dHeight/dPictureHeight*dPictureWidth);
		//让图像居中
		int nCenteringFactor= (rcScreen->Width() - nNewWidth) / 2;
		rect.SetRect( nCenteringFactor, 0,nNewWidth + nCenteringFactor,(int)(dHeight));
	}
	return rect;
}

void CPotoProcessor::OnRadioCanny() 
{
	// TODO: Add your control notification handler code here
	//m_sliderCanny.SetPos(0);
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=TRUE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;

	m_smoothSlider2.EnableWindow(TRUE);
}
/***************************************************************************
* 单击RadioButton时，相应改变平滑类型
* 当选择中值滤波时，第二个参数无效，因此将第二个滚动条也置为无效
***************************************************************************/
void CPotoProcessor::OnRadioBlur() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=TRUE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;

	smoothtype = CV_BLUR ;
}

void CPotoProcessor::OnRadioGaussian() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=TRUE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;

	smoothtype = CV_GAUSSIAN ;
    m_smoothSlider2.EnableWindow(TRUE);
}

void CPotoProcessor::OnRadioMedian() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=TRUE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;

	smoothtype = CV_MEDIAN ;
    m_smoothSlider2.EnableWindow(FALSE);   //当选择中值滤波时，第二个参数无效，因此将第二个滚动条也置为无效
}

void CPotoProcessor::OnRadioPyrsegment() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=TRUE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;
}

void CPotoProcessor::OnRadioExpend() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=TRUE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;
}

void CPotoProcessor::OnRadioLaplace() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=TRUE;
	m_bBright = FALSE;
	m_bContrast = FALSE;
}

void CPotoProcessor::OnRadioBrightness() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = TRUE;
	m_bContrast = FALSE;

	m_sliderLightness.EnableWindow(FALSE);
}

void CPotoProcessor::OnRadioErode() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=TRUE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = FALSE;
}

void CPotoProcessor::OnRadioContrast() 
{
	// TODO: Add your control notification handler code here
	m_bThrshold=FALSE;
	m_bSthengthen=FALSE;
	m_bFacedDetection=FALSE;
	m_bSmooth=FALSE;
	m_bCanny=FALSE;
	m_bExpend=FALSE;
	m_bErod=FALSE;
	m_bPrySegment=FALSE;
	m_bBlur=FALSE;
	m_bGaussian=FALSE;
	m_bMedian=FALSE;
	m_bLaplace=FALSE;
	m_bBright = FALSE;
	m_bContrast = TRUE;

	m_sliderLightness.EnableWindow(TRUE);
}


void CPotoProcessor::OnCustomdrawSliderPara1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if(m_bSmooth)
	{
		if(m_image)
		{

			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);

			IplImage* dst;
			m_smooth_para1 = m_smoothSlider1.GetPos();
			UpdateData(FALSE);
			
			dst=cvCloneImage(m_image);
			int param1 = m_smooth_para1*2+1;
			int param2 = m_smooth_para2*2-1;
			cvSmooth( m_image, dst, smoothtype, param1, param2);
			cvReleaseImage( &m_image );
			//m_image=cvCreateImage(cvGetSize(dst),8,1);
			m_image=cvCloneImage(dst);
			cvReleaseImage( &dst );
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderPara2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if(m_bSmooth)
	{
		if(m_image)
		{
			//cvReleaseImage( &m_image );
			//m_image = cvCloneImage(m_orImage);

			IplImage* dst;
			m_smooth_para2= m_smoothSlider2.GetPos();
			UpdateData(FALSE);
			
			dst=cvCloneImage(m_image);
			int param1 = m_smooth_para1*2+1;
			int param2 = m_smooth_para2*2-1;
			cvSmooth( m_image, dst, smoothtype, param1, param2);
			cvReleaseImage( &m_image );
			//m_image=cvCreateImage(cvGetSize(dst),8,1);
			m_image=cvCloneImage(dst);
			cvReleaseImage( &dst );
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}


void CPotoProcessor::OnCustomdrawSliderCanny(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bCanny)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);
			m_editThresh=m_sliderCanny.GetPos();  //获得滑块的值 
			UpdateData(FALSE);
			cvProcessor.CVCanny(m_image,m_editThresh);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
			//Invalidate();
		}
		/*
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);
			cvProcessor.CVResizeImage(m_image,m_threshCanny/2);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
			//Invalidate();
		}*/
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderLaplace(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	if(m_bLaplace)
	{
		m_apertureSize = m_sliderLaplace.GetPos();

		if(m_apertureSize == 2||m_apertureSize == 4||m_apertureSize == 6)
		m_sliderLaplace.SetPos(m_apertureSize++);
		UpdateData(FALSE);

		if(m_image)
		{
			cvReleaseImage( &m_image );
			cvProcessor.CVLaplace(m_orImage,m_image,m_apertureSize);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderPyrsegment1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bPrySegment)
	{
		m_pyrSegment1 = m_sliderPyrSegment1.GetPos();
		UpdateData(FALSE);
		if(m_image)
		{
			//cvReleaseImage( &m_image );
			cvProcessor.CVPyrSegmentation(m_orImage,m_image,m_pyrSegment1,m_pyrSegment2);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderPyrsegment2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bPrySegment)
	{
		m_pyrSegment2 = m_sliderPyrSegment2.GetPos();
		UpdateData(FALSE);
		if(m_image)
		{
			//cvReleaseImage( &m_image );
			cvProcessor.CVPyrSegmentation(m_orImage,m_image,m_pyrSegment1,m_pyrSegment2);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnBtnSthengthen() //图像增强
{
	// TODO: Add your control notification handler code here
	if(m_image)
	{
		cvReleaseImage( &m_image );
		cvProcessor.CVImageStrengthen(m_orImage,m_image);
		DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
	}
}

void CPotoProcessor::OnBtnRecoverorimage() //恢复原图
{
	// TODO: Add your control notification handler code here
	if(m_image)
	{
		cvReleaseImage( &m_image );
		m_image = cvCloneImage(m_orImage);
		DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
	}
}

void CPotoProcessor::OnBtnThrshold() 
{
	// TODO: Add your control notification handler code here
	if(m_image)
	{
		IplImage* pGrayImg;
		cvReleaseImage( &m_image );
		//转化为灰度图
		pGrayImg=cvCreateImage(cvGetSize(m_orImage),IPL_DEPTH_8U,1);
		cvCvtColor(m_orImage,pGrayImg,CV_RGB2GRAY);

		m_image=cvCloneImage(pGrayImg);
		cvThreshold(pGrayImg,m_image, 100, 255.0, CV_THRESH_BINARY); //100为阈值，可以调整
		DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		cvReleaseImage(&pGrayImg);
	}
	
}

void CPotoProcessor::OnBtnFacedetection() 
{
	// TODO: Add your control notification handler code here
	cvProcessor.CVFaceDetection(m_image, m_orImage);
	DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
}

void CPotoProcessor::OnCustomdrawSliderBrightness(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bBright)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);

			m_editBrightness = m_sliderBrightness.GetPos()-255;
			UpdateData(FALSE);
			cvProcessor.ImgLighteness(m_orImage,m_image , cvScalarAll( m_editBrightness ), 0);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderContrast(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bContrast)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);

			m_editContrast = m_sliderContast.GetPos()-255;
			UpdateData(FALSE);
			cvProcessor.ImgContrast(m_orImage, m_image, m_editContrast, m_editLightness);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}

void CPotoProcessor::OnCustomdrawSliderLightness(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bContrast)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);

			m_editLightness = m_sliderLightness.GetPos()-255;
			UpdateData(FALSE);
			cvProcessor.ImgContrast(m_orImage, m_image, m_editContrast, m_editLightness);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}


void CPotoProcessor::OnCustomdrawSliderExpend(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bExpend)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);
			m_editTimes = m_sliderExpend.GetPos();
			UpdateData(FALSE);
			cvProcessor.Expend(m_orImage,m_image,m_editTimes);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	else if(m_bErod)
	{
		if(m_image)
		{
			cvReleaseImage( &m_image );
			m_image = cvCloneImage(m_orImage);
			m_editTimes = m_sliderExpend.GetPos();
			UpdateData(FALSE);
			cvProcessor.Erode(m_orImage,m_image,m_editTimes);
			DrawPicToHDC(m_image,IDC_POTO_SHOW,m_rect);
		}
	}
	*pResult = 0;
}


void CPotoProcessor::OnBtnOpen() 
{
	// TODO: Add your control notification handler code here
	OnFileOpen();
}

void CPotoProcessor::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	CString fileName;
	CString fileExt;
	CFileDialog OpenDlg( FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		"(*.bmp) |*.bmp|(*.jpg)|*.jpg|(*.png)|*.png|",
		NULL);
	if(OpenDlg.DoModal()!=IDOK)
		return;
	//获得文件名
	fileName = OpenDlg.GetPathName();
	//fileName = OpenDlg.GetFileName();
	if(m_image)
	{
		cvSaveImage(fileName, m_image);//把图像写入文件
	}
}
