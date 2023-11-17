#pragma once

#include "cAnimation.h"
#include <vector>

class cModelAnimation : public cAnimation
{
public:

	glm::vec3& positionRef;
	glm::vec3& orintationRef;
	glm::vec3& scaleRef;

	glm::vec3 initPosition;
	glm::vec3 initOrientation;
	glm::vec3 initScale;

	std::vector<sKeyFrameVec3> positionKeyframes;
	std::vector<sKeyFrameVec3> orientationKeyframes;
	std::vector<sKeyFrameVec3> scaleKeyframes;

	cModelAnimation(glm::vec3& _posRef, glm::vec3& _rotRef, glm::vec3& _sclRef);
	void AddPositionKeyFrame(sKeyFrameVec3 newKeyframe);
	void AddOrientationKeyFrame(sKeyFrameVec3 newKeyframe);
	void AddScaleKeyFrame(sKeyFrameVec3 newKeyframe);

	virtual void Process(float deltaTime);

	virtual void Reset();
	void Reset(glm::vec3 newInitPos, glm::vec3 newInitOri, glm::vec3 newInitScale);
};