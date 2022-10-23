
// CardMatchingGameDlg.h: 헤더 파일
//

#pragma once


// CCardMatchingGameDlg 대화 상자
class CCardMatchingGameDlg : public CDialogEx
{
private:
	CImage m_card_image[19]; // 0 -> 뒷면, 1 ~ 18 -> 카드사진
	char m_view_flag = 1;
	char m_table[36]; // 1차원 배열로 카드판 관리
// 생성입니다.
public:
	CCardMatchingGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARDMATCHINGGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
