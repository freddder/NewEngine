#include "cCharacter.h"
#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cMapManager.h"

cCharacter::cCharacter(glm::vec3 pos, std::string textureName)
{
	position = pos;

	model = cRenderManager::CreateSpriteModel();
	model->meshName = "SpriteHolder.obj";
	model->position = pos;
	model->textureName = textureName;

	spriteAnimation = std::make_shared<cSpriteAnimation>(model->currSpriteId, model->scale);
	modelAnimation = std::make_shared<cModelAnimation>(model->position, model->orientation, model->scale);

	cAnimationManager::AddAnimation(spriteAnimation);
	cAnimationManager::AddAnimation(modelAnimation);
}

cCharacter::~cCharacter()
{
	cRenderManager::RemoveModel(model);

	cAnimationManager::RemoveAnimation(spriteAnimation);
	cAnimationManager::RemoveAnimation(modelAnimation);
}

eEntityMoveResult cCharacter::ProcessMovement(eDirection dir, bool run)
{
	modelAnimation->Reset(model->position, model->orientation, model->scale);

	// Make model animation
	eEntityMoveResult moveResult = cMapManager::GetInstance()->TryMoveEntity(this, dir);
	glm::vec3 newPosition = model->position;

	if (moveResult != eEntityMoveResult::FAILURE)
	{
		if (dir == UP) newPosition.x += 1.f;
		else if (dir == DOWN) newPosition.x -= 1.f;
		else if (dir == LEFT) newPosition.z -= 1.f;
		else if (dir == RIGHT) newPosition.z += 1.f;
	}

	// Ajust height
	if (moveResult == eEntityMoveResult::SUCCESS_UP) newPosition.y += 1.f;
	else if (moveResult == eEntityMoveResult::SUCCESS_DOWN) newPosition.y -= 1.f;

	if (!run || moveResult == 0) modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));
	else modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.14f, newPosition));

	position = newPosition;

	return moveResult;
}

void cCharacter::MoveFollower(glm::vec3 newPosition, bool run)
{
	eDirection dir = UP;
	if ((newPosition - follower->model->position).z == 1.f) dir = RIGHT;
	else if ((newPosition - follower->model->position).z == -1.f) dir = LEFT;
	else if ((newPosition - follower->model->position).x == 1.f) dir = UP;
	else if ((newPosition - follower->model->position).x == -1.f) dir = DOWN;
	else return;

	follower->modelAnimation->isDone = true;

	follower->Move(dir, run);
}

void cCharacter::SetFollower(cCharacter* newFollower)
{
	if (follower) return;

	follower = newFollower;
}

void cCharacter::UnsetFollower()
{
	follower = nullptr;
}
