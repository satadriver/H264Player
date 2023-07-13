// Decode.cpp: implementation of the CDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "NetPlayer.h"
#include "Decode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDecode::CDecode()
{
	m_pFormatCtx=NULL;	
	m_pCodecCtx=NULL;
	m_pCodec=NULL;
	m_pFrameRGB=NULL;
	m_pFrame=NULL;
	m_buffer=NULL;

}

CDecode::~CDecode()
{

}

bool CDecode::GetNextFrame()
{
	static AVPacket  packet;
	static bool      fFirstTime=true;
	static int       bytesRemaining=0;
	static uint8_t   *RawData;
	int              bytesdecoded;
	int              frameFinished;

	//第一次执行时，将packet.data  置空表明他没必要被释放
	if (fFirstTime)
	{
		fFirstTime = false;
		packet.data = NULL; 
	}

	//解码数据包直到解满一桢
	while (true)
	{
		//不断得解码当前数据包直到解完全部数据
		while (bytesRemaining > 0)
		{
			//解码下一段截获的数据
			bytesdecoded = avcodec_decode_video(m_pCodecCtx,m_pFrame,&frameFinished,RawData,bytesRemaining);
			//判断是否出错
			if (bytesdecoded < 0)
			{
				fprintf(stderr,"Error while decoding frame\n");
				return false;
			}

			bytesRemaining -= bytesdecoded;
			RawData += bytesdecoded;

			//判断是否解完当前帧
			if (frameFinished)   			                    
				return true;		   
		}

		//读取下一个数据包，跳过非当前码流的数据包
		do 
		{
			//释放前一个数据包
			if (packet.data != NULL)
				av_free_packet(&packet);

			//读取新的包
			if(av_read_frame(m_pFormatCtx,&packet) < 0)
				goto loop_exit;

		} while (packet.stream_index != m_videoStream);

		bytesRemaining = packet.size;
		RawData = packet.data;

	}

loop_exit:
	//解码最后一桢剩下的包
	bytesdecoded = avcodec_decode_video(m_pCodecCtx,m_pFrame,&frameFinished,RawData,
		bytesRemaining);

	//释放最后一个数据包
	if (packet.data != NULL)
		av_free_packet(&packet);

	return frameFinished != 0;	   
}


//////////////////////////////////////////////////////////////////////////
// show Image

bool CDecode::InitDecode(char* infile)
{
	int             i;
	int             i_frame=0;	
	int             numBytes;

	// Register all formats and codecs
	av_register_all();
	// Open video file
	if (av_open_input_file(&m_pFormatCtx,infile, NULL, 0, NULL) != 0)
	{
		AfxMessageBox("无法打开文件!");// Couldn't open file
		return false;
	}

	// Retrieve stream information
	if (av_find_stream_info(m_pFormatCtx) < 0)
	{
		AfxMessageBox("Couldn't find stream information");// Couldn't find stream information
		return false;
	} 
	// Dump information about file onto standard error
	dump_format(m_pFormatCtx, 0, infile, false);

	// Find the first video stream
	m_videoStream = -1;
	for (i=0; i<m_pFormatCtx->nb_streams; i++)
		if(m_pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO){
			m_videoStream=i;
			break;
		}
		if (m_videoStream == -1)
		{
			AfxMessageBox("Didn't find a video stream");// Didn't find a video stream
			return false;
		}  
		// Get a pointer to the codec context for the video stream
		m_pCodecCtx = m_pFormatCtx->streams[m_videoStream]->codec;
		// Find the decoder for the video stream
		m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
		if (m_pCodec == NULL)
		{
			AfxMessageBox("Codec not found");// Codec not found
			return false;
		}  
		// Inform the codec that we can handle truncated bitstreams -- i.e.,
		// bitstreams where frame boundaries can fall in the middle of packets
		if(m_pCodec->capabilities & CODEC_CAP_TRUNCATED)
			m_pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
		// Open codec
		if (avcodec_open(m_pCodecCtx, m_pCodec) < 0)
		{
			AfxMessageBox("Could not open codec");// Could not open codec
			return false;
		}   
		// Allocate video frame
		m_pFrame = avcodec_alloc_frame();
		// Allocate an AVFrame structure
		m_pFrameRGB = avcodec_alloc_frame();
		if(m_pFrameRGB == NULL)
			return false;        
		// Determine required buffer size and allocate buffer
		numBytes=avpicture_get_size(PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height);
		m_buffer=new uint8_t[numBytes];        
		// Assign appropriate parts of buffer to image planes in pFrameRGB
		avpicture_fill((AVPicture *)m_pFrameRGB, m_buffer, PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height);
		return true;
}

//封装ffmpeg中的ima_convert()函数
void CDecode::ImgConvert() 
{
	img_convert((AVPicture *)m_pFrameRGB, PIX_FMT_BGR24,(AVPicture*)m_pFrame,m_pCodecCtx->pix_fmt, m_pCodecCtx->width,m_pCodecCtx->height);
}

unsigned char* CDecode::GetBmpData()   //取得图像数据
{
	return m_pFrameRGB->data[0];
}

int CDecode::GetFrameWidth()   //取得图像的实际宽度
{
	return m_pCodecCtx->width;
}

int CDecode::GetFrameHeight()           //取得图像的实际高度
{
	return m_pCodecCtx->height;
}

//释放对象关闭资源
void CDecode::ReleseObj()
{
	if(m_buffer!=NULL)     free(m_buffer);
	if(m_pFrameRGB!=NULL)  av_free(m_pFrameRGB);
	// Free the YUV frame
	if(m_pFrame!=NULL)     av_free(m_pFrame);
	// Close the codec
	if(m_pCodecCtx!=NULL)  avcodec_close(m_pCodecCtx);
	// Close the video file
	if(m_pFormatCtx!=NULL) av_close_input_file(m_pFormatCtx);
}