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
	if (engine.shouldMoveMouse())
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
	this->worldRenderTarget = this->loadWorld(DEFAULT_1);
	printf("Finished engine initialization\n");
}
                                                                       
World *Engine::loadWorld(const WORLD_TYPE &ID)
{
	World *ptr = nullptr;
	switch (ID) 
	{
	case DEFAULT_1:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		ptr->addCube(glm::vec3(0, 0, 0), -1.0F, 10.0F, -2.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);


		printf("loaded world DEFAULT_1\n");
		break;
	case MANY_CUBES:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		for (float x = 0; x < 300; x += 3)
		{
			for (int z = 0; z < 300; z += 3)
			{
				ptr->addCube(glm::vec3(0, 0, 0), (x - 250.0F), 10.0F, (z - 250.0F), 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);

			}
		}
		printf("loaded world MANY_CUBES\n");

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

	static ImVec4 RED(1, 0, 0, 1);
	static ImVec4 WHITE(1, 1, 1, 1);
	




	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::Begin("debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	ImGui::TextColored(WHITE, "%.10fms", this->deltaTime);
	ImGui::TextColored(WHITE, "%.3ffps", this->FPS);





	ImGui::TextColored(WHITE, "WASD -\t");
	ImGui::SameLine();
	ImGui::TextColored(RED, "MOVE");
	ImGui::TextColored(WHITE, "E\t-\t");
	ImGui::SameLine();
	ImGui::TextColored(RED, "UP");
	ImGui::TextColored(WHITE, "Q\t-\t");
	ImGui::SameLine();
	ImGui::TextColored(RED, "DOWN");
	ImGui::TextColored(WHITE, "ESC  -\t");
	ImGui::SameLine();
	ImGui::TextColored(RED, "TOGGLE MOUSE ");

	ImGui::End();

	
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

bool Engine::shouldMoveMouse()
{
	return !this->mouseData.currentModeFlag;
}
