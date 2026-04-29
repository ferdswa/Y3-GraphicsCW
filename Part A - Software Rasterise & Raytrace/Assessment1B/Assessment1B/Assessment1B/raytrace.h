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
    vec3 col = tri->v1.col, diffuse;
    float t = FLT_MAX, amb = 0.1, idiff;
    //Only 1 light, no need for for loop
    //Get dir for ray to l
    vec3 dirRtoL = light_pos - p;
    dirRtoL = normalize(dirRtoL);
    trace(p, dirRtoL, t, col, depth, Shade);
    if (t == INT_MIN) {
        col = col * amb;
    }
    else {//Open
        idiff = dot(normalize(tri->v1.nor), dirRtoL);
        if (idiff > 0) {//angle between dirRtoL and v1.nor in -90,90 so correct side of geometry
            diffuse = col * idiff;
            col = col * amb + diffuse;
        }
        else//Wrong side of geometry
        {
            col = col * amb;
        }
    }
    if (tri->reflect) {//TODO: ADD REFLECT
        vec3 r = dirRtoL - 2 * dot(dirRtoL, normalize(tri->v1.nor)) * tri->v1.nor;
        trace(p, normalize(r), t, col, 2, Shade);
    }
    return col;
}

void trace(glm::vec3 o, glm::vec3 dir, float& t, glm::vec3& io_col, int depth, closest_hit p_hit)
{
    triangle closest = triangle();
    closest.v1.pos = vec3();
    vec3 vtiPt = vec3();
    float cl = FLT_MAX;
    for (triangle tri : tris) {
        t = RayTriangleIntersection(o, dir, &tri, vtiPt);
        if (PointInTriangle(vtiPt, tri.v1.pos, tri.v2.pos, tri.v3.pos)) {
            if (t < cl) {
                cl = t;
                closest = tri;
            }
        }
    }
    if (depth == 0) {
        if (closest.v1.pos == vec3())
            io_col = bkgd;
        else
            io_col = p_hit(&closest, 1, vtiPt, dir);
    }
    else if (depth == 1) {
        if(closest.v1.pos != vec3())
            io_col = io_col;
        else
            t = INT_MIN;
    }
    else if (depth == 2) {
        if (closest.v1.pos != vec3())//Triangle hit
        {
            io_col = io_col + (vec3(0.1) * closest.v1.col);
        }
    }
}

vec3 GetRayDirection(float px, float py, int W, int H, float aspect_ratio, float fov)
{
    vec3 d, id;
    vec3 rVec = vec3(1, 0, 0)/*inv*/, uVec = vec3(0, -1, 0)/*n*/, fVec = vec3(0, 0, -1)/*inv*/;
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
    float t = FLT_MAX;
    for (int pixel_y = 0; pixel_y < PIXEL_H; ++pixel_y)
    {
        float percf = (float)pixel_y / (float)PIXEL_H;
        int perci = percf * 100;
        std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;
        for (int pixel_x = 0; pixel_x < PIXEL_W; ++pixel_x)
        {
            ray = GetRayDirection(pixel_x, pixel_y, PIXEL_W, PIXEL_H, (float)PIXEL_W / (float)PIXEL_H, radians(90.f));
            trace(eye, ray, t, col, 0, Shade);
            writeCol(col, pixel_x, pixel_y);
        }
    }
    std::clog << "\rFinish rendering.           \n";
}