#pragma once


glm::vec3 DoNothing(triangle* tri, int depth, glm::vec3 p, glm::vec3 dir)
{
    return vec3(0);
}

glm::vec3 Shade(triangle* tri, int depth, glm::vec3 p, glm::vec3 dir)
{
    vec3 col = vec3(0);
    return col;
}

bool PointInTriangle(glm::vec3 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    return false;
}

float RayTriangleIntersection(glm::vec3 o, glm::vec3 dir, triangle* tri, glm::vec3& point)
{
    return FLT_MAX;
}

void trace(glm::vec3 o, glm::vec3 dir, float& t, glm::vec3& io_col, int depth, closest_hit p_hit)
{
}

vec3 GetRayDirection(float px, float py, int W, int H, float aspect_ratio, float fov)
{
    vec3 d, id;
    vec3 rVec = vec3(1, 0, 0), uVec = vec3(0, -1, 0), fVec = vec3(0, 0, -1);
    float coefficient, c1, f;

    //Calculate coefficient of R term and multiply with R
    f = tan(fov / 2);
    coefficient = aspect_ratio * f;
    c1 = 2 * (px + 0.5);
    c1 = c1 / W;
    c1 = c1 - 1;
    coefficient = coefficient * c1;
    d = coefficient * rVec;
    //Calculate coefficient of U term and multiply
    c1 = 2 * (py + 0.5);
    c1 = c1 / H;
    c1 = c1 - 1;
    coefficient = f * c1;
    id = coefficient * uVec;
    //Add terms
    d = d + id + fVec;
    //d = normalize(d); unsure if needed here or later


    return d;
}

void raytrace()
{
    vec3 ray, col;
    for (int pixel_y = 0; pixel_y < PIXEL_H; ++pixel_y)
    {
        float percf = (float)pixel_y / (float)PIXEL_H;
        int perci = percf * 100;
        std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;

        for (int pixel_x = 0; pixel_x < PIXEL_W; ++pixel_x)
        {
            ray = GetRayDirection(pixel_x, pixel_y, PIXEL_W, PIXEL_H, (float)PIXEL_W / (float)PIXEL_H, radians(90.f));
            //trace(ray, , , &col, , )
        }
    }
    std::clog << "\rFinish rendering.           \n";
}
