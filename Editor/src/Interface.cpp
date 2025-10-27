#include "Interface.h"
#include "imgui_internal.h"
#include "DockLayout.h"
#include "Themes.h"

#include "MenuBar/MenuBar.h"
#include "Console/ConsolePanel.h"
#include "Tree/TreePanel.h"
#include "Inspector/InspectorPanel.h"
#include "GamePanel/GamePanel.h"

#include "SceneManager.h"
#include "GameObject.h"
#include "GameObject.inl"
#include "ComponentRegistry.h"

#include "Components/Transform.h"

static DockLayout layout;

static SceneManager& sceneManager = SceneManager::GetInstance();

static bool firstTime = true;

class Jugador : public Component {
public:
    int vida = 100;

    Jugador(GameObject* owner) : Component(owner) {
        SERIALIZED_FIELD(vida);
    }

    const std::string& GetName() const override {
        static const std::string name = "Jugador";
        return name; 
    }
};

void ShowInterface() {
    if (firstTime) {
        Themes::LoadGlobalStyles();
        Themes::ApplyPreferredTheme();

        SceneManager& sceneManager = SceneManager::GetInstance();
        Scene& scene = sceneManager.CreateScene("Escena 1");
        GameObject& cubo1 = scene.CreateGameObject("Cubo 1");
        GameObject& cubo2 = scene.CreateGameObject("Cubo 2");
        GameObject& cubo3 = scene.CreateGameObject("Cubo 3");

        cubo3.AddComponent<Transform>();

        cubo2.AddChild(cubo1);
        cubo3.AddChild(cubo1);

        ComponentRegistry::Register("Jugador", [](GameObject* owner) {
            return std::make_unique<Jugador>(owner);
        });

        std::unique_ptr<Component> componenteJugador = ComponentRegistry::Create("Jugador", &cubo3);
        cubo3.AddComponent(std::move(componenteJugador));

        ComponentRegistry::LoadLibrary("build/Components/libComponents.so");

        firstTime = false;
    }

    SplitList splitList{
        { "root", ImGuiDir_Right, "###Inspector", "Left", 0.25 },
        { "Left", ImGuiDir_Down, "###Console", "Up", 0.25 },
        { "Up", ImGuiDir_Left, "###Tree", "###Game", 0.3 },
    };
    layout.Setup(splitList);

    MenuBar::Render();
    ConsolePanel::Render();
    TreePanel::Render(sceneManager.GetScenes());
    InspectorPanel::Render();
    GamePanel::Render();
}
