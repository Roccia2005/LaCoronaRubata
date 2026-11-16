#pragma once
#include "MeshData.h"
#include <glm/glm.hpp>
#include <array>
namespace DecorativeShapes {
// Tutti i vertici sono in coordinate locali.
MeshData littlePrince();
MeshData fox();
MeshData crystal(float radius);
MeshData crate();
MeshData goblin();
MeshData ghost();
MeshData wall();
MeshData movingWall();
MeshData bridge();
struct DoorMeshes {
    MeshData recess;
    std::array<MeshData, 2> leaves;
    MeshData frame;
    static constexpr std::array<float, 2> hingeX{-0.38f, 0.38f};
};
DoorMeshes door();

struct PressurePlateMeshes {
    MeshData base;
    MeshData releasedPlate;
    MeshData pressedPlate;
};
PressurePlateMeshes pressurePlate();
}
