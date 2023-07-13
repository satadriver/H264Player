#pragma once
#include "cvheader.h"
//#include "cv.h"
//#include "highgui.h"

class CPicture
{
public:
	CPicture(void);
	~CPicture(void);
	void CreateBmpInfo(BITMAPFILEHEADER &bmpheader,BITMAPINFO &bmpinfo,int flag);
	int  CreateBmp(char* filename);
	void ShowBmpImage(CDC* pDC,CRect rect,bool originalSize);  //originalSize为是否按原尺寸显示
	void ReleaseImage();

	bool GetBmpFromIpl(IplImage* pIpl, BYTE* &pBmp, DWORD& size);
	IplImage* GetIplFromBmp(BYTE* pBmp/*, DWORD size*/);
	void CreateIplImage();
	IplImage * BmpTOIplimage(int width,int height,unsigned char *pBuffer);

public:
	unsigned char* m_pBmpData;
	unsigned char* m_pRGBBuffer;

	IplImage* m_image;
    DWORD m_width;
	DWORD m_height;

	int m_bpp;
};
