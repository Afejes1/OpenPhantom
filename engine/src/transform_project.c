/* Reconstructed x87 kernel. Instruction choices preserve observed stores,
 * retained precision, raw depth-bit tests, and condition-code behavior. */
#include "baseline.h"

/* C4725 warns about defective early Pentium FDIV hardware. Retain the observed
 * divide instructions; the waiver is registered, scoped, and changes no bytes.
 * All other diagnostics remain enabled under /W4 /WX. */
#pragma warning(disable: 4725)
/* FUNCTION: WMAIN 0x0040efd0 */
void op_transform_project(const float *vertices, float *output,
                          const int *indices, unsigned char *codes,
                          int count, const float *matrix)
{
    if (count == 0) return;
    __asm {
        push esi
        push edi
        mov ebx, matrix
        mov edi, output
        mov esi, vertices
        mov edx, indices
        mov ecx, codes
        /* Keep vertical center, horizontal center, then focal on the x87 stack. */
        fld dword ptr op_height_bits
        fld dword ptr op_width_bits
        fld op_focal

    next_vertex:
        cmp edx, 0
        je transform_vertex
        mov eax, [edx]
        cmp eax, 0
        je advance_slots
        dec eax
        imul eax, 12
        add eax, vertices
        mov esi, eax

    transform_vertex:
        /* Column-major affine transform, rounded at each output component. */
        fld dword ptr [ebx]
        fmul dword ptr [esi]
        fld dword ptr [ebx+12]
        fmul dword ptr [esi+4]
        faddp st(1), st(0)
        fld dword ptr [ebx+24]
        fmul dword ptr [esi+8]
        faddp st(1), st(0)
        fadd dword ptr [ebx+36]
        fstp dword ptr [edi]
        fld dword ptr [ebx+4]
        fmul dword ptr [esi]
        fld dword ptr [ebx+16]
        fmul dword ptr [esi+4]
        faddp st(1), st(0)
        fld dword ptr [ebx+28]
        fmul dword ptr [esi+8]
        faddp st(1), st(0)
        fadd dword ptr [ebx+40]
        fstp dword ptr [edi+4]
        fld dword ptr [ebx+8]
        fmul dword ptr [esi]
        fld dword ptr [ebx+20]
        fmul dword ptr [esi+4]
        faddp st(1), st(0)
        fld dword ptr [ebx+32]
        fmul dword ptr [esi+8]
        faddp st(1), st(0)
        fadd dword ptr [ebx+44]
        fstp dword ptr [edi+8]

        /* Y is camera depth. Preserve its raw bits while replacing screen Y. */
        fld dword ptr [edi+4]
        fdivr st(0), st(1)
        fld dword ptr [edi]
        fmul st(0), st(1)
        fadd st(0), st(3)
        fstp dword ptr [edi]
        fmul dword ptr [edi+8]
        fchs
        fadd st(0), st(3)
        mov eax, [edi+4]
        fstp dword ptr [edi+4]
        mov [edi+8], eax
        push eax

        cmp ecx, 0
        je map_depth
        fldz
        fcomp dword ptr [edi+8]
        fnstsw ax
        test ah, 1
        jnz clip_planes
        mov byte ptr [ecx], 128
        jmp map_depth

    clip_planes:
        fld dword ptr [edi+8]
        fld dword ptr [edi+4]
        fld dword ptr [edi]
        mov byte ptr [ecx], 0
        fcom op_edge_18
        fnstsw ax
        test ah, 1
        jz clip_right
        or byte ptr [ecx], 1
        jmp clip_vertical
    clip_right:
        fcom op_edge_20
        fnstsw ax
        test ah, 65
        jnz clip_vertical
        or byte ptr [ecx], 2
    clip_vertical:
        fstp st(0)
        fcom op_edge_1c
        fnstsw ax
        test ah, 1
        jz clip_bottom
        or byte ptr [ecx], 8
        jmp clip_near
    clip_bottom:
        fcom op_edge_24
        fnstsw ax
        test ah, 65
        jnz clip_near
        or byte ptr [ecx], 4
    clip_near:
        fstp st(0)
        fcomp op_near
        fnstsw ax
        test ah, 1
        jz map_depth
        or byte ptr [ecx], 16

    map_depth:
        pop eax
        cmp eax, 0
        je positive_zero_depth
        fld1
        fdiv dword ptr [edi+8]
        fst dword ptr [edi+8]
        fmul op_depth_scale
        cmp op_depth_mode, 16
        je store_depth
        fld1
        fsub st(0), st(1)
        fstp dword ptr [edi+12]
        fstp st(0)
        jmp advance_slots
    positive_zero_depth:
        fldz
        cmp op_depth_mode, 16
        je store_depth
        fld1
        faddp st(1), st(0)
    store_depth:
        fstp dword ptr [edi+12]

    advance_slots:
        add esi, 12
        add edi, 16
        cmp edx, 0
        je advance_codes
        add edx, 4
    advance_codes:
        cmp ecx, 0
        je decrement_count
        inc ecx
    decrement_count:
        dec count
        jg next_vertex
        fstp st(0)
        fstp st(0)
        fstp st(0)
        pop edi
        pop esi
    }
}

#pragma warning(4: 4725)
