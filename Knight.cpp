#include "Knight.h"
#include <Windowsx.h>


Knight::Knight()
{
    m_prevYaw=0, m_prevPitch=0, m_CurrentYaw=0, m_CurrentPitch=0;
}


Knight::~Knight()
{
}

bool Knight::Initialize()
{
	//	set speed variables
	speed = 10000.0f;
	strafeSpeed = 10000.0f;
	turnSpeed = 200.0f;

	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };

	//	initialize the physical mesh object
	characterMesh.Initialize("models/knight.x");

	//	set the scale
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	return true;
}

void Knight::Render()
{
	IDirect3DDevice9* Device = graphics.GetDevice();

	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = S*T;
	Device->SetTransform(D3DTS_WORLD, &P);

	characterMesh.Render();

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box.MIN, &characterMesh.min, &P);
	D3DXVec3TransformCoord(&box.MAX, &characterMesh.max, &P);
}

void Knight::Update( )
{
    //	update third person camera position (rear view)
    thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;
}

void Knight::GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    if( !isAuto )
    {
        //int l_deltaYaw = 0, l_deltaPitch = 0;

        //m_prevPitch = m_CurrentPitch;
        //m_prevYaw = m_CurrentYaw;

        //m_CurrentYaw = GET_Y_LPARAM( lParam );
        //m_CurrentPitch = GET_X_LPARAM( lParam );
        //
        //l_deltaPitch =  m_CurrentPitch- m_prevPitch;
        //l_deltaYaw= m_CurrentYaw- m_prevYaw ;
        ////this is code to use the message handler, maybe later
        //yaw( timer.DeltaTime( )*l_deltaPitch );
        //pitch( timer.DeltaTime( )*l_deltaYaw );

        //switch( msg )
        //{
        //case WM_KEYDOWN:
        //    switch( wParam )//switch on key down
        //    {
            //case VK_RIGHT:
            //    yaw( timer.DeltaTime( )* turnSpeed );
            //    if(turnSpeed<200.0f )
            //        turnSpeed++;
            //    break;
            //case VK_LEFT:
            //    yaw( -timer.DeltaTime( ) * turnSpeed );
            //    if( turnSpeed<200.0f )
            //        turnSpeed++;
            //    break;
            //case VK_UP:
            //    pitch( -timer.DeltaTime( )* turnSpeed );
            //    if( turnSpeed<200.0f )
            //        turnSpeed++;
            //    break;
            //case VK_DOWN:
            //    pitch( timer.DeltaTime( ) * turnSpeed );
            //    if( turnSpeed<200.0f )
            //        turnSpeed++;
            //    break;
        //    case 'W':
        //        walk( timer.DeltaTime( ) * speed );
        //        if( speed<10000.0f )
        //            speed += 50.0f;
        //        break;
        //    case 'S':
        //        walk( -timer.DeltaTime( ) * speed );
        //        if( speed<10000.0f )
        //            speed += 50.0f;
        //        break;
        //    case 'A':
        //        strafe( -timer.DeltaTime( ) * strafeSpeed );
        //        if( strafeSpeed<10000.0f )
        //            strafeSpeed+= 50.0f;
        //        break;
        //    case 'D':
        //        strafe( timer.DeltaTime( ) * strafeSpeed );
        //        if( strafeSpeed<10000.0f )
        //            strafeSpeed += 50.0f;
        //        break;
        //    default:
        //        break;
        //    }break;
        //case WM_KEYUP:
        //{
        //    switch( wParam )//switch on key down
        //    {
            //case VK_RIGHT:
            //    turnSpeed = 0;
            //    //break;
            //case VK_LEFT:
            //    turnSpeed = 0;
            //    //break;
            //case VK_UP:
            //    turnSpeed = 0;
            //    //break;
            //case VK_DOWN:
            //    turnSpeed = 0;
            //    //break;
        //    case 'W':
        //        speed = 1000;
        //        break;
        //    case 'S':
        //        speed = 1000;
        //        break;
        //    case 'A':
        //        strafeSpeed = 1000;
        //        break;
        //    case 'D':
        //        strafeSpeed = 1000;
        //        break;
        //    default:
        //        break;
        //    }
        //}break;
        //}

        //using asynckeystate...
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) {
			yaw(timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000f) {
			yaw(-timer.DeltaTime() * turnSpeed);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000f) {
			pitch(-timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000f) {
			pitch(timer.DeltaTime() * turnSpeed);
		}
		if (GetAsyncKeyState('W') & 0x8000f) {
			walk(timer.DeltaTime() * speed);
		}
		if (GetAsyncKeyState('S') & 0x8000f) {
			walk(-timer.DeltaTime() * speed);
		}
		if (GetAsyncKeyState('A') & 0x8000f) {
			strafe(-timer.DeltaTime() * strafeSpeed);
		}
		if (GetAsyncKeyState('D') & 0x8000f) {
			strafe(timer.DeltaTime() * strafeSpeed);
		}
	}
	//else {
		//	do ai for knight here
		//	maybe strafe back and forth or whatever
	//}	
}

void Knight::Reset()
{
	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };
}


D3DXMATRIX Knight::getRearView()
{
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(0, 1, 0));
	return V;
}

D3DXMATRIX Knight::getSideView()
{
	D3DXMATRIX V;
	thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(1, 0, 0));

	return V;
}
