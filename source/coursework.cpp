#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/maths.hpp>
#include <common/camera.hpp>
#include <common/model.hpp>
#include <common/light.hpp>
#include "common/maths.hpp"

struct LightSource
{
    glm::vec3 position;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    float focalStrength;
    float specularIntensity;
};

LightSource Source[4];

float deltaTime = 1.0f;
Camera camera(Vec3(0.0f, 140.0f, 340.5f));
int CameraType  = 0;
bool ToggleLight1 = true;
bool ToggleLight2 = true;

// Function prototypes
void keyboardInput(GLFWwindow *window);
int GetuniformLocation(unsigned int Program, const char* name);
void setvec3(const char* name, glm::vec3 Data, unsigned int Program);

void setFloat(const char* name, float Data, unsigned int Program);

int main( void )
{
    // =========================================================================
    // Window creation - you shouldn't need to change this code
    // -------------------------------------------------------------------------
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Computer Graphics Coursework", NULL, NULL);
    
    if( window == NULL ){
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // -------------------------------------------------------------------------
    // End of window creation
    // =========================================================================
    
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)[](GLFWwindow* window, int button, int action, int mod) 
    {
        if (button == GLFW_MOUSE_BUTTON_1)
        {
            if (action == GLFW_PRESS)
            {
                ToggleLight1 = !ToggleLight1;
            }
        }

        if (button == GLFW_MOUSE_BUTTON_2)
        {
            if (action == GLFW_PRESS)
            {
                ToggleLight2 = !ToggleLight2;
            }
        }
    });


    glfwSetKeyCallback(window, (GLFWkeyfun)[](GLFWwindow* window, int button, int scancode, int action, int mod)
    {
        if (button == GLFW_KEY_3)
        {
            if (action == GLFW_PRESS)
            {
                CameraType++;

                if (CameraType >= 3)
                {
                    CameraType = 0;
                }
            }
        }


    });

    // Ensure we can capture keyboard inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    //shader setup
    unsigned int Program  =  LoadShaders("VertexShader.glsl", "fragmentShader.glsl");
    glUseProgram(Program);

    Mat4 ProjectionMatrix = PerspectiveFov(45.0f, 16.0f / 9.0f, 0.1f, 10000.0f);
    glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(500, 30, 500));
    glm::mat4 view = glm::mat4(1.0f);



    //Model Cube = Model("")
    glm::mat4 PlaneModels = ModelMatrix;
    Model model = Model("../assets/Cube.obj");
    model.addTexture("../assets/floor.jpg", "diffuse");
    model.addTexture("../assets/floor_normal.jpg", "normal");
    model.addTexture("../assets/floor_spec.jpg", "spec");

    Model StoneAltar = Model("../assets/stonealtar.obj");
    StoneAltar.addTexture("../assets/altar.jpg", "diffuse");


    Model bowlingPin = Model("../assets/bowling_pin.obj");
    bowlingPin.addTexture("../assets/tile_floor.jpg", "diffuse");


    Model Crate = Model("../assets/crate.obj");
    Crate.addTexture("../assets/crate.jpg", "diffuse");

    Model Barrel = Model("../assets/barrel.obj");
    Crate.addTexture("../assets/crate.jpg", "diffuse");


    // Position and Color data
    GLfloat verts[] = {
        // Position            // Normal             // TexCoords   // Tangent

        // Back Face (-Z)
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,   -1.0f, 0.0f, 0.0f,

        // Bottom Face (-Y)
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,    1.0f, 0.0f, 0.0f,

         // Left Face (-X)
         -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,    0.0f, 0.0f, 1.0f,

         // Right Face (+X)
          0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
          0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
          0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,    0.0f, 0.0f, -1.0f,

          // Top Face (+Y)
          -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
           0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
           0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f,    1.0f, 0.0f, 0.0f,

           // Front Face (+Z)
           -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,
           -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,    1.0f, 0.0f, 0.0f,
    };


    // Index data
    GLuint indices[] = {
        0,1,2,
        0,3,2,
        4,5,6,
        4,7,6,
        8,9,10,
        8,11,10,
        12,13,14,
        12,15,14,
        16,17,18,
        16,19,18,
        20,21,22,
        20,23,22
    };

    unsigned int cubeVAO, VBO[2];

    unsigned int nVertices = sizeof(verts) / (sizeof(verts[0]) * (3 + 3 + 2 + 3));
    unsigned int nIndices = sizeof(indices) / sizeof(indices[0]);

    glGenVertexArrays(1, &cubeVAO); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(cubeVAO);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]); // Activates the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);





    //light 1
    Source[0].ambientColor = glm::vec3(0.1, 0.1, 0.1);
    Source[0].diffuseColor = glm::vec3(0.6, 0.6, 0.6);
    Source[0].specularColor = glm::vec3(0.9, 0.9, 0.9);
    Source[0].position = glm::vec3(1000, 100.9, 1000.0);
    Source[0].focalStrength = 32;
    Source[0].specularIntensity = 1.0f;

    //light 2
    Source[1].ambientColor = glm::vec3(0.1);
    Source[1].diffuseColor = glm::vec3(0.6);
    Source[1].specularColor = glm::vec3(0.9);
    Source[1].position = glm::vec3(1000, 100.9, -1000.0);
    Source[1].focalStrength = 32;
    Source[1].specularIntensity = 1.0f;

   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glUniformMatrix4fv(glGetUniformLocation(Program, "projection"), 1, false, ProjectionMatrix.data());
    int modelLoc = glGetUniformLocation(Program, "model");
    int viewLoc = glGetUniformLocation(Program, "view");
    int ViewPositionloc = glGetUniformLocation(Program, "viewPosition");
    int toggleLight1loc = glGetUniformLocation(Program, "toggleLight1");
    int toggleLight2loc = glGetUniformLocation(Program, "toggleLight2");
    int NomaANdSpecLoc = glGetUniformLocation(Program, "bUseNormAndSpec");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Get inputs
        keyboardInput(window);
        
        // Clear the window
        glClearColor(0.2f, 0.2f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render Cube
        ModelMatrix = PlaneModels;
        glUniformMatrix4fv(modelLoc, 1, false, &ModelMatrix[0][0]);

        if (CameraType == 0)
        {
            glUniformMatrix4fv(viewLoc, 1, false, camera.GetViewMatrixCustonm().data());
        }


        if (CameraType == 1)
        {
            glUniformMatrix4fv(viewLoc, 1, false, camera.GetViewMatrixQuat().data());
        }

        if (CameraType == 2)
        {
            glUniformMatrix4fv(viewLoc, 1, false, camera.GetViewMatrixQuat().data());
        }

        glUniform3fv(ViewPositionloc, 1, &camera.Position.x);


        for (int i = 0; i < 2; i++)
        {
            glUniform1i(toggleLight1loc, ToggleLight1);
            glUniform1i(toggleLight2loc, ToggleLight2);



            std::string IndexStringRepresentation = std::to_string(i);
            std::string ConcatWithClassAccess = "lightSources[" + IndexStringRepresentation + "]";

            setvec3((ConcatWithClassAccess + ".position").c_str(), Source[i].position, Program);
            setvec3((ConcatWithClassAccess + ".ambientColor").c_str(), Source[i].ambientColor, Program);
            setvec3((ConcatWithClassAccess + ".diffuseColor").c_str(), Source[i].diffuseColor, Program);
            setvec3((ConcatWithClassAccess + ".specularColor").c_str(), Source[i].specularColor, Program);
        }

        
        {
            glUniform1i(NomaANdSpecLoc, true);


            //use The Texture Of this to Render the Below Object
            model.draw(Program, false);

            glBindVertexArray(cubeVAO);
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, (void*)0);
            glBindVertexArray(0);

            glUniform1i(NomaANdSpecLoc, false);

        }


        //Draw Other Models
        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 20.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(70.5f));
        glUniformMatrix4fv(modelLoc, 1, false, &ModelMatrix[0][0]);

        StoneAltar.draw(Program, true);


        //bowling Pin
        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 80.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(3.5f));
        glUniformMatrix4fv(modelLoc, 1, false, &ModelMatrix[0][0]);

        bowlingPin.draw(Program, true);

        //Crate
        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 20.0f, 7.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(25.5f));
        glUniformMatrix4fv(modelLoc, 1, false, &ModelMatrix[0][0]);

        Crate.draw(Program, true);

        //barrel
        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 20.0f, 100.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(30.5f));
        glUniformMatrix4fv(modelLoc, 1, false, &ModelMatrix[0][0]);

        Barrel.draw(Program, true);


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}


double KeyboardCursor_x = 5.0f, keyboard_cursor_y = 5.0f;

void keyboardInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    float speed = 5.125f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * speed);

    //glfwGetCursorPos(window, &KeyboardCursor_x, &keyboard_cursor_y);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        //	KeyboardCursor_x -= 0.01f;
        camera.ProcessMouseMovement(-KeyboardCursor_x, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        //KeyboardCursor_x += 0.01f;
        camera.ProcessMouseMovement(KeyboardCursor_x, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        //keyboard_cursor_y += 0.01f;
        camera.ProcessMouseMovement(0.0f, keyboard_cursor_y);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        //keyboard_cursor_y -= 0.01f;
        camera.ProcessMouseMovement(0.0f, -keyboard_cursor_y);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        ToggleLight1 = !ToggleLight1;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        ToggleLight2 = !ToggleLight2;
    }


}


int GetuniformLocation(unsigned int Program, const char* name)
{
    return glGetUniformLocation(Program, name);
}

void setvec3(const char* name, glm::vec3 Data, unsigned int Program)
{
    int loc = GetuniformLocation(Program, name);
    glUniform3f(loc, Data.x, Data.y, Data.z);
}


void setFloat(const char* name, float Data, unsigned int Program)
{
    int loc = GetuniformLocation(Program, name);
    glUniform1f(loc, Data);
}