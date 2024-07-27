#include <h/Engine.h>



Engine::Engine(ENGINE_CONFIG_ID ID)
{
	glfwInit();
	this->window = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, "Macro Log", NULL, NULL);
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
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	printf("initialized imgui\n");


	this->windowWidth = INIT_WINDOW_WIDTH;

	this->windowHeight = INIT_WINDOW_HEIGHT;
	this->lastMousePos[MOUSE_POS_INDEX_ID::X] = INIT_WINDOW_WIDTH / 2.0;
	this->lastMousePos[MOUSE_POS_INDEX_ID::Y] = INIT_WINDOW_HEIGHT / 2.0;
	this->resourceManager.loadResources();
	printf("Finished engine initialization\n");


	this->worldRenderTarget = this->loadWorld(DEFAULT_1);
	printf("Loaded world DEFAULT_1\n");

}

void _glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void _glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
	Engine& engine = Engine::instance();
	engine.getWorldRenderTarget()->getTargetCamera()->updateMousePos(xpos, ypos, engine.getLastMousePos(MOUSE_POS_INDEX_ID::X), engine.getLastMousePos(MOUSE_POS_INDEX_ID::Y));
	engine.setLastMousePosition(xpos, ypos);

}

void _glfwMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}


void Engine::End()
{
	glfwTerminate();
	if (worldRenderTarget != nullptr)
		delete worldRenderTarget;

}

World *Engine::loadWorld(const WORLD_TYPE &ID)
{
	World *ptr = nullptr;
	switch (ID)
	{
	case DEFAULT_1:
		ptr = new World();
		ptr->addCamera(true, this->windowWidth, this->windowHeight, Camera(windowWidth, windowHeight));
		ptr->addCube(glm::vec3(0, 0, 0), -1.0F, 0.0F, -2.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);
		ptr->addCube(glm::vec3(0, 0, 0), 1.0F, 0.0F, -2.0F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);

		break;
	}


	return ptr;
}

Engine &Engine::instance()
{
	static Engine *instance = new Engine(CONFIG_1);
	return *instance;
}

bool Engine::isRunning()
{
	return this->running;
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
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
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

void Engine::processKeyboardInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->running = false;
	}

// Camera Movement
	Camera *targetCam = this->worldRenderTarget->getTargetCamera();
	if (targetCam)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			targetCam->processCameraMovement(FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			targetCam->processCameraMovement(BACK);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			targetCam->processCameraMovement(LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			targetCam->processCameraMovement(RIGHT);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			targetCam->processCameraMovement(UP);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			targetCam->processCameraMovement(DOWN);
	}

}

void Engine::updateFrame()
{
	processKeyboardInput(window);
	glClearColor(0.1F, 0.1F, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!worldRenderTarget)
		return;
	worldRenderTarget->drawWorld(resourceManager.getMeshBufferPtr(), resourceManager.getShaderBufferPtr());
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

const World *Engine::getWorldRenderTarget()
{
	return worldRenderTarget;
}



