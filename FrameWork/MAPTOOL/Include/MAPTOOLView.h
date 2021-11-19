
// MAPTOOLView.h: CMAPTOOLView 클래스의 인터페이스
//

#pragma once

class CMAPTOOLDoc;
class CForm;
class CMainFrame;
class CDynamicCamera;

class CMAPTOOLView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMAPTOOLView() noexcept;
	DECLARE_DYNCREATE(CMAPTOOLView)

// 특성입니다.
public:
	CMAPTOOLDoc* GetDocument() const;
	//=============
	CMainFrame* m_pMainFrame;
	CForm* m_pForm;
	//=============
	CGraphicDev* m_pGraphicDev;
	LPDIRECT3DDEVICE9 m_pDevice;
	CInputDev* m_pInputDev;
	CProtoMgr* m_pProtoMgr;
	CTextureMgr* m_pTextureMgr;
	//=============
	CDynamicCamera* m_pDynamicCamera;

	//=============
	//라인 위치
	std::array<_vec3, 6> LineXYZ;
	std::array<_vec3, 6> ObjectLineXYZ;
	//폴더이름
	CString m_tTextureFolder;
	//파일이름
	CString m_tTexturePath;
	//뷰활성화 여부
	_bool m_bOnActive;
	//각 Obj별 Scale,Rot,Pos
	_vec3 m_vecSRP[3];
	//==============
	std::list<CGameObject*> m_listTerrain;
	std::list<CGameObject*> m_listQuad;
	std::list<CGameObject*> m_listCube;
	std::list<CGameObject*> m_listItem;
	//================
	// LIGHT
	D3DLIGHT9 mLight;

	 
	
// 작업입니다.
public:
	CDynamicCamera* Get_DynamicCamera() { return m_pDynamicCamera; }
	LPDIRECT3DDEVICE9 View_Get_Deivce() { return m_pDevice; }
	void SetUp_DefaultGraphicDevSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	void Update_View(const float& fTimeDelta);
	void Init_LineXYZ(std::array<_vec3,6>& LineXYZ,_float Length , CGameObject* NowObject = nullptr);
	void Render_LineXYZ();
	void Init_ToolWindows();
	void Init_Component();
	void Set_XYZKey();
	void UpDown_Key(_vec3* pVector);
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // MAPTOOLView.cpp의 디버그 버전
inline CMAPTOOLDoc* CMAPTOOLView::GetDocument() const
   { return reinterpret_cast<CMAPTOOLDoc*>(m_pDocument); }
#endif

