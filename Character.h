#ifndef CHARACTER_H
#define CHARACTER_H

#include "Camera.h"
//#include "Mesh.h"
#include "GameTimer.h"
#include "AnimationX.h"
#include "Graphics.h"
class Character : public Camera {
public:
    LPDIRECT3DDEVICE9 m_Device;

	Character( LPDIRECT3DDEVICE9 Device);
	~Character();

	virtual bool Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;

	//Mesh characterMesh;
	D3D::BoundingBox box;
    AnimationX* m_Avatar;

	D3DXMATRIX P, T, S;

	bool isAuto;
	int currentState;
	virtual enum STATES;
    bool LoadXFile( const std::string &filename, int startAnimation );
    void NextAnimation( );
    void AnimateFaster( );
    void AnimateSlower( );

	float speed, strafeSpeed, turnSpeed;
};

#endif