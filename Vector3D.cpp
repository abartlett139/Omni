#include "Vector3D.h"


Vector3D::Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
}


Vector3D::~Vector3D()
{

}

void Vector3D::AddVec(Vector3D vec)
{
	m_x = m_x + vec.m_x;
	m_y = m_y + vec.m_y;
	m_z = m_z + vec.m_z;
}

void Vector3D::SubVec(Vector3D vec)
{
	m_x = m_x - vec.m_x;
	m_y = m_y - vec.m_y;
	m_z = m_z - vec.m_z;
}
