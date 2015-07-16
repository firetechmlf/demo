#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "stdlib.h"





// CssbsDlg 对话框

class CssbsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CssbsDlg)

public:
	CssbsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CssbsDlg();

// 对话框数据
	enum { IDD = DEMO_SSBS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void SSBS_View_Init();




	DECLARE_MESSAGE_MAP()
public:
	CListCtrl C_SSBS_DATA;
	virtual BOOL OnInitDialog();


private:
	WSADATA ssbs_wsd;
	SOCKET ssbs_socket;
	RECVTHREAD *SSBS_Recv_DATA;
	CWinThread *SSBS_Recv_thread;
	static UINT SSBS_RecvPro(LPVOID pParam);
public:
	afx_msg void OnBnClickedSsbsConnect();
private:
	CButton C_SSBS_CONNECT;
	CIPAddressCtrl C_SSBS_IP;
	CEdit C_SSBS_PORT;
	afx_msg void OnBnClickedSsbsDisconnect();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	static CssbsDlg* test;

private:
	 unsigned char SSBS_RecvBuff[256][6];
	 int SSBS_Save_write = 0;
	 int SSBS_Save_read = 0;
	  int line_number = 0;
	  int recv_message_count = 0;
	  virtual void OnCancel();
	  void OnDrawColorForMyList(NMHDR *pNmHdr, LRESULT *pResult);
	  void send_cmd_to_device(SOCKET socket_to,BYTE buf[6]);
	  void user_dlg_init();
	  void user_enablewindow();
	  void relay_status_display(BYTE data);
	  char* get_alarm_status(BYTE data);
	  char* get_relay_status(BYTE data);
	afx_msg void OnBnClickedDoorLock();
	CButton C_DOOR_LOCK;
	CButton C_110_BACK;
	CButton C_LED_BACK;
	CButton C_ALARM_CTRL;
	CButton C_LED_OUT;
	afx_msg void OnBnClickedSsbsClean();
	CButton C_SSBS_CLEAN;
	afx_msg void OnBnClicked110Back();
	afx_msg void OnBnClickedLedBack();
	afx_msg void OnBnClickedAlarmCtrl();
	afx_msg void OnBnClickedLedOut();
	CButton C_DOOR_CTRL;
	afx_msg void OnBnClickedDoorCtrl();
	afx_msg void OnLvnItemchangedSsbsData(NMHDR *pNMHDR, LRESULT *pResult);
};



struct SSBS_RECVTHREAD
{
	SOCKET recvsock;
	HWND hwnd;
	CssbsDlg *test;
};