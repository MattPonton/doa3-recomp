#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;
void InitOnlineMenuFonts();
void InitOnlineMenuRenderer(ID3D11Device *device, ID3D11DeviceContext *context);
void ShutdownOnlineMenuRenderer();
void DrawOnlineMainMenu();
