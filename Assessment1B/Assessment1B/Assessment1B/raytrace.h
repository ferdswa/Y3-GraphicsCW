#pragma once

glm::vec3 DoNothing(triangle* tri, int depth, glm::vec3 p, glm::vec3 dir)
{
    return vec3(0);
}

bool PointInTriangle(glm::vec3 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    float d1, d2, d3;
    vec3 v1q, v2q, v3q, v12, v13, v23, v21, v32, v31, crossLeft, crossRight;
    //Calculate relative vectors
    v1q = pt - v1;
    v2q = pt - v2;
    v3q = pt - v3;
    v12 = v2 - v1;
    v13 = v3 - v1;
    v23 = v3 - v2;
    v21 = v1 - v2;
    v32 = v2 - v3;
    v31 = v1 - v3;

    crossLeft = cross(v12, v1q);
    crossRight = cross(v12, v13);
    d1 = dot(crossLeft, crossRight);

    crossLeft = cross(v23, v2q);
    crossRight = cross(v23, v21);
    d2 = dot(crossLeft, crossRight);

    crossLeft = cross(v31, v3q);
    crossRight = cross(v31, v32);
    d3 = dot(crossLeft, crossRight);

    if (d1 > 0 && d2 > 0 && d3 > 0) {
        return true;
    }
    return false;
}

float RayTriangleIntersection(glm::vec3 o, glm::vec3 dir, triangle* tri, glm::vec3& point)//Could be wrong
{
    float t = FLT_MAX;
    vec3 knownPointSubOrigin = tri->v1.pos - o;
    vec3 v13 = tri->v3.pos - tri->v1.pos;
    vec3 v12 = tri->v2.pos - tri->v1.pos;
    vec3 res = normalize(cross(v12, v13));
    float resF = dot(knownPointSubOrigin, res);
    float denom = dot(dir, res);
    t = resF / denom;

    //Calc q
    vec3 dt = dir * t;
    point = o + dt;
    return t;
}

glm::vec3 Shade(triangle* tri, int depth, glm::vec3 p, glm::vec3 dir)
{
    vec3 col = tri->v1.col, idiff;
    float t, diffuse, amb = 0.1;

    col = amb * col;
    //Only 1 light, no need for for loop
    //Get dir for ray to l
    vec3 dirRtoL = light_pos - p;
    vec3 toLIntersectP = vec3(0, 0, 0);
    bool intersects = false;
    for (triangle tri0 : tris) {
        toLIntersectP = vec3(0, 0, 0);
        if (&tri0 != tri) {
            t = RayTriangleIntersection(p, dirRtoL, &tri0, toLIntersectP);
            if (toLIntersectP != vec3(0, 0, 0))//If the vector exists, there is an intersection
            {
                intersects = true;
                break;//now shadow, no need to check remaining tris
            }
        }
    }
    if (!intersects) {
        idiff = tri->v1.nor * dirRtoL;
        diffuse = dot(idiff, col);
        col = col + diffuse;
    }
    if (tri->reflect) {//TODO: ADD REFLECT

    }
    return col;
}

void trace(glm::vec3 o, glm::vec3 dir, float& t, glm::vec3& io_col, int depth, closest_hit p_hit)
{
    triangle closest = triangle();
    vec3 vtiPt = vec3();
    for (triangle tri : tris) {
        t = RayTriangleIntersection(o, dir, &tri, vtiPt);
        if (PointInTriangle(vtiPt, tri.v1.pos, tri.v2.pos, tri.v3.pos)) {
            if (t < depth) {
                depth = t;
                closest = tri;
            }
        }
    }
    if (closest.v1.pos == vec3()) {
        io_col = bkgd;
    }
    else {
        io_col = p_hit(&closest, depth, vtiPt, dir);
    }
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
    d = normalize(d);// unsure if needed here or later
    return d;
}

void raytrace()
{
    vec3 ray, col = bkgd, point;
    light_pos = light_pos * vec3(1, 1, 1);
    float t;
    for (int pixel_y = 0; pixel_y < PIXEL_H; ++pixel_y)
    {
        float percf = (float)pixel_y / (float)PIXEL_H;
        int perci = percf * 100;
        std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;

        for (int pixel_x = 0; pixel_x < PIXEL_W; ++pixel_x)
        {
            
            ray = GetRayDirection(pixel_x, pixel_y, PIXEL_W, PIXEL_H, (float)PIXEL_W / (float)PIXEL_H, radians(90.f));
            trace(eye, ray, t, col, INT_MAX, Shade);
            writeCol(col, pixel_x, pixel_y);
        }
    }
    std::clog << "\rFinish rendering.           \n";
}
