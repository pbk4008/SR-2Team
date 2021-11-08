// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "Form.h"
#include "MAPTOOLView.h"
#include "MainFrm.h"

#include "TerrainTex.h"
#include "VIBuffer.h"

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
	DDX_Control(pDX, List_Terrain, m_List_Terrain);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(Terrain_CreateButton, &CForm::OnBnClickedCreatebutton)
	ON_BN_CLICKED(Terrain_Texture, &CForm::OnBnClickedTexture)
	ON_NOTIFY(UDN_DELTAPOS, Terrain_DetailSpin, &CForm::OnDeltaposDetailspin)
	ON_BN_CLICKED(BUTTON_Terrain_Save, &CForm::OnBnClickedTerrainSave)
	ON_BN_CLICKED(BUTTON_Terrain_Load, &CForm::OnBnClickedTerrainLoad)
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
	m_iTerrain_Detail = 1;


}


void CForm::OnBnClickedCreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pMapToolView->m_pBufferCom->Release();
	m_pMapToolView->m_pBufferCom = CTerrainTex::Create(m_pMapToolView->View_Get_Deivce(),m_dwTerrainX, m_dwTerrainZ, m_dwInterval);
	CString Indextemp;
	Indextemp.Format(L"%d", m_listIndex);
	m_List_Terrain.AddString(Indextemp);
	++m_listIndex;
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
		++m_iTerrain_Detail;
	}
	else
	{
		--m_iTerrain_Detail;
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
	WriteFile(hFile, &m_dwTerrainX, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_dwTerrainZ, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_dwInterval, sizeof(int), &dwByte, nullptr);
	WriteFile(hFile, &m_iTerrain_Detail, sizeof(int), &dwByte, nullptr);

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

	ReadFile(hFile, &m_dwTerrainX, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &m_dwTerrainZ, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &m_dwInterval, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &m_iTerrain_Detail, sizeof(int), &dwByte, nullptr);

	int TerrainTextureFolderLength = 0;
	ReadFile(hFile, &TerrainTextureFolderLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFolder = new TCHAR[TerrainTextureFolderLength];
	ReadFile(hFile, strFolder, sizeof(TCHAR) * TerrainTextureFolderLength, &dwByte, nullptr);
	m_pMapToolView->m_tTextureFolder = strFolder;




	int TerrainTexturePathLength = 0;
	ReadFile(hFile, &TerrainTexturePathLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFile = new TCHAR[TerrainTexturePathLength];
	ReadFile(hFile, strFile, sizeof(TCHAR) * TerrainTexturePathLength, &dwByte, nullptr);
	m_pMapToolView->m_tTexturePath = strFile;

	TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
	lstrcat(strtemp, strFolder);
	lstrcat(strtemp, L"\\");
	lstrcat(strtemp, strFile);
	lstrcat(strtemp, L".png");
	if (!GetTexture(strFile, TEXTURETYPE::TEX_NORMAL))
		Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp	, strFile, 1);

	delete[] strFolder;
	delete[] strFile;

	CloseHandle(hFile);
	UpdateData(FALSE);
}

void CForm::ReSize_Detail()
{
	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;
	dynamic_cast<CTerrainTex*>(m_pMapToolView->m_pBufferCom)->Get_VBuffer()->Lock(0, 0, (void**)&pVertex, 0);
	UPOINT tPoint = dynamic_cast<CTerrainTex*>(m_pMapToolView->m_pBufferCom)->Get_XZ();
	for (_ulong i = 0; i < tPoint.z; i++)
	{
		for (_ulong j = 0; j < tPoint.x; j++)
		{
			dwIndex = i * tPoint.x + j;
			pVertex[dwIndex].vUV = _vec2(_float(j) / (tPoint.x - 1) * m_iTerrain_Detail, _float(i) / (tPoint.z - 1) * m_iTerrain_Detail);
		}
	}
	dynamic_cast<CTerrainTex*>(m_pMapToolView->m_pBufferCom)->Get_VBuffer()->Unlock();
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
