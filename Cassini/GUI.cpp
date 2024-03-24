#pragma once
#include "imgui.h"
#include "GUI.h"

void GUI::RenderScene(Graphics* gfx) {
	ImGui::DockSpaceOverViewport(viewport);
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 650, viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 1000), ImGuiCond_FirstUseEver);
	ImGui::Begin("Cassini", &open);
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::Image(gfx->CreateSceneTexture(size), size);
	ImGui::End();
}