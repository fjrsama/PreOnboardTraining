 

// ClientDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "ClientDemoDlg.h"
#include "afxdialogex.h"
#include "..\Server\Server_i.c"
#include "Resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDemoDlg dialog



CClientDemoDlg::CClientDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENTDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CClientDemoDlg::~CClientDemoDlg()
{
	delete m_pSink;
}

void CClientDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CClientDemoDlg message handlers

BOOL CClientDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	
	m_pOPCServer.CoCreateInstance(CLSID_OPCServer);

	if (m_pOPCServer != NULL)
	{
		LONG TimeBias = 0;
		FLOAT DeadBand = 0.0f;
		OPCHANDLE phServerGroup;
		DWORD UpdateRate;

		IOPCItemMgt *tmpOPCItemMgt;
		m_pOPCServer->AddGroup(L"Group1", TRUE, 200, 1, &TimeBias, &DeadBand, 0, &phServerGroup, &UpdateRate,IID_IOPCItemMgt, (LPUNKNOWN*)&tmpOPCItemMgt);
		CComPtr<IOPCItemMgt> pOPCItemMgt = tmpOPCItemMgt;
		
		DWORD dwCookie;
		m_pSink = new DataCallbackSink(this->m_wave,this);
		IConnectionPoint *pCP = NULL;
		IConnectionPointContainer *pCPC = NULL;
		HRESULT hr = pOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);
		if (pCPC != NULL)
		{
			hr = pCPC->FindConnectionPoint(IID_IOPCDataCallback, &pCP);
			pCPC->Release();
		}

		if (pCP != NULL)
		{
			hr = pCP->Advise(m_pSink, &dwCookie);
		}
		OPCITEMDEF OPCItem[] = {
			{
			NULL,
			L"ItemY1",
			FALSE,
			0,
			0,
			NULL,
			VT_R8,
			0
			},
			{
				NULL,
				L"ItemY2",
			FALSE,
			0,
			0,
			NULL,
			VT_R8,
			0
			},
			{
				NULL,
				L"ItemY3",
			FALSE,
			0,
			0,
			NULL,
			VT_R8,
			0
			}
		};
		OPCITEMRESULT *OPCItemResult;
		HRESULT *ErrorResult;
		pOPCItemMgt->AddItems(3, OPCItem, &OPCItemResult, &ErrorResult);

		//初始化绘图
		m_pDrawWnd[ID_ItemY1] = GetDlgItem(IDC_STATIC1);
		m_pDrawWnd[ID_ItemY2] = GetDlgItem(IDC_STATIC2);
		m_pDrawWnd[ID_ItemY3] = GetDlgItem(IDC_STATIC3);

		m_pDrawWindDC[ID_ItemY1] = new CClientDC(m_pDrawWnd[ID_ItemY1]);
		m_pDrawWindDC[ID_ItemY2] = new CClientDC(m_pDrawWnd[ID_ItemY2]);
		m_pDrawWindDC[ID_ItemY3] = new CClientDC(m_pDrawWnd[ID_ItemY3]);

		m_pDrawWnd[ID_ItemY1]->GetClientRect(m_DrawRect + ID_ItemY1);
		m_pDrawWnd[ID_ItemY2]->GetClientRect(m_DrawRect + ID_ItemY2);
		m_pDrawWnd[ID_ItemY3]->GetClientRect(m_DrawRect + ID_ItemY3);

		m_iZeroPoint[ID_ItemY1] = m_DrawRect[ID_ItemY1].bottom / 2;
		m_iZeroPoint[ID_ItemY2] = m_DrawRect[ID_ItemY2].bottom / 2;
		m_iZeroPoint[ID_ItemY3] = m_DrawRect[ID_ItemY3].bottom / 2;

		y_k[ID_ItemY1] = 0.6;
		y_k[ID_ItemY2] = 0.6;
		y_k[ID_ItemY3] = 0.6;
		x_k[ID_ItemY1] = 5;
		x_k[ID_ItemY2] = 5;
		x_k[ID_ItemY3] = 5;

		for (int i = ID_ItemY1; i <= ID_ItemY3; ++i)
		{
			CRect tmpCRect(m_DrawRect[i]);
			DrawBufferDC[i].CreateCompatibleDC(m_pDrawWindDC[i]);
			DrawMap[i].CreateCompatibleBitmap(m_pDrawWindDC[i], tmpCRect.Width(), tmpCRect.Height());
		}

	}
	else
	{
		std::cout << "failed to connect to the OPC server";
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawPic(ID_ItemY1);
		DrawPic(ID_ItemY2);
		DrawPic(ID_ItemY3);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CClientDemoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		{
			if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
				return TRUE;
			break; 
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CClientDemoDlg::DrawPic(int i)
{
	CPen* pOldPen;
	CBitmap *pOldMap = DrawBufferDC[i].SelectObject(DrawMap + i);

	DrawBufferDC[i].FillRect(m_DrawRect + i, &m_CbrushBlack);
	pOldPen = DrawBufferDC[i].SelectObject(&GreenPen);
	DrawBufferDC[i].MoveTo(0, m_iZeroPoint[i]);
	DrawBufferDC[i].LineTo((m_DrawRect[i].right),m_iZeroPoint[i]);

	
	DrawBufferDC[i].SelectObject(&BluePen);


	double BeginAxis = x_k[i] * m_wave[i].size();

	if (BeginAxis >= m_DrawRect[i].right)BeginAxis = m_DrawRect[i].right;

	
	double x = BeginAxis;

	if(m_wave[i].size()>1)
	{
		auto iter = m_wave[i].rbegin();
		double y = *iter;

		for (; x >2 * x_k[i] ; x -= x_k[i] )
		{
			double _y = *(++iter);
			DrawBufferDC[i].MoveTo(x, -y*y_k[i] + m_iZeroPoint[i]);
			DrawBufferDC[i].LineTo(x - x_k[i], -y*y_k[i] + m_iZeroPoint[i]);
			DrawBufferDC[i].MoveTo(x - x_k[i], -y*y_k[i] + m_iZeroPoint[i]);
			DrawBufferDC[i].LineTo(x - x_k[i], -_y*y_k[i] + m_iZeroPoint[i]);
			y = _y;
		}
	}
	DrawBufferDC[i].SelectObject(&GreenPen);
	double j;
	for (j = m_iZeroPoint[i]; j >= m_DrawRect[i].top; j -= ((int)(y_k[i] * 20)))
	{
		DrawBufferDC[i].MoveTo(0, j);
		DrawBufferDC[i].LineTo(6, j);
	}
	for (j = m_iZeroPoint[i]; j < m_DrawRect[i].bottom - 6; j += ((int)(y_k[i] * 20)))
	{
		DrawBufferDC[i].MoveTo(0, j);
		DrawBufferDC[i].LineTo(6, j);
	}
	for (j = m_DrawRect[i].right; j > 6; j -= (int)(x_k[i] * 20))
	{
		DrawBufferDC[i].MoveTo(j, m_DrawRect[i].bottom);
		DrawBufferDC[i].LineTo(j, m_DrawRect[i].bottom - 6);
	}
	DrawBufferDC[i].SelectObject(pOldPen);
	m_pDrawWindDC[i]->BitBlt(m_DrawRect[i].left, m_DrawRect[i].top, CRect(m_DrawRect[i]).Width(), CRect(m_DrawRect[i]).Height(),
		DrawBufferDC + i, 0, 0, SRCCOPY);
	DrawBufferDC[i].SelectObject(pOldMap);
}