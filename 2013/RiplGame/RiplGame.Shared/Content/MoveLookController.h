#pragma once

#include "pch.h"
#include "Content\Structures\OtherStructures.h"
//#include "Ray.h"

#define ROTATION_GAIN 0.2f	// sensitivity adjustment for look controller
#define MOVEMENT_GAIN 6.0f		// sensitivity adjustment for move controller
#define OBJ_MOVEMENT_GAIN 2.0f
#define DIV_CAMERA_SPEED 10.0f

using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Devices::Input;
using namespace DirectX::TriangleTests;

namespace RiplGame {
	ref class MoveLookController
	{
	private:
		// properties of the controller object
		XMFLOAT3 m_position, m_lookat, m_upaxis, m_camPosition;
		float m_pitch, m_yaw;			// orientation euler angles in radians

		// properties of the Move control
		bool m_moveInUse;			    // specifies whether the move control is in use
		uint32 m_movePointerID;		    // id of the pointer in this control
		XMFLOAT2 m_moveFirstDown;		    // point where initial contact occurred
		XMFLOAT2 m_movePointerPosition;   // point where the move pointer is currently located
		XMFLOAT3 m_moveCommand;		    // the net command from the move control

		// properties of the Look control
		bool m_lookInUse;			    // specifies whether the look control is in use
		uint32 m_lookPointerID;		    // id of the pointer in this control
		XMFLOAT2 m_lookLastPoint;		    // last point (from last frame)
		XMFLOAT2 m_lookLastDelta;		    // for smoothing

		bool m_forward, m_back;			// states for movement
		bool m_left, m_right;
		bool m_up, m_down;

		bool obj_up, obj_down, obj_fwd, obj_back, obj_left, obj_rght;

		float deltaTime;

		XMFLOAT3 dest_position, dir_vector, dest_lookat;
		float target_pitch, target_yaw;
		bool m_point, reached_dest;
		bool m_followBlock; // should the camera follow the block

		//Ray Tracing STANLEY
		bool tapped;
		XMFLOAT2 tapPosition;
		XMFLOAT2 ripplePosition;

	public:

		// Methods to get input from the UI pointers
		void OnPointerPressed(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::PointerEventArgs^ args
			);

		void OnPointerMoved(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::PointerEventArgs^ args
			);

		void OnPointerReleased(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::PointerEventArgs^ args
			);

		void OnKeyDown(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
			);

		void OnKeyUp(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
			);

		void OnMouseMoved(
			_In_ MouseDevice^,
			_In_ MouseEventArgs^ args
			);

		// set up the Controls that this controller supports
		void Initialize(_In_ Windows::UI::Core::CoreWindow^ window);
		void rayCalc(Size size, int x, int y);
		

	internal:
		// accessor to set position of controller
		void SetPosition(_In_ XMFLOAT3 pos);

		// accessor to set position of controller
		void SetOrientation(_In_ float pitch, _In_ float yaw);

		// returns the position of the controller object
		XMFLOAT3 get_Position();

		// returns the point  which the controller is facing
		XMFLOAT3 get_LookAt();

		XMFLOAT3 get_UpAxis();

		void Update(Windows::UI::Core::CoreWindow^ window, float timeDelta, XMFLOAT4X4* moveObjectTransform, Size outputSize, XMFLOAT4X4 proj, XMFLOAT4X4 view, std::list<XMFLOAT3>* wallList);

		XMFLOAT3 computeRAxis();
		XMFLOAT3 computeDirection();

		XMFLOAT3 camPosition();

		void moveTo(XMFLOAT3 center);

		bool equal(XMFLOAT3 pos1, XMFLOAT3 pos2);
		XMFLOAT3 computeDirectionVector();
		XMFLOAT3 computeLookAtVector();
		XMFLOAT2 computeTargetOrientation();

	};  // class MoveLookController
}
