#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform float uTime;

const float PLANET_CROSSING_SECONDS = 60.0; // Durata positiva del ciclo, in secondi.
const float PLANET_VERTICAL_AMPLITUDE = 0.05; // Scostamento massimo, in altezze del viewport.
const float PI = 3.14159265358979323846;
const float PLANET_RADIUS = 0.092;
const float PLANET_RING_OUTER_RADIUS = 2.04; // Raggi del disco.

float hash(vec2 point)
{
    return fract(sin(dot(point, vec2(127.1, 311.7))) * 43758.5453);
}

// Ogni cella puo' produrre una stella con posizione e luminosita' stabili.
float stars(vec2 position, float density, float threshold, float pixel)
{
    vec2 grid = position * density;
    vec2 cell = floor(grid);
    float seed = hash(cell);
    vec2 offset = 0.22 + 0.56 * vec2(hash(cell + 17.0), hash(cell + 41.0));
    float distanceToStar = length(fract(grid) - offset) / density;
    float radius = mix(0.0009, 0.0024, hash(cell + 29.0));
    float core = 1.0 - smoothstep(max(0.0, radius - pixel), radius + pixel, distanceToStar);
    float halo = 1.0 - smoothstep(radius, 3.0 * radius, distanceToStar);
    return step(threshold, seed) * 0.78 * (0.78 * core + 0.22 * halo);
}

vec3 drawPlanet(vec3 sky, vec2 position, float pixel)
{
    vec2 point = position / PLANET_RADIUS;
    float distanceToCenter = length(point);
    float edge = pixel / PLANET_RADIUS;
    float globe = 1.0 - smoothstep(1.0 - edge, 1.0 + edge, distanceToCenter);

    vec2 ringSpace = mat2(0.94, 0.34, -0.34, 0.94) * point;
    float ringRadius = length(ringSpace * vec2(1.0, 3.4));
    float ring = smoothstep(1.24, 1.35, ringRadius) *
                 (1.0 - smoothstep(1.90, PLANET_RING_OUTER_RADIUS, ringRadius));
    ring *= 0.48 + 0.15 * sin(ringRadius * 34.0);
    const vec3 ringColor = vec3(0.56, 0.55, 0.71);

    // Disegna prima l'anello dietro al disco, poi la meta' che passa davanti.
    vec3 color = mix(sky, ringColor, ring);
    vec3 normal = vec3(point, sqrt(max(0.0, 1.0 - dot(point, point))));
    const vec3 light = vec3(-0.58, 0.52, 0.63);
    float daylight = smoothstep(-0.32, 0.68, dot(normal, light));
    float band = 0.5 + 0.5 * sin(ringSpace.y * 14.0 - normal.z * 2.0);
    vec3 surface = mix(vec3(0.30, 0.37, 0.53), vec3(0.65, 0.49, 0.62), band);
    color = mix(color, surface * (0.25 + 0.65 * daylight), globe);
    float nearSide = 1.0 - smoothstep(-edge, edge, ringSpace.y);
    return mix(color, ringColor, ring * globe * nearSide);
}

void main()
{
    // Le coordinate corrette per il viewport mantengono tondo il pianeta.
    float pixel = max(length(dFdy(vUV)), 0.00001);
    float aspect = pixel / max(length(dFdx(vUV)), 0.00001);
    vec2 position = vec2((vUV.x - 0.5) * aspect, vUV.y);

    vec3 sky = mix(vec3(0.19, 0.15, 0.28), vec3(0.032, 0.048, 0.12),
                   smoothstep(0.08, 0.92, position.y));
    float dustLine = 0.54 + position.x * 0.25 + 0.055 * sin(position.x * 5.0);
    float dustDistance = (position.y - dustLine) / 0.19;
    float dust = exp(-dustDistance * dustDistance);
    sky += dust * vec3(0.058, 0.056, 0.13);

    float farStars = stars(position, 46.0, 0.962, pixel);
    float nearStars = stars(position + vec2(1.7, -2.4), 21.0, 0.953, pixel);
    sky += vec3(0.46, 0.59, 0.83) * farStars * 0.48;
    sky += vec3(0.95, 0.84, 0.72) * nearStars * 0.68;
    float phase = fract(uTime / PLANET_CROSSING_SECONDS);
    // Il margine contiene anche gli anelli inclinati, piu' un pixel di sicurezza.
    float margin = PLANET_RADIUS * PLANET_RING_OUTER_RADIUS + pixel;
    vec2 planetCenter = vec2(mix(-0.5 * aspect - margin, 0.5 * aspect + margin, phase),
                             0.77 + PLANET_VERTICAL_AMPLITUDE * sin(2.0 * PI * phase));
    sky = drawPlanet(sky, position - planetCenter, pixel);

    // Due profili bassi danno profondita' al paesaggio dietro al piano di gioco.
    float distantRidge = 0.11 + 0.045 * sin(position.x * 5.2 + 0.7)
                             + 0.022 * sin(position.x * 10.5);
    float nearRidge = 0.058 + 0.025 * sin(position.x * 4.7 - 1.1)
                            + 0.012 * sin(position.x * 12.0 + 0.4);
    sky = mix(sky, vec3(0.10, 0.091, 0.19),
              1.0 - smoothstep(distantRidge - pixel, distantRidge + pixel, position.y));
    sky = mix(sky, vec3(0.053, 0.06, 0.126),
              1.0 - smoothstep(nearRidge - pixel, nearRidge + pixel, position.y));

    FragColor = vec4(sky, 1.0);
}
