#include "pch.h"
#include "RiplGameMain.h"
#include "Common\DirectXHelper.h"

using namespace std;
using namespace RiplGame;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
// Again this is using initialiser list syntax to initialise each member separately
RiplGameMain::RiplGameMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocationX(0.0f)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	m_sceneRenderer = std::unique_ptr<SceneRenderer>(new SceneRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	
	CoreWindow::GetForCurrentThread()->PointerCursor = nullptr;

	//HeightMapInfo* hm = new HeightMapInfo;
	HeightMapInfo* hm = HeightMapLoad("heightmap.bmp");
	// Add breakpoint at this line to see heightmap values
	string width = "width=";
	OutputDebugStringA(width.c_str());

}

// The tilde (~) means this is the DESTRUCTOR, or denitialiser
RiplGameMain::~RiplGameMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void RiplGameMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// This just starts the render loop (the infinite loop that happens for each frame)
void RiplGameMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	// NOTE: The ^ is a microsoft specific implementation of pointers in CLI (google C++ CLI for info)
	// ^ is the same as *, and % is the same as & for these types of pointers
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void RiplGameMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void RiplGameMain::Update() 
{
	// Take care of mouse interaction or touching, etc.
	ProcessInput();

	// Update scene objects.
	// The timer just calculates how much time has passed since the last frame
	// Tick basically says "execute this code when the next frame should happen"
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
	});
}

// Process all input from the user before updating game state (like mouse clicks)
void RiplGameMain::ProcessInput()
{
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool RiplGameMain::Render() 
{
	// Don't try to render anything before the first Update.
	// Or there will nothing to render, silly
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	// Grab the current settings of the graphics device
	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	// Render targets are just things you send pictures to, like a screen!
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	// These things have to do with the queue of frames waiting to be displayed on the screen
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	m_sceneRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void RiplGameMain::OnDeviceLost()
{
	// Basically get rid of, or reset, everything in case the system loses power or something bad happens
	// Or maybe the app just quits
	m_sceneRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void RiplGameMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
