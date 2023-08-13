#ifndef VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
#define VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "math/vector.hpp"
#include <d3d9.h>
#include <string>
#include <windows.h>

class D3D9Screen;
class AnimationEngine;

class Screen {
public:
  virtual void clear() = 0;
  [[nodiscard]] virtual size_t width() const = 0;
  [[nodiscard]] virtual size_t height() const = 0;
  virtual void drawLine(V3F, V3F) = 0;
  virtual void present() = 0;
  virtual HWND window() = 0;

protected:
};

class D3D9Screen : public Screen {
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
  explicit D3D9Screen(HINSTANCE instance, const std::string &windowTitle,
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
                       D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                       &presentationParameters, &device_);
    if (!device_)
      throw Error{"could not create D3D device"};

    ShowWindow(window_, SW_SHOWDEFAULT);
  }

  D3D9Screen(const D3D9Screen &) = delete;
  D3D9Screen(D3D9Screen &&) = delete;

  ~D3D9Screen() {
    DestroyWindow(window_);
    UnregisterClass(windowClassName_, instance_);
    device_->Release();
    d3d_->Release();
  }

  void drawLine(V3F a, V3F b) override {
    struct Vertex {
      float x, y, z, rhw;
      D3DCOLOR color;
    };
    constexpr auto VertexType{D3DFVF_XYZRHW | D3DFVF_DIFFUSE};

    const Vertex vertices[2]{{a.x, a.y, a.z, 1, D3DCOLOR_XRGB(255, 0, 0)},
                             {b.x, b.y, b.z, 1, D3DCOLOR_XRGB(0, 0, 255)}};

    device_->SetFVF(VertexType);

    IDirect3DVertexBuffer9 *vertexBuffer;
    device_->CreateVertexBuffer(2 * sizeof(Vertex), 0, VertexType,
                                D3DPOOL_DEFAULT, &vertexBuffer, nullptr);

    void *vertexBufferData;
    vertexBuffer->Lock(0, 2 * sizeof(Vertex), &vertexBufferData, 0);
    memcpy(vertexBufferData, vertices, 2 * sizeof(Vertex));
    vertexBuffer->Unlock();

    device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    device_->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));

    device_->BeginScene();

    device_->DrawPrimitive(D3DPT_LINELIST, 0, 1);

    device_->EndScene();

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

#endif // VERY_BASIC_ASCII_GRAPHICS_API_INCLUDE_SCREEN_HPP
