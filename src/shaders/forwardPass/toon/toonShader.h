#ifndef TOON_SHADER_H
#define TOON_SHADER_H

#include "../../../shader.h"
#include "../../../resourceManager.h"
#include "../../../entityModules/renderModule.h"
#include "outlineShader.h"


class ToonShader : public Shader {
public:
    ToonShader(const char* PVS, const char* PFS) {
        this->Compile(this->readShaderSource(PVS), this->readShaderSource(PFS));
        std::string vshaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toon/outlineShader.vert";
        std::string fshaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/toon/outlineShader.frag";
        outlineShader = new OutlineShader(vshaderPath.c_str(), fshaderPath.c_str());
    }

    void Render() override {        
        this->Use();

        // Load camera uniforms

        this->SetMatrix4("view", ResourceManager::getActiveCamera()->getViewMatrix());
        this->SetMatrix4("projection", ResourceManager::getActiveCamera()->getProjectionMatrix());

        if(pointLightsToRender[0] != nullptr){
            this->SetVector3f("light.position", pointLightsToRender[0]->getPosition());
            this->SetVector3f("light.ambient", pointLightsToRender[0]->getAmbient());
            this->SetVector3f("light.diffuse", pointLightsToRender[0]->getDiffuse());
            this->SetVector3f("light.specular", pointLightsToRender[0]->getSpecular());
        }

        for(RenderModule* module : objectsToRender){
            this->SetMatrix4("model", module->getParent()->getTransform());
            module->material->Draw(this);
            module->model->Draw(this);
        }

        outlineShader->Render();
    }

    void bindRenderModule(RenderModule* object) override{
        objectsToRender.push_back(object);
        outlineShader->bindRenderModule(object);
    }
    
private:
    Shader* outlineShader;

};

#endif // TOON_SHADER_H