#include "Input.h"

Input::Input():keyStates{}, mPos(0, 0), mRaw(0, 0), mVisible(true) {
}

void Input::update(char keyCode) {
	if(GetAsyncKeyState(keyCode) && !keyStates[keyCode].kp) {
		keyStates[keyCode].kp = true;
		keyStates[keyCode].kd = true;
	}
	if(!GetAsyncKeyState(keyCode) && keyStates[keyCode].kp) {
		keyStates[keyCode].kp = false;
		keyStates[keyCode].ku = true;
	}
}

void Input::update(int keyCode) {
	if(GetAsyncKeyState(keyCode) && !keyStates[keyCode].kp) {
		keyStates[keyCode].kp = true;
		keyStates[keyCode].kd = true;
	}
	if(!GetAsyncKeyState(keyCode) && keyStates[keyCode].kp) {
		keyStates[keyCode].kp = false;
		keyStates[keyCode].ku = true;
	}
}

void Input::Update() {
	POINT p;
	if(GetCursorPos(&p)) {
		mPos.x = p.x;
		mPos.y = p.y;
	}

	for(int i = 0; i < 256; i++)
		if(!keyStates[i].kp) {
			if(keyStates[i].ku)
				keyStates[i].ku = false;
		} else
			if(keyStates[i].kd)
				keyStates[i].kd = false;
}

Input& Input::Get() {
	static Input KE;
	return KE;
}

void Input::SetMouseVisible(bool state) {
	Get().smv(state);
}

void Input::SetMousePos(Vector2 pos)
{
	Get().smp(pos);
}

void Input::SetMousePos(int x, int y) {
	Get().smp(x, y);
}

const bool& Input::GetMouseVisible() {
	return Get().gmv();
}

const Vector2& Input::MousePos() {
	return Get().mp();
}

const Vector2& Input::MouseRaw() {
	return Get().mr();
}

const bool& Input::KeyDown(char keyCode) {
	return Get().kD(keyCode);
}

const bool& Input::KeyDown(int keyCode) {
	return Get().kD(keyCode);
}

const bool& Input::KeyPress(char keyCode) {
	return Get().kP(keyCode);
}

const bool& Input::KeyPress(int keyCode) {
	return Get().kP(keyCode);
}

const bool& Input::KeyUp(char keyCode) {
	return Get().kU(keyCode);
}

const bool& Input::KeyUp(int keyCode) {
	return Get().kU(keyCode);
}

void Input::smv(bool state) {
	mVisible = state;
	ShowCursor(mVisible);
}

void Input::smp(Vector2 pos) {
	SetCursorPos(pos.x, pos.y);
}

void Input::smp(int x, int y) {
	SetCursorPos(x, y);
}

const bool& Input::gmv() {
	return mVisible;
}

const Vector2& Input::mp() {
	return mPos;
}

const Vector2& Input::mr() {
	return mRaw;
}

const bool& Input::kD(char keyCode) {
	update(keyCode);
	return keyStates[keyCode].kd;
}

const bool& Input::kD(int keyCode) {
	update(keyCode);
	return keyStates[keyCode].kd;
}

const bool& Input::kP(char keyCode) {
	update(keyCode);
	return keyStates[keyCode].kp;
}

const bool& Input::kP(int keyCode) {
	update(keyCode);
	return keyStates[keyCode].kp;
}
	
const bool& Input::kU(char keyCode) {
	update(keyCode);
	return keyStates[keyCode].ku;
}

const bool& Input::kU(int keyCode) {
	update(keyCode);
	return keyStates[keyCode].ku;
}
