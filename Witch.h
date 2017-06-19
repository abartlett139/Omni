#ifndef WITCH_H
#define WITCH_H

#include "Character.h"
#include "PSYS.h"

class Witch: public Character
{
public:
    Witch( );
    ~Witch( );

    bool Initialize(Terrain * terrain);
    void Render( );
    void Update( );
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    void Reset( );

    D3DXMATRIX getRearView( );
    D3DXMATRIX getSideView( );

    Camera thirdPersonCamera;

    PSYS::ParticleGun* magic = 0;
};

#endif
