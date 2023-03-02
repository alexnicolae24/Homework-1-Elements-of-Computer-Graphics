#pragma once

#include "components/simple_scene.h"
#include "utils/glm_utils.h"
#include <string>
#include <cstdlib>
#include "core/gpu/mesh.h"



namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        float CalculateRandom();
        void CreateDuck();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        glm::ivec2 resolution;
        glm::mat3 modelMatrix,modelMatrix1;
        float translateX, translateY;
        float angleWings, angleDuck;
        int nrLives = 3;
        int nrBullets = 3;
        int nrScorebars = 0;
        int nrDucks = 1;
        int deltaD1,deltaD2,deltaD3;
        float randFactor1;
        float gameTime = 0;
        float speedFactor = 1;
        bool dir1 = false ,dir2 = false ,dead = false ,released = false;
        //float mx = 0, my = 0;
    };
}   // namespace m1

namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::mat3(1, 0, 0,
                         0, 1, 0,
                         translateX, translateY, 1);

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::mat3(scaleX, 0, 0,
                         0, scaleY, 0,
                         0, 0, 1);

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::mat3(cos(radians), sin(radians), 0,
                        -sin(radians), cos(radians), 0,
                         0, 0, 1);

    }
}   // namespace transform2D

namespace objects
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string& name, glm::vec3 circleOrigin,float length, glm::vec3 color, bool fill = false);
}
