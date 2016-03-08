
// ClientDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo.h"
#include "ClientDemoDlg.h"
#include "afxdialogex.h"
#include "..\Server\Server_i.c"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDemoDlg dialog



CClientDemoDlg::CClientDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENTDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	CoInitialize(NULL);


	
	pOPCServer.CoCreateInstance(CLSID_OPCServer);

	if (pOPCServer != NULL)
	{
		LONG TimeBias = 0;
		FLOAT DeadBand = 0.0f;
		OPCHANDLE phServerGroup;
		DWORD UpdateRate;

		IOPCItemMgt *tmpOPCItemMgt;
		pOPCServer->AddGroup(L"Group1", TRUE, 200, 1, &TimeBias, &DeadBand, 0, &phServerGroup, &UpdateRate,IID_IOPCItemMgt, (LPUNKNOWN*)&tmpOPCItemMgt);
		CComPtr<IOPCItemMgt> pOPCItemMgt = tmpOPCItemMgt;
		
		DWORD dwCookie;

		CComModule _Module;
		
		_Module.Init(NULL, GetModuleHandle(NULL));
		

		pSink = new DataCallbackSink;

		
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
			hr = pCP->Advise(pSink, &dwCookie);
		}
		OPCITEMDEF OPCItem = {
			NULL,
			L"ItemY1",
			FALSE,
			0,
			0,
			NULL,
			VT_R8,
			0
		};
		OPCITEMRESULT *OPCItemResult;
		HRESULT *ErrorResult;
		pOPCItemMgt->AddItems(1, &OPCItem, &OPCItemResult, &ErrorResult);
		//pCP->Unadvise(dwCookie);
		//delete pSink;
		//_Module.Term();

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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

