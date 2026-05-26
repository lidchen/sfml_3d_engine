# sfml_3d_engine

A lightweight software 3D engine built with C++17 and SFML, focused on learning core real-time rendering concepts without relying on OpenGL/DirectX.  
It loads a triangle mesh from an `.obj` file, transforms it with a movable camera, and rasterizes shaded triangles directly through SFML draw calls.

## Why this project is interesting

- It demonstrates a full custom 3D pipeline in plain C++ math.
- It keeps the rendering logic explicit and easy to inspect for learning/debugging.
- It combines classic engine concepts (camera, projection, clipping, back-face culling, depth ordering, lighting) in a compact codebase.
- It includes real-time interaction controls and optional gaze input via ZeroMQ.

## Graphics render mechanism

The frame pipeline is implemented in `main.cpp` and follows this flow:

1. **Mesh input**: load triangles from an OBJ model (`Mesh::load_from_obj`).
2. **World transform**: optional rotation and object-space updates.
3. **Back-face culling**: skip triangles facing away from the camera using normal/view direction dot product.
4. **Lighting**: compute per-triangle light intensity from triangle normal and a directional light vector.
5. **View transform**: convert world-space vertices into camera/view space (`pointat_matrix` + `quick_inverse_matrix`).
6. **Near-plane clipping**: clip or discard geometry against the near plane (`obj_clipping` / `tri_clipping`).
7. **Projection**: apply perspective projection matrix to map 3D vertices into normalized screen coordinates.
8. **Depth ordering**: painter-style sort by average triangle `z` before drawing.
9. **Rasterization**: draw filled SFML triangles with grayscale intensity (plus optional debug wireframe).

This approach is CPU-side rasterization using SFML as the 2D drawing backend.

## Build

Dependencies:
- SFML 2.6 (`graphics`, `window`, `system`)
- cppzmq
- CMake (3.10+)
- C++17 compiler

Build commands:

```bash
cmake -S . -B build
cmake --build build
```
