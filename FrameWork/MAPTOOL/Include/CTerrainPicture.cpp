// CTerrainPicture.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "CTerrainPicture.h"
#include "afxdialogex.h"


// CTerrainPicture 대화 상자

IMPLEMENT_DYNAMIC(CTerrainPicture, CDialogEx)

CTerrainPicture::CTerrainPicture(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTerrainPicture, pParent)
{

}

CTerrainPicture::~CTerrainPicture()
{
}

void CTerrainPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Terrain_Picture, m_Picture_Terrain);
	DDX_Control(pDX, Picture_Scroll, m_Scroll_Picture);
}


BEGIN_MESSAGE_MAP(CTerrainPicture, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainPicture::OnBnClickedButton1)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CTerrainPicture 메시지 처리기


BOOL CTerrainPicture::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	Init_TerrainPicture();
	//Init_ScrollBar();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTerrainPicture::Get_FileList(CString strFolder)
{

	CFileFind finder;
	CString strFolderItem, strFileExt;
	BOOL bWorking = finder.FindFile(strFolder);

	CDC* dc = nullptr; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture_Terrain.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_Picture_Terrain.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		//상위 디렉토리 건너뛰기
		if (finder.IsDots()) continue;

		//폴더 속 폴더이면 재귀호출해주기
		if (finder.IsDirectory() && !m_bFirst)
		{
			strFolderItem = finder.GetFilePath() + L"\\*.*";
			m_bFirst = true;
			Get_FileList(strFolderItem);

		}
		//파일 일때
		if (finder.IsArchived())
		{
			CImage image;//불러오고 싶은 이미지를 로드할 CImage 
			if (FAILED(image.Load(finder.GetFilePath())))
			{
				ERR_MSG(L"Texture 이미지 로드에 실패했습니다.");//이미지 로드
			}

			//image.StretchBlt(dc->m_hDC, 0, 0, rect.Width() / 5 * m_iPictureX, rect.Height() / 5 * m_iPictureY, SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
			image.StretchBlt(dc->m_hDC,  rect.Width() / 5 * (m_iPictureX-1), 
				rect.Height() / 5 * (m_iPictureY -1), 
				rect.Width() / 5 , 
				rect.Height() / 5 , SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
			++m_iPictureX;
			if (m_iPictureX == 6)
			{
				++m_iPictureY;
				m_iPictureX = 1;
			}

			
		}
	}
	m_Scroll_Picture.SetScrollRange(0, rect.Height() / 5 * (m_iPictureY - 2));
	
	ReleaseDC(dc);//DC 해제
}

void CTerrainPicture::Init_TerrainPicture()
{

	////리소스 절대 경로
	//CString strFolder = L"..\\..\\Client\\Bin\\Resource\\Texture\\TILES*.*";

	//	Get_FileList(strFolder);
}


void CTerrainPicture::Init_ScrollBar()
{
	CRect VScroll;

	m_Scroll_Picture.GetWindowRect(&VScroll);
	m_Scroll_Picture.SetScrollRange(0, VScroll.Height());
	c.x = 0;
	c.y = 0;
}

void CTerrainPicture::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//리소스 절대 경로
	CString strFolder = L"..\\..\\Client\\Bin\\Resource\\Texture\\TILES*.*";

	Get_FileList(strFolder);
}


void CTerrainPicture::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	//현재위치
	if (pScrollBar->GetSafeHwnd() == m_Scroll_Picture.GetSafeHwnd())
	{
		int pos;
		pos = m_Scroll_Picture.GetScrollPos();


		if (nSBCode == SB_LINEDOWN)	m_Scroll_Picture.SetScrollPos(pos + 10);
		if (nSBCode == SB_LINEUP)	m_Scroll_Picture.SetScrollPos(pos - 10);
		if (nSBCode == SB_PAGEUP)	m_Scroll_Picture.SetScrollPos(pos - 50);
		if (nSBCode == SB_PAGEDOWN)	m_Scroll_Picture.SetScrollPos(pos + 50);
		if (nSBCode == SB_THUMBTRACK)	m_Scroll_Picture.SetScrollPos(nPos);

		c.y = m_Scroll_Picture.GetScrollPos();

		m_Picture_Terrain.ScrollWindow(0, nPos);

		Invalidate(false);

		CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}
