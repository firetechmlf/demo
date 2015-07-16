// ssbs.cpp : 实现文件
//

#include "stdafx.h"
#include "demo.h"
#include "ssbs.h"
#include "afxdialogex.h"
#include "wchartochar.h"
#include "string.h"
#include "WinSock2.h"

//int SSBS_Save_write = 0;
//int SSBS_Save_read = 0;
//unsigned char SSBS_RecvBuff[256][6];
// CssbsDlg 对话框




IMPLEMENT_DYNAMIC(CssbsDlg, CDialogEx)

CssbsDlg::CssbsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CssbsDlg::IDD, pParent)
{

}

CssbsDlg::~CssbsDlg()
{
}

void CssbsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SSBS_DATA, C_SSBS_DATA);

	//	SSBS_View_Init();
	DDX_Control(pDX, IDC_SSBS_CONNECT, C_SSBS_CONNECT);
	DDX_Control(pDX, IDC_SSBS_IP, C_SSBS_IP);
	DDX_Control(pDX, IDC_SSBS_PORT, C_SSBS_PORT);
	DDX_Control(pDX, IDC_DOOR_LOCK, C_DOOR_LOCK);
	DDX_Control(pDX, IDC_110_BACK, C_110_BACK);
	DDX_Control(pDX, IDC_LED_BACK, C_LED_BACK);
	DDX_Control(pDX, IDC_ALARM_CTRL, C_ALARM_CTRL);
	DDX_Control(pDX, IDC_LED_OUT, C_LED_OUT);
	DDX_Control(pDX, IDC_SSBS_CLEAN, C_SSBS_CLEAN);
	DDX_Control(pDX, IDC_DOOR_CTRL, C_DOOR_CTRL);
}


BEGIN_MESSAGE_MAP(CssbsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SSBS_CONNECT, &CssbsDlg::OnBnClickedSsbsConnect)
	ON_BN_CLICKED(IDC_SSBS_DISCONNECT, &CssbsDlg::OnBnClickedSsbsDisconnect)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SSBS_DATA, OnDrawColorForMyList)
	ON_BN_CLICKED(IDC_DOOR_LOCK, &CssbsDlg::OnBnClickedDoorLock)
	ON_BN_CLICKED(IDC_SSBS_CLEAN, &CssbsDlg::OnBnClickedSsbsClean)
	ON_BN_CLICKED(IDC_110_BACK, &CssbsDlg::OnBnClicked110Back)
	ON_BN_CLICKED(IDC_LED_BACK, &CssbsDlg::OnBnClickedLedBack)
	ON_BN_CLICKED(IDC_ALARM_CTRL, &CssbsDlg::OnBnClickedAlarmCtrl)
	ON_BN_CLICKED(IDC_LED_OUT, &CssbsDlg::OnBnClickedLedOut)
	ON_BN_CLICKED(IDC_DOOR_CTRL, &CssbsDlg::OnBnClickedDoorCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SSBS_DATA, &CssbsDlg::OnLvnItemchangedSsbsData)
END_MESSAGE_MAP()


// CssbsDlg 消息处理程序





void CssbsDlg::SSBS_View_Init()
{
	LONG lStyle;
	lStyle = GetWindowLong(C_SSBS_DATA.m_hWnd,GWL_STYLE);
	lStyle &=~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(C_SSBS_DATA.m_hWnd,GWL_STYLE,lStyle);

	DWORD dwStyle = C_SSBS_DATA.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_SIMPLESELECT;
	C_SSBS_DATA.SetExtendedStyle(dwStyle);



	C_SSBS_DATA.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 40);
	C_SSBS_DATA.InsertColumn(1, _T("SSBS报告"), LVCFMT_CENTER, 200);
	C_SSBS_DATA.InsertColumn(2, _T("主控应答"), LVCFMT_CENTER, 200);
	C_SSBS_DATA.InsertColumn(3, _T("主控命令"), LVCFMT_CENTER, 200);
	C_SSBS_DATA.InsertColumn(4, _T("SSBS应答"), LVCFMT_CENTER, 200);

//	SetDlgItemText(IDC_CMD_STATIC,_T(""));

}

BOOL CssbsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetDlgItemText(IDC_SSBS_PORT, _T("4001"));
	SetDlgItemText(IDC_SSBS_IP, _T("192.68.70.110"));

	SSBS_View_Init();
	user_dlg_init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}




void CssbsDlg::relay_status_display(BYTE data)
{
	if (data & 0x04)
	{
		C_110_BACK.SetCheck(true);
	}
	else
	{
		C_110_BACK.SetCheck(false);
	}
	if (data & 0x08)
	{
		C_LED_OUT.SetCheck(true);
	}
	else
	{
		C_LED_OUT.SetCheck(false);
	}
	if (data & 0x20)
	{
		C_DOOR_LOCK.SetCheck(true);
	}
	else
	{
		C_DOOR_LOCK.SetCheck(false);
	}
	if (data & 0x40)
	{
		C_LED_BACK.SetCheck(true);
	}
	else
	{
		C_LED_BACK.SetCheck(false);
	}
	if (data & 0x80)
	{
		C_ALARM_CTRL.SetCheck(true);
	}
	else
	{
		C_ALARM_CTRL.SetCheck(false);
	}
	if (data & 0x10)
	{
		C_DOOR_CTRL.EnableWindow(false);
	}
	else
	{
		C_DOOR_CTRL.EnableWindow(true);
	}

}

void CssbsDlg::OnBnClickedSsbsConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	int retVal;
	SOCKADDR_IN servAddr;
	SetDlgItemText(IDC_SSBS_CONNECT, _T("正在连接..."));
	UpdateData(true);
	C_SSBS_CONNECT.EnableWindow(false);
	if (WSAStartup(MAKEWORD(2, 2), &ssbs_wsd) != 0)
	{
		SetDlgItemText(IDC_SSBS_CONNECT, _T("WSA 失败"));
		UpdateData(true);
		::MessageBox(m_hWnd, _T("wsa 失败"), _T("请重试"), MB_OK);
		C_SSBS_CONNECT.EnableWindow(true);
		return;
	}
	ssbs_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (ssbs_socket == INVALID_SOCKET)
	{
		SetDlgItemText(IDC_SSBS_CONNECT, _T("sock失败"));
		UpdateData(true);
		::MessageBox(m_hWnd, _T("SOCKET 创建失败"), _T("请重试"), MB_OK);
		C_SSBS_CONNECT.EnableWindow(true);
		WSACleanup();
		return;
	}
	BYTE SSBS_IP[4];
	CString SSBS_PORT;
	unsigned int SSBS_nPort;
	char* SSBS_Port_char;

	C_SSBS_IP.GetAddress(SSBS_IP[0], SSBS_IP[1], SSBS_IP[2], SSBS_IP[3]);
	C_SSBS_PORT.GetWindowTextW(SSBS_PORT);
	servAddr.sin_addr.s_addr = ((unsigned int)(SSBS_IP[3] & 0xff) << 24) | ((unsigned int)(SSBS_IP[2] & 0xff) << 16) | ((unsigned int)(SSBS_IP[1] & 0xff) << 8) | (unsigned int)(SSBS_IP[0] & 0xff);
	servAddr.sin_family = AF_INET;
	USES_CONVERSION;
	SSBS_Port_char = T2A(SSBS_PORT);
	SSBS_nPort = atoi(SSBS_Port_char);
	servAddr.sin_port = htons(SSBS_nPort);
	retVal = connect(ssbs_socket, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (retVal == SOCKET_ERROR)
	{
		SetDlgItemText(IDC_SSBS_CONNECT, _T("connect失败"));
		UpdateData(true);
		::MessageBox(m_hWnd, _T("connect连接失败"), _T("请重试"), MB_OK);
		C_SSBS_CONNECT.EnableWindow(true);
		closesocket(ssbs_socket);
		WSACleanup();
		return;
	}
	SetDlgItemText(IDC_SSBS_CONNECT, _T("连接成功"));
	UpdateData(true);
	user_enablewindow();

	SetTimer(ssbs_socket, 100, NULL);

	SSBS_RECVTHREAD *SSBS_Recv_DATA = new SSBS_RECVTHREAD;
	SSBS_Recv_DATA->hwnd = m_hWnd;
	SSBS_Recv_DATA->recvsock = ssbs_socket;
	SSBS_Recv_DATA->test = this;
	
	
	SSBS_Recv_thread = AfxBeginThread(SSBS_RecvPro, (LPVOID)(SSBS_Recv_DATA));
}


UINT CssbsDlg::SSBS_RecvPro(LPVOID pParam)
{
	SOCKADDR_IN addrfrom;
	long int RecvState = 0;
	int length;
	static int temp, loop,loop_t;
	char recv_buff[RECVDATA_SIZE];
	SOCKET RecvSock = (((SSBS_RECVTHREAD*)pParam)->recvsock);
	HWND HwndRecv = ((SSBS_RECVTHREAD*)pParam)->hwnd;
	CssbsDlg *test = (((SSBS_RECVTHREAD*)pParam)->test);
	delete pParam;
	while (true)
	{
		memset(recv_buff, 0, RECVDATA_SIZE);
		RecvState = recvfrom(RecvSock, recv_buff, RECVDATA_SIZE, 0, (SOCKADDR*)&addrfrom, &length);
		if (RecvState)
		{
			if (!(RecvState % 6))
			{
				temp = RecvState / 6;
				for (loop = 0; loop < temp; loop++)
				{
					for (loop_t = 0; loop_t < 6; loop_t++)
					{
						test->SSBS_RecvBuff[test->SSBS_Save_write][loop_t] = recv_buff[loop_t + loop * 6];
					}
					test->SSBS_Save_write++;
					if (test->SSBS_Save_write>99)
					{
						test->SSBS_Save_write = 0;
					}					
				}
			}
		}
		else
		{
			break;
		}		
	}
	return 0;
}

void CssbsDlg::OnBnClickedSsbsDisconnect()
{

	SetDlgItemText(IDC_SSBS_CONNECT, _T("连接"));
	UpdateData(true);
	C_SSBS_CONNECT.EnableWindow(true);
	closesocket(ssbs_socket);
	WSACleanup();
	KillTimer(ssbs_socket);
	user_dlg_init();
	// TODO:  在此添加控件通知处理程序代码
}


void CssbsDlg::OnTimer(UINT_PTR nIDEvent)
{
	BYTE buf[6];
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	int line;
	int loop;	
	if (nIDEvent == ssbs_socket)
	{
		if (SSBS_Save_write != SSBS_Save_read)
		{
			memset(buf, 0, 6);
			memset(ptr,0,64);
			if ((SSBS_RecvBuff[SSBS_Save_read][0] == 0x02) && (SSBS_RecvBuff[SSBS_Save_read][4] == 0x03) && (
				SSBS_RecvBuff[SSBS_Save_read][5] == (unsigned char)(SSBS_RecvBuff[SSBS_Save_read][0] + SSBS_RecvBuff[SSBS_Save_read][1]
				+ SSBS_RecvBuff[SSBS_Save_read][2] + SSBS_RecvBuff[SSBS_Save_read][3] + SSBS_RecvBuff[SSBS_Save_read][4])))
			{
				 for (loop = 0; loop < 6; loop++)
				{
					buf[loop] = SSBS_RecvBuff[SSBS_Save_read][loop];
				}
				SSBS_Save_read += 1;
				if (SSBS_Save_read>99)
				{
					SSBS_Save_read = 0;
				}
				switch (buf[1])
				{
					case 0xAA:				//常态报警及状态上传
						_itoa_s((recv_message_count + 1), loop_ptr, 10);
						loop_str = loop_ptr;
						line = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
						sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
						loop_str = ptr;
						C_SSBS_DATA.SetItemText(line, 1, (LPCTSTR)loop_str);
						C_SSBS_DATA.EnsureVisible(line, false);
						recv_message_count++;
						relay_status_display(buf[3]);
						break;
					case 0x15:
					case 0x06:		//命令反馈
//						_itoa_s((recv_message_count + 1), loop_ptr, 10);
//						loop_str = loop_ptr;
//						line = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
						sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
						loop_str = ptr;
						C_SSBS_DATA.SetItemText(line_number, 4, (LPCTSTR)loop_str);
						C_SSBS_DATA.EnsureVisible(line_number, false);
//						recv_message_count++;
						break;
					case 0x05:		//2分钟同步
						_itoa_s((recv_message_count + 1), loop_ptr, 10);
						loop_str = loop_ptr;
						line = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
						sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
						loop_str = ptr;
						C_SSBS_DATA.SetItemText(line, 1, (LPCTSTR)loop_str);
						buf[1] = 0x06;
						buf[5] += 0x01;
						send_cmd_to_device(ssbs_socket, buf);
						sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
						loop_str = ptr;
						C_SSBS_DATA.SetItemText(line, 2, (LPCTSTR)loop_str);
						C_SSBS_DATA.EnsureVisible(line, false);
						recv_message_count++;
						relay_status_display(buf[3]);
						break;
					default:
						break;
						
				}
			}	
		}
	}
}

void CssbsDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	closesocket(ssbs_socket);
	WSACleanup();
	KillTimer(ssbs_socket);
	CDialogEx::OnCancel();
}
	
/*修改LISTCTRL控件单行颜色*/
void CssbsDlg::OnDrawColorForMyList(NMHDR *pNmHdr, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNmHdr);

	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF   clrNewBkColor;

		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);




		//设置背景色
		if (nItem % 2 == 0)
		{
			clrNewBkColor = RGB(240, 240, 240); //偶数行背景色为灰色
		}
		else
		{
			clrNewBkColor = RGB(255, 255, 255); //奇数行背景色为白色
		}

		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}

void CssbsDlg::OnBnClickedDoorLock()
{
	BYTE CMD_buf[6] = {0x02,0x55,0x00,0x00,0x03,0x5A};
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];

	if (C_DOOR_LOCK.GetCheck())
	{
		//发送锁门信号
		CMD_buf[2] = 0x06;
		CMD_buf[3] = 0x11;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	}
	else
	{
		//::MessageBox(m_hWnd, _T("未选中"), _T("未选中"), MB_OK);
		//取消锁门
		CMD_buf[2] = 0x06;
		CMD_buf[3] = 0x13;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];

	}
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	send_cmd_to_device(ssbs_socket, CMD_buf);
}


void CssbsDlg::OnBnClickedSsbsClean()
{
	// TODO:  在此添加控件通知处理程序代码
	int loop;
	for (loop = 0; loop < 256; loop++)
	{
		memset(SSBS_RecvBuff[loop], 0, 6);
	}
	SSBS_Save_write = 0;
	SSBS_Save_read = 0;
	line_number = 0;
	recv_message_count = 0;
	C_SSBS_DATA.DeleteAllItems();
	C_DOOR_LOCK.SetCheck(false);
	C_110_BACK.SetCheck(false);
	C_LED_BACK.SetCheck(false);
	C_LED_OUT.SetCheck(false);
	C_ALARM_CTRL.SetCheck(false);

}


void CssbsDlg::send_cmd_to_device(SOCKET socket_to, BYTE buf[6])
{
	int report;
	report = send(socket_to,(char*)buf,6,0);
	if (report == SOCKET_ERROR)
	{
		::MessageBox(m_hWnd, _T("发送失败,请连接设备"), _T("无连接"), MB_OK);
	}
}


void CssbsDlg::user_dlg_init()
{
	C_DOOR_LOCK.EnableWindow(false);
	C_110_BACK.EnableWindow(false);
	C_ALARM_CTRL.EnableWindow(false);
	C_LED_BACK.EnableWindow(false);
	C_LED_OUT.EnableWindow(false);
	C_SSBS_CLEAN.EnableWindow(false);
	C_DOOR_CTRL.EnableWindow(false);
}

void CssbsDlg::user_enablewindow()
{
	C_DOOR_LOCK.EnableWindow(true);
	C_110_BACK.EnableWindow(true);
	C_ALARM_CTRL.EnableWindow(true);
	C_LED_BACK.EnableWindow(true);
	C_LED_OUT.EnableWindow(true);
	C_SSBS_CLEAN.EnableWindow(true);
	C_DOOR_CTRL.EnableWindow(true);
}

void CssbsDlg::OnBnClicked110Back()
{
	// TODO:  110反馈继电器控制
	
	BYTE CMD_buf[6] = { 0x02, 0x55, 0x00, 0x00, 0x03, 0x5A };
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	if (C_110_BACK.GetCheck())
	{
		//发送锁门信号
		CMD_buf[2] = 0x03;
		CMD_buf[3] = 0x11;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	}
	else
	{
		//::MessageBox(m_hWnd, _T("未选中"), _T("未选中"), MB_OK);
		//取消锁门
		CMD_buf[2] = 0x03;
		CMD_buf[3] = 0x13;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];

	}
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	send_cmd_to_device(ssbs_socket, CMD_buf);
}


void CssbsDlg::OnBnClickedLedBack()
{
	// TODO:  开灯反馈

	BYTE CMD_buf[6] = { 0x02, 0x55, 0x00, 0x00, 0x03, 0x5A };
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	if (C_LED_BACK.GetCheck())
	{
		//发送锁门信号
		CMD_buf[2] = 0x07;
		CMD_buf[3] = 0x11;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	}
	else
	{
		//::MessageBox(m_hWnd, _T("未选中"), _T("未选中"), MB_OK);
		//取消锁门
		CMD_buf[2] = 0x07;
		CMD_buf[3] = 0x13;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];

	}
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	send_cmd_to_device(ssbs_socket, CMD_buf);
}


void CssbsDlg::OnBnClickedAlarmCtrl()
{
	// TODO:  在此添加控件通知处理程序代码
	BYTE CMD_buf[6] = { 0x02, 0x55, 0x00, 0x00, 0x03, 0x5A };
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	if (C_ALARM_CTRL.GetCheck())
	{
		//发送锁门信号
		CMD_buf[2] = 0x08;
		CMD_buf[3] = 0x11;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	}
	else
	{
		//::MessageBox(m_hWnd, _T("未选中"), _T("未选中"), MB_OK);
		//取消锁门
		CMD_buf[2] = 0x08;
		CMD_buf[3] = 0x13;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];

	}
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	send_cmd_to_device(ssbs_socket, CMD_buf);
}


void CssbsDlg::OnBnClickedLedOut()
{
	// TODO:  在此添加控件通知处理程序代码
	BYTE CMD_buf[6] = { 0x02, 0x55, 0x00, 0x00, 0x03, 0x5A };
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	if (C_LED_OUT.GetCheck())
	{
		//发送锁门信号
		CMD_buf[2] = 0x04;
		CMD_buf[3] = 0x11;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	}
	else
	{
		//::MessageBox(m_hWnd, _T("未选中"), _T("未选中"), MB_OK);
		//取消锁门
		CMD_buf[2] = 0x04;
		CMD_buf[3] = 0x13;
		CMD_buf[5] += CMD_buf[2] + CMD_buf[3];

	}
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	send_cmd_to_device(ssbs_socket, CMD_buf);
}


void CssbsDlg::OnBnClickedDoorCtrl()
{
	// TODO:  在此添加控件通知处理程序代码
	BYTE CMD_buf[6] = { 0x02, 0x55, 0x00, 0x00, 0x03, 0x5A };
	char loop_ptr[10];
	CString loop_str;
	char ptr[64];
	CMD_buf[2] = 0x05;
	CMD_buf[3] = 0x11;
	CMD_buf[5] += CMD_buf[2] + CMD_buf[3];
	_itoa_s((recv_message_count + 1), loop_ptr, 10);
	loop_str = loop_ptr;
	line_number = C_SSBS_DATA.InsertItem(recv_message_count + 1, (LPCTSTR)loop_str);
	sprintf_s(ptr, "%02X  %02X  %02X  %02X  %02X  %02X", CMD_buf[0], CMD_buf[1], CMD_buf[2], CMD_buf[3], CMD_buf[4], CMD_buf[5]);
	loop_str = ptr;
	C_SSBS_DATA.SetItemText(line_number, 3, (LPCTSTR)loop_str);
	C_SSBS_DATA.EnsureVisible(line_number, false);
	recv_message_count++;
	C_DOOR_CTRL.EnableWindow(false);
	send_cmd_to_device(ssbs_socket, CMD_buf);

}

char* CssbsDlg::get_relay_status(BYTE data)
{
	char ptr[128] = { 0 };
	char *return_ptr = (char*)malloc(128);
	memset(return_ptr, 0, 128);
	strcat_s(ptr, "输出状态：");
	if (data)
	{
		if (data & 0x04)
		{
			strcat_s(ptr, "110反馈输出;");
		}
		if (data & 0x08)
		{
			strcat_s(ptr, "联动灯输出;");
		}
		if (data & 0x10)
		{
			strcat_s(ptr, "门打开;");
		}
		if (data & 0x20)
		{
			strcat_s(ptr, "门禁锁定，进门按钮无效;");
		}
		if (data & 0x40)
		{
			strcat_s(ptr, "联动灯输出反馈;");
		}
		if (data & 0x80)
		{
			strcat_s(ptr, "设备警号输出;");
		}

		strcat_s(ptr, "\r\n");
	}
	else
	{
		strcat_s(ptr,"输出无动作！\r\n");
	}
	memcpy(return_ptr, ptr, 128);
	return return_ptr;
}

char* CssbsDlg::get_alarm_status(BYTE data)
{
	char ptr[128] = {0};
	char *return_ptr = (char*)malloc(128);
	memset(return_ptr, 0, 128);
	strcat_s(ptr, "输入状态：");
	if (data)
	{
		if (data & 0x04)
		{
			strcat_s(ptr, "进门信号报警；");
		}
		if (data & 0x08)
		{
			strcat_s(ptr, "出门信号报警；");
		}
		if (data & 0x10)
		{
			strcat_s(ptr, "紧急信号报警；");
		}
		if (data & 0x20)
		{
			strcat_s(ptr, "ATM有人靠近信号报警；");
		}
		if (data & 0x40)
		{
			strcat_s(ptr, "加钞间探头报警；");
		}
		if (data & 0x80)
		{
			strcat_s(ptr, "110报警主机报警；");
		}
		strcat_s(ptr, "\r\n");
	}
	else
	{
		strcat_s(ptr, "设备无报警输入；\r\n");
	}
	memcpy(return_ptr,ptr,128);
	return return_ptr;
}




void CssbsDlg::OnLvnItemchangedSsbsData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码


	DWORD dwpos = GetMessagePos();
	CPoint point(LOWORD(dwpos),HIWORD(dwpos));

	C_SSBS_DATA.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	
	int nItem = C_SSBS_DATA.SubItemHitTest(&lvinfo);
	if (nItem != -1)
	{
		CString str_cmd;
		static char ptr[512];
		CString str;

		wchar_t *buf_wchar;
		char* buf_char;
		BYTE *buf_byte;
		CUser u;
		memset(ptr,0,512);
		str = C_SSBS_DATA.GetItemText(lvinfo.iItem, lvinfo.iSubItem);	
		buf_wchar = str.GetBuffer(str.GetLength());
		buf_char = u.WcharToChar(buf_wchar);
		buf_byte = u.CharToBYTE(buf_char);
		SetDlgItemText(IDC_CMD_STATIC, str);

		switch (buf_byte[1])
		{
		case 0xaa:
			strcat_s(ptr,"设备报警：\r\n ");
			strcat_s(ptr, get_alarm_status(buf_byte[2]));
			strcat_s(ptr, get_relay_status(buf_byte[3]));
			break;
		case 0x55:
			strcat_s(ptr, "主控命令：\r\n");
			switch (buf_byte[2])
			{
			case 0x03:  //relay 1
				strcat_s(ptr, "110反馈输出");
				break;
			case 0x04:		//relay2
				strcat_s(ptr, "联动灯输出");
				break;
			case 0x05:		//open door
				strcat_s(ptr, "门");
				break;
			case 0x06:		//lock door
				strcat_s(ptr, "锁门命令");
				break;
			case 0x07:		//relay3
				strcat_s(ptr, "开灯反馈输出");
				break;
			case 0x08:		//alarm out
				strcat_s(ptr, "警号输出");
				break;
			}
			if (buf_byte[3] == 0x13)
			{
				strcat_s(ptr, "关闭；\r\n");
			}
			else if (buf_byte[3] == 0x11)
			{
				strcat_s(ptr, "打开；\r\n");
			}
			break;
		case 0x06:
			if (buf_byte[2] != 0x88)
			{
				strcat_s(ptr, "ssbs对主控命令执行反馈；\r\n");
				switch (buf_byte[2])
				{
				case 0x03:  //relay 1
					strcat_s(ptr, "110反馈输出");
					break;
				case 0x04:		//relay2
					strcat_s(ptr, "联动灯输出");
					break;
				case 0x05:		//open door
					strcat_s(ptr, "门");
					break;
				case 0x06:		//lock door
					strcat_s(ptr, "锁门命令");
					break;
				case 0x07:		//relay3
					strcat_s(ptr, "开灯反馈输出");
					break;
				case 0x08:		//alarm out
					strcat_s(ptr, "警号输出");
					break;
				}
				if (buf_byte[3] == 0x13)
				{
					strcat_s(ptr, "关闭成功；\r\n");
				}
				else if (buf_byte[3] == 0x11)
				{
					strcat_s(ptr, "打开成功；\r\n");
				}
			}
			else if (buf_byte[2] == 0x88)
			{
				strcat_s(ptr, "主控状态反馈；\r\n");
			}
			break;
		case 0x05:
			strcat_s(ptr, "SSBS状态反馈；\r\n");
			break;
		default:
			strcat_s(ptr, "请选择正确数据：\r\n ");
			break;
		}



		str_cmd = ptr;
		SetDlgItemText(IDC_STATUS_STATIC, str_cmd);

	}

	*pResult = 0;
}
