#ifndef OMNI_GRAPHICS_H
#define OMNI_GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

#define screenWidth 1024
#define screenHeight 768

// Forward declerations to eliminate header file cycles
class Input;
class Mouse;
class Keyboard;
class Sprite;
class Texture;
class GameState;

extern IDirect3DDevice9* Device;

namespace D3D
{

    bool InitD3D( HINSTANCE hInstance );

    LRESULT CALLBACK MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    D3DMATERIAL9 InitMtrl( D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p );

    const D3DXCOLOR		 WHITE( D3DCOLOR_XRGB( 255, 255, 255 ) );
    const D3DXCOLOR		 BLACK( D3DCOLOR_XRGB( 0, 0, 0 ) );
    const D3DXCOLOR        RED( D3DCOLOR_XRGB( 255, 0, 0 ) );
    const D3DXCOLOR      GREEN( D3DCOLOR_XRGB( 0, 255, 0 ) );
    const D3DXCOLOR       BLUE( D3DCOLOR_XRGB( 0, 0, 255 ) );
    const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB( 255, 255, 0 ) );
    const D3DXCOLOR       CYAN( D3DCOLOR_XRGB( 0, 255, 255 ) );
    const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB( 255, 0, 255 ) );

    //const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
    //const D3DMATERIAL9 BLACK_MTRL = InitMtrl(BLACK, BLACK, BLACK, BLACK, 2.0f);

    D3DLIGHT9 InitDirectionalLight( D3DXVECTOR3* direction, D3DXCOLOR* color );
    D3DLIGHT9 InitPointLight( D3DXVECTOR3* position, D3DXCOLOR* color );
    D3DLIGHT9 InitSpotLight( D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color );

    IDirect3DTexture9 * LoadTexture(char * fileName);



     //	lerp for terrain interpolation
    float Lerp( float a, float b, float t );

    //	convert a float value to dword
    DWORD FtoDw( float f );
    //	get a random float
    float GetRandomFloat( float min, float max );
    //	get a random vector
    void GetRandomVector( D3DXVECTOR3 *out, D3DXVECTOR3 *min, D3DXVECTOR3 *max );

    //	structures for animated mesh container
    struct D3DXMESHCONTAINER_EXTENDED: public D3DXMESHCONTAINER
    {
        IDirect3DTexture9** exTextures;
        D3DMATERIAL9* exMaterials;

        ID3DXMesh* exSkinMesh;
        D3DXMATRIX* exBoneOffsets;
        D3DXMATRIX** exFrameCombinedMatrixPointer;
    };

    //	structure for extended frame for animated meshes
    struct D3DXFRAME_EXTENDED: public D3DXFRAME
    {
        D3DXMATRIX exCombinedTransformationMatrix;
    };

    //	vertex struct for environment objects
    struct EVertex
    {
        EVertex( ) { }
        EVertex( float X, float Y, float Z, float U, float V ): x( X ), y( Y ), z( Z ), u( U ), v( V ) { }
        float x, y, z, u, v;
        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };

    struct BoundingBox
    {
        BoundingBox( ): MIN( INFINITY, INFINITY, INFINITY ), MAX( -INFINITY, -INFINITY, -INFINITY ) { }

        D3DXVECTOR3 center( )
        {
            return 0.5f*(MIN + MAX);
        }

		bool isPointInside(D3DXVECTOR3 &p) {

		}

		bool isCollision(BoundingBox *box) {
			if (isPointInside(box->MIN) || isPointInside(box->MAX))
				return true;
			return false;
		}

        D3DXVECTOR3 MIN, MAX;
    };


    struct BoundingSphere
    {
        BoundingSphere( );

        D3DXVECTOR3 _center;
        float _radius;
    };


    template<class T> void Release( T t )
    {
        if( t )
        {
            t->Release( );
            t = 0;
        }
    }

    template<class T> void Delete( T t )
    {
        if( t )
        {
           delete  t;
           t = 0;
        }
    }


}



class Graphics
{
private:
    bool m_vsync_enabled;
    int m_viideoCardMemrory;
    char m_videoCardDexcription[ 128 ];
	// LPDIRECT3DDEVICE9 Device;
    LPDIRECT3D9 m_D3DInterface;
    D3DXMATRIX m_projectionMatrix;
    D3DXMATRIX m_worldMatrix;
    D3DXMATRIX m_orthoMatrix;
    //Input * m_Input;
    Sprite* sprt;
    Texture* tex;
public:
    D3DPRESENT_PARAMETERS d3dpp; //create a struct ot hold device ingormation
    RECT m_ScreneRect;
    IDirect3DSurface9* m_Cursor;
    GameState* m_MainMenu, *m_GameWorld, *m_Story, *m_CurrentState, *m_PreviousState, *m_Credits, *m_Options;
    Graphics( );
    ~Graphics( );
    bool Initialized( int height, int width, HINSTANCE hInstance );
    void Shutdown( );
    void RecvMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    bool Render( );
    void Update( );
    void SetScreenRect( );
   // LPDIRECT3DDEVICE9 GetDevice( ) { return Device; }
    void GetProjectionMatrix( D3DXMATRIX& projMat ) { projMat = m_projectionMatrix; }
    void GetWorldMatrix( D3DXMATRIX& worldMat ) { worldMat = m_worldMatrix; }
    void GetOrthoMatrix( D3DXMATRIX& orthoMat ) { orthoMat = m_orthoMatrix; }
};

extern Graphics graphics;

#endif // OMNI_GRAPHICS_H
