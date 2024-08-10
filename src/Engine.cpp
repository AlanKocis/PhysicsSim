#include <h/Engine.h>

void _glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	static Engine &engine = Engine::instance();
	engine.setWindowWidth(width);
	engine.setWindowHeight(height);
	glViewport(0, 0, width, height);
}

void _glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	Engine &engine = Engine::instance();
	if (engine.shouldMoveTargetCam())
		engine.getWorldRenderTarget()->getTargetCamera()->updateMousePos(xpos, ypos, engine.getLastMousePos(MOUSE_POS_INDEX_ID::X), engine.getLastMousePos(MOUSE_POS_INDEX_ID::Y));
	engine.setLastMousePosition(xpos, ypos);
}

void _glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}

Engine::Engine(ENGINE_CONFIG_ID ID)
{
	glfwInit();
	this->window = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, "hObbes", NULL, NULL);
	if (!this->window)
	{
		printf("Critical error in Engine.cpp :: Couldn't create GLFW window\n");
		glfwTerminate();
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(this->window, &_glfwFramebufferSizeCallback);
	glfwSetCursorPosCallback(this->window, &_glfwCursorPosCallback);
	glfwSetMouseButtonCallback(this->window, &_glfwMouseButtonCallback);
	glfwMakeContextCurrent(this->window);
	glfwSetInputMode(this->window, GLFW_CURSOR, INIT_MOUSE_MODE);
	//printf("initialized glfw\n");
	glfwMakeContextCurrent(this->window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Critical error in Engine.cpp :: GLAD failed\n");
		exit(-1);
	}
	glViewport(0, 0, INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);


	printf("initialized GLFW\n");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;


	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui::StyleColorsLight();
	printf("initialized imgui\n");

	this->windowWidth = INIT_WINDOW_WIDTH;
	this->windowHeight = INIT_WINDOW_HEIGHT;
	this->lastMousePos[MOUSE_POS_INDEX_ID::X] = INIT_WINDOW_WIDTH / 2.0;
	this->lastMousePos[MOUSE_POS_INDEX_ID::Y] = INIT_WINDOW_HEIGHT / 2.0;
	this->lastFrameTime = glfwGetTime();
	this->captureMouseIndex = 1;
	this->running = true;
	this->firstRun = true;
	this->keyboardInputFlag = 1;
	this->resourceManager.loadResources();
//	LOAD WORLD
	this->worldRenderTarget = this->loadWorld(GRAVITY_DEMO);
	printf("Finished engine initialization\n");
}
                                                                       
World *Engine::loadWorld(const WORLD_TYPE &ID)
{
	World *ptr = nullptr;
	switch (ID) 
	{
	case DEFAULT:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		ptr->addCube(glm::vec3(0, 0, 0), -1.0F, 10.0F, -2.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);


		printf("loaded world DEFAULT_1\n");
		break;
	case GRAVITY_DEMO:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		for (float x = 0; x < 30; x += 3)
		{
			for (int z = 0; z < 30; z += 3)
			{
				ptr->addCube(glm::vec3(0, 0, 0), (x - 25.0F), 30.0F, (z - 25.0F), 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);

			}
		}
		printf("loaded world MANY_CUBES\n");
		break;
	case FORCE_DEMO:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		ptr->addCube(glm::vec3(0, 0, 0), 0.0F, 5.0F, -2.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);

		printf("loaded world FORCE_DEMO\n");
		break;
	}


	return ptr;
}

//________________________________________________________________________________________________________
//							 __         ______     ______     ______  
//							/\ \       /\  __ \   /\  __ \   /\  == \ 
//							\ \ \____  \ \ \/\ \  \ \ \/\ \  \ \  _-/ 
//							 \ \_____\  \ \_____\  \ \_____\  \ \_\   
//							  \/_____/   \/_____/   \/_____/   \/_/   
//________________________________________________________________________________________________________
//
//  here 8^)
void Engine::updateFrame()
{
	float T = glfwGetTime();
	this->deltaTime = T - lastFrameTime;
	this->lastFrameTime = T;
	this->FPS = 1.0f / deltaTime;

	//update Positions
	if (this->worldRenderTarget)
		this->worldRenderTarget->update(this->deltaTime);




	processKeyboardInput();
	glClearColor(0.1F, 0.1F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (worldRenderTarget)
		Renderer::DrawWorld(*worldRenderTarget);
	this->firstRun = false;
}


void Engine::updateWindow()
{
	glfwSwapBuffers(this->window);
	glfwPollEvents();

	if (glfwWindowShouldClose(this->window))
	{
		this->running = false;
	}
}



void Engine::updateGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	




	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 200));
	ImGui::Begin("debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	ImGui::TextColored(GUI::WHITE, "%.10fms", this->deltaTime);
	ImGui::TextColored(GUI::WHITE, "%.3ffps", this->FPS);





	ImGui::TextColored(GUI::WHITE, "WASD -\t");
	ImGui::SameLine();
	ImGui::TextColored(GUI::RED, "MOVE");
	ImGui::TextColored(GUI::WHITE, "E\t-\t");
	ImGui::SameLine();
	ImGui::TextColored(GUI::RED, "UP");
	ImGui::TextColored(GUI::WHITE, "Q\t-\t");
	ImGui::SameLine();
	ImGui::TextColored(GUI::RED, "DOWN");
	ImGui::TextColored(GUI::WHITE, "ESC  -\t");
	ImGui::SameLine();
	ImGui::TextColored(GUI::RED, "EDITOR MODE ");

	ImGui::End();

//	Had to wrap this in a function so I could do a safe return; on nullptr.
	this->guiObjectList();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Engine::guiObjectList()
{
	if (this->shouldMoveTargetCam())
		return;
	World *targetWorld = this->worldRenderTarget;
	if (!targetWorld)
		return;
	if (!targetWorld->getTargetCamera())
		return;

	std::vector<Cube> &targetCubeBuffer = this->worldRenderTarget->getCubeBufferReference();
	if (targetCubeBuffer.size() <= 0)
		return;


	ImGui::SetNextWindowPos(ImVec2(0, 200));
	ImGui::SetNextWindowSize(ImVec2(250, 600));
	ImGui::StyleColorsDark();
	ImGui::Begin("Physics", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextColored(GUI::WHITE, "Physics");
	if (ImGui::Button("Start Simulation", ImVec2(200, 20)))
	{
		for (Cube &cube : targetCubeBuffer)
		{
			cube.setPhysics(true);
		}
	}
	if (ImGui::Button("Pause Simulation", ImVec2(200, 20)))
	{
		for (Cube &cube : targetCubeBuffer)
		{
			cube.setPhysics(false);
		}
	}


	static int cubeIndex = 0;
	int height = getWindowHeight();
	
	ImGui::SeparatorText("Obj Debug");
	ImGui::TextColored(GUI::WHITE, "Object Type:\tCube");
	ImGui::TextColored(GUI::WHITE, "Index:      \t%d", cubeIndex);
	if (ImGui::Button("##left", ImVec2(20, 20)))
	{
		if (cubeIndex > 0)
			cubeIndex--;
	}

	ImGui::SameLine();
	if (ImGui::Button("##right", ImVec2(20, 20)))
	{
		if ((cubeIndex + 1) <= targetWorld->getNumCubes())
			cubeIndex++;

	}
	ImGui::Dummy(ImVec2(0.0F, 100.0F));


	static glm::vec3 guiForceVec{ 0.0F, 0.0F, 0.0F };
	Transform &cubeTransform = targetCubeBuffer[cubeIndex].getTransform();



	ImGui::TextColored(GUI::WHITE, "Apply Force:");
	ImGui::SameLine();
	ImGui::InputFloat3("##force", &guiForceVec[0], "%.2f");
	if (ImGui::Button("Apply", ImVec2(40, 20)))
	{
		guiForceVec.x *= this->FPS * 2;
		guiForceVec.y *= this->FPS * 2;
		guiForceVec.z *= this->FPS * 2;
		targetCubeBuffer[cubeIndex].addForce(guiForceVec);
		guiForceVec = glm::vec3(0, 0, 0);
	}


	ImGui::End();
}

void Engine::processKeyboardInput()
{
	static bool mouseModeToggle = 0;
//	Toggle mouse mode
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mouseModeToggle = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		if (mouseModeToggle)
		{
			mouseModeToggle = 0;
			this->mouseData.currentModeFlag = !this->mouseData.currentModeFlag;
			glfwSetInputMode(this->window, GLFW_CURSOR, this->mouseData.mouseModes[this->mouseData.currentModeFlag]);

			if (this->worldRenderTarget && this->worldRenderTarget->getTargetCamera())
							this->worldRenderTarget->getTargetCamera()->setFirstMouse();

		}
	}

// Camera Movement
	Camera *targetCam = worldRenderTarget->getTargetCamera();
	if (targetCam)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			targetCam->processCameraMovement(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			targetCam->processCameraMovement(BACK, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			targetCam->processCameraMovement(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			targetCam->processCameraMovement(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			targetCam->processCameraMovement(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			targetCam->processCameraMovement(DOWN, deltaTime);
	}

}

double Engine::getLastMousePos(const MOUSE_POS_INDEX_ID& index)
{
	return this->lastMousePos[index];
}

void Engine::setLastMousePosition(const double &x, const double &y)
{
	lastMousePos[MOUSE_POS_INDEX_ID::X] = x;
	lastMousePos[MOUSE_POS_INDEX_ID::Y] = y;
}

float &Engine::getDeltaTime()
{
	return deltaTime;
}

float &Engine::getFPS()
{
	return FPS;
}

World *Engine::getWorldRenderTarget()
{
	return worldRenderTarget;
}

Engine &Engine::instance()
{
	static Engine *instance = new Engine(CONFIG_1);
	return *instance;
} 

ResourceManager &Engine::getResourceManager()
{
	return this->resourceManager;
}

bool Engine::isRunning()
{
	return this->running;
}

void Engine::End()
{
	glfwTerminate();
	if (worldRenderTarget != nullptr)
		delete worldRenderTarget;

}

void Engine::setWindowWidth(const int &w)
{
	this->windowWidth = w;
}

void Engine::setWindowHeight(const int &h)
{
	this->windowHeight = h;
}

int Engine::getWindowWidth()
{
	return this->windowWidth;
}

int Engine::getWindowHeight()
{
	return this->windowHeight;
}

bool Engine::shouldMoveTargetCam()
{
	return !this->mouseData.currentModeFlag;
}
