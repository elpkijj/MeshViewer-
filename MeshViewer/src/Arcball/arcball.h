#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#include "geometry.h"
#include "quat.h"
//通过鼠标拖动来旋转3D对象
class CArcball
{
public:
  CArcball(){};

  CArcball( int width, int height, int ox, int oy );
  CQrot update( int nx, int ny );


private:
  void _plane2sphere( const Vector2D & v, Vector3D & r );

  Vector3D m_position;
  double   m_radius;
  Vector2D m_center;
  double width;
  double height;
};


#endif