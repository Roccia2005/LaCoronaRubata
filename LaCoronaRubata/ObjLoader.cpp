#include "ObjLoader.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <charconv>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace {
using Palette = std::unordered_map<std::string, glm::vec3>;

/// Legge dal materiale MTL il colore diffuso usato dallo shader 2D.
void loadMaterials(const std::filesystem::path& path, Palette& palette)
{
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Materiale non trovato: " + path.string());
    std::string line, name;
    while (std::getline(file, line)) {
        std::istringstream row(line);
        std::string tag;
        row >> tag;
        if (tag == "newmtl") {
            row >> name;
        } else if (tag == "Kd" && !name.empty()) {
            glm::vec3 rgb;
            if (!(row >> rgb.r >> rgb.g >> rgb.b))
                throw std::runtime_error("Colore Kd non valido in " + path.string());
            palette[name] = rgb;
        }
    }
}

/// Risolve l'indice di posizione OBJ, anche relativo o seguito da /uv/normale.
std::size_t positionIndex(const std::string& token, std::size_t count)
{
    const char* begin = token.data();
    const char* end = begin + std::min(token.size(), token.find('/'));
    std::ptrdiff_t index = 0;
    const auto parsed = std::from_chars(begin, end, index);
    if (parsed.ec != std::errc{} || parsed.ptr != end || index == 0)
        throw std::runtime_error("Indice OBJ non valido: " + token);
    index = index > 0 ? index - 1 : static_cast<std::ptrdiff_t>(count) + index;
    if (index < 0 || static_cast<std::size_t>(index) >= count)
        throw std::runtime_error("Indice OBJ fuori intervallo: " + token);
    return static_cast<std::size_t>(index);
}
}

/// Converte le facce in triangoli con vertici x, y, r, g, b, come richiesto da MeshData.
MeshData ObjLoader::load(const std::filesystem::path& path)
{
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Modello non trovato: " + path.string());
    MeshData mesh;
    std::vector<glm::vec2> positions;
    Palette palette;
    glm::vec3 color(1.0f);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream row(line);
        std::string tag;
        row >> tag;
        if (tag == "v") {
            glm::vec2 point;
            if (!(row >> point.x >> point.y))
                throw std::runtime_error("Vertice non valido in " + path.string());
            positions.push_back(point);
        } else if (tag == "mtllib") {
            std::string name;
            while (row >> name && name.front() != '#')
                loadMaterials(path.parent_path() / name, palette);
        } else if (tag == "usemtl") {
            std::string name;
            row >> name;
            const auto material = palette.find(name);
            if (material == palette.end())
                throw std::runtime_error("Materiale " + name + " sconosciuto in " + path.string());
            color = material->second;
        } else if (tag == "f") {
            std::vector<std::size_t> face;
            std::string token;
            while (row >> token && token.front() != '#')
                face.push_back(positionIndex(token, positions.size()));
            for (std::size_t i = 2; i < face.size(); ++i) {
                for (const auto index : {face.front(), face[i - 1], face[i]}) {
                    const auto& p = positions[index];
                    mesh.vertices.insert(mesh.vertices.end(), {p.x, p.y, color.r, color.g, color.b});
                }
            }
        }
    }
    if (mesh.vertices.empty()) throw std::runtime_error("Modello senza triangoli: " + path.string());

    // Le coordinate dell'editor possono essere decentrate. Una scala uniforme conserva le proporzioni.
    auto& vertices = mesh.vertices;
    glm::vec2 low(vertices[0], vertices[1]), high = low;
    for (std::size_t i = 0; i < vertices.size(); i += 5) {
        const glm::vec2 point(vertices[i], vertices[i + 1]);
        low = glm::min(low, point);
        high = glm::max(high, point);
    }
    const float extent = std::max(high.x - low.x, high.y - low.y);
    if (extent <= 0.0f) throw std::runtime_error("Modello senza estensione: " + path.string());
    const glm::vec2 center = (low + high) * 0.5f;
    for (std::size_t i = 0; i < vertices.size(); i += 5) {
        vertices[i] = (vertices[i] - center.x) / extent;
        vertices[i + 1] = (vertices[i + 1] - center.y) / extent;
    }
    return mesh;
}
