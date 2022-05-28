//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include <random>
#include "GameFramework.h"
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uid(0, 3);
CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_nBackObjects = 8;			// 선인장, 돌1, 돌2
// 선인장 center: 3.92249*scale 0 4.797254*scale, scale : 40
// 돌1 center: 3*scale 0 2*scale, scale: 50
// 돌2 center: 3*scale 0 3.267579*scale, scale: 50
// 나무 center: 0 0 0
	m_ppBackObjects = new CGameObject * [m_nBackObjects];
	for (int i = 0; i < m_nBackObjects; ++i) {
		CGameObject* pBackModel = NULL;
		CBackObject* pBackObject = NULL;
		pBackObject = new CBackObject();
		switch (uid(dre)) {
		case 0:			// 선인장
			pBackModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/Cactus.bin");
			pBackObject->type = 0;
			pBackObject->SetChild(pBackModel, true);
			if (i < (m_nBackObjects / 2))pBackObject->SetPosition(-3.92249 * 40 - 100, 0, -4.797254 * 40 + (i * 200));
			else pBackObject->SetPosition(-3.92249 * 40 + 105, 0, -4.797254 * 40 + (i % (m_nBackObjects / 2)) * 200);
			pBackObject->SetScale(40.f, 40.f, 40.f);
			break;
		case 1:			// 돌 1
			pBackModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/Rock.bin");
			pBackObject->type = 1;
			pBackObject->SetChild(pBackModel, true);
			if (i < (m_nBackObjects / 2))pBackObject->SetPosition(-3 * 50 - 110, 0, -2 * 50 + (i * 200));
			else pBackObject->SetPosition(-3 * 50 + 115, 0, -2 * 50 + (i % (m_nBackObjects / 2)) * 200);
			pBackObject->SetScale(50.f, 50.f, 50.f);
			break;
		case 2:			// 돌 2
			pBackModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/Rock2.bin");
			pBackObject->type = 2;
			pBackObject->SetChild(pBackModel, true);
			if (i < (m_nBackObjects / 2))pBackObject->SetPosition(-3 * 50 - 110, 0, -3.267579 * 50 + (i * 200));
			else pBackObject->SetPosition(-3 * 50 + 115, 0, -3.267579 * 50 + (i % (m_nBackObjects / 2)) * 200);
			pBackObject->SetScale(50.f, 50.f, 50.f);
			break;
		case 3:			// 나무
			pBackModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/Tree.bin");
			pBackObject->type = 3;
			pBackObject->SetChild(pBackModel, true);
			if (i < (m_nBackObjects / 2))pBackObject->SetPosition(100.0f, 0.0f, (i * 200));
			else pBackObject->SetPosition(-100.0f, 0.0f,(i % (m_nBackObjects / 2)) * 200);
			pBackObject->SetScale(10.f, 10.f, 10.f);
			break;
		}
		pBackObject->Rotate(0.0f, 0.0f, 0.0f);
		m_ppBackObjects[i] = pBackObject;
	}
	//------------------------------------------------------------------------
	m_nGameObjects = 4;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];

	CGameObject *pHummerModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/Hummer.bin");
	CHummerObject* pHummerObject = NULL;

	pHummerObject = new CHummerObject();
	pHummerObject->SetChild(pHummerModel, true);
	pHummerObject->OnInitialize();
	// 9.227764 0.1698588 -4.310363
	pHummerObject->m_xmf3ModelPosition = XMFLOAT3(92.27764, 1.698588, -43.10363);
	pHummerObject->SetPosition(-92.27764f, -1.698588f, 43.10363f);
	pHummerObject->SetScale(10.f, 10.f, 10.f);
	pHummerObject->Rotate(0.0f, 0.0f, 0.0f);
	pHummerObject->m_xmf3BodyExtents = XMFLOAT3(20, 10, 40);
	pHummerObject->m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), pHummerObject->m_xmf3BodyExtents, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppGameObjects[0] = pHummerObject;

	CGameObject *pPoliceCarModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/PoliceCar.bin");
	CPoliceCarObject* pPoliceCarObject = NULL;

	pPoliceCarObject = new CPoliceCarObject();
	pPoliceCarObject->SetChild(pPoliceCarModel, true);
	pPoliceCarObject->OnInitialize();
	// 0 0 0
	pPoliceCarObject->SetPosition(0,0,0);
	pPoliceCarObject->SetScale(12.f, 12.f, 12.f);
	pPoliceCarObject->Rotate(0.0f, 0.0f, 0.0f);
	pPoliceCarObject->m_xmf3BodyExtents = XMFLOAT3(20, 10, 40);
	pPoliceCarObject->m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), pPoliceCarObject->m_xmf3BodyExtents, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppGameObjects[1] = pPoliceCarObject;

	CGameObject *pRallyCarModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Models/RallyCar.bin");
	CRallyCarObject* pRallyCarObject = NULL;

	pRallyCarObject = new CRallyCarObject();
	pRallyCarObject->SetChild(pRallyCarModel, true);
	pRallyCarObject->OnInitialize();
	//13.50904 0.01345238 -0.2916864
	pRallyCarObject->m_xmf3ModelPosition = XMFLOAT3(13.50904 * 12, 0.01345238 * 12, -0.2916864 * 12);
	pRallyCarObject->SetPosition(-13.50904 * 12, -0.01345238 * 12, 0.2916864 * 12);
	pRallyCarObject->SetScale(12.f, 12.f, 12.f);
	pRallyCarObject->Rotate(0.0f, 0.0f, 0.0f);
	pRallyCarObject->m_xmf3BodyExtents = XMFLOAT3(20, 10, 40);
	pRallyCarObject->m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), pRallyCarObject->m_xmf3BodyExtents, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_ppGameObjects[2] = pRallyCarObject;

	CGameObject* pRoadModel = CGameObject::MakeRoad(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);			//  길
	CRoadObject* pRoadObject = new CRoadObject();
	pRoadObject->SetChild(pRoadModel, true);
	pRoadObject->SetPosition(0, 0, 0);
	pRoadObject->SetScale(1, 1, 1);
	pRoadObject->Rotate(0, 0, 0);
	m_ppGameObjects[3] = pRoadObject;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': m_ppGameObjects[0]->MoveForward(+1.0f); break;
		case 'S': m_ppGameObjects[0]->MoveForward(-1.0f); break;
		case 'A': m_ppGameObjects[0]->MoveStrafe(-1.0f); break;
		case 'D': m_ppGameObjects[0]->MoveStrafe(+1.0f); break;
		case 'Q': m_ppGameObjects[0]->MoveUp(+1.0f); break;
		case 'R': m_ppGameObjects[0]->MoveUp(-1.0f); break;
		case VK_LEFT:
			if (m_pPlayer->m_iBeforeLine == m_pPlayer->m_iMovetoLine) {
				m_pPlayer->m_iMovetoLine--;
				if (m_pPlayer->m_iMovetoLine < 0) {
					m_pPlayer->m_iMovetoLine = 0;
				}
				cout << "m_iMoveLine : " << m_pPlayer->m_iMovetoLine << endl;
				//cout << "("<<m_pPlayer->m_pChild->m_xmOOBB.Center.x<<","<< m_pPlayer->m_pChild->m_xmOOBB.Center.z<<")" << endl;
				cout << "------------------------------------------------------------------" << endl;
			}
			break;
		case VK_RIGHT:
			if (m_pPlayer->m_iBeforeLine == m_pPlayer->m_iMovetoLine) {
				m_pPlayer->m_iMovetoLine++;
				if (m_pPlayer->m_iMovetoLine > 2) {
					m_pPlayer->m_iMovetoLine = 2;
				}
				std::cout << "m_iMoveLine : " << m_pPlayer->m_iMovetoLine << endl;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->UpdateBoundingBox();
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
	//m_ppGameObjects[3]->Animate(m_fElapsedTime, NULL);
	//m_ppGameObjects[3]->UpdateTransform(NULL);
	//m_ppGameObjects[3]->Render(pd3dCommandList, pCamera);
	for (int i = 0; i < m_nBackObjects; ++i)
	{
		if (m_ppBackObjects[i])
		{
			m_ppBackObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppBackObjects[i]->UpdateTransform(NULL);
			m_ppBackObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
}

