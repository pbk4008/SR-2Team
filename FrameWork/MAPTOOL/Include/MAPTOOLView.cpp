
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
#include "DynamicCamera.h"
#include "TerrainTex.h"
#include "TerrainObject.h"
#include "QuadObject.h"
#include "ToolGameObject.h"
#include "ItemObject.h"
#include "MonsterObject.h"

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
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// CMAPTOOLView 생성/소멸

CMAPTOOLView::CMAPTOOLView() noexcept
	: m_pDevice(nullptr)
	, m_pForm(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pMainFrame(nullptr)
	, m_pProtoMgr(nullptr)
	, m_bOnActive(true)
	, m_pDynamicCamera(nullptr)
	, m_pInputDev(nullptr)
	, m_pTextureMgr(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_vecSRP[0] = { 1.f,1.f,1.f };
	ZeroMemory(m_vecSRP[1], sizeof(_vec3) * 2);



}

CMAPTOOLView::~CMAPTOOLView()
{
	std::for_each(m_listTerrain.begin(), m_listTerrain.end(), DeleteObj);
	m_listTerrain.clear();

	std::for_each(m_listQuad.begin(), m_listQuad.end(), DeleteObj);
	m_listQuad.clear();

	std::for_each(m_listCube.begin(), m_listCube.end(), DeleteObj);
	m_listCube.clear();

	std::for_each(m_listItem.begin(), m_listItem.end(), DeleteObj);
	m_listItem.clear();

	std::for_each(m_listMonster.begin(), m_listMonster.end(), DeleteObj);
	m_listMonster.clear();

	m_pGraphicDev->DestroyInstance();
	Safe_Release(m_pDynamicCamera);
	Utility_Release();
	System_Release();
}

BOOL CMAPTOOLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CMAPTOOLView::SetUp_DefaultGraphicDevSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	// ppGraphicDev = m_pDevice
	Engine::Init_GraphicDev(WINDOW::WIN, WINCX, WINCY, g_hWnd, &m_pGraphicDev);
	
	(*ppGraphicDev) = m_pGraphicDev->getDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Device가 알아서 법선벡터들을 관리하게 해준다.

	// 폰트 설치

	// Input 설치
	m_pInputDev = CInputDev::GetInstance();
	// ProtoMgr 설치
	m_pProtoMgr = CProtoMgr::GetInstance();

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	/*	wrap: 0~1을 넘는 경우 다시 0부터 출력 //기본옵션
		clamp : 0~1을 넘는 경우 그냥 잘라냄
		mirror : 0~1을 넘는 경우 반전하여 출력 */
}

void CMAPTOOLView::Update_View(const float& fTimeDelta)
{
	if (m_bOnActive)
	{
		CInputDev::GetInstance()->Update_InputDev();
		m_pDynamicCamera->Update_Object(fTimeDelta);
		Set_XYZKey();
	}
	if (m_pForm->m_Button_LightOnOff.GetCheck())
	{
		Update_Light();
	}
	if (!m_pForm->m_Button_FogOnOff.GetCheck())
	{
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}

		if (!m_listTerrain.empty())
		{
			for (const auto& Obj : m_listTerrain)
			{
				m_vecSRP[0] = Obj->getTransform()->getScale();
				//m_vecSRP[1] = Obj->getTransform()->getToolAngle();
				m_vecSRP[1] = Obj->getTransform()->getAngle();
				m_vecSRP[2] = Obj->getTransform()->getPos();
				dynamic_cast<CToolGameObject*>(Obj)->Set_Transform(m_vecSRP[0],m_vecSRP[1],m_vecSRP[2]);
				dynamic_cast<CToolGameObject*>(Obj)->Update_GameObject(fTimeDelta);
			}
		}
		if (!m_listQuad.empty())
		{
			for (const auto& Obj : m_listQuad)
			{
				m_vecSRP[0] = Obj->getTransform()->getScale();
				m_vecSRP[1] = Obj->getTransform()->getAngle();

				//m_vecSRP[1] = Obj->getTransform()->getToolAngle();
				m_vecSRP[2] = Obj->getTransform()->getPos();
				dynamic_cast<CToolGameObject*>(Obj)->Set_Transform(m_vecSRP[0], m_vecSRP[1], m_vecSRP[2]);
				dynamic_cast<CToolGameObject*>(Obj)->Update_GameObject(fTimeDelta);
			}
		}
		if (!m_listCube.empty())
		{
			for (const auto& Obj : m_listCube)
			{
				m_vecSRP[0] = Obj->getTransform()->getScale();
				m_vecSRP[1] = Obj->getTransform()->getAngle();

				//m_vecSRP[1] = Obj->getTransform()->getToolAngle();
				m_vecSRP[2] = Obj->getTransform()->getPos();
				dynamic_cast<CToolGameObject*>(Obj)->Set_Transform(m_vecSRP[0], m_vecSRP[1], m_vecSRP[2]);
				dynamic_cast<CToolGameObject*>(Obj)->Update_GameObject(fTimeDelta);
			}
		}
		if (!m_listItem.empty())
		{
			for (const auto& Obj : m_listItem)
			{
				m_vecSRP[0] = Obj->getTransform()->getScale();
				m_vecSRP[1] = Obj->getTransform()->getAngle();

				//m_vecSRP[1] = Obj->getTransform()->getToolAngle();
				m_vecSRP[2] = Obj->getTransform()->getPos();
				dynamic_cast<CToolGameObject*>(Obj)->Set_Transform(m_vecSRP[0], m_vecSRP[1], m_vecSRP[2]);
				dynamic_cast<CToolGameObject*>(Obj)->Update_GameObject(fTimeDelta);
			}
		}
		if (!m_listMonster.empty())
		{
			for (const auto& Obj : m_listMonster)
			{
				m_vecSRP[0] = Obj->getTransform()->getScale();
				m_vecSRP[1] = Obj->getTransform()->getAngle();

				//m_vecSRP[1] = Obj->getTransform()->getToolAngle();
				m_vecSRP[2] = Obj->getTransform()->getPos();
				dynamic_cast<CToolGameObject*>(Obj)->Set_Transform(m_vecSRP[0], m_vecSRP[1], m_vecSRP[2]);
				dynamic_cast<CToolGameObject*>(Obj)->Update_GameObject(fTimeDelta);
			}
		}

		if (m_pForm->m_pNowObject)
		{
			Init_LineXYZ(ObjectLineXYZ, 1.f, m_pForm->m_pNowObject);
		}
		
			
}

void CMAPTOOLView::Init_LineXYZ(std::array<_vec3, 6>& LineXYZ, _float Length, CGameObject* NowObject )
{
	if (!NowObject)
	{
		LineXYZ[0] = _vec3{ 0	  ,0     ,0 };
		LineXYZ[1] = _vec3{ 0	  ,0	 ,Length };
		LineXYZ[2] = _vec3{ 0	  ,0	 ,0 };
		LineXYZ[3] = _vec3{ 0	  ,Length,0 };
		LineXYZ[4] = _vec3{ 0     ,0	 ,0 };
		LineXYZ[5] = _vec3{ Length,0	 ,0 };
	}
	else
	{
		_matrix	ObjectWorld = NowObject->getTransform()->getWorldMatrix();

		LineXYZ[0] = _vec3{ ObjectWorld._41								, ObjectWorld._42							  , ObjectWorld._43								};
		LineXYZ[1] = _vec3{ ObjectWorld._41	+ (Length * ObjectWorld._31), ObjectWorld._42 + (Length * ObjectWorld._32), ObjectWorld._43 + (Length * ObjectWorld._33)};
		LineXYZ[2] = _vec3{ ObjectWorld._41						   		, ObjectWorld._42							  , ObjectWorld._43								};
		LineXYZ[3] = _vec3{ ObjectWorld._41 + (Length * ObjectWorld._21), ObjectWorld._42 + (Length * ObjectWorld._22), ObjectWorld._43 + (Length * ObjectWorld._23)};
		LineXYZ[4] = _vec3{ ObjectWorld._41								, ObjectWorld._42							  , ObjectWorld._43								};
		LineXYZ[5] = _vec3{ ObjectWorld._41 + (Length * ObjectWorld._11), ObjectWorld._42 + (Length * ObjectWorld._12), ObjectWorld._43 + (Length * ObjectWorld._13)};

	}
}

void CMAPTOOLView::Render_LineXYZ()
{
	_matrix matWorld, matView, matProj;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->getLine()->Begin();
	m_pGraphicDev->getLine()->DrawTransform(&LineXYZ[0], 2, &(matView * matProj), D3DCOLOR_XRGB(255, 0, 0));
	m_pGraphicDev->getLine()->DrawTransform(&LineXYZ[2], 2, &(matView * matProj), D3DCOLOR_XRGB(0, 255, 0));
	m_pGraphicDev->getLine()->DrawTransform(&LineXYZ[4], 2, &(matView * matProj), D3DCOLOR_XRGB(0, 0, 255));
	if (m_pForm->m_pNowObject)
	{
		m_pGraphicDev->getLine()->DrawTransform(&ObjectLineXYZ[0], 2, &(matView * matProj), D3DCOLOR_XRGB(255, 0, 0));
		m_pGraphicDev->getLine()->DrawTransform(&ObjectLineXYZ[2], 2, &(matView * matProj), D3DCOLOR_XRGB(0, 255, 0));
		m_pGraphicDev->getLine()->DrawTransform(&ObjectLineXYZ[4], 2, &(matView * matProj), D3DCOLOR_XRGB(0, 0, 255));
	}
	m_pGraphicDev->getLine()->End();

}

void CMAPTOOLView::Init_ToolWindows()
{
	//View창 정확한 크기구하기
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

	//글로벌 핸들 만들어주기
	g_hWnd = m_hWnd;
}

void CMAPTOOLView::Init_Component()
{
	// ==================================== 컴포넌트 원본 생성 =====================================================
	// Camera
	const _vec3 vLook = { 0.f,10.f,-10.f };
	const _vec3 vAt = { 0.f,0.f,1.f };
	const _vec3 vUp = { 0.f,1.f,0.f };
	Init_ComProto(COMPONENTID::CAMERA, CDynamicCamera::Create(m_pDevice, &vLook, &vAt, &vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f));

	// Transform
	Init_ComProto(COMPONENTID::TRANSFORM, CTransform::Create());

	Init_ComProto(COMPONENTID::SPHERECOL, CSphereCollision::Create(m_pDevice));


	// ==================================== 컴포넌트 원본 생성 =====================================================
	// ==================================== 아이템 텍스처 넣기 =====================================================
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP20PATH, L"HP20", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP50PATH, L"HP50", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP100PATH, L"HP100", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, SHURIKEN20PATH, L"SHURIKEN20", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, SHURIKEN50PATH, L"SHURIKEN50", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, BOMB2PATH, L"BOMB2", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, BOMB5PATH, L"BOMB5", 1);
	// ==================================== 아이템 텍스처 넣기 =====================================================
	// ==================================== 몬스터 텍스쳐 넣기 =====================================================
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, MeleeMon, L"MeleeMon", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, FlyMon, L"FlyMon", 1);
	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, ShootMon, L"ShootMon", 1);
	// ==================================== 몬스터 텍스쳐 넣기 =====================================================

}

void CMAPTOOLView::Set_XYZKey()
{
	if (m_pForm->m_pNowObject)
	{
		m_vecSRP[0] = m_pForm->m_pNowObject->getTransform()->getScale();
		m_vecSRP[1] = m_pForm->m_pNowObject->getTransform()->getAngle();
		m_vecSRP[2] = m_pForm->m_pNowObject->getTransform()->getPos();
	}
	else
	{
		return;
	}
	
	if (m_pForm->m_Button_Scale.GetCheck())
	{
		UpDown_Key(&m_vecSRP[0]);
	}
	else if (m_pForm->m_Button_Roation.GetCheck())
	{
		UpDown_Key(&m_vecSRP[1]);
	}
	else if (m_pForm->m_Button_Position.GetCheck())
	{
		UpDown_Key(&m_vecSRP[2]);
	}

	m_pForm->m_pNowObject->getTransform()->setScale(m_vecSRP[0]);
	m_pForm->m_pNowObject->getTransform()->setAngle(m_vecSRP[1]);
	m_pForm->m_pNowObject->getTransform()->setPos(m_vecSRP[2]);
	m_pForm->Set_SRP(m_vecSRP[0], m_vecSRP[1], m_vecSRP[2]);

}

void CMAPTOOLView::UpDown_Key(_vec3* pVector)
{
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
	{
		pVector->x -= m_pForm->m_fMovePower;
	}
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	{
		pVector->x += m_pForm->m_fMovePower;
	}
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		pVector->y -= m_pForm->m_fMovePower;
	}
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		pVector->y += m_pForm->m_fMovePower;
	}
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
	{
		pVector->z -= m_pForm->m_fMovePower;
	}
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	{
		pVector->z += m_pForm->m_fMovePower;
	}
}

void CMAPTOOLView::Init_Light()
{
	ZeroMemory(&mLight, sizeof(D3DLIGHT9));
	mLight.Type = D3DLIGHT_SPOT;
	// Update View에서 position Direction 업데이트해줌
	/*mLight.Position = { 0.f,0.f,0.f };
	mLight.Direction = { 0.f,0.f,0.f };*/
	mLight.Diffuse.r = 1.f;
	mLight.Diffuse.g = 1.f;
	mLight.Diffuse.b = 1.f;
	mLight.Attenuation0 = 0.01f;
	mLight.Attenuation1 = 0.0f;
	mLight.Attenuation2 = 0.0f;
	mLight.Theta = D3DX_PI / 4.f;
	mLight.Phi = D3DX_PI / 3.f;
	mLight.Falloff = 1.f;
	//mLight.Theta = 0.f;
	//mLight.Phi = D3DX_PI / 2.f;
	mLight.Ambient = { 0.4f,0.4f,0.4f,0.4f };
	mLight.Specular = { 0.6f,0.6f,0.6f ,0.6f };
	mLight.Range = 50.f;

	//mLight.Type = D3DLIGHT_DIRECTIONAL;
	//// Update View에서 position Direction 업데이트해줌
	////mLight.Position = { 0.f,0.f,0.f };
	//mLight.Direction = { 0.f,-1.f,-1.f };
	//mLight.Diffuse.r = 1.f;
	//mLight.Diffuse.g = 1.f;
	//mLight.Diffuse.b = 1.f;
	//mLight.Attenuation0 = 0.01f;
	//mLight.Attenuation1 = 0.0f;
	//mLight.Attenuation2 = 0.0f;
	//mLight.Theta = D3DX_PI / 4.f;
	//mLight.Phi = D3DX_PI / 3.f;
	//mLight.Falloff = 1.f;
	////mLight.Theta = 0.f;
	////mLight.Phi = D3DX_PI / 2.f;
	//mLight.Ambient = { 0.4f,0.4f,0.4f,0.4f };
	//mLight.Specular = { 0.6f,0.6f,0.6f ,0.6f };
	//mLight.Range = 50.f;

	
}

void CMAPTOOLView::Update_Light()
{
	_matrix matCameraWorld;
	_matrix matView = static_cast<CCamera*>(m_pDynamicCamera)->getViewmat();
	D3DXMatrixInverse(&matCameraWorld, nullptr, &matView);


	_vec3 NormalAt = {
		matCameraWorld.m[2][0],
		matCameraWorld.m[2][1],
		matCameraWorld.m[2][2]
	};
	D3DXVec3Normalize(&NormalAt, &NormalAt);
	mLight.Direction = NormalAt;
	m_pForm->m_fLightDirectionX = NormalAt.x;
	m_pForm->m_fLightDirectionY = NormalAt.y;
	m_pForm->m_fLightDirectionZ = NormalAt.z;
	m_pForm->UpdateData(FALSE);


	_vec3 NormalEye = {
		matCameraWorld.m[3][0],
		matCameraWorld.m[3][1],
		matCameraWorld.m[3][2]
	};
	mLight.Position = NormalEye;
}

void CMAPTOOLView::Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density)
{
	float Start = 0.5f;
	float End = 50.f;

	m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	if (D3DFOG_LINEAR == Mode)
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
		m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));
	}

	// Enable range-based fog if desired (only supported for
	//   vertex fog).  For this example, it is assumed that UseRange
	//   is set to a nonzero value only if the driver exposes the 
	//   D3DPRASTERCAPS_FOGRANGE capability.
	// Note: This is slightly more performance intensive
	//   than non-range-based fog.
	if (UseRange)
		m_pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	
}

// CMAPTOOLView 그리기


void CMAPTOOLView::OnDraw(CDC* /*pDC*/)
{
	CMAPTOOLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
			//루프 처리
	m_pGraphicDev->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));




	{
		if (m_pForm->m_Button_LightOnOff.GetCheck())
		{
			m_pDevice->SetLight(0, &mLight);
			m_pDevice->LightEnable(0, TRUE);
			m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
			m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		}
	}

	{
		if (m_pForm->m_bWireFrame.GetCheck())
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	{
		if (m_pForm->m_bAlphaTest.GetCheck())
		{
			m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			m_pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
		}
	}

	Render_GameObject(m_pDevice);

	{
		if(m_pForm->m_bAlphaTest.GetCheck())
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	}

	{
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		if (m_pForm->m_bWireFrame.GetCheck())
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	{
		if (!m_pForm->m_Button_LightOnOff.GetCheck())
		{
			m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}
	}

	if (m_pForm->m_Button_Zbuffer.GetCheck())
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}

	Render_LineXYZ();

	if (m_pForm->m_Button_Zbuffer.GetCheck())
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	m_pGraphicDev->Render_End();

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

	//기본 Tool창 설정
	Init_ToolWindows();

	// Graphic Device 기초설정
	SetUp_DefaultGraphicDevSetting(&m_pDevice);

	// XYZ색선 점 설정
	Init_LineXYZ(LineXYZ,5.f);

	// 매니저 활성
	Init_ProtoMgr();
	m_pTextureMgr = Init_TextureMgr();

	//컴포넌트들 생성
	Init_Component();

	//카메라 붙이기
	m_pDynamicCamera = Clone_ComProto<CDynamicCamera>(COMPONENTID::CAMERA);

	Init_Light();
	// fog는 Alpha값 무시됨
	Init_Fog(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.f), D3DFOG_EXP2,TRUE, 0.075f);


	m_pForm->UpdateData(false);
}


BOOL CMAPTOOLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// WM_PAINT 메세지가 날아오기전 날아오는메세지 이것으로 인해 directx clear 함수가 지워주는데도 그전에 한번더 지워주어서 깜빡임이 발생할수있다 0을 반환 해주자
	//return CView::OnEraseBkgnd(pDC);
	return 0;
}


void CMAPTOOLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (TRUE == bActivate) m_bOnActive = true;
	if (FALSE == bActivate) m_bOnActive = false;
}
