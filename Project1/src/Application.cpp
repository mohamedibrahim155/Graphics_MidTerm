


#include "Shader.h"
#include <glad/glad.h>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexArray.h"
#include  "model.h"
#include "material.h"
#include "Light.h"
#include "Transform.h"
#include"LightManager.h"
#include <random>


bool switchCamera = false;
bool switchToPointLight = false;

glm::vec3 currentLight(1.0f, 0.0f, 1.0f);
glm::vec3 spotlightPos(1.0f, 0.0f, 1.0f);
glm::vec3 PointLightPos(-1.0f, 0.0f, 1.0f);

bool isAnimationKeyPressed = false;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        switchCamera = !switchCamera;
        std::cout << "Value of swtich camera : " << switchCamera << std::endl;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {

        switchToPointLight = !switchToPointLight;
        std::cout << "Value of swtich pointLight : " << switchToPointLight << std::endl;
        if (switchToPointLight)
        {
            currentLight = PointLightPos;
        }
        else
        {
            currentLight = spotlightPos;

        }
    }


    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
     

    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        isAnimationKeyPressed = !isAnimationKeyPressed;
    }

}



const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


Camera camera(glm::vec3(10, 2, 20));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<Model*> loadedModels;
std::vector<Model*> lightDebugModels;
std::vector<Model*> animatingDoorModels;
std::vector<Model*> starsModels;
Model* testingModel;
bool isTestingModel = false;
void CheckingValues(Model* testModel, float x, float y, float z);
bool isfirstDoorOpen = false;
void AnimationModels(float deltaTime);

int randomNumberGen(int min, int max);
void LoadStarsModel();
int main()
{



    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "KAIZOKU", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    glEnable(GL_DEPTH_TEST);


    Shader defaultShader("Shaders/Light_VertexShader.vert", "Shaders/Light_FragmentShader.frag");


    Shader lightSource("Shaders/lighting.vert", "Shaders/lighting.frag");


 ////////// Loading every models
#pragma region ModelLoading

// Loading all the models here
#pragma region Wall Model
    Model* wall = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Wall_02_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    // wallcopy  Straight
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            Model* wallCopy = new Model();
            wallCopy->meshes = std::vector<Mesh>(wall->meshes.begin(), wall->meshes.end());

           // wallCopy->transform.SetTranslation(glm::vec3(j*10, i * 5, 0));
            wallCopy->transform.SetTranslation(glm::vec3(i*10, j * 5, 0));
            loadedModels.push_back(wallCopy);
            animatingDoorModels.push_back(wallCopy);
        }
      
    }

    // wallcopy  back
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            Model* wallCopy = new Model();
            wallCopy->meshes = std::vector<Mesh>(wall->meshes.begin(), wall->meshes.end());

            // wallCopy->transform.SetTranslation(glm::vec3(j*10, i * 5, 0));
            wallCopy->transform.SetTranslation(glm::vec3(10+(i * 10), j * 5, 50));
            wallCopy->transform.SetRotation(180, glm::vec3(0, 1, 0));
            loadedModels.push_back(wallCopy);
            //testingModel = wallCopy;
            //isTestingModel = true;
           
        }

    }


    // wallcopy  right
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            Model* wallCopy = new Model();
            wallCopy->meshes = std::vector<Mesh>(wall->meshes.begin(), wall->meshes.end());

            wallCopy->transform.SetTranslation(glm::vec3(40, i * 5, j * 10));
            wallCopy->transform.SetRotation(-90,glm::vec3(0, 1, 0));
            loadedModels.push_back(wallCopy);
        }

    }
    // wallcopy  left
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            Model* wallCopy = new Model();
            wallCopy->meshes = std::vector<Mesh>(wall->meshes.begin(), wall->meshes.end());

            wallCopy->transform.SetTranslation(glm::vec3(0, i * 5, j * 10+10));
            wallCopy->transform.SetRotation(90, glm::vec3(0, 1, 0));
            loadedModels.push_back(wallCopy);
        }

    }
  
#pragma endregion

#pragma region Floor Model
    Model* floor = new Model((char*)"Models/SpaceStation/SM_Env_Floor_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            Model* floorCopy = new Model();
            floorCopy->meshes = std::vector<Mesh>(floor->meshes.begin(), floor->meshes.end());

            //floorCopy->transform.SetTranslation(glm::vec3(j * 10, i * 5, 0));
            floorCopy->transform.SetTranslation(glm::vec3(5+i*5, 0, 0+j*5));
            loadedModels.push_back(floorCopy);
        }

    }
#pragma endregion


#pragma region ceiling

    Model* ceiling = new Model((char*)"Models/SpaceStation/SM_Env_Ceiling_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 10; j++)
        {
            Model* ceilingCopy = new Model();
            ceilingCopy->meshes = std::vector<Mesh>(ceiling->meshes.begin(), ceiling->meshes.end());

            //floorCopy->transform.SetTranslation(glm::vec3(j * 10, i * 5, 0));
            ceilingCopy->transform.SetTranslation(glm::vec3(5 + i * 5, 25, 5 + j * 5));
            loadedModels.push_back(ceilingCopy);
        }

    }
#pragma endregion

#pragma region rail
    Model* constructionBlock = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Block_06_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    //straight rails
    for (size_t i = 0; i < 8; i++)
    {
        Model* constructionBlockCopy = new Model();
        constructionBlockCopy->meshes = std::vector<Mesh>(constructionBlock->meshes.begin(), constructionBlock->meshes.end());
        constructionBlockCopy->transform.SetTranslation(glm::vec3(5+(i*5), 15.0f, 25.0f));
        constructionBlockCopy->transform.SetRotation(180,glm::vec3(1,0,0));
        loadedModels.push_back(constructionBlockCopy);
    }
   
    Model* constructionBlockCopy3 = new Model();
    constructionBlockCopy3->meshes = std::vector<Mesh>(constructionBlock->meshes.begin(), constructionBlock->meshes.end());
    constructionBlockCopy3->transform.SetTranslation(glm::vec3(5, 15.0f, 20));
    constructionBlockCopy3->transform.SetRotation(180, glm::vec3(1, 0, 0));
    loadedModels.push_back(constructionBlockCopy3);



    for (size_t j = 0; j < 3; j++) 
    {
        float zOffset;
        if (j < 2) {
            zOffset = (j == 0) ? 30.0f : 35.0f;
            for (size_t i = 0; i < 2; i++) 
            {
                Model* constructionBlockCopy2 = new Model();
                constructionBlockCopy2->meshes = std::vector<Mesh>(constructionBlock->meshes.begin(), constructionBlock->meshes.end());
                constructionBlockCopy2->transform.SetTranslation(glm::vec3(40, 5 * i, zOffset));
                loadedModels.push_back(constructionBlockCopy2);
            }
        }
        else 
        {
            zOffset = 40.0f;
            Model* constructionBlockCopy2 = new Model();
            constructionBlockCopy2->meshes = std::vector<Mesh>(constructionBlock->meshes.begin(), constructionBlock->meshes.end());
            constructionBlockCopy2->transform.SetTranslation(glm::vec3(40, 0, zOffset));
            loadedModels.push_back(constructionBlockCopy2);
        }
    }

  

#pragma endregion


#pragma region stairs
    Model* stairs = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Stairs_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    
    
    float translations[3] = { 45, 40, 35 }; // array of z-axis translation values
    float initialY = 0; // initial y translation value
    float deltaY = 5; // difference between consecutive y translation values

    for (int i = 0; i < 3; i++) {
        Model* stairsCopy = new Model();
        stairsCopy->meshes = std::vector<Mesh>(stairs->meshes.begin(), stairs->meshes.end());
        stairsCopy->transform.SetTranslation(glm::vec3(35, initialY + i * deltaY, translations[i]));
        stairsCopy->transform.SetRotation(-90, glm::vec3(0, 1, 0));
        loadedModels.push_back(stairsCopy);
    }

    
#pragma endregion

#pragma region panel

    Model* panel = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Panel_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    for (size_t i = 0; i < 8; i++)
    {
        Model* panelCopy = new Model();
        panelCopy->meshes = std::vector<Mesh>(panel->meshes.begin(), panel->meshes.end());
        panelCopy->transform.SetTranslation(glm::vec3(5+(i*5.0f), 14.8f, 30.0f));
        // panelCopy->transform.SetRotation(-90, glm::vec3(0, 1, 0));
        loadedModels.push_back(panelCopy);
    }
    Model* panelCopy2 = new Model();
    panelCopy2->meshes = std::vector<Mesh>(panel->meshes.begin(), panel->meshes.end());
    panelCopy2->transform.SetTranslation(glm::vec3(5, 14.8f, 25.0f));
    // panelCopy->transform.SetRotation(-90, glm::vec3(0, 1, 0));
    loadedModels.push_back(panelCopy2);

    //testingModel = panelCopy;
    //isTestingModel = true;
#pragma endregion


#pragma region RailConstruction

    Model* railConnector = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Rail_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    for (size_t i = 0; i < 14; i++) {
        Model* railconnecorCopy = new Model();
        railconnecorCopy->meshes = std::vector<Mesh>(railConnector->meshes.begin(), railConnector->meshes.end());

        if (i < 7) {
            railconnecorCopy->transform.SetTranslation(glm::vec3(0 + (i * 5.0f), 15.0f, 30.0f));
        }
        else if (i < 15) 
        {
            railconnecorCopy->transform.SetTranslation(glm::vec3(5 + ((i - 7) * 5.0f), 15.0f, 25.0f));
        }

        railconnecorCopy->transform.SetRotation(-90, glm::vec3(0, 1, 0));
        loadedModels.push_back(railconnecorCopy);
    }
  
    Model* railConnectorUShaped = new Model((char*)"Models/SpaceStation/SM_Env_Construction_Rail_03_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    Model* railConnectorUShapedCopy = new Model();
    railConnectorUShapedCopy->meshes = std::vector<Mesh>(railConnectorUShaped->meshes.begin(), railConnectorUShaped->meshes.end());
    railConnectorUShapedCopy->transform.SetTranslation(glm::vec3(5,15,20));
    railConnectorUShapedCopy->transform.SetRotation(90, glm::vec3(0, 1, 0));
    loadedModels.push_back(railConnectorUShapedCopy);
 
    //testingModel = railConnectorUShapedCopy;
    //isTestingModel = true;

#pragma endregion


#pragma region CeilingLight

    Model* ceilingLight = new Model((char*)"Models/SpaceStation/SM_Prop_Light_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
  

    for (size_t j = 0; j < 9; j++) {
        float zOffset;
        if (j < 3) {
            zOffset = 5.0f;
        }
        else if (j < 6) {
            zOffset = 20.0f;
        }
        else {
            zOffset = 40.0f;
        }

        size_t i = j % 3;
        Model* CeilingCentrifugeCopy = new Model();
        CeilingCentrifugeCopy->meshes = std::vector<Mesh>(ceilingLight->meshes.begin(), ceilingLight->meshes.end());
        CeilingCentrifugeCopy->transform.SetTranslation(glm::vec3(10 + (i * 10), 26.0f, zOffset));
        loadedModels.push_back(CeilingCentrifugeCopy);
    }
   

  /*  testingModel = CeilingCentrifugeCopy;
    isTestingModel = true;*/

#pragma endregion


#pragma region Plane                                                                                                                                                                                                              www

    Model* plane = new Model((char*)"Models/SpaceStation/SM_Prop_Shuttle_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    
    plane->transform.SetTranslation(glm::vec3(20, 0, 35));
    plane->transform.SetRotation(180, glm::vec3(0, 1, 0));

    loadedModels.push_back(plane);
  // testingModel = plane;
  // isTestingModel = true;

#pragma endregion


#pragma region desk
    Model* desk = new Model((char*)"Models/SpaceStation/SM_Prop_Desk_Lab_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    desk->transform.SetTranslation(glm::vec3(10, 0, 40));
    desk->transform.SetRotation(-40.0f, glm::vec3(0, 1, 0));
    loadedModels.push_back(desk);


    Model* desk2 = new Model((char*)"Models/SpaceStation/SM_Prop_Desk_Lab_02_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    desk2->transform.SetTranslation(glm::vec3(5, 0, 38));
    loadedModels.push_back(desk2);


    Model* desk3 = new Model((char*)"Models/SpaceStation/SM_Prop_ControlDesk_03_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    desk3->transform.SetTranslation(glm::vec3(17, 0, 46));
    desk3->transform.SetScale(glm::vec3(0.85f));
    loadedModels.push_back(desk3);

    Model* deskLamp = new Model((char*)"Models/SpaceStation/SM_Item_Lamp_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    deskLamp->transform.SetTranslation(glm::vec3(18, 2.3f, 45.8f));
    deskLamp->transform.SetRotation(-70,glm::vec3(0,1,0));
    loadedModels.push_back(deskLamp);
//    testingModel = deskLamp;
//isTestingModel = true;


#pragma endregion

#pragma region DoorWayAndDoor

    Model* doorway = new Model((char*)"Models/SpaceStation/SM_Env_ControlRoom_Doorway_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    doorway->transform.SetTranslation(glm::vec3(0.5f, 15, 24.8f));
    doorway->transform.SetRotation(90, glm::vec3(0, 1, 0));
    loadedModels.push_back(doorway);

   

    Model* door = new Model((char*)"Models/SpaceStation/SM_Env_ControlRoom_Door_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    door->transform.SetTranslation(glm::vec3(0.4, 15, 28.4f));
    door->transform.SetRotation(90, glm::vec3(0, 1, 0));
    loadedModels.push_back(door);

    //testingModel = door;
    //isTestingModel = true;

#pragma endregion


#pragma region satelitte

    Model* satelitte = new Model((char*)"Models/SpaceStation/SM_Prop_Satellite_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    satelitte->transform.SetTranslation(glm::vec3(10, 0, 12));
    satelitte->transform.SetRotation(45, glm::vec3(0, 1, 0));
    satelitte->transform.SetScale(glm::vec3(0.75f));

    loadedModels.push_back(satelitte);
     //testingModel = satelitte;
     //isTestingModel = true;

#pragma endregion

#pragma region rocket

    Model* rocket = new Model((char*)"Models/SpaceStation/SM_Prop_Rocket_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    rocket->transform.SetTranslation(glm::vec3(35, 0, 10));
   // rocket->transform.SetRotation(0, glm::vec3(0, 1, 0));
    loadedModels.push_back(rocket);


#pragma endregion

#pragma region Props

    //Loading prop models
    Model* stirCase = new Model((char*)"Models/SpaceStation/SM_Prop_Stairs_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    stirCase->transform.SetTranslation(glm::vec3(4, 0, 33.0f));
    stirCase->transform.SetRotation(-110, glm::vec3(0, 1, 0));
    loadedModels.push_back(stirCase);


    Model* Chair = new Model((char*)"Models/SpaceStation/SM_Prop_SwivelChair_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    Chair->transform.SetTranslation(glm::vec3(8.8f, 0, 42.0f));
    Chair->transform.SetRotation(135, glm::vec3(0, 1, 0));
    loadedModels.push_back(Chair);


    Model* Chair2 = new Model((char*)"Models/SpaceStation/SM_Prop_SwivelChair_04_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    Chair2->transform.SetTranslation(glm::vec3(5, 0, 40));
    Chair2->transform.SetRotation(180, glm::vec3(0, 1, 0));
    loadedModels.push_back(Chair2);

    Model* trolley = new Model((char*)"Models/SpaceStation/SM_Prop_Trolley_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    trolley->transform.SetTranslation(glm::vec3(7.5f, 0, 33));
    trolley->transform.SetRotation(120, glm::vec3(0, 1, 0));
    trolley->transform.SetScale(glm::vec3(0.5f));
    loadedModels.push_back(trolley);

    Model* table = new Model((char*)"Models/SpaceStation/SM_Prop_Table_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    table->transform.SetTranslation(glm::vec3(6.5f, 0, 35.5f));
    table->transform.SetRotation(20, glm::vec3(0, 1, 0));
    table->transform.SetScale(glm::vec3(0.75f));
    loadedModels.push_back(table);

    Model* battery = new Model((char*)"Models/SpaceStation/SM_Prop_Battery_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    battery->transform.SetTranslation(glm::vec3(5.8f, 1.1f, 35.6f));
    battery->transform.SetRotation(20, glm::vec3(0, 1, 0));
    battery->transform.SetScale(glm::vec3(0.5f));
    loadedModels.push_back(battery);


    Model* battery2 = new Model((char*)"Models/SpaceStation/SM_Prop_Battery_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    battery2->transform.SetTranslation(glm::vec3(6.9f, 1.1f, 35.4f));
    battery2->transform.SetRotation(60, glm::vec3(0, 1, 0));
    battery2->transform.SetScale(glm::vec3(0.5f));
    loadedModels.push_back(battery2);


    Model* tape = new Model((char*)"Models/SpaceStation/SM_Prop_Tape_02_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    tape->transform.SetTranslation(glm::vec3(3, 0, 20));
    loadedModels.push_back(tape);

    Model* tapeCopy = new Model();
    tapeCopy->meshes = std::vector<Mesh>(tape->meshes.begin(), tape->meshes.end());
    tapeCopy->transform.SetTranslation(glm::vec3(7, 0, 18.5f));
    tapeCopy->transform.SetRotation(-130,glm::vec3(0, 1, 0));
    loadedModels.push_back(tapeCopy);

    Model* cabinet = new Model((char*)"Models/SpaceStation/SM_Prop_Cabinets_02_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    cabinet->transform.SetTranslation(glm::vec3(1, 0, 24));
    cabinet->transform.SetRotation(90,glm::vec3(0, 1, 0));
    loadedModels.push_back(cabinet);

    Model* server = new Model((char*)"Models/SpaceStation/SM_Prop_Server_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);

    server->transform.SetTranslation(glm::vec3(1.2f, 0, 18.5f));
    server->transform.SetRotation(90, glm::vec3(0, 1, 0));
    loadedModels.push_back(server);

    Model* serverCopy = new Model();
    serverCopy->meshes = std::vector<Mesh>(server->meshes.begin(), server->meshes.end());
    serverCopy->transform.SetTranslation(glm::vec3(1.2f, 0, 16.5f));
    serverCopy->transform.SetRotation(90, glm::vec3(0, 1, 0));
    loadedModels.push_back(serverCopy);

    Model* tableEmpty = new Model((char*)"Models/SpaceStation/SM_Prop_Desk_01_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    tableEmpty->transform.SetTranslation(glm::vec3(5, 0, 28.0f));
    tableEmpty->transform.SetRotation(-45, glm::vec3(0, 1, 0));
    loadedModels.push_back(tableEmpty);

    Model* Chair3 = new Model((char*)"Models/SpaceStation/SM_Prop_SwivelChair_04_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    Chair3->transform.SetTranslation(glm::vec3(4.0f, 0, 30));
    Chair3->transform.SetRotation(180, glm::vec3(0, 1, 0));
    loadedModels.push_back(Chair3);

    Model* Monitor = new Model((char*)"Models/SpaceStation/SM_Prop_Monitor_03_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    Monitor->transform.SetTranslation(glm::vec3(4.0f, 1.5f, 27));
    Monitor->transform.SetRotation(-45, glm::vec3(0, 1, 0));
    loadedModels.push_back(Monitor);

    Model* CelingLight = new Model((char*)"Models/SpaceStation/SM_Env_Ceiling_Light_04_xyz_n_rgba_uv_flatshaded_xyz_n_rgba_uv.ply", false, true);
    CelingLight->transform.SetTranslation(glm::vec3(21.5f, 10.2f, 28.0));
    //CelingLight->transform.SetRotation(-45, glm::vec3(0, 1, 0));
    loadedModels.push_back(CelingLight);
    //testingModel = CelingLight;
    //isTestingModel = true;

#pragma endregion



#pragma endregion
    Material material( 128.0f);


    LoadStarsModel(); // laoding starts as sphere model
    
    Renderer render;

    // lightObject reference declration and assigning to model
// applying required parameters for light

#pragma region Mulitple LightHandler


    // lightObject reference declration and assigning to model
    // applying required parameters for light







    LightManager lightManager;

    Model* directionLightModel = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", false, true);
    lightDebugModels.push_back(directionLightModel);
  
    Light directionLight;
    directionLight.lightType = LightType:: DIRECTION_LIGHT;
    directionLight.lightModel = directionLightModel;
    directionLight.ambient =  glm::vec3(0.1f);
    directionLight.specular = glm::vec3(0.1f);
    directionLight.diffuse =  glm::vec3(0.1f);

    Model* SpotLightModel = new Model();
    SpotLightModel = &(*directionLightModel);
    lightDebugModels.push_back(SpotLightModel);
    SpotLightModel->transform.SetTranslation(glm::vec3(10, 20.5f, 5));

    Light SpotLight1;
    SpotLight1.lightType = LightType::SPOT_LIGHT;
    SpotLight1.lightModel = SpotLightModel;
    SpotLight1.ambient = glm:: vec3(5);
    SpotLight1.specular = glm:: vec3(5);
    SpotLight1.diffuse = glm:: vec3(5);
    SpotLight1.direction = glm::vec3(0, -1, 0);
    SpotLight1.outerCutOffAngle = 20.0f;
    SpotLight1.cutOffAngle = 18.0f;



    Model* SpotLight2Model = new Model();
    SpotLight2Model->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLight2Model->transform.SetTranslation(glm::vec3(20, 20.5f, 5));
    lightDebugModels.push_back(SpotLight2Model);

    Light spotLight2;
    spotLight2.lightType = LightType::SPOT_LIGHT;
    spotLight2.lightModel = SpotLight2Model;
    spotLight2.ambient = glm::vec3(100);
    spotLight2.specular = glm::vec3(100);
    spotLight2.diffuse = glm::vec3(100);
    spotLight2.direction = glm::vec3(0, -1, 0);
    spotLight2.constant = 1;
    spotLight2.linear = 1;
    spotLight2.quadratic = 1;
    spotLight2.cutOffAngle = 15;
    spotLight2.outerCutOffAngle = 20; 

    Model* SpotLightModel3 = new Model();
    SpotLightModel3->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel3->transform.SetTranslation(glm::vec3(30, 20.5f, 5));
    lightDebugModels.push_back(SpotLightModel3);

    Light Spotlight3;
    Spotlight3.lightType = LightType::SPOT_LIGHT;
    Spotlight3.lightModel = SpotLightModel3;
    Spotlight3.ambient = glm::vec3(100);
    Spotlight3.specular = glm::vec3(100);
    Spotlight3.diffuse = glm::vec3(100);
    Spotlight3.direction = glm::vec3(0, -1, 0);
    Spotlight3.constant = 1;
    Spotlight3.linear = 1;
    Spotlight3.quadratic = 1;
    Spotlight3.cutOffAngle = 15;
    Spotlight3.outerCutOffAngle = 20;



    Model* SpotLightModel4 = new Model();
    SpotLightModel4->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel4->transform.SetTranslation(glm::vec3(10, 20.5f, 20));
    lightDebugModels.push_back(SpotLightModel4);

    Light SpotLight4;
    SpotLight4.lightType = LightType::SPOT_LIGHT;
    SpotLight4.lightModel = SpotLightModel4;

   SpotLight4.ambient = glm::vec3(100);
   SpotLight4.specular = glm::vec3(100);
   SpotLight4.diffuse = glm::vec3(100);
   SpotLight4.direction = glm::vec3(0, -1, 0);
   SpotLight4.constant = 1;
   SpotLight4.linear = 1;
   SpotLight4.quadratic = 1;
   SpotLight4.cutOffAngle = 15;
   SpotLight4.outerCutOffAngle = 20;

    



    Model* SpotLightModel5 = new Model();
    SpotLightModel5->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel5->transform.SetTranslation(glm::vec3(20, 20.5f, 20));
    lightDebugModels.push_back(SpotLightModel5);

    Light spotLight5;
    spotLight5.lightType = LightType::SPOT_LIGHT;
    spotLight5.lightModel = SpotLightModel5;
    spotLight5.ambient = glm::vec3(100);
    spotLight5.specular = glm::vec3(100);
    spotLight5.diffuse = glm::vec3(100);
    spotLight5.direction = glm::vec3(0, -1, 0);
    spotLight5.constant = 1;
    spotLight5.linear = 1;
    spotLight5.quadratic = 1;
    spotLight5.cutOffAngle = 15;
    spotLight5.outerCutOffAngle = 20;

    Model* SpotLightModel6 = new Model();
    SpotLightModel6->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel6->transform.SetTranslation(glm::vec3(30, 20.5f, 20));
    lightDebugModels.push_back(SpotLightModel6);

    Light SpotLight6;
    SpotLight6.lightType = LightType::SPOT_LIGHT;
    SpotLight6.lightModel = SpotLightModel6;
    SpotLight6.ambient =   glm::vec3(10);
    SpotLight6.specular =  glm::vec3(10);
    SpotLight6.diffuse =   glm::vec3(10);
    SpotLight6.direction = glm::vec3(0, -1, 0);
    SpotLight6.constant = 0.1f;
    SpotLight6.linear = 0.1f;
    SpotLight6.quadratic = 0.1f;
    SpotLight6.cutOffAngle = 20;
    SpotLight6.outerCutOffAngle = 20;


    Model* pointLightModel1 = new Model();
    pointLightModel1->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    pointLightModel1->transform.SetTranslation(glm::vec3(10, 20.5f, 40));
    lightDebugModels.push_back(pointLightModel1);

    Light pointLight1;
    pointLight1.lightType = LightType::POINT_LIGHT;
    pointLight1.lightModel = pointLightModel1;
    pointLight1.ambient =  glm::vec3(2);
    pointLight1.specular = glm::vec3(2);
    pointLight1.diffuse =  glm::vec3(2);

    Model* pointLightModel2 = new Model();
    pointLightModel2->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    pointLightModel2->transform.SetTranslation(glm::vec3(20, 20.5f, 40));
    lightDebugModels.push_back(pointLightModel2);

    Light PointLight2;
    PointLight2.lightType = LightType::POINT_LIGHT;
    PointLight2.lightModel = pointLightModel2;
    PointLight2.ambient =  glm::vec3(2);
    PointLight2.specular = glm::vec3(2);
    PointLight2.diffuse =  glm::vec3(2);


    Model* pointLightModel3 = new Model();
    pointLightModel3->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    pointLightModel3->transform.SetTranslation(glm::vec3(30, 20.5f, 40));
    lightDebugModels.push_back(pointLightModel3);

    Light PointLight3;
    PointLight3.lightType = LightType::POINT_LIGHT;
    PointLight3.lightModel = pointLightModel3;
    PointLight3.ambient = glm::vec3(2);
    PointLight3.specular = glm::vec3(2);
    PointLight3.diffuse = glm::vec3(2);



    Model* SpotLightModel7 = new Model();
    SpotLightModel7->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel7->transform.SetTranslation(glm::vec3(20, 9, 26));
    lightDebugModels.push_back(SpotLightModel7);

    Light SpotLight7;
    SpotLight7.lightType = LightType::SPOT_LIGHT;
    SpotLight7.lightModel = SpotLightModel7;
    SpotLight7.ambient = glm::vec3(5);
    SpotLight7.specular = glm::vec3(5);
    SpotLight7.diffuse = glm::vec3(5);
    SpotLight7.direction = glm::vec3(-1, 0, 0);
    SpotLight7.constant = 0.1f;
    SpotLight7.linear = 0.1f;
    SpotLight7.quadratic = 0.1f;
    SpotLight7.cutOffAngle = 12;
    SpotLight7.outerCutOffAngle = 15;
    SpotLight7.color = glm::vec3(0, 1, 0); // green color



    Model* SpotLightModel8 = new Model();
    SpotLightModel8->meshes = std::vector<Mesh>(directionLightModel->meshes.begin(), directionLightModel->meshes.end());
    SpotLightModel8->transform.SetTranslation(glm::vec3(20, 9, 26));
    lightDebugModels.push_back(SpotLightModel8);

    Light SpotLight8;
    SpotLight8.lightType = LightType::SPOT_LIGHT;
    SpotLight8.lightModel = SpotLightModel8;
    SpotLight8.ambient = glm::vec3(5);
    SpotLight8.specular = glm::vec3(5);
    SpotLight8.diffuse = glm::vec3(5);
    SpotLight8.direction = glm::vec3(1, 0, 0);
    SpotLight8.constant = 0.1f;
    SpotLight8.linear = 0.1f;
    SpotLight8.quadratic = 0.1f;
    SpotLight8.cutOffAngle = 12;
    SpotLight8.outerCutOffAngle = 15;
    SpotLight8.color = glm::vec3(1, 0, 0); // red color

    lightManager.AddNewLight(directionLight);
    lightManager.AddNewLight(SpotLight1);
    lightManager.AddNewLight(spotLight2);
    lightManager.AddNewLight(Spotlight3);
    lightManager.AddNewLight(SpotLight4);
    lightManager.AddNewLight(spotLight5);
    lightManager.AddNewLight(SpotLight6);
    lightManager.AddNewLight(pointLight1);
    lightManager.AddNewLight(PointLight2);
    lightManager.AddNewLight(PointLight3);
    lightManager.AddNewLight(SpotLight7);
    lightManager.AddNewLight(SpotLight8);

    //lightManager.AddNewLight(spotLight);
     lightManager.SetUniforms(defaultShader.ID);
#pragma endregion
    


    bool isWireFrame = false;

    double lastTime = glfwGetTime();

    float updatedXPos;
    float updatedYPos;
    float updatedZPos;
    char inputBufferX[256]; 
    char inputBufferY[256]; 
    char inputBufferZ[256]; 

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        processInput(window);

        AnimationModels(deltaTime); // animating the sliding door, "PRESS R to play"
        render.Clear();

        glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 _view = camera.GetViewMatrix();
        glm::mat4 _lightModel = glm::mat4(1.0f);


        lightSource.use();
        lightSource.setMat4("projection", _projection);
        lightSource.setMat4("view", _view);

        for (size_t i = 0; i < lightDebugModels.size(); i++)
        {
            lightDebugModels[i]->transform.scale =glm::vec3(0.5f); // setting scale of the light object sphere 
            lightDebugModels[i]->Draw(lightSource);
        }
        for (size_t i = 0; i < starsModels.size(); i++)
        {
            starsModels[i]->Draw(lightSource);
        }
    


        defaultShader.use();
        lightManager.UpdateUniformValues(defaultShader.ID);
        material.SetMaterialProperties(defaultShader);
         // view/projection transformations
         glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
         glm::mat4 view2 = camera.GetViewMatrix();
         defaultShader.setMat4("projection", projection2);
         defaultShader.setMat4("view", view2);
         //drawing all the models
         for (size_t i = 0; i < loadedModels.size(); i++)
         {
           
             loadedModels[i]->Draw(defaultShader);
         }
      

         if (isWireFrame)
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
         else
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         }
        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    glfwTerminate();
    return 0;


}


void LoadStarsModel()
{
   
    Model* stars = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", false, true);
    //lightDebugModels.push_back(directionLightModel);
    for (size_t i = 0; i < 100; i++)
    {
        int MinX = randomNumberGen(0, 50);
        int MaxY = randomNumberGen(0, 20);
        Model* starsCopy = new Model();
        starsCopy->meshes = std::vector<Mesh>(stars->meshes.begin(), stars->meshes.end());
        starsCopy->transform.SetTranslation(glm::vec3(MinX, MaxY,-15));
        starsCopy->transform.SetScale(glm::vec3(0.02f));
        starsModels.push_back(starsCopy);
    }
}

int randomNumberGen(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max - 1);
    int random_number = dis(gen);
    return random_number;
}
void AnimationModels(float deltaTime)
{
    if (isAnimationKeyPressed)
    {
        for (size_t i = 5; i < 10; i++) {
            if (animatingDoorModels[i]->transform.position.x >= 0)
            {
                animatingDoorModels[i]->transform.position -= glm::vec3(1, 0, 0) * deltaTime;
            }
            else 
            {
                isfirstDoorOpen = true;
            }
        }

        for (size_t i = 10; i < 15; i++) {
            if (animatingDoorModels[i]->transform.position.x <= 30) 
            {
                animatingDoorModels[i]->transform.position += glm::vec3(1, 0, 0) * deltaTime;
            }
            else {
                isfirstDoorOpen = true;
            }
        }

        if (isfirstDoorOpen)
        {
            for (size_t i = 0; i < 10; i++) 
            {
                if (animatingDoorModels[i]->transform.position.x < 0.0f) 
                { // Check if the door has closed
                    animatingDoorModels[i]->transform.position += glm::vec3(1, 0, 0) * deltaTime; // Reverse the direction for closing
                }
                else
                {
                    //isAnimationKeyPressed = false;
                }
            }

            for (size_t i = 10; i < 20; i++) {
                if (animatingDoorModels[i]->transform.position.x > 30)
                { // Check if the door has closed
                    animatingDoorModels[i]->transform.position -= glm::vec3(1, 0, 0) * deltaTime; // Reverse the direction for closing
                }
                else
                {
                    isfirstDoorOpen = false; // Reset the flag for closing
                }
            }
        }
    }
    else {
        for (size_t i = 5; i < 10; i++) {
            if (animatingDoorModels[i]->transform.position.x < 10) 
            { // Check if the door has closed
                animatingDoorModels[i]->transform.position += glm::vec3(1, 0, 0) * deltaTime; // Reverse the direction for closing
            }
            else 
            {
                isfirstDoorOpen = false; // Reset the flag for closing
            }
        }

        for (size_t i = 10; i < 15; i++)
        {
            if (animatingDoorModels[i]->transform.position.x > 20) { // Check if the door has closed
                animatingDoorModels[i]->transform.position -= glm::vec3(1, 0, 0) * deltaTime; // Reverse the direction for closing
            }
            else 
            {
                isfirstDoorOpen = false; // Reset the flag for closing
            }
        }
    }
}

void CheckingValues(Model* testModel, float x, float y, float z)
{
    if (isTestingModel)
    {
        testModel->transform.position = glm::vec3(x, y, z);

    }
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraMoveSpeed = 5.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {

        if (switchCamera)
        {
            float velocity = camera.MovementSpeed * deltaTime;
            currentLight += camera.Front * velocity;
        }
        else
        {
            camera.ProcessKeyboard(FORWARD, deltaTime* cameraMoveSpeed);
        }


    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {

        if (switchCamera)
        {
            float velocity = camera.MovementSpeed * deltaTime;
            currentLight -= camera.Front * velocity;
        }
        else
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime* cameraMoveSpeed);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (switchCamera)
        {
            float velocity = camera.MovementSpeed * deltaTime;
            currentLight -= camera.Right * velocity;
        }
        else
        {
            camera.ProcessKeyboard(LEFT, deltaTime* cameraMoveSpeed);

        }

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (switchCamera)
        {
            float velocity = camera.MovementSpeed * deltaTime;
            currentLight += camera.Right * velocity;

        }
        else
        {
            camera.ProcessKeyboard(RIGHT, deltaTime* cameraMoveSpeed);

        }

    }

    if (switchToPointLight)
    {
        PointLightPos = currentLight;
    }
    else
    {
        spotlightPos = currentLight;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}








