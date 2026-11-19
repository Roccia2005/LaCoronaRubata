#pragma once
#include "MeshData.h"
#include <filesystem>

namespace ObjLoader {
    // Restituisce la sagoma 2D centrata, con il lato maggiore lungo una unita'.
    MeshData load(const std::filesystem::path& path);
}
