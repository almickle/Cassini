#pragma once
#include "Graphics.h"

class GUI
{
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoSavedSettings;
  bool open = true;

public:
  GUI() {}
  void RenderScene(ImTextureID);
};
