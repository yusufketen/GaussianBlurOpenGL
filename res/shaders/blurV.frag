#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform float height;
uniform float sigma;

uniform sampler2D screenTexture;

const int kernel_len = 101;
const int half_len = (kernel_len-1)/2;
float kernel[kernel_len];

float GetGaussianWeight(in float x)
{
	return 0.39894 * exp(-0.5 * x * x / (sigma * sigma)) / sigma;
}

void main()
{
    vec3 cur_pixel = vec3(0.0);
    float weight_sum = 0.0;

    for (int i = 0; i <= half_len; ++i) {
        kernel[half_len+i] = kernel[half_len-i] = GetGaussianWeight(float(i));
    }

    for (int i = 0; i < kernel_len; ++i) {
        weight_sum += kernel[i];
    }

    for (int i = -half_len; i <= half_len; ++i) {
        cur_pixel += kernel[half_len+i] * texture(screenTexture, TexCoords + vec2(0.0,float(i)) / vec2(1, height)).rgb;
    }
    
    FragColor = vec4(cur_pixel / weight_sum, 1.0);
    
}  