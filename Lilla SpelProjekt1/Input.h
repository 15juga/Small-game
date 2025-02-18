#pragma once
#include<Windows.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct KeyState {
	bool kd;
	bool kp;
	bool ku;
	KeyState() 
		:kd(false), kp(false), ku(false){
	}
};

class Input {
private:
	KeyState keyStates[256];
	Vector2 mPos;
	Vector2 mRaw;
	bool mVisible;

	void smv(bool state);
	void smp(Vector2 pos);
	void smp(int x, int y);

	const bool& gmv();

	const Vector2& mp();
	const Vector2& mr();

	const bool& kD(char keyCode);
	const bool& kD(int keyCode);
	const bool& kP(char keyCode);
	const bool& kP(int keyCode);
	const bool& kU(char keyCode);
	const bool& kU(int keyCode);
		
	void update(char keyCode);
	void update(int keyCode);

public:
	Input();
	Input(const Input&) = delete;

	void Update();

	static Input& Get();

	static void SetMouseVisible(bool state);
	static void SetMousePos(Vector2 pos);
	static void SetMousePos(int x, int y);

	static const bool& GetMouseVisible();

	static const Vector2& MousePos();
	static const Vector2& MouseRaw();

	static const bool& KeyDown(char keyCode);
	static const bool& KeyDown(int keyCode);
	static const bool& KeyPress(char keyCode);
	static const bool& KeyPress(int keyCode);
	static const bool& KeyUp(char keyCode);
	static const bool& KeyUp(int keyCode);

	friend class WindowContainer;
};

