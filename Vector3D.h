#pragma once
class Vector3D
{
	float m_x;
	float m_y;
	float m_z;
public:
	Vector3D(float x, float y,float z);
	~Vector3D();
	void AddVec(Vector3D vec);
	void SubVec(Vector3D vec);
};

