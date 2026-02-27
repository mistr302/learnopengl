#version 460 core
#ifdef GL_ES
precision mediump float;
#endif

// uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_zoom;
in vec2 uv;
out vec4 FragColor;

vec3 palette(float t) {
    return vec3(
        0.5 + 0.5*cos(6.28318*(t + 0.0)),
        0.5 + 0.5*cos(6.28318*(t + 0.33)),
        0.5 + 0.5*cos(6.28318*(t + 0.67))
    );
}

void main() {
    // Adjust aspect ratio
    // uv.x *= u_resolution.x / u_resolution.y;

    // float rad = u_time/2.0;
    // mat2 rot = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    // vec2 pos = uv * rot;
    
    vec2 c = uv/u_zoom.z+u_zoom.xy;      // constant per pixel
    // vec2 c = uv;      // constant per pixel
    vec2 z = vec2(0.0); // start at 0 for Mandelbrot
    // int maxIter = int(100.0 * exp2(u_zoom.z));
    // maxIter = clamp(maxIter, 50, 5000);;
    int maxIter = 100;
    int i;
    for (i = 0; i < maxIter; i++) {
        // z = z² + c
        z = vec2(
            z.x*z.x - z.y*z.y,
            2.0*z.x*z.y
        ) + c;

        // Escape condition (|z| > 2)
        if (dot(z, z) > 4.0) break;
    }
    float s = float(i) - log2(log(length(z)));
    float t = s / float(maxIter);
    FragColor = vec4(palette(t), 1.0);
}
