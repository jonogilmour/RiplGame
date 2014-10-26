#include "pch.h"
#include "Content\CommonFunctions.h"
#include "Collision.h"
#include "SceneRenderer.h"
#include "WallCollision.h"

void MoveLookController::Initialize(_In_ CoreWindow^ window)
{
	
	// opt in to recieve touch/mouse events


	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &MoveLookController::OnPointerReleased);

	/*window->CharacterReceived +=
	ref new TypedEventHandler<CoreWindow^, CharacterReceivedEventArgs^>(this, &MoveLookController::OnCharacterReceived);*/

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyUp);

	Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved +=
		ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &MoveLookController::OnMouseMoved);

	//	need to init this as it is reset every frame
	m_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);
	deltaTime = 0;

	SetOrientation(-(XM_PI / 4.0f), 0);				// look down slightly ahead when the app starts
	SetPosition(XMFLOAT3(0, 10.0f, 0));

	tapped = false;
	acc = ref new Windows::Devices::Sensors::Accelerometer();
	auto Currread =acc ->GetCurrentReading;
}

void MoveLookController::OnPointerPressed(
	_In_ CoreWindow^ sender,
	_In_ PointerEventArgs^ args)
{
	OutputDebugString(L"TAP ON\n");

	// get the current pointer position
	uint32 pointerID = args->CurrentPoint->PointerId;

	// STANLEY
	tapped = true;
	tapPosition = XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);
}

void MoveLookController::OnPointerMoved(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{

	XMFLOAT2 position = XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);

	XMFLOAT2 pointerDelta;
	pointerDelta = XMFLOAT2(position.x - m_lookLastPoint.x, position.y - m_lookLastPoint.y);		// how far did pointer move

	XMFLOAT2 rotationDelta;
	rotationDelta = XMFLOAT2(pointerDelta.x * deltaTime * ROTATION_GAIN, pointerDelta.y * deltaTime * ROTATION_GAIN);	// scale for control sensitivity
	m_lookLastPoint = position;			 			// save for next time through

	// update our orientation based on the command
	m_pitch -= rotationDelta.y;						// mouse y increases down, but pitch increases up
	m_yaw += rotationDelta.x;						// yaw defined as CCW around y-axis

	// Limit pitch to straight up or straight down
	m_pitch = (float)__max(-XM_PI / 2.0f, m_pitch);
	m_pitch = (float)__min(+XM_PI / 2.0f, m_pitch);
}

void MoveLookController::OnPointerReleased(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{
	uint32 pointerID = args->CurrentPoint->PointerId;
	XMFLOAT2 position = XMFLOAT2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);
}

void MoveLookController::OnKeyDown(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// CAMERA
	if (Key == VirtualKey::W)		// forward
		m_forward = true;
	if (Key == VirtualKey::S)		// back
		m_back = true;
	if (Key == VirtualKey::A)		// left
		m_left = true;
	if (Key == VirtualKey::D)		// right
		m_right = true;
	if (Key == VirtualKey::Space)		// Up
		m_up = true;
	if (Key == VirtualKey::Control)		// down
		m_down = true;

	// test movement to dest_position
	if (Key == VirtualKey::M){
		dest_position = XMFLOAT3(0, 5.0f, 0);	// temporary destination location for testing
		dest_lookat = XMFLOAT3(3.0f, 3.0f, 3.0f);				// temporary look at location for testing

		XMFLOAT2 target_orientation = computeTargetOrientation();
		float target_pitch = target_orientation.x;
		float target_yaw = target_orientation.y;

		m_point = true;
	}

	// OBJECT
	if (Key == VirtualKey::PageDown)		// down
		obj_down = true;
	if (Key == VirtualKey::PageUp)		// up
		obj_up = true;
	if (Key == VirtualKey::Up)		// forward
		obj_fwd = true;
	if (Key == VirtualKey::Down){		// back
		obj_back = true;
	}
	if (Key == VirtualKey::Left)		// left
		obj_left = true;
	if (Key == VirtualKey::Right)		// right
		obj_rght = true;

	// MAKE NEW BOX
	if (Key == VirtualKey::B) {
		if (current_game_info.current_life < current_game_info.max_lives) {
			current_game_info.current_life += 1;
		}
	}
}

void MoveLookController::OnKeyUp(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	// CAMERA
	if (Key == VirtualKey::W)		// forward
		m_forward = false;
	if (Key == VirtualKey::S)		// back
		m_back = false;
	if (Key == VirtualKey::A)		// left
		m_left = false;
	if (Key == VirtualKey::D)		// right
		m_right = false;
	if (Key == VirtualKey::Space)		// Up
		m_up = false;
	if (Key == VirtualKey::Control)		// down
		m_down = false;

	// OBJECT
	if (Key == VirtualKey::PageDown)		// down
		obj_down = false;
	if (Key == VirtualKey::PageUp)		// up
		obj_up = false;
	if (Key == VirtualKey::Up)		// forward
		obj_fwd = false;
	if (Key == VirtualKey::Down)		// back
		obj_back = false;
	if (Key == VirtualKey::Left)		// left
		obj_left = false;
	if (Key == VirtualKey::Right)		// right
		obj_rght = false;
}

void MoveLookController::OnMouseMoved(
	_In_ MouseDevice^,
	_In_ MouseEventArgs^ args
	)
{
	// Handle Mouse Input via dedicated relative movement handler.

	XMFLOAT2 mouseDelta;
	mouseDelta.x = static_cast<float>(args->MouseDelta.X);
	mouseDelta.y = static_cast<float>(args->MouseDelta.Y);

	XMFLOAT2 rotationDelta;
	rotationDelta.x = mouseDelta.x * deltaTime * ROTATION_GAIN;   // scale for control sensitivity
	rotationDelta.y = mouseDelta.y * deltaTime * ROTATION_GAIN;

	// Update our orientation based on the command.
	m_pitch -= rotationDelta.y;
	m_yaw += rotationDelta.x;

	// Limit pitch to straight up or straight down.
	float limit = XM_PI / 2.0f - 0.01f;
	m_pitch = __max(-limit, m_pitch);
	m_pitch = __min(+limit, m_pitch);

	// Keep longitude in same range by wrapping.
	if (m_yaw >  XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}
}

void MoveLookController::SetPosition(_In_ XMFLOAT3 pos)
{
	m_position = pos;
}

// accessor to set position of controller
void MoveLookController::SetOrientation(_In_ float pitch, _In_ float yaw)
{
	m_pitch = pitch;
	m_yaw = yaw;
}

// returns the position of the controller object
XMFLOAT3 MoveLookController::get_Position()
{
	return m_position;
}

// returns the point at which the camera controller is facing
XMFLOAT3 MoveLookController::get_LookAt() {
	return m_lookat;
}

XMFLOAT3 MoveLookController::get_UpAxis() {
	return m_upaxis;
}

XMFLOAT3 MoveLookController::computeRAxis() {
	return XMFLOAT3(
		(float)(sinf(m_yaw + (XM_PI / 2.0f))),
		0.0f,
		(float)(cosf(m_yaw + (XM_PI / 2.0f)))
		);
}

XMFLOAT3 MoveLookController::computeDirection() {
	return XMFLOAT3(
		(float)(cosf(m_pitch) * sinf(m_yaw)),
		(float)(sinf(m_pitch)),
		(float)(cosf(m_pitch) * cosf(m_yaw))
		);
}

XMFLOAT3 computeUpAxis(XMFLOAT3 r, XMFLOAT3 l) {
	XMVECTOR dir = XMLoadFloat3(&l);
	XMVECTOR right = XMLoadFloat3(&r);
	XMVECTOR cross = XMVectorNegate(XMVector3Cross(right, dir));
	XMFLOAT3 returnVal;
	XMStoreFloat3(&returnVal, cross);
	return returnVal;
}

XMFLOAT3 MoveLookController::camPosition(){

	auto camX = m_position.x;
	auto camY = m_position.y;
	auto camZ = m_position.z;

	return (XMFLOAT3(camX, camY, camZ));
}

void MoveLookController::moveTo(XMFLOAT3 center){

	m_position.x = center.x;
	m_position.y = center.y;
	m_position.z = center.z;
}

//**
XMFLOAT2 MoveLookController::computeTargetOrientation() {

	//dest_position = XMFLOAT3(0, 5.0f, 0);
	//dest_lookat = XMFLOAT3(0, 3, 0);

	XMFLOAT3 v1 = XMFLOAT3((dest_lookat.x - m_position.x), (dest_lookat.y - m_position.y), (dest_lookat.z - m_position.z));
	float target_pitch = asin(fabs(v1.y / sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2))));
	float target_yaw = asin(fabs(v1.z / sqrt(pow(v1.x, 2) + pow(v1.y, 2) + pow(v1.z, 2))));

	m_pitch = (float)__max(-XM_PI / 2.0f, m_pitch);
	m_pitch = (float)__min(+XM_PI / 2.0f, m_pitch);

	if (m_yaw >  XM_PI)
	{
		m_yaw -= XM_PI * 2.0f;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_PI * 2.0f;
	}

	return XMFLOAT2(
		(float)(target_pitch),
		(float)(target_yaw)
		);
}

bool MoveLookController::equal(XMFLOAT3 curr, XMFLOAT3 dest){
	float range = 0.05f;

	if (curr.x <= dest.x + range && curr.x >= dest.x - range
		&& curr.y <= dest.y + range && curr.y >= dest.y - range
		&& curr.z <= dest.z + range && curr.z >= dest.z - range)
		return true;
	return false;
}

XMFLOAT3 MoveLookController::computeDirectionVector(){
	return XMFLOAT3(
		(float)(dest_position.x - m_position.x),
		(float)(dest_position.y - m_position.y),
		(float)(dest_position.z - m_position.z)
		);
}

void MoveLookController::Update(CoreWindow ^window, float timeDelta, XMFLOAT4X4* moveObjectTransform, Size outputSize, XMFLOAT4X4 view, XMFLOAT4X4 proj, std::list<XMFLOAT3>* wallList)
{
	deltaTime = timeDelta;
	XMFLOAT3 dir = computeDirection();
	XMFLOAT3 r_axis = computeRAxis();
	XMFLOAT3 up_axis = computeUpAxis(r_axis, dir);

	XMFLOAT3 lookat_vector = XMFLOAT3(0, 0, 0);

	// poll our state bits set by the keyboard input events
	if (m_forward) {
		m_moveCommand.x += (dir.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (dir.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (dir.z * timeDelta * MOVEMENT_GAIN);
	}
	if (m_back) {
		m_moveCommand.x -= (dir.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (dir.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (dir.z * timeDelta * MOVEMENT_GAIN);
	}
	if (m_left){
		m_moveCommand.x -= (r_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (r_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (r_axis.z * timeDelta * MOVEMENT_GAIN);
	}
	if (m_right) {
		m_moveCommand.x += (r_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (r_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (r_axis.z * timeDelta * MOVEMENT_GAIN);
	}
	if (m_up) {
		m_moveCommand.x += (up_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (up_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (up_axis.z * timeDelta * MOVEMENT_GAIN);
	}
	if (m_down) {
		m_moveCommand.x -= (up_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (up_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (up_axis.z * timeDelta * MOVEMENT_GAIN);
	}

	// Move camera to a specific location
	if (m_point) {				// if keypressed and current position is not destination position
		XMFLOAT3 dir_vector = computeDirectionVector();
		XMFLOAT3 dir = computeDirection();

		bool dir_completed = false;
		bool pitch_completed = false;
		bool yaw_completed = false;

		float speed = 2;

		if (!equal(m_position, dest_position)) {
			m_moveCommand.x += (dir_vector.x * timeDelta * speed);
			m_moveCommand.y += (dir_vector.y * timeDelta * speed);
			m_moveCommand.z += (dir_vector.z * timeDelta * speed);
		}
		else {
			dir_completed = true;
		}

		if (m_pitch >= target_pitch + 0.02){
			m_pitch -= ((m_pitch - target_pitch)* timeDelta * speed);
		}
		else if (m_pitch <= target_pitch - 0.02){
			m_pitch += ((target_pitch - m_pitch) * timeDelta * speed);
		}
		else {
			pitch_completed = true;
		}

		if (m_yaw >= target_yaw + 0.02){
			m_yaw -= ((m_yaw - target_yaw)* timeDelta * speed);
		}
		else if (m_yaw <= target_yaw - 0.02){
			m_yaw += ((target_yaw - m_yaw) * timeDelta * speed);
		}
		else {
			yaw_completed = true;
		}

		if (dir_completed && pitch_completed && yaw_completed)
			m_point = false;
	}

	// make sure that 45 degree cases are not faster
	XMFLOAT3 command = m_moveCommand;
	
	//if (fabsf(command.x) > 0.1f || fabsf(command.z) > 0.1f || fabsf(command.y) > 0.1f)
		normalizeF3(&command);
		command.x = command.x / DIV_CAMERA_SPEED;
		command.y = command.y / DIV_CAMERA_SPEED;
		command.z = command.z / DIV_CAMERA_SPEED;
	

	// integrate
	m_position = XMFLOAT3(m_position.x + command.x, m_position.y + command.y, m_position.z + command.z);
	// Sample collision for origin
	spheresphereCollision(&m_position, 0.5, XMFLOAT3(0, 0, 0), 0.5);
	m_lookat = XMFLOAT3(m_position.x + dir.x, m_position.y + dir.y, m_position.z + dir.z);
	m_upaxis = up_axis;


	// clear movement input accumulator for use during next frame
	m_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);


	// OBJECT MOVEMENT
	if (moveObjectTransform != nullptr) {

		if (obj_fwd) {
			moveObjectTransform->_34 += (timeDelta * OBJ_MOVEMENT_GAIN);
		}
		if (obj_back) {
			moveObjectTransform->_34 -= (timeDelta * OBJ_MOVEMENT_GAIN);
		}
		if (obj_left){
			moveObjectTransform->_14 -= (timeDelta * OBJ_MOVEMENT_GAIN);
		}
		if (obj_rght) {
			moveObjectTransform->_14 += (timeDelta * OBJ_MOVEMENT_GAIN);
		}
		if (obj_up) {
			moveObjectTransform->_24 += (timeDelta * OBJ_MOVEMENT_GAIN);
		}
		if (obj_down) {
			moveObjectTransform->_24 -= (timeDelta * OBJ_MOVEMENT_GAIN);
		}

		// Save new centre point
		XMFLOAT3 centre(moveObjectTransform->_14, moveObjectTransform->_24, moveObjectTransform->_34);

		wallCollision(&centre, 0.5, wallList);

		moveObjectTransform->_14 = centre.x;
		moveObjectTransform->_24 = centre.y;
		moveObjectTransform->_34 = centre.z;

		// Save new coordinates into F3
		// Detect collisions, which may alter F3
		// If true, apply F3 to MOT
	}

}


