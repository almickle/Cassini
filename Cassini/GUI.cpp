#pragma once
#include "GUI.h"
#include "imgui.h"

void
GUI::RenderScene(ImTextureID sceneTexture)
{
  ImGui::DockSpaceOverViewport(viewport);
  ImGui::SetNextWindowPos(
    ImVec2(viewport->WorkPos.x + 650, viewport->WorkPos.y + 20),
    ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(600, 1000), ImGuiCond_FirstUseEver);
  ImGui::Begin("Cassini", &open);
  sceneSize = ImGui::GetWindowSize();
  ImGui::Image(sceneTexture, sceneSize);
  ImGui::End();
}

ImVec2
GUI::GetSceneSize()
{
  return sceneSize;
}