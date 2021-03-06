
// myspyDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <Windows.h>

// CmyspyDlg 대화 상자
class CmyspyDlg : public CDialog
{
// 생성입니다.
public:
	CmyspyDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//2017-11-22 사용자 정의 함수
public:
	CListBox mProcessList;
};
