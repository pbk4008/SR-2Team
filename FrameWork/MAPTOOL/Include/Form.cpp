// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "Form.h"
#include "MAPTOOLView.h"
#include "MainFrm.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
	, m_dwTerrainX(0)
	, m_dwTerrainY(0)
	, m_dwInterval(0)
{
	//m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd())->m_tMainSplitter.GetPane(0, 1));
}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, Terrain_dwCntX, m_dwTerrainX);
	DDX_Text(pDX, Terrain_dwCntY, m_dwTerrainY);
	DDX_Text(pDX, Terrain_DwInterval, m_dwInterval);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(Terrain_CreateButton, &CForm::OnBnClickedCreatebutton)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SetScrollSizes(MM_TEXT, CSize(0, 0));

	// View창 연결하기
	m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveView());

}





void CForm::OnBnClickedCreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
