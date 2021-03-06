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

	SetPosition(XMFLOAT3(-20, 13, -28));

	acc = Windows::Devices::Sensors::Accelerometer::GetDefault();
	// Establish the report interval

	if (acc != nullptr) {
		auto minimumReportInterval = acc->MinimumReportInterval;
		auto reportInterval = minimumReportInterval > 16 ? minimumReportInterval : 16;
		acc->ReportInterval = reportInterval;
	}

	shouldLookAt = false;

	m_lookat = XMFLOAT3(-28, 4, -28);
}

void MoveLookController::OnPointerPressed(
	_In_ CoreWindow^ sender,
	_In_ PointerEventArgs^ args)
{
}

void MoveLookController::OnPointerMoved(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{
}

void MoveLookController::OnPointerReleased(
	_In_ CoreWindow ^sender,
	_In_ PointerEventArgs ^args)
{
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
		//dest_position = ; // follow cube position
		//dest_lookat = ; // look at cube

		XMFLOAT2 target_orientation = computeTargetOrientation();
		float target_pitch = target_orientation.x;
		float target_yaw = target_orientation.y;

		m_point = true;
	} else if (Key == VirtualKey::P) { // follow the object
		// set m_followBlock to true
		m_followBlock = true;
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

	if (Key == VirtualKey::P) { // dont follow object
		m_followBlock = false;
	}

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

void MoveLookController::moveCameraToLocation(XMFLOAT3 position, XMFLOAT3 lookAt, bool shouldLook) {
	dest_position = position;
	shouldLookAt = shouldLook;
	if (shouldLookAt) dest_lookat = lookAt;
	m_point = true;
}

void MoveLookController::Update(CoreWindow ^window, float timeDelta, XMFLOAT4X4* moveObjectTransform, Size outputSize, XMFLOAT4X4 view, XMFLOAT4X4 proj, std::list<XMFLOAT3>* wallList)
{
	deltaTime = timeDelta;
	XMFLOAT3 dir = computeDirection();
	XMFLOAT3 r_axis = computeRAxis();
	XMFLOAT3 up_axis = computeUpAxis(r_axis, dir);

	if (!m_point) {
		if (acc != nullptr) {
			auto reading = acc->GetCurrentReading();

			auto txtX = reading->AccelerationX;
			m_moveCommand.z += (float)txtX * timeDelta * MOVEMENT_GAIN * 2.0f;
			moveObjectTransform->_34 += (float)txtX * timeDelta * MOVEMENT_GAIN * 2.0f;

			auto txtZ = -reading->AccelerationY;
			m_moveCommand.x += (float)txtZ * timeDelta * MOVEMENT_GAIN;
			moveObjectTransform->_14 += (float)txtZ * timeDelta * MOVEMENT_GAIN;
		}

		// poll our state bits set by the keyboard input events
		if (m_right) {
			m_moveCommand.x += (dir.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y += (dir.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z += (dir.z * timeDelta * MOVEMENT_GAIN);
			moveObjectTransform->_34 += (timeDelta * MOVEMENT_GAIN);
		}
		if (m_left) {
			m_moveCommand.x -= (dir.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y -= (dir.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z -= (dir.z * timeDelta * MOVEMENT_GAIN);
			moveObjectTransform->_34 -= (timeDelta * MOVEMENT_GAIN);
		}
		if (m_forward){
			m_moveCommand.x -= (r_axis.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y -= (r_axis.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z -= (r_axis.z * timeDelta * MOVEMENT_GAIN);
			moveObjectTransform->_14 -= (timeDelta * MOVEMENT_GAIN);
		}
		if (m_back) {
			m_moveCommand.x += (r_axis.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y += (r_axis.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z += (r_axis.z * timeDelta * MOVEMENT_GAIN);
			moveObjectTransform->_14 += (timeDelta * MOVEMENT_GAIN);
		}/*
		if (m_up) {
			m_moveCommand.x += (up_axis.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y += (up_axis.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z += (up_axis.z * timeDelta * MOVEMENT_GAIN);
		}
		if (m_down) {
			m_moveCommand.x -= (up_axis.x * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.y -= (up_axis.y * timeDelta * MOVEMENT_GAIN);
			m_moveCommand.z -= (up_axis.z * timeDelta * MOVEMENT_GAIN);
		}*/

		//m_lookat = XMFLOAT3(moveObjectTransform->_14, 4, moveObjectTransform->_34);
	}

	else { // Move camera to a specific location. if keypressed and current position is not destination position 
		//m_lookat = XMFLOAT3(moveObjectTransform->_14, 4, moveObjectTransform->_34);

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
		} else {
			dir_completed = true;
		}

		if (shouldLookAt) {
			if (m_pitch >= target_pitch + 0.02) {
				m_pitch -= ((m_pitch - target_pitch) * timeDelta * speed);
			}
			else if (m_pitch <= target_pitch - 0.02) {
				m_pitch += ((target_pitch - m_pitch) * timeDelta * speed);
			}
			else {
				pitch_completed = true;
			}

			if (m_yaw >= target_yaw + 0.02) {
				m_yaw -= ((m_yaw - target_yaw) * timeDelta * speed);
			}
			else if (m_yaw <= target_yaw - 0.02) {
				m_yaw += ((target_yaw - m_yaw) * timeDelta * speed);
			}
			else {
				yaw_completed = true;
			}
		}
		else {
			pitch_completed = true;
			yaw_completed = true;
		}

		if (dir_completed && pitch_completed && yaw_completed) {
			m_point = false;
		}
	}

	XMFLOAT3 command = m_moveCommand;
	m_position = XMFLOAT3(m_position.x + command.x, m_position.y + command.y, m_position.z + command.z);
	m_lookat = XMFLOAT3(moveObjectTransform->_14, moveObjectTransform->_24, moveObjectTransform->_34);
	
	m_upaxis = up_axis;

	// clear movement input accumulator for use during next frame
	m_moveCommand = XMFLOAT3(0.0f, 0.0f, 0.0f);

	XMFLOAT3 centre;

	

}


