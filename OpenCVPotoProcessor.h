// OpenCVPotoProcessor.h: interface for the COpenCVPotoProcessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENCVPOTOPROCESSOR_H__D7F4FE6C_DF4B_42CB_A3A4_DD3AB9917BB4__INCLUDED_)
#define AFX_OPENCVPOTOPROCESSOR_H__D7F4FE6C_DF4B_42CB_A3A4_DD3AB9917BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cvheader.h"
//#include "cv.h"
//#include "highgui.h"

class COpenCVPotoProcessor  
{
public:
	void CVFaceDetection(IplImage* &src ,IplImage* dest);
	CvHaarClassifierCascade* cascade;//特征器分类
	CvMemStorage* storage;
	char* cascade_name;
	void detect_and_draw( IplImage* &img );


	COpenCVPotoProcessor();
	virtual ~COpenCVPotoProcessor();
	
	void CVResizeImage(IplImage *&src,int scale);
	void CVContours(IplImage* img);
	int  CVCanny(IplImage* &pSrcImg ,int thresh);
	void CVLaplace(IplImage* orimg,IplImage* &image,int aperture_size) ; //普拉斯锐化,aperture_size为核值，只能取1、3、5、7
	void CVPyrSegmentation(IplImage* orimg,IplImage* &image,int threshold1,int threshold2);
	void CVImageStrengthen(IplImage* orimg,IplImage* &image);
	bool ImgLighteness(IplImage *src, IplImage *dst, CvScalar value, CvArr *mask);
	bool ImgContrast(IplImage *src, IplImage *dst, double contrast, double lighteness);
	void Erode(IplImage* orimg,IplImage* &image,int  times);
	void Expend(IplImage* orimg,IplImage* &image,int  times);
};

#endif // !defined(AFX_OPENCVPOTOPROCESSOR_H__D7F4FE6C_DF4B_42CB_A3A4_DD3AB9917BB4__INCLUDED_)
