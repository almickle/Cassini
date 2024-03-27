#pragma once
#include "Graphics.h"

class GUI
{
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoScrollWithMouse |
                                 ImGuiWindowFlags_NoScrollbar;
  const ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
  bool open = true;
  ImVec2 sceneSize;

public:
  GUI() {}
  void RenderScene(ImTextureID);
  ImVec2 GetSceneSize();
};
