#include "DecorativeShapes.h"
#include <cmath>
#include <utility>

// Figure 2D composte soltanto da triangoli.
// Ogni vertice occupa cinque float: x, y, r, g, b.
namespace {

/// Aggiunge alla mesh tre vertici in coordinate locali, assegnando a tutti lo stesso colore RGB.
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

/// Divide in due triangoli un quadrilatero convesso con vertici ordinati lungo il bordo.
void addQuad(MeshData& mesh, const glm::vec2& a, const glm::vec2& b,
             const glm::vec2& c, const glm::vec2& d, const glm::vec3& color)
{
    addTriangle(mesh, a, b, c, color);
    addTriangle(mesh, a, c, d, color);
}

/// Aggiunge un rettangolo delimitato dalle coordinate minime e massime.
void addRectangle(MeshData& mesh, float xMin, float yMin,
                  float xMax, float yMax, const glm::vec3& color)
{
    addQuad(mesh, {xMin, yMin}, {xMax, yMin}, {xMax, yMax}, {xMin, yMax}, color);
}

} // namespace anonimo

namespace DecorativeShapes {

/// Compone il Piccolo Principe con il cappotto azzurro, il mantello e la sciarpa.
/// La base degli stivali a y = -0.5 coincide con la quota dei piedi.
CharacterMeshes littlePrinceParts()
{
    CharacterMeshes parts;
    const glm::vec3 navy{0.08f, 0.15f, 0.28f};
    const glm::vec3 cape{0.14f, 0.36f, 0.66f};
    const glm::vec3 coat{0.31f, 0.66f, 0.93f};
    const glm::vec3 trim{0.68f, 0.88f, 1.00f};
    const glm::vec3 scarf{0.43f, 0.84f, 0.95f};
    const glm::vec3 skin{0.98f, 0.77f, 0.58f};
    const glm::vec3 hair{0.96f, 0.73f, 0.34f};
    const glm::vec3 gold{1.00f, 0.88f, 0.53f};

    addQuad(parts.body, {-0.44f, -0.43f}, {0.39f, -0.40f},
            {0.23f, 0.15f}, {-0.19f, 0.15f}, cape);
    addRectangle(parts.body, -0.20f, -0.50f, -0.05f, -0.25f, navy);
    addRectangle(parts.body, 0.05f, -0.50f, 0.20f, -0.25f, navy);
    addQuad(parts.body, {-0.28f, -0.35f}, {0.28f, -0.35f},
            {0.19f, 0.13f}, {-0.19f, 0.13f}, coat);
    addQuad(parts.body, {-0.35f, -0.16f}, {-0.23f, -0.19f},
            {-0.13f, 0.10f}, {-0.24f, 0.11f}, coat);
    addQuad(parts.body, {0.23f, -0.19f}, {0.35f, -0.16f},
            {0.24f, 0.11f}, {0.13f, 0.10f}, coat);
    addRectangle(parts.body, -0.35f, -0.20f, -0.25f, -0.16f, skin);
    addRectangle(parts.body, 0.25f, -0.20f, 0.35f, -0.16f, skin);
    addRectangle(parts.body, -0.24f, -0.24f, 0.24f, -0.19f, cape);
    addRectangle(parts.body, -0.04f, -0.245f, 0.04f, -0.18f, gold);

    addQuad(parts.head, {-0.14f, 0.14f}, {0.14f, 0.14f},
            {0.20f, 0.40f}, {-0.20f, 0.40f}, skin);
    addQuad(parts.head, {-0.23f, 0.29f}, {0.21f, 0.40f},
            {0.08f, 0.49f}, {-0.14f, 0.47f}, hair);
    addTriangle(parts.head, {-0.18f, 0.41f}, {-0.02f, 0.50f}, {-0.02f, 0.42f}, gold);
    addTriangle(parts.head, {0.04f, 0.43f}, {0.20f, 0.50f}, {0.15f, 0.41f}, hair);
    addRectangle(parts.head, -0.12f, 0.25f, -0.08f, 0.30f, navy);
    addRectangle(parts.head, 0.08f, 0.25f, 0.12f, 0.30f, navy);

    addQuad(parts.foreground, {0.11f, 0.04f}, {0.44f, 0.00f},
            {0.50f, 0.11f}, {0.16f, 0.15f}, scarf);
    addRectangle(parts.foreground, -0.19f, 0.06f, 0.18f, 0.15f, trim);
    return parts;
}

/// Compone la Volpe con orecchie a punta, muso chiaro e coda folta.
/// La base delle zampe ha la stessa quota locale degli stivali del Principe.
CharacterMeshes foxParts()
{
    CharacterMeshes parts;
    parts.neckPivot = {-0.05f, 0.06f};
    const glm::vec3 dark{0.23f, 0.09f, 0.12f};
    const glm::vec3 rust{0.77f, 0.25f, 0.13f};
    const glm::vec3 orange{0.96f, 0.45f, 0.19f};
    const glm::vec3 honey{1.00f, 0.61f, 0.29f};
    const glm::vec3 cream{1.00f, 0.88f, 0.71f};

    // La coda parte dietro il corpo e risale sul fianco.
    addQuad(parts.body, {0.12f, -0.38f}, {0.38f, -0.30f},
            {0.50f, -0.07f}, {0.28f, -0.13f}, rust);
    addQuad(parts.body, {0.28f, -0.13f}, {0.50f, -0.07f},
            {0.46f, 0.15f}, {0.34f, 0.08f}, orange);
    addTriangle(parts.body, {0.36f, 0.00f}, {0.48f, 0.02f}, {0.46f, 0.15f}, cream);

    addQuad(parts.body, {-0.33f, -0.36f}, {0.21f, -0.36f},
            {0.18f, 0.09f}, {-0.25f, 0.09f}, rust);
    addQuad(parts.body, {-0.24f, -0.27f}, {0.07f, -0.30f},
            {0.12f, 0.06f}, {-0.20f, 0.06f}, orange);
    addTriangle(parts.body, {-0.22f, 0.06f}, {-0.06f, -0.26f}, {0.10f, 0.06f}, cream);
    addRectangle(parts.body, -0.29f, -0.50f, -0.14f, -0.34f, dark);
    addRectangle(parts.body, 0.02f, -0.50f, 0.17f, -0.34f, dark);

    addTriangle(parts.head, {-0.37f, 0.19f}, {-0.34f, 0.50f}, {-0.12f, 0.30f}, rust);
    addTriangle(parts.head, {0.03f, 0.30f}, {0.24f, 0.50f}, {0.27f, 0.19f}, rust);
    addTriangle(parts.head, {-0.32f, 0.24f}, {-0.31f, 0.42f}, {-0.21f, 0.29f}, cream);
    addTriangle(parts.head, {0.12f, 0.29f}, {0.22f, 0.42f}, {0.23f, 0.24f}, cream);
    addQuad(parts.head, {-0.45f, 0.13f}, {0.35f, 0.13f},
            {0.17f, 0.32f}, {-0.27f, 0.32f}, orange);
    addTriangle(parts.head, {-0.34f, 0.24f}, {-0.05f, 0.34f}, {0.23f, 0.24f}, honey);
    addQuad(parts.head, {-0.45f, 0.13f}, {-0.18f, -0.05f},
            {0.08f, -0.05f}, {0.35f, 0.13f}, cream);
    addTriangle(parts.head, {-0.20f, 0.15f}, {-0.05f, 0.01f}, {0.10f, 0.15f}, orange);
    addRectangle(parts.head, -0.24f, 0.15f, -0.18f, 0.21f, dark);
    addRectangle(parts.head, 0.08f, 0.15f, 0.14f, 0.21f, dark);
    addTriangle(parts.head, {-0.11f, 0.03f}, {0.01f, 0.03f}, {-0.05f, -0.03f}, dark);
    return parts;
}

namespace {

MeshData combine(CharacterMeshes parts)
{
    auto& vertices = parts.body.vertices;
    vertices.reserve(vertices.size() + parts.head.vertices.size() + parts.foreground.vertices.size());
    for (const auto* layer : {&parts.head, &parts.foreground}) {
        vertices.insert(vertices.end(), layer->vertices.begin(), layer->vertices.end());
    }
    return std::move(parts.body);
}

} // namespace

MeshData littlePrince() { return combine(littlePrinceParts()); }
MeshData fox() { return combine(foxParts()); }

/// Restituisce un rombo bicolore centrato nell'origine. radius e' la distanza dal centro a
/// ciascuna punta, in unita' mondo.
MeshData crystal(float radius)
{
    MeshData mesh;
    // Rombo: quattro vertici e due triangoli, senza dettagli aggiuntivi.
    // Resta contenuto nell'AABB di semilato STAR_RADIUS.
    const glm::vec2 top{0.0f, radius};
    const glm::vec2 right{radius, 0.0f};
    const glm::vec2 bottom{0.0f, -radius};
    const glm::vec2 left{-radius, 0.0f};

    addTriangle(mesh, top, left, bottom, {0.17f, 0.87f, 0.97f});
    addTriangle(mesh, top, bottom, right, {0.06f, 0.56f, 0.88f});
    return mesh;
}

namespace {

constexpr float bottom = -0.5f;
constexpr float archBase = 0.17f;
constexpr float pi = 3.14159265f;
constexpr int archStones = 10;

/// Genera il vano scuro che si vede tra le pietre della cornice.
MeshData makeDoorRecess()
{
    MeshData mesh;
    const glm::vec3 mortar{0.20f, 0.19f, 0.24f};
    // Il fondo scuro affiora nelle fughe tra le pietre.
    addRectangle(mesh, -0.50f, bottom, 0.50f, archBase, mortar);
    for (int i = 0; i < archStones; ++i) {
        const float start = pi * i / archStones;
        const float end = pi * (i + 1) / archStones;
        addTriangle(mesh, {0.0f, archBase},
                    {0.50f * std::cos(start), archBase + 0.33f * std::sin(start)},
                    {0.50f * std::cos(end), archBase + 0.33f * std::sin(end)}, mortar);
    }
    return mesh;
}

/// Genera i montanti, i conci dell'arco e la soglia della cornice in pietra.
MeshData makeDoorFrame()
{
    MeshData mesh;
    const glm::vec3 stoneLight{0.55f, 0.51f, 0.53f};
    const glm::vec3 stoneDark{0.43f, 0.40f, 0.45f};
    // Blocchi dei montanti e conci dell'arco.
    for (int i = 0; i < 4; ++i) {
        const float y = bottom + (archBase - bottom) * i / 4;
        const float top = bottom + (archBase - bottom) * (i + 1) / 4 - 0.010f;
        const glm::vec3& stone = i % 2 == 0 ? stoneLight : stoneDark;
        addRectangle(mesh, -0.50f, y, -0.38f, top, stone);
        addRectangle(mesh, 0.38f, y, 0.50f, top, stone);
    }
    for (int i = 0; i < archStones; ++i) {
        const float start = pi * i / archStones + 0.012f;
        const float end = pi * (i + 1) / archStones - 0.012f;
        const glm::vec2 innerStart{0.38f * std::cos(start), archBase + 0.24f * std::sin(start)};
        const glm::vec2 innerEnd{0.38f * std::cos(end), archBase + 0.24f * std::sin(end)};
        const glm::vec2 outerStart{0.50f * std::cos(start), archBase + 0.33f * std::sin(start)};
        const glm::vec2 outerEnd{0.50f * std::cos(end), archBase + 0.33f * std::sin(end)};
        const glm::vec3& stone = i % 2 == 0 ? stoneLight : stoneDark;
        addTriangle(mesh, innerStart, outerStart, outerEnd, stone);
        addTriangle(mesh, innerStart, outerEnd, innerEnd, stone);
    }

    addRectangle(mesh, -0.50f, bottom, 0.50f, -0.465f, stoneDark);
    return mesh;
}

/// Genera un'anta con doghe e dettagli metallici nel riferimento locale della porta.
MeshData makeDoorLeaf(std::size_t side)
{
    MeshData leaf;
    const bool left = side == 0;
    const float hinge = DoorMeshes::hingeX[side];
    const glm::vec3 wood{0.43f, 0.23f, 0.13f};
    const glm::vec3 woodShade{0.31f, 0.15f, 0.09f};
    const glm::vec3 iron{0.19f, 0.19f, 0.23f};
    const glm::vec3 brass{0.93f, 0.70f, 0.31f};

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
    return leaf;
}

} // namespace

/// Restituisce le mesh del vano, della cornice e delle due ante nello stesso riferimento locale.
/// Le ante sono separate per consentire trasformazioni indipendenti attorno ai cardini.
DoorMeshes door()
{
    DoorMeshes meshes;
    meshes.recess = makeDoorRecess();
    meshes.frame = makeDoorFrame();
    for (std::size_t side = 0; side < meshes.leaves.size(); ++side) {
        meshes.leaves[side] = makeDoorLeaf(side);
    }
    return meshes;
}

/// Restituisce la base della pedana e due varianti cromatiche del pulsante. Lo spostamento
/// verticale del pulsante viene applicato dalla vista.
PressurePlateMeshes pressurePlate()
{
    PressurePlateMeshes meshes;
    const glm::vec3 housing{0.20f, 0.23f, 0.34f};
    const glm::vec3 steel{0.64f, 0.70f, 0.80f};
    const glm::vec3 bevel{0.82f, 0.88f, 0.94f};

    // I bordi sporgono dalla sagoma fisica per restare visibili sotto una cassa.
    addQuad(meshes.base, {-0.88f, -0.50f}, {0.88f, -0.50f},
            {0.77f, 0.15f}, {-0.77f, 0.15f}, housing);
    addRectangle(meshes.base, -0.80f, -0.39f, 0.80f, -0.08f, steel);
    addRectangle(meshes.base, -0.77f, -0.08f, 0.77f, 0.15f, bevel);

    // La vista abbassa il pulsante premuto. Il bordo superiore segue il collider.
    const auto makeButton = [](const glm::vec3& body, const glm::vec3& light) {
        MeshData mesh;
        addQuad(mesh, {-0.64f, -0.12f}, {0.64f, -0.12f},
                {0.57f, 0.50f}, {-0.57f, 0.50f}, body);
        addQuad(mesh, {-0.59f, 0.24f}, {0.59f, 0.24f},
                {0.57f, 0.50f}, {-0.57f, 0.50f}, light);
        return mesh;
    };

    meshes.releasedPlate = makeButton({0.74f, 0.14f, 0.21f}, {1.0f, 0.43f, 0.46f});
    meshes.pressedPlate = makeButton({0.10f, 0.62f, 0.33f}, {0.41f, 1.0f, 0.58f});
    return meshes;
}

/// Cassa in legno con assi verticali, rinforzo diagonale e chiodi.
MeshData crate()
{
    MeshData mesh;
    const glm::vec3 edge{0.22f, 0.12f, 0.08f};
    const glm::vec3 plank{0.56f, 0.34f, 0.17f};
    const glm::vec3 darkPlank{0.47f, 0.27f, 0.13f};
    const glm::vec3 brace{0.69f, 0.45f, 0.23f};
    const glm::vec3 iron{0.30f, 0.31f, 0.34f};

    addRectangle(mesh, -0.50f, -0.50f, 0.50f, 0.50f, edge);
    for (int i = 0; i < 4; ++i) {
        const float x = -0.39f + 0.20f * i;
        addRectangle(mesh, x, -0.39f, x + 0.18f, 0.39f,
                     i % 2 == 0 ? plank : darkPlank);
    }
    addTriangle(mesh, {-0.40f, -0.39f}, {-0.22f, -0.39f}, {0.40f, 0.39f}, brace);
    addTriangle(mesh, {-0.40f, -0.39f}, {0.40f, 0.39f}, {0.22f, 0.39f}, brace);
    for (float y : {-0.44f, 0.39f}) {
        addRectangle(mesh, -0.45f, y, 0.45f, y + 0.05f, brace);
        for (float x : {-0.39f, 0.35f}) {
            addRectangle(mesh, x, y + 0.01f, x + 0.04f, y + 0.04f, iron);
        }
    }
    return mesh;
}

/// Goblin con orecchie appuntite, tunica consumata e stivali alla base della sagoma.
MeshData goblin()
{
    MeshData mesh;
    const glm::vec3 skin{0.40f, 0.65f, 0.28f};
    const glm::vec3 skinShade{0.24f, 0.43f, 0.19f};
    const glm::vec3 tunic{0.47f, 0.27f, 0.19f};
    const glm::vec3 leather{0.20f, 0.13f, 0.12f};
    const glm::vec3 amber{0.98f, 0.76f, 0.30f};
    const glm::vec3 ivory{0.95f, 0.89f, 0.69f};

    addRectangle(mesh, -0.29f, -0.50f, -0.06f, -0.22f, leather);
    addRectangle(mesh, 0.06f, -0.50f, 0.29f, -0.22f, leather);
    addRectangle(mesh, -0.44f, -0.23f, -0.22f, 0.04f, skinShade);
    addRectangle(mesh, 0.22f, -0.23f, 0.44f, 0.04f, skinShade);
    addRectangle(mesh, -0.30f, -0.33f, 0.30f, 0.13f, tunic);
    addRectangle(mesh, -0.30f, -0.24f, 0.30f, -0.16f, leather);
    addRectangle(mesh, -0.05f, -0.24f, 0.05f, -0.16f, amber);

    addTriangle(mesh, {-0.50f, 0.45f}, {-0.22f, 0.17f}, {-0.22f, 0.35f}, skinShade);
    addTriangle(mesh, {0.50f, 0.45f}, {0.22f, 0.35f}, {0.22f, 0.17f}, skinShade);
    addRectangle(mesh, -0.27f, 0.12f, 0.27f, 0.42f, skin);
    addRectangle(mesh, -0.19f, 0.42f, 0.19f, 0.50f, skin);
    addRectangle(mesh, -0.19f, 0.28f, -0.06f, 0.34f, amber);
    addRectangle(mesh, 0.06f, 0.28f, 0.19f, 0.34f, amber);
    addRectangle(mesh, -0.14f, 0.16f, 0.14f, 0.21f, leather);
    addTriangle(mesh, {-0.12f, 0.17f}, {-0.05f, 0.17f}, {-0.10f, 0.25f}, ivory);
    addTriangle(mesh, {0.05f, 0.17f}, {0.12f, 0.17f}, {0.10f, 0.25f}, ivory);
    return mesh;
}

/// Fantasma con cupola arrotondata, orlo a punte e ombre azzurre.
MeshData ghost()
{
    MeshData mesh;
    const glm::vec3 cloth{0.80f, 0.91f, 0.97f};
    const glm::vec3 shadow{0.42f, 0.61f, 0.76f};
    const glm::vec3 face{0.14f, 0.20f, 0.32f};

    addRectangle(mesh, -0.50f, -0.27f, 0.50f, 0.12f, cloth);
    constexpr float pi = 3.14159265f;
    constexpr int segments = 12;
    for (int i = 0; i < segments; ++i) {
        const float start = pi * i / segments;
        const float end = pi * (i + 1) / segments;
        addTriangle(mesh, {0.0f, 0.12f},
                    {0.50f * std::cos(start), 0.12f + 0.38f * std::sin(start)},
                    {0.50f * std::cos(end), 0.12f + 0.38f * std::sin(end)}, cloth);
    }
    for (int i = 0; i < 4; ++i) {
        const float x = -0.50f + 0.25f * i;
        addTriangle(mesh, {x, -0.27f}, {x + 0.125f, -0.50f}, {x + 0.25f, -0.27f}, shadow);
    }
    addRectangle(mesh, -0.23f, 0.01f, -0.12f, 0.17f, face);
    addRectangle(mesh, 0.12f, 0.01f, 0.23f, 0.17f, face);
    addRectangle(mesh, -0.055f, -0.16f, 0.055f, -0.08f, face);
    return mesh;
}

/// Pannello in muratura con filari sfalsati e fughe incassate.
MeshData wall()
{
    MeshData mesh;
    const glm::vec3 mortar{0.23f, 0.23f, 0.28f};
    const glm::vec3 stone{0.49f, 0.47f, 0.52f};
    const glm::vec3 darkStone{0.40f, 0.39f, 0.45f};
    const glm::vec3 bevel{0.59f, 0.57f, 0.61f};
    addRectangle(mesh, -0.50f, -0.50f, 0.50f, 0.50f, mortar);

    for (int row = 0; row < 4; ++row) {
        const float y = -0.48f + 0.25f * row;
        const bool staggered = row % 2 != 0;
        for (int column = 0; column < (staggered ? 3 : 2); ++column) {
            const float x = -0.50f + 0.50f * column - (staggered ? 0.25f : 0.0f);
            const float left = x < -0.50f ? -0.48f : x + 0.02f;
            const float right = x + 0.50f > 0.50f ? 0.48f : x + 0.48f;
            addRectangle(mesh, left, y, right, y + 0.21f,
                         (row + column) % 2 == 0 ? stone : darkStone);
            addRectangle(mesh, left, y + 0.17f, right, y + 0.21f, bevel);
        }
    }
    return mesh;
}

/// Il materiale del muro viene disegnato sul rettangolo unitario.
MeshData movingWall()
{
    MeshData mesh;
    addRectangle(mesh, -0.5f, -0.5f, 0.5f, 0.5f, {1.0f, 1.0f, 1.0f});
    return mesh;
}

/// Impalcato orizzontale con tavole di legno e due traverse in ferro.
MeshData bridge()
{
    MeshData mesh;
    const glm::vec3 gap{0.22f, 0.14f, 0.12f};
    const glm::vec3 wood{0.61f, 0.40f, 0.22f};
    const glm::vec3 darkWood{0.49f, 0.30f, 0.16f};
    const glm::vec3 iron{0.24f, 0.25f, 0.30f};
    const glm::vec3 rivet{0.60f, 0.61f, 0.65f};
    addRectangle(mesh, -0.50f, -0.50f, 0.50f, 0.50f, gap);

    for (int i = 0; i < 8; ++i) {
        const float x = -0.50f + 0.125f * i;
        addRectangle(mesh, x + 0.008f, -0.50f, x + 0.117f, 0.50f,
                     i % 2 == 0 ? wood : darkWood);
    }
    for (float y : {-0.36f, 0.19f}) {
        addRectangle(mesh, -0.50f, y, 0.50f, y + 0.17f, iron);
        for (int i = 0; i < 8; ++i) {
            const float x = -0.4375f + 0.125f * i;
            addRectangle(mesh, x - 0.012f, y + 0.035f, x + 0.012f, y + 0.135f, rivet);
        }
    }
    return mesh;
}

} // namespace DecorativeShapes
