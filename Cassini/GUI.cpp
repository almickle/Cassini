#pragma once
#include "GUI.h"
#include "imgui.h"

void
GUI::RenderScene(ImTextureID sceneTexture)
{
  ImGui::DockSpaceOverViewport(viewport, dockFlags);
  bool showDemo = true;
  // ImGui::ShowDemoWindow(&showDemo);
  ImGui::SetNextWindowSize(ImVec2(600, 1000), ImGuiCond_FirstUseEver);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Cassini", &open, flags);
  ImGui::PopStyleVar();
  sceneSize = ImGui::GetWindowContentRegionMax();
  ImGui::Image(sceneTexture, sceneSize);
  ImGui::End();
}

ImVec2
GUI::GetSceneSize()
{
  return sceneSize;
}