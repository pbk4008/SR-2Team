
// MAPTOOLView.h: CMAPTOOLView 클래스의 인터페이스
//

#pragma once

class CMAPTOOLDoc;
class CForm;
class CMainFrame;


class CMAPTOOLView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMAPTOOLView() noexcept;
	DECLARE_DYNCREATE(CMAPTOOLView)

// 특성입니다.
public:
	CMAPTOOLDoc* GetDocument() const;
	CMainFrame* m_pMainFrame;
	CForm* m_pForm;
	CGraphicDev* m_pGraphicDev;
	LPDIRECT3DDEVICE9 m_pDevice;
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMAPTOOLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // MAPTOOLView.cpp의 디버그 버전
inline CMAPTOOLDoc* CMAPTOOLView::GetDocument() const
   { return reinterpret_cast<CMAPTOOLDoc*>(m_pDocument); }
#endif

