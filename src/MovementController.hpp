#pragma once

#include "GameObject.hpp"
#include "Window.hpp"
#include <mutex>


enum KEY
{
	KEY_UNDEFINED,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_PgUP,
	KEY_PgDOWN
};

class InputHandler {
 public:

	 static InputHandler* getInput();

	 InputHandler(InputHandler& other) = delete;
	 void operator=(const InputHandler&) = delete;

	void processInput(float dt, GameObject& gameObject);
	
	bool wasQuitEvetn() const { return QuitEvent; }

private:

	InputHandler();
	~InputHandler();

	static InputHandler* Singleton;
	static std::mutex Mutex;
	
	float moveSpeed{15.f};
	float lookSpeed{5.f};
	bool QuitEvent{ false };

	static KEY getKey(SDL_Keycode SDLK);
	static KEY getKey(SDL_Scancode SDLS);

};
