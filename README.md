# Very Basic ASCII Graphics (VBAG) API

https://github.com/thiagoferronatto/VBAG/assets/31262053/b8568e66-7834-4a8d-be12-3e8daf725203

## 1. Overview

VBAG is a boredom-induced holiday project turned game engine, capable of rendering (more on that below) scenes in 3D and projecting them into your console window while simultaneously processing user keyboard inputs in real time.

## 2. Installation

It shouldn't be hard to try the demo at all; just download the Windows executable from the Releases section and you're probably good to go! Unfortunately I had to use the Windows API to handle user inputs, because that was the easiest way for me to do it while maintaining good response times. If you have some ideas for dealing with keyboard inputs in a cross-platform way, please feel free to try and implement them!

## 3. Usage

To actually make something useful with this, you'll have to download the source code, write your scripts, and compile everything to then have access to the binary you want.

### 3.1. Compilation

To use the CMakeLists.txt I provided, you'll probably need MinGW64 on your windows machine; what I am sure of is that you will at least have to use GCC. However, if you feel comfortable with drifting away from CMake, then the world is your oyster; use your preferred build system with your preferred compiler, as long as it has support for the Windows API.

You will notice that there are build rules for the application icon. That may be left out of your compilation endeavors, as it is for entirely aesthetic purposes.

### 3.2. Writing your own scripts

To get started writing your stuff, you'll need to know a few things about the engine.

#### 3.2.1. General structure of a VBAG script

If you're familiar with other game engines (such as Unity, for example), then you'll know that there is usually a Start function and an Update function; one is run once at the start of the game loop, and the other is run every frame. Here you will find the same structure: you will have to define a Setup function and a Loop function -- they both serve the same purpose as Start and Update -- and write most of your code there. These functions must have the signature `void(AnimationEngine*)`.

I recommend using lambdas for both of them, since otherwise you'd have to rely on either global variables or class methods to emulate the functionality of a lambda capture list.

Basically, the idea is that if you have to use a resource in both Setup and Loop, then you define the variable that contains that resource outside of both functions, so that they have access to the scope that contains it. Here are some options. First, there's lambdas.

```cpp
int main() {
  GV3F g{"a_graph"}; // a Graph object, can be drawn
  // ...
  auto setup = [&](AnimationEngine* engine) {
    // you can use g here
  };

  auto loop = [&](AnimationEngine* engine) {
    // and you can also use it here
  }
}
```

Then, there's the global variable approach.

```cpp
GV3F g{"a_graph"};

auto setup(AnimationEngine* engine) {
  // you can use g here
};

void loop(AnimationEngine* engine) {
  // and you can also use it here
}
```

And, finally, the Java approach.

```cpp
class AReallyLongAndVerboseWrapperClassName {
public:
  void aReallyLongAndVerboseSetupFunctionName(AnimationEngine* engine) {
    // at least it works, I guess
  }

  void aReallyLongAndVerboseLoopFunctionName(AnimationEngine* engine) {
    // help
  }
private:
  GV3F aReallyLongAndVerboseAttributeName_{"a_graphs_verbose_name"};
};
```

#### 3.2.2. Screens

To actually run the scripts, you'll need a Screen to render your stuff onto. Getting that is pretty simple.

```cpp
#include "output/screen.hpp"

Screen screen{200, 100};
```

Notice that you can specify a resolution that works best for you, but it should be kept small; there ain't no GPU accelerated rendering pipeline here.

#### 3.2.3. Scenes

Scenes are collections of Objects. Objects can -- at the time of writing -- be either Cameras or Graphs. Creating a scene can be done as follows.

```cpp
#include "geometry/scene.hpp

Scene scene;
```

Cameras serve as points of view through which you can observe the other objects in a scene. A scene can have multiple cameras, but only one may be chosen as the main camera at a time. To create a camera, just go ahead and

```cpp
#include "graphics/camera.hpp"

auto cameraName{"my_camera"};
auto cameraFovDeg{3.0f};
auto cameraAspectRatio{float(screen.width())/screen.height()};

Camera camera{cameraName, cameraFovDeg, cameraAspectRatio};
```

Do make sure to pay attention to your aspect ratio, as it will stretch and squish your resulting image depending on the screen resolution you defined earlier.

When it comes to graphs, there are no caveats; every single graph in a scene that is within the vision frustum of the main camera will be drawn. It is worth mentioning that, as we are not bound by OpenGL standards, there is no need for near and far planes in the frustum; your graphs will be drawn even if they're infinitely far away. Magnificent. You can instantiate and populate a graph like so.

```cpp
#include "geometry/graph.hpp"

GV3F graph{"my_graph"};

graph.addVertex(0, 0, 0);
graph.addVertex(0, 0, 1);
graph.addVertex(0, 1, 0);
graph.addVertex(0, 1, 1);

graph.addEdge(0, 1);
graph.addEdge(1, 2);
graph.addEdge(2, 3);
graph.addEdge(3, 0);
```

Now, I'm not absolutely sure and find myself too sleepy to confirm, but I think that ends up as a square in the YZ plane.

Graphs are drawn as wireframes, meaning that only their edges are visible. Build your graphs with that in mind. Maybe at some point I'll try to implement triangle meshes and a lighting system again. Anyway, now you should probably add all that stuff to your scene. Here goes.

```cpp
scene.addObject(&camera);
scene.addObject(&graph);
```

And that's that. Now, that will give you a runtime error if you try and run it, since we need to explicitly specify a main camera. No biggie, do it like this.

```cpp
scene.setMainCamera("my_camera");
```

Here's one important concept to remember: once your objects are added to the scene, you will only reference them by their name. No pointers, no workarounds, just do it like that. I recommend creating references to make the code less spaghettified.

```cpp
auto &myObject{*scene.object("my_object")};
```

Notice the dereference there, since Scene::object returns a pointer. Currently you have to do the dynamic casting yourself to check if it is a graph or a camera. Sorry. In my defense, you'll usually only access them like this if you want to move them around, so just leaving them as an `Object&` is enough, since all objects have transforms.

Now, it is worth mentioning that every object can be part of a larger hierarchy. A scene is a tree, with itself at its root, and the objects as the leaves. Every object can have its own children. When you add an object to the scene, all of its children are added. When you draw an object, all of its children are drawn. When you transform (translate, rotate, or scale) an object, all of its children are transformed. That does not mean that children don't have a degree of indepencence; you CAN transform them separately from their parents, just not the other way around. Here's an example.

```cpp
GV3F player{"player"};
Camera camera{"a_camera", ...};

player.addChild(&camera);

player.transform().translate(1, 1, 1);
camera.transform().translate(1, 1, 1);

player.removeChild(camera);

player.transform().translate(-1, -1, -1);
```

Here we made the camera a child of the player object; when we translated the player, the camera moved alongside it. When we moved the camera specifically, the player was not affected. After we removed the camera as a child, it was no longer bound to the player and would no longer be affected by its transformations. In the end there, the player would be at (0, 0, 0), while the camera would be at (2, 2, 2).

#### 3.2.4. The engine

After you have your screen, functions and scene set up, the next step should be to actually create an instance of the engine; again, super simple. Here you'll need to use both your screen and your setup/loop functions. The screen is taken by reference, while for the functions you can use lambdas, function pointers, functors, really anything that respects the signature mentioned earlier.

```cpp
#include "animation/animation_engine.hpp"

AnimationEngine engine{screen, setup, loop, scene};
```

If you went the Java way, you'll end up with something like

```cpp
AReallyLongAndVerboseWrapperClassName aRLAVWCN;
AnimationEngine engine{screen,
    aRLAVWCN.aReallyLongAndVerboseSetupFunctionName,
    aRLAVWCN.aReallyLongAndVerboseLoopFunctionName, scene};
```

Nice.

At last, you can now run the engine with

```cpp
engine.run();
```

Notice that this will make it run indefinitely; it is your responsibility to provide at least one way for the user to exit the game loop, whether by a button press inside the Loop function or through signal handling.

#### 3.2.5. What can you do?

You can manipulate all objects using their transforms (transformation matrices), which provide means to translate, rotate and scale their objects at will. You can either manually animate a sequence with these operations or use keyboard inputs to trigger them. To accomplish the latter, use the Input class.

```cpp
#include "input/input.hpp"

if (Input::getKey(KeyCode::W)) {
  // perform your logic if W is pressed
}
```

Just like in Unity. Any similarity is pure coincidence. Now, you can go into `input/input.hpp` and check out the key codes currently available, but you can also create your own. Go nuts.

All relevant functions are properly documented with (AI generated) Doxygen comments, so feel free to delve deep into the source code here; maybe you'll find something interesting.

And that's about it. The rest is up to your imagination. If you need some more inspiration, there is the code for the demo at `source/tests/animation_test.cpp` that you can take a look at. Not my best work, but who gives a f
