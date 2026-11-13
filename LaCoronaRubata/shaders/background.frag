#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform float uTime;

float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float rectangle(vec2 p, vec2 halfSize)
{
    vec2 edge = abs(p) - halfSize;
    return 1.0 - smoothstep(0.0, 0.002, max(edge.x, edge.y));
}

// Contorno con base piana, lati verticali e sommità arrotondata.
float arch(vec2 p, float radius, float halfHeight)
{
    vec2 edge = vec2(abs(p.x), max(p.y - halfHeight, 0.0));
    return max(length(edge) - radius, -halfHeight - p.y);
}

vec3 drawWindow(vec3 wall, vec2 p)
{
    float opening = arch(p, 0.073, 0.105);
    float frame = arch(p, 0.091, 0.124);
    vec3 stone = mix(vec3(0.11, 0.12, 0.16), vec3(0.30, 0.30, 0.34),
                     smoothstep(-0.12, 0.17, p.y));
    wall = mix(wall, stone, 1.0 - smoothstep(0.0, 0.003, frame));

    vec3 glass = mix(vec3(0.095, 0.17, 0.25), vec3(0.15, 0.28, 0.39),
                     smoothstep(-0.10, 0.17, p.y));
    float bars = max(rectangle(p, vec2(0.004, 0.22)),
                     rectangle(p - vec2(0.0, 0.035), vec2(0.09, 0.004)));
    glass = mix(glass, vec3(0.055, 0.065, 0.09), bars);
    wall = mix(wall, glass, 1.0 - smoothstep(0.0, 0.002, opening));
    float sill = rectangle(p + vec2(0.0, 0.122), vec2(0.101, 0.012));
    return mix(wall, vec3(0.29, 0.29, 0.33), sill);
}

vec3 drawTorch(vec3 wall, vec2 p, float phase)
{
    float sway = sin(phase * 6.0) * 0.004 + sin(phase * 11.0) * 0.002;
    vec2 flame = (p - vec2(sway, 0.038)) / vec2(0.021, 0.041);
    flame.x /= mix(1.0, 0.42, smoothstep(-0.2, 1.0, flame.y));
    float shape = length(flame);
    float outer = 1.0 - smoothstep(0.89, 1.04, shape);
    float inner = 1.0 - smoothstep(0.22, 0.77, shape);

    float bracket = rectangle(p + vec2(0.0, 0.056), vec2(0.009, 0.05));
    float bowl = rectangle(p, vec2(0.024, 0.009));
    wall = mix(wall, vec3(0.09, 0.065, 0.055), max(bracket, bowl));
    vec3 fire = mix(vec3(1.0, 0.38, 0.075), vec3(1.0, 0.87, 0.47), inner);
    return mix(wall, fire, outer);
}

float torchLight(vec2 p, float phase)
{
    vec2 offset = p - vec2(0.0, 0.038);
    float flicker = 0.90 + 0.06 * sin(phase * 7.0) + 0.04 * sin(phase * 13.0);
    return exp(-11.0 * dot(offset, offset)) * flicker;
}

void main()
{
    // La correzione delle proporzioni mantiene gli archi rotondi.
    float aspect = length(dFdy(vUV)) / length(dFdx(vUV));
    vec2 p = vec2((vUV.x - 0.5) * aspect, vUV.y);
    vec2 grid = p * vec2(8.0, 14.0);
    grid.x += 0.5 * mod(floor(grid.y), 2.0);

    vec2 cell = fract(grid);
    vec2 edges = min(cell, 1.0 - cell);
    float stoneMask = smoothstep(0.025, 0.055, min(edges.x, edges.y));
    float stoneShade = (hash(floor(grid)) - 0.5) * 0.075;
    float grain = (hash(floor(p * 430.0)) - 0.5) * 0.025;
    float bevel = smoothstep(0.85, 0.97, cell.y) * 0.025
                - (1.0 - smoothstep(0.03, 0.14, cell.y)) * 0.035;
    vec3 mortar = vec3(0.10, 0.10, 0.13);
    vec3 stone = vec3(0.25, 0.24, 0.28) + stoneShade + grain + bevel;
    vec3 color = mix(mortar, stone, stoneMask);
    color *= 0.72 + 0.28 * (1.0 - smoothstep(0.18, 0.68, length(vUV - 0.5)));

    color = drawWindow(color, p - vec2(-0.28 * aspect, 0.69));
    color = drawWindow(color, p - vec2( 0.28 * aspect, 0.69));

    vec2 leftTorch = p - vec2(-0.15 * aspect, 0.48);
    vec2 rightTorch = p - vec2(0.15 * aspect, 0.48);
    float light = torchLight(leftTorch, uTime) + torchLight(rightTorch, uTime + 2.3);
    color += vec3(0.21, 0.105, 0.033) * light;
    color = drawTorch(color, leftTorch, uTime);
    color = drawTorch(color, rightTorch, uTime + 2.3);
    FragColor = vec4(color, 1.0);
}
