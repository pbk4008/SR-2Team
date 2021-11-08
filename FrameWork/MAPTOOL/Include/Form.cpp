// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "Form.h"
#include "MAPTOOLView.h"
#include "MainFrm.h"

#include "TerrainTex.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
	, m_dwTerrainX(0)
	, m_dwTerrainZ(0)
	, m_dwInterval(0)
	, m_iTerrain_Detail(0)
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
	DDX_Text(pDX, Terrain_dwCntZ, m_dwTerrainZ);
	DDX_Text(pDX, Terrain_DwInterval, m_dwInterval);
	DDX_Control(pDX, Terrain_WireFrame, m_bWireFrame);
	DDX_Text(pDX, Terrain_DeTail, m_iTerrain_Detail);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(Terrain_CreateButton, &CForm::OnBnClickedCreatebutton)
	ON_BN_CLICKED(Terrain_Texture, &CForm::OnBnClickedTexture)
	ON_NOTIFY(UDN_DELTAPOS, Terrain_DetailSpin, &CForm::OnDeltaposDetailspin)
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
	//m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveView());
	m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 1));

	m_iTerrain_Detail = 0;


}


void CForm::OnBnClickedCreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pMapToolView->m_pBufferCom->Release();
	m_pMapToolView->m_pBufferCom = CTerrainTex::Create(m_pMapToolView->View_Get_Deivce(),m_dwTerrainX, m_dwTerrainZ, m_dwInterval);

}


void CForm::OnBnClickedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*if (!m_tTerrainTexture.GetSafeHwnd())
		m_tTerrainTexture.Create(IDD_CTerrainTexture);
	m_tTerrainTexture.ShowWindow(SW_SHOW);*/

	if (!m_tTerrainPickture.GetSafeHwnd())
		m_tTerrainPickture.Create(IDD_CTerrainPicture);
	m_tTerrainPickture.ShowWindow(SW_SHOW);
}




void CForm::OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		++m_iTerrain_Detail;
	}
	else
	{
		--m_iTerrain_Detail;
	}
	// false = 우리가눈으로 보여지는 리소스로 코드안에있는 변수들을내보냄
	// true = 리소스에있는 데이터들을 코드안에있는 변수들에게 집어넣음
	UpdateData(false);


	*pResult = 0;
}
