#pragma once

#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <vector>

enum eSpriteEntityType
{
	PLAYER,
	NPC,
	OW_POKEMON
};

enum eDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct sKeyFrameVec3 // rgb
{
	sKeyFrameVec3(): 
		time(0.f), 
		value(glm::vec3(0)) {}
	sKeyFrameVec3(float _time, glm::vec3 _value) :
		time(_time),
		value(_value) {}

	glm::vec3 value;
	float time;
};

struct sKeyFrameVec4 // rgba
{
	sKeyFrameVec4() :
		time(0.f),
		value(glm::vec4(0)) {}
	sKeyFrameVec4(float _time, glm::vec4 _value) :
		time(_time),
		value(_value) {}

	glm::vec4 value;
	float time;
};

struct sKeyFrame3Vec3 // position, rotation, scale
{
	glm::vec3 value1;
	glm::vec3 value2;
	glm::vec3 value3;
	float time;
};

struct sKeyFrameString // texture
{
	std::string value;
	float time;
};

struct sKeyFrameFloat
{
	sKeyFrameFloat():
		time(0.f),
		value(0.f) {}
	sKeyFrameFloat(float _time, float _value) :
		time(_time),
		value(_value) {}

	float value;
	float time;
};

struct sKeyFrameInt
{
	sKeyFrameInt(float _time, int _value) : 
		time(_time), 
		value(_value) {}

	unsigned int value;
	float time;
};

struct sKeyFrameSprite // sprite
{
	sKeyFrameSprite(float _time, int _value, bool _flip = false) :
		time(_time),
		value(_value),
		flip(_flip) {}

	unsigned int value;
	bool flip;
	float time;
};

class cAnimation
{
public:

	float timer;
	bool isDone;
	float maxDuration;
	float speed;
	bool isRepeat;
	bool removeAfterComplete;
	std::function<void()> callback;

	cAnimation();
	~cAnimation();
	// maybe try removing this from the animation manager when delteting

	virtual void Process(float deltaTime) = 0;

	virtual void Reset();
};

class cFloatAnimation : public cAnimation
{
public:
	float& valueRef;
	float initValue;

	std::vector<sKeyFrameFloat> keyframes;

	cFloatAnimation(float& _valueRef);
	void AddKeyFrame(sKeyFrameFloat newKeyframe);

	virtual void Process(float deltaTime);
};

class cVec3Animation : public cAnimation
{
public:
	glm::vec3& valueRef;
	glm::vec3 initValue;

	std::vector<sKeyFrameVec3> keyframes;

	cVec3Animation(glm::vec3& _valueRef);
	void AddKeyFrame(sKeyFrameVec3 newKeyframe);

	virtual void Process(float deltaTime);
};

class cVec4Animation : public cAnimation
{
public:
	glm::vec4& valueRef;
	glm::vec4 initValue;

	std::vector<sKeyFrameVec4> keyframes;

	cVec4Animation(glm::vec4& _valueRef);
	void AddKeyFrame(sKeyFrameVec4 newKeyframe);

	virtual void Process(float deltaTime);
};

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

class cSinAnimation : public cAnimation
{
public:
	glm::vec3& valueRef;
	glm::vec3 initValue;

	float valueRange;
	float valueOffset;

	std::vector<sKeyFrameVec3> keyframes;

	cSinAnimation(glm::vec3& _value, float _valueRange, float _valueOffset);
	void AddKeyFrame(sKeyFrameVec3 newKeyframe);

	virtual void Reset();
	void Reset(glm::vec3 newValue);

	virtual void Process(float deltaTime);
};

class cSpriteAnimation : public cAnimation
{
public:
	int& spriteIdRef;
	int initId;
	glm::vec3& modelScaleRef;
	glm::vec3 initScale;

	std::vector<sKeyFrameSprite> keyframes;

	cSpriteAnimation(int& _spriteIdRef, glm::vec3& _modelScale);
	void AddKeyFrame(sKeyFrameSprite newKeyframe);
	void AddKeyFrames(std::vector<sKeyFrameSprite>& newKeyframes);

	virtual void Reset();
	void Reset(int newInitId, glm::vec3 newInitScale);

	virtual void Process(float deltaTime);
};

class cPeriodicSpriteAnimation : public cAnimation
{
public:
	int& spriteIdRef;
	float interval = 0.1f;
	int maxId; // If this number is hit, reset id red to 0

	cPeriodicSpriteAnimation(int& _spriteIdRef, int _maxId);

	virtual void Process(float deltaTime);
};