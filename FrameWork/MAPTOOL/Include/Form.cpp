// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "Form.h"
#include "MAPTOOLView.h"
#include "MainFrm.h"

#include "TerrainTex.h"
#include "VIBuffer.h"
#include "TerrainObject.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
	, m_strFolderName(_T(""))
	, m_strFileName(_T(""))
	, m_TerrainlistIndex(0)
	, m_pMapToolView(nullptr)
{
	ZeroMemory(&m_tPlayerPos, sizeof(_float) * 2);
	ZeroMemory(&m_tTerrainInfo, sizeof(TERRAININFO));
}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, Terrain_dwCntX, m_tTerrainInfo.X);
	DDX_Text(pDX, Terrain_dwCntZ, m_tTerrainInfo.Z);
	DDX_Text(pDX, Terrain_DwInterval, m_tTerrainInfo.Interval);
	DDX_Control(pDX, Terrain_WireFrame, m_bWireFrame);
	DDX_Text(pDX, Terrain_DeTail, m_tTerrainInfo.Detail);
	DDX_Control(pDX, List_Terrain, m_List_Terrain);
	DDX_Text(pDX, Player_PosX, m_tPlayerPos[0]);
	DDX_Text(pDX, Player_PosY, m_tPlayerPos[1]);
	DDX_Text(pDX, Terrain_FolderName, m_strFolderName);
	DDX_Text(pDX, Terrain_FileName, m_strFileName);
	DDX_Text(pDX, Object_ScaleX, m_vecScale.x);
	DDX_Text(pDX, Object_ScaleY, m_vecScale.y);
	DDX_Text(pDX, Object_ScaleZ, m_vecScale.z);
	DDX_Text(pDX, Object_RotX, m_vecRotaion.x);
	DDX_Text(pDX, Object_RotY, m_vecRotaion.y);
	DDX_Text(pDX, Object_RotZ, m_vecRotaion.z);
	DDX_Text(pDX, Object_PosX, m_vecPosition.x);
	DDX_Text(pDX, Object_PosY, m_vecPosition.y);
	DDX_Text(pDX, Object_PosZ, m_vecPosition.z);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(Terrain_CreateButton, &CForm::OnBnClickedCreatebutton)
	ON_BN_CLICKED(Terrain_Texture, &CForm::OnBnClickedTexture)
	ON_NOTIFY(UDN_DELTAPOS, Terrain_DetailSpin, &CForm::OnDeltaposDetailspin)
	ON_BN_CLICKED(BUTTON_Terrain_Save, &CForm::OnBnClickedTerrainSave)
	ON_BN_CLICKED(BUTTON_Terrain_Load, &CForm::OnBnClickedTerrainLoad)
	ON_EN_CHANGE(Terrain_DeTail, &CForm::OnEnChangeDetail)
	ON_LBN_SELCHANGE(List_Terrain, &CForm::OnLbnSelchangeTerrain)
	ON_BN_CLICKED(Terrain_ModifyButton, &CForm::OnBnClickedModifybutton)
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
	m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 1));

	// 기본 Detail값
	m_tTerrainInfo.Detail = 1;

	// 기본 vector값
	m_vecScale = { 1.f,1.f,1.f };
	m_vecRotaion = { 0.f,0.f,0.f };
	m_vecPosition = { 0.f,0.f,0.f };

}


void CForm::OnBnClickedCreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (!m_tTerrainInfo.X || !m_tTerrainInfo.Z)
		return;
	CGameObject* pObj = nullptr;
	m_pNowObject = pObj = CTerrainObject::Create(m_pMapToolView->m_pDevice,m_tTerrainInfo);
	//===================
	// CString -> TCHAR
	// (TCHAR*)(LPCTSTR)
	//===================
	m_pMapToolView->m_vectorTerrain.emplace_back(pObj);
	dynamic_cast<CTerrainObject*>(pObj)->Get_Detail(&m_tTerrainInfo.Detail);

	CString Indextemp;
	Indextemp.Format(L"%d", m_TerrainlistIndex);
	m_List_Terrain.AddString(L"Terrain_" + Indextemp);
	m_List_Terrain.SetCurSel(m_TerrainlistIndex);
	++m_TerrainlistIndex;
	ReSize_Detail();



}


void CForm::OnBnClickedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_tTerrainTexture.GetSafeHwnd())
		m_tTerrainTexture.Create(IDD_CTerrainTexture);
	m_tTerrainTexture.ShowWindow(SW_SHOW);
}




void CForm::OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		dynamic_cast<CTerrainObject*>(m_pNowObject)->Set_Detail(++m_tTerrainInfo.Detail);
		//++m_tTerrainInfo.Detail;
	}
	else
	{
		dynamic_cast<CTerrainObject*>(m_pNowObject)->Set_Detail(--m_tTerrainInfo.Detail);

		//--m_tTerrainInfo.Detail;
	}

	// false = 우리가눈으로 보여지는 리소스로 코드안에있는 변수들을내보냄
	// true = 리소스에있는 데이터들을 코드안에있는 변수들에게 집어넣음
	UpdateData(false);
	ReSize_Detail();

	*pResult = 0;
}


void CForm::OnBnClickedTerrainSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 
	//다이얼로그열기
	static TCHAR szFilter[] = L"Terrain Data파일(.Terraindat)||";
	CFileDialog Dlg(false, L"Terraindat",NULL,OFN_OVERWRITEPROMPT,szFilter);

	TCHAR szFilePath[] = L"..//..//Client//Bin//Resource//Stage";
	
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		Save_Terrain(Dlg.GetPathName());
	}

}

void CForm::Save_Terrain(CString strFilePath)
{
	HANDLE hFile = CreateFile(strFilePath,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"TerrainSave : CreateFile 실패");
		return;
	}

	DWORD dwByte = 0;
	// X,Z,interval, Detail저장하기
	WriteFile(hFile, &m_tTerrainInfo.X, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_tTerrainInfo.Z, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_tTerrainInfo.Interval, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_tTerrainInfo.Detail, sizeof(int), &dwByte, nullptr);

	//문자열 저장하기 
	//문자열의 길이를 저장하고 
	//문자열을 저장함 


	//폴더 저장하기
	TCHAR strFolder[MAX_PATH] = L"";
	lstrcpy(strFolder, m_pMapToolView->m_tTextureFolder);
	int TerrainTextureFolderLength = m_pMapToolView->m_tTextureFolder.GetLength() + 1;
	WriteFile(hFile, &TerrainTextureFolderLength, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, strFolder, sizeof(TCHAR) * TerrainTextureFolderLength, &dwByte, nullptr);



	//파일저장하기
	TCHAR strFile[MAX_PATH] = L"";
	lstrcpy(strFile, m_pMapToolView->m_tTexturePath);
	int TerrainTexturePathLength = m_pMapToolView->m_tTexturePath.GetLength() + 1;
	WriteFile(hFile, &TerrainTexturePathLength, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, strFile, sizeof(TCHAR) * TerrainTexturePathLength, &dwByte, nullptr);

	CloseHandle(hFile);
	
}

void CForm::Load_Terrain(CString strFilePath)
{
	HANDLE hFile = CreateFile(strFilePath,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Terrain Load : CreateFile : 실패");
		return;
	}

	DWORD dwByte = 0;
	BOOL bResult = FALSE;

	bResult = ReadFile(hFile, &m_tTerrainInfo.X, sizeof(int), &dwByte, nullptr);
	bResult = ReadFile(hFile, &m_tTerrainInfo.Z, sizeof(int), &dwByte, nullptr);
	bResult = ReadFile(hFile, &m_tTerrainInfo.Interval, sizeof(int), &dwByte, nullptr);
	bResult = ReadFile(hFile, &m_tTerrainInfo.Detail, sizeof(int), &dwByte, nullptr);

	int TerrainTextureFolderLength = 0;
	bResult = ReadFile(hFile, &TerrainTextureFolderLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFolder = new TCHAR[TerrainTextureFolderLength];
	bResult = ReadFile(hFile, strFolder, sizeof(TCHAR) * TerrainTextureFolderLength, &dwByte, nullptr);
	m_pMapToolView->m_tTextureFolder = strFolder;




	int TerrainTexturePathLength = 0;
	bResult = ReadFile(hFile, &TerrainTexturePathLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFile = new TCHAR[TerrainTexturePathLength];
	bResult = ReadFile(hFile, strFile, sizeof(TCHAR) * TerrainTexturePathLength, &dwByte, nullptr);
	m_pMapToolView->m_tTexturePath = strFile;
	if (TRUE == bResult)
	{
		TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
		lstrcat(strtemp, strFolder);
		lstrcat(strtemp, L"\\");
		lstrcat(strtemp, strFile);
		lstrcat(strtemp, L".png");
		if (!GetTexture(strFile, TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp, strFile, 1);

		delete[] strFolder;
		delete[] strFile;

		CloseHandle(hFile);
		UpdateData(FALSE);
	}
}

void CForm::ReSize_Detail()
{
	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;
	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainObject*>(m_pNowObject)->Get_Tex();

	pTerrainTex->Get_VBuffer()->Lock(0, 0, (void**)&pVertex, 0);
	UPOINT tPoint = pTerrainTex->Get_XZ();
	for (_ulong i = 0; i < tPoint.z; i++)
	{
		for (_ulong j = 0; j < tPoint.x; j++)
		{
			dwIndex = i * tPoint.x + j;
			pVertex[dwIndex].vUV = _vec2(_float(j) / (tPoint.x - 1) * m_tTerrainInfo.Detail, _float(i) / (tPoint.z - 1) * m_tTerrainInfo.Detail);
		}
	}
	pTerrainTex->Get_VBuffer()->Unlock();
}

void CForm::OnBnClickedTerrainLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR szFilter[] = L"Terrain Data파일(.Terraindat)||";
	CFileDialog Dlg(true, L"Terraindat", NULL, OFN_OVERWRITEPROMPT, szFilter);

	TCHAR szFilePath[] = L"..//..//Client//Bin//Resource//Stage";

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		Load_Terrain(Dlg.GetPathName());
	}
	ReSize_Detail();
}

void CForm::OnEnChangeDetail()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_tTerrainInfo.Detail <= 0)
		return;
	UpdateData(true);
	ReSize_Detail();
}

// Terrain List control
void CForm::OnLbnSelchangeTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCursel = m_List_Terrain.GetCurSel();

	 m_pNowObject =  *(m_pMapToolView->m_vectorTerrain.begin() + iCursel);
	
	 
	CTransform* pObjTransform = m_pNowObject->getTransform();
	m_vecScale = pObjTransform->getScale();
	m_vecPosition =  pObjTransform->getPos();
	m_vecRotaion = pObjTransform->getAngle();
	static_cast<CTerrainObject*>(m_pNowObject)->Get_Path(m_strFolderName, m_strFileName);
	static_cast<CTerrainObject*>(m_pNowObject)->Linking_Transform(m_vecScale, m_vecRotaion, m_vecPosition);
	static_cast<CTerrainObject*>(m_pNowObject)->Linking_TerrainInfo(&m_tTerrainInfo);

	//static_cast<CTerrainObject*>(m_pNowObject)->Linking_TerrainInfo(&m_tTerrainInfo);
	
	UpdateData(false);

}


void CForm::OnBnClickedModifybutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// X,Z,interval,Detail값을 연동시킨다.
	UpdateData(true);
	static_cast<CTerrainObject*>(m_pNowObject)->Set_TerrainInfo(&m_tTerrainInfo);
	static_cast<CTerrainObject*>(m_pNowObject)->Set_Transform(m_vecScale, m_vecRotaion, m_vecPosition);

}
