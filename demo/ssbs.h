#pragma once


// CssbsDlg �Ի���

class CssbsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CssbsDlg)

public:
	CssbsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CssbsDlg();

// �Ի�������
	enum { IDD = DEMO_SSBS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
