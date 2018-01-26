#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace Utility
{

   class Camera
   {
   public:
      Camera();
      Camera(vec3& pos);
      ~Camera();

      bool SetSpeed(float speed);
      void Set(vec3& absPos, vec3& viewVector);
      void ResetLast();

      void MoveForward();
      void MoveBackward();
      void MoveUp();
      void MoveDown();
      void MoveLeft();
      void MoveRight();

      void RotateLeft();
      void RotateRight();
      void RotateUp();
      void RotateDown();

      void Reverse();

      void HandleMouse(vec2 mousePos);

      vec3 GetWorldLocation();
      mat4 GetViewMatrix();

   private:
      float m_Speed;
      vec3 m_Position, m_oldPosition;
      vec3 m_viewVector, m_oldViewVector;
      vec3 m_upVector;
      vec2 m_MousePos;

      mat4 m_ViewMatrix;
   };
}

#endif
