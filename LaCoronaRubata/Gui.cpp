#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Config.h"
#include "Gui.h"
#include "GameFlow.h"
#include <algorithm>

namespace {

const ImVec4 gold{0.91f, 0.73f, 0.45f, 1.0f};

/// Posiziona un pannello lungo l'asse centrale, adattandolo alla larghezza della finestra.
bool beginPanel(const char* name, float maxWidth, float positionY, float anchorY,
                ImGuiWindowFlags extraFlags = 0)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 pos{viewport->WorkPos.x + viewport->WorkSize.x * 0.5f,
                    viewport->WorkPos.y + viewport->WorkSize.y * positionY};
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, {0.5f, anchorY});
    ImGui::SetNextWindowSize({std::min(maxWidth, viewport->WorkSize.x * 0.92f), 0.0f});
    ImGui::SetNextWindowBgAlpha(0.95f);
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize;
    return ImGui::Begin(name, nullptr, flags | extraFlags);
}

/// Evidenzia il pulsante selezionato con le frecce e rileva anche il clic.
bool menuButton(const char* label, bool selected)
{
    if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    }
    const bool clicked = ImGui::Button(label, {-1.0f, 42.0f});
    if (selected) {
        ImGui::PopStyleColor();
    }
    return clicked;
}

} // namespace

/// Crea il contesto ImGui e inizializza i backend GLFW e OpenGL 3.3 per la finestra ricevuta.
void Gui::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = nullptr;

    // Il font integrato usa il glifo dell'apostrofo semplice anche per quello tipografico.
    ImFontAtlas& fonts = *ImGui::GetIO().Fonts;
    ImFont* font = fonts.AddFontDefault();
    if (fonts.Build()) {
        font->AddRemapChar(0x2019, '\'');
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = {24.0f, 20.0f};
    style.ItemSpacing = {8.0f, 10.0f};
    style.WindowRounding = 9.0f;
    style.FrameRounding = 5.0f;
    style.Colors[ImGuiCol_WindowBg] = {0.08f, 0.09f, 0.13f, 1.0f};
    style.Colors[ImGuiCol_Border] = {0.57f, 0.43f, 0.27f, 0.75f};
    style.Colors[ImGuiCol_Button] = {0.19f, 0.23f, 0.31f, 1.0f};
    style.Colors[ImGuiCol_ButtonHovered] = {0.31f, 0.35f, 0.44f, 1.0f};
    style.Colors[ImGuiCol_ButtonActive] = {0.43f, 0.34f, 0.23f, 1.0f};

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

/// Avvia un frame ImGui dopo avere aggiornato i due backend. Chiamare prima di costruire i
/// widget.
void Gui::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/// Costruisce il pannello Controlli. I widget aggiornano il colore di sfondo e la preferenza
/// wireframe della GUI.
void Gui::draw()
{
    ImGui::Begin("Controlli");

    ImGui::Text("Colore dello sfondo");
    ImGui::ColorEdit3(
        "Background",
        &m_clearColor.r
    );

    ImGui::Separator();

    ImGui::Text("Modalita' di visualizzazione");
    ImGui::Checkbox(
        "Wireframe",
        &m_wireframe
    );

    ImGui::End();
}

UiAction Gui::drawMenu()
{
    UiAction action = UiAction::None;
    if (beginPanel("Menu principale", 400.0f, 0.48f, 0.5f)) {
        if (ImGui::IsWindowAppearing() || ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
            m_tutorialSelected = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false)) {
            m_tutorialSelected = true;
        }

        const char* title = GameConfig::GAME_TITLE;
        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const float titleWidth = ImGui::CalcTextSize(title).x;
        const float titleScale = std::clamp(availableWidth / titleWidth, 1.0f, 2.0f);
        ImGui::SetWindowFontScale(titleScale);
        const float margin = std::max(0.0f, (availableWidth - titleWidth * titleScale) * 0.5f);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + margin);
        ImGui::TextColored(gold, "%s", title);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::Spacing();

        if (menuButton("Avvia storia", !m_tutorialSelected)) {
            action = UiAction::StartStory;
        }
        if (menuButton("Tutorial", m_tutorialSelected)) {
            action = UiAction::StartTutorial;
        }
        if (action == UiAction::None && ImGui::IsKeyPressed(ImGuiKey_Enter, false)) {
            action = m_tutorialSelected ? UiAction::StartTutorial : UiAction::StartStory;
        }
    }
    ImGui::End();
    return action;
}

UiAction Gui::drawDialogue(const DialogueLine& line)
{
    UiAction action = UiAction::None;
    if (beginPanel("Dialogo", 680.0f, 0.94f, 1.0f)) {
        ImGui::TextColored(gold, "%.*s", static_cast<int>(line.speaker.size()), line.speaker.data());
        ImGui::PushTextWrapPos(0.0f);
        ImGui::TextUnformatted(line.text.data(), line.text.data() + line.text.size());
        ImGui::PopTextWrapPos();
        if (ImGui::Button("Continua (Invio)") || ImGui::IsKeyPressed(ImGuiKey_Enter, false)) {
            action = UiAction::AdvanceDialogue;
        }
    }
    ImGui::End();
    return action;
}

void Gui::drawTutorial()
{
    if (beginPanel("Comandi del tutorial", 670.0f, 0.04f, 0.0f, ImGuiWindowFlags_NoInputs)) {
        ImGui::TextColored(gold, "%s", "Tutorial");
        ImGui::TextWrapped("Piccolo Principe   A / D movimento, W salto, S abbassati");
        ImGui::TextWrapped(u8"Volpe   Frecce laterali, Su salto, Gi\u00f9 abbassati");
        ImGui::TextWrapped("Spingi la cassa sulla pedana, abbassati nel passaggio basso e salta sopra il serpente.");
        ImGui::TextWrapped("R  Ricomincia     Esc  Menu principale");
    }
    ImGui::End();
}

/// Conclude il frame ImGui e invia a OpenGL i comandi di disegno dei widget.
void Gui::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// Disattiva i backend e distrugge il contesto ImGui prima della chiusura della finestra.
void Gui::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
