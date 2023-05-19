#include "forward-renderer.hpp"
#include "../common/components/mesh-renderer.hpp"
#include "../common/components/movement.hpp"
#include "../components/collision.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include "./chicken-renderer.hpp"
#include "./hearts-renderer.hpp"
#include "./monkeys-renderer.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

namespace our
{
// int counter = 0;
// int monkeysFrames = 0;
// int heartsFrames = 0;
// int zCounter = 0;
// int zCounterMonkeys = 0;
// int zCounterHearts = 0;
// int timeToIncrease = 0;
// int zCounter = 0;

// ChickenRenderer *chicken_renderer = new ChickenRenderer();
MonkeyRenderer *monkey_renderer = new MonkeyRenderer();
HeartRenderer *heart_renderer = new HeartRenderer();
// double generateRandomNumber(double minX, double maxX)
// {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(minX, maxX);
//     return dis(gen);
// }

void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
{
    // First, we store the window size for later use
    this->windowSize = windowSize;

    // Then we check if there is a sky texture in the configuration
    if (config.contains("sky"))
    {
        // First, we create a sphere which will be used to draw the sky
        this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

        // We can draw the sky using the same shader used to draw textured objects
        ShaderProgram *skyShader = new ShaderProgram();
        skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        skyShader->link();

        // TODO: (Req 10) Pick the correct pipeline state to draw the sky
        //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion
        //  should we pick? We will draw the sphere from the inside, so what options should we pick for the face
        //  culling.
        PipelineState skyPipelineState{};
        skyPipelineState.depthTesting.enabled = true;       // Enabled depthTesting as sky would be drawn after opaque
        skyPipelineState.depthTesting.function = GL_LEQUAL; // GL_LEQUAL because object would be drawn from near to far

        skyPipelineState.faceCulling.enabled = true;        // Enabled faceCulling so we can draw the sphere from inside
        skyPipelineState.faceCulling.culledFace = GL_FRONT; // GL_FRONT because culling the Front of sphere

        // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while
        // rendering the sky)
        std::string skyTextureFile = config.value<std::string>("sky", "");
        Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

        // Setup a sampler for the sky
        Sampler *skySampler = new Sampler();
        skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
        skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Combine all the aforementioned objects (except the mesh) into a material
        this->skyMaterial = new TexturedMaterial();
        this->skyMaterial->shader = skyShader;
        this->skyMaterial->texture = skyTexture;
        this->skyMaterial->sampler = skySampler;
        this->skyMaterial->pipelineState = skyPipelineState;
        this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        this->skyMaterial->alphaThreshold = 1.0f;
        this->skyMaterial->transparent = false;
    }

    // Then we check if there is a postprocessing shader in the configuration
    if (config.contains("postprocess"))
    {
        // TODO: (Req 11) Create a framebuffer
        glGenFramebuffers(1, &postprocessFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

        // TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
        //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
        //  The depth format can be (Depth component with 24 bits).
        colorTarget = texture_utils::empty(GL_RGBA8, windowSize);
        depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);

        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(),
                               0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(),
                               0);
        // TODO: (Req 11) Unbind the framebuffer just to be safe
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // Create a vertex array to use for drawing the texture
        glGenVertexArrays(1, &postProcessVertexArray);

        // Create a sampler to use for sampling the scene texture in the post processing shader
        Sampler *postprocessSampler = new Sampler();
        postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        for (const auto &shader : config["postprocess"])
        {

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(shader.get<std::string>(), GL_FRAGMENT_SHADER);
            postprocessShader->link();
            postprocessShadersList.push_back(postprocessShader);
        }
        if ((int)postprocessShadersList.size() > 0)
        {
            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShadersList[0];
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }
}
void ForwardRenderer::changePostprocessShader(int index)
{
    if (index < postprocessShadersList.size())
        postprocessMaterial->shader = postprocessShadersList[index];
}

void ForwardRenderer::destroy()
{
    // Delete all objects related to the sky
    if (skyMaterial)
    {
        delete skySphere;
        delete skyMaterial->shader;
        delete skyMaterial->texture;
        delete skyMaterial->sampler;
        delete skyMaterial;
    }
    // Delete all objects related to post processing
    if (postprocessMaterial)
    {
        glDeleteFramebuffers(1, &postprocessFrameBuffer);
        glDeleteVertexArrays(1, &postProcessVertexArray);
        delete colorTarget;
        delete depthTarget;
        delete postprocessMaterial->sampler;
        delete postprocessMaterial->shader;
        delete postprocessMaterial;
        postprocessShadersList.clear();
    }
}

void ForwardRenderer::render(World *world)
{
    // counter++;
    // monkeysFrames++;
    // heartsFrames++;
    // if (counter >= 20)
    // {

    //     timeToIncrease++;
    //     counter = 0;
    //     if (level_state == true)
    //     {
    //         chicken_renderer->delete_all_chickens(world);
    //     }
    // chicken_renderer->rendering(world);
    //     chicken_renderer->delete_chickens(world);
    //     // chicken_renderer->printing();
    // }
    // chicken_renderer->delete_chickens(world);
    // if (monkeysFrames >= 700)
    // {
    //     zCounterMonkeys = 5;
    //     monkey_renderer->rendering(world, zCounterMonkeys);
    //     // monkey_renderer->printing();
    //     monkeysFrames = 0;
    // }
    // if (heartsFrames >= 500)
    // {
    //     zCounterHearts = 5;
    //     heart_renderer->rendering(world, zCounterHearts);
    //     // monkey_renderer->printing();
    //     heartsFrames = 0;
    // }

    // First of all, we search for a camera and for all the mesh renderers
    CameraComponent *camera = nullptr;
    opaqueCommands.clear();
    transparentCommands.clear();

    lightCommands.clear();
    for (auto entity : world->getEntities())
    {

        // If we hadn't found a camera yet, we look for a camera in this entity
        if (!camera)
            camera = entity->getComponent<CameraComponent>();
        // If this entity has a mesh renderer component
        if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
        {
            // We construct a command from it
            RenderCommand command;
            command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
            command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
            command.mesh = meshRenderer->mesh;
            meshRenderer->mesh->draw();
            command.material = meshRenderer->material;
            // if it is transparent, we add it to the transparent commands list
            if (command.material->transparent)
            {
                transparentCommands.push_back(command);
            }
            else if (command.material->affectedByLight)
            {
                // there is no command for the light source but those commands for all objects that will be affected by
                // the light source so we store their data in RenderCommmad and then lightCommands list will be used to
                // draw those objects
                lightCommands.push_back(command);
            }
            else
            {
                // Otherwise, we add it to the opaque command list
                opaqueCommands.push_back(command);
            }
        }
    }

    // If there is no camera, we return (we cannot render without a camera)
    if (camera == nullptr)
        return;

    // TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward
    // direction
    //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
    //  glm::vec3 cameraForward = glm::vec3(0.0, 0.0, -1.0f);
    glm::vec3 cameraForward = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0, 0.0, -1.0f, 0.0f);
    std::sort(transparentCommands.begin(), transparentCommands.end(),
              [cameraForward](const RenderCommand &first, const RenderCommand &second) {
                  // TODO: (Req 9) Finish this function
                  //  HINT: the following return should return true "first" should be drawn before "second".

                  // Get the projection of objects to find which object is further from the camera.
                  // The object with the higher value of projection is the one farthest and thus should be drawn first.
                  return glm::dot(cameraForward, first.center) > glm::dot(cameraForward, second.center);
              });

    // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
    //  to get the VP matrix, we multiply the projection and view matrices in order
    glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();

    // TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
    //  This decides which part of the screen to draw in.
    //  The first two parameters set the location of the lower left corner of the window.
    //  The third and fourth parameters set the width and height of the rendering window in pixel.
    glViewport(0, 0, windowSize.x, windowSize.y);

    // TODO: (Req 9) Set the clear color to black and the clear depth to 1
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    // TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the
    // framebuffer)
    glColorMask(true, true, true, true);
    glDepthMask(true);

    // If there is a postprocess material, bind the framebuffer
    if (postprocessMaterial)
    {
        // TODO: (Req 11) bind the framebuffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
    }

    // TODO: (Req 9) Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: (Req 9) Draw all the opaque commands
    //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
    for (auto &command : opaqueCommands)
    {
        command.material->setup();
        // Model matrix is the transformation matrix from local space to world space.
        // To obtain MVP we need to multiply the model matrix from the left (from the right in code)
        glm::mat4 transform = VP * command.localToWorld;
        command.material->shader->set("transform", transform);
        command.mesh->draw();
    }

    //-------------------------------------------
    //-----------Support for light---------------
    //-------------------------------------------
    int light_count = world->light_count;
    Light *lights = world->lights;
    // std::cout << "light objects" << lightCommands.size() << std::endl;
    for (auto &command : lightCommands)
    {
        command.material->setup();

        glm::mat4 M = command.localToWorld;
        glm::mat4 M_IT = glm::transpose(glm::inverse(M));
        glm::vec3 eye = camera->getOwner()->localTransform.position;
        glm::vec3 sky_top = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 sky_horizon = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 sky_bottom = glm::vec3(1.0f, 1.0f, 1.0f);

        // float time = (float)glfwGetTime();
        // float sky_r = 0.5f + 0.5f * sin(time);
        // float sky_g = 0.5f + 0.5f * sin(time + 2.0f);
        // float sky_b = 0.5f + 0.5f * sin(time + 4.0f);

        // glm::vec3 sky_top = glm::vec3(sky_r, sky_g, sky_b);
        // glm::vec3 sky_horizon = glm::vec3(0.5f, 0.5f, 0.5f);
        // glm::vec3 sky_bottom = glm::vec3(sky_r, sky_g, sky_b);

        command.material->shader->set("M", M);
        command.material->shader->set("VP", VP);
        command.material->shader->set("M_IT", M_IT);
        command.material->shader->set("eye", eye);
        command.material->shader->set("sky.top", sky_top);
        command.material->shader->set("sky.horizon", sky_horizon);
        command.material->shader->set("sky.bottom", sky_bottom);
        command.material->shader->set("light_count", light_count);
        for (int i = 0; i < light_count; i++)
        {
            command.material->shader->set("lights[" + std::to_string(i) + "].type", lights[i].kind);
            command.material->shader->set("lights[" + std::to_string(i) + "].position", lights[i].position);
            command.material->shader->set("lights[" + std::to_string(i) + "].color", lights[i].color);
            command.material->shader->set("lights[" + std::to_string(i) + "].attenuation", lights[i].attenuation);
            command.material->shader->set("lights[" + std::to_string(i) + "].direction", lights[i].direction);
            command.material->shader->set("lights[" + std::to_string(i) + "].cone_angles", lights[i].cone_angles);
        }
        command.mesh->draw();
        // std::cout<<"light command end"<<std::endl;
    }
    //-------------------------------------------
    //-------------------------------------------

    // If there is a sky material, draw the sky
    if (this->skyMaterial)
    {
        // TODO: (Req 10) setup the sky material
        this->skyMaterial->setup();

        // TODO: (Req 10) Get the camera position
        glm::vec3 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        // TODO: (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center =
        // camera position) We need to translate the sky sphere center to the camera position Set the sky sphere center
        // as a 4x4 matrix of ones
        glm::mat4 identity(1.0f);
        // translating shpere position to camera position
        // Translate this center to the camera position such that it equals it & store
        // the resulting transformed matrix in modelMatrix
        glm::mat4 M = glm::translate(identity, cameraPosition);

        // TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
        //  We can acheive the is by multiplying by an extra matrix after the projection but what values should we put
        //  in it?
        // We can acheive this by multiplying by an extra matrix after the projection but what values should we put in
        // it? For the sky to be drawn behind, we need to modify the z-index. How? Simply the 3rd column of our
        // generated alwaysBehindTransform matrix will set the z-coordinate as w. Then after normalization, when the
        // values of the matrix are divided by w, we will obtain the z which was previously set to w as 1. Our goal is
        // achieved. This will ensure that the sky is the very first thing drawn behind and set the 4th column to
        // 0,0,0,1 to keep the w-coordinate unchanged
        glm::mat4 alwaysBehindTransform =
            glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // set z=0
                      0.0f, 0.0f, 1.0f, 1.0f                                                  // set z=1
            );
        // TODO: (Req 10) set the "transform" uniform
        // We need to get the view Matrix and Projection Matrix
        // Following TRS sequence of matrices, we will get the VP matrix as we did before from the projection and view
        // matrices and multiply it by the model matrix to get the transformation matrix Finally we set it to the
        // "transform" uniform variable in the shader
        glm::mat4 transform = alwaysBehindTransform * VP * M;
        skyMaterial->shader->set("transform", transform);

        // TODO: (Req 10) draw the sky sphere
        skySphere->draw();
    }
    // TODO: (Req 9) Draw all the transparent commands
    //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
    for (auto &command : transparentCommands)
    {
        command.material->setup();
        // Model matrix is the transformation matrix from local space to world space.
        // To obtain MVP we need to multiply the model matrix from the left (from the right in code)
        glm::mat4 transform = VP * command.localToWorld;
        command.material->shader->set("transform", transform);
        command.mesh->draw();
    }

    // If there is a postprocess material, apply postprocessing
    if (postprocessMaterial)
    {
        // TODO: (Req 11) Return to the default framebuffer
        // Bind Frame buffer as default (0) using glBindFramebuffer openGL function
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
        // Bind the Vertex Array
        glBindVertexArray(postProcessVertexArray);
        // Setup the post process material
        postprocessMaterial->setup();
        // Drawing the new triangles after postProcessing of the image
        // By setting the mode as GL_TRIANGLES
        // The first argument to 0 since we want to start from the beginning
        // The count is set to 3
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

} // namespace our