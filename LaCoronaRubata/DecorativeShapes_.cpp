#include "DecorativeShapes.h"
#include <cmath>
namespace {
/// Aggiunge alla mesh tre vertici con lo stesso colore RGB.
void triangle(MeshData& m, glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 col) {
    for (const auto& v : {a,b,c}) m.vertices.insert(m.vertices.end(), {v.x,v.y,col.r,col.g,col.b});
}
/// Aggiunge un rettangolo di colore uniforme usando due triangoli definiti da angoli opposti.
void rect(MeshData& m, float x0,float y0,float x1,float y1,glm::vec3 col) {
    triangle(m,{x0,y0},{x1,y0},{x1,y1},col);
    triangle(m,{x0,y0},{x1,y1},{x0,y1},col);
}
}
namespace DecorativeShapes {
/// Compone la sagoma del robot del laboratorio. w e h scalano le coordinate locali delle singole
/// parti.
MeshData robot(float w,float h) {
    MeshData m;
    // Corpo, testa, schermo, occhi e piedi: una singola mesh non animata.
    rect(m,-.36f*w,-.20f*h,.36f*w,.22f*h,{.16f,.68f,.76f});
    rect(m,-.43f*w,.13f*h,.43f*w,.46f*h,{.11f,.43f,.63f});
    rect(m,-.33f*w,.20f*h,.33f*w,.37f*h,{.04f,.14f,.27f});
    rect(m,-.23f*w,.26f*h,-.10f*w,.32f*h,{.55f,1.f,.98f});
    rect(m,.10f*w,.26f*h,.23f*w,.32f*h,{.55f,1.f,.98f});
    rect(m,-.40f*w,-.49f*h,-.08f*w,-.32f*h,{.12f,.28f,.42f});
    rect(m,.08f*w,-.49f*h,.40f*w,-.32f*h,{.12f,.28f,.42f});
    rect(m,-.49f*w,-.13f*h,-.37f*w,.13f*h,{.23f,.82f,.86f});
    rect(m,.37f*w,-.13f*h,.49f*w,.13f*h,{.23f,.82f,.86f});
    rect(m,-.05f*w,.46f*h,.05f*w,.55f*h,{.22f,.74f,.85f});
    rect(m,-.10f*w,.52f*h,.10f*w,.57f*h,{1.f,.83f,.30f});
    return m;
}
/// Genera un cristallo sfaccettato attorno all'origine. r determina la distanza delle punte
/// verticali dal centro.
MeshData crystal(float r) {
    MeshData m;
    // Cristallo sfaccettato con sagoma romboidale, centrato nell'origine.
    glm::vec2 top{0.f,r}, left{-.72f*r,.15f*r}, right{.72f*r,.15f*r}, bottom{0.f,-r};
    triangle(m,top,left,{-.13f*r,.10f*r},{.25f,.98f,1.f});
    triangle(m,top,{-.13f*r,.10f*r},right,{.10f,.66f,.96f});
    triangle(m,left,bottom,{-.13f*r,.10f*r},{.06f,.45f,.80f});
    triangle(m,{- .13f*r,.10f*r},bottom,right,{.10f,.78f,.92f});
    triangle(m,{-.28f*r,.25f*r},{-.17f*r,.50f*r},{-.05f*r,.32f*r},{.85f,1.f,1.f});
    return m;
}
}
