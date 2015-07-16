
// demoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "winsock2.h"




// CdemoDlg �Ի���
class CdemoDlg : public CDialogEx
{
// ����
public:
	CdemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = DEMO_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void View_Init();

// ʵ��
protected:
	HICON m_hIcon;
	CListCtrl C_DEVICE_VIEW;
	// ���ɵ���Ϣӳ�亯��
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
