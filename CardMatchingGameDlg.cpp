
// CardMatchingGameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CardMatchingGame.h"
#include "CardMatchingGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCardMatchingGameDlg 대화 상자



CCardMatchingGameDlg::CCardMatchingGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARDMATCHINGGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCardMatchingGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCardMatchingGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCardMatchingGameDlg 메시지 처리기

BOOL CCardMatchingGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CString str;
	for (int i = 0; i < 19; i++)
	{
		str.Format(L".\\card_image\\%03d.bmp", i);
		m_card_image[i].Load(str);
	}

	for (int i = 0; i < 18; i++)
	{
		m_table[i] = i + 1;			// 1 ~ 18, m_table[0] ~ m_table[17]
		m_table[18 + i] = i + 1;	// 1 ~ 18, m_table[18] ~ m_table[35]
	}

	srand((unsigned int)time(NULL)); // 난수 시드 설정
	int first, second, temp;
	for (int i = 0; i < 100; i++)
	{
		first = rand() % 36; // 0 ~ 35
		second = rand() % 36; // 0 ~ 35
		if (first != second)
		{
			// m_table[first] <--> m_table[second]   SWAP
			temp = m_table[first];
			m_table[first] = m_table[second];
			m_table[second] = temp;
		}
	}

	SetTimer(1, 3000, NULL); // NULL을 쓰면 WM_TIMER 메시지를 쓰겠다는 뜻

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCardMatchingGameDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
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
		CString str;
		int card_index =0;
		for (int i = 0; i < 36; i++)
		{
			if (m_view_flag)
			{
				card_index = m_table[i];
			}
				

			m_card_image[card_index].Draw(dc, (i % 6) * 36, (i / 6) * 56);

			str.Format(L"%d", m_table[i]);
			dc.TextOutW(5+(i % 6) * 36, 5+(i / 6) * 56, str);
		}
		
		

		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCardMatchingGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCardMatchingGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		m_view_flag = 0;
		Invalidate(); // WM_PAINT 메시지 발생 -> OnPaint()
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CCardMatchingGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}
