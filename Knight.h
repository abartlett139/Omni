#ifndef KNIGHT_H
#define KNIGHT_H

#include "Character.h"

class Knight: public Character
{
public:
    Knight( );
    ~Knight( );

    bool Initialize( );
    void Render( );
    void Update( );
    void Reset( );
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    D3DXMATRIX getRearView( );
    D3DXMATRIX getSideView( );

    Camera thirdPersonCamera;

    int m_prevYaw, m_prevPitch, m_CurrentYaw, m_CurrentPitch;

};

#endif
