
// demoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "winsock2.h"




// CdemoDlg 对话框
class CdemoDlg : public CDialogEx
{
// 构造
public:
	CdemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = DEMO_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void View_Init();

// 实现
protected:
	HICON m_hIcon;
	CListCtrl C_DEVICE_VIEW;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMainTest();
	afx_msg void OnBnClickedButton4();
private:

	WSADATA wsd;
	SOCKET sHost;
	RECVTHREAD *PrecvData;
	CWinThread *recv_thread;
	static UINT RecvPro(LPVOID pParam);

public:
	afx_msg void OnBnClickedTargetConnectButton();


	CIPAddressCtrl C_TARGET_IP;
private:
	CButton C_TARGET_CONNECT_BUTTON;	
	CEdit C_TARGET_PORT;
	CButton C_TARGET_CLOSE_BUTTON;
public:
	afx_msg void OnBnClickedTargetCloseButton();



private:			//UDP
	int device_queue;
	CWinThread *udp_recv_thread;
	HANDLE udp_hMutex;
	SOCKET udpsocket;
	RECVTHREAD *UDP_Precvdata;

	SOCKADDR_IN remotAddr;

	static UINT udp_recvpro(LPVOID pParam);
	void UDP_Init();
public:
	CButton C_FIND_BUTTON;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnBnClickedSsbsDemo();
};
