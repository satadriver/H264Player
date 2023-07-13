//bmp2ipl.h
//#include "cvheader.h"
#include "cv.h"
#include "highgui.h"

#ifndef BMP2IPL_H
#define BMP2IPL_H

class BMP
{
public:
	BMP():bmpData(NULL) 
	{
		memset(&biHeader, 0, sizeof(biHeader));
	}

	BMP(const BMP &img);
	BMP(const IplImage &img);
	BMP(int width, int height, int bitCount);

	~BMP()
	{ 
		delete [] bmpData; 
	}
	
	bool CreateImage(const BITMAPINFOHEADER &biHeader);
	
	//Export
	IplImage * BMP2Ipl();
	//void Show(HWND hWnd, int nID);
	//void Show(CDC *pDC,  CRect & rect);
	//void Show(HWND hWnd);
	
	void ReSize(int newW, int newH);
	
private:
	void CopyData(char *dest, const char *src, int dataByteSize,bool isConvert, int height);
	// isConvert=true 進行頂左和底左之間的轉換(頂左到底左或底左到頂左)，否則不轉換
	// biSizeImage may be set to zero for BI_RGB bitmaps, so calculate it.
	int ImgSize() 
	{
		return ( biHeader.biHeight * ((biHeader.biWidth * biHeader.biBitCount / 8 + 3) & (-4)));
	}
	
public:
	void Clear();
	BITMAPINFOHEADER biHeader;
	unsigned char * bmpData;
};
#endif