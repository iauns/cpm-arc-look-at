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

#include "ArcLookAt.hpp"
#include <arc-ball/ArcBall.hpp>

namespace ArcBall = CPM_ARC_BALL_NS;

namespace CPM_ARC_LOOK_AT_NS {

//------------------------------------------------------------------------------
ArcLookAt::ArcLookAt() :
    mArcBall(new ArcBall::ArcBall(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)),
    mCamDistance(0.0f),
    mReferenceCamDistance(0.0f)
{
}

//------------------------------------------------------------------------------
ArcLookAt::~ArcLookAt()
{
}

//------------------------------------------------------------------------------
void ArcLookAt::doReferenceDown(const glm::vec2& ssPos)
{
  mReferenceLookAt      = mCamLookAt;
  mReferenceScreenPos   = ssPos;
  mReferenceTransform   = getWorldViewTransform();
  mReferenceCamDistance = mCamDistance;
}

//------------------------------------------------------------------------------
void ArcLookAt::doPan(const glm::vec2& ssPos)
{
  glm::vec2 delta = mReferenceScreenPos - ssPos;
  glm::vec2 trans = delta * mCamDistance / 2.0f;

  glm::mat4 camRot = mArcBall->getTransformation();
  glm::vec3 translation = 
        static_cast<glm::vec3>(camRot[0].xyz()) * trans.x
      + static_cast<glm::vec3>(camRot[1].xyz()) * trans.y;
  mCamAccumPosNow = mReferenceLookAt + translation;
}

//------------------------------------------------------------------------------
void ArcLookAt::doRotation(const glm::vec2& ssPos)
{
  mArcBall->drag(ssPos);
}

//------------------------------------------------------------------------------
void ArcLookAt::doZoom(glm::float_t camZoom)
{
  mCamDistance += camZoom;
}

//------------------------------------------------------------------------------
void ArcLookAt::doZoom(const glm::vec2& ssPos)
{
  // Use distance delta from center of screen to control zoom.
  // Will need a new variable to control this.
  spire::V2 delta = ssPos - mReferenceScreenPos;
  glm::float_t xScale = 4.0f;
  glm::float_t yScale = 4.0f;
  mCamDistance = mReferenceCamDistance + (delta.x) * xScale + (-delta.y) * yScale;
}

//------------------------------------------------------------------------------
glm::mat4 ArcLookAt::getWorldViewTransform() const
{
  glm::mat4 camRot      = mArcBall->getTransformation();
  glm::mat4 finalTrafo  = camRot;

  // Translation is a post rotation operation where as zoom is a pre transform
  // operation. We should probably ensure the user doesn't scroll passed zero.
  // Remember, we are looking down NEGATIVE z.
  // NOTE: We are translating both the lookat and the eye point.
  // Eyepoint is a function of the lookat, the camera transform, and the
  // camera distance.
  finalTrafo[3].xyz() = mCamLookAt + static_cast<glm::vec3>(camRot[2].xyz()) * mCamDistance;
  return finalTrafo;
}

//------------------------------------------------------------------------------
void ArcLookAt::autoview(const CPM_GLM_AABB_NS::AABB& aabb)
{
  
}

} // namespace CPM_ARC_LOOK_AT_NS

