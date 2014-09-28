#include "stdafx.h"
#include "HoriVert.h"

void thresh_horiz_project(IplImage* src, IplImage* proj_image)
{
	int* v=new int[src->width];
	memset(v,0,src->width*4);

	int x,y;
	CvScalar s,t;
	for(x=0;x<src->width;x++)
	{
		for(y=0;y<src->height;y++)
		{
			s=cvGet2D(src,y,x);			
			if(s.val[0]==0) //如果使用的图像是1通道的，则s.val[0]中存储数据
				v[x]++;					
		}		
	}

	for(x=0;x<src->width;x++)
	{
		for(y=0;y<v[x];y++)
		{		
			t.val[0]=0;
			cvSet2D(proj_image,y,x,t);		
		}		
	}
}

void thresh_vertical_project(IplImage* src, IplImage* proj_image)
{
	int* h=new int[src->height];
	memset(h,0,src->height*4);

	int x,y;
	CvScalar s,t;
	for(y=0;y<src->height;y++)
	{
		for(x=0;x<src->width;x++)
		{
			s=cvGet2D(src,y,x);			
			if(s.val[0]==0)
				h[y]++;		
		}	
	}
	for(y=0;y<src->height;y++)
	{
		for(x=0;x<h[y];x++)
		{			
			t.val[0]=0;
			cvSet2D(proj_image,y,x,t);			
		}		
	}
}

void hue_horiz_project(IplImage* src, IplImage* proj_image)
{
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 ); 
	cvCvtColor( src, hsv, CV_BGR2HSV );

	IplImage* h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	
	int* v=new int[src->width];
	memset(v,0,src->width*4);

	int x,y;
	CvScalar s,t;
	for(x=0;x<src->width;x++)
	{
		for(y=0;y<src->height;y++)
		{
			s=cvGet2D(src,y,x);			
			if(s.val[0]==0) 
				v[x]++;					
		}		
	}

	for(x=0;x<src->width;x++)
	{
		for(y=0;y<v[x];y++)
		{		
			t.val[0]=0;
			cvSet2D(proj_image,y,x,t);		
		}		
	}
}

