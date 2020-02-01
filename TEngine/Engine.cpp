#include "Engine.h"
#include "Core/Config/Config.h"
#include "Render/D3D11/D3D11Renderer.h"
#include "Helpers/DebugSystem.h"

namespace TEngine
{
	Engine* Engine::instance = nullptr;

	Engine::Engine() 
		: systemsStack(sizeof(WindowManager) + sizeof(InputSystem)
			+ sizeof(WorldSystem) + sizeof(RENDERER))
	{
		maxFixedSteps = CONFIG_INT32("Loop", "maxFixedSteps", ConfigVar("8"));
		fixedTimeStep = CONFIG_FLOAT32("Loop", "fixedTimeStep", ConfigVar("0.02"));

		windowManager = systemsStack.NewOnStack<WindowManager>();
		inputSystem = systemsStack.NewOnStack<InputSystem>();
		worldSystem = systemsStack.NewOnStack<WorldSystem>();
		renderer = systemsStack.NewOnStack<RENDERER>();
#ifdef _DEBUG
		debugSystem = new DebugSystem();
#endif
		instance = this;
	}

	Engine::~Engine()
	{
		// Subsystems will be freed as stack is deconstructed
#ifdef _DEBUG
		delete debugSystem;
#endif
	}

	Clock& Engine::GetClock()
	{
		return gameClock;
	}

	RENDERER& Engine::GetRenderer() const
	{
		return *renderer;
	}

	WorldSystem& Engine::GetWorldSys() const
	{
		return *worldSystem;
	}

	void Engine::StartUp()
	{
		//windowManager->StartUp();
		//inputSystem->StartUp(windowManager->GetWindow());
		worldSystem->StartUp();
		//renderer->StartUp(windowManager->GetWindow(), worldSystem);
		renderer->StartUp();
#ifdef _DEBUG
		//debugSystem->StartUp(windowManager->GetWindow());
#endif
	}

	void Engine::ShutDown()
	{
#ifdef _DEBUG
		//debugSystem->ShutDown();
#endif
		renderer->ShutDown();
		worldSystem->ShutDown();
		//inputSystem->ShutDown();
		//windowManager->ShutDown();
	}

	void Engine::Update()
	{
		gameClock.Update();

		accumulatedTime += gameClock.GetDeltaTime();

		// If game lags, make sure fixed update catches up
		uint8 stepCount = 0;
		while (accumulatedTime >= fixedTimeStep && stepCount <= maxFixedSteps) 
		{
			accumulatedTime -= fixedTimeStep;
			FixedUpdate(fixedTimeStep);
			stepCount++;
		}

		VariableUpdate((float32)gameClock.GetDeltaTime());
	}

	void Engine::FixedUpdate(const float32 timeStep)
	{
		worldSystem->FixedUpdate(timeStep);
#ifdef _DEBUG
		//debugSystem->FixedUpdate(timeStep);
#endif
	}

	void Engine::VariableUpdate(const float32 deltaTime)
	{
		//inputSystem->Update();
		worldSystem->Update(deltaTime);

		renderer->ClearBuffer(0.f, 0.f, 0.f);
		renderer->Render(deltaTime);
		renderer->Present();

#ifdef _DEBUG
		//debugSystem->VariableUpdate(deltaTime);
#endif
		//windowManager->Update(deltaTime);
	}
}
