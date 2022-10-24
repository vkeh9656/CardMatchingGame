
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
	DDX_Control(pDX, IDC_TIME_PROGRESS, m_time_progress);
}

BEGIN_MESSAGE_MAP(CCardMatchingGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_HINT_BTN, &CCardMatchingGameDlg::OnBnClickedHintBtn)
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
	srand((unsigned int)time(NULL)); // 난수 시드 설정
	m_time_progress.SetRange(0, 60);
	StartGame();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCardMatchingGameDlg::StartGame()
{
	SetDlgItemInt(IDC_HINT_BTN, 3);
	m_view_flag = 1;

	for (int i = 0; i < 18; i++)
	{
		m_table[i] = i + 1;			// 1 ~ 18, m_table[0] ~ m_table[17]
		m_table[18 + i] = i + 1;	// 1 ~ 18, m_table[18] ~ m_table[35]
	}

	
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

	m_time_progress.SetPos(60);

	SetTimer(1, 3000, NULL); // NULL을 쓰면 WM_TIMER 메시지를 쓰겠다는 뜻
	SetTimer(10, 1000, NULL);
}

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
			if (m_table[i] == 0) continue; // 카드 제거 !
 
			if (m_view_flag == 1) card_index = m_table[i];
			
			m_card_image[card_index].Draw(dc, (i % 6) * 36, (i / 6) * 56);

			if (m_view_flag == 1)
			{
				str.Format(L"%d", m_table[i]);
				dc.TextOutW(5 + (i % 6) * 36, 5 + (i / 6) * 56, str);
			}
			
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

void CCardMatchingGameDlg::EndOfGame(const wchar_t* ap_ment)
{
	KillTimer(10);
	
	if (IDOK == MessageBox(L"다시 게임을 하시겠습니까?", ap_ment, MB_OKCANCEL | MB_ICONQUESTION))
	{
		StartGame();
		Invalidate();
	}
	else
	{
		EndDialog(IDOK); // 프로그램 종료
	}
}


void CCardMatchingGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		KillTimer(1);
		m_view_flag = 0;
		Invalidate(); // WM_PAINT 메시지 발생 -> OnPaint()
	}
	else if (nIDEvent == 2)
	{
		KillTimer(2);
		m_view_flag = 0;
		Invalidate();	
	}
	else if (nIDEvent == 10)
	{
		int num = m_time_progress.GetPos() - 1;
		if (num > 0) m_time_progress.SetPos(num);
		else {
			// 게임 종료 (패배)
			EndOfGame(L"졌지만 잘싸웠어..");
		}
	}
	else
	{
		CDialogEx::OnTimer(nIDEvent);
	}
}


void CCardMatchingGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_view_flag != 0) return; // 카드가 다 열려있을땐 못누르게 세팅

	
	int x = point.x / 36;
	int y = point.y / 56;

	if (x < 6 && y < 6)
	{
		int select_pos = y * 6 + x;
		if (m_table[select_pos] == 0) return; // 이미 제거된 카드는 클릭해도 상관없도록 return

		if (m_first_pos == -1)
		{
			m_first_pos = select_pos; // 첫번째 카드 선택
		}
		else
		{
			if (m_first_pos == select_pos) return;	// 같은 위치의 카드를 선택할 경우 return

			if (m_table[m_first_pos] == m_table[select_pos]) // 처음 선택한 카드와 두번째 선택한 카드가 같다면?
			{
				m_table[m_first_pos] = m_table[select_pos] = 0;
				Invalidate();
				m_find_count++;
				if (m_find_count == 18)
				{
					// 게임 종료 (승리)
					EndOfGame(L"WINNER WINNER CHICKEN DINNER!");
				}
			}
			else
			{
				m_view_flag = 2;
				SetTimer(2, 1000, NULL);
			}

			m_first_pos = -1; // 다시 선택하지 않은것으로 롤백
		}

		CClientDC dc(this);
		int card_index = m_table[select_pos];
		m_card_image[card_index].Draw(dc, x * 36, y * 56);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCardMatchingGameDlg::OnBnClickedHintBtn()
{
	int num = GetDlgItemInt(IDC_HINT_BTN);
	if (num > 0)
	{
		SetDlgItemInt(IDC_HINT_BTN, num - 1);
		m_view_flag = 1;
		Invalidate();
		
		SetTimer(1, 3000, NULL);
	}
}
