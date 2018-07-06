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
void Camera::LookFront()
{
   m_viewVector = vec3(0.0f, 0.0f, -1.0f);
}

//---------------------------------------------------------------------------------------------------
void Camera::LookBack()
{
    m_viewVector = vec3(0.0f, 0.0f, -1.0f);
    m_viewVector = mat3(glm::rotate(glm::radians(180.0f), m_upVector)) * m_viewVector;
    m_viewVector = glm::normalize(m_viewVector);
}

//---------------------------------------------------------------------------------------------------
void Camera::LookLeft()
{
   m_viewVector = vec3(0.0f, 0.0f, -1.0f);
   m_viewVector = mat3(glm::rotate(glm::radians(270.0f), m_upVector)) * m_viewVector;
   m_viewVector = glm::normalize(m_viewVector);
}

//---------------------------------------------------------------------------------------------------
void Camera::LookRight()
{
   m_viewVector = vec3(0.0f, 0.0f, -1.0f);
   m_viewVector = mat3(glm::rotate(glm::radians(90.0f), m_upVector)) * m_viewVector;
   m_viewVector = glm::normalize(m_viewVector);
}

//---------------------------------------------------------------------------------------------------
void Camera::LookUp()
{
   m_viewVector = vec3(0.0f, 0.0f, -1.0f);

   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_viewVector = mat3(glm::rotate(glm::radians(-90.0f), rightVector)) * m_viewVector;
   m_viewVector = glm::normalize(m_viewVector);
}

//---------------------------------------------------------------------------------------------------
void Camera::LookDown()
{
   m_viewVector = vec3(0.0f, 0.0f, -1.0f);

   vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
   m_viewVector = mat3(glm::rotate(glm::radians(90.0f), rightVector)) * m_viewVector;
   m_viewVector = glm::normalize(m_viewVector);
}


//---------------------------------------------------------------------------------------------------
void Camera::Look(int i)
{
   // based on the expected order in normal cubemapping
   // look forward/front is assumed to be in the default -Z direction
   switch(i) {
   case 0:
      LookRight();
      break;

   case 1:
      LookLeft();
      break;

   case 2:
      LookUp();
      break;

   case 3:
      LookDown();
      break;

   case 4:
      LookBack();
      break;

   case 5:
      LookFront();
      break;
   }
}

//---------------------------------------------------------------------------------------------------
void Camera::HandleMouse(vec2 mousePos, bool updatePositionOnly)
{
   if(!updatePositionOnly) {
      float deltaX = mousePos.x - m_MousePos.x;
      float deltaY = mousePos.y - m_MousePos.y;

      // horizontal rotation
      m_viewVector = mat3(glm::rotate(deltaX * 0.001f, m_upVector)) * m_viewVector;

      // vertical rotation
      vec3 rightVector = glm::normalize(glm::cross(m_viewVector, m_upVector));
      m_viewVector = mat3(glm::rotate(deltaY * 0.001f, rightVector)) * m_viewVector;
   }

   m_MousePos = mousePos;
}

//---------------------------------------------------------------------------------------------------
vec3 Camera::GetWorldLocation()
{
   return m_Position;
}

//---------------------------------------------------------------------------------------------------
mat4 Camera::GetViewMatrix()
{
   return glm::lookAt(m_Position, m_Position + m_viewVector, m_upVector);
}
