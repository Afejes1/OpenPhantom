#include "api.h"
// FUNCTION: WMAIN 0x0047cac0
void op_euler_affine_transform(OP_MATRIX34 *out, const OP_VECTOR3 *angles, const OP_VECTOR3 *position)
{
    float sinA, cosA, sinB, cosB, sinC, cosC;
    float cosBcosC, sinBsinC, cosBsinC, sinBcosC;
    op_angle_sincos(angles->x, &sinA, &cosA);
    op_angle_sincos(angles->y, &sinB, &cosB);
    op_angle_sincos(angles->z, &sinC, &cosC);
    cosBcosC = cosB * cosC;
    sinBsinC = sinB * sinC;
    cosBsinC = cosB * sinC;
    sinBcosC = sinB * cosC;
    out->basis[0] = -sinBsinC * sinA + cosBcosC;
    out->basis[1] = cosBsinC * sinA + sinBcosC;
    out->basis[2] = -sinC * cosA;
    out->basis[3] = -sinB * cosA;
    out->basis[4] = cosB * cosA;
    out->basis[5] = sinA;
    out->basis[6] = sinBcosC * sinA + cosBsinC;
    out->basis[7] = -sinA * cosBcosC + sinBsinC;
    out->basis[8] = cosC * cosA;
    out->translation = *position;
}
