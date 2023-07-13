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
	//cascade_name ="\opencv\data\haarcascades\haarcascade_frontalface_alt.xml";//������������
	cascade_name ="haarcascade_frontalface_alt.xml";
}

COpenCVPotoProcessor::~COpenCVPotoProcessor()
{
	
}

int COpenCVPotoProcessor::CVCanny(IplImage* &pSrcImg ,int thresh)
{
	//����ͼ��IplImageָ��
	IplImage* pGrayImg = NULL; 
	IplImage* pCannyImg = NULL;
	IplImage* pCEdge = NULL;
	
	// Create the output image
	pCEdge = cvCreateImage(cvSize(pSrcImg->width,pSrcImg->height), IPL_DEPTH_8U, 3);
	// Convert to grayscale
	pGrayImg=cvCreateImage(cvGetSize(pSrcImg),IPL_DEPTH_8U,1);
	cvCvtColor(pSrcImg,pGrayImg,CV_RGB2GRAY);
	//canny��Ե���
	pCannyImg = cvCreateImage(cvGetSize(pGrayImg),IPL_DEPTH_8U,1);
	cvSmooth( pGrayImg, pCannyImg, CV_BLUR, 3, 3, 0);
	cvNot( pGrayImg, pCannyImg );
	// Run the edge detector on grayscale
	cvCanny(pGrayImg, pCannyImg,(float)thresh,(float)thresh*3, 3);
	
	cvZero( pCEdge );
	// copy edge points
	cvCopy( pSrcImg, pCEdge, pCannyImg );
	
	cvReleaseImage( &pSrcImg );
	pSrcImg=cvCloneImage(pCEdge);       // ��ɫ���
	//pSrcImg=cvCloneImage(pCannyImg);  //�Ҷ�ͼ���
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
    IplImage *dst = 0;			//Ŀ��ͼ��ָ��
    float scal = ((float)scale)/10;		//scaleΪ100�����ű���Ϊ100/10Ϊ10��
	if (scal == 0)
		scal = 0.1;
    CvSize dst_cvsize;			//Ŀ��ͼ��ߴ�
	
    dst_cvsize.width = src->width * scal;		//Ŀ��ͼ��Ŀ�ΪԴͼ����scale��
    dst_cvsize.height = src->height * scal;	//Ŀ��ͼ��ĸ�ΪԴͼ��ߵ�scale��
	
    dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels);	//����Ŀ��ͼ��
    cvResize(src, dst, CV_INTER_LINEAR);	//����Դͼ��Ŀ��ͼ��
	
	cvReleaseImage( &src );
    src=cvCloneImage(dst);
	
    cvReleaseImage(&dst);	//�ͷ�Ŀ��ͼ��ռ�õ��ڴ�
}


void COpenCVPotoProcessor::CVLaplace(IplImage* orimg,IplImage* &image,int aperture_size) //����˹��,aperture_sizeΪ��ֵ��ֻ��ȡ1��3��5��7
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
    // �Բ�ɫͼ����зָ�
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
		// �ŵ�����
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			// ֱ��ͼ���⻯
			cvEqualizeHist( pImageChannel[i], pImageChannel[i] );
		}
		// �ŵ����
		cvMerge( pImageChannel[0], pImageChannel[1], pImageChannel[2],
			pImageChannel[3], image );
		// ����ͼ����ʾ���루�ԣ�
		// �ͷ���Դ
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
	//���أ������������ѵ����
	cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0 );
	//���ز��ɹ�����ʾ����ѶϢ�����˳�
	if(cascade)
	   {
		storage = cvCreateMemStorage(0);
		//���ͼƬ�������������ʾ����������˳�����
		if(src) detect_and_draw(src);//�������������ʾ�¼�
		//cvReleaseImage(&src);
		cvReleaseMemStorage( &storage );
	}
	else
	{
		//&nbsp;
		AfxMessageBox("�޷����ط���������ȷ�Ϻ�����!");
	}
	cvReleaseHaarClassifierCascade( &cascade );
}

void COpenCVPotoProcessor::detect_and_draw(IplImage *&img)//���������ʾ�¼�
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
    };//�������ñ�ʾͼ������������ɫ
	double scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8,1 );
	int i;
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );
	if( cascade )
	{//�������
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

// ͼ�����ȵ���
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
			// δ���� COI
			if (nCOI == 0)
			{
				// ͼ������
				cvAddS(src, value, dst, mask);
				bResult = true;
			}
			// �ṩ COI ֧��
			else
			{
				IplImage *pGray = cvCreateImage(cvGetSize(src), src->depth, 1);
				if (pGray)
				{
					cvCopy(src, pGray, NULL);
					// ͼ������
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

// ͼ��Աȶȵ���
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
			// δ���� COI		
			if (nCOI == 0)			
			{
				// �Աȶȵ���				
				cvConvertScale(src, dst, dScale, dShift);				
				bResult = true;				
			}			
			// �ṩ COI ֧��			
			else				
			{				
				IplImage *pGray = cvCreateImage(cvGetSize(src), src->depth, 1);				
				if (pGray)					
				{					
					cvCopy(src, pGray, NULL);					
					// �Աȶȵ���					
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
		// �ŵ�����
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			cvErode( pImageChannel[i], pImageChannel[i], 0, times);
		}
		// �ŵ����
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
		// �ŵ�����
		cvSplit( orimg, pImageChannel[0], pImageChannel[1],
			pImageChannel[2], pImageChannel[3] );
		for( i = 0; i < image->nChannels; i++ )
		{
			cvDilate(pImageChannel[i], pImageChannel[i], 0, times);
		}
		// �ŵ����
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
