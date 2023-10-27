


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



/*TIPS and notes

* Shaders does not need to be stored in stack or heap as they are mostly read only and used in compile time. what you can do is
  link the shaders and once its job is done in the graphics card you should delete the shaders as its not necessary anymore

* Always abstract classes properly to maintain proper code.

* Use uniforms in the shader whenever you want to change the values for the shader like moving the triangle etc
*
* Always check if you binded every vertex array correctly that you created, remember the day you tried to make a diffuse light.
*
*
*



*/
bool switchCamera = false;
bool switchToPointLight = false;

glm::vec3 currentLight(1.0f, 0.0f, 1.0f);
glm::vec3 spotlightPos(1.0f, 0.0f, 1.0f);
glm::vec3 PointLightPos(-1.0f, 0.0f, 1.0f);

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
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {

    }

}

const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 2000;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

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


    Model* backpack = new Model((char*)"Models/Backpack/backpack.obj", false,true);
    Model* LightSphere = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", false, true);
    Model* pointLightModel= new Model();
   
    pointLightModel->meshes = std::vector<Mesh>(LightSphere->meshes.begin(), LightSphere->meshes.end());

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
    //Light directionLight;
    //directionLight.lightType = LightType:: DIRECTION_LIGHT;
    //directionLight.lightModel = LightSphere;


  /*  Light PointLight;
    PointLight.lightType = LightType::POINT_LIGHT;
    PointLight.lightModel = pointLightModel;
    PointLight.ambient = glm:: vec3(5);
    PointLight.specular = glm:: vec3(5);
    PointLight.diffuse = glm:: vec3(5);*/

    Light spotLight;
    spotLight.lightType = LightType::SPOT_LIGHT;
    spotLight.lightModel = pointLightModel;
    spotLight.direction = glm::vec3(0, 0, -1);
    spotLight.ambient = glm::vec3(5);
    spotLight.specular = glm::vec3(5);
    spotLight.diffuse = glm::vec3(5); 
    spotLight.cutOffAngle = 15;
    spotLight.outerCutOffAngle = 15;




    //lightManager.AddNewLight(directionLight);
    //lightManager.AddNewLight(PointLight);
    lightManager.AddNewLight(spotLight);
     lightManager.SetUniforms(defaultShader.ID);

    
    unsigned int VBO, lightCubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  


    float lightSize = 0.5f;
    float xPos = 0.0f;
    float yPos = 0.0f;
    float zPos = 0.0f;

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

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

      
        processInput(window);

    
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
            ImGui::SliderFloat("##X Position", &xPos, -10.0f, 10.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Position", &yPos, -10.0f, 10.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Position", &zPos, -10.0f, 10.0f, "Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("POSITION");

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
        pointLightModel->transform.position = lightPos;
        pointLightModel->Draw(lightSource);
    

      
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
         backpack->transform.position = glm::vec3(0);
         backpack->Draw(defaultShader);


     



       
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






void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {

        if (switchCamera)
        {
            float velocity = camera.MovementSpeed * deltaTime;
            currentLight += camera.Front * velocity;
        }
        else
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
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
            camera.ProcessKeyboard(BACKWARD, deltaTime);
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
            camera.ProcessKeyboard(LEFT, deltaTime);

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
            camera.ProcessKeyboard(RIGHT, deltaTime);

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








