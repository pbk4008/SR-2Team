#include "pch.h"
#include "UI.h"
#include "Player.h"
#include "UiChar.h"

CUI::CUI()
	:m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_pBufferCom(nullptr), 
	m_pTexture(nullptr)
	,mPlayerTargetCheck(false)
	,mPlayer(nullptr)
	, mHpUI{}
	, mShurikenFrontUI{}
	, mShurikenUI{}
	, mBombFrontUI{}
	, mBombUI{}
	, mMonCountUI{}
	, mKeyCountUi(nullptr)
	, mKeyUi(nullptr)
	, mKeyTexture(nullptr)
	, mKeyMatrix{}
	,m_dwMosnterCount(0)

{

}

CUI::CUI(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice),m_fX(0.f), m_fY(0.f), m_fSizeX(0.f), m_fSizeY(0.f), m_pBufferCom(nullptr),
	m_pTexture(nullptr)
	, mPlayerTargetCheck(false)
	, mPlayer(nullptr)
	, mHpUI{}
	, mShurikenFrontUI{}
	, mShurikenUI{}
	, mBombFrontUI{}
	, mBombUI{}
	, mMonCountUI{}
	, mKeyCountUi(nullptr)
	, mKeyUi(nullptr)
	, mKeyTexture(nullptr)
	, mKeyMatrix{}
	, m_dwMosnterCount(0)
{

}

CUI::CUI(const CUI& rhs)
	:m_fX(rhs.m_fX), m_fY(rhs.m_fY), m_fSizeX(rhs.m_fSizeX), m_fSizeY(rhs.m_fSizeY), m_pBufferCom(rhs.m_pBufferCom),
	m_pTexture(rhs.m_pTexture)
	, mPlayerTargetCheck(false)
	, mPlayer(nullptr)
	, mHpUI{}
	, mShurikenFrontUI{}
	, mShurikenUI{}
	, mBombFrontUI{}
	, mBombUI{}
	, mMonCountUI{}
	, mKeyCountUi(nullptr)
	, mKeyUi(nullptr)
	, mKeyTexture(nullptr)
	, mKeyMatrix{}
	, m_dwMosnterCount(rhs.m_dwMosnterCount)
{

}

CUI::~CUI()
{

}

HRESULT CUI::Init_UI()
{
	BackGroundUiInit();

	frontCharInit();

	D3DXMatrixOrthoLH(&m_ProjMatrix, WINCX, WINCY, 0.0f, 1.f);



	return S_OK;
}

/* 직교투영행렬을 만든다. */
/* 3차원 로컬스페이스 상에 정의된 정점정보를 2차원 투영스페이스로 변환한다. */

Engine::_int CUI::Update_GameObject(const _float& fDeltaTime)
{
	if (!mPlayerTargetCheck)
	{
		mPlayer = static_cast<CPlayer*>(GetGameObject(GAMEOBJECTID::PLAYER));
		mPlayerTargetCheck = true;
	}
	mPlayerState = mPlayer->getState();
	_int iExit = 0;

	D3DXMatrixIdentity(&m_pTransMatrix);

	m_pTransMatrix._11 = m_fSizeX;
	m_pTransMatrix._22 = m_fSizeY;

	m_pTransMatrix._41 = m_fX - (WINCX >> 1);
	m_pTransMatrix._42 = -m_fY + (WINCY >> 1);


	_float fY = (WINCY>>1)-250;
	_float fX = (WINCX >> 1) + 150;
	D3DXMatrixIdentity(&mKeyMatrix);

	mKeyMatrix._11 = 30;
	mKeyMatrix._22 = 30;

	mKeyMatrix._41 = fX - (WINCX >> 1);
	mKeyMatrix._42 = -fY + (WINCY >> 1);

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	////for (auto& HpUi : mHpUI)
	_int KeyCount = mPlayer->getKeyCount();
	if (KeyCount > 6)
		KeyCount = 6;
	mKeyCountUi->setTextureNumber(KeyCount);
	mKeyCountUi->Update_GameObject(fDeltaTime);

	int NowHp = std::get<0>(mPlayerState);
	int quotient = 100;

	for (size_t HpUiIndex = 0; HpUiIndex < mHpUI.size(); ++HpUiIndex)
	{
		mHpUI[HpUiIndex]->Update_GameObject(fDeltaTime);
		mHpUI[HpUiIndex]->setTextureNumber(NowHp / quotient);
		NowHp %= quotient;
		quotient /= 10;
	}
	for (size_t MonsterCount = 0; MonsterCount < mMonCountUI.size(); ++MonsterCount)
	{
		mMonCountUI[MonsterCount]->Update_GameObject(fDeltaTime);
		if(m_dwMosnterCount==0)
			mMonCountUI[MonsterCount]->setTextureNumber(0);
		else
		{
			mMonCountUI[MonsterCount]->setTextureNumber(m_dwMosnterCount / quotient);
			m_dwMosnterCount %= quotient;
		}
		quotient /= 10;
	}
	quotient = 10;

	int NowShuriken = std::get<1>(mPlayerState);

	for (size_t ShurikenFrontIndex = 0; ShurikenFrontIndex < mShurikenFrontUI.size(); ++ShurikenFrontIndex)
	{
		if (NowShuriken == 0)
			mShurikenFrontUI[ShurikenFrontIndex]->setTexture(L"RedChar");
		else
			mShurikenFrontUI[ShurikenFrontIndex]->setTexture(L"YellowChar");


		mShurikenFrontUI[ShurikenFrontIndex]->Update_GameObject(fDeltaTime);
	}


	//for (auto& ShurikenUi : mShurikenUI)
	for (size_t ShurikenIndex = 0; ShurikenIndex < mShurikenUI.size(); ++ShurikenIndex)
	{
		
		mShurikenUI[ShurikenIndex]->Update_GameObject(fDeltaTime);
		if(ShurikenIndex < 2)
		{ 
			mShurikenUI[ShurikenIndex]->setTextureNumber(NowShuriken / quotient);
			NowShuriken %= quotient;
			quotient /= 10;
		}
	}

	quotient = 10;

	int NowBomb = std::get<2>(mPlayerState);
	for (size_t BombFrontIndex = 0; BombFrontIndex < mBombFrontUI.size(); ++BombFrontIndex)
	{
		if (NowBomb == 0)
			mBombFrontUI[BombFrontIndex]->setTexture(L"RedChar");
		else
			mBombFrontUI[BombFrontIndex]->setTexture(L"YellowChar");

		mBombFrontUI[BombFrontIndex]->Update_GameObject(fDeltaTime);
	}


	for (size_t BombIndex = 0; BombIndex < mBombUI.size() ; ++BombIndex)
	{
		mBombUI[BombIndex]->Update_GameObject(fDeltaTime);
		if (BombIndex < 2)
		{
			mBombUI[BombIndex]->setTextureNumber(NowBomb / quotient);
			NowShuriken %= quotient;
			quotient /= 10;
		}
	}
	Insert_RenderGroup(RENDERGROUP::UI, this);

	return iExit;
}

void CUI::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CUI::Render_GameObject()
{
	_matrix			OldViewMatrix, OldProjMatrix;

	_matrix			IdentityMatrix;
	D3DXMatrixIdentity(&IdentityMatrix);

	m_pDevice->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
	
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransMatrix);
	m_pDevice->SetTransform(D3DTS_VIEW, &IdentityMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);
	
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 50);


	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pDevice->SetTexture(0, nullptr);

	m_pDevice->SetTransform(D3DTS_WORLD, &mKeyMatrix);
	mKeyTexture->Render_Texture();
	mKeyUi->Render_Buffer();

	m_pDevice->SetTexture(0, nullptr);

	mKeyCountUi->Render_GameObject();

	for (auto& monCountUI : mMonCountUI)
	{
		monCountUI->Render_GameObject();
		if (m_dwMosnterCount==0)
			break;
	}


	for (auto& HpUi : mHpUI)
	{
		HpUi->Render_GameObject();
	}

	for (auto& ShurikenFrontUi : mShurikenFrontUI)
	{
		ShurikenFrontUi->Render_GameObject();
	}

	for (auto& ShurikenUi : mShurikenUI)
	{
		ShurikenUi->Render_GameObject();
	}

	for (auto& BombFrontUi : mBombFrontUI)
	{
		BombFrontUi->Render_GameObject();
	}

	for (auto& BombUi : mBombUI)
	{
		BombUi->Render_GameObject();
	}

	CGameObject::Render_GameObject();

	m_pDevice->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI* pInstance = new CUI(pDevice);
	if (FAILED(pInstance->Init_UI()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CUI::Clone_GameObject()
{
	return new CUI(*this);
}

HRESULT CUI::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex 버퍼
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	pComponent = m_pTexture = CTexture::Create(m_pDevice);
	pComponent->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::TEXTURE, pComponent);

	return S_OK;
}

void CUI::Free(void)
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	Safe_Release(mKeyCountUi);
	Safe_Release(mKeyTexture);
	Safe_Release(mKeyUi);

	for_each(mHpUI.begin(), mHpUI.end(), DeleteObj);
	for_each(mShurikenUI.begin(), mShurikenUI.end(), DeleteObj);
	for_each(mBombUI.begin(), mBombUI.end(), DeleteObj);
	for_each(mShurikenFrontUI.begin(), mShurikenFrontUI.end(), DeleteObj);
	for_each(mBombFrontUI.begin(), mBombFrontUI.end(), DeleteObj);


	CGameObject::Free();
}

HRESULT CUI::BackGroundUiInit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	this->setActive(true);

	m_pTexture->setTexture(GetTexture(L"UI", TEXTURETYPE::TEX_NORMAL));

	m_fX = WINCX >> 1;
	m_fY = WINCY - 60.f;
	m_fSizeX = 400.f;
	m_fSizeY = 60.f;

	return S_OK;
}

HRESULT CUI::frontCharInit()
{
	//for (auto& HpUi : mHpUI)
	for (size_t HpUiIndex = 0 ; HpUiIndex < mHpUI.size() ; ++HpUiIndex)
	{
		mHpUI[HpUiIndex] = CUiChar::Create(m_pDevice,(WINCX>>1) - 280 + 25.f * HpUiIndex,WINCY-55.f,10.f,20.f);
		mHpUI[HpUiIndex]->setTexture(L"GreenChar");
	}


	for (size_t shurikenFrontIndex = 0; shurikenFrontIndex < mShurikenFrontUI.size(); ++shurikenFrontIndex)
	{
		mShurikenFrontUI[shurikenFrontIndex] = CUiChar::Create(m_pDevice, (WINCX >> 1) - 100 + 25.f * shurikenFrontIndex, WINCY - 55.f, 10.f, 20.f);
		if (shurikenFrontIndex == 0)
			mShurikenFrontUI[shurikenFrontIndex]->setTextureNumber(2);
		else
		{
			mShurikenFrontUI[shurikenFrontIndex]->setTextureNumber(10);
			mShurikenFrontUI[shurikenFrontIndex]->setSize(2.f, 20.f);
		}
		mShurikenFrontUI[shurikenFrontIndex]->setTexture(L"YellowChar");
	}


	//for (auto& ShurikenUi : mShurikenUI)
	for (size_t ShurikenIndex = 0 ; ShurikenIndex < mShurikenUI.size() ; ++ShurikenIndex)
	{
		mShurikenUI[ShurikenIndex] = CUiChar::Create(m_pDevice, (WINCX >> 1) - 60 + 25.f * ShurikenIndex, WINCY - 55.f, 10.f, 20.f);
		if (ShurikenIndex == 2)
			mShurikenUI[ShurikenIndex]->setTextureNumber(11);
		else if (ShurikenIndex == 3 || ShurikenIndex == 4)
			mShurikenUI[ShurikenIndex]->setTextureNumber(9);
		mShurikenUI[ShurikenIndex]->setTexture(L"GreenChar");

	}

	for (size_t bombFrontIndex = 0; bombFrontIndex < mBombFrontUI.size(); ++bombFrontIndex)
	{
		mBombFrontUI[bombFrontIndex] = CUiChar::Create(m_pDevice, (WINCX >> 1) + 100 + 25.f * bombFrontIndex, WINCY - 55.f, 10.f, 20.f);
		if (bombFrontIndex == 0)
			mBombFrontUI[bombFrontIndex]->setTextureNumber(3);
		else
		{
			mBombFrontUI[bombFrontIndex]->setTextureNumber(10);
			mBombFrontUI[bombFrontIndex]->setSize(2.f, 20.f);
		}
		mBombFrontUI[bombFrontIndex]->setTexture(L"YellowChar");
	}

	//for (auto& BombUi : mBombUI)
	for(size_t BombIndex = 0 ; BombIndex < mBombUI.size() ; ++BombIndex)
	{
		mBombUI[BombIndex] = CUiChar::Create(m_pDevice, (WINCX >> 1) + 150 + 25.f * BombIndex, WINCY - 55.f, 10.f, 20.f);
		if (BombIndex == 2)
		{
			mBombUI[BombIndex]->setTextureNumber(11);
		}
		else if (BombIndex == 3)
		{
			mBombUI[BombIndex]->setTextureNumber(2);
		}
		mBombUI[BombIndex]->setTexture(L"GreenChar");

	}


	mKeyCountUi = CUiChar::Create(m_pDevice, (WINCX >> 1)+230 , (WINCY >> 1)-250 , 20.f, 30.f);
	mKeyCountUi->setTexture(L"YellowChar");

	mKeyTexture = Clone_ComProto<CTexture>(COMPONENTID::TEXTURE);
	mKeyTexture->setTexture(GetTexture(L"Key", TEXTURETYPE::TEX_NORMAL));

	mKeyUi = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);

	for (size_t MonsterIndex = 0; MonsterIndex < mMonCountUI.size(); ++MonsterIndex)
	{
		mMonCountUI[MonsterIndex] = CUiChar::Create(m_pDevice, (WINCX >> 1) + 300 + 25.f* MonsterIndex, (WINCY >> 1) - 250, 20.f, 30.f);
		mMonCountUI[MonsterIndex]->setTexture(L"YellowChar");
	}
	return S_OK;
}
