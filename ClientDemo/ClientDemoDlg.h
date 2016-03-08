
// ClientDemoDlg.h : header file
//

#pragma once
#include "iostream"
#include "..\Server\Server_i.h"
#include <exception>
#include "atlcomcli.h" 
#include "DataCallbackSink.h"

// CClientDemoDlg dialog
class CClientDemoDlg : public CDialogEx
{
// Construction
public:
	CClientDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()



private:
	CComPtr<IOPCServer> pOPCServer;
	DataCallbackSink *pSink;
};
