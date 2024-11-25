#include "Labs/3-Rendering/tasks.h"

namespace VCX::Labs::Rendering {

    glm::vec4 GetTexture(Engine::Texture2D<Engine::Formats::RGBA8> const & texture, glm::vec2 const & uvCoord) {
        if (texture.GetSizeX() == 1 || texture.GetSizeY() == 1) return texture.At(0, 0);
        glm::vec2 uv      = glm::fract(uvCoord);
        uv.x              = uv.x * texture.GetSizeX() - .5f;
        uv.y              = uv.y * texture.GetSizeY() - .5f;
        std::size_t xmin  = std::size_t(glm::floor(uv.x) + texture.GetSizeX()) % texture.GetSizeX();
        std::size_t ymin  = std::size_t(glm::floor(uv.y) + texture.GetSizeY()) % texture.GetSizeY();
        std::size_t xmax  = (xmin + 1) % texture.GetSizeX();
        std::size_t ymax  = (ymin + 1) % texture.GetSizeY();
        float       xfrac = glm::fract(uv.x), yfrac = glm::fract(uv.y);
        return glm::mix(glm::mix(texture.At(xmin, ymin), texture.At(xmin, ymax), yfrac), glm::mix(texture.At(xmax, ymin), texture.At(xmax, ymax), yfrac), xfrac);
    }

    glm::vec4 GetAlbedo(Engine::Material const & material, glm::vec2 const & uvCoord) {
        glm::vec4 albedo       = GetTexture(material.Albedo, uvCoord);
        glm::vec3 diffuseColor = albedo;
        return glm::vec4(glm::pow(diffuseColor, glm::vec3(2.2)), albedo.w);
    }

    /******************* 1. Ray-triangle intersection *****************/
    bool IntersectTriangle(Intersection & output, Ray const & ray, glm::vec3 const & p1, glm::vec3 const & p2, glm::vec3 const & p3) {
        // your code here
        glm::vec3 D = ray.Direction, E1 = p2 - p1, E2 = p3 - p1, T = ray.Origin - p1;
        glm::vec3 P = glm::cross(D, E2), Q = glm::cross(T, E1);
        float div = glm::dot(P, E1);
        if(fabs(div) < 1e-4) return false;
        output.t = glm::dot(Q, E2)  / div;
        output.u = glm::dot(P, T)   / div;
        output.v = glm::dot(Q, D)   / div;
        if(output.t < 0 || output.u < 0 || output.v < 0 || output.u + output.v > 1) return false;
        return true;
    }

    glm::vec3 RayTrace(const RayIntersector & intersector, Ray ray, int maxDepth, bool enableShadow) {
        glm::vec3 color(0.0f);
        glm::vec3 weight(1.0f);

        for (int depth = 0; depth < maxDepth; depth++) {
            auto rayHit = intersector.IntersectRay(ray);
            if (! rayHit.IntersectState) return color;
            const glm::vec3 pos       = rayHit.IntersectPosition;
            const glm::vec3 n         = rayHit.IntersectNormal;
            const glm::vec3 kd        = rayHit.IntersectAlbedo;
            const glm::vec3 ks        = rayHit.IntersectMetaSpec;
            const float     alpha     = rayHit.IntersectAlbedo.w;
            const float     shininess = rayHit.IntersectMetaSpec.w * 256;

            glm::vec3 result(0.0f);
            /******************* 2. Whitted-style ray tracing *****************/
            // your code here

            for (const Engine::Light & light : intersector.InternalScene->Lights) {
                glm::vec3 l;
                float     attenuation;
                /******************* 3. Shadow ray *****************/
                if (light.Type == Engine::LightType::Point) {
                    l           = light.Position - pos;
                    attenuation = 1.0f / glm::dot(l, l);
                    if (enableShadow) {
                        // your code here
                        auto hit = intersector.IntersectRay(Ray(pos, glm::normalize(l)));
                        while (hit.IntersectState && hit.IntersectAlbedo.w < 0.2) {
                            hit = intersector.IntersectRay(Ray(hit.IntersectPosition, glm::normalize(l)));
                        }
                        if (hit.IntersectState) {
                            glm::vec3 p = hit.IntersectPosition - pos;
                            if(glm::dot(p, p) < glm::dot(l, l)) attenuation = 0;
                        }
                    }
                } else if (light.Type == Engine::LightType::Directional) {
                    l           = light.Direction;
                    attenuation = 1.0f;
                    if (enableShadow) {
                        // your code here
                        auto hit = intersector.IntersectRay(Ray(pos, glm::normalize(l)));
                        while (hit.IntersectState && hit.IntersectAlbedo.w < 0.2) {
                            hit = intersector.IntersectRay(Ray(hit.IntersectPosition, glm::normalize(l)));
                        }
                        if (hit.IntersectState) {
                            glm::vec3 p = hit.IntersectPosition - pos;
                            if(glm::dot(p, p) < glm::dot(l, l)) attenuation = 0;
                        }
                    }
                }

                /******************* 2. Whitted-style ray tracing *****************/
                // your code here
                glm::vec3 h = glm::normalize(ray.Direction + glm::normalize(l));
                result = kd * intersector.InternalScene->AmbientIntensity;
                result += light.Intensity * attenuation * kd * std::max((float)0, glm::dot(n, glm::normalize(l)));
                result += light.Intensity * attenuation * ks * std::pow(std::max((float)0, glm::dot(h, n)), shininess);
            }

            if (alpha < 0.9) {
                // refraction
                // accumulate color
                glm::vec3 R = alpha * glm::vec3(1.0f);
                color += weight * R * result;
                weight *= glm::vec3(1.0f) - R;

                // generate new ray
                ray = Ray(pos, ray.Direction);
            } else {
                // reflection
                // accumulate color
                glm::vec3 R = ks * glm::vec3(0.5f);
                color += weight * (glm::vec3(1.0f) - R) * result;
                weight *= R;

                // generate new ray
                glm::vec3 out_dir = ray.Direction - glm::vec3(2.0f) * n * glm::dot(n, ray.Direction);
                ray               = Ray(pos, out_dir);
            }
        }

        return color;
    }
} // namespace VCX::Labs::Rendering