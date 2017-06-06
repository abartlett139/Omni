#pragma once
#ifndef PSYS_H
#define PSYS_H

#include "Graphics.h"
#include "Camera.h"
#include "GameTimer.h"
#include <list>

namespace PSYS {

	struct Particle {
		D3DXVECTOR3 position;
		D3DCOLOR color;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};

	struct Attribute {
		Attribute() {
			lifeTime = 0.0f;
			age = 0.0f;
			isAlive = true;
		}

		D3DXVECTOR3 position;
		D3DXVECTOR3 velocity;
		D3DXVECTOR3 acceleration;
		float lifeTime;
		float age;
		D3DXCOLOR color;
		D3DXCOLOR colorFade;
		bool isAlive;
	};

	class ParticleSystem {
	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		virtual bool Init(char * fileName);
		virtual void Reset();

		virtual void ResetParticle(Attribute* attribute) = 0;
		virtual void AddParticle();

		virtual void Update() = 0;

		virtual void PreRender();
		virtual void Render();
		virtual void PostRender();

		bool isEmpty();
		bool isDead();

		virtual void RemoveDeadParticles();

		D3DXVECTOR3 origin;
		D3D::BoundingBox box;
		float emitRate;
		float size;
		IDirect3DTexture9* texture;
		IDirect3DVertexBuffer9* vertexBuffer;
		std::list<Attribute> particles;
		int maxParticles;

		DWORD vertexBufferSize;
		DWORD vertexBufferOffset;
		DWORD vertexBufferBatchSize;
	};

	class ParticleGun : public ParticleSystem {
	public:
		ParticleGun(Camera* camera);
		void ResetParticle(Attribute* attribute);
		void Update();

		Camera* camera;
	};
}

#endif