// Copyright 2019 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/NetPlayGolfUI.h"

#include <fmt/format.h>
#include <imgui.h>

#ifndef __SWITCH__
#include "Core/NetPlayClient.h"
#endif

constexpr float DEFAULT_WINDOW_WIDTH = 220.0f;
constexpr float DEFAULT_WINDOW_HEIGHT = 45.0f;

std::unique_ptr<NetPlayGolfUI> g_netplay_golf_ui;

NetPlayGolfUI::NetPlayGolfUI(std::shared_ptr<NetPlay::NetPlayClient> netplay_client)
    : m_netplay_client{netplay_client}
{
}

NetPlayGolfUI::~NetPlayGolfUI() = default;

void NetPlayGolfUI::Display()
{
  auto client = m_netplay_client.lock();
  if (!client)
    return;

  const float scale = ImGui::GetIO().DisplayFramebufferScale.x;

  ImGui::SetNextWindowPos(ImVec2((20.0f + DEFAULT_WINDOW_WIDTH) * scale, 10.0f * scale),
                          ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(
      ImVec2(DEFAULT_WINDOW_WIDTH * scale, DEFAULT_WINDOW_HEIGHT * scale),
      ImGui::GetIO().DisplaySize);

  // TODO: Translate these strings once imgui has multilingual fonts
  if (!ImGui::Begin("Golf Mode", nullptr, ImGuiWindowFlags_None))
  {
    ImGui::End();
    return;
  }

    //TODO: check better method of disabling this

#ifndef __SWITCH__
ImGui::Text("Current Golfer: %s", client->GetCurrentGolfer().c_str());


  if (client->LocalPlayerHasControllerMapped())
  {
    if (ImGui::Button("Take Control"))
    {
      client->RequestGolfControl();
    }

    for (auto player : client->GetPlayers())
    {
      if (client->IsLocalPlayer(player->pid) || !client->PlayerHasControllerMapped(player->pid))
        continue;

      if (ImGui::Button(fmt::format("Give Control to {}", player->name).c_str()))
      {
        client->RequestGolfControl(player->pid);
      }
    }
  }
#endif

  ImGui::End();
}
