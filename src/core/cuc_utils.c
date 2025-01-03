#include "cuc_utils.h"
#include "platform/platform.h"

#include <math.h>
#include <float.h>

timer_t start_timer(double lifetime) {
    return (timer_t) { .start_time = platform_get_time(), .lifetime = lifetime };
}

bool is_timer_finished(timer_t timer) {
    return platform_get_time() - timer.start_time >= timer.lifetime;
}

double get_elapsed_time(timer_t timer) {
    return platform_get_time() - timer.start_time;
}

angle_t deg_to_radf(angle_t angle) {
    return (angle * PI / 180.0f);
}

angle_t rad_to_degf(angle_t angle) {
    return (angle * 180.0f / PI);
}

float clampf(float value, float min, float max) {
    if (value <= min) {
        return min;
    }

    if (value >= max) {
        return max;
    }

    return value;
}

float lerpf(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

float inv_lerpf(float a, float b, float v) {
    return (v - a) / (b - a);
}

angle_t lerp_angle(angle_t start_angle, angle_t target_angle, float t) {
    float delta = fmodf(target_angle - start_angle + 180.0f, 360.0f) - 180.0f;
    if (delta < -180.0f) {
        delta += 360.0f;
    }

    return start_angle + delta * t;
}

float remapf(float imin, float imax, float omin, float omax, float v) {
    float t = inv_lerpf(imin, imax, v);
    return lerpf(omin, omax, t);
}

vec2f_t vec2f_add(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x + b.x, a.y + b.y };
}

vec2f_t vec2f_add_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x + b, a.y + b };
}

vec2f_t vec2f_sub(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x - b.x, a.y - b.y };
}

vec2f_t vec2f_sub_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x - b, a.y - b };
}

vec2f_t vec2f_mult(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x * b.x, a.y * b.y };
}

vec2f_t vec2f_mult_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x * b, a.y * b };
}

vec2f_t vec2f_div(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x / b.x, a.y / b.y };
}

vec2f_t vec2f_div_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x / b, a.y / b };
}

bool vec2f_equal(vec2f_t a, vec2f_t b) {
    bool result = ((fabsf(a.x - b.x)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(a.x), fabsf(b.x))))) &&
                ((fabsf(a.y - b.y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(a.y), fabsf(b.y)))));

    return result;
}

float vec2f_length(vec2f_t vec) {
    return sqrtf((vec.x*vec.x) + (vec.y*vec.y));
}

float vec2f_distance(vec2f_t a, vec2f_t b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

float vec2f_direction(vec2f_t a, vec2f_t b) {
    return rad_to_degf(atan2f(b.y - a.y, b.x - a.x));
}

angle_t vec2f_angle(vec2f_t vec) {
    if (vec.x == 0.0f && vec.y == 0.0f) return 0.0f;
    return rad_to_degf(atan2f(vec.y, vec.x));
}

vec2f_t vec2f_set_angle(vec2f_t vec, angle_t angle) {
    vec2f_t polar = vec2f_cartesian2polar(vec);
    polar.y = angle;
    return vec2f_polar2cartesian(polar);
}

vec2f_t vec2f_rotate(vec2f_t vec, angle_t angle) {
    vec2f_t polar = vec2f_cartesian2polar(vec);
    polar.y += angle;
    return vec2f_polar2cartesian(polar);
}

vec2f_t vec2f_wrap_rect(vec2f_t vec, rectf_t rect) {
    vec2f_t result = VECTOR2_ZERO;

    if (vec.x < rect.x) {
        result.x = (rect.x + rect.width) - (rect.x - vec.x);
    } else if (vec.x > rect.x + rect.width) {
        result.x = rect.x + (vec.x - (rect.x + rect.width));
    } else {
        result.x = vec.x;
    }

    if (vec.y < rect.y) {
        result.y = (rect.y + rect.height) - (rect.y - vec.y);
    } else if (vec.y > rect.y + rect.height) {
        result.y = rect.y + (vec.y - (rect.y + rect.height));
    } else {
        result.y = vec.y;
    }

    return result;
}

bool vec2f_out_of_rect(vec2f_t vec, rectf_t rect) {
    return !check_collision_aabb_point((aabb_t) { .x = rect.x, .y = rect.y, .width = rect.width, .height = rect.height }, vec);
}

vec2f_t vec2f_move(vec2f_t vec, float angle, float distance) {
    vec2f_t result = VECTOR2_ZERO;
    result.x = vec.x + cosf(deg_to_radf(angle)) * distance;
    result.y = vec.y + sinf(deg_to_radf(angle)) * distance;
    return result;
}

vec2f_t vec2f_move_towards(vec2f_t start, vec2f_t target, float distance) {
    float dist = vec2f_distance(start, target);

    if (dist <= distance) {
        return target;
    }

    float angle = vec2f_direction(start, target);
    return vec2f_move(start, angle, distance);
}

float vec2f_dot_product(vec2f_t a, vec2f_t b) {
    return (a.x*b.x + a.y*b.y);
}

vec2f_t vec2f_opposite(vec2f_t vec) {
    return (vec2f_t) { -vec.x, -vec.y };
}

vec2f_t vec2f_perpendicular(vec2f_t vec) {
    return (vec2f_t) { -vec.y, vec.x };
}

vec2f_t vec2f_perpendicular_clockwise(vec2f_t vec) {
    return (vec2f_t) { vec.y, -vec.x };
}

vec2f_t vec2f_clamp(vec2f_t vec, vec2f_t min, vec2f_t max) {
    vec2f_t result = { .x = clampf(vec.x, min.x, max.x), .y = clampf(vec.y, min.y, max.y) };
    return result;
}

vec2f_t vec2f_clamp_value(vec2f_t vec, float min, float max) {
    return (vec2f_t) { .x = clampf(vec.x, min, max), .y = clampf(vec.y, min, max) };
}

vec2f_t vec2f_normalize(vec2f_t vec) {
    vec2f_t result = VECTOR2_ZERO;

    float length = sqrtf((vec.x*vec.x) + (vec.y*vec.y));

    if (length == 0.0f) {
        return VECTOR2_ZERO;
    }

    result.x = vec.x/length;
    result.y = vec.y/length;

    return result;
}

vec2f_t vec2f_lerp(vec2f_t a, vec2f_t b, float t) {
    return (vec2f_t) { .x = lerpf(a.x, b.x, t), .y = lerpf(a.y, b.y, t) };
}

float vec2f_inv_lerp(vec2f_t a, vec2f_t b, vec2f_t v) {
    vec2f_t ab = { b.x - a.x, b.y - a.y };
    vec2f_t av = { v.x - a.x, v.y - a.y };

    // Calculate the projection of `value - a` onto `b - a`
    float ab_length_squared = (ab.x * ab.x + ab.y * ab.y);
    if (ab_length_squared == 0.0f) {
        return 0.0f; // Handle edge case where a == b
    }

    float t = (av.x * ab.x + av.y * ab.y) / ab_length_squared;
    return t;
}

vec2f_t vec2f_slerp(vec2f_t a, vec2f_t b, float t) {
    return vec2f_lerp(vec2f_cartesian2polar(a), vec2f_cartesian2polar(b), t);
}

vec2f_t vec2f_cartesian2polar(vec2f_t vec) {
    return (vec2f_t) { hypotf(vec.x, vec.y), rad_to_degf(atan2f(vec.y, vec.x)) };
}

vec2f_t vec2f_polar2cartesian(vec2f_t vec) {
    return (vec2f_t) { vec.x * cosf(deg_to_radf(vec.y)), vec.x * sinf(deg_to_radf(vec.y)) };
}

// NOTE: i am not sure about always normalizing the axis
float vec2f_project_on_axis(vec2f_t vec, vec2f_t axis) {
    return vec2f_dot_product(vec, vec2f_normalize(axis));
}

vec2f_t vec2f_get_projected_point_on_axis(vec2f_t vec, vec2f_t axis) {
    return vec2f_lerp(vec, vec2f_normalize(axis), vec2f_project_on_axis(vec, axis));
}

rectf_t rectf_scale(rectf_t rect, float scalar) {
    rectf_t result = RECTANGLE_ZERO;

    result.x = rect.x * scalar;
    result.y = rect.y * scalar;
    result.width = rect.width * scalar;
    result.height = rect.height * scalar;

    return result;
}

vec2f_t rectf_center_offset(rectf_t rect) {
    return (vec2f_t) { rect.width/2, rect.height/2 };
}

color_t color_from_u32(uint32_t value) {
    return *(color_t*)&value;
}

uint32_t color_to_u32(color_t color) {
    return *(uint32_t*)&color;
}

color_t color_lerpf(color_t a, color_t b, float t) {
    float ar = a.r/255.0f;
    float ag = a.g/255.0f;
    float ab = a.b/255.0f;
    float aa = a.a/255.0f;

    float br = b.r/255.0f;
    float bg = b.g/255.0f;
    float bb = b.b/255.0f;
    float ba = b.a/255.0f;

    float rr = lerpf(ar, br, t);
    float rg = lerpf(ag, bg, t);
    float rb = lerpf(ab, bb, t);
    float ra = lerpf(aa, ba, t);

    return (color_t) { (uint8_t)(rr*255.0f), (uint8_t)(rg*255.0f), (uint8_t)(rb*255.0f), (uint8_t)(ra*255.0f) };
}

bool check_collision_aabb_aabb(aabb_t a, aabb_t b) {
    return (a.x < (b.x + b.width) && (a.x + a.width) > b.x) &&
        (a.y < (b.y + b.height) && (a.y + a.height) > b.y);
}

bool check_collision_aabb_point(aabb_t aabb, vec2f_t point) {
    return (point.x >= aabb.x) && (point.x < (aabb.x + aabb.width)) &&
        (point.y >= aabb.y) && (point.y < (aabb.y + aabb.height));
}

bool check_collision_aabb_circle(aabb_t aabb, circle_t circle) {
    float aabb_center_x = aabb.x + aabb.width/2.0f;
    float aabb_center_y = aabb.y + aabb.height/2.0f;

    float dx = fabsf(circle.center.x - aabb_center_x);
    float dy = fabsf(circle.center.y - aabb_center_y);

    if (dx > (aabb.width/2.0f + circle.radius)) {
        return false;
    }

    if (dy > (aabb.height/2.0f + circle.radius)) {
        return false;
    }

    if (dx <= (aabb.width/2.0f + circle.radius)) {
        return true;
    }

    if (dy <= (aabb.height/2.0f + circle.radius)) {
        return true;
    }

    float corner_distance_squared = (dx - aabb.width/2.0f)*(dx - aabb.width/2.0f) +
                                    (dy - aabb.height/2.0f)*(dy - aabb.height/2.0f);

    return (corner_distance_squared <= (circle.radius*circle.radius));
}

bool check_collision_circle_circle(circle_t a, circle_t b) {
    float dx = b.center.x - a.center.x;
    float dy = b.center.y - a.center.y;

    float distance_squared = dx*dx + dy*dy;
    float radius_sum = a.radius + b.radius;

    return (distance_squared <= (radius_sum*radius_sum));
}

bool check_collision_circle_point(circle_t circle, vec2f_t point) {
    float distance_squared = (point.x - circle.center.x)*(point.x - circle.center.x) +
        (point.y - circle.center.y)*(point.y - circle.center.y);

    return (distance_squared <= (circle.radius*circle.radius));
}

bool check_collision_obb_obb(obb_t a, obb_t b) {
    obb_corners_t a_corners = get_obb_corners(a);
    obb_corners_t b_corners = get_obb_corners(b);
    
    sat_t a_sat = { .vertices = a_corners.corners, .vertex_count = 4 };
    sat_t b_sat = { .vertices = b_corners.corners, .vertex_count = 4 };
    return check_collision_sat(a_sat, b_sat);
}

void project_sat(sat_t a, vec2f_t axis, float *out_min, float *out_max) {
    if (a.vertex_count == 0) {
        return;
    }

    vec2f_t used_axis = vec2f_normalize(axis);
    
    float projection = vec2f_dot_product(a.vertices[0], used_axis);
    float min = projection;
    float max = projection;
    for (size_t i = 1; i < a.vertex_count; i++) {
        projection = vec2f_dot_product(a.vertices[i], used_axis);
        if (projection < min) {
            min = projection;
        }

        if (projection > max) {
            max = projection;
        }
    }

    if (out_min != NULL) {
        *out_min = min;
    }

    if (out_max != NULL) {
        *out_max = max;
    }
}

// NOTE: THIS FUNCTION ONLY WORKS WITH CLOCKWISE POLYGONS FOR SOME REASON
bool check_collision_sat(sat_t a, sat_t b) {
    if (a.vertex_count < 2) {
        return false;
    }

    if (b.vertex_count < 2) {
        return false;
    }
    
    for (size_t i = 0; i < a.vertex_count; i++) {
        vec2f_t edge = vec2f_sub(a.vertices[(i+1)%a.vertex_count], a.vertices[i]);
        // vec2f_t edge = vec2f_sub(a.vertices[i], a.vertices[(i+1)%a.vertex_count]);
        vec2f_t axis = vec2f_perpendicular(edge);
        // vec2f_t axis = vec2f_perpendicular_clockwise(edge);
        // axis = vec2f_normalize(axis);

        float a_max = FLT_MAX;
        float a_min = -FLT_MAX;
        float b_max = FLT_MAX;
        float b_min = -FLT_MAX;

        project_sat(a, axis, &a_min, &a_max);
        project_sat(b, axis, &b_min, &b_max);

        if (a_max < b_min || b_max < a_min) {
            return false;
        }
    }

    for (size_t i = 0; i < b.vertex_count; i++) {
        vec2f_t edge = vec2f_sub(b.vertices[(i+1)%b.vertex_count], b.vertices[i]);
        // vec2f_t edge = vec2f_sub(b.vertices[i], b.vertices[(i+1)%b.vertex_count]);
        vec2f_t axis = vec2f_perpendicular(edge);
        // vec2f_t axis = vec2f_perpendicular_clockwise(edge);

        float a_max = FLT_MAX;
        float a_min = -FLT_MAX;
        float b_max = FLT_MAX;
        float b_min = -FLT_MAX;

        project_sat(a, axis, &a_min, &a_max);
        project_sat(b, axis, &b_min, &b_max);

        if (a_max < b_min || b_max < a_min) {
            return false;
        }
    }

    return true;
}

aabb_t circle_bounding_box(circle_t circle) {
    aabb_t result = RECTANGLE_ZERO;

    result.x = circle.center.x - circle.radius;
    result.y = circle.center.y - circle.radius;
    result.width= circle.radius + circle.radius;
    result.height = circle.radius + circle.radius;

    return result;
}

aabb_t obb_bounding_box(obb_t obb) {
    // OBB center
    float centerX = obb.rect.x + obb.rect.width / 2.0f;
    float centerY = obb.rect.y + obb.rect.height / 2.0f;

    // Half-extents
    float halfWidth = obb.rect.width / 2.0f;
    float halfHeight = obb.rect.height / 2.0f;

    // Precompute rotation matrix components
    float cosAngle = cosf(obb.rotation);
    float sinAngle = sinf(obb.rotation);

    // Corners of the OBB relative to the center
    float corners[4][2] = {
        {-halfWidth, -halfHeight},
        { halfWidth, -halfHeight},
        { halfWidth,  halfHeight},
        {-halfWidth,  halfHeight}
    };

    // Rotate and translate corners
    float minX = INFINITY, minY = INFINITY;
    float maxX = -INFINITY, maxY = -INFINITY;

    for (int i = 0; i < 4; ++i) {
        float rotatedX = cosAngle * corners[i][0] - sinAngle * corners[i][1];
        float rotatedY = sinAngle * corners[i][0] + cosAngle * corners[i][1];

        float worldX = rotatedX + centerX;
        float worldY = rotatedY + centerY;

        if (worldX < minX) minX = worldX;
        if (worldY < minY) minY = worldY;
        if (worldX > maxX) maxX = worldX;
        if (worldY > maxY) maxY = worldY;
    }

    // AABB
    aabb_t aabb;
    aabb.x = minX;
    aabb.y = minY;
    aabb.width = maxX - minX;
    aabb.height = maxY - minY;

    return aabb;
}

obb_corners_t get_obb_corners(obb_t obb) {
    obb_corners_t corners = {0};

    // Center of the rectangle
    vec2f_t center = (vec2f_t) { obb.rect.x + obb.rect.width / 2, obb.rect.y + obb.rect.height / 2 };

    // Half extents (relative to center)
    vec2f_t half_extents = (vec2f_t) { obb.rect.width / 2, obb.rect.height / 2 };

    // Calculate unrotated corner offsets from center
    // vec2f_t offsets[4] = {
    //     { -half_extents.x, -half_extents.y }, // Top-left
    //     { -half_extents.x,  half_extents.y }, // Bottom-left
    //     {  half_extents.x, -half_extents.y }, // Top-right
    //     {  half_extents.x,  half_extents.y }, // Bottom-right
    // };
    vec2f_t offsets[4] = {
        { -half_extents.x, -half_extents.y }, // Top-left
        {  half_extents.x,  half_extents.y }, // Bottom-right
        { -half_extents.x,  half_extents.y }, // Bottom-left
        {  half_extents.x, -half_extents.y }, // Top-right
    };

    // Rotate each corner offset and translate back to center
    for (int i = 0; i < 4; i++) {
        vec2f_t rotated_offset = vec2f_rotate(offsets[i], obb.rotation);
        corners.corners[i] = vec2f_add(center, rotated_offset);
    }

    return corners;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Physics //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void physics_apply_force(physics_object_t *obj, vec2f_t force) {
    if (obj == NULL) {
        return;
    }

    if (force.x != 0.0) {
        obj->acceleration.x += force.x / obj->mass;
    }

    if (force.y != 0.0) {
        obj->acceleration.y += force.y / obj->mass;
    }
}

void physics_update_obj(physics_object_t *obj, float dt) {
    if (obj == NULL) {
        return;
    }
    
    // Update velocity: v = v0 + a * dt
    obj->velocity.x += obj->acceleration.x * dt;
    obj->velocity.y += obj->acceleration.y * dt;

    // Update position: p = p0 + v * dt
    obj->pos.x += obj->velocity.x * dt;
    obj->pos.y += obj->velocity.y * dt;

    // Reset acceleration for the next frame
    obj->acceleration = VECTOR2_ZERO;
}

void physics_set_obj_velocity(physics_object_t *obj, vec2f_t velocity) {
    if (obj == NULL) {
        return;
    }

    obj->velocity = velocity;
}

void physics_clear_obj_forces(physics_object_t *obj) {
    if (obj == NULL) {
        return;
    }

    obj->acceleration = VECTOR2_ZERO;
}

void physics_kinetic_energy(physics_object_t *obj);

void physics_resolve_circle_collision(physics_object_t *a, physics_object_t *b, float restitution) {
    if (a == NULL) {
        return;
    }

    if (b == NULL) {
        return;
    }
    
    vec2f_t normal = { b->pos.x - a->pos.x, b->pos.y - a->pos.y };
    float distance = sqrtf(normal.x * normal.x + normal.y * normal.y);

    // Normalize the collision normal
    if (distance == 0.0f) return;
    normal.x /= distance;
    normal.y /= distance;

    // Relative velocity along the normal
    vec2f_t relative_velocity = { b->velocity.x - a->velocity.x, b->velocity.y - a->velocity.y };
    float velocity_along_normal = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

    // Ignore if moving apart
    if (velocity_along_normal > 0.0f) return;

    // Compute impulse scalar
    // float total_mass = a->mass + b->mass;
    float j = -(1 + restitution) * velocity_along_normal / (1 / a->mass + 1 / b->mass);

    // Apply impulse
    vec2f_t impulse = { j * normal.x, j * normal.y };
    a->velocity.x -= impulse.x / a->mass;
    a->velocity.y -= impulse.y / a->mass;
    b->velocity.x += impulse.x / b->mass;
    b->velocity.y += impulse.y / b->mass;
}