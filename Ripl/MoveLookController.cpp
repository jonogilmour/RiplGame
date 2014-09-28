#include "pch.h"
#include "MoveLookController.h"

XMFLOAT3 norm(XMFLOAT3 in) {
	float len = sqrt(in.x * in.y + in.y * in.y + in.z * in.z);
	return XMFLOAT3(in.x/len, in.y/len, in.z/len);
}

void MoveLookController::Initialize( _In_ CoreWindow^ window )
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

    // Initialize state of the controller
    m_moveInUse = FALSE;				// no pointer is in the Move control
    m_movePointerID = 0;

    m_lookInUse = FALSE;				// no pointer is in the Look control
    m_lookPointerID = 0;

    //	need to init this as it is reset every frame
	m_moveCommand = XMFLOAT3( 0.0f, 0.0f, 0.0f );

    SetOrientation( -(XM_PI/4.0f), 0 );				// look straight ahead when the app starts
	SetPosition(XMFLOAT3(0, 3.0f, -3.0f));
}

void MoveLookController::OnPointerPressed(
_In_ CoreWindow^ sender,
_In_ PointerEventArgs^ args)
{
    // get the current pointer position
    uint32 pointerID = args->CurrentPoint->PointerId;
	XMFLOAT2 position = XMFLOAT2( args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y );

    auto device = args->CurrentPoint->PointerDevice;
    auto deviceType = device->PointerDeviceType;
    if ( deviceType == PointerDeviceType::Mouse )
    {
        // Action, Jump, or Fire
    }

    // check  if this pointer is in the move control
    // Change the values  to percentages of the preferred screen resolution.
    // You can set the x value to <preferred resolution> * <percentage of width>
    // for example, ( position.x < (screenResolution.x * 0.15) )

    if (( position.x < 300 && position.y > 380 ) && ( deviceType != PointerDeviceType::Mouse ))
    {
        if ( !m_moveInUse )	// if no pointer is in this control yet
        {
            // process a DPad touch down event
            m_moveFirstDown = position;					// save location of initial contact
            m_movePointerPosition = position;
            m_movePointerID = pointerID;				// store the id of pointer using this control
            m_moveInUse = TRUE;
        }
    }
    else // this pointer must be in the look control
    {
        if ( !m_lookInUse )	// if no pointer is in this control yet
        {
            m_lookLastPoint = position;							// save point for later move
            m_lookPointerID = args->CurrentPoint->PointerId;	// store the id of pointer using this control
            m_lookLastDelta.x = m_lookLastDelta.y = 0;			// these are for smoothing
            m_lookInUse = TRUE;
        }
    }
}

void MoveLookController::OnPointerMoved(
                                        _In_ CoreWindow ^sender,
                                        _In_ PointerEventArgs ^args)
{
    uint32 pointerID = args->CurrentPoint->PointerId;
    XMFLOAT2 position = XMFLOAT2( args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y );

    // decide which control this pointer is operating
    if ( pointerID == m_movePointerID )			// this is the move pointer
    {
        // Move control
        m_movePointerPosition = position;		// save current position

    }
    else if (pointerID == m_lookPointerID )		// this is the look pointer
    {
        // Look control

        XMFLOAT2 pointerDelta;
		pointerDelta = XMFLOAT2(position.x - m_lookLastPoint.x, position.y - m_lookLastPoint.y);		// how far did pointer move

        XMFLOAT2 rotationDelta;
        rotationDelta = XMFLOAT2(pointerDelta.x * ROTATION_GAIN,pointerDelta.y * ROTATION_GAIN);	// scale for control sensitivity
        m_lookLastPoint = position;			 			// save for next time through

        // update our orientation based on the command
        m_pitch -= rotationDelta.y;						// mouse y increases down, but pitch increases up
        m_yaw   += rotationDelta.x;						// yaw defined as CCW around y-axis

        // Limit pitch to straight up or straight down
        m_pitch = (float) __max( -XM_PI/2.0f, m_pitch );
        m_pitch = (float) __min( +XM_PI/2.0f, m_pitch );
    }
}

void MoveLookController::OnPointerReleased(
_In_ CoreWindow ^sender,
_In_ PointerEventArgs ^args)
{
    uint32 pointerID = args->CurrentPoint->PointerId;
    XMFLOAT2 position = XMFLOAT2( args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y );


    if ( pointerID == m_movePointerID )    // this was the move pointer
    {
        m_moveInUse = FALSE;
        m_movePointerID = 0;
    }
    else if (pointerID == m_lookPointerID ) // this was the look pointer
    {
        m_lookInUse = FALSE;
        m_lookPointerID = 0;
    }
}

void MoveLookController::OnKeyDown(
                                   __in CoreWindow^ sender,
                                   __in KeyEventArgs^ args )
{
    Windows::System::VirtualKey Key;
    Key = args->VirtualKey;

    // figure out the command from the keyboard
    if ( Key == VirtualKey::W )		// forward
        m_forward = true;
    if ( Key == VirtualKey::S )		// back
        m_back = true;
    if ( Key == VirtualKey::A )		// left
        m_left = true;
    if ( Key == VirtualKey::D )		// right
        m_right = true;
	if ( Key == VirtualKey::Space )		// Up
        m_up = true;
	if ( Key == VirtualKey::Control )		// down
        m_down = true;
}

void MoveLookController::OnKeyUp(
                                 __in CoreWindow^ sender,
                                 __in KeyEventArgs^ args)
{
    Windows::System::VirtualKey Key;
    Key = args->VirtualKey;

    // figure out the command from the keyboard
    if ( Key == VirtualKey::W )		// forward
        m_forward = false;
    if ( Key == VirtualKey::S )		// back
        m_back = false;
    if ( Key == VirtualKey::A )		// left
        m_left = false;
    if ( Key == VirtualKey::D )		// right
        m_right = false;
	if ( Key == VirtualKey::Space )		// Up
        m_up = false;
	if ( Key == VirtualKey::Control )		// down
        m_down = false;
}

void MoveLookController::SetPosition( _In_ XMFLOAT3 pos )
{
    m_position = pos;
}

// accessor to set position of controller
void MoveLookController::SetOrientation( _In_ float pitch, _In_ float yaw )
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
        (float)(sinf(m_yaw + XM_PI / 2.0f)),
        0.0f,
        (float)(cosf(m_yaw + XM_PI / 2.0f))
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

void MoveLookController::Update( CoreWindow ^window, float timeDelta)
{
	XMFLOAT3 dir = computeDirection();
	XMFLOAT3 r_axis = computeRAxis();
	XMFLOAT3 up_axis = XMFLOAT3(0,1,0);//computeUpAxis(r_axis, dir);

    // poll our state bits set by the keyboard input events
    if ( m_forward ) {
		m_moveCommand.x += (dir.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (dir.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (dir.z * timeDelta * MOVEMENT_GAIN);
	}
    if ( m_back ) {
        m_moveCommand.x -= (dir.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (dir.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (dir.z * timeDelta * MOVEMENT_GAIN);
	}
    if ( m_left ){
		m_moveCommand.x -= (r_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (r_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (r_axis.z * timeDelta * MOVEMENT_GAIN);
	}
    if ( m_right ) {
        m_moveCommand.x += (r_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (r_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (r_axis.z * timeDelta * MOVEMENT_GAIN);
	}
    if ( m_up ) {
		m_moveCommand.x += (up_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y += (up_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z += (up_axis.z * timeDelta * MOVEMENT_GAIN);
	}
    if ( m_down ) {
		m_moveCommand.x -= (up_axis.x * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.y -= (up_axis.y * timeDelta * MOVEMENT_GAIN);
		m_moveCommand.z -= (up_axis.z * timeDelta * MOVEMENT_GAIN);
	}

    // make sure that 45  degree cases are not faster
	XMFLOAT3 command = m_moveCommand;
    if ( fabsf(command.x) > 0.1f || fabsf(command.z) > 0.1f || fabsf(command.y) > 0.1f )
		command = norm(command);

    // integrate
    m_position = XMFLOAT3(m_position.x + command.x, m_position.y + command.y, m_position.z + command.z);
	m_lookat = XMFLOAT3(m_position.x + dir.x, m_position.y + dir.y, m_position.z + dir.z);
	m_upaxis = up_axis;


    // clear movement input accumulator for use during next frame
	m_moveCommand = XMFLOAT3( 0.0f, 0.0f, 0.0f );

}