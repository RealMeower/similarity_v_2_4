// similarity_v_2_4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "similarity_v_2_4.h"
#include "similarity_v_2_4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Csimilarity_v_2_4Dlg 对话框




Csimilarity_v_2_4Dlg::Csimilarity_v_2_4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Csimilarity_v_2_4Dlg::IDD, pParent)
	, Image1(NULL)
	, Image2(NULL)
	, m_edit2(_T(""))
	, m_edit1(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csimilarity_v_2_4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
}

BEGIN_MESSAGE_MAP(Csimilarity_v_2_4Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &Csimilarity_v_2_4Dlg::OnBnClickedButtonOpenImage1)
	ON_BN_CLICKED(IDC_BUTTON3, &Csimilarity_v_2_4Dlg::OnBnClickedButtonOpenImage2)
	ON_BN_CLICKED(IDC_BUTTON1, &Csimilarity_v_2_4Dlg::OnBnClickedButtonColorCompare)
	ON_BN_CLICKED(IDC_BUTTON4, &Csimilarity_v_2_4Dlg::OnBnClickedButtonContoursCompare)
	ON_BN_CLICKED(IDC_BUTTON5, &Csimilarity_v_2_4Dlg::OnBnClickedButtonMeanshiftSeg)
	ON_BN_CLICKED(IDC_BUTTON7, &Csimilarity_v_2_4Dlg::OnBnClickedButtonHVPro)
	ON_BN_CLICKED(IDC_BUTTON6, &Csimilarity_v_2_4Dlg::OnBnClickedButtonSubRectCompare)
END_MESSAGE_MAP()


// Csimilarity_v_2_4Dlg 消息处理程序

BOOL Csimilarity_v_2_4Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Csimilarity_v_2_4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Csimilarity_v_2_4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Csimilarity_v_2_4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Csimilarity_v_2_4Dlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf( img ); // 复制图片
	cimg.DrawToHDC( hDC, &rect ); // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}


void Csimilarity_v_2_4Dlg::OnBnClickedButtonOpenImage1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,"All File",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files (*.*)|*.*|");
	if(dlg.DoModal()!=IDOK)
	{
		return ;
	}
	CString cscsFilePath = dlg.GetPathName();

	Image1 = cvLoadImage(cscsFilePath,1); //显示图片
	DrawPicToHDC(Image1, IDC_STATIC1);
}

void Csimilarity_v_2_4Dlg::OnBnClickedButtonOpenImage2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,"All File",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files (*.*)|*.*|");
	if(dlg.DoModal()!=IDOK)
	{
		return ;
	}
	CString cscsFilePath = dlg.GetPathName();

	Image2 = cvLoadImage(cscsFilePath,1); //显示图片
	DrawPicToHDC(Image2, IDC_STATIC2);
}


void Csimilarity_v_2_4Dlg::OnBnClickedButtonColorCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!Image1 || !Image2)
	{
		MessageBox(_T("请打开两幅图像"), NULL, MB_OK);
		return;
	}

	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())//RGB对比
	{
		//调整图像二大小
		IplImage *imageresize = 0;
		imageresize	= cvCreateImage(cvSize(Image1->width,Image2->height),IPL_DEPTH_8U,3);
		cvResize(Image2,imageresize,CV_INTER_LINEAR);

		RGB_HIST* hist_1 = (RGB_HIST *)malloc(sizeof(CvHistogram*)*3);
		RGB_HIST* hist_2 = (RGB_HIST *)malloc(sizeof(CvHistogram*)*3);
		cal_rgb_hist(Image1, hist_1);
		cal_rgb_hist(imageresize, hist_2);

		cvNamedWindow(win_name_r, 1);
		cvNamedWindow(win_name_g, 1);
		cvNamedWindow(win_name_b, 1);

		IplImage* hist_imageR = cvCreateImage(cvSize(320,200), 8, 3);
		IplImage* hist_imageG = cvCreateImage(cvSize(320,200), 8, 3);
		IplImage* hist_imageB = cvCreateImage(cvSize(320,200), 8, 3);

		cvSet( hist_imageR, cvScalarAll(255), 0 );
		cvSet( hist_imageG, cvScalarAll(255), 0 );
		cvSet( hist_imageB, cvScalarAll(255), 0 );

		show_rgb_histogram(hist_1, hist_imageR, hist_imageG, hist_imageB, CV_RGB(0xff,0x00,0x00));
		show_rgb_histogram(hist_2, hist_imageR, hist_imageG, hist_imageB, CV_RGB(0x00,0x00,0xff));

		double dist2 = rgb_total_hist_compare(hist_1, hist_2);
		CString result;
		if(dist2>0 && dist2<0.25)
		{
			result.Format("巴氏距离: %f 非常相似", dist2);
			MessageBox(result);
		}
		else if(dist2 == 0.25 || (dist2>0.25 && dist2<0.45))
		{
			result.Format("巴氏距离: %f 比较相似", dist2);
			MessageBox(result);
		}
		else if(dist2 == 0.45 || dist2>0.45)
		{
			result.Format("巴氏距离: %f 不相似", dist2);
			MessageBox(result);
		}
		cvDestroyAllWindows();
	}

	else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())//HS对比
	{
		CvHistogram* h_s_hist1;
		CvHistogram* h_s_hist2;

		int h_bins = 30, s_bins = 32; 
		int scale = 10;
		int    h_s_hist_size[] = { h_bins, s_bins };
		float  h_ranges[]  = { 0, 180 };        
		float  s_ranges[]  = { 0, 255 }; 
		float* ranges[]    = { h_ranges, s_ranges };

		IplImage *imageresize = 0;
		imageresize	= cvCreateImage(cvSize(Image1->width,Image2->height),IPL_DEPTH_8U,3);
		cvResize(Image2,imageresize,CV_INTER_LINEAR);

		h_s_hist1 = cvCreateHist( 
			2, 
			h_s_hist_size, 
			CV_HIST_ARRAY, 
			ranges, 
			1 
			); 
		h_s_hist2 = cvCreateHist( 
			2, 
			h_s_hist_size, 
			CV_HIST_ARRAY, 
			ranges, 
			1
			); 

		cal_2D_hsv_hist(Image1, h_s_hist1);
		cal_2D_hsv_hist(imageresize, h_s_hist2);
		//显示二维直方图
		IplImage* hist_img1 = cvCreateImage(  
			cvSize( h_bins * scale, s_bins * scale ), 
			8, 
			3
			); 
		cvZero( hist_img1 );
		draw_2D_h_s_hist(hist_img1, h_s_hist1, h_bins, s_bins, scale);

		IplImage* hist_img2 = cvCreateImage(  
			cvSize( h_bins * scale, s_bins * scale ), 
			8, 
			3
			); 
		cvZero( hist_img2 );
		draw_2D_h_s_hist(hist_img2, h_s_hist2, h_bins, s_bins, scale);
		cvNamedWindow( "H-S Histogram1", 1 );
		cvShowImage(   "H-S Histogram1", hist_img1);
		cvNamedWindow( "H-S Histogram2", 1 );
		cvShowImage(   "H-S Histogram2", hist_img2);

		double dist_h_s = cvCompareHist(h_s_hist1, h_s_hist2, CV_COMP_BHATTACHARYYA);	

		CString result;
		if(dist_h_s>0 && dist_h_s<0.4)
		{
			result.Format("巴氏距离: %f 非常相似", dist_h_s);
			MessageBox(result);
		}
		else if(dist_h_s == 0.4 || (dist_h_s>0.4 && dist_h_s<0.55))
		{
			result.Format("巴氏距离: %f 比较相似", dist_h_s);
			MessageBox(result);
		}
		else if(dist_h_s == 0.55 || dist_h_s>0.55)
		{
			result.Format("巴氏距离: %f 不相似", dist_h_s);
			MessageBox(result);
		}

		cvDestroyAllWindows();
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())//H通道对比
	{
		IplImage* ResizeImage1 = 0;
		IplImage* ResizeImage2 = 0;

		ResizeImage1 = cvCreateImage(cvSize(150,150),Image1->depth,3);
		cvResize(Image1 ,ResizeImage1,CV_INTER_LINEAR);
		ResizeImage2 = cvCreateImage(cvSize(150,150),Image1->depth,3);
		cvResize(Image2, ResizeImage2,CV_INTER_LINEAR);

		CvHistogram *hist1, *hist2;

		IplImage *hist_image1;
		hist_image1 = cvCreateImage(cvSize(200, 300), IPL_DEPTH_8U, 1);
		cvSet( hist_image1, cvScalarAll(255), 0 );
		hist1 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
		cal_1D_h_hist(ResizeImage1, hist1);
		draw_1D_h_histogram(hist1, hist_image1);
		cvNamedWindow("H histogram1",CV_WINDOW_AUTOSIZE);
		cvShowImage("H histogram1", hist_image1);

		IplImage *hist_image2;
		hist_image2 = cvCreateImage(cvSize(200, 300), IPL_DEPTH_8U, 1);
		cvSet( hist_image2, cvScalarAll(255), 0 );
		hist2 = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
		cal_1D_h_hist(ResizeImage2, hist2);
		draw_1D_h_histogram(hist2, hist_image2);
		cvNamedWindow("H histogram2",CV_WINDOW_AUTOSIZE);
		cvShowImage("H histogram2", hist_image2);

		double dist_h;
		dist_h = cvCompareHist(hist1, hist2, CV_COMP_BHATTACHARYYA);
		CString result;
		result.Format("巴氏距离: %f ", dist_h);
		MessageBox(result);
		cvDestroyAllWindows();
	}
	else
		MessageBox(_T("请选择颜色对比方式！"), NULL, MB_OK);
}

void Csimilarity_v_2_4Dlg::OnBnClickedButtonContoursCompare()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!Image1 || !Image2)
	{
		MessageBox(_T("请打开两幅图像"), NULL, MB_OK);
		return;
	}

	if(((CButton *)GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		IplImage* GrayImage1 = 0;
		IplImage* GrayImage2 = 0;

		GrayImage1 = cvCreateImage(cvGetSize(Image1),Image1->depth,1);
		GrayImage2 = cvCreateImage(cvGetSize(Image2),Image2->depth,1);

		cvCvtColor(Image1, GrayImage1, CV_BGR2GRAY);
		cvCvtColor(Image2, GrayImage2, CV_BGR2GRAY);

		IplImage* ResizeImage1 = 0;
		IplImage* ResizeImage2 = 0;

		ResizeImage1 = cvCreateImage(cvSize(150,150),Image1->depth,1);
		ResizeImage2 = cvCreateImage(cvSize(150,150),Image2->depth,1);

		cvResize(GrayImage1 ,ResizeImage1,CV_INTER_LINEAR);
		cvResize(GrayImage2, ResizeImage2,CV_INTER_LINEAR);

		CvSeq* first_contour1 = NULL;
		CvSeq* first_contour2 = NULL;

		CvMemStorage* storage1 = cvCreateMemStorage();
		CvMemStorage* storage2 = cvCreateMemStorage();

		IplImage* img_conA = cvCreateImage( cvGetSize(ResizeImage1), 8, 1 );
		IplImage* img_conB = cvCreateImage( cvGetSize(ResizeImage2), 8, 1 );

		calc_contours( "A", first_contour1, storage1, ResizeImage1, img_conA);
		calc_contours( "B", first_contour2, storage2, ResizeImage2, img_conB);
		double dist_con = cvMatchShapes( img_conA, img_conB, CV_CONTOURS_MATCH_I1, 0 );

		CString result;
		result.Format("轮廓矩对比结果: %f ", dist_con);
		MessageBox(result);
		cvDestroyAllWindows();

		cvReleaseImage(&ResizeImage1);
		cvReleaseImage(&ResizeImage2);
		cvReleaseImage(&img_conA);
		cvReleaseImage(&img_conB);
	}

	else if (((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck()) //几何直方图对比方法
	{
		IplImage *imageresize = 0;
		imageresize	= cvCreateImage(cvSize(Image1->width,Image1->height),IPL_DEPTH_8U,3);
		cvResize(Image2,imageresize,CV_INTER_LINEAR);
		IplImage *contours1;
		IplImage *contours2;
		contours1 = cvCreateImage(cvGetSize(Image1), IPL_DEPTH_8U, 1);
		contours2 = cvCreateImage(cvGetSize(Image1), IPL_DEPTH_8U, 1);
		CvSeq* ContourSeq1;
		CvSeq* ContourSeq2;
		CvMemStorage *storage1;
		CvMemStorage *storage2;
		storage1 = cvCreateMemStorage();
		storage2 = cvCreateMemStorage();
		gesFindContours(Image1, contours1, &ContourSeq1, storage1, 1);
		gesFindContours(imageresize, contours2, &ContourSeq2, storage2, 1);
		cvNamedWindow("src1",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("src2",CV_WINDOW_AUTOSIZE);
		cvShowImage("src1", Image1);
		cvShowImage("src2", imageresize);
		cvNamedWindow("con1", CV_WINDOW_AUTOSIZE);
		cvNamedWindow("con2", CV_WINDOW_AUTOSIZE);
		cvShowImage("con1", contours1);
		cvShowImage("con2", contours2);
		CvHistogram* hist1;
		CvHistogram* hist2;
		hist1 = gesCalcContoursPGH(ContourSeq1);
		double dist_con_hist = gesMatchContoursPGH(ContourSeq2, hist1);

		CString result;
		result.Format("几何直方图对比结果: %f ", dist_con_hist);
		MessageBox(result);
		cvDestroyAllWindows();
	}
	else 
		MessageBox(_T("请选择轮廓对比方式！"), NULL, MB_OK);

}


void Csimilarity_v_2_4Dlg::OnBnClickedButtonMeanshiftSeg()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!Image1 || !Image2)
	{
		MessageBox(_T("请打开两幅图像"), NULL, MB_OK);
		return;
	}	
	IplImage* ResizeImage1 = 0;
	IplImage* ResizeImage2 = 0;

	ResizeImage1 = cvCreateImage(cvSize(150,150),Image1->depth,3);
	cvResize(Image1 ,ResizeImage1,CV_INTER_LINEAR);
	ResizeImage2 = cvCreateImage(cvSize(150,150),Image1->depth,3);
	cvResize(Image2, ResizeImage2,CV_INTER_LINEAR);

	IplImage* dst1 = cvCreateImage(cvGetSize(ResizeImage1),ResizeImage1->depth,3);
	IplImage* dst2 = cvCreateImage(cvGetSize(ResizeImage2),ResizeImage2->depth,3);

	cvNamedWindow("Image1",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image2",CV_WINDOW_AUTOSIZE);

	cvPyrMeanShiftFiltering(ResizeImage1,dst1,10,40,1);
	cvPyrMeanShiftFiltering(ResizeImage2,dst2,10,40,1);

	CvConnectedComp comp;
	int lo = 1, up = 2;
	cvFloodFill( dst1, cvPoint(10,10),  CV_RGB(0,0,0), CV_RGB(lo,lo,lo), CV_RGB(up,up,up), NULL, 4,  NULL );
	cvFloodFill( dst1, cvPoint(dst1->width-10,10),  CV_RGB(0,0,0), CV_RGB(lo,lo,lo), CV_RGB(up,up,up), NULL, 4,  NULL );
	cvFloodFill( dst2, cvPoint(10,10),  CV_RGB(0,0,0), CV_RGB(lo,lo,lo), CV_RGB(up,up,up), NULL, 4,  NULL );
	cvFloodFill( dst2, cvPoint(dst2->width-10,10),  CV_RGB(0,0,0), CV_RGB(lo,lo,lo), CV_RGB(up,up,up), NULL, 4,  NULL );

	IplImage* sing_dst1 = cvCreateImage(cvGetSize(ResizeImage1),ResizeImage1->depth,1);
	IplImage* sing_dst2 = cvCreateImage(cvGetSize(ResizeImage2),ResizeImage2->depth,1);

	cvCvtColor(dst1, sing_dst1, CV_RGB2GRAY);
	cvCvtColor(dst2, sing_dst2, CV_RGB2GRAY);

	IplImage* white_image = cvCreateImage(cvGetSize(ResizeImage1),ResizeImage1->depth,1);
	cvSet(white_image, cvScalarAll(0), NULL);

	cvCmp(sing_dst1, white_image, sing_dst1, CV_CMP_EQ);
	cvCmp(sing_dst2, white_image, sing_dst2, CV_CMP_EQ );

	cvNot(sing_dst1, sing_dst1);
	cvNot(sing_dst2, sing_dst2);

	IplImage* sub_dst1 = cvCreateImage(cvGetSize(ResizeImage1),ResizeImage1->depth,3);
	IplImage* sub_dst2 = cvCreateImage(cvGetSize(ResizeImage2),ResizeImage2->depth,3);

	cvZero(sub_dst1);
	cvZero(sub_dst2);

	cvCopy(ResizeImage1,sub_dst1,sing_dst1);
	cvCopy(ResizeImage2,sub_dst2,sing_dst2);

	cvShowImage("Image1", sub_dst1);
	cvShowImage("Image2", sub_dst2);
}

void Csimilarity_v_2_4Dlg::OnBnClickedButtonHVPro()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!Image1 || !Image2)
	{
		MessageBox(_T("请打开两幅图像"), NULL, MB_OK);
		return;
	}	
	IplImage* ResizeImage1 = 0;
	IplImage* ResizeImage2 = 0;

	IplImage* ImageGray1 = 0;
	IplImage* ImageGray2 = 0;

	ImageGray1 = cvCreateImage(cvGetSize(Image1),Image1->depth,1);
	ImageGray2 = cvCreateImage(cvGetSize(Image2),Image2->depth,1);

	cvCvtColor(Image1, ImageGray1, CV_BGR2GRAY);
	cvCvtColor(Image2, ImageGray2, CV_BGR2GRAY);

	ResizeImage1 = cvCreateImage(cvSize(150,150),Image1->depth,1);
	ResizeImage2 = cvCreateImage(cvSize(150,150),Image1->depth,1);

	cvResize(ImageGray1 ,ResizeImage1,CV_INTER_LINEAR);
	cvResize(ImageGray2, ResizeImage2,CV_INTER_LINEAR);

	IplImage* ThreImage1 = cvCreateImage(cvSize(150,150),ResizeImage1->depth,1);
	IplImage* ThreImage2 = cvCreateImage(cvSize(150,150),ResizeImage2->depth,1);

	cvAdaptiveThreshold(ResizeImage1, ThreImage1, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 9, 15);
	cvAdaptiveThreshold(ResizeImage2, ThreImage2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 9, 15);

	//cvThreshold(ResizeImage1,ResizeImage1,50,255,CV_THRESH_BINARY_INV); 
	//cvThreshold(ResizeImage2,ResizeImage2,50,255,CV_THRESH_BINARY_INV); 

	cvNamedWindow("Image1",CV_WINDOW_AUTOSIZE);
	cvShowImage("Image1", ThreImage1);
	cvNamedWindow("Image2",CV_WINDOW_AUTOSIZE);
	cvShowImage("Image2", ThreImage2);

	//计算水平垂直积分投影

	IplImage* paintx1=cvCreateImage( cvGetSize(ThreImage1),IPL_DEPTH_8U, 1 );
	IplImage* painty1=cvCreateImage( cvGetSize(ThreImage1),IPL_DEPTH_8U, 1 );

	IplImage* paintx2=cvCreateImage( cvGetSize(ThreImage2),IPL_DEPTH_8U, 1 );
	IplImage* painty2=cvCreateImage( cvGetSize(ThreImage2),IPL_DEPTH_8U, 1 );

	cvSet(paintx1,cvScalarAll(255), NULL);
	cvSet(painty1,cvScalarAll(255), NULL);

	cvSet(paintx2,cvScalarAll(255), NULL);
	cvSet(painty2,cvScalarAll(255), NULL);

	thresh_horiz_project(ThreImage1, paintx1);
	thresh_vertical_project(ThreImage1, painty1);

	thresh_horiz_project(ThreImage2, paintx2);
	thresh_vertical_project(ThreImage2, painty2);

	cvNamedWindow("垂直1",1);
	cvNamedWindow("水平1",1);
	cvShowImage("垂直1",paintx1);
	cvShowImage("水平1",painty1);

	cvNamedWindow("垂直2",1);
	cvNamedWindow("水平2",1);
	cvShowImage("垂直2",paintx2);
	cvShowImage("水平2",painty2);
}

void Csimilarity_v_2_4Dlg::OnBnClickedButtonSubRectCompare()
{
	if(!Image1 || !Image2)
	{
		MessageBox(_T("请打开两幅图像"), NULL, MB_OK);
		return;
	}	
	IplImage* ResizeImage1 = 0;
	IplImage* ResizeImage2 = 0;

	ResizeImage1 = cvCreateImage(cvSize(200,200),Image1->depth,3);
	cvResize(Image1 ,ResizeImage1,CV_INTER_LINEAR);
	ResizeImage2 = cvCreateImage(cvSize(200,200),Image1->depth,3);
	cvResize(Image2, ResizeImage2,CV_INTER_LINEAR);
	
	CString str1; 
	GetDlgItem(IDC_EDIT1)->GetWindowText(str1);
	//CString类型的转换成int 
	int row_num =  atoi(str1) ;
	CString str2; 
	GetDlgItem(IDC_EDIT2)->GetWindowText(str2);
	//CString类型的转换成int 
	int col_num =  atoi(str2) ; 
	double dist = rgb_subset_compare_hist(ResizeImage1, ResizeImage2, row_num, col_num);

	cvNamedWindow("Image1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image2", CV_WINDOW_AUTOSIZE);

	cvShowImage("Image1",ResizeImage1);
	cvShowImage("Image2",ResizeImage2);

	CString result;
	result.Format("平均巴氏距离: %f ", dist);
	MessageBox(result);
	cvDestroyAllWindows();
}
