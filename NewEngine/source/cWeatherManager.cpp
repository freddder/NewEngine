#include "cWeatherManager.h"
#include "cFloatAnimation.h"
#include "cParticleManager.h"
#include "cRenderManager.h"

cWeatherManager* cWeatherManager::singleton = NULL;

cWeatherManager::cWeatherManager()
{
	currWeather = NONE;

	//offsetDegree = 0.f;
}

cWeatherManager::~cWeatherManager()
{
	//for (unsigned int i = 0; i < particleTypes.size(); i++)
	//{
	//	glDeleteBuffers(1, &particleTypes[i].positionsBufferId);
	//}
}

void cWeatherManager::SetWeather(eWeather newWeather)
{
	//if (newWeather == currWeather)
	//	return;

	if ((currWeather == SNOW || currWeather == HAIL || currWeather == SNOWSTORM) && // snow transition
		(newWeather == SNOW || newWeather == HAIL || newWeather == SNOWSTORM))
	{
		// animate values to correct number
		

		if (newWeather == SNOW)
		{

		}
		else if (newWeather == HAIL)
		{

		}
	}
	else if ((currWeather == RAIN || currWeather == HEAVYRAIN) && // rain transition
		(newWeather == RAIN || newWeather == HEAVYRAIN))
	{
		// animate values to correct number


	}
	else // no transition
	{
		if (newWeather == NONE)
		{
			fogDensity = 0.f;
			fogGradient = 0.1f;
		}
		else if (newWeather == SNOW)
		{
			fogDensity = 0.037f;
			fogGradient = 1.59f;
			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);

			std::shared_ptr<cRenderModel> prtcl = cRenderManager::CreateRenderModel();
			prtcl->meshName = "SpriteHolder.obj";
			prtcl->shaderName = "particle";
			prtcl->textureName = "SnowFlake3.png";
			prtcl->scale = glm::vec3(0.3f);

			cParticleSpawner weatherSpawner(glm::vec3(0.f, 20.f, 0.f), prtcl, 1000);
			weatherSpawner.minPositionOffset = glm::vec3(-20.f, 0.f, -20.f);
			weatherSpawner.maxPositionOffset = glm::vec3(20.f, 0.f, 20.f);
			weatherSpawner.spawnSpeed = glm::vec3(0.f, -3.f, 0.f);
			weatherSpawner.spawnRate = 0.05f;
			weatherSpawner.particleLifeTime = 10.f;
			cParticleManager::GetInstance()->AddSpawner(weatherSpawner);
		}
		else if (newWeather == HAIL)
		{
			fogDensity = 0.022f;
			fogGradient = 0.48f;
			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);

			
		}
		else if (newWeather == SNOWSTORM)
		{

		}
		else if (newWeather == RAIN)
		{

		}
		else if (newWeather == HEAVYRAIN)
		{

		}
		else if (newWeather == SANDSTORM)
		{

		}
		else if (newWeather == LEAVES)
		{

		}
	}

	currWeather = newWeather;
}

//void cWeatherManager::SetWeather(eWeather newWeather)
//{
//	//if (newWeather == currWeather)
//	//	return;
//
//	if ((currWeather == SNOW || currWeather == HAIL || currWeather == SNOWSTORM) && // snow transition
//		(newWeather == SNOW || newWeather == HAIL || newWeather == SNOWSTORM))
//	{
//		// animate values to correct number
//		cFloatAnimation* particleSpeedXAnim = new cFloatAnimation(particleSpeed.x);
//		particleSpeedXAnim->clearAfterComplete = true;
//
//		cFloatAnimation* particleSpeedYAnim = new cFloatAnimation(particleSpeed.y);
//		particleSpeedYAnim->clearAfterComplete = true;
//
//		cFloatAnimation* fogDensityAnim = new cFloatAnimation(fogDensity);
//		fogDensityAnim->clearAfterComplete = true;
//
//		cFloatAnimation* fogGradientAnim = new cFloatAnimation(fogGradient);
//		fogGradientAnim->clearAfterComplete = true;
//
//		if (newWeather == SNOW)
//		{
//			particleSpeedXAnim->AddKeyFrame(sKeyFrameFloat(3.f, 0.f));
//			particleSpeedYAnim->AddKeyFrame(sKeyFrameFloat(3.f, -0.15f));
//			fogDensityAnim->AddKeyFrame(sKeyFrameFloat(3.f, 0.037f));
//			fogGradientAnim->AddKeyFrame(sKeyFrameFloat(3.f, 1.59f));
//		}
//		else if (newWeather == HAIL)
//		{
//			particleSpeedXAnim->AddKeyFrame(sKeyFrameFloat(3.f, -0.75));
//			particleSpeedYAnim->AddKeyFrame(sKeyFrameFloat(3.f, -1.65f));
//			fogDensityAnim->AddKeyFrame(sKeyFrameFloat(3.f, 0.022f));
//			fogGradientAnim->AddKeyFrame(sKeyFrameFloat(3.f, 0.48f));
//		}
//
//		g_AnimationManager->AddAnimation(particleSpeedXAnim);
//		g_AnimationManager->AddAnimation(particleSpeedYAnim);
//		g_AnimationManager->AddAnimation(fogDensityAnim);
//		g_AnimationManager->AddAnimation(fogGradientAnim);
//	}
//	else if ((currWeather == RAIN || currWeather == HEAVYRAIN) && // rain transition
//		(newWeather == RAIN || newWeather == HEAVYRAIN))
//	{
//		// animate values to correct number
//
//
//	}
//	else // no transition
//	{
//		if (newWeather == NONE)
//		{
//			fogDensity = 0.f;
//			fogGradient = 0.1f;
//		}
//		else if (newWeather == SNOW)
//		{
//			fogDensity = 0.037f;
//			fogGradient = 1.59f;
//			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);
//
//			particleSpeed.x = 0.f;
//			particleSpeed.y = -0.15f; // 0.5 - -0.5
//
//			CreateWeatherParticleType("SnowFlake1.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 4);
//			CreateWeatherParticleType("SnowFlake2.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 6);
//			CreateWeatherParticleType("SnowFlake3.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 8);
//		}
//		else if (newWeather == HAIL)
//		{
//			fogDensity = 0.022f;
//			fogGradient = 0.48f;
//			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);
//
//			particleSpeed.x = -0.75f;
//			particleSpeed.y = -1.65f;

//			CreateWeatherParticleType("SnowFlake1.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 4);
//			CreateWeatherParticleType("SnowFlake2.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 6);
//			CreateWeatherParticleType("SnowFlake3.png", "SpriteHolder2.obj", glm::vec2(0.f, -0.05f), glm::vec2(0.f, 0.05f), 8);
//		}
//		else if (newWeather == SNOWSTORM)
//		{
//
//		}
//		else if (newWeather == RAIN)
//		{
//
//		}
//		else if (newWeather == HEAVYRAIN)
//		{
//
//		}
//		else if (newWeather == SANDSTORM)
//		{
//
//		}
//		else if (newWeather == LEAVES)
//		{
//
//		}
//	}
//
//	currWeather = newWeather;
//}

//void cWeatherManager::Process(float deltaTime)
//{
//	for (unsigned int i = 0; i < particleTypes.size(); i++)
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, particleTypes[i].positionsBufferId);
//
//		for (unsigned j = 0; j < particleTypes[i].positions.size(); j++)
//		{
//			sWeatherParticlePosition& currParticlePosition = particleTypes[i].positions[j];
//
//			// process speed
//			currParticlePosition.position += (particleSpeed + currParticlePosition.speedOffset) * deltaTime;
//
//			// process camera position
//			glm::vec2 positionWithCam = currParticlePosition.position;
//
//			positionWithCam.x -= std::fmodf(g_Camera->position.z / 6.f, 2.4f);
//			positionWithCam.y -= std::fmodf(g_Camera->position.x / 20.f, 2.4f);
//			positionWithCam.y -= std::fmodf(g_Camera->position.y / 10.f, 2.4f);
//
//			if (positionWithCam.y <= -1.2f)
//				positionWithCam.y += 2.4f;
//			else if (positionWithCam.y >= 1.2f)
//				positionWithCam.y -= 2.4f;
//
//			if (positionWithCam.x <= -1.2f)
//				positionWithCam.x += 2.4f;
//			else if (positionWithCam.x >= 1.2f)
//				positionWithCam.x -= 2.4f;
//
//			if (currParticlePosition.position.y <= -1.2f)
//				currParticlePosition.position.y += 2.4f;
//			else if (currParticlePosition.position.y >= 1.2f)
//				currParticlePosition.position.y -= 2.4f;
//
//			if (currParticlePosition.position.x <= -1.2f)
//				currParticlePosition.position.x += 2.4f;
//			else if (currParticlePosition.position.x >= 1.2f)
//				currParticlePosition.position.x -= 2.4f;
//
//			glBufferSubData(GL_ARRAY_BUFFER, j * sizeof(glm::vec2), sizeof(glm::vec2), glm::value_ptr(positionWithCam));
//		}
//
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//	}
//
//	// move UVs here i guess
//}

//void cWeatherManager::CreateWeatherParticleType(std::string _textureName, std::string _modelName,
//												glm::vec2 minSpeedOffset, glm::vec2 maxSpeedOffset,
//												int amount)
//{
//	sWeatherParticleType newType;
//
//	newType.modelName = _modelName;
//	newType.textureName = _textureName;
//
//	glGenBuffers(1, &newType.positionsBufferId);
//	glBindBuffer(GL_ARRAY_BUFFER, newType.positionsBufferId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * amount, NULL, GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	srand((int)time(0));
//	unsigned int init_seed = (rand() % 10) + 1;
//	cLinearCongruentialGenerator lcg(init_seed);
//
//	std::vector<double> random_draws;
//	lcg.get_uniform_draws2(random_draws, amount);
//
//	for (unsigned int i = 0; i < random_draws.size(); i++)
//	{
//		sWeatherParticlePosition newPosition;
//		newPosition.position.x = (float)random_draws[i]; // -1 | 1
//		newPosition.position.y = (float)(rand() % 200 - 100) / (float)100; // -1 | 1
//		newPosition.speedOffset.x = minSpeedOffset.x + (float)(rand()) * (float)(maxSpeedOffset.x - minSpeedOffset.x) / RAND_MAX;
//		newPosition.speedOffset.y = minSpeedOffset.y + (float)(rand()) * (float)(maxSpeedOffset.y - minSpeedOffset.y) / RAND_MAX; // 0.05 | -0.05
//
//		newType.positions.push_back(newPosition);
//	}
//
//	particleTypes.push_back(newType);
//}
