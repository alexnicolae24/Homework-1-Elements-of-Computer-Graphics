#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;
using namespace transform2D;
using namespace objects;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;
    float circleSide = 45;


    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize angleWings
    angleWings = 0;
    
    //Calculation of a random angle 
    angleDuck = CalculateRandom();

    //Creation of all meshes
    Mesh* grass = objects::CreateSquare("grass", corner, 100, glm::vec3(0, 1, 0), true);
    AddMeshToList(grass);

    Mesh* body = objects::CreateTriangle("body", corner, squareSide, glm::vec3(1,0, 0), true);
    AddMeshToList(body);

    Mesh* wing1 = objects::CreateTriangle("wing1", corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(wing1);

    Mesh* wing2 = objects::CreateTriangle("wing2",corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(wing2);

    Mesh* head = objects::CreateCircle("head", corner,circleSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(head);

    Mesh* beak = objects::CreateTriangle("beak", corner, 100, glm::vec3(1, 0, 0), true);
    AddMeshToList(beak);

    Mesh* life = objects::CreateCircle("life", corner,10, glm::vec3(1, 0, 0), true);
    AddMeshToList(life);

    Mesh* bullet = objects::CreateSquare("bullet", corner, 10, glm::vec3(0, 1, 0), false);
    AddMeshToList(bullet);

    Mesh* score = objects::CreateSquare("score", corner, 10, glm::vec3(0.502, 0.502, 0.502), false);
    AddMeshToList(score);

    Mesh* score_bar = objects::CreateSquare("score_bar", corner, 10, glm::vec3(0, 0, 1), true);
    AddMeshToList(score_bar);

    Mesh* hitBox = objects::CreateSquare("hitBox", corner, 30, glm::vec3(0, 0, 0), false);
    AddMeshToList(hitBox);

}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

float Tema1::CalculateRandom() {
    randFactor1 = 0.15  + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - 0.15)));
    std::cout << randFactor1 << " ";
    return randFactor1;
}

void Tema1::Update(float deltaTimeSeconds)
{
    resolution = window->GetResolution();

    //Rendering the grass
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(200, 1.5);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    //Condition for wings rotation
    angleWings += deltaTimeSeconds;
    if (cos(angleWings) <= 0 && sin(angleWings) >= 0)
        angleWings = 0;

    //Conditions for the entire game
    modelMatrix = glm::mat3(1);
    gameTime += deltaTimeSeconds;
    if (dead == true) {
        angleDuck = -M_PI / 2;
        translateY -= deltaTimeSeconds * 500;
        nrScorebars++;
        dead = false;       
    }
    else {
        if (gameTime <= 10) {
            if (dir1 == false) {
                translateY += deltaTimeSeconds * 200 * sin(angleDuck) * speedFactor;
                if (translateY > resolution.y - 50) {
                    angleDuck = -angleDuck;
                    translateY = resolution.y - 50;
                    dir1 = true;
                }
            }
            else if (dir1 == true) {
                translateY += deltaTimeSeconds * 200 * sin(angleDuck) * speedFactor;
                if (translateY < 0) {
                    angleDuck = -angleDuck;
                    translateY = 0;
                    dir1 = false;
                }
            }

            if (dir2 == false) {
                translateX += deltaTimeSeconds * 200 * cos(angleDuck) * speedFactor;
                if (translateX > resolution.x) {
                    translateX = resolution.x;
                    angleDuck = M_PI - angleDuck;
                    dir2 = true;
                }
            }
            else if (dir2) {
                translateX += deltaTimeSeconds * 200 * cos(angleDuck) * speedFactor;
                if (translateX < 100) {
                    translateX = 100;
                    angleDuck = M_PI - angleDuck;
                    dir2 = false;
                }
            }
        }
        else {
            angleDuck = M_PI / 2;
            translateY += deltaTimeSeconds * 500;
            released = false;
        }
    }

    //Conditions for initialization
   if (translateY > resolution.y + 200) {
        nrLives--;
        released = true;
    }

    if (translateY < -200)
        dead = true;

    if (dead == true || released == true) {
        angleDuck = CalculateRandom();
        translateX = 0;
        translateY = 0;
        dead = false;
        released = false;
        gameTime = 0;
        nrDucks++;
        speedFactor = speedFactor * (nrDucks/2);
        nrBullets = 3;
    }
 
    //Condition for quitting the game
    if (nrLives == 0)
        exit(0); 

    //Rendering the whole duck
    modelMatrix *=
        transform2D::Translate(translateX, translateY) *
        transform2D::Rotate(angleDuck) *
        transform2D::Scale(0.5, 0.5);

    CreateDuck();

    //Rendering the lives
    modelMatrix1 = glm::mat3(1);
    deltaD1 = 0;
    for (int i = 0; i < nrLives; i++) {
         modelMatrix1 *= transform2D::Translate(1000 + deltaD1, 700);
         RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix1);
         deltaD1 += 50;
         modelMatrix1 = glm::mat3(1);
    }

    //Rendering the bullets
    modelMatrix1 = glm::mat3(1);
    deltaD2 = 0;
    for (int i = 0; i < nrBullets; i++) {
         modelMatrix1 *= transform2D::Translate(1150 + deltaD2, 690) * transform2D::Scale(1.5, 2);
         RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix1);
         deltaD2 += 50;
         modelMatrix1 = glm::mat3(1);
    }

    //Rendering the score
    modelMatrix1 = glm::mat3(1);
    modelMatrix1 *= transform2D::Translate(750, 650) * transform2D::Scale(50, 2.5);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix1);
     
    //Rendering the score bars
    modelMatrix1 = glm::mat3(1);
    deltaD3 = 0;
    for (int i = 0; i < nrScorebars && nrScorebars <= 33; i++ ) {
         modelMatrix1 *= transform2D::Translate(750 + deltaD3, 650) * transform2D::Scale(2, 2.4);
            RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix1);
            deltaD3 += 15;
            modelMatrix1 = glm::mat3(1);
    }
      
}

void Tema1::CreateDuck() {
    RenderMesh2D(meshes["hitBox"], shaders["VertexColor"], modelMatrix * transform2D::Scale(12.7, 5));

    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix  
        * transform2D::Scale(5, 1.5));
    
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix * transform2D::Translate(130, 110) 
        * transform2D::Rotate(M_PI/2) * transform2D::Rotate(angleWings)
        * transform2D::Scale(2.75, 0.5));

    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelMatrix * transform2D::Translate(100, 85)   
       * transform2D::Rotate(-M_PI/2) * transform2D::Rotate(-angleWings)
        * transform2D::Scale(2.75, 0.5));

    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix * transform2D::Translate(295, 75));

    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix * transform2D::Translate(345, 55) 
        * transform2D::Scale(0.75, 0.35));

}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    nrBullets--;

    //Condition for shooting the duck
    if (mouseX >= translateX - 50 && mouseX <  translateX + 50 && 
        resolution.y - mouseY > translateY - 50 && mouseY - resolution.y < translateY + 50){
        cout << "ok";
        nrScorebars++;
        dead = true;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
