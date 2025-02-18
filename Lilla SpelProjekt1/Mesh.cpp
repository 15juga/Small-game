#include "Mesh.h"

void MeshT::loadObj(const char* path, std::vector<Vertex>& mesh)
{
	std::vector<Vector3> vertex;
	std::vector<Vector3> normal;
	std::vector<Vector2> uv;

	std::vector<DWORD> vIndex;
	std::vector<DWORD> nIndex;
	std::vector<DWORD> uIndex;

	std::stringstream ss;
	std::ifstream file(path);
	std::string line;

	if (!file.is_open())
	{
		ErrorLogger::Log("Error: Failed to find file");
	}

	std::string pf;
	Vector3 v3Temp; //Container for position och senare ers�tts med normal
	Vector2 v2Temp;
	DWORD iTemp = 0;
	while (std::getline(file, line)) {
		ss.clear();
		ss.str(line);
		ss >> pf;

		if (pf == "v") {
			ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
			vertex.push_back(v3Temp);

			// add vertices into a pointer that is used for building bounding volumes
			//pPosV[nrVertices] = v3Temp;
			//nrVertices++;
		}
		else if (pf == "vt") { //uv
			ss >> v2Temp.x >> v2Temp.y;
			uv.push_back(v2Temp);
		}
		else if (pf == "vn") {
			ss >> v3Temp.x >> v3Temp.y >> v3Temp.z;
			normal.push_back(v3Temp);
		}
		else if (pf == "f") { //pos, uv , normal
			uint32_t count = 0;

			while (ss >> iTemp) {
				switch (count) {
				case 0:
					vIndex.push_back(iTemp - 1); //R�knar fr�n 0-1 obj filer r�knar fr�n 1 och up�tt
					break;
				case 1:
					uIndex.push_back(iTemp - 1);
					break;
				case 2:
					nIndex.push_back(iTemp - 1);
					break;
				}

				if (ss.peek() == '/') {
					count++;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					count++;
					ss.ignore(1, ' ');
				}

				if (count > 2)
					count = 0;
			}
		}
	}


	mesh.resize(vIndex.size(), Vertex());
	nrVertices = vIndex.size();

	for (int i = 0; i < vIndex.size(); i++) {
		mesh[i].pos = vertex[vIndex[i]];
		mesh[i].normals = normal[nIndex[i]];
		mesh[i].uv = uv[uIndex[i]];

		pPosV[i] = vertex[vIndex[i]];
	}
	file.close();
}

void MeshT::Load(const char* path, std::vector<Vertex>& mesh, ACJL::Material& material)
{
	LIB::Reader::LoadMesh(path, mesh, material);
	this->pPosV = new Vector3[mesh.size()];
	nrVertices = mesh.size();
	count = mesh.size();
	for (int i = 0; i < mesh.size(); i++)
	{
		pPosV[i] = mesh[i].pos;
	}
}

MeshT::MeshT()
{
	
}

int MeshCluster::index = { 0 };

MeshT::MeshT(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//this->ConstantBuffer = &ConstantBuffer;
	this->device = device;
	this->deviceContext = deviceContext;
	//MeshCluster::Get().emplace(MeshCluster::index, this);
	MeshCluster::Get().push_back(this);
	MeshCluster::index++;
}

MeshT::MeshT(const MeshT& mesh)
{
	this->vertexBuffer = mesh.vertexBuffer;
	this->deviceContext = mesh.deviceContext;
	this->device = mesh.device;
//	this->ConstantBuffer = mesh.ConstantBuffer;
	this->count = mesh.count;
	this->shape = mesh.shape;
	this->pPosV = mesh.pPosV;
	this->nrVertices = mesh.nrVertices;
}

void MeshT::loadMesh(const char* path, ID3D11Device*& device)
{
	//loadObj(path, shape);
	ACJL::Material padding;
	Load(path, shape, padding);
	count = shape.size();

	HRESULT hr = vertexBuffer.Initialize(shape.data(), shape.size(), device);
	if (FAILED(hr)) {

		ErrorLogger::Log(hr, "Failed to load object");
	}
}

void MeshT::loadMesh(const char* path, ID3D11Device*& device, std::wstring texturePath)
{
	//loadObj(path, shape);
	Load(path, shape, this->material);
	this->textureFile.initTextureFile(texturePath, device);
	//this->texture.
	count = shape.size();

	HRESULT hr = vertexBuffer.Initialize(shape.data(), shape.size(), device);
	if (FAILED(hr)) {

		ErrorLogger::Log(hr, "Failed to load object");
	}
}

void MeshT::initMesh(const char* path, ID3D11Device*& device, ID3D11DeviceContext*& devContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	Load(path, shape, this->material);
	count = shape.size();

	std::string txtPath = path;
	int steps = txtPath.rfind("/");
	txtPath.erase(steps+1, txtPath.length());
	txtPath += material.albedoName;
	std::wstring textureWstring = StringConverter::StringToWide(txtPath);

	this->textureFile.initTextureFile(textureWstring, device);
	this->device = device;
	this->deviceContext = devContext;
	this->ConstantBuffer = &ConstantBuffer;

	HRESULT hr = vertexBuffer.Initialize(shape.data(), shape.size(), device);
	if (FAILED(hr)) {

		ErrorLogger::Log(hr, "Failed to load object");
	}
}

//get position of vertices as array pointer
const XMFLOAT3* MeshT::getpPos()
{
	return this->pPosV;
}

const size_t MeshT::getnrV()
{
	return this->nrVertices;
}

const int MeshT::getCount()
{
	return this->count;
}

void MeshT::RenderTexture(ID3D11DeviceContext* devicecontext, ID3D11Device* device)
{
}

const bool& MeshT::isLoaded() const
{
	return true;
}

void MeshT::Draw( ID3D11DeviceContext*& deviceContext)
{
	//Setting data
	UINT offset = 0;
	/*for (int i = 0; i < textures.size(); i++)
	{*/
		//this->deviceContext->PSSetShaderResources(0, 1, textures[0].getTextureResourceViewAdress());
	//}
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer.Get(), vertexBuffer.Stride(), &offset);
	deviceContext->PSSetShaderResources(0, 1, this->textureFile.getTextureResourceViewAdress());
}

void MeshT::Pdraw(ID3D11DeviceContext*& deviceContext)
{
	//Setting data
	UINT offset = 0;
	/*for (int i = 0; i < textures.size(); i++)
	{*/
	//this->deviceContext->PSSetShaderResources(0, 1, textures[0].getTextureResourceViewAdress());
//}
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer.Get(), vertexBuffer.Stride(), &offset);
	deviceContext->PSSetShaderResources(0, 1, this->textureFile.getTextureResourceViewAdress());
}

ID3D11DeviceContext*& MeshT::getDeviceContext()
{
	return this->deviceContext;
}

ID3D11Device*& MeshT::getDevice()
{
	return this->device;
}

constantBuffer<MatrixVertex>*& MeshT::getConstantBuffer()
{
	return this->ConstantBuffer;
}

MeshT::~MeshT()
{
	pPosV = nullptr;
	delete[] this->pPosV;
}
