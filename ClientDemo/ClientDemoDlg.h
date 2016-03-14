
// ClientDemoDlg.h : header file
//

#ifndef ClientDemoDlg_H
#define ClientDemoDlg_H
#include "stdafx.h"
#include <iostream>
#include "..\Server\Server_i.h"
#include <exception>
#include <list>
#include "atlcomcli.h" 
#include "DataCallbackSink.h"


// CClientDemoDlg dialog
class CClientDemoDlg : public CDialogEx
{
// Construction
public:
	CClientDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CClientDemoDlg();

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
	enum { ID_ItemY1 = 0, ID_ItemY2, ID_ItemY3};
	CComPtr<IOPCServer> m_pOPCServer;
	DataCallbackSink *m_pSink;
	CWnd *m_pDrawWnd[3];
	CBrush m_CbrushBlack= RGB(0,0,0); 
	CPen GreenPen= { PS_SOLID,1,RGB(0,128,0) };
	CPen BluePen= { PS_SOLID, 1, RGB(15, 117, 255) };
	CClientDC *m_pDrawWindDC[3];

	RECT m_DrawRect[3];
	int m_iZeroPoint[3];
	double y_k[3];
	double x_k[3];
	CDC DrawBufferDC[3];
	CBitmap DrawMap[3];
	std::list<double> m_wave[3];
	
	

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DrawPic(int i);
};
#endif