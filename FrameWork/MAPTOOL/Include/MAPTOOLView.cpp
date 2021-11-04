
// MAPTOOLView.cpp: CMAPTOOLView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MAPTOOL.h"
#endif

#include "MAPTOOLDoc.h"
#include "MAPTOOLView.h"
#include "Form.h"
#include "MainFrm.h"
#include "StaticCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
// CMAPTOOLView

IMPLEMENT_DYNCREATE(CMAPTOOLView, CView)

BEGIN_MESSAGE_MAP(CMAPTOOLView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMAPTOOLView 생성/소멸

CMAPTOOLView::CMAPTOOLView() noexcept
	: m_pDevice(nullptr)
	, m_pForm(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pMainFrame(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMAPTOOLView::~CMAPTOOLView()
{
	m_pGraphicDev->DestroyInstance();
	m_pInputDev->DestroyInstance();
}

BOOL CMAPTOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CMAPTOOLView::SetUp_DefaultGraphicDevSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	Engine::Init_GraphicDev(WINDOW::WIN, WINCX, WINCY, g_hWnd, &m_pGraphicDev);
	
	(*ppGraphicDev) = m_pGraphicDev->getDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 폰트 설치

	// Input 설치
	m_pInputDev = CInputDev::GetInstance();

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	/*	wrap: 0~1을 넘는 경우 다시 0부터 출력
		clamp : 0~1을 넘는 경우 그냥 잘라냄
		mirror : 0~1을 넘는 경우 반전하여 출력 */

}

// CMAPTOOLView 그리기


void CMAPTOOLView::OnDraw(CDC* /*pDC*/)
{
	CMAPTOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.




}


// CMAPTOOLView 인쇄

BOOL CMAPTOOLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMAPTOOLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMAPTOOLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMAPTOOLView 진단

#ifdef _DEBUG
void CMAPTOOLView::AssertValid() const
{
	CView::AssertValid();
}

void CMAPTOOLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMAPTOOLDoc* CMAPTOOLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMAPTOOLDoc)));
	return (CMAPTOOLDoc*)m_pDocument;
}
#endif //_DEBUG


// CMAPTOOLView 메시지 처리기


void CMAPTOOLView::OnInitialUpdate()
{
	//VIew는 도크먼트와 프레임과 함께 동작해야됨
	// 스스로 윈도우가 된건 중요 하지않음
	// 도큐먼트가 준비가 되어있는지가 중요 그래서 OninitialUpdate를 함

	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(m_pMainFrame->m_tMainSplitter.GetPane(0, 0));

	RECT rcMain = {};
	m_pMainFrame->GetWindowRect(&rcMain); //전체 윈도우크기 구하기

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView); // 현재 클래스의 윈도우 크기 구하기

	const int iGapX = rcMain.right - rcView.right; //갭 차이 구하기
	const int iGapY = rcMain.bottom - rcView.bottom;
	m_pMainFrame->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOMOVE);
	// 갭차이만큼 더해서 진짜 자신의 크기를 구해준다

	g_hWnd = m_hWnd;

	SetUp_DefaultGraphicDevSetting(&m_pDevice);

	// ==================================== 컴포넌트 원본 생성 =====================================================
	Init_ProtoMgr();
	const _vec3 vLook = { 0.f,10.f,-1.f };
	const _vec3 vAt = { 0.f,0.f,1.f };
	const _vec3 vUp = { 0.f,1.f,0.f };
	Init_ComProto(COMPONENTID::CAMERA, CStaticCamera::Create(m_pDevice, &vLook, &vAt, &vUp,D3DXToRadian(60.f),(_float)WINCX / WINCY , 0.1f, 1000.f));


	// ==================================== 컴포넌트 원본 생성 =====================================================



}
