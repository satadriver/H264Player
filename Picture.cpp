#include "StdAfx.h"
#include "Picture.h"


#define BUFLEN	1024*1024*3
CPicture::CPicture(void)
{
	m_pBmpData = NULL;
	m_pRGBBuffer = NULL;
	m_bpp = 24;
}

CPicture::~CPicture(void)
{
}

void CPicture::CreateBmpInfo(BITMAPFILEHEADER &bmpheader,BITMAPINFO &bmpinfo,int flag)//显示图片的时候flag为-1，截图创建图片的时候flag为1，否则opencv不能读取图片
{
	bmpheader.bfType = ('M'<<8)|'B';
    bmpheader.bfReserved1 = 0;
    bmpheader.bfReserved2 = 0;
    bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpheader.bfSize = bmpheader.bfOffBits + m_width*m_height*m_bpp/8;
	
    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth = m_width;
    bmpinfo.bmiHeader.biHeight = m_height*flag;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = m_bpp;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    bmpinfo.bmiHeader.biSizeImage = (m_width*m_bpp+31)/32*4*m_height;
    bmpinfo.bmiHeader.biXPelsPerMeter = 100;
    bmpinfo.bmiHeader.biYPelsPerMeter = 100;
    bmpinfo.bmiHeader.biClrUsed = 0;
    bmpinfo.bmiHeader.biClrImportant = 0;
}

int CPicture::CreateBmp(char *filename)
{
	BITMAPFILEHEADER bmpheader;
    BITMAPINFO bmpinfo;
    FILE *fp;
	
    fp = fopen(filename,"wb");
    if(!fp)return -1;
	
	CreateBmpInfo(bmpheader,bmpinfo,1);
	
    fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);
    fwrite(&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
	//fwrite(&bmpinfo,sizeof(BITMAPINFO),1,fp);
    fwrite(m_pBmpData,m_width*m_height*m_bpp/8,1,fp);  //这里如果把m_pBmpData换成m_pRGBBuffer,所截的图不正确，
    fclose(fp);										   //因为按下截图的按钮时m_pRGBBuffer正在被赋值，所以不能得到完整的一幅正确的图
	
    return 0;	
}

void CPicture::ShowBmpImage(CDC* pDC,CRect rect,bool originalSize)
{
	//////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER bmpheader;
    BITMAPINFO bmpinfo;
	
    CreateBmpInfo(bmpheader,bmpinfo,-1);
	
	memcpy(m_pBmpData,&bmpheader,sizeof(BITMAPFILEHEADER));
	memcpy(m_pBmpData+sizeof(BITMAPFILEHEADER),&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER));
	memcpy(m_pBmpData,m_pRGBBuffer,m_width * m_height * m_bpp/8);
	
	HBITMAP hBmp;
	CDC MemDC;
	
	hBmp = CreateDIBitmap(
		pDC->m_hDC,					// handle to device context
		(BITMAPINFOHEADER*)(&bmpinfo),   //	pointer to bitmap size and format data
		CBM_INIT,	// initialization flag
		m_pBmpData,	// pointer to initialization data
		&bmpinfo,	// pointer to bitmap color-format data
		DIB_RGB_COLORS	// color-data usage
		);
	
	if(hBmp == NULL)
	{
		pDC->DeleteDC();
		return;
	}
	
	MemDC.CreateCompatibleDC(pDC);	
	SelectObject(MemDC.m_hDC,hBmp);
	
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,this->m_width,this->m_height,SRCCOPY);
	pDC->SetStretchBltMode(COLORONCOLOR);
	
	DeleteObject(hBmp);
	MemDC.DeleteDC();
	//////////////////////////////////////////////////////////////////////////
	
}

void CPicture::ReleaseImage()
{
	if(m_pBmpData!=NULL) //free(m_pBmpData);
	delete m_pBmpData;
	m_pBmpData=NULL;
}


/** 从IPLIMAGE对象中生成BMP的图像文件流
*/
bool CPicture::GetBmpFromIpl(IplImage* pIpl, BYTE* &pBmp, DWORD& size)
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
    
    //size = bfh.bfSize;
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

IplImage* CPicture::GetIplFromBmp(BYTE* pBmp/*, DWORD size*/)
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
    
    memcpy(pIpl->imageData, p, w * h * 3 *sizeof(char));
    return pIpl;
}

void CPicture::CreateIplImage()
{
	m_image=BmpTOIplimage(m_width,m_height,m_pBmpData); 
}

IplImage* CPicture::BmpTOIplimage(int width,int height,unsigned char *pBuffer)
{
	IplImage *image =new IplImage();
	image=cvCreateImageHeader(cvSize(width,height),IPL_DEPTH_8U,3);
	cvSetData(image, pBuffer, width*3); //copy数据
	return image;
}
