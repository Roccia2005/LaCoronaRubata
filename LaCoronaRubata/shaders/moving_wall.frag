#version 330 core
in vec2 localPosition;
flat in vec2 wallSize;
uniform float uMoving;
out vec4 fragColor;

float segmentDistance(vec2 p, vec2 a, vec2 b)
{
    vec2 ab = b - a;
    float t = clamp(dot(p - a, ab) / dot(ab, ab), 0.0, 1.0);
    return length(p - (a + t * ab));
}

void main()
{
    vec2 extent = wallSize / min(wallSize.x, wallSize.y);
    vec2 p = localPosition * extent;
    vec2 inset = 0.5 * extent - abs(p);
    float edge = min(inset.x, inset.y);
    float aa = 0.5 * max(fwidth(p.x), fwidth(p.y));
    vec3 frame = vec3(0.40, 0.43, 0.57);
    if (inset.y < inset.x && inset.y < 0.085)
        frame += vec3(p.y > 0.0 ? 0.16 : -0.12);
    vec3 metal = mix(vec3(0.12, 0.11, 0.19), frame,
                     smoothstep(0.035-aa, 0.035+aa, edge));

    vec2 q = wallSize.x >= wallSize.y ? p : vec2(p.y, -p.x);
    float end = 0.5 * max(extent.x, extent.y) - 0.14;
    vec2 a = vec2(0.0, -0.10), b = vec2(0.0, 0.10);
    float d = min(segmentDistance(q, vec2(-end, a.y), a),
              min(segmentDistance(q, a, b),
                  segmentDistance(q, b, vec2(end, b.y))));
    vec3 panel = mix(vec3(0.09, 0.08, 0.12), vec3(0.23, 0.25, 0.36),
                     smoothstep(0.085-aa, 0.085+aa, d));
    vec3 wire = mix(vec3(0.65, 0.36, 0.13), vec3(1.0, 0.72, 0.16), uMoving);
    wire = mix(wire, vec3(1.0, 0.94, 0.66),
               uMoving * (1.0 - smoothstep(0.018-aa, 0.018+aa, d)));
    panel = mix(wire, panel, smoothstep(0.05-aa, 0.05+aa, d));
    fragColor = vec4(mix(metal, panel, smoothstep(0.14-aa, 0.14+aa, edge)), 1.0);
}
