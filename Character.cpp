#include "Character.h"

Character::Character()
{
	isAuto = true;
}

Character::~Character()
{
}

void Character::IdleState()
{
	D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
	m_Enemy->getPosition(&l_KnightLoc);
	//printf("Knight: x %f, y %f", l_KnightLoc.x, l_KnightLoc.y);
	//Calculate vector pointing from dragon to knight	
	D3DXVECTOR3 l_TempDirection{ 0,0,0 };
	D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
	D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
	float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);

	D3DXVECTOR3 l_dragonLookDir{ 0,0,0 };
	getLook(&l_dragonLookDir);
	D3DXVec3Normalize(&l_dragonLookDir, &l_dragonLookDir);

	float l_angle = D3DXVec3Dot(&l_dragonLookDir, &l_TempDirection);

	// turn dragon to face the knight, if the angle between the two is greater than a small theshold
	if (fabs(l_angle) >= 0.001f)
	{
		yaw(l_angle);
	}
	printf("Distance: %f \n", l_dragon_knight_distance);
	printf("computed distance: %f", (2 * speed));
	if (l_dragon_knight_distance <= (2 * speed))
	{
		m_State = IDLE;
		printf("Dragon is idle!\n");
	}
}

void Character::GoHome()
{
	if (m_Enemy)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_Enemy{ 0,0,0 };
		m_Enemy->getPosition(&l_Enemy);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_TempDirection, &l_Enemy, &_pos);
		float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);
		float l_temp = (timer.DeltaTime()) - m_FleeTimer;
		if ((l_dragon_knight_distance >= (5 * speed)) && (l_temp > .1))
		{
			m_State = IDLE;
			printf("Dragon ate you; O' the indignity & travesty!\n");
			m_FleeTimer = 0;
		}

		D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
		_pos += l_TempDirection*-speed;
	}
}

void Character::ChaseState()
{
	if (m_Enemy)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_Enemy->getPosition(&l_KnightLoc);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
		float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);

		if (l_dragon_knight_distance <= (3 * speed))
		{
			m_State = ATTACK;
			m_FleeTimer = timer.DeltaTime();
			printf("Dragon is chasing you!\n");
		}
		D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
		_pos += l_TempDirection*speed* (timer.DeltaTime() * 10);
	}
}

void Character::AttackState()
{
	if (m_Enemy)
	{
		D3DXVECTOR3 l_dragon_knight_direction{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_Enemy->getPosition(&l_KnightLoc);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_dragon_knight_direction, &l_KnightLoc, &_pos);
		D3DXVec3Normalize(&l_dragon_knight_direction, &l_dragon_knight_direction);

		D3DXVECTOR3 l_dragonLookDir{ 0,0,0 };
		getLook(&l_dragonLookDir);
		D3DXVec3Normalize(&l_dragonLookDir, &l_dragonLookDir);

		float l_angle = D3DXVec3Dot(&l_dragonLookDir, &l_dragon_knight_direction);

		// Turn dragon to face the knight
		// Dont bother turning the dragon unless the angle between the two is greater than a small theshold
		// The angle can be negative or positive, so we take the absolute value because all we care about is the 
		// magnitude of the turn 
		// Not turning the dragon for a very small angle is only an optimization
		if (fabs(l_angle) >= 0.001f)
		{
			yaw(l_angle);
		}

		// To calculate a vector that is 90 degrees tangent to another pair of vectors, we use
		// a cross product.  The input to the cross product are two vectors that define a plant, in 
		// this case a vector pointing from the dragon to the knight, and the second vector a vector 
		// pointing up from the dragon.  The cross product will give a vector that points out of that 
		// plane by 90 degrees
		D3DXVECTOR3 l_dragon_knight_tangent{ 0, 0, 0 };
		D3DXVECTOR3 l_dragon_up{ 0, 0, 0 };
		getUp(&l_dragon_up);
		D3DXVec3Cross(&l_dragon_knight_tangent, &l_dragon_knight_direction, &l_dragon_up);

		// We have to normalize the distance vector because we want it to only 'point'
		// in the direction we are interested in.  We are going to multiply the vector
		// by our speed to scale the vector to update our postion.  A vector of length
		// 1 just represents a direction, which we then multiply by our speed to update
		// our position
		D3DXVec3Normalize(&l_dragon_knight_tangent, &l_dragon_knight_tangent);

		//if (l_dragon_knight_distance >= (2 * speed))
		//{
		//	m_State = IDLE;
		//	printf("Dragon ate you; O' the indignity & travesty!\n");
		//}
		float l_temp = (timer.DeltaTime()) - m_AttackTimer;

		// (timer.DeltaTime( ) * 10) compensates for fast computers
		if (m_AttackTimer < 5)
		{
			_pos += l_dragon_knight_tangent * speed * (timer.DeltaTime() * 10);
		}
		else
		{
			m_State = LUNGE;
			m_AttackTimer = 0;
		}

	}
}

void Character::FleeState()
{
	if (m_Enemy)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_Enemy->getPosition(&l_KnightLoc);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
		float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);
		float l_temp = (timer.DeltaTime()) - m_FleeTimer;
		if ((l_dragon_knight_distance >= (5 * speed)) && (l_temp > .1))
		{
			m_State = IDLE;
			printf("Dragon ate you; O' the indignity & travesty!\n");
			m_FleeTimer = 0;
		}

		D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
		_pos += l_TempDirection*-speed;
	}
}

void Character::LungeState()
{
	if (m_Enemy)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_Enemy->getPosition(&l_KnightLoc);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
		float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);

		if (l_dragon_knight_distance <= (1 * speed))
		{
			m_State = ATTACK;
			m_FleeTimer = timer.DeltaTime();
			printf("Ha, Ha, I Lunge");
		}
		D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
		_pos += l_TempDirection*speed;
	}
}
