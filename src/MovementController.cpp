#include "MovementController.hpp"

#include <SDL2/SDL.h>
#include <limits>
#include <iostream>


InputHandler* InputHandler::Singleton = nullptr;

std::mutex InputHandler::Mutex{};

InputHandler* InputHandler::getInput()
{
	std::lock_guard<std::mutex> Lock(Mutex);

	if (Singleton == nullptr)
		Singleton = new InputHandler();

	return Singleton;
}

void InputHandler::processInput(float dt, GameObject& gameObject)
{
	glm::vec3 rotate{0};

	float yaw = gameObject.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
	const glm::vec3 upDir{ 0.f, -1.f, 0.f };
	
	glm::vec3 moveDir{ 0.f };
	
	
	SDL_Event Event;


	while (SDL_PollEvent(&Event) != 0)
	{
		if (Event.type == SDL_QUIT)
			QuitEvent = true;

	}

	int ArraySize = 0;
	auto KeysState = SDL_GetKeyboardState(&ArraySize);


	for (int i = 0; i < ArraySize; i++)
	{
		KEY currentKey = getKey((SDL_Scancode)i);

		if (currentKey == KEY_UNDEFINED)
			continue;

		int keyInnput = KeysState[i];
		
		if (keyInnput == 0)
			continue;

		switch (currentKey)
		{
			case KEY_UP:
				rotate.x += 1.f;
				break;
			case KEY_DOWN:
				rotate.x -= 1.f;
				break;
			case KEY_LEFT:
				rotate.y -= 1.f;
				break;
			case KEY_RIGHT:
				rotate.y += 1.f;
				break;
			case KEY_W:
				moveDir += forwardDir;
				break;
			case KEY_S:
				moveDir -= forwardDir;
				break;
			case KEY_A:
				moveDir -= rightDir;
				break;
			case KEY_D:
				moveDir += rightDir;
				break;
			case KEY_PgUP:
				moveDir += upDir;
				break;
			case KEY_PgDOWN:
				moveDir -= upDir;
				break;
			default:
				break;
		}

	}

	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
	  gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
	}

	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
	  gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
	}
}

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

KEY InputHandler::getKey(SDL_Keycode SDLK)
{
	switch (SDLK)
	{
	case SDLK_w:
		return KEY_W;
	case SDLK_a:
		return KEY_A;
	case SDLK_s:
		return KEY_S;
	case SDLK_d:
		return KEY_D;
	case SDLK_UP:
		return KEY_UP;
	case SDLK_DOWN:
		return KEY_DOWN;
	case SDLK_LEFT:
		return KEY_LEFT;
	case SDLK_RIGHT:
		return KEY_RIGHT;
	case SDLK_PAGEUP:
		return KEY_PgUP;
	case SDLK_PAGEDOWN:
		return KEY_PgDOWN;
	default:
		return KEY_UNDEFINED;
	}
}

KEY InputHandler::getKey(SDL_Scancode SDLS)
{
	switch (SDLS)
	{
	case SDL_SCANCODE_W:
		return KEY_W;
	case SDL_SCANCODE_A:
		return KEY_A;
	case SDL_SCANCODE_S:
		return KEY_S;
	case SDL_SCANCODE_D:
		return KEY_D;
	case SDL_SCANCODE_UP:
		return KEY_UP;
	case SDL_SCANCODE_DOWN:
		return KEY_DOWN;
	case SDL_SCANCODE_LEFT:
		return KEY_LEFT;
	case SDL_SCANCODE_RIGHT:
		return KEY_RIGHT;
	case SDL_SCANCODE_PAGEUP:
		return KEY_PgUP;
	case SDL_SCANCODE_PAGEDOWN:
		return KEY_PgDOWN;
	default:
		return KEY_UNDEFINED;
	}
}
