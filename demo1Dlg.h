   
// demo1Dlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"
#include<opencv2/opencv.hpp>

// Cdemo1Dlg 对话框
class Cdemo1Dlg : public CDialogEx
{
// 构造
public:
	Cdemo1Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	//定义相机句柄对象
	HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_image;
	
	//HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_imag;


	//MVImage m_csSaveImage;
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	int OnStreamCB1(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();

	void DrawImage1();
	//关闭相机、释放资源
	void OnClose();
	//void connected_components_stat(Mat& image);
	//void pro(Mat& img1); //处理图像
	//void processing(Mat& img);//处理图像

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnStnClickedpic();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSave();
	//afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedPicsave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedpic3();
	//afx_msg void OnBnClickedButton6();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
};
