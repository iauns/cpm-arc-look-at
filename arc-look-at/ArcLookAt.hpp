/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2013 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/// \author James Hughes
/// \date   April 2013

#ifndef IAUNS_ARC_LOOK_AT_H
#define IAUNS_ARC_LOOK_AT_H

#include <stdint.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration of ArcBall
namespace CPM_ARC_BALL_NS {
  class ArcBall;
}

namespace CPM_ARC_LOOK_AT_NS {

/// Basic camera class that uses cpm-arc-ball to control view orientation.
/// Supports a number of utility functions such as ensuring a given
/// AABB is in the view of the frustum (zooming only).
/// All coordinates, unless otherwise specified, are in 2D screen space
/// coordinates.
class ArcLookAt
{
public:
  ArcLookAt();
  virtual ~ArcLookAt();
  
  /// Sets the reference point for upcoming operations.
  /// Should always be called before any of the 'do' functions below.
  void doReferenceDown(const glm::vec2& ssPos);

  /// Pans the camera based on the reference position, current orientation, and
  /// the current screen / space position.
  /// Note that this pans the lookat point by the same amount as the camera.
  void doPan(const glm::vec2& ssPos);

  /// Rotates about the look at position.
  void doRotation(const glm::vec2& ssPos);

  /// Dollys the camera towards(negative) / away(positive) from the look at
  /// point.
  void doZoom(float camZoom);

  /// Retrieves the camera's distance away from the look at point.
  void getCamDistance();

  /// Retrieves the current lookat point.
  glm::vec2 getLookAt();

  /// Retrieves the world transformation for the camera (looking down
  /// negative z).
  glm::mat4 getWorldViewTransform() const;

private:
  std::unique_ptr<ArcBall>  mArcBall;

  glm::vec3                 mCamLookAt;     ///< Current lookat position.
  float                     mCamDistance;   ///< Distance from look-at.

  // The following are reference variables set when doReferenceDown is called.
  glm::vec2                 mReferenceScreenPos;
  glm::vec3                 mReferenceLookAt;
  glm::mat4                 mReferenceTransform;
  float                     mReferenceCamDistance;
};

} // namespace CPM_ARC_LOOK_AT_NS 

#endif
