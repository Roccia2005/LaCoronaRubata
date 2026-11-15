#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Gui.h"

/// Crea il contesto ImGui e inizializza i backend GLFW e OpenGL 3.3 per la finestra ricevuta.
void Gui::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

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
