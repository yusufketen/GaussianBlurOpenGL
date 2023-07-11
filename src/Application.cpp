#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Blur.h"
#include "Renderer/Texture.h"
#include "Window.h"
#include "Renderer/Image.h"

#define WIDTH 1024
#define HEIGHT 768

char location_buffer[1024];
bool is_blur_active = true;
bool is_vsync_active = true;
float sigma = 7.0f;

const std::string imagePathPrefix = "res/images/";
std::string currentImage = "city.jpg";
Image* img = nullptr;

void SaveScreen(const char* filepath, unsigned int product_fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, product_fbo);

    GLsizei channel_count = 3;
    GLsizei stride = channel_count * WIDTH;
    stride += (stride % 4) ? (4 - stride % 4) : 0;

    GLubyte* pixels = (GLubyte*)malloc(WIDTH * HEIGHT * sizeof(GLubyte) * 4);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    stbi_flip_vertically_on_write(true);
    stbi_write_jpg(filepath, WIDTH, HEIGHT, channel_count, pixels, stride);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ImGuiInit(Window* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiRender(unsigned int product_fbo)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Menu");

    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::Checkbox("VSync", &is_vsync_active);

    const char* items[] = { "ankara.jpg", "city.jpg", "container.jpg" };
    static std::string item_current = items[0];
    if (ImGui::BeginListBox("Cities"))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current == items[n]);
            if (ImGui::Selectable(items[n], is_selected))
                item_current = items[n];

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();

            if (item_current == currentImage)
				continue;
            else
            {
                delete img;
                img = new Image(imagePathPrefix + item_current);
                currentImage = item_current;
            }
        }
        ImGui::EndListBox();
    }

    ImGui::InputFloat("Sigma", &sigma, 0.25f, 1.0f, "%.2f");
    ImGui::Checkbox("Gaussian Blur", &is_blur_active);

    if (is_blur_active)
    {
        if (ImGui::Button("Save Image")) {
            SaveScreen("Result.jpg", product_fbo);
            printf("Screen is saved!\n");
        }
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main()
{
    Window window{ "Image Gaussian Blur", WIDTH, HEIGHT, false };
    img = new Image(imagePathPrefix + currentImage);
    Blur blur{ WIDTH, HEIGHT };

    ImGuiInit(&window);

    while (!window.ShouldClose())
    {
        if (window.IsVsync() != is_vsync_active)
            window.SetVsync(is_vsync_active);

        glClearColor(0.7f, 0.3f, 0.2f, 0.8f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!is_blur_active)
        {
            img->Draw();
        } else 
        {
            blur.SetSigma(sigma);
            blur.DrawHorizontal();
            // Draw Image to Horizontal FBO and pass horizontally
        	glBindTexture(GL_TEXTURE_2D, img->m_texture.GetRendererID());
            glBindVertexArray(img->m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glUseProgram(0);

            // Second pass of Gaussian Blur - Horizontal Pass is done above
        	blur.DrawVertical();
            glBindVertexArray(img->m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Draw processed image to screen from vertical blur framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            img->m_shader.Bind();
            glBindVertexArray(blur.GetVAO());
            glBindTexture(GL_TEXTURE_2D, blur.GetFinalTextureID());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            img->m_shader.Unbind();

        }

        ImGuiRender(blur.GetFinalFramebufferID());
        window.Update();
    }

    if (img != nullptr)
		delete img;
}
