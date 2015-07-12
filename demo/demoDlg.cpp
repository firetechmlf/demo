
// demoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "ssbs.h"
#include "string"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdemoDlg 对话框



CdemoDlg::CdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	device_queue = 0;
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_VIEW, C_DEVICE_VIEW);

	DDX_Control(pDX, IDC_TARGET_CONNECT_BUTTON, C_TARGET_CONNECT_BUTTON);
	DDX_Control(pDX, IDC_TARGET_IP, C_TARGET_IP);
	DDX_Control(pDX, IDC_TARGET_PORT, C_TARGET_PORT);
	DDX_Control(pDX, IDC_TARGET_CLOSE_BUTTON, C_TARGET_CLOSE_BUTTON);
	DDX_Control(pDX, IDC_FIND_BUTTON, C_FIND_BUTTON);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TARGET_CONNECT_BUTTON, &CdemoDlg::OnBnClickedTargetConnectButton)
	ON_BN_CLICKED(IDC_TARGET_CLOSE_BUTTON, &CdemoDlg::OnBnClickedTargetCloseButton)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CdemoDlg::OnBnClickedFindButton)
	ON_BN_CLICKED(IDC_SSBS_DEMO, &CdemoDlg::OnBnClickedSsbsDemo)
END_MESSAGE_MAP()


// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetDlgItemText(IDC_TARGET_PORT, _T("4002"));
	SetDlgItemText(IDC_TARGET_IP,_T("192.168.0.213"));

	View_Init();
	UDP_Init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdemoDlg::OnPaint()
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
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdemoDlg::View_Init()
{
	
	LONG lStyle;
	lStyle = GetWindowLong(C_DEVICE_VIEW.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(C_DEVICE_VIEW.m_hWnd, GWL_STYLE,lStyle);
	
	DWORD dwStyle = C_DEVICE_VIEW.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	C_DEVICE_VIEW.SetExtendedStyle(dwStyle);

	C_DEVICE_VIEW.InsertColumn(0,_T("序号"),LVCFMT_CENTER,40);
	C_DEVICE_VIEW.InsertColumn(1, _T("设备类型"), LVCFMT_CENTER, 120);
	C_DEVICE_VIEW.InsertColumn(2, _T("IP"), LVCFMT_CENTER, 100);
	C_DEVICE_VIEW.InsertColumn(3, _T("网关"), LVCFMT_CENTER, 100);
	C_DEVICE_VIEW.InsertColumn(4, _T("掩码"), LVCFMT_CENTER, 100);
	C_DEVICE_VIEW.InsertColumn(5, _T("数据端口"), LVCFMT_CENTER, 60);
	C_DEVICE_VIEW.InsertColumn(6, _T("配置端口"), LVCFMT_CENTER, 60);
	C_DEVICE_VIEW.InsertColumn(7, _T("软件版本"), LVCFMT_CENTER, 235);
	C_DEVICE_VIEW.InsertColumn(8, _T("IAP版本"), LVCFMT_CENTER, 235);
}
UINT CdemoDlg::RecvPro(LPVOID pParam)
{
	SOCKADDR_IN addrfrom;
	long int RecvState = 0;
	int length;
	char recv_buff[RECVDATA_SIZE];
	char* ptr;
	SOCKET RecvSock = (((RECVTHREAD*)pParam)->recvsock);
	HWND HwndRecv = ((RECVTHREAD*)pParam)->hwnd;
	delete pParam;
	while (true)
	{
		memset(recv_buff,0,RECVDATA_SIZE);
		RecvState = recvfrom(RecvSock, recv_buff, RECVDATA_SIZE, 0, (SOCKADDR*)&addrfrom, &length);
		if (!RecvState)
		{
			break;
		}
		ptr = strstr(recv_buff,"Send_Device_Infor");
		if (ptr)
		{
			//MessageBox.show(_T("你好"));
			::MessageBox(HwndRecv, _T("收到数据"), _T("收到数据"), MB_OK);
		}
	}
	return 0;
}


void CdemoDlg::OnBnClickedTargetConnectButton()
{
	// TODO:  在此添加控件通知处理程序代码
	int retVal;
	SOCKADDR_IN  servAddr;
	SetDlgItemText(IDC_TARGET_CONNECT_BUTTON,_T("正在连接..."));
	UpdateData(true);
	C_TARGET_CONNECT_BUTTON.EnableWindow(FALSE);
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		SetDlgItemText(IDC_TARGET_CONNECT_BUTTON,_T("WSA 失败"));
		UpdateData(true);
		C_TARGET_CONNECT_BUTTON.EnableWindow(true);
		return;
	}
	sHost = socket(AF_INET, SOCK_STREAM, 6);
	if (sHost == INVALID_SOCKET)
	{
		SetDlgItemText(IDC_TARGET_CLOSE_BUTTON,_T("sock 失败"));
		UpdateData(true);
		C_TARGET_CONNECT_BUTTON.EnableWindow(true);
		WSACleanup();
		return;
	}
	BYTE ip[4];
	CString PORT;
	unsigned int nPort;
	char* Port_char;
	C_TARGET_IP.GetAddress(ip[0],ip[1],ip[2],ip[3]);
	C_TARGET_PORT.GetWindowTextW(PORT);
	servAddr.sin_addr.s_addr = ((unsigned int)(ip[3] & 0xff) << 24) | ((unsigned int)(ip[2] & 0xff) << 16) | ((unsigned int)(ip[1] & 0xff) << 8) | (unsigned int)(ip[0] & 0xff);	
	servAddr.sin_family = AF_INET;
	USES_CONVERSION;
	Port_char = T2A(PORT);
	nPort = atoi(Port_char);
	servAddr.sin_port = htons(nPort);
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (retVal == SOCKET_ERROR)
	{
		SetDlgItemText(IDC_TARGET_CONNECT_BUTTON, _T("connect失败"));
		C_TARGET_CONNECT_BUTTON.EnableWindow(true);
		UpdateData(true);
		closesocket(sHost);
		WSACleanup();
		return;
	}
	SetDlgItemText(IDC_TARGET_CONNECT_BUTTON, _T("连接成功"));
	UpdateData(true);

	RECVTHREAD *PrecvData = new RECVTHREAD;
	PrecvData->recvsock = sHost;
	PrecvData->hwnd = m_hWnd;

	recv_thread = AfxBeginThread(RecvPro, (LPVOID)PrecvData);

}


void CdemoDlg::OnBnClickedTargetCloseButton()
{
	
	// TODO:  在此添加控件通知处理程序代码
	C_TARGET_CONNECT_BUTTON.EnableWindow(true);
	closesocket(sHost);
	WSACleanup();
}

void CdemoDlg::UDP_Init()
{
	WSADATA wsadata_udp;
	SOCKADDR_IN localAddr;
	int err;
	bool broad =1;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata_udp) != 0)
	{
		SetDlgItemText(IDC_FIND_BUTTON,_T("WSA失败"));
		UpdateData(true);
		C_FIND_BUTTON.EnableWindow(FALSE);
		return;
	}
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(8000);
	localAddr.sin_addr.s_addr = INADDR_ANY;

	remotAddr.sin_family = AF_INET;
	remotAddr.sin_addr.s_addr = INADDR_BROADCAST;
	remotAddr.sin_port = htons(8000);
	

//	localAddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);//INADDR_BROADCAST;


	udpsocket = socket(AF_INET, SOCK_DGRAM, 0);

	setsockopt(udpsocket,SOL_SOCKET,SO_BROADCAST,(const char *)&broad,sizeof(broad));

	if (udpsocket == INVALID_SOCKET)
	{
		WSACleanup();
		SetDlgItemText(IDC_FIND_BUTTON,_T("socket err"));
		UpdateData(true);
		C_FIND_BUTTON.EnableWindow(FALSE);
		return;
	}
	
	err = bind(udpsocket, (struct sockaddr*)&localAddr, sizeof(SOCKADDR_IN));
	if (err == SOCKET_ERROR)
	{
		err = closesocket(udpsocket);
		WSACleanup();
		SetDlgItemText(IDC_FIND_BUTTON, _T("bind err"));
		UpdateData();
		C_FIND_BUTTON.EnableWindow(FALSE);
		return;
	}


	RECVTHREAD *UDP_Precvdata = new RECVTHREAD;
	UDP_Precvdata->recvsock = udpsocket;
	UDP_Precvdata->hwnd = m_hWnd;

	udp_recv_thread = AfxBeginThread(udp_recvpro,(LPVOID)UDP_Precvdata);
}



UINT CdemoDlg::udp_recvpro(LPVOID pParam)
{
	long int RecvState = 0;
	int length = sizeof(SOCKADDR);
	
	char UDP_buff[RECVDATA_SIZE];
	char* ptr_UDP;
	SOCKET UDP_RecvSock = (((RECVTHREAD*)pParam)->recvsock);
	HWND HwndRecv = ((RECVTHREAD*)pParam)->hwnd;
	delete pParam;
	SOCKADDR_IN addrfrom;
	
	addrfrom.sin_family = AF_INET;
	addrfrom.sin_addr.s_addr = INADDR_BROADCAST;
	addrfrom.sin_port = htons(8000);
	while (true)
	{
		
		memset(UDP_buff, 0, RECVDATA_SIZE);
		RecvState = recvfrom(UDP_RecvSock, UDP_buff, RECVDATA_SIZE, 0, ( SOCKADDR FAR*)&addrfrom,(int far*) &length);
		if (RecvState>0)
		{
			ptr_UDP = strstr(UDP_buff, "Send_Device_Infor");
			if (ptr_UDP)
			{
				//MessageBox.show(_T("你好"));
				::MessageBox(HwndRecv, _T("udp收到数据"), _T("收到数据"), MB_OK);
			}
		}		
	}
	return 0;
}

void CdemoDlg::OnBnClickedFindButton()
{
	// TODO:  在此添加控件通知处理程序代码
	char buf[256] = "IPSerch_Form_PC";
	int length = sizeof(SOCKADDR);
	sendto(udpsocket,buf,strlen(buf)+1,0,(SOCKADDR FAR*)&remotAddr,length);
	ZeroMemory(buf, 256);
}


void CdemoDlg::OnBnClickedSsbsDemo()
{
	// TODO:  在此添加控件通知处理程序代码
	CssbsDlg *dlg = new CssbsDlg;
	dlg->Create(DEMO_SSBS, NULL);
	dlg->ShowWindow(SW_SHOW);
}

