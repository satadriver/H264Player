// OpenCVPotoProcessor.cpp: implementation of the COpenCVPotoProcessor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "h264player.h"
#include "OpenCVPotoProcessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenCVPotoProcessor::COpenCVPotoProcessor()
{
	//cascade_name ="\opencv\data\haarcascades\haarcascade_frontalface_alt.xml";//分类器的名称
	cascade_name ="haarcascade_frontalface_alt.xml";
}

COpenCVPotoProcessor::~COpenCVPotoProcessor()
{
	
}

int COpenCVPotoProcessor::CVCanny(IplImage* &pSrcImg ,int thresh)
{
	//声明图像IplImage指针
	IplImage* pGrayImg = NULL; 
	IplImage* pCannyImg = NULL;
	IplImage* pCEdge = NULL;
	
	// Create the output image
	pCEdge = cvCreateImage(cvSize(pSrcImg->width,pSrcImg->height), IPL_DEPTH_8U, 3);
	// Convert to grayscale
	pGrayImg=cvCreateImage(cvGetSize(pSrcImg),IPL_DEPTH_8U,1);
	cvCvtColor(pSrcImg,pGrayImg,CV_RGB2GRAY);
	//canny边缘检测
	pCannyImg = cvCreateImage(cvGetSize(pGrayImg),IPL_DEPTH_8U,1);
	cvSmooth( pGrayImg, pCannyImg, CV_BLUR, 3, 3, 0);
	cvNot( pGrayImg, pCannyImg );
	// Run the edge detector on grayscale
	cvCanny(pGrayImg, pCannyImg,(float)thresh,(float)thresh*3, 3);
	
	cvZero( pCEdge );
	// copy edge points
	cvCopy( pSrcImg, pCEdge, pCannyImg );
	
	cvReleaseImage( &pSrcImg );
	pSrcImg=cvCloneImage(pCEdge);       // 彩色输出
	//pSrcImg=cvCloneImage(pCannyImg);  //灰度图输出
	cvSaveImage("dest.bmp",pSrcImg);
	
	cvReleaseImage( &pCEdge ); 
	cvReleaseImage( &pGrayImg ); 
	cvReleaseImage( &pCannyImg ); 
	
	return 1;
}

void COpenCVPotoProcessor::CVContours(IplImage *img)
{
	
}

void COpenCVPotoProcessor::CVResizeImage(IplImage *&src,int scale)
{
    IplImage *dst = 0;			//目标图像指针
    float scal = ((float)scale)/10;		//scale为100则缩放倍数为100/10为10倍
	if (scal == 0)
		scal = 0.1;
    CvSize dst_cvsize;			//目标图像尺寸
	
    dst_cvsize.width = src->width * scal;		//目标图像的宽为源图象宽的scale倍
    dst_cvsize.height = src->height * scal;	//目标图像的高为源图象高的scale倍
	
    dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	//构造目标图象
    cvResize(src, dst, CV_INTER_LINEAR);	//缩放源图像到目标图像
	
	cvReleaseImage( &src );
    src=cvCloneImage(dst);
	
    cvReleaseImage(&dst);	//释放目标图像占用的内存
}


void COpenCVPotoProcessor::CVLaplace(IplImage* orimg,IplImage* &image,int aperture_size) //普拉斯锐化,aperture_size为核值，只能取1、3、5、7
{
	//IplImage *imgA = cvLoadImage( "image_sample3.bmp", -1);
	/*IplImage* pGrayImg=cvCreateImage(cvGetSize(orimg),IPL_DEPTH_8U,1);
	cvCvtColor(orimg,pGrayImg,CV_RGB2GRAY);
	
	  IplImage *edge = cvCreateImage( cvGetSize(pGrayImg), IPL_DEPTH_16S, 1 );
	  image = cvCreateImage( cvGetSize(pGrayImg), IPL_DEPTH_8U, 1 );
	  
		cvLaplace( pGrayImg, edge, aperture_size);
		
		  for ( int i=0;i < (pGrayImg->height*orimg->width);i++ )
		  {
		  image->imageData[i] = (unsigned char)(((short*)edge->imageData)[i]/8+128);
		  }
		  cvSaveImage("orimage.bmp",orimg);
		  cvReleaseImage( &edge );
		  cvReleaseImage( &pGrayImg );
	*/
	image=cvCreateImage(cvGetSize(orimg),IPL_DEPTH_32F,orimg->nChannels);
	cvLaplace( orimg, image, aperture_size);
}

void COpenCVPotoProcessor::CVPyrSegmentation(IplImage* orimg,IplImage* &image,int threshold1,int threshold2)
{
	IplImage *image0 = 0;
	int  level = 4;
	int block_size = 1024;
	CvSeq *comp;
	CvMemStorage *storage;
    storage = cvCreateMemStorage ( block_size );
    orimg->width &= -(1<<level);
    orimg->height &= -(1<<level);
    image0 = cvCloneImage( orimg );
	//image = cvCloneImage( orimg );
    // 对彩色图像进行分割
	cvPyrSegmentation(image0, image, storage, &comp, level, threshold1, threshold2);
	
	cvReleaseMemStorage(&storage );
    cvReleaseImage(&image0);
}

void COpenCVPotoProcessor::CVImageStrengthen(IplImage* orimg,IplImage* &image)
{
	int i;
	IplImage *pImageChannel[4] = { 0, 0, 0, 0 }; 
	image = cvCreateImage(cvGetSize(orimg), orimg->depth, orimg->nChannels);
	if( orimg )
	{
		for( i = 0; i < orimg->nChannels; i++ )
		{
			pImageChannel[i] = cvCreateImage( cvGetSize(orimg), orimg->depth, 1 );
		}
		// 信道分离
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			// 直方图均衡化
			cvEqualizeHist( pImageChannel[i], pImageChannel[i] );
		}
		// 信道组合
		cvMerge( pImageChannel[0], pImageChannel[1], pImageChannel[2],
			pImageChannel[3], image );
		// ……图像显示代码（略）
		// 释放资源
		//cvSaveImage("test.bmp", pImage);//
		for( i = 0; i < orimg->nChannels; i++ )
		{
			if ( pImageChannel[i] )
			{
				cvReleaseImage( &pImageChannel[i] );
				pImageChannel[i] = 0;
			}
		}
	}
}

void COpenCVPotoProcessor::CVFaceDetection(IplImage *&src, IplImage *dest)
{
	//加载（分类器层叠）训练库
	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0 );
	//加载不成功则显示错误讯息，并退出
	if(cascade)
	   {
		storage = cvCreateMemStorage(0);
		//如果图片存在则分析并显示结果，否则退出程序
		if(src) detect_and_draw(src);//调用人脸检与标示事件
		//cvReleaseImage(&src);
		cvReleaseMemStorage( &storage );
	}
	else
	{
		//&nbsp;
		AfxMessageBox("无法加载分类器，请确认后重试!");
	}
	cvReleaseHaarClassifierCascade( &cascade );
}

void COpenCVPotoProcessor::detect_and_draw(IplImage *&img)//人脸检与标示事件
{
	    static CvScalar color[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };//用于设置标示图像中人脸的颜色
	double scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8,1 );
	int i;
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );
	if( cascade )
	{//检测人脸
		CvSeq*  faces=cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0, cvSize(30,30));
		for( i = 0; i < (faces ? faces->total : 0); i++ )
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			cvCircle( img, center, radius, color[i%8], 3, 8, 0 );
		}
	}
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
}

// 图像亮度调节
bool COpenCVPotoProcessor::ImgLighteness(IplImage *src, IplImage *dst, CvScalar value, CvArr *mask)

{
	bool bResult = false;
	if (src && dst)
	{
		if (src->depth == dst->depth &&
			src->nChannels == dst->nChannels &&
			src->width == dst->width &&
			src->height == dst->height)
		{
			double dMinValue = 0;
			double dMaxValue = 0;
			int    nCOI      = cvGetImageCOI(src);
			CvRect rect      = cvGetImageROI(src);
			cvSetImageCOI(dst, nCOI);
			cvSetImageROI(dst, rect);
			// 未设置 COI
			if (nCOI == 0)
			{
				// 图像亮度
				cvAddS(src, value, dst, mask);
				bResult = true;
			}
			// 提供 COI 支持
			else
			{
				IplImage *pGray = cvCreateImage(cvGetSize(src), src->depth, 1);
				if (pGray)
				{
					cvCopy(src, pGray, NULL);
					// 图像亮度
					cvAddS(pGray, cvScalarAll(value.val[nCOI - 1]), pGray, mask);
					cvCopy(pGray, dst, NULL);
					cvReleaseImage(&pGray);
					pGray = NULL;
				}
			}
			bResult = true;
		}
	}
	return bResult;
}

// 图像对比度调节
bool COpenCVPotoProcessor::ImgContrast(IplImage *src, IplImage *dst, double contrast, double lighteness)

{
	bool bResult = false;
	if (src && dst)
	{
		if (src->depth == dst->depth &&
			src->nChannels == dst->nChannels &&
			src->width == dst->width &&
			src->height == dst->height)
		{			
			double dScale    = 1.0 + contrast;			
			double dShift    = 0;			
			int    nCOI      = cvGetImageCOI(src);			
			CvRect rect      = cvGetImageROI(src);			
			cvSetImageCOI(dst, nCOI);			
			cvSetImageROI(dst, rect);			
			switch (src->depth)				
			{				
			case IPL_DEPTH_8U:				
				dShift = 128.0 * (lighteness - contrast);				
				break;				
			case IPL_DEPTH_8S:				
				dShift = 128.0 * lighteness;				
				break;				
			case IPL_DEPTH_16U:				
				dShift = 32768.0 * (lighteness - contrast);				
				break;				
			case IPL_DEPTH_16S:				
				dShift = 32768.0 * lighteness;				
				break;				
			case IPL_DEPTH_32S:				
				dShift = 2147483648.0 * lighteness;				
				break;				
			case IPL_DEPTH_32F:				
			case IPL_DEPTH_64F:				
				dShift = 0.5 * (lighteness - contrast);				
				break;				
			default:;			
			}		
			// 未设置 COI		
			if (nCOI == 0)			
			{
				// 对比度调节				
				cvConvertScale(src, dst, dScale, dShift);				
				bResult = true;				
			}			
			// 提供 COI 支持			
			else				
			{				
				IplImage *pGray = cvCreateImage(cvGetSize(src), src->depth, 1);				
				if (pGray)					
				{					
					cvCopy(src, pGray, NULL);					
					// 对比度调节					
					cvConvertScale(pGray, pGray, dScale, dShift);					
					cvCopy(pGray, dst, NULL);					
					cvReleaseImage(&pGray);					
					pGray = NULL;
					bResult = true;					
				}				
			}			
              }			  
       }	   
       return bResult;	   
}

void COpenCVPotoProcessor::Erode(IplImage* orimg,IplImage* &image,int  times)
{        
	int i;
	IplImage *pImageChannel[4] = { 0, 0, 0, 0 }; 
	image = cvCreateImage(cvGetSize(orimg), orimg->depth, orimg->nChannels);
	if( orimg )
	{
		for( i = 0; i < orimg->nChannels; i++ )
		{
			pImageChannel[i] = cvCreateImage( cvGetSize(orimg), orimg->depth, 1 );
		}
		// 信道分离
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			cvErode( pImageChannel[i], pImageChannel[i], 0, times);
		}
		// 信道组合
		cvMerge( pImageChannel[0], pImageChannel[1], pImageChannel[2],
			pImageChannel[3], image );

		for( i = 0; i < orimg->nChannels; i++ )
		{
			if ( pImageChannel[i] )
			{
				cvReleaseImage( &pImageChannel[i] );
				pImageChannel[i] = 0;
			}
		}
	}
}
	

void COpenCVPotoProcessor::Expend(IplImage* orimg,IplImage* &image,int  times)
{        
	int i;
	IplImage *pImageChannel[4] = { 0, 0, 0, 0 }; 
	image = cvCreateImage(cvGetSize(orimg), orimg->depth, orimg->nChannels);
	if( orimg )
	{
		for( i = 0; i < orimg->nChannels; i++ )
		{
			pImageChannel[i] = cvCreateImage( cvGetSize(orimg), orimg->depth, 1 );
		}
		// 信道分离
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			cvDilate(pImageChannel[i], pImageChannel[i], 0, times);
		}
		// 信道组合
		cvMerge( pImageChannel[0], pImageChannel[1], pImageChannel[2],
			pImageChannel[3], image );

		for( i = 0; i < orimg->nChannels; i++ )
		{
			if ( pImageChannel[i] )
			{
				cvReleaseImage( &pImageChannel[i] );
				pImageChannel[i] = 0;
			}
		}
	}
}
