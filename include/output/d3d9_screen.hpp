#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <d3d9.h>
#include <string>
#include <windows.h>

#include "graphics/color.hpp"
#include "math/vector.hpp"

namespace vbag {

class D3d9Screen;
class Engine;

struct Line {
  V3F start, end;
  D3DCOLOR color;
};

class Screen {
public:
  virtual void clear() = 0;
  [[nodiscard]] virtual size_t width() const = 0;
  [[nodiscard]] virtual size_t height() const = 0;
  virtual void drawLine(const Line &line) = 0;
  virtual void drawLines(const Line *lines, size_t n) = 0;
  virtual void drawTriangle(V3F v1, V3F v2, V3F v3, D3DCOLOR c1, D3DCOLOR c2,
                            D3DCOLOR c3) = 0;
  virtual void drawPoint(V3F p) = 0;
  virtual void present() = 0;
  virtual HWND window() = 0;

protected:
};

class D3d9Screen : public Screen {
private:
  class Error : public std::exception {
  public:
    explicit Error(std::string msg) : msg_{std::move(msg)} {}

    [[nodiscard]] const char *what() const noexcept override {
      return msg_.c_str();
    }

  private:
    std::string msg_;
  };

public:
  explicit D3d9Screen(HINSTANCE instance, const std::string &windowTitle,
                      size_t width, size_t height)
      : instance_{instance}, d3d_{Direct3DCreate9(D3D_SDK_VERSION)},
        width_{width}, height_{height} {
    if (!d3d_)
      throw Error{"could not create D3D9 runtime environment"};

    // initializing a window
    // creating and registering a window class
    {
      WNDCLASS windowClass{};
      windowClass.style = CS_HREDRAW | CS_VREDRAW;
      windowClass.lpfnWndProc = windowProcedure_;
      windowClass.hInstance = instance_;
      windowClass.lpszClassName = windowClassName_;
      RegisterClass(&windowClass);
    }

    // creating a normal window that can't be resized or maximized
    window_ = CreateWindow(windowClassName_, windowTitle.c_str(),
                           WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr,
                           nullptr, instance_, nullptr);
    if (!window_)
      throw Error{"could not create window"};

    // initializing d3d9
    // setting up presentation
    D3DPRESENT_PARAMETERS presentationParameters{};
    presentationParameters.Windowed = true;
    presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentationParameters.hDeviceWindow = window_;

    // creating a device
    d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_,
                       D3DCREATE_HARDWARE_VERTEXPROCESSING,
                       &presentationParameters, &device_);
    if (!device_)
      throw Error{"could not create D3D device"};

    ShowWindow(window_, SW_SHOWDEFAULT);
  }

  D3d9Screen(const D3d9Screen &) = delete;
  D3d9Screen(D3d9Screen &&) = delete;

  ~D3d9Screen() {
    DestroyWindow(window_);
    UnregisterClass(windowClassName_, instance_);
    device_->Release();
    d3d_->Release();
  }

  void drawLine(const Line &line) override {
    struct Vertex {
      float x, y, z, rhw;
      D3DCOLOR diffuse;
    };

    constexpr auto VertexType{D3DFVF_XYZRHW | D3DFVF_DIFFUSE};

    const Vertex vertices[2]{
        {line.start.x, line.start.y, line.start.z, 1, line.color},
        {line.end.x, line.end.y, line.end.z, 1, line.color}};

    device_->SetFVF(VertexType);

    IDirect3DVertexBuffer9 *vertexBuffer;
    device_->CreateVertexBuffer(2 * sizeof(Vertex), 0, VertexType,
                                D3DPOOL_DEFAULT, &vertexBuffer, nullptr);
    void *vertexBufferData;
    vertexBuffer->Lock(0, 2 * sizeof(Vertex), &vertexBufferData, 0);
    memcpy(vertexBufferData, vertices, 2 * sizeof(Vertex));
    vertexBuffer->Unlock();
    device_->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    device_->BeginScene();
    device_->DrawPrimitive(D3DPT_LINELIST, 0, 1);
    device_->EndScene();
    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    vertexBuffer->Release();
  }

  void drawLines(const Line *lines, size_t n) override {
    struct Vertex {
      float x, y, z, rhw;
      D3DCOLOR diffuse;
    };

    if (n == 0)
      return;

    constexpr auto VertexType{D3DFVF_XYZRHW | D3DFVF_DIFFUSE};

    std::vector<Vertex> vertices;
    for (size_t i{}; i < n; ++i) {
      auto color{lines[i].color};
      vertices.push_back(
          {lines[i].start.x, lines[i].start.y, lines[i].start.z, 1, color});
      vertices.push_back(
          {lines[i].end.x, lines[i].end.y, lines[i].end.z, 1, color});
    }

    device_->SetFVF(VertexType);

    IDirect3DVertexBuffer9 *vertexBuffer;
    device_->CreateVertexBuffer(2 * n * sizeof(Vertex), 0, VertexType,
                                D3DPOOL_DEFAULT, &vertexBuffer, nullptr);
    void *vertexBufferData;
    vertexBuffer->Lock(0, 2 * n * sizeof(Vertex), &vertexBufferData, 0);
    memcpy(vertexBufferData, vertices.data(), 2 * n * sizeof(Vertex));
    vertexBuffer->Unlock();
    device_->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    device_->BeginScene();
    device_->DrawPrimitive(D3DPT_LINELIST, 0, n);
    device_->EndScene();

    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    vertexBuffer->Release();
  }

  void drawTriangle(V3F v1, V3F v2, V3F v3, D3DCOLOR c1, D3DCOLOR c2,
                    D3DCOLOR c3) override {
    struct Vertex {
      float x, y, z, rhw;
      D3DCOLOR diffuse;
    };

    constexpr auto VertexType{D3DFVF_XYZRHW | D3DFVF_DIFFUSE};
    const Vertex vertices[3]{{v1.x, v1.y, v1.z, 1, c1},
                             {v2.x, v2.y, v2.z, 1, c2},
                             {v3.x, v3.y, v3.z, 1, c3}};
    device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    IDirect3DVertexBuffer9 *vertexBuffer;
    device_->CreateVertexBuffer(3 * sizeof(Vertex), 0, VertexType,
                                D3DPOOL_DEFAULT, &vertexBuffer, nullptr);
    void *vertexBufferData;
    vertexBuffer->Lock(0, 3 * sizeof(Vertex), &vertexBufferData, 0);
    memcpy(vertexBufferData, vertices, 3 * sizeof(Vertex));
    vertexBuffer->Unlock();
    device_->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
    device_->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

    device_->BeginScene();
    device_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    device_->EndScene();
    vertexBuffer->Release();
  }

  void drawPoint(V3F p) override {
    struct Vertex {
      float x, y, z, rhw;
      D3DCOLOR diffuse;
    };

    constexpr auto VertexType{D3DFVF_XYZRHW | D3DFVF_DIFFUSE};

    const Vertex vertices[1]{{p.x, p.y, p.z, 1, D3DCOLOR_XRGB(255, 255, 255)}};

    device_->SetFVF(VertexType);

    IDirect3DVertexBuffer9 *vertexBuffer;
    device_->CreateVertexBuffer(sizeof(Vertex), 0, VertexType, D3DPOOL_DEFAULT,
                                &vertexBuffer, nullptr);
    void *vertexBufferData;
    vertexBuffer->Lock(0, sizeof(Vertex), &vertexBufferData, 0);
    memcpy(vertexBufferData, vertices, sizeof(Vertex));
    vertexBuffer->Unlock();
    device_->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    float pointScale{5};
    device_->SetRenderState(D3DRS_POINTSIZE, *(DWORD *)&pointScale);
    device_->BeginScene();
    device_->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
    device_->EndScene();
    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    vertexBuffer->Release();
  }

  void clear() override { fill(0, 0, 0); }

  void fill(uint8_t r, uint8_t g, uint8_t b) {
    const auto color{D3DCOLOR_XRGB(r, g, b)};
    device_->Clear(0, nullptr, D3DCLEAR_TARGET, color, 1, 0);
  }

  [[nodiscard]] size_t width() const override { return width_; }
  [[nodiscard]] size_t height() const override { return height_; }
  [[nodiscard]] HWND window() override { return window_; }

  void present() override {
    device_->Present(nullptr, nullptr, nullptr, nullptr);
  }

private:
  static LRESULT windowProcedure_(HWND window, UINT msg, WPARAM wParam,
                                  LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
    case WM_DESTROY:
      PostQuitMessage(0);
      exit(0); // this is dirty
    default:
      return DefWindowProc(window, msg, wParam, lParam);
    }
  }

  static constexpr auto windowClassName_{"d3d9_window_class"};

  HINSTANCE instance_;
  HWND window_{};
  LPDIRECT3D9 d3d_;
  LPDIRECT3DDEVICE9 device_{};
  size_t width_, height_;
};

} // namespace vbag

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
