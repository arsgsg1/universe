
// myspyDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "myspy.h"
#include "myspyDlg.h"
#include "afxdialogex.h"
#include <vector>

std::vector<CString> gWindowList;
std::vector<CString> gClassList;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR windowName[MAX_PATH] = { 0, };
	TCHAR className[MAX_PATH] = { 0, };
	if (!GetWindowText(hwnd, windowName, sizeof(windowName)) &&
		GetLastError() != ERROR_SUCCESS)
		return FALSE;

	if (!GetClassName(hwnd, className, sizeof(className)) &&
		GetLastError() != ERROR_SUCCESS)
		return FALSE;

	//작업관리자는 화면에 표시되는 top윈도우 중에서 다음 속성을 만족하는 것을 열거한다.
	//WS_EX_APPWINDOW 속성을 가지고 있다. 
	//WS_EX_TOOLWINDOW 속성을 가지고 있지 않으면서, 소유주(owner) 윈도우가 없다. 
	DWORD exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	BOOL isVisible = IsWindowVisible(hwnd);
	BOOL isToolWindow = (exStyle & WS_EX_TOOLWINDOW);
	BOOL isAppWindow = (exStyle & WS_EX_APPWINDOW);
	BOOL isOwned = GetWindow(hwnd, GW_OWNER) ? TRUE : FALSE;

	

	if (isVisible && (isAppWindow || (!isToolWindow && !isOwned)))
	{
		// 발견한 윈도우와 클래스들을 넣는다.
		gWindowList.push_back((LPCTSTR)windowName);
		gClassList.push_back((LPCTSTR)className);
	}


	//EnumChildWindows(hwnd, EnumWindowsProc, lParam + 4);
	return TRUE;
}
// CmyspyDlg 대화 상자



CmyspyDlg::CmyspyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYSPY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmyspyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_LIST, mProcessList);
}

BEGIN_MESSAGE_MAP(CmyspyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CmyspyDlg 메시지 처리기

BOOL CmyspyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetLastError(0);
	EnumWindows(EnumWindowsProc, 0);
	for (int i = 0; i < gWindowList.size(); i++) {
		mProcessList.AddString(gWindowList[i]);
		mProcessList.AddString(gClassList[i]);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CmyspyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CmyspyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmyspyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

