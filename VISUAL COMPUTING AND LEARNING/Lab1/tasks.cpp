#include <random>

#include <spdlog/spdlog.h>

#include "Labs/1-Drawing2D/tasks.h"

using VCX::Labs::Common::ImageRGB;

std :: mt19937 rnd(time(NULL));

namespace VCX::Labs::Drawing2D {
    /******************* 1.Image Dithering *****************/
    void DitheringThreshold(
        ImageRGB &       output,
        ImageRGB const & input) {
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                output.At(x, y) = {
                    color.r > 0.5 ? 1 : 0,
                    color.g > 0.5 ? 1 : 0,
                    color.b > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringRandomUniform(
        ImageRGB &       output,
        ImageRGB const & input) {
        std::uniform_real_distribution<double> dis(-0.5, 0.5);
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                double t = dis(rnd);
                output.At(x, y) = {
                    color.r + t > 0.5 ? 1 : 0,
                    color.g + t > 0.5 ? 1 : 0,
                    color.b + t > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringRandomBlueNoise(
        ImageRGB &       output,
        ImageRGB const & input,
        ImageRGB const & noise) {
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                glm::vec3 ncolor = noise.At(x, y);
                output.At(x, y) = {
                    color.r + ncolor.r - 0.5 > 0.5 ? 1 : 0,
                    color.g + ncolor.g - 0.5 > 0.5 ? 1 : 0,
                    color.b + ncolor.b - 0.5 > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringOrdered(
        ImageRGB &       output,
        ImageRGB const & input) {
        for (std::size_t x = 0; x < output.GetSizeX(); ++x)
            for (std::size_t y = 0; y < output.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x / 3, y / 3);
                int mx = x % 3, my = y % 3;
                output.At(x, y) = {
                    color.r + (mx * 3 + my) / 9. - 0.5 > 0.5 ? 1 : 0,
                    color.g + (mx * 3 + my) / 9. - 0.5 > 0.5 ? 1 : 0,
                    color.b + (mx * 3 + my) / 9. - 0.5 > 0.5 ? 1 : 0,
                };
            }
    }

    void DitheringErrorDiffuse(
        ImageRGB &       output,
        ImageRGB const & input) {
        glm::vec3 ret[input.GetSizeX() + 1][input.GetSizeY() + 1];
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                ret[x][y] = { 0, 0, 0 };
            }
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                glm::vec3 diff  = ret[x][y]; //output.At(x, y);
                output.At(x, y) = {
                    color.r + diff.r > 0.5 ? 1 : 0,
                    color.g + diff.g > 0.5 ? 1 : 0,
                    color.b + diff.b > 0.5 ? 1 : 0,
                };
                glm::vec3 vcolor = output.At(x, y);
                diff = color - vcolor;
                float tot = 7 * (y + 1 < input.GetSizeY()) + (x + 1 < input.GetSizeX() && y - 1 > 0) * 3 + 
                    (x + 1 < input.GetSizeX()) * 5 + (x + 1 < input.GetSizeX() && y + 1 < input.GetSizeY());
                if(tot < 0.5) tot += 1;
                tot = 16;
                ret[x][y + 1] += diff * float(7. / tot);
                if(y > 0) ret[x + 1][y - 1] += diff * float(3. / tot);
                ret[x + 1][y] += diff * float(5. / tot);
                ret[x + 1][y + 1] += diff * float(1. / tot);
            }
    }

    /******************* 2.Image Filtering *****************/
    void Blur(
        ImageRGB &       output,
        ImageRGB const & input) {
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 ret = { 0., 0., 0. };
                for(std::size_t dx = 0; dx < 3; dx++) 
                    for(std::size_t dy = 0; dy < 3; dy++) 
                        if(x + dx > 0 && y + dy > 0 && x + dx < input.GetSizeX() + 1 && 
                         y + dy < input.GetSizeY())
                            ret += input.At(x + dx - 1, y + dy - 1) / float(9.);
                output.At(x, y) = ret;
            }
    }

    void Edge(
        ImageRGB &       output,
        ImageRGB const & input) {
        float t[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        for (std::size_t x = 0; x < input.GetSizeX(); ++x)
            for (std::size_t y = 0; y < input.GetSizeY(); ++y) {
                glm::vec3 color = input.At(x, y);
                glm::vec3 ret = { 0., 0., 0. };
                for(std::size_t dx = 0; dx < 3; dx++) 
                    for(std::size_t dy = 0; dy < 3; dy++) 
                        if(x + dx > 0 && y + dy > 0 && x + dx < input.GetSizeX() + 1 && 
                         y + dy < input.GetSizeY())
                            ret += (input.At(x + dx - 1, y + dy - 1) - color) 
                                * sqrtf(t[dx][dy] * t[dx][dy] + t[dy][dx] * t[dy][dx]);
                output.At(x, y) = ret;
            }
    }

    /******************* 3. Image Inpainting *****************/
    void Inpainting(
        ImageRGB &         output,
        ImageRGB const &   inputBack,
        ImageRGB const &   inputFront,
        const glm::ivec2 & offset) {
        output             = inputBack;
        std::size_t width  = inputFront.GetSizeX();
        std::size_t height = inputFront.GetSizeY();
        glm::vec3 * g      = new glm::vec3[width * height];
        memset(g, 0, sizeof(glm::vec3) * width * height);
        // set boundary condition
        for (std::size_t y = 0; y < height; ++y) {
            // set boundary for (0, y), your code: g[y * width] = ?
            g[y * width] = inputBack.At(0 + offset.x, y + offset.y) - inputFront.At(0, y);
            // set boundary for (width - 1, y), your code: g[y * width + width - 1] = ?
            g[y * width + width - 1] = inputBack.At(width - 1 + offset.x, y + offset.y) - inputFront.At(width - 1, y);
        }
        for (std::size_t x = 0; x < width; ++x) {
            // set boundary for (x, 0), your code: g[x] = ?
            g[x] = inputBack.At(x + offset.x, 0 + offset.y) - inputFront.At(x, 0);
            // set boundary for (x, height - 1), your code: g[(height - 1) * width + x] = ?
            g[(height - 1) * width + x] = inputBack.At(x + offset.x, height - 1 + offset.y) - inputFront.At(x, height - 1);
        }

        // Jacobi iteration, solve Ag = b
        for (int iter = 0; iter < 8000; ++iter) {
            for (std::size_t y = 1; y < height - 1; ++y)
                for (std::size_t x = 1; x < width - 1; ++x) {
                    g[y * width + x] = (g[(y - 1) * width + x] + g[(y + 1) * width + x] + g[y * width + x - 1] + g[y * width + x + 1]);
                    g[y * width + x] = g[y * width + x] * glm::vec3(0.25);
                }
        }

        for (std::size_t y = 0; y < inputFront.GetSizeY(); ++y)
            for (std::size_t x = 0; x < inputFront.GetSizeX(); ++x) {
                glm::vec3 color = g[y * width + x] + inputFront.At(x, y);
                output.At(x + offset.x, y + offset.y) = color;
            }
        delete[] g;
    }

    /******************* 4. Line Drawing *****************/
    void DrawLine(
        ImageRGB &       canvas,
        glm::vec3 const  color,
        glm::ivec2 const p0,
        glm::ivec2 const p1) {
        glm::ivec2 pp0 = p0, pp1 = p1;
        bool swp1 = p0.x > p1.x, swp2 = p0.y > p1.y;
        if(swp1) std::swap(pp0.x, pp1.x);
        if(swp2) std::swap(pp0.y, pp1.y);
        bool swp3 = pp1.y - pp0.y > pp1.x - pp0.x;
        if(swp3) std::swap(pp0.x, pp0.y), std::swap(pp1.x, pp1.y);
        int dx = 2 * (pp1.x - pp0.x), dy = 2 * (pp1.y - pp0.y);
        int dydx = dy - dx, F = dy - dx / 2;
        for (std::size_t x = pp0.x, y = pp0.y; x <= pp1.x; x++) {
            size_t nx = x, ny = y;
            if(swp3) std::swap(nx, ny);
            if(swp2) ny = p0.y + p1.y - ny;
            if(swp1) nx = p0.x + p1.x - nx;
            canvas.At(nx, ny) = color;
            if (F < 0) F += dy;
            else { y++; F += dydx; }
        }
    }

    /******************* 5. Triangle Drawing *****************/
    void DrawTriangleFilled(
        ImageRGB &       canvas,
        glm::vec3 const  color,
        glm::ivec2 const p0,
        glm::ivec2 const p1,
        glm::ivec2 const p2) {
    }

    /******************* 6. Image Supersampling *****************/
    void Supersample(
        ImageRGB &       output,
        ImageRGB const & input,
        int              rate) {
        for (std::size_t x = 0; x < output.GetSizeX(); ++x)
            for (std::size_t y = 0; y < output.GetSizeY(); ++y) {
                std::size_t nx = 1. * x / output.GetSizeX() * input.GetSizeY();
                std::size_t ny = 1. * y / output.GetSizeY() * input.GetSizeY();
                glm::vec3 color = glm::vec3(0);
                for(int dx = -rate / 2, t = dx; dx < t + rate; dx++) {
                    for(int dy = t; dy < t + rate; dy++) {
                        int nnx = nx + dx, nny = ny + dy;
                        nnx = std::max(0, std::min(nnx, (int)input.GetSizeX()));
                        nny = std::max(0, std::min(nny, (int)input.GetSizeY()));
                        color += input.At(nnx, nny);
                    }
                }
                color /= glm::vec3(rate * rate);
                output.At(x, y) = color;
            }
    }

    /******************* 7. Bezier Curve *****************/
    // Note: Please finish the function [DrawLine] before trying this part.
    glm::vec2 CalculateBezierPoint(
        std::span<glm::vec2> points,
        float const          t) {
        // your code here:
        std::vector<glm::vec2> gpoints;
        for(auto x : points) gpoints.push_back(x);
        while(gpoints.size() > 1) {
            std::vector<glm::vec2> ret;
            for(int i = 1; i < gpoints.size(); i++) {
                glm::vec2 v = gpoints[i - 1] * glm::vec2(1 - t) + gpoints[i] * glm::vec2(t);
                ret.push_back(v);
            }
            gpoints = ret;
        }
        return gpoints[0];
    }
} // namespace VCX::Labs::Drawing2D