#include "Camera.h"
using namespace Utility;

#include <gtx\transform.hpp>
using glm::vec4;
using glm::mat3;

//---------------------------------------------------------------------------------------------------
Camera::Camera() : 
   m_viewVector(vec3(0.0f, 0.0f, -1.0f)),
   m_upVector(vec3(0.0f, 1.0f, 0.0f)),
   m_Speed(1.0f),
   m_MousePos(vec2(0, 0))
{

}

//---------------------------------------------------------------------------------------------------
Camera::Camera(vec3& pos) : Camera()
{
   m_Position = pos;
}

//---------------------------------------------------------------------------------------------------
Camera::~Camera()
{
}

//---------------------------------------------------------------------------------------------------
bool Camera::SetSpeed(float speed)
{
   if(speed <= 0.0f || speed > 5000.0f)
      return false;

   m_Speed = speed;

   return true;
}

//---------------------------------------------------------------------------------------------------
void Camera::Set(vec3& absPos, vec3& viewVector)
{
   m_oldPosition = m_Position;
   m_oldViewVector = m_viewVector;

   m_Position = absPos;
   m_viewVector = viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::ResetLast()
{
   m_Position = m_oldPosition;
   m_viewVector = m_oldViewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveForward()
{
   m_Position += m_Speed * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveBackward()
{
   m_Position -= m_Speed * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveUp()
{
   m_Position += m_Speed * m_upVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveDown()
{
   m_Position -= m_Speed * m_upVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveLeft()
{
   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_Position -= m_Speed * rightVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::MoveRight()
{
   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_Position += m_Speed * rightVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::RotateLeft()
{
   m_viewVector = mat3(glm::rotate(m_Speed * 0.01f, m_upVector)) * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::RotateRight()
{
   m_viewVector = mat3(glm::rotate(-m_Speed * 0.01f, m_upVector)) * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::RotateUp()
{
   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_viewVector = mat3(glm::rotate(-m_Speed * 0.01f, rightVector)) * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::RotateDown()
{
   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_viewVector = mat3(glm::rotate(m_Speed * 0.01f, rightVector)) * m_viewVector;
}

//---------------------------------------------------------------------------------------------------
void Camera::Reverse()
{
   m_viewVector *= -1.0f;
}

//---------------------------------------------------------------------------------------------------
void Camera::HandleMouse(vec2 mousePos)
{
   return;

   //if((mousePos.x - m_MousePos.x) > 100.0f)
   //   return;

   m_viewVector = mat3(glm::rotate((mousePos.x - m_MousePos.x) * 0.001f, m_upVector)) * m_viewVector;

   m_MousePos = mousePos;
}

//---------------------------------------------------------------------------------------------------
vec3 Utility::Camera::GetWorldLocation()
{
   return m_Position;
}

//---------------------------------------------------------------------------------------------------
mat4 Camera::GetViewMatrix()
{
   return glm::lookAt(m_Position, m_Position + m_viewVector, m_upVector);
}
