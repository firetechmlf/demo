#pragma once


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

	DECLARE_MESSAGE_MAP()
};
