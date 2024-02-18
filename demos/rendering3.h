#ifndef RENDERING1_H
#define RENDERING1_H

// Scene to showcase different shading techniques

#include "../src/resourceManager.h"
#include "../src/entityModules/controllerModule.h"
#include "../src/entityModules/renderModule.h"
#include "../src/entityModules/gameplayModule.h"
#include "../src/materials/tessMaterial.h"
#include "../src/materials/texturedMaterial.h"
#include "../src/shaders/forwardPass/textured/texturedShader.h"
#include "../src/shaders/forwardPass/tessalation/tessShader.h"
#include "../src/imgui/imguiWrapper.h"

void setUpScene(){

    std::string potPath = std::string(ASSET_DIR) + "/models/teapot.fbx";
    std::string spherePath = std::string(ASSET_DIR) + "/models/defaultSphere.fbx";
    std::string planePath = std::string(ASSET_DIR) + "/models/defaultPlane.fbx";

    std::string vShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.vert";
    std::string tcsShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.tesc";
    std::string tesShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.tese";
    std::string gShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.geom";
    std::string fShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/tessalation/tessShader.frag";

    std::string vPShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/textured/texturedShader.vert";
    std::string fPShaderPath = std::string(SRC_DIR) + "/shaders/forwardPass/textured/texturedShader.frag";

    std::string rock_diffuse_path = std::string(ASSET_DIR) + "/textures/materials/rock_cliff/albedo.jpg";
    std::string rock_normal_path = std::string(ASSET_DIR) + "/textures/materials/rock_cliff/normal.jpg";
    std::string rock_height_path = std::string(ASSET_DIR) + "/textures/materials/rock_cliff/displacement.jpg";

    std::string snow_diffuse_path = std::string(ASSET_DIR) + "/textures/materials/snow_pure/albedo.jpg";
    std::string snow_normal_path = std::string(ASSET_DIR) + "/textures/materials/snow_pure/normal.jpg";
    std::string snow_height_path = std::string(ASSET_DIR) + "/textures/materials/snow_pure/displacement.jpg";

    std::string wood_diffuse_path = std::string(ASSET_DIR) + "/textures/materials/wood_bark/albedo.jpg";
    std::string wood_normal_path = std::string(ASSET_DIR) + "/textures/materials/wood_bark/normal.jpg";
    std::string wood_height_path = std::string(ASSET_DIR) + "/textures/materials/wood_bark/displacement.jpg";

    DirectionalLight* directionalLight = ResourceManager::loadDirectionalLight(0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    PointLight* pointLight = ResourceManager::loadPointLight(0.1f, glm::vec3(-3.0f, 3.0f, -3.0f), 1.0f, 0.09f, 0.032f);

    TexturedShader* parallaxRockShader = new TexturedShader(vPShaderPath.c_str(), fPShaderPath.c_str());
    TexturedShader* parallaxSnowShader = new TexturedShader(vPShaderPath.c_str(), fPShaderPath.c_str());
    TexturedShader* parallaxWoodShader = new TexturedShader(vPShaderPath.c_str(), fPShaderPath.c_str()); 

    TessalationShader* rockShader = new TessalationShader(vShaderPath.c_str(), fShaderPath.c_str(), gShaderPath.c_str(), tcsShaderPath.c_str(), tesShaderPath.c_str());
    TessalationShader* snowShader = new TessalationShader(vShaderPath.c_str(), fShaderPath.c_str(), gShaderPath.c_str(), tcsShaderPath.c_str(), tesShaderPath.c_str());
    TessalationShader* woodShader = new TessalationShader(vShaderPath.c_str(), fShaderPath.c_str(), gShaderPath.c_str(), tcsShaderPath.c_str(), tesShaderPath.c_str());

    Texture* rockDiffuseTexture = ResourceManager::loadTexture(TextureType::DIFFUSE, rock_diffuse_path.c_str());
    Texture* rockNormalTexture = ResourceManager::loadTexture(TextureType::NORMAL, rock_normal_path.c_str());
    Texture* rockHeightTexture = ResourceManager::loadTexture(TextureType::HEIGHT, rock_height_path.c_str());

    Texture* snowDiffuseTexture = ResourceManager::loadTexture(TextureType::DIFFUSE, snow_diffuse_path.c_str());
    Texture* snowNormalTexture = ResourceManager::loadTexture(TextureType::NORMAL, snow_normal_path.c_str());
    Texture* snowHeightTexture = ResourceManager::loadTexture(TextureType::HEIGHT, snow_height_path.c_str());

    Texture* woodDiffuseTexture = ResourceManager::loadTexture(TextureType::DIFFUSE, wood_diffuse_path.c_str());
    Texture* woodNormalTexture = ResourceManager::loadTexture(TextureType::NORMAL, wood_normal_path.c_str());
    Texture* woodHeightTexture = ResourceManager::loadTexture(TextureType::HEIGHT, wood_height_path.c_str());

    rockShader->addTexture(rockDiffuseTexture);
    rockShader->addTexture(rockNormalTexture);
    rockShader->addTexture(rockHeightTexture);

    snowShader->addTexture(snowDiffuseTexture);
    snowShader->addTexture(snowNormalTexture);
    snowShader->addTexture(snowHeightTexture);

    woodShader->addTexture(woodDiffuseTexture);
    woodShader->addTexture(woodNormalTexture);
    woodShader->addTexture(woodHeightTexture);

    parallaxRockShader->addTexture(rockDiffuseTexture);
    parallaxRockShader->addTexture(rockNormalTexture);
    parallaxRockShader->addTexture(rockHeightTexture);

    parallaxSnowShader->addTexture(snowDiffuseTexture);
    parallaxSnowShader->addTexture(snowNormalTexture);
    parallaxSnowShader->addTexture(snowHeightTexture);

    parallaxWoodShader->addTexture(woodDiffuseTexture);
    parallaxWoodShader->addTexture(woodNormalTexture);
    parallaxWoodShader->addTexture(woodHeightTexture);


    ResourceManager::addShader(rockShader);
    ResourceManager::addShader(snowShader);
    ResourceManager::addShader(woodShader);
    ResourceManager::addShader(parallaxRockShader);
    ResourceManager::addShader(parallaxSnowShader);
    ResourceManager::addShader(parallaxWoodShader);

    TessalationMaterial *rockMaterial = new TessalationMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f, 10.0f, 5.0f);
    TessalationMaterial *snowMaterial = new TessalationMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f, 10.0f, 5.0f);
    TessalationMaterial *woodMaterial = new TessalationMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f, 10.0f, 5.0f);

    TexturedMaterial* parallaxRockMaterial = new TexturedMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f);
    TexturedMaterial* parallaxSnowMaterial = new TexturedMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f);
    TexturedMaterial* parallaxWoodMaterial = new TexturedMaterial(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.5f,0.5f,0.5f), glm::vec3(1.0f,1.0f,1.0f), 32.0f, 0.0f);

    Model* sphereModel = ResourceManager::loadModel(spherePath.c_str());
    Model* potModel = ResourceManager::loadModel(potPath.c_str());
    Model* surfaceModel = ResourceManager::loadModel(planePath.c_str());

    Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), Camera_Projection::PERSP, 45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    ResourceManager::setActiveCamera(camera);
    camera->setMode(Camera_Mode::FREE);

    GameObject* pot = ResourceManager::loadGameObject();
    RenderModule* potRenderModule = new RenderModule(potModel, rockMaterial, rockShader);
    GameplayModule* potGameplayModule = new GameplayModule();
    pot->addModule(potRenderModule);
    pot->addModule(potGameplayModule);
    pot->setPosition(glm::vec3(10.0f, -0.5f, 3.0f));

    GameObject* sphere = ResourceManager::loadGameObject();
    RenderModule* sphereRenderModule = new RenderModule(sphereModel, rockMaterial, rockShader);
    GameplayModule* sphereGameplayModule = new GameplayModule();
    sphere->addModule(sphereRenderModule);
    sphere->addModule(sphereGameplayModule);
    sphere->setPosition(glm::vec3(10.0f, 0.0f, 0.0f));

    GameObject* surface = ResourceManager::loadGameObject();
    RenderModule* surfaceRenderModule = new RenderModule(surfaceModel, parallaxRockMaterial, parallaxRockShader);
    surface->addModule(surfaceRenderModule);
    surface->setPosition(glm::vec3(10.0f, 0.0f, -3.0f));
    surface->setRotation(glm::vec3(0.0f, 270.0f, 0.0f));

    GameObject* pot2 = ResourceManager::loadGameObject();
    RenderModule* pot2RenderModule = new RenderModule(potModel, snowMaterial, snowShader);
    GameplayModule* pot2GameplayModule = new GameplayModule();
    pot2->addModule(pot2RenderModule);
    pot2->addModule(pot2GameplayModule);
    pot2->setPosition(glm::vec3(10.0f, 2.5f, 3.0f));

    GameObject* sphere2 = ResourceManager::loadGameObject();
    RenderModule* sphere2RenderModule = new RenderModule(sphereModel, snowMaterial, snowShader);
    GameplayModule* sphere2GameplayModule = new GameplayModule();
    sphere2->addModule(sphere2RenderModule);
    sphere2->addModule(sphere2GameplayModule);
    sphere2->setPosition(glm::vec3(10.0f, 3.0f, 0.0f));

    GameObject* surface2 = ResourceManager::loadGameObject();
    RenderModule* surface2RenderModule = new RenderModule(surfaceModel, parallaxSnowMaterial, parallaxSnowShader);
    surface2->addModule(surface2RenderModule);
    surface2->setPosition(glm::vec3(10.0f, 3.0f, -3.0f));
    surface2->setRotation(glm::vec3(0.0f, 270.0f, 0.0f));

    GameObject* pot3 = ResourceManager::loadGameObject();
    RenderModule* pot3RenderModule = new RenderModule(potModel, woodMaterial, woodShader);
    GameplayModule* pot3GameplayModule = new GameplayModule();
    pot3->addModule(pot3RenderModule);
    pot3->addModule(pot3GameplayModule);
    pot3->setPosition(glm::vec3(10.0f, -3.5f, 3.0f));

    GameObject* sphere3 = ResourceManager::loadGameObject();
    RenderModule* sphere3RenderModule = new RenderModule(sphereModel, woodMaterial, woodShader);
    GameplayModule* sphere3GameplayModule = new GameplayModule();
    sphere3->addModule(sphere3RenderModule);
    sphere3->addModule(sphere3GameplayModule);
    sphere3->setPosition(glm::vec3(10.0f, -3.0f, 0.0f));

    GameObject* surface3 = ResourceManager::loadGameObject();
    RenderModule* surface3RenderModule = new RenderModule(surfaceModel, parallaxWoodMaterial, parallaxWoodShader);
    surface3->addModule(surface3RenderModule);
    surface3->setPosition(glm::vec3(10.0f, -3.0f, -3.0f));
    surface3->setRotation(glm::vec3(0.0f, 270.0f, 0.0f));

    camera->setTarget(sphere);
    
    ImGuiWrapper::attachGuiFunction("Rock Properties", [rockMaterial](){rockMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Snow Properties", [snowMaterial](){snowMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Wood Properties", [woodMaterial](){woodMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Parallax Rock Properties", [parallaxRockMaterial](){parallaxRockMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Parallax Snow Properties", [parallaxSnowMaterial](){parallaxSnowMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Parallax Wood Properties", [parallaxWoodMaterial](){parallaxWoodMaterial->OnGui();});
    ImGuiWrapper::attachGuiFunction("Point Light", [pointLight](){pointLight->OnGui();});
    
}

#endif // RENDERING1_H