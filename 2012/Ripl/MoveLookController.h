#pragma once

#include "pch.h"

#define ROTATION_GAIN 0.2f	// sensitivity adjustment for look controller
#define MOVEMENT_GAIN 0.5f		// sensitivity adjustment for move controller

using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Devices::Input;

ref class MoveLookController
{
private:
    // properties of the controller object
    XMFLOAT3 m_position, m_lookat, m_upaxis;
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

	float deltaTime;


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

    // set up the Controls that this controller supports
    void Initialize( _In_ Windows::UI::Core::CoreWindow^ window );

internal:
    // accessor to set position of controller
    void SetPosition( _In_ XMFLOAT3 pos );

    // accessor to set position of controller
    void SetOrientation( _In_ float pitch, _In_ float yaw );

    // returns the position of the controller object
    XMFLOAT3 get_Position();

    // returns the point  which the controller is facing
    XMFLOAT3 get_LookAt();

	XMFLOAT3 get_UpAxis();

	void Update( Windows::UI::Core::CoreWindow^ window , float timeDelta);

	XMFLOAT3 computeRAxis();
	XMFLOAT3 computeDirection();

};  // class MoveLookController