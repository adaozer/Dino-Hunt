# 🦖 Dino Hunt

A first-person shooter built **from scratch in raw DirectX 12 (C++)** — no game engine, no third-party renderer. You drop into a jungle clearing armed with an automatic carbine and hunt down a fully animated T-Rex.

Every piece of the rendering pipeline — the device/swapchain setup, pipeline state objects, descriptor heaps, skeletal animation, instancing, collision — is hand-written to understand how a modern graphics API and a small game engine actually fit together.

## Gameplay

- Explore a procedurally scattered jungle: 1,000 GPU-instanced bamboo clumps and 10,000 instanced grass clumps around a central landmark tree
- Hunt a skeletally-animated T-Rex that idles, chases, attacks, and dies based on distance to the player
- Fire, reload, and inspect your weapon, each with its own animation and timing
- Full 3D collision against the environment, the enemy, and the world boundary (skybox radius)

## Controls

| Input | Action |
|---|---|
| `W` `A` `S` `D` | Move |
| Mouse | Look |
| Left Click | Fire |
| `R` | Reload |
| `F` | Inspect weapon |
| `Z` | Toggle mouse capture (lock/center cursor) |
| `Esc` | Quit |

## Engine features

Built entirely on the raw Direct3D 12 API:

- **Low-level D3D12 core** — device/adapter selection, command queues, double-buffered swapchain, descriptor heaps, GPU fences and resource barriers managed explicitly (`core.h`, `descriptorheap.h`, `Fence.h`, `Barrier.h`)
- **Pipeline state management** — a `PSOManager` that compiles and caches PSOs per shader/vertex-layout combination
- **Custom model format** — a `.gem` model loader (`GEMLoader.h`) for static and skeletally-rigged meshes
- **Skeletal animation system** — an `AnimationManager` that maps semantic actions (Idle/Run/Attack/Death/Reload/Inspect) to per-model animation clips, driving GPU vertex-shader skinning
- **GPU instancing** — thousands of vegetation instances drawn in a single draw call via `InstancedObject`
- **Custom shaders (HLSL)** — dedicated vertex/pixel shaders for static, animated, instanced, normal-mapped, and alpha-tested geometry
- **Collision system** — AABB-vs-sphere and ray-vs-sphere intersection tests for player movement, hitscan shooting, and enemy hitboxes (`collision.h`)
- **Texture streaming** — a `TextureManager` that loads and binds diffuse/normal maps into the shader-visible descriptor heap

## Tech stack

- **Language:** C++
- **Graphics API:** DirectX 12 (D3D12, DXGI)
- **Shaders:** HLSL
- **Platform:** Windows / Win32
- **Build:** Visual Studio 2022 (`window.sln`)

## Building & running

1. Open `window.sln` in Visual Studio 2022 with the Desktop C++ / Windows SDK workload installed.
2. Select the `Debug|x64` or `Release|x64` configuration.
3. Build and run — a console window opens alongside the game for debug/FPS output.

## Project structure

```
window/
├── core.h / core.cpp        # D3D12 device, swapchain, command queues
├── PSOManager.h              # Pipeline state object creation & caching
├── descriptorheap.h          # SRV/CBV/UAV descriptor heap management
├── Fence.h / Barrier.h       # GPU synchronization
├── GEMLoader.h / mesh.h      # Custom model format + GPU mesh upload
├── AnimatedModel.h           # Skeletal mesh + skinning
├── Animation.h / AnimationManager.h  # Animation clips & action mapping
├── InstancedObject.h         # GPU-instanced static meshes
├── Camera.h                  # FPS camera (yaw/pitch, view matrix)
├── Gun.h / Enemy.h           # Player weapon & T-Rex behaviour
├── collision.h / maths.h     # AABB/sphere collision, vector/matrix math
├── *.hlsl                    # Vertex & pixel shaders
├── models/                   # .gem meshes (T-Rex, carbine, vegetation)
└── textures/                 # Albedo & normal maps
```

## Acknowledgements

`GEMLoader.h` and `GamesEngineeringBase.h` are course-provided utilities (MIT licensed) from the MSc Games Engineering programme; everything else — the engine glue, gameplay systems, shaders, and hunt loop — is original.
