#include "cUIManager.h"
#include <glm/ext/matrix_clip_space.hpp>

cUIManager* cUIManager::singleton = NULL;

cUIManager::cUIManager()
{

}

cUIManager::~cUIManager()
{

}

void cUIManager::DrawUI()
{
	//glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 1.f, 100.f);
}


