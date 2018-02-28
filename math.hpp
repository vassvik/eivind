#ifndef PI
#define PI 3.14159265358979323846
#endif

////////////////////////////////////////////////////////////////////////////////
/// 3D Vector
////////////////////////////////////////////////////////////////////////////////

struct Vec3f {
    union {
        struct { float x, y, z; };
        float xyz[3];
    };
    Vec3f();
    Vec3f(float x, float y, float z);
    float& operator[](int rhs);
    const float& operator[](int rhs) const;
};

Vec3f operator+(Vec3f lhs, Vec3f rhs);
Vec3f operator-(Vec3f lhs, Vec3f rhs);
Vec3f operator*(Vec3f lhs, Vec3f rhs);
Vec3f operator/(Vec3f lhs, Vec3f rhs);

Vec3f operator+(float lhs, Vec3f rhs);
Vec3f operator+(Vec3f lhs, float rhs);

Vec3f operator-(float lhs, Vec3f rhs);
Vec3f operator-(Vec3f lhs, float rhs);

Vec3f operator*(float lhs, Vec3f rhs);
Vec3f operator*(Vec3f lhs, float rhs);

Vec3f operator/(float lhs, Vec3f rhs);
Vec3f operator/(Vec3f lhs, float rhs);

bool operator!=(Vec3f lhs, Vec3f rhs);
bool operator==(Vec3f lhs, Vec3f rhs);

float dot(Vec3f u, Vec3f v);
float length(Vec3f u);
float distance(Vec3f u, Vec3f v);

Vec3f abs(Vec3f u);
Vec3f cross(Vec3f u, Vec3f v);
Vec3f normalize(Vec3f v);
Vec3f swizzle(Vec3f u, int a, int b, int c);
Vec3f rotate_axis(Vec3f v, Vec3f axis, float angle);
Vec3f rotateX(Vec3f v, float angle);
Vec3f rotateY(Vec3f v, float angle);
Vec3f rotateZ(Vec3f v, float angle);


////////////////////////////////////////////////////////////////////////////////
/// 4x4 Matrix
////////////////////////////////////////////////////////////////////////////////

struct Mat4f {
    float M[4][4];

    Mat4f();  
};

Mat4f operator*(Mat4f lhs, Mat4f rhs); // matrix-matrix product

Mat4f translate(Vec3f move);
Mat4f scale(Vec3f scale);
Mat4f rotate(Vec3f axis, float angle);

Mat4f view(Vec3f r, Vec3f u, Vec3f f, Vec3f p);
Mat4f look_at(Vec3f eye, Vec3f center, Vec3f up);
Mat4f perspective(float fov, float ratio, float zmin, float zmax);




// impl


////////////////////////////////////////////////////////////////////////////////
/// 3D Vector
////////////////////////////////////////////////////////////////////////////////

Vec3f::Vec3f() : x(0.0), y(0.0), z(0.0) {

}

Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z) {

}

float& Vec3f::operator[](int rhs) {
    return xyz[rhs];
}

const float& Vec3f::operator[](int rhs) const {
    return xyz[rhs];
}

Vec3f  operator+(Vec3f lhs, Vec3f rhs) { return Vec3f{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z}; }
Vec3f  operator-(Vec3f lhs, Vec3f rhs) { return Vec3f{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z}; }
Vec3f  operator*(Vec3f lhs, Vec3f rhs) { return Vec3f{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z}; }
Vec3f  operator/(Vec3f lhs, Vec3f rhs) { return Vec3f{lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z}; }

Vec3f  operator+(float lhs, Vec3f rhs) { return Vec3f{lhs   + rhs.x, lhs   + rhs.y, lhs   + rhs.z}; }
Vec3f  operator+(Vec3f lhs, float rhs) { return Vec3f{lhs.x + rhs,   lhs.y + rhs,   lhs.z + rhs}; }

Vec3f  operator-(float lhs, Vec3f rhs) { return Vec3f{lhs   - rhs.x, lhs   - rhs.y, lhs   - rhs.z}; }
Vec3f  operator-(Vec3f lhs, float rhs) { return Vec3f{lhs.x - rhs,   lhs.y - rhs,   lhs.z - rhs}; }

Vec3f  operator*(float lhs, Vec3f rhs) { return Vec3f{lhs   * rhs.x, lhs   * rhs.y, lhs   * rhs.z}; }
Vec3f  operator*(Vec3f lhs, float rhs) { return Vec3f{lhs.x * rhs,   lhs.y * rhs,   lhs.z * rhs}; }

Vec3f  operator/(float lhs, Vec3f rhs) { return Vec3f{lhs / rhs.x, lhs / rhs.y, lhs / rhs.z}; }
Vec3f  operator/(Vec3f lhs, float rhs) { return Vec3f{lhs.x / rhs,   lhs.y / rhs,   lhs.z / rhs}; }

bool operator!=(Vec3f lhs, Vec3f rhs) { return !(lhs == rhs); }
bool operator==(Vec3f lhs, Vec3f rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }

float dot(Vec3f u, Vec3f v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

float length(Vec3f u) {
    return sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
}

float distance(Vec3f u, Vec3f v) {
    return sqrt((u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y) + (u.z - v.z)*(u.z - v.z));
}

Vec3f abs(Vec3f u) {
    return Vec3f{(float)fabs(u.x), (float)fabs(u.y), (float)fabs(u.z)};
}

Vec3f cross(Vec3f u, Vec3f v) {
    return Vec3f{u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x};
}

Vec3f normalize(Vec3f u) {
    float l = length(u);
    return u/l;
}

Vec3f rotate_axis(Vec3f u, Vec3f axis, float angle) {
    Vec3f a = normalize(axis);

    float c = cos(angle);
    float s = sin(angle);
    float oc = 1.0 - c;

    return Vec3f{(oc * a.x * a.x + 1.f * c)*u.x + (oc * a.x * a.y - a.z * s)*u.y + (oc * a.z * a.x + a.y * s)*u.z,
                 (oc * a.x * a.y + a.z * s)*u.x + (oc * a.y * a.y + 1.f * c)*u.y + (oc * a.y * a.z - a.x * s)*u.z,
                 (oc * a.z * a.x - a.y * s)*u.x + (oc * a.y * a.z + a.x * s)*u.y + (oc * a.z * a.z + 1.f * c)*u.z};
}

Vec3f rotateX(Vec3f v, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    return Vec3f{v.x, v.y*c - v.z*s, v.y*s + v.z*c};
}

Vec3f rotateY(Vec3f v, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    return Vec3f{v.x*c + v.z*s, v.y, -v.x*s + v.z*c};
}

Vec3f rotateZ(Vec3f v, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    return Vec3f{v.x*c - v.y*s, v.x*s + v.y*c, v.z};
}


Vec3f swizzle(Vec3f u, int a, int b, int c) {
    return Vec3f{u[a], u[b], u[c]};
}


////////////////////////////////////////////////////////////////////////////////
/// 4x4 Matrix
////////////////////////////////////////////////////////////////////////////////

Mat4f :: Mat4f() {
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) M[i][j] = 0.0;
    M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1.0;

}

Mat4f operator*(Mat4f lhs, Mat4f rhs) {
    Mat4f out;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            out.M[j][i] = 0.0;
            for (int k = 0; k < 4; k++) {
                out.M[j][i] += lhs.M[k][i]*rhs.M[j][k];
            }
        }
    }
    return out;
}

Mat4f translate(Vec3f move) {
    Mat4f out;
    out.M[3][0] = move.x, out.M[3][1] = move.y, out.M[3][2] = move.z;
    return out;
}

Mat4f scale(Vec3f scale) {
    Mat4f out;
    out.M[0][0] = scale.x;
    out.M[1][1] = scale.y;
    out.M[2][2] = scale.z;
    return out;
}

Mat4f rotate(Vec3f axis, float angle) {
    axis = normalize(axis);
    float c = cos(-angle*PI/180.0); // negative because of column-major
    float s = sin(-angle*PI/180.0); // negative because of column-major
    float u = axis.x;
    float v = axis.y;
    float w = axis.z;

    Mat4f mat;
    mat.M[0][0] =      c + u*u*(1.0 - c);
    mat.M[1][0] =  w * s + v*u*(1.0 - c);
    mat.M[2][0] = -v * s + w*u*(1.0 - c);
    mat.M[0][1] = -w * s + u*v*(1.0 - c);
    mat.M[1][1] =      c + v*v*(1.0 - c);
    mat.M[2][1] =  u * s + w*v*(1.0 - c);
    mat.M[0][2] =  v * s + u*w*(1.0 - c);
    mat.M[1][2] = -u * s + v*w*(1.0 - c);
    mat.M[2][2] =      c + w*w*(1.0 - c);

    return mat;
}

Mat4f view(Vec3f r, Vec3f u, Vec3f f, Vec3f p) {
    Mat4f mat;
    mat.M[0][0] =  r.x, mat.M[1][0] =  r.y, mat.M[2][0] =  r.z, mat.M[3][0] = -dot(r, p);
    mat.M[0][1] =  u.x, mat.M[1][1] =  u.y, mat.M[2][1] =  u.z, mat.M[3][1] = -dot(u, p);
    mat.M[0][2] = -f.x, mat.M[1][2] = -f.y, mat.M[2][2] = -f.z, mat.M[3][2] =  dot(f, p);

    return mat;
}

Mat4f look_at(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f f = normalize(Vec3f(center.x - eye.x, center.y - eye.y, center.z - eye.z));
    Vec3f u = normalize(up);
    Vec3f r = normalize(cross(f,u));
    u = cross(r,f);

    return view(r, u ,f, eye);
}

Mat4f perspective(float fov, float ratio, float zmin, float zmax) {
    Mat4f out;
    float f = 1.0/tan((fov*PI/180.0)/2.0);
    out.M[0][0] = f/ratio; 
    out.M[1][1] = f;
    out.M[2][2] = (zmax + zmin)/(zmin - zmax);
    out.M[3][2] = 2*zmax*zmin/(zmin - zmax);
    out.M[2][3] = -1.0;
    out.M[3][3] = 0.0;

    return out;
}
