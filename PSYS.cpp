#include "PSYS.h"


PSYS::ParticleSystem::ParticleSystem() : vertexBuffer(0), texture(0)
{
}

PSYS::ParticleSystem::~ParticleSystem()
{
	D3D::Release(vertexBuffer);
	D3D::Release(texture);
}

bool PSYS::ParticleSystem::Init(char * fileName)
{
	//	IDirect3DDevice9* Device = graphics.GetDevice();
	Device->CreateVertexBuffer(vertexBufferSize * sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY, Particle::FVF, D3DPOOL_DEFAULT, &vertexBuffer, 0);
	
	D3DXCreateTextureFromFile(Device, fileName, &texture);
	
	return true;
}

void PSYS::ParticleSystem::Reset()
{
	std::list<Attribute>::iterator i;
	for (i = particles.begin(); i != particles.end(); i++) {
		ResetParticle(&(*i));
	}
}

void PSYS::ParticleSystem::AddParticle()
{
	Attribute attribute;

	ResetParticle(&attribute);

	particles.push_back(attribute);
}

void PSYS::ParticleSystem::PreRender()
{
	//	IDirect3DDevice9* Device = graphics.GetDevice();
	Device->SetRenderState(D3DRS_LIGHTING, false);
	Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	Device->SetRenderState(D3DRS_POINTSIZE, D3D::FtoDw(size));
	Device->SetRenderState(D3DRS_POINTSIZE_MIN, D3D::FtoDw(0.0f));

	Device->SetRenderState(D3DRS_POINTSCALE_A, D3D::FtoDw(0.0f));
	Device->SetRenderState(D3DRS_POINTSCALE_B, D3D::FtoDw(0.0f));
	Device->SetRenderState(D3DRS_POINTSCALE_C, D3D::FtoDw(1.0f));

	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	
}

void PSYS::ParticleSystem::Render()
{
	if (!particles.empty()) {
		PreRender();

		//	IDirect3DDevice9* Device = graphics.GetDevice();
		Device->SetTexture(0, texture);
		Device->SetFVF(Particle::FVF);
		Device->SetStreamSource(0, vertexBuffer, 0, sizeof(Particle));

		if (vertexBufferOffset >= vertexBufferSize)
			vertexBufferOffset = 0;

		Particle* v = 0;

		vertexBuffer->Lock(vertexBufferOffset * sizeof(Particle), vertexBufferBatchSize * sizeof(Particle), (void**)&v, vertexBufferOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
		DWORD numParticlesInBatch = 0;

		std::list<Attribute>::iterator i;
		for (i = particles.begin(); i != particles.end(); i++) {
			if (i->isAlive) {
				v->position = i->position;
				v->color = (D3DCOLOR)i->color;
				v++;
				numParticlesInBatch++;

				if (numParticlesInBatch == vertexBufferBatchSize) {
					vertexBuffer->Unlock();
					Device->DrawPrimitive(D3DPT_POINTLIST, vertexBufferOffset, vertexBufferBatchSize);

					vertexBufferOffset += vertexBufferBatchSize;

					if (vertexBufferOffset >= vertexBufferSize)
						vertexBufferOffset = 0;

					vertexBuffer->Lock(vertexBufferOffset * sizeof(Particle), vertexBufferBatchSize * sizeof(Particle), (void**)&v, vertexBufferOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0;
				}
			}
		}
		vertexBuffer->Unlock();

		if (numParticlesInBatch) {
			Device->DrawPrimitive(D3DPT_POINTLIST, vertexBufferOffset, numParticlesInBatch);
		}

		vertexBufferOffset += vertexBufferBatchSize;

		PostRender();
	}
}

void PSYS::ParticleSystem::PostRender()
{
	//	IDirect3DDevice9* Device = graphics.GetDevice();
	Device->SetRenderState(D3DRS_LIGHTING, true);
	Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool PSYS::ParticleSystem::isEmpty()
{
	return particles.empty();
}

bool PSYS::ParticleSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = particles.begin(); i != particles.end(); i++) {
		if (i->isAlive)
			return false;
	}

	return true;
}

void PSYS::ParticleSystem::RemoveDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = particles.begin();

	while (i != particles.end()) {
		if (!i->isAlive) {
			i = particles.erase(i);
		}
		else {
			i++;
		}
	}
}

PSYS::ParticleGun::ParticleGun(Camera * _camera)
{
	camera = _camera;
	size = 0.8f;
	vertexBufferSize = 2048;
	vertexBufferOffset = 0;
	vertexBufferBatchSize = 512;
}

void PSYS::ParticleGun::ResetParticle(Attribute * attribute)
{
	attribute->isAlive = true;

	D3DXVECTOR3 cameraPos;
	camera->getPosition(&cameraPos);

	D3DXVECTOR3 cameraDir;
	camera->getLook(&cameraDir);

	attribute->position = cameraPos;
	attribute->position.y -= 1.0f;

	attribute->velocity = cameraDir * 10.0f;

	attribute->color = D3D::BLUE;

	attribute->age = 0.0f;

	attribute->lifeTime = 1.0f;
}

void PSYS::ParticleGun::Update()
{
	std::list<Attribute>::iterator i;

	for (i = particles.begin(); i != particles.end(); i++) {
		i->position += i->velocity *timer.DeltaTime();

		i->age += timer.DeltaTime();

		if (i->age > i->lifeTime)
			i->isAlive = false;
	}

	RemoveDeadParticles();
}
