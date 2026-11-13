#include "DecorativeShapes.h"
#include <cmath>

// Figure 2D composte soltanto da triangoli.
// Ogni vertice occupa cinque float: x, y, r, g, b.
namespace {

// Inserisce un triangolo nella mesh, con un unico colore per i tre vertici.
void addTriangle(MeshData& mesh, const glm::vec2& a,
                 const glm::vec2& b, const glm::vec2& c,
                 const glm::vec3& color)
{
    const glm::vec2 points[3] = {a, b, c};
    for (const glm::vec2& p : points) {
        mesh.vertices.insert(mesh.vertices.end(),
                             {p.x, p.y, color.r, color.g, color.b});
    }
}

// Un rettangolo e' formato da due triangoli: ABC e ACD.
void addRectangle(MeshData& mesh, float xMin, float yMin,
                  float xMax, float yMax, const glm::vec3& color)
{
    const glm::vec2 A{xMin, yMin};
    const glm::vec2 B{xMax, yMin};
    const glm::vec2 C{xMax, yMax};
    const glm::vec2 D{xMin, yMax};
    addTriangle(mesh, A, B, C, color);
    addTriangle(mesh, A, C, D, color);
}

} // namespace anonimo

namespace DecorativeShapes {

MeshData man()
{
    MeshData mesh;
    // Sagoma unitaria centrata. Gli stivali toccano y = -0.5.
    const glm::vec3 boots{0.16f, 0.12f, 0.22f};
    const glm::vec3 tunic{0.16f, 0.38f, 0.68f};
    const glm::vec3 gold{1.00f, 0.73f, 0.28f};
    const glm::vec3 skin{0.90f, 0.65f, 0.47f};
    const glm::vec3 hair{0.27f, 0.15f, 0.11f};

    addRectangle(mesh, -0.32f, -0.50f, -0.06f, -0.21f, boots);
    addRectangle(mesh,  0.06f, -0.50f,  0.32f, -0.21f, boots);
    addRectangle(mesh, -0.43f, -0.12f,  0.43f,  0.12f, tunic);
    addRectangle(mesh, -0.32f, -0.31f,  0.32f,  0.16f, tunic);
    addRectangle(mesh, -0.32f, -0.23f,  0.32f, -0.15f, boots);
    addRectangle(mesh, -0.08f, -0.23f,  0.08f, -0.15f, gold);
    addTriangle(mesh, {-0.19f, 0.13f}, {0.0f, -0.01f}, {0.19f, 0.13f}, gold);

    addRectangle(mesh, -0.25f, 0.15f, 0.25f, 0.44f, skin);
    addRectangle(mesh, -0.28f, 0.38f, 0.28f, 0.50f, hair);
    addRectangle(mesh, -0.15f, 0.26f, -0.09f, 0.32f, boots);
    addRectangle(mesh,  0.09f, 0.26f,  0.15f, 0.32f, boots);
    return mesh;
}

MeshData women()
{
    MeshData mesh;
    const glm::vec3 boots{0.23f, 0.10f, 0.16f};
    const glm::vec3 cape{0.50f, 0.14f, 0.29f};
    const glm::vec3 tunic{0.81f, 0.27f, 0.39f};
    const glm::vec3 gold{1.00f, 0.76f, 0.37f};
    const glm::vec3 skin{0.82f, 0.54f, 0.36f};
    const glm::vec3 hair{0.22f, 0.13f, 0.15f};

    addTriangle(mesh, {-0.49f, -0.35f}, {0.49f, -0.35f}, {0.0f, 0.20f}, cape);
    addRectangle(mesh, -0.26f, -0.50f, -0.05f, -0.25f, boots);
    addRectangle(mesh,  0.05f, -0.50f,  0.26f, -0.25f, boots);
    addRectangle(mesh, -0.39f, -0.11f,  0.39f,  0.11f, tunic);
    addRectangle(mesh, -0.25f, -0.33f,  0.25f,  0.15f, tunic);
    addRectangle(mesh, -0.25f, -0.22f,  0.25f, -0.16f, gold);
    addRectangle(mesh, -0.06f,  0.03f,  0.06f,  0.11f, gold);

    addRectangle(mesh, -0.30f,  0.12f,  0.30f, 0.50f, hair);
    addRectangle(mesh,  0.20f, -0.11f,  0.33f, 0.30f, hair);
    addRectangle(mesh,  0.20f, -0.08f,  0.33f, -0.02f, gold);
    addRectangle(mesh, -0.22f,  0.15f,  0.22f, 0.39f, skin);
    addTriangle(mesh, {-0.24f, 0.33f}, {0.18f, 0.40f}, {-0.24f, 0.44f}, hair);
    addRectangle(mesh, -0.14f, 0.24f, -0.08f, 0.30f, boots);
    addRectangle(mesh,  0.08f, 0.24f,  0.14f, 0.30f, boots);
    return mesh;
}

MeshData crystal(float radius)
{
    MeshData mesh;
    // Rombo: quattro vertici e due triangoli, senza dettagli aggiuntivi.
    // Resta contenuto nell'AABB di semilato COIN_RADIUS.
    const glm::vec2 top{0.0f, radius};
    const glm::vec2 right{radius, 0.0f};
    const glm::vec2 bottom{0.0f, -radius};
    const glm::vec2 left{-radius, 0.0f};

    addTriangle(mesh, top, left, bottom, {0.17f, 0.87f, 0.97f});
    addTriangle(mesh, top, bottom, right, {0.06f, 0.56f, 0.88f});
    return mesh;
}

MeshData platformRock()
{
    MeshData mesh;
    // Blocco unitario marrone. La Model Matrix lo scala alla dimensione
    // dell'ostacolo; la collisione AABB rimane quella originale.
    addRectangle(mesh, -0.5f, -0.5f, 0.5f, 0.5f,
                 {0.50f, 0.35f, 0.24f});
    return mesh;
}

MeshData grassStrip()
{
    MeshData mesh;
    // Sottile fascia verde che viene disegnata sopra il blocco.
    // E' solo decorativa e non modifica le collisioni.
    addRectangle(mesh, -0.5f, -0.5f, 0.5f, 0.5f,
                 {0.26f, 0.72f, 0.33f});
    return mesh;
}

DoorMeshes door()
{
    DoorMeshes meshes;
    constexpr float bottom = -0.5f;
    constexpr float archBase = 0.17f;
    constexpr float pi = 3.14159265f;
    constexpr int archStones = 10;
    const glm::vec3 mortar{0.20f, 0.19f, 0.24f};
    const glm::vec3 stoneLight{0.55f, 0.51f, 0.53f};
    const glm::vec3 stoneDark{0.43f, 0.40f, 0.45f};
    const glm::vec3 wood{0.43f, 0.23f, 0.13f};
    const glm::vec3 woodShade{0.31f, 0.15f, 0.09f};
    const glm::vec3 iron{0.19f, 0.19f, 0.23f};
    const glm::vec3 brass{0.93f, 0.70f, 0.31f};

    // Il fondo scuro affiora nelle fughe tra le pietre.
    addRectangle(meshes.recess, -0.50f, bottom, 0.50f, archBase, mortar);
    for (int i = 0; i < archStones; ++i) {
        const float start = pi * i / archStones;
        const float end = pi * (i + 1) / archStones;
        addTriangle(meshes.recess, {0.0f, archBase},
                    {0.50f * std::cos(start), archBase + 0.33f * std::sin(start)},
                    {0.50f * std::cos(end), archBase + 0.33f * std::sin(end)}, mortar);
    }

    // Blocchi dei montanti e conci dell'arco.
    for (int i = 0; i < 4; ++i) {
        const float y = bottom + (archBase - bottom) * i / 4;
        const float top = bottom + (archBase - bottom) * (i + 1) / 4 - 0.010f;
        const glm::vec3& stone = i % 2 == 0 ? stoneLight : stoneDark;
        addRectangle(meshes.frame, -0.50f, y, -0.38f, top, stone);
        addRectangle(meshes.frame, 0.38f, y, 0.50f, top, stone);
    }
    for (int i = 0; i < archStones; ++i) {
        const float start = pi * i / archStones + 0.012f;
        const float end = pi * (i + 1) / archStones - 0.012f;
        const glm::vec2 innerStart{0.38f * std::cos(start), archBase + 0.24f * std::sin(start)};
        const glm::vec2 innerEnd{0.38f * std::cos(end), archBase + 0.24f * std::sin(end)};
        const glm::vec2 outerStart{0.50f * std::cos(start), archBase + 0.33f * std::sin(start)};
        const glm::vec2 outerEnd{0.50f * std::cos(end), archBase + 0.33f * std::sin(end)};
        const glm::vec3& stone = i % 2 == 0 ? stoneLight : stoneDark;
        addTriangle(meshes.frame, innerStart, outerStart, outerEnd, stone);
        addTriangle(meshes.frame, innerStart, outerEnd, innerEnd, stone);
    }

    addRectangle(meshes.frame, -0.50f, bottom, 0.50f, -0.465f, stoneDark);

    // Le ante condividono le coordinate locali della cornice.
    for (int side = 0; side < 2; ++side) {
        const bool left = side == 0;
        MeshData& leaf = meshes.leaves[side];
        const float hinge = meshes.hingeX[side];
        const float minX = left ? hinge : 0.0f;
        const float maxX = left ? 0.0f : hinge;
        addRectangle(leaf, minX, bottom, maxX, archBase, wood);

        const int firstSegment = left ? archStones / 2 : 0;
        for (int i = firstSegment; i < firstSegment + archStones / 2; ++i) {
            const float start = pi * i / archStones;
            const float end = pi * (i + 1) / archStones;
            addTriangle(leaf, {0.0f, archBase},
                        {0.38f * std::cos(start), archBase + 0.24f * std::sin(start)},
                        {0.38f * std::cos(end), archBase + 0.24f * std::sin(end)}, wood);
        }
        for (float distance : {0.123f, 0.246f}) {
            const float x = left ? -distance : distance;
            const float ratio = (distance + 0.005f) / 0.38f;
            const float top = archBase + 0.24f * std::sqrt(1.0f - ratio * ratio) - 0.006f;
            addRectangle(leaf, x - 0.005f, bottom, x + 0.005f, top, woodShade);
        }
        addRectangle(leaf, left ? -0.012f : 0.0f, bottom,
                     left ? 0.0f : 0.012f, archBase + 0.233f, woodShade);

        for (float y : {-0.34f, 0.04f}) {
            addRectangle(leaf, left ? -0.37f : 0.09f, y,
                         left ? -0.09f : 0.37f, y + 0.045f, iron);
            addRectangle(leaf, left ? -0.32f : 0.30f, y + 0.013f,
                         left ? -0.30f : 0.32f, y + 0.032f, brass);
        }
        addRectangle(leaf, left ? -0.105f : 0.035f, -0.19f,
                     left ? -0.035f : 0.105f, -0.075f, iron);
        addRectangle(leaf, left ? -0.092f : 0.048f, -0.146f,
                     left ? -0.048f : 0.092f, -0.114f, brass);
    }
    return meshes;
}

} // namespace DecorativeShapes
