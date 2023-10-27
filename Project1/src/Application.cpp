


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
        isAnimationKeyPressed = true;
    }

}


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;


Camera camera(glm::vec3(5.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<Model*> loadedModels;
std::vector<Model*> lightDebugModels;
std::vector<Model*> animatingDoorModels;
Model* testingModel;
bool isTestingModel = false;
void CheckingValues(Model* testModel, float x, float y, float z);
bool isfirstDoorOpen = false;
void AnimationModels(float deltaTime);
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



   // Shader Shadersrc("Shaders/Shader.vert", "Shaders/Shader.frag");
    Shader defaultShader("Shaders/Light_VertexShader.vert", "Shaders/Light_FragmentShader.frag");


    Shader lightSource("Shaders/lighting.vert", "Shaders/lighting.frag");



#pragma region ModelLoading


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

#pragma endregion




    //Light models
    Model* LightSphere = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", false, true);

    Model* pointLightModel = new Model();
    pointLightModel = &(*LightSphere);

    lightDebugModels.push_back(LightSphere);
    lightDebugModels.push_back(pointLightModel);
    




#pragma endregion
    Material material( 128.0f);



    float vertices[] = {
        // positions          // normals           // texture coords
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };






    Renderer render;
  
    LightManager lightManager;
    Light directionLight;
    directionLight.lightType = LightType:: DIRECTION_LIGHT;
    directionLight.lightModel = LightSphere;


    Light PointLight;
    PointLight.lightType = LightType::POINT_LIGHT;
    PointLight.lightModel = pointLightModel;
    PointLight.ambient = glm:: vec3(5);
    PointLight.specular = glm:: vec3(5);
    PointLight.diffuse = glm:: vec3(5);

   /* Light spotLight;
    spotLight.lightType = LightType::SPOT_LIGHT;
    spotLight.lightModel = pointLightModel;
    spotLight.direction = glm::vec3(0, 0, -1);
    spotLight.ambient = glm::vec3(5);
    spotLight.specular = glm::vec3(5);
    spotLight.diffuse = glm::vec3(5); 
    spotLight.cutOffAngle = 15;
    spotLight.outerCutOffAngle = 15;*/




    lightManager.AddNewLight(directionLight);
    lightManager.AddNewLight(PointLight);
    //lightManager.AddNewLight(spotLight);
     lightManager.SetUniforms(defaultShader.ID);

    
    unsigned int VBO, lightCubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  
    float xPos = 0.0f;
    float yPos = 0.0f;
    float zPos = 0.0f;

    float lightSize = 0.5f;

    float lightX = 0.0f;
    float lightY = 0.3f;
    float lightZ = 10.0f;

    float rotX = 1.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    float color[3] = { 1.0f, 1.0f, 1.0f };
    bool isWireFrame = false;

 
    float yAxis = 10;

    glm::vec3 collisionPosition = glm::vec3(0.0f);
    

    double lastTime = glfwGetTime();



    float updatedXPos;
    float updatedYPos;
    float updatedZPos;
    char inputBufferX[256]; // Assuming a buffer size of 256 characters
    char inputBufferY[256]; // Assuming a buffer size of 256 characters
    char inputBufferZ[256]; // Assuming a buffer size of 256 characters

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

      
        processInput(window);

        AnimationModels(deltaTime);
        render.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        {

            static float f = 0.0f;
            static int counter = 0;


            //add a Title to your GUI layout
            ImGui::Begin("Media Player Lite!");
            // ImGui::SetWindowFontScale(2.0f);
            ImGui::SetWindowSize(ImVec2(800, 800));

            //add a intro text
            ImGui::Text("KAIZOKU ENGINE");

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Position", &xPos, -10, 10, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 20);
            ImGui::SliderFloat("##Y Position", &yPos, -10, 10, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 20);
            ImGui::SliderFloat("##Z Position", &zPos, -10, 10, "Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 20); ImGui::Text("POSITION");



            snprintf(inputBufferX, sizeof(inputBufferX), "%.1f", xPos);
            ImGui::InputText("##X Position Input", inputBufferX, IM_ARRAYSIZE(inputBufferX));
            xPos = atof(inputBufferX);

            snprintf(inputBufferY, sizeof(inputBufferY), "%.1f", yPos);
            ImGui::InputText("##Y Position Input", inputBufferY, IM_ARRAYSIZE(inputBufferY));
            yPos = atof(inputBufferY);

            snprintf(inputBufferZ, sizeof(inputBufferZ), "%.1f", zPos);
            ImGui::InputText("##Z Position Input", inputBufferZ, IM_ARRAYSIZE(inputBufferZ));
            zPos = atof(inputBufferZ);


            CheckingValues(testingModel, xPos, yPos, zPos);

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Pos", &lightX, -10.0f, 10.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Pos", &lightY, -10.0f, 10.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Pos", &lightZ, -100.0f, 10.0f,"Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("LIGHT POSITION");

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Rot", &rotX, 0.0f, 1.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Rot", &rotY, 0.0f, 1.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Rot", &rotZ, 0.0f, 1.0f, "Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("OBJECT POSITION");



            ImGui::NewLine();
            ImGui::PushItemWidth(150);
            ImGui::SliderFloat("SCALE", &lightSize, 0.0f, 10.0f);

            ImGui::NewLine();
            ImGui::PushItemWidth(400);
            ImGui::ColorEdit3("LIGHT COLOR", color);

            ImGui::NewLine();
            ImGui::Checkbox("WIREFRAME", &isWireFrame);
            // ImGui::Checkbox("Loop Audio", );

            // ImGui::ColorPicker3("LightColor", 1.0f, 1.0f, 1.0f);

           // constructionBlock->transform.position =(glm::vec3(xPos, yPos, zPos));



             //framerate
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

        }


        glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

     //  Light light(lightPos, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
     //      glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), camera.Position);


        

        // view/projection transformations
        glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 _view = camera.GetViewMatrix();
        glm::mat4 _lightModel = glm::mat4(1.0f);







        lightSource.use();
        lightSource.setMat4("projection", _projection);
        lightSource.setVec3("objCol", color[0], color[1], color[2]);
        lightSource.setMat4("view", _view);

        for (size_t i = 0; i < lightDebugModels.size(); i++)
        {
            lightDebugModels[i]->Draw(lightSource);
        }
        //pointLightModel->transform.position = lightPos;
       
    

      
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

       
        defaultShader.use();
        lightManager.UpdateUniformValues(defaultShader.ID);
        material.SetMaterialProperties(defaultShader);
         // view/projection transformations
         glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
         glm::mat4 view2 = camera.GetViewMatrix();
         defaultShader.setMat4("projection", projection2);
         defaultShader.setMat4("view", view2);
        // backpack->transform.position = glm::vec3(0);
        // backpack->Draw(defaultShader);
         for (size_t i = 0; i < loadedModels.size(); i++)
         {
           
             loadedModels[i]->Draw(defaultShader);
         }

     



       
         //light.SetLightProperties(Shadersrc);
         defaultShader.setVec3("objPos", xPos, yPos, zPos);

         if (isWireFrame)
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
         else
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


         }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    //glDeleteVertexArrays(1, &cubeVAO);
     glDeleteVertexArrays(1, &lightCubeVAO);


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;


}




void AnimationModels(float deltaTime)
{
    if (isAnimationKeyPressed)
    {
        for (size_t i = 5; i < 10; i++)
        {
            if (animatingDoorModels[i]->transform.position.x>=0)
            {
                animatingDoorModels[i]->transform.position -= glm::vec3(1, 0, 0)*deltaTime;

            }
            else
            {
                isfirstDoorOpen = true;
            }
        }

        for (size_t i = 10; i < 15; i++)
        {
            if (animatingDoorModels[i]->transform.position.x <= 30)
            {
                animatingDoorModels[i]->transform.position += glm::vec3(1, 0, 0) * deltaTime;

            }
            else
            {
                isfirstDoorOpen = true;
            }
        }

        if (isfirstDoorOpen)
        {
            for (size_t i = 0; i < 10; i++)
            {
                if (animatingDoorModels[i]->transform.position.x >= -10.0f)
                {
                    animatingDoorModels[i]->transform.position -= glm::vec3(1, 0, 0) * deltaTime;

                }
                else
                {
                    isAnimationKeyPressed = false;
                }
            }

            for (size_t i = 10; i < 20; i++)
            {
                if (animatingDoorModels[i]->transform.position.x <= 40)
                {
                    animatingDoorModels[i]->transform.position += glm::vec3(1, 0, 0) * deltaTime;

                }
                else
                {
                    isfirstDoorOpen = true;
                }
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








