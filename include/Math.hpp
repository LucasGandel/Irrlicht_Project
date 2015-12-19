#ifndef MATH_HPP
#define MATH_HPP

//Clamp a value in range [lowValue,hightValue]
void clamp( float* param, float lowValue, float hightValue )
{
  if( *param > hightValue )
    {
    *param = hightValue;
    }
  if( *param < lowValue )
    {
    *param = lowValue;
    }
}

//Clamp an angle value in range [-180,180]
void clampAsAngle( float* angle )
{
  if( *angle > 180.f )
    {
    *angle -= 360.0f;
    }
  if( *angle <- 180.f )
    {
    *angle += 360.0f;
    }
}

#endif // MATH_HPP
