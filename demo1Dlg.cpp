
// demo1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo1.h"
#include "demo1Dlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include<stdio.h> 


using namespace cv;
using namespace std;
RNG rng(123);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cdemo1Dlg 对话框



Cdemo1Dlg::Cdemo1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO1_DIALOG, pParent)
	, m_bRun(FALSE)
	, m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cdemo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cdemo1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_EN_CHANGE(IDC_EDIT1, &Cdemo1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_StartGrab, &Cdemo1Dlg::OnBnClickedStartgrab)
	ON_LBN_SELCHANGE(IDC_LIST1, &Cdemo1Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_OpenCam, &Cdemo1Dlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_BUTTON1, &Cdemo1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CloseCam, &Cdemo1Dlg::OnBnClickedClosecam)
	ON_LBN_SELCHANGE(IDC_LIST2, &Cdemo1Dlg::OnLbnSelchangeList2)
	ON_STN_CLICKED(pic, &Cdemo1Dlg::OnStnClickedpic)
	ON_BN_CLICKED(IDC_BUTTON2, &Cdemo1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cdemo1Dlg::OnBnClickedButton3)
	
	
	ON_BN_CLICKED(IDC_PICSAVE, &Cdemo1Dlg::OnBnClickedPicsave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &Cdemo1Dlg::OnBnClickedButton5)
	//ON_STN_CLICKED(pic2, &Cdemo1Dlg::OnStnClickedpic2)
	ON_STN_CLICKED(pic3, &Cdemo1Dlg::OnStnClickedpic3)
	//ON_BN_CLICKED(IDC_BUTTON6, &Cdemo1Dlg::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT2, &Cdemo1Dlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &Cdemo1Dlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// Cdemo1Dlg 消息处理程序

BOOL Cdemo1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("函数库初始化失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("查找连接计算机的相机失败！"),_T( "提示"), MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cdemo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cdemo1Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cdemo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cdemo1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Cdemo1Dlg::OnBnClickedOpencam()//打开相机
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T(" 没 有 找 到 相 机 , 请 确 认 连 接 和 相 机 IP 设 置 "), _T(" 提 示"), MB_ICONWARNING);

		return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T(" 无 法 打 开 相 机 ， 可 能 正 被 别 的 软 件 控 制 "), _T(" 提 示 "), MB_ICONWARNING);

		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING);
		return;
	}

	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}

//采集
void Cdemo1Dlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_image.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
}

int Cdemo1Dlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	
	    MVInfo2Image(m_hCam, pInfo, &m_image);
		DrawImage();
	    return 0;
}
int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	Cdemo1Dlg* pDlg = (Cdemo1Dlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void Cdemo1Dlg::OnBnClickedStartgrab()//开始采集
{
	
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
	
	
		DrawImage();
		
	
		
		
	
	//DrawImage();
}



void Cdemo1Dlg::OnLbnSelchangeList1()//显示绿豆数量
{
	// TODO: 在此添加控件通知处理程序代码
}





void Cdemo1Dlg::OnBnClickedButton1()// 算法放到这个下面
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);                   //停止定时器
	
	
}


void Cdemo1Dlg::OnBnClickedClosecam()//关闭相机
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();
	CDialog::OnClose();
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}

void Cdemo1Dlg::OnClose()
{
	
}

void Cdemo1Dlg::OnLbnSelchangeList2()//豌豆数量
{
	// TODO: 在此添加控件通知处理程序代码
}


void Cdemo1Dlg::OnStnClickedpic()//图片显示区域
{
	// TODO: 在此添加控件通知处理程序代码
}


void Cdemo1Dlg::OnBnClickedButton2()//豌豆数量按钮 实际没有
{
	// TODO: 在此添加控件通知处理程序代码
}


void Cdemo1Dlg::OnBnClickedButton3()//绿豆按钮 实际没有
{
	// TODO: 在此添加控件通知处理程序代码
}



void nihe(Mat& src_image)
{
	int numl[100];//下标
//	int* numw = NULL;
	int countl = 0;  //两种豆子的数量 l绿豆 w豌豆
	int countw = 0;
	float averl = 0.0;  //面积平均值
	float averw = 0.0;
	//const char* filename = "1.png";

	//Mat src_image = imread(filename, 0);

	/*if (src_image.empty())

	{

		cout << "Couldn't open image!" << filename;

		return ;

	}*/



	//imshow("原图", src_image);



	//轮廓

	vector<vector<Point>> contours;



	//使用canny检测出边缘

	Mat edge_image;

	Canny(src_image, edge_image, 30, 70);

	//imshow("canny边缘", edge_image);



	//边缘追踪，没有存储边缘的组织结构

	findContours(edge_image, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat cimage = Mat::zeros(edge_image.size(), CV_8UC3);





	for (int i = 0; i < contours.size(); i++)

	{


		//拟合的点至少为6

		size_t count = contours[i].size();

		if (count < 40)

			continue;



		//椭圆拟合

		RotatedRect box = fitEllipse(contours[i]);



		//如果长宽比大于30，则排除，不做拟合

		if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 3)

			continue;

		//RotatedRect *allbox+j= box;
		//画出追踪出的轮廓

		//drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

		if (averl == 0.0)
		{
			averl = box.size.width * box.size.height;
			numl[countl] = i;
			countl++;
			continue;
		}
		if (box.size.width * box.size.height > (0.4 * averl) && box.size.width* box.size.height < (1.7 * averl))
		{
			//printf("%f\n", averl);
			//printf("s=");
			//printf("%f\t%f\t%f\n", box.size.width, box.size.height,box.size.width * box.size.height);
			numl[countl] = i;
			averl = (countl * averl + box.size.width * box.size.height) / (countl + 1);
			countl++;

		}
	}

	for (int ia = 0; ia < contours.size(); ia++)

	{

		int flagg = 0;
		if (contours[ia].size() < 40)

			continue;

		RotatedRect boxa = fitEllipse(contours[ia]);
		//如果长宽比大于30，则排除，不做拟合

		if (MAX(boxa.size.width, boxa.size.height) > MIN(boxa.size.width, boxa.size.height) * 3)

			continue;
		for (size_t j = 0; j < countl; j++)

		{

			if (ia == numl[j])
			{

				flagg = 1;
				//画出拟合的椭圆

				ellipse(cimage, boxa, Scalar(0, 0, 255), 1, LINE_8);
			}

		}
		if (!flagg)
		{
			ellipse(cimage, boxa, Scalar(0, 255, 0), 1, LINE_8);
			averw = (countw * averw + boxa.size.width * boxa.size.height) / (countw + 1);
			countw = countw + 1;
		}
	}
	if (averl > averw)
	{
		swap(averl, averw);
		swap(countl, countw);
	}
	//imshow("拟合结果", cimage);
	imwrite("D://4.bmp", cimage);

	
	

}



void distance_star(Mat& imge, Mat& outimge) //距离变换函数
{
	Mat gray, binary;
	//滤波后的二值化
	//cvtColor(imge, gray, COLOR_BGR2GRAY);
	//threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//imshow("binary", binary);

	// distance transform 距离变换
	Mat dist;
	distanceTransform(imge, dist, DistanceTypes::DIST_L2, 3, CV_32F);
	normalize(dist, dist, 0, 1, NORM_MINMAX);  // 归一化函数
	imshow("distance result", dist);

	// binary二值化函数
	threshold(dist, outimge, 0.17, 255, THRESH_BINARY);
	imshow("distan4 result", outimge);

	return;

}



void connected_components_stat(Mat& image) {


	int sum = 0;//记录总面积
	int average = 0;//记录平均面积的1/2
	int A = 0;
	int B = 0;
	// 二值化
	Mat gray, binary;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	distance_star(binary, binary);
	Mat dist_m;
	binary.convertTo(binary, CV_8UC1);//通道转换

	//开运算、闭运算
	Mat k = getStructuringElement(MORPH_RECT, Size(13, 13), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_OPEN, k);
	//morphologyEx(binary, binary, MORPH_CLOSE, k);
	 // 形态学操作 - 彩色图像，目的是去掉干扰，让结果更好
	Mat o = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_ERODE, o);// 腐蚀，去粘连部位的干扰


	//计算连通域
	Mat labels = Mat::zeros(image.size(), CV_32S);
	Mat stats, centroids;
	int num_labels = connectedComponentsWithStats(binary, labels, stats, centroids, 8, 4);//对比新旧函数，用于过滤原始图像中轮廓分析后较小的区域，留下较大区域。

	//使用不同的颜色标记连通域
	vector<Vec3b> colors(num_labels);
	vector<Vec3b> colors1(num_labels);
	// background color
	colors[0] = Vec3b(0, 0, 0);
	colors1[0] = Vec3b(0, 0, 0);
	// object color
	for (int i = 1; i < num_labels; i++) {
		colors1[i] = Vec3b(rng.uniform(125, 125), rng.uniform(125, 125), rng.uniform(125, 125));
		colors[i] = Vec3b(rng.uniform(255, 255), rng.uniform(255, 255), rng.uniform(255, 255));
	}
	//Mat dst = Mat::zeros(image.size(), image.type());

	// render result
	Mat dst = Mat::zeros(image.size(), image.type());
	int w = image.cols;
	int h = image.rows;
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			int label = labels.at<int>(row, col);
			if (label == 0) continue;
			dst.at<Vec3b>(row, col) = colors[label];
		}
	}

	//利用统计信息标记连通域
	for (int i = 1; i < num_labels; i++) {
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);
		sum += area;
		printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
		//circle(dst, Point(pt[0], pt[1]), 2, Scalar(0, 0, 255), -1, 8, 0);
		//rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);
	}
	average = sum / num_labels;
	for (int i = 1; i < num_labels; i++)
	{
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);
		if (area > average) {
			A = A + 1;
			//printf("hh:%d\t", x);
			circle(dst, CvPoint2D32f(pt[0], pt[1]), 2, Scalar(0, 0, 255), -1, 8, 0);
			//rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);
		}
		else
		{
			B = B + 1;
			circle(dst, CvPoint2D32f(pt[0], pt[1]), 2, Scalar(255, 0, 0), -1, 8, 0);
			//rectangle(dst, Rect(x, y, width, height), Scalar(255, 255, 0), 1, 8, 0);


		}
	}
	//CString str;
	ofstream file;
	ofstream file1;
	file.open("D://1.txt");
    file << A;
	file1.open("D://2.txt");
	file1 << B;
	file.close();
	file1.close();
	printf("豌豆 : %d, \n绿豆：%d", A, B);
	imwrite("D://3.bmp", dst);
	imshow("连通域标记图像", dst);
	nihe(dst);
}


void pro(Mat& img1) //处理图像
{
	
	IplImage* src = cvLoadImage("D://2.bmp", 0);
	
	Mat img = imread("D://2.bmp");
	

	IplImage* dst = cvCreateImage(cvSize(src->width, src->height), 8, 3);
	CvMemStorage* stor = cvCreateMemStorage(0);
	CvSeq* cont = NULL;
	int num = cvFindContours(src, stor, &cont, sizeof(CvContour), CV_RETR_LIST);      //记录总轮廓数
	int i = 0;
	int j = 0;
	int k = 0;
	float g = 0.0;
	int z = 0;
	float w = 0.0;
	float d = 0.0;
	float flag = 0.0;

	float array1[700][3] = { 0.0 };




	for (i = 0; cont; cont = cont->h_next)
	{
		int* length = new   int[num];        //作用是记录每个轮廓上的点数
		length[i++] = cont->total;         //记录每个轮廓上的元素个数





		printf("cont->total:%d\n", cont->total);
		CvPoint* point = new CvPoint[cont->total];
		CvSeqReader reader;
		CvPoint pt = cvPoint(0, 0);
		CvPoint pt1 = cvPoint(0, 0);
		CvPoint pt2 = cvPoint(0, 0);
		CvPoint pt3 = cvPoint(0, 0);
		CvPoint pt4 = cvPoint(0, 0);
		cvStartReadSeq(cont, &reader);
		for (int j = 0; j < cont->total; j++)
		{
			CV_READ_SEQ_ELEM(pt, reader);
			point[j] = pt;
			//cout << pt.x << "  " << pt.y << endl;
		}
		for (j = 0; j < cont->total; j++)
		{
			int k = (j + 1) % cont->total;
			cvLine(dst, point[j], point[k], cvScalar(0, 0, 255), 1, 4, 0);
		}


		for (int j = 0; j < cont->total - 14; j++)
		{
			CV_READ_SEQ_ELEM(pt1, reader);
			CV_READ_SEQ_ELEM(pt2, reader);
			CV_READ_SEQ_ELEM(pt3, reader);
			pt1 = point[j];
			pt2 = point[j + 7];
			pt3 = point[j + 14];
			

			if ((pt1.x != pt2.x) & (pt3.x != pt2.x))
			{
				//w = j + 8;
				//int g = int(((pt1.y-pt2.y)/(pt1.x-pt2.x))+ ((pt2.y - pt3.y) / (pt2.x - pt3.x)));
				float g = floor((pt2.x - pt1.x) * (pt3.x - pt2.x) + (pt2.y - pt1.y) * (pt3.y - pt2.y));
				if (g < 1.0)
				{
					//w = j + 8;
					flag = 1;

					//cvLine(dst, point[j], point[j+2], cvScalar(0, 255, 0), 1, 4, 0);
					cvCircle(dst, pt2, 1, Scalar(0, 255, 255), -1);
					array1[z][0] = floor(pt2.x);
					array1[z][1] = floor(pt2.y);
					array1[z][2] = flag;
					z++;

				}
			}
		}

		delete point;
	}




	//free(array1);静态数组不需要释放

	
	for (i = 0; i < 700; i++)
	{
		if (array1[i][2] == 1.0)
		{
			for (j = 1; j < 700; j++) {
				w = 35.0;
				float d = sqrt(pow((array1[i][0] - array1[j][0]), 2) +
					pow((array1[i][1] - array1[j][1]), 2));
				if ((d < w) & (d > 5.0)) {
					//w = d;
					CvPoint2D32f pt5 = cvPoint2D32f(0.0, 0.0);
					CvPoint2D32f pt6 = cvPoint2D32f(0.0, 0.0);
					//CvPoint pt6 = cvPoint(0, 0);
					pt5.x = array1[i][0];
					pt5.y = array1[i][1];
					pt6.x = array1[j][0];
					pt6.y = array1[j][1];

					//凹点分割画线
					line(img, pt5, pt6, Scalar(0, 0, 0), 2, 4, 0);
					//printf("z:%d\t", d);


				}
			}
		}
		else {

		}


	}
	cvShowImage("dst", dst);

	imshow("img", img);
	connected_components_stat(img);
	

	
	

	return;
}




void Cdemo1Dlg::OnBnClickedPicsave()
{
	// TODO: 在此添加控件通知处理程序代码
	
	SetTimer(1, 1000, NULL); //设置哪个定时器多久触发
	m_image.Save("D://1.bmp");
	Mat img = imread("D://1.bmp",0);
	//Mat img;
	////进行二值化处理，选择30，200.0为阈值
	threshold(img, img, 40, 255, CV_THRESH_BINARY);
	bitwise_not(img, img, Mat());  //对二进制数据进行“非”操作
	imshow("imge", img);
	imwrite("D://2.bmp",img);

	pro(img);//处理图像
	//m_image.Load("D://3.bmp");
	//DrawImage();
	m_imag.Load("D://4.bmp");
	DrawImage1();



	CFile  file2("D://1.txt", CFile::modeRead);
	CFile  file1("D://2.txt", CFile::modeRead);
	char* pBuf;
	int  iLen = file2.GetLength();
	pBuf = new  char[iLen + 1];
	file2.Read(pBuf, iLen);
	pBuf[iLen] = 0;

	char* pBuf1;
	int  iLen1 = file1.GetLength();
	pBuf1 = new  char[iLen1 + 1];
	file1.Read(pBuf1, iLen1);
	pBuf1[iLen1] = 0;

	file1.Close();
	file2.Close();
	GetDlgItem(IDC_EDIT2)->SetWindowText(pBuf);
	GetDlgItem(IDC_EDIT3)->SetWindowText(pBuf1);
	delete pBuf;
	delete pBuf1;
	////读取结果
	//ifstream file;
	//file.open("D://1.txt");
	//file >> a;
	//LPCTSTR   lp = (LPCTSTR)a;

	//ifstream file1;
	//file1.open("D://2.txt");
	//file1 >> b;
	//LPCTSTR   lp1 = (LPCTSTR)b;
	//
	//file.close();
	//file1.close();//

	////GetDlgItemText(IDC_EDIT2,a,2);
	////GetDlgItem(IDC_EDIT2)->SetDlgItemText(a);
	//SetDlgItemText(IDC_EDIT2,lp);

	//SetDlgItemText(IDC_EDIT2,lp1);


	//
	//m_image.Destroy();
	//GetDlgItem()->
	
}



void Cdemo1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		// 如果收到ID为1的定时器的消息则调用func1函数  
		m_image.Save("D://1.bmp");
		Mat img = imread("D://1.bmp", 0);

		//进行二值化处理，选择30，200.0为阈值
		threshold(img, img, 40, 255, CV_THRESH_BINARY);
		bitwise_not(img, img, Mat());  //对二进制数据进行“非”操作
		imshow("imge", img);
		imwrite("D://2.bmp", img);
		pro(img);//处理图像
		m_imag.Load("D://4.bmp");
		DrawImage1();

		CFile  file2("D://1.txt", CFile::modeRead);
		CFile  file1("D://2.txt", CFile::modeRead);
		char* pBuf;
		int  iLen = file2.GetLength();
		pBuf = new  char[iLen + 1];
		file2.Read(pBuf, iLen);
		pBuf[iLen] = 0;

		char* pBuf1;
		int  iLen1 = file1.GetLength();
		pBuf1 = new  char[iLen1 + 1];
		file1.Read(pBuf1, iLen1);
		pBuf1[iLen1] = 0;

		file1.Close();
		file2.Close();
		GetDlgItem(IDC_EDIT2)->SetWindowText(pBuf);
		GetDlgItem(IDC_EDIT3)->SetWindowText(pBuf1);
		delete pBuf;
		delete pBuf1;

		//读取结果
		/*ifstream file;
		file.open("D://1.txt");
		file >> a;
		LPCTSTR   lp = (LPCTSTR)a;

		ifstream file1;
		file1.open("D://2.txt");
		file1 >> b;
		LPCTSTR   lp1 = (LPCTSTR)b;

		file.close();
		file1.close();



		SetDlgItemText(IDC_EDIT2, lp);*/
		//SetDlgItemText(IDC_EDIT2, lp1);

		//break;

	}


	CDialogEx::OnTimer(nIDEvent);
}

void Cdemo1Dlg::DrawImage1()
{
	CRect rct1;
	GetDlgItem(pic3)->GetClientRect(&rct1);
	int dstW = rct1.Width();
	int dstH = rct1.Height();
	CDC* pDC1 = GetDlgItem(pic3)->GetDC();
	{
		pDC1->SetStretchBltMode(COLORONCOLOR);
		m_imag.Draw(pDC1->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC1);
}

int Cdemo1Dlg::OnStreamCB1(MV_IMAGE_INFO* pInfo)
{

	MVInfo2Image(m_hCam, pInfo, &m_imag);
	DrawImage1();
	return 0;
}

int __stdcall StreamCB1(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	Cdemo1Dlg* pDlg1 = (Cdemo1Dlg*)nUserVal;
	return (pDlg1->OnStreamCB1(pInfo));
}


void Cdemo1Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_imag.Load("D://3.bmp");
	//DrawImage1();
	//return;

}


//void Cdemo1Dlg::OnStnClickedpic2()
//{
	// TODO: 在此添加控件通知处理程序代码
//}


void Cdemo1Dlg::OnStnClickedpic3()
{
	// TODO: 在此添加控件通知处理程序代码
}












//void Cdemo1Dlg::OnBnClickedButton6()
//{
	// TODO: 在此添加控件通知处理程序代码
//}


void Cdemo1Dlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
	
}


void Cdemo1Dlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
