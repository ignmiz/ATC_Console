/*******************************************************************************
* Copyright 2016-2017 Intel Corporation All Rights Reserved.
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*******************************************************************************/

/*
!  Content:
!      Intel(R) Math Kernel Library (Intel(R) MKL) C functions that can be inlined
!******************************************************************************/

#undef mkl_dc_getrf
#undef mkl_dc_lapacke_getrf_convert

#undef mkl_dc_getrf_1x1
#undef mkl_dc_getrf_2x2
#undef mkl_dc_getrf_3x3
#undef mkl_dc_getrf_4x4
#undef mkl_dc_getrf_5x5
#undef mkl_dc_getrf_mxn
#undef mkl_dc_getrf_generic

#undef mkl_dc_getri
#undef mkl_dc_lapacke_getri_convert

#undef mkl_dc_getri_1x1
#undef mkl_dc_getri_2x2
#undef mkl_dc_getri_3x3
#undef mkl_dc_getri_4x4
#undef mkl_dc_getri_5x5
#undef mkl_dc_getri_6x6
#undef mkl_dc_getri_n
#undef mkl_dc_getri_inverse_upper
#undef mkl_dc_getri_solve_lower
#undef mkl_dc_getri_generic

#undef mkl_dc_getrs
#undef mkl_dc_lapacke_getrs_convert

#undef mkl_dc_getrs_trans
#undef mkl_dc_getrs_nxnrhs
#undef mkl_dc_getrs_generic

#if defined(MKL_DOUBLE)
#define mkl_dc_getrf                    mkl_dc_dgetrf
#define mkl_dc_lapacke_getrf_convert    mkl_dc_lapacke_dgetrf_convert
#define mkl_dc_getri                    mkl_dc_dgetri
#define mkl_dc_lapacke_getri_convert    mkl_dc_lapacke_dgetri_convert
#define mkl_dc_getrs                    mkl_dc_dgetrs
#define mkl_dc_lapacke_getrs_convert    mkl_dc_lapacke_dgetrs_convert

#elif defined(MKL_SINGLE)
#define mkl_dc_getrf                    mkl_dc_sgetrf
#define mkl_dc_lapacke_getrf_convert    mkl_dc_lapacke_sgetrf_convert
#define mkl_dc_getri                    mkl_dc_sgetri
#define mkl_dc_lapacke_getri_convert    mkl_dc_lapacke_sgetri_convert
#define mkl_dc_getrs                    mkl_dc_sgetrs
#define mkl_dc_lapacke_getrs_convert    mkl_dc_lapacke_sgetrs_convert

#elif defined(MKL_COMPLEX)
#define mkl_dc_getrf                    mkl_dc_cgetrf
#define mkl_dc_lapacke_getrf_convert    mkl_dc_lapacke_cgetrf_convert
#define mkl_dc_getri                    mkl_dc_cgetri
#define mkl_dc_lapacke_getri_convert    mkl_dc_lapacke_cgetri_convert
#define mkl_dc_getrs                    mkl_dc_cgetrs
#define mkl_dc_lapacke_getrs_convert    mkl_dc_lapacke_cgetrs_convert

#elif defined(MKL_COMPLEX16)
#define mkl_dc_getrf                    mkl_dc_zgetrf
#define mkl_dc_lapacke_getrf_convert    mkl_dc_lapacke_zgetrf_convert
#define mkl_dc_getri                    mkl_dc_zgetri
#define mkl_dc_lapacke_getri_convert    mkl_dc_lapacke_zgetri_convert
#define mkl_dc_getrs                    mkl_dc_zgetrs
#define mkl_dc_lapacke_getrs_convert    mkl_dc_lapacke_zgetrs_convert
#endif

#define mkl_dc_getrf_mxn(m, n, a, lda, ipiv, info, a_access) \
do { \
    MKL_INT i, j, k, ii, jj; \
    *info = 0; \
    for (j = 0; j < MKL_DC_MIN(m, n); j++) { \
        mkl_dc_real_type pivot = MKL_DC_ABS1(a_access(a, lda, j, j)); \
        MKL_INT jp = j; \
        for (i = j + 1; i < m; i++) { \
            mkl_dc_real_type d = MKL_DC_ABS1(a_access(a, lda, i, j)); \
            if (d > pivot) { \
                pivot = d; \
                jp = i; \
            } \
        } \
        ipiv[j] = jp + 1; \
        if (!MKL_DC_IS_ZERO(a_access(a, lda, jp, j))) { \
            if (jp != j) { \
                for (k = 0; k < n; k++) { \
                    MKL_DC_SWAP(a_access(a, lda, j, k), a_access(a, lda, jp, k)); \
                } \
            } \
            for (i = j + 1; i < m; i++) { \
                MKL_DC_DIV(a_access(a, lda, i, j), a_access(a, lda, i, j), a_access(a, lda, j, j)); \
            } \
        } else if (*info == 0) { \
            *info = j + 1; \
        } \
        for (jj = j + 1; jj < n; jj++) { \
            mkl_dc_type a_j_jj = a_access(a, lda, j, jj); \
            __pragma(simd) \
            for (ii = j + 1; ii < m; ii++) { \
                MKL_DC_SUB_MUL(a_access(a, lda, ii, jj), a_access(a, lda, ii, jj), \
                               a_access(a, lda, ii, j), a_j_jj); \
            } \
        } \
    } \
} while (0) \

#define mkl_dc_getrf_1x1(m, n, a, lda, ipiv, info, a_access) \
do { \
    ipiv[0] = 1; \
    *info = !MKL_DC_IS_ZERO(a_access(a, lda, 0, 0)) ? 0 : 1; \
} while (0) \

#define mkl_dc_getrf_2x2(m, n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_real_type pivot = MKL_DC_ABS1(a00); \
    mkl_dc_real_type d = MKL_DC_ABS1(a10); \
    MKL_INT jp = 0; \
    *info = 0; \
    if (d > pivot) { \
        pivot = d; \
        jp = 1; \
    } \
    ipiv[0] = jp + 1; \
    if (jp == 0) { \
        if (!MKL_DC_IS_ZERO(a00)) { \
            MKL_DC_DIV(a10, a10, a00); \
            MKL_DC_SUB_MUL(a11, a11, a10, a01); \
        } else { \
            *info = 1; \
        } \
    } else { \
        MKL_DC_SWAP(a00, a10); \
        MKL_DC_SWAP(a01, a11); \
        MKL_DC_DIV(a10, a10, a00); \
        MKL_DC_SUB_MUL(a11, a11, a10, a01); \
    } \
    ipiv[1] = 2; \
    if (MKL_DC_IS_ZERO(a11) && *info == 0) { \
        *info = 2; \
    } \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 1, 1) = a11; \
} while (0) \

#define mkl_dc_getrf_3x3(m, n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_real_type pivot = MKL_DC_ABS1(a00); \
    mkl_dc_real_type d = MKL_DC_ABS1(a10); \
    MKL_INT jp = 0; \
    *info = 0; \
    if (d > pivot) { \
        pivot = d; \
        jp = 1; \
    } \
    d = MKL_DC_ABS1(a20); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    ipiv[0] = jp + 1; \
    switch (jp) { \
        case 0: \
            if (!MKL_DC_IS_ZERO(a00)) { \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 1: \
            if (!MKL_DC_IS_ZERO(a10)) { \
                MKL_DC_SWAP(a00, a10); \
                MKL_DC_SWAP(a01, a11); \
                MKL_DC_SWAP(a02, a12); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a20)) { \
                MKL_DC_SWAP(a00, a20); \
                MKL_DC_SWAP(a01, a21); \
                MKL_DC_SWAP(a02, a22); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a11, a11, a10, a01); \
    MKL_DC_SUB_MUL(a21, a21, a20, a01); \
    MKL_DC_SUB_MUL(a12, a12, a10, a02); \
    MKL_DC_SUB_MUL(a22, a22, a20, a02); \
    pivot = MKL_DC_ABS1(a11); \
    jp = 1; \
    d = MKL_DC_ABS1(a21); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    ipiv[1] = jp + 1; \
    switch (jp) { \
        case 1: \
            if (!MKL_DC_IS_ZERO(a11)) { \
                MKL_DC_DIV(a21, a21, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a21)) { \
                MKL_DC_SWAP(a10, a20); \
                MKL_DC_SWAP(a11, a21); \
                MKL_DC_SWAP(a12, a22); \
                MKL_DC_DIV(a21, a21, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a22, a22, a21, a12); \
    ipiv[2] = 3; \
    if (MKL_DC_IS_ZERO(a22) && *info == 0) { \
        *info = 3; \
    } \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 2, 2) = a22; \
} while (0) \

#define mkl_dc_getrf_4x4(m, n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a30 = a_access(a, lda, 3, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a31 = a_access(a, lda, 3, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type a32 = a_access(a, lda, 3, 2); \
    mkl_dc_type a03 = a_access(a, lda, 0, 3); \
    mkl_dc_type a13 = a_access(a, lda, 1, 3); \
    mkl_dc_type a23 = a_access(a, lda, 2, 3); \
    mkl_dc_type a33 = a_access(a, lda, 3, 3); \
    mkl_dc_real_type pivot = MKL_DC_ABS1(a00); \
    mkl_dc_real_type d = MKL_DC_ABS1(a10); \
    MKL_INT jp = 0; \
    *info = 0; \
    if (d > pivot) { \
        pivot = d; \
        jp = 1; \
    } \
    d = MKL_DC_ABS1(a20); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    d = MKL_DC_ABS1(a30); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    ipiv[0] = jp + 1; \
    switch (jp) { \
        case 0: \
            if (!MKL_DC_IS_ZERO(a00)) { \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 1: \
            if (!MKL_DC_IS_ZERO(a10)) { \
                MKL_DC_SWAP(a00, a10); \
                MKL_DC_SWAP(a01, a11); \
                MKL_DC_SWAP(a02, a12); \
                MKL_DC_SWAP(a03, a13); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a20)) { \
                MKL_DC_SWAP(a00, a20); \
                MKL_DC_SWAP(a01, a21); \
                MKL_DC_SWAP(a02, a22); \
                MKL_DC_SWAP(a03, a23); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a30)) { \
                MKL_DC_SWAP(a00, a30); \
                MKL_DC_SWAP(a01, a31); \
                MKL_DC_SWAP(a02, a32); \
                MKL_DC_SWAP(a03, a33); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a11, a11, a10, a01); \
    MKL_DC_SUB_MUL(a21, a21, a20, a01); \
    MKL_DC_SUB_MUL(a31, a31, a30, a01); \
    MKL_DC_SUB_MUL(a12, a12, a10, a02); \
    MKL_DC_SUB_MUL(a22, a22, a20, a02); \
    MKL_DC_SUB_MUL(a32, a32, a30, a02); \
    MKL_DC_SUB_MUL(a13, a13, a10, a03); \
    MKL_DC_SUB_MUL(a23, a23, a20, a03); \
    MKL_DC_SUB_MUL(a33, a33, a30, a03); \
    pivot = MKL_DC_ABS1(a11); \
    jp = 1; \
    d = MKL_DC_ABS1(a21); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    d = MKL_DC_ABS1(a31); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    ipiv[1] = jp + 1; \
    switch (jp) { \
        case 1: \
            if (!MKL_DC_IS_ZERO(a11)) { \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a21)) { \
                MKL_DC_SWAP(a10, a20); \
                MKL_DC_SWAP(a11, a21); \
                MKL_DC_SWAP(a12, a22); \
                MKL_DC_SWAP(a13, a23); \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a31)) { \
                MKL_DC_SWAP(a10, a30); \
                MKL_DC_SWAP(a11, a31); \
                MKL_DC_SWAP(a12, a32); \
                MKL_DC_SWAP(a13, a33); \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a22, a22, a21, a12); \
    MKL_DC_SUB_MUL(a32, a32, a31, a12); \
    MKL_DC_SUB_MUL(a23, a23, a21, a13); \
    MKL_DC_SUB_MUL(a33, a33, a31, a13); \
    pivot = MKL_DC_ABS1(a22); \
    jp = 2; \
    d = MKL_DC_ABS1(a32); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    ipiv[2] = jp + 1; \
    switch (jp) { \
        case 2: \
            if (!MKL_DC_IS_ZERO(a22)) { \
                MKL_DC_DIV(a32, a32, a22); \
            } else if (*info == 0) { \
                *info = 3; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a32)) { \
                MKL_DC_SWAP(a20, a30); \
                MKL_DC_SWAP(a21, a31); \
                MKL_DC_SWAP(a22, a32); \
                MKL_DC_SWAP(a23, a33); \
                MKL_DC_DIV(a32, a32, a22); \
            } else if (*info == 0) { \
                *info = 3; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a33, a33, a32, a23); \
    ipiv[3] = 4; \
    if (MKL_DC_IS_ZERO(a33) && *info == 0) { \
        *info = 4; \
    } \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 3, 0) = a30; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 3, 1) = a31; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 2, 2) = a22; \
    a_access(a, lda, 3, 2) = a32; \
    a_access(a, lda, 0, 3) = a03; \
    a_access(a, lda, 1, 3) = a13; \
    a_access(a, lda, 2, 3) = a23; \
    a_access(a, lda, 3, 3) = a33; \
} while (0)

#define mkl_dc_getrf_5x5(m, n, a, lda, ipiv, info, a_access) \
{ \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a30 = a_access(a, lda, 3, 0); \
    mkl_dc_type a40 = a_access(a, lda, 4, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a31 = a_access(a, lda, 3, 1); \
    mkl_dc_type a41 = a_access(a, lda, 4, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type a32 = a_access(a, lda, 3, 2); \
    mkl_dc_type a42 = a_access(a, lda, 4, 2); \
    mkl_dc_type a03 = a_access(a, lda, 0, 3); \
    mkl_dc_type a13 = a_access(a, lda, 1, 3); \
    mkl_dc_type a23 = a_access(a, lda, 2, 3); \
    mkl_dc_type a33 = a_access(a, lda, 3, 3); \
    mkl_dc_type a43 = a_access(a, lda, 4, 3); \
    mkl_dc_type a04 = a_access(a, lda, 0, 4); \
    mkl_dc_type a14 = a_access(a, lda, 1, 4); \
    mkl_dc_type a24 = a_access(a, lda, 2, 4); \
    mkl_dc_type a34 = a_access(a, lda, 3, 4); \
    mkl_dc_type a44 = a_access(a, lda, 4, 4); \
    mkl_dc_real_type pivot = MKL_DC_ABS1(a00); \
    mkl_dc_real_type d = MKL_DC_ABS1(a10); \
    MKL_INT jp = 0; \
    *info = 0; \
    if (d > pivot) { \
        pivot = d; \
        jp = 1; \
    } \
    d = MKL_DC_ABS1(a20); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    d = MKL_DC_ABS1(a30); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    d = MKL_DC_ABS1(a40); \
    if (d > pivot) { \
        pivot = d; \
        jp = 4; \
    } \
    ipiv[0] = jp + 1; \
    switch (jp) { \
        case 0: \
            if (!MKL_DC_IS_ZERO(a00)) { \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
                MKL_DC_DIV(a40, a40, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 1: \
            if (!MKL_DC_IS_ZERO(a10)) { \
                MKL_DC_SWAP(a00, a10); \
                MKL_DC_SWAP(a01, a11); \
                MKL_DC_SWAP(a02, a12); \
                MKL_DC_SWAP(a03, a13); \
                MKL_DC_SWAP(a04, a14); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
                MKL_DC_DIV(a40, a40, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a20)) { \
                MKL_DC_SWAP(a00, a20); \
                MKL_DC_SWAP(a01, a21); \
                MKL_DC_SWAP(a02, a22); \
                MKL_DC_SWAP(a03, a23); \
                MKL_DC_SWAP(a04, a24); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
                MKL_DC_DIV(a40, a40, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a30)) { \
                MKL_DC_SWAP(a00, a30); \
                MKL_DC_SWAP(a01, a31); \
                MKL_DC_SWAP(a02, a32); \
                MKL_DC_SWAP(a03, a33); \
                MKL_DC_SWAP(a04, a34); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
                MKL_DC_DIV(a40, a40, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
        case 4: \
            if (!MKL_DC_IS_ZERO(a40)) { \
                MKL_DC_SWAP(a00, a40); \
                MKL_DC_SWAP(a01, a41); \
                MKL_DC_SWAP(a02, a42); \
                MKL_DC_SWAP(a03, a43); \
                MKL_DC_SWAP(a04, a44); \
                MKL_DC_DIV(a10, a10, a00); \
                MKL_DC_DIV(a20, a20, a00); \
                MKL_DC_DIV(a30, a30, a00); \
                MKL_DC_DIV(a40, a40, a00); \
            } else { \
                *info = 1; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a11, a11, a10, a01); \
    MKL_DC_SUB_MUL(a21, a21, a20, a01); \
    MKL_DC_SUB_MUL(a31, a31, a30, a01); \
    MKL_DC_SUB_MUL(a41, a41, a40, a01); \
    MKL_DC_SUB_MUL(a12, a12, a10, a02); \
    MKL_DC_SUB_MUL(a22, a22, a20, a02); \
    MKL_DC_SUB_MUL(a32, a32, a30, a02); \
    MKL_DC_SUB_MUL(a42, a42, a40, a02); \
    MKL_DC_SUB_MUL(a13, a13, a10, a03); \
    MKL_DC_SUB_MUL(a23, a23, a20, a03); \
    MKL_DC_SUB_MUL(a33, a33, a30, a03); \
    MKL_DC_SUB_MUL(a43, a43, a40, a03); \
    MKL_DC_SUB_MUL(a14, a14, a10, a04); \
    MKL_DC_SUB_MUL(a24, a24, a20, a04); \
    MKL_DC_SUB_MUL(a34, a34, a30, a04); \
    MKL_DC_SUB_MUL(a44, a44, a40, a04); \
    pivot = MKL_DC_ABS1(a11); \
    jp = 1; \
    d = MKL_DC_ABS1(a21); \
    if (d > pivot) { \
        pivot = d; \
        jp = 2; \
    } \
    d = MKL_DC_ABS1(a31); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    d = MKL_DC_ABS1(a41); \
    if (d > pivot) { \
        pivot = d; \
        jp = 4; \
    } \
    ipiv[1] = jp + 1; \
    switch (jp) { \
        case 1: \
            if (!MKL_DC_IS_ZERO(a11)) { \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
                MKL_DC_DIV(a41, a41, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 2: \
            if (!MKL_DC_IS_ZERO(a21)) { \
                MKL_DC_SWAP(a10, a20); \
                MKL_DC_SWAP(a11, a21); \
                MKL_DC_SWAP(a13, a23); \
                MKL_DC_SWAP(a14, a24); \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
                MKL_DC_DIV(a41, a41, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a31)) { \
                MKL_DC_SWAP(a10, a30); \
                MKL_DC_SWAP(a11, a31); \
                MKL_DC_SWAP(a12, a32); \
                MKL_DC_SWAP(a13, a33); \
                MKL_DC_SWAP(a14, a34); \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
                MKL_DC_DIV(a41, a41, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
        case 4: \
            if (!MKL_DC_IS_ZERO(a41)) { \
                MKL_DC_SWAP(a10, a40); \
                MKL_DC_SWAP(a11, a41); \
                MKL_DC_SWAP(a12, a42); \
                MKL_DC_SWAP(a13, a43); \
                MKL_DC_SWAP(a14, a44); \
                MKL_DC_DIV(a21, a21, a11); \
                MKL_DC_DIV(a31, a31, a11); \
                MKL_DC_DIV(a41, a41, a11); \
            } else if (*info == 0) { \
                *info = 2; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a22, a22, a21, a12); \
    MKL_DC_SUB_MUL(a32, a32, a31, a12); \
    MKL_DC_SUB_MUL(a42, a42, a41, a12); \
    MKL_DC_SUB_MUL(a23, a23, a21, a13); \
    MKL_DC_SUB_MUL(a33, a33, a31, a13); \
    MKL_DC_SUB_MUL(a43, a43, a41, a13); \
    MKL_DC_SUB_MUL(a24, a24, a21, a14); \
    MKL_DC_SUB_MUL(a34, a34, a31, a14); \
    MKL_DC_SUB_MUL(a44, a44, a41, a14); \
    pivot = MKL_DC_ABS1(a22); \
    jp = 2; \
    d = MKL_DC_ABS1(a32); \
    if (d > pivot) { \
        pivot = d; \
        jp = 3; \
    } \
    d = MKL_DC_ABS1(a42); \
    if (d > pivot) { \
        pivot = d; \
        jp = 4; \
    } \
    ipiv[2] = jp + 1; \
    switch (jp) { \
        case 2: \
            if (!MKL_DC_IS_ZERO(a22)) { \
                MKL_DC_DIV(a32, a32, a22); \
                MKL_DC_DIV(a42, a42, a22); \
            } else if (*info == 0) { \
                *info = 3; \
            } \
            break; \
        case 3: \
            if (!MKL_DC_IS_ZERO(a32)) { \
                MKL_DC_SWAP(a20, a30); \
                MKL_DC_SWAP(a21, a31); \
                MKL_DC_SWAP(a22, a32); \
                MKL_DC_SWAP(a23, a33); \
                MKL_DC_SWAP(a24, a34); \
                MKL_DC_DIV(a32, a32, a22); \
                MKL_DC_DIV(a42, a42, a22); \
            } else if (*info == 0) { \
                *info = 3; \
            } \
            break; \
        case 4: \
            if (!MKL_DC_IS_ZERO(a42)) { \
                MKL_DC_SWAP(a20, a40); \
                MKL_DC_SWAP(a21, a41); \
                MKL_DC_SWAP(a22, a42); \
                MKL_DC_SWAP(a23, a43); \
                MKL_DC_SWAP(a24, a44); \
                MKL_DC_DIV(a32, a32, a22); \
                MKL_DC_DIV(a42, a42, a22); \
            } else if (*info == 0) { \
                *info = 3; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a33, a33, a32, a23); \
    MKL_DC_SUB_MUL(a43, a43, a42, a23); \
    MKL_DC_SUB_MUL(a34, a34, a32, a24); \
    MKL_DC_SUB_MUL(a44, a44, a42, a24); \
    pivot = MKL_DC_ABS1(a33); \
    jp = 3; \
    d = MKL_DC_ABS1(a43); \
    if (d > pivot) { \
        pivot = d; \
        jp = 4; \
    } \
    ipiv[3] = jp + 1; \
    switch (jp) { \
        case 3: \
            if (!MKL_DC_IS_ZERO(a33)) { \
                MKL_DC_DIV(a43, a43, a33); \
            } else if (*info == 0) { \
                *info = 4; \
            } \
            break; \
        case 4: \
            if (!MKL_DC_IS_ZERO(a43)) { \
                MKL_DC_SWAP(a30, a40); \
                MKL_DC_SWAP(a31, a41); \
                MKL_DC_SWAP(a32, a42); \
                MKL_DC_SWAP(a33, a43); \
                MKL_DC_SWAP(a34, a44); \
                MKL_DC_DIV(a43, a43, a33); \
            } else if (*info == 0) { \
                *info = 4; \
            } \
            break; \
    } \
    MKL_DC_SUB_MUL(a44, a44, a43, a34); \
    ipiv[4] = 5; \
    if (MKL_DC_IS_ZERO(a44) && *info == 0) { \
        *info = 5; \
    } \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 3, 0) = a30; \
    a_access(a, lda, 4, 0) = a40; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 3, 1) = a31; \
    a_access(a, lda, 4, 1) = a41; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 2, 2) = a22; \
    a_access(a, lda, 3, 2) = a32; \
    a_access(a, lda, 4, 2) = a42; \
    a_access(a, lda, 0, 3) = a03; \
    a_access(a, lda, 1, 3) = a13; \
    a_access(a, lda, 2, 3) = a23; \
    a_access(a, lda, 3, 3) = a33; \
    a_access(a, lda, 4, 3) = a43; \
    a_access(a, lda, 0, 4) = a04; \
    a_access(a, lda, 1, 4) = a14; \
    a_access(a, lda, 2, 4) = a24; \
    a_access(a, lda, 3, 4) = a34; \
    a_access(a, lda, 4, 4) = a44; \
} while (0)

#define mkl_dc_getrf_generic(m, n, a, lda, ipiv, info, a_access) \
do { \
    if (m == n && m <= 5) { \
        switch (m) { \
            case 0: \
                break; \
            case 1: \
                mkl_dc_getrf_1x1(m, n, a, lda, ipiv, info, a_access); \
                break; \
            case 2: \
                mkl_dc_getrf_2x2(m, n, a, lda, ipiv, info, a_access); \
                break; \
            case 3: \
                mkl_dc_getrf_3x3(m, n, a, lda, ipiv, info, a_access); \
                break; \
            case 4: \
                mkl_dc_getrf_4x4(m, n, a, lda, ipiv, info, a_access); \
                break; \
            case 5: \
                mkl_dc_getrf_5x5(m, n, a, lda, ipiv, info, a_access); \
                break; \
        } \
    } else { \
        mkl_dc_getrf_mxn(m, n, a, lda, ipiv, info, a_access); \
    } \
} while (0)

static __inline void mkl_dc_getrf(MKL_INT m, MKL_INT n, mkl_dc_type* a, MKL_INT lda, MKL_INT* ipiv, MKL_INT* info)
{
    mkl_dc_getrf_generic(m, n, a, lda, ipiv, info, MKL_DC_MN);
}

#ifndef MKL_STDCALL
static __inline lapack_int mkl_dc_lapacke_getrf_convert(int matrix_layout, lapack_int m, lapack_int n, mkl_dc_type* a, lapack_int lda, lapack_int* ipiv)
{
    lapack_int info = 0;
    if (MKL_DC_GETRF_CHECKSIZE(m, n)) {
        if (matrix_layout == LAPACK_ROW_MAJOR) {
            mkl_dc_getrf_generic(m, n, a, lda, ipiv, &info, MKL_DC_MT);
        } else {
            mkl_dc_getrf_generic(m, n, a, lda, ipiv, &info, MKL_DC_MN);
        }
        return info;
    }
    return MKL_DC_CONCAT3(LAPACKE_, MKL_DC_PREC_LETTER, getrf)(matrix_layout, m, n, a, lda, ipiv);
}
#endif

/* ?GETRS */
#define mkl_dc_getrs_nxnrhs(trans, n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access) \
do { \
    MKL_INT i, j, k; \
    if (trans == 'N') { \
        for (i = 0; i < n; i++) { \
            MKL_INT ip = ipiv[i] - 1; \
            if (ip != i) { \
                for (k = 0; k < nrhs; k++) { \
                    MKL_DC_SWAP(b_access(b, ldb, i, k), b_access(b, ldb, ip, k)); \
                } \
            } \
        } \
        for (j = 0; j < nrhs; j++) { \
            for (k = 0; k < n; k++) { \
                if (!MKL_DC_IS_ZERO(b_access(b, ldb, k, j))) { \
                    for (i = k + 1; i < n; i++) { \
                        MKL_DC_SUB_MUL(b_access(b, ldb, i, j), b_access(b, ldb, i, j), b_access(b, ldb, k, j), a_access(a, lda, i, k)); \
                    } \
                } \
            } \
        } \
        for (j = 0; j < nrhs; j++) { \
            for (k = n - 1; k >= 0; k--) { \
                if (!MKL_DC_IS_ZERO(b_access(b, ldb, k, j))) { \
                    MKL_DC_DIV(b_access(b, ldb, k, j), b_access(b, ldb, k, j), a_access(a, lda, k, k)); \
                    for (i = 0; i < k; i++) { \
                        MKL_DC_SUB_MUL(b_access(b, ldb, i, j), b_access(b, ldb, i, j), b_access(b, ldb, k, j), a_access(a, lda, i, k)); \
                    } \
                } \
            } \
        } \
    } else { \
        for (j = 0; j < nrhs; j++) { \
            for (i = 0; i < n; i++) { \
                mkl_dc_type temp = b_access(b, ldb, i, j); \
                mkl_dc_type aii = a_access(a, lda, i, i); \
                for (k = 0; k < i; k++) { \
                    mkl_dc_type aki = a_access(a, lda, k, i); \
                    if (trans == 'C') { \
                        MKL_DC_CONJ(aki, aki); \
                    } \
                    MKL_DC_SUB_MUL(temp, temp, aki, b_access(b, ldb, k, j)); \
                } \
                if (trans == 'C') { \
                    MKL_DC_CONJ(aii, aii); \
                } \
                MKL_DC_DIV(b_access(b, ldb, i, j), temp, aii); \
            } \
        } \
        for (j = 0; j < nrhs; j++) { \
            for (i = n - 1; i >= 0; i--) { \
                mkl_dc_type temp = b_access(b, ldb, i, j); \
                for (k = i + 1; k < n; k++) { \
                    mkl_dc_type aki = a_access(a, lda, k, i); \
                    if (trans == 'C') { \
                        MKL_DC_CONJ(aki, aki); \
                    } \
                    MKL_DC_SUB_MUL(temp, temp, aki, b_access(b, ldb, k, j)); \
                }  \
                b_access(b, ldb, i, j) = temp; \
            } \
        } \
        for (i = n - 1; i >= 0; i--) { \
            MKL_INT ip = ipiv[i] - 1; \
            if (ip != i) { \
                for (k = 0; k < nrhs; k++) { \
                    MKL_DC_SWAP(b_access(b, ldb, i, k), b_access(b, ldb, ip, k)); \
                } \
            } \
        } \
    } \
} while (0)

#define mkl_dc_getrs_trans(trans, n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access) \
do { \
    switch (n) { \
        case 1: \
            mkl_dc_getrs_nxnrhs(trans, 1, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
        case 2: \
            mkl_dc_getrs_nxnrhs(trans, 2, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
        case 3: \
            mkl_dc_getrs_nxnrhs(trans, 3, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
        case 4: \
            mkl_dc_getrs_nxnrhs(trans, 4, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
        case 5: \
            mkl_dc_getrs_nxnrhs(trans, 5, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
        default: \
            mkl_dc_getrs_nxnrhs(trans, n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
            break; \
    } \
} while (0)

#define mkl_dc_getrs_generic(trans, n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access) \
do { \
    int ntrans = MKL_DC_MisN(trans); \
    int ttrans = MKL_DC_MisT(trans); \
    *info = 0; \
    if (n != 0 && nrhs != 0) { \
        if (ntrans) { \
            mkl_dc_getrs_trans('N', n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
        } else if (ttrans) { \
            mkl_dc_getrs_trans('T', n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
        } else { \
            mkl_dc_getrs_trans('C', n, nrhs, a, lda, ipiv, b, ldb, info, a_access, b_access); \
        } \
    } \
} while (0)

static __inline void mkl_dc_getrs(char trans, MKL_INT n, MKL_INT nrhs, const mkl_dc_type* a, MKL_INT lda,
                                  const MKL_INT* ipiv, mkl_dc_type* b, MKL_INT ldb, MKL_INT* info)
{
    mkl_dc_getrs_generic(trans, n, nrhs, a, lda, ipiv, b, ldb, info, MKL_DC_MN, MKL_DC_MN);
}

#ifndef MKL_STDCALL
static __inline lapack_int mkl_dc_lapacke_getrs_convert(int matrix_layout, char trans, lapack_int n, lapack_int nrhs,
                                                        const mkl_dc_type* a, lapack_int lda, const lapack_int* ipiv,
                                                        mkl_dc_type* b, lapack_int ldb)
{
    lapack_int info = 0;
    if (MKL_DC_GETRS_CHECKSIZE(n, nrhs)) {
        if (matrix_layout == LAPACK_ROW_MAJOR) {
            mkl_dc_getrs_generic(trans, n, nrhs, a, lda, ipiv, b, ldb, &info, MKL_DC_MT, MKL_DC_MT);
        } else {
            mkl_dc_getrs_generic(trans, n, nrhs, a, lda, ipiv, b, ldb, &info, MKL_DC_MN, MKL_DC_MN);
        }
        return info;
    }
    return MKL_DC_CONCAT3(LAPACKE_, MKL_DC_PREC_LETTER, getrs)(matrix_layout, trans, n, nrhs, a, lda, ipiv, b, ldb);
}
#endif

/* ?GETRI */
#define mkl_dc_getri_1x1(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    a_access(a, lda, 0, 0) = a00; \
} while (0)

#define mkl_dc_getri_2x2(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type w1; \
    mkl_dc_type tmp; \
    MKL_INT jp; \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a11)) { \
        *info = 2; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    MKL_DC_INV(a11, a11); \
    MKL_DC_NEG(ajj, a11); \
    tmp = a01; \
    MKL_DC_MUL(a01, a01, a00); \
    MKL_DC_MUL(a01, a01, ajj); \
    w1 = a10; \
    MKL_DC_SET_ZERO(a10); \
    MKL_DC_NEG(tmp, w1); \
    MKL_DC_MUL_ADD(a00, tmp, a01, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a11, a10); \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 1, 1) = a11; \
    jp = ipiv[0] - 1; \
    if (jp != 0) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 0), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 0), a_access(a, lda, 1, jp)); \
    } \
} while (0)

#define mkl_dc_getri_3x3(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type w1; \
    mkl_dc_type w2; \
    mkl_dc_type tmp; \
    MKL_INT jp; \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a11)) { \
        *info = 2; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a22)) { \
        *info = 3; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    MKL_DC_INV(a11, a11); \
    MKL_DC_NEG(ajj, a11); \
    tmp = a01; \
    MKL_DC_MUL(a01, a01, a00); \
    MKL_DC_MUL(a01, a01, ajj); \
    MKL_DC_INV(a22, a22); \
    MKL_DC_NEG(ajj, a22); \
    tmp = a02; \
    MKL_DC_MUL(a02, a02, a00); \
    tmp = a12; \
    MKL_DC_MUL_ADD(a02, tmp, a01, a02); \
    MKL_DC_MUL(a12, a12, a11); \
    MKL_DC_MUL(a02, a02, ajj); \
    MKL_DC_MUL(a12, a12, ajj); \
    w2 = a21; \
    MKL_DC_SET_ZERO(a21); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a01, tmp, a02, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a12, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a22, a21); \
    w1 = a10; \
    MKL_DC_SET_ZERO(a10); \
    w2 = a20; \
    MKL_DC_SET_ZERO(a20); \
    MKL_DC_NEG(tmp, w1); \
    MKL_DC_MUL_ADD(a00, tmp, a01, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a11, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a21, a20); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a00, tmp, a02, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a12, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a22, a20); \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 2, 2) = a22; \
    jp = ipiv[1] - 1; \
    if (jp != 1) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 1), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 1), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 1), a_access(a, lda, 2, jp)); \
    } \
    jp = ipiv[0] - 1; \
    if (jp != 0) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 0), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 0), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 0), a_access(a, lda, 2, jp)); \
    } \
} while (0)

#define mkl_dc_getri_4x4(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a03 = a_access(a, lda, 0, 3); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a13 = a_access(a, lda, 1, 3); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type a23 = a_access(a, lda, 2, 3); \
    mkl_dc_type a30 = a_access(a, lda, 3, 0); \
    mkl_dc_type a31 = a_access(a, lda, 3, 1); \
    mkl_dc_type a32 = a_access(a, lda, 3, 2); \
    mkl_dc_type a33 = a_access(a, lda, 3, 3); \
    mkl_dc_type w1; \
    mkl_dc_type w2; \
    mkl_dc_type w3; \
    mkl_dc_type tmp; \
    MKL_INT jp; \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a11)) { \
        *info = 2; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a22)) { \
        *info = 3; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a33)) { \
        *info = 4; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    MKL_DC_INV(a11, a11); \
    MKL_DC_NEG(ajj, a11); \
    tmp = a01; \
    MKL_DC_MUL(a01, a01, a00); \
    MKL_DC_MUL(a01, a01, ajj); \
    MKL_DC_INV(a22, a22); \
    MKL_DC_NEG(ajj, a22); \
    tmp = a02; \
    MKL_DC_MUL(a02, a02, a00); \
    tmp = a12; \
    MKL_DC_MUL_ADD(a02, tmp, a01, a02); \
    MKL_DC_MUL(a12, a12, a11); \
    MKL_DC_MUL(a02, a02, ajj); \
    MKL_DC_MUL(a12, a12, ajj); \
    MKL_DC_INV(a33, a33); \
    MKL_DC_NEG(ajj, a33); \
    tmp = a03; \
    MKL_DC_MUL(a03, a03, a00); \
    tmp = a13; \
    MKL_DC_MUL_ADD(a03, tmp, a01, a03); \
    MKL_DC_MUL(a13, a13, a11); \
    tmp = a23; \
    MKL_DC_MUL_ADD(a03, tmp, a02, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a12, a13); \
    MKL_DC_MUL(a23, a23, a22); \
    MKL_DC_MUL(a03, a03, ajj); \
    MKL_DC_MUL(a13, a13, ajj); \
    MKL_DC_MUL(a23, a23, ajj); \
    w3 = a32; \
    MKL_DC_SET_ZERO(a32); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a02, tmp, a03, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a13, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a23, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a33, a32); \
    w2 = a21; \
    MKL_DC_SET_ZERO(a21); \
    w3 = a31; \
    MKL_DC_SET_ZERO(a31); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a01, tmp, a02, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a12, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a22, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a32, a31); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a01, tmp, a03, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a13, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a23, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a33, a31); \
    w1 = a10; \
    MKL_DC_SET_ZERO(a10); \
    w2 = a20; \
    MKL_DC_SET_ZERO(a20); \
    w3 = a30; \
    MKL_DC_SET_ZERO(a30); \
    MKL_DC_NEG(tmp, w1); \
    MKL_DC_MUL_ADD(a00, tmp, a01, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a11, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a21, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a31, a30); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a00, tmp, a02, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a12, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a22, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a32, a30); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a00, tmp, a03, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a13, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a23, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a33, a30); \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 0, 3) = a03; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 1, 3) = a13; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 2, 2) = a22; \
    a_access(a, lda, 2, 3) = a23; \
    a_access(a, lda, 3, 0) = a30; \
    a_access(a, lda, 3, 1) = a31; \
    a_access(a, lda, 3, 2) = a32; \
    a_access(a, lda, 3, 3) = a33; \
    jp = ipiv[2] - 1; \
    if (jp != 2) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 2), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 2), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 2), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 2), a_access(a, lda, 3, jp)); \
    } \
    jp = ipiv[1] - 1; \
    if (jp != 1) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 1), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 1), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 1), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 1), a_access(a, lda, 3, jp)); \
    } \
    jp = ipiv[0] - 1; \
    if (jp != 0) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 0), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 0), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 0), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 0), a_access(a, lda, 3, jp)); \
    } \
} while (0)

#define mkl_dc_getri_5x5(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a03 = a_access(a, lda, 0, 3); \
    mkl_dc_type a04 = a_access(a, lda, 0, 4); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a13 = a_access(a, lda, 1, 3); \
    mkl_dc_type a14 = a_access(a, lda, 1, 4); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type a23 = a_access(a, lda, 2, 3); \
    mkl_dc_type a24 = a_access(a, lda, 2, 4); \
    mkl_dc_type a30 = a_access(a, lda, 3, 0); \
    mkl_dc_type a31 = a_access(a, lda, 3, 1); \
    mkl_dc_type a32 = a_access(a, lda, 3, 2); \
    mkl_dc_type a33 = a_access(a, lda, 3, 3); \
    mkl_dc_type a34 = a_access(a, lda, 3, 4); \
    mkl_dc_type a40 = a_access(a, lda, 4, 0); \
    mkl_dc_type a41 = a_access(a, lda, 4, 1); \
    mkl_dc_type a42 = a_access(a, lda, 4, 2); \
    mkl_dc_type a43 = a_access(a, lda, 4, 3); \
    mkl_dc_type a44 = a_access(a, lda, 4, 4); \
    mkl_dc_type w1; \
    mkl_dc_type w2; \
    mkl_dc_type w3; \
    mkl_dc_type w4; \
    mkl_dc_type tmp; \
    MKL_INT jp; \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a11)) { \
        *info = 2; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a22)) { \
        *info = 3; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a33)) { \
        *info = 4; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a44)) { \
        *info = 5; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    MKL_DC_INV(a11, a11); \
    MKL_DC_NEG(ajj, a11); \
    tmp = a01; \
    MKL_DC_MUL(a01, a01, a00); \
    MKL_DC_MUL(a01, a01, ajj); \
    MKL_DC_INV(a22, a22); \
    MKL_DC_NEG(ajj, a22); \
    tmp = a02; \
    MKL_DC_MUL(a02, a02, a00); \
    tmp = a12; \
    MKL_DC_MUL_ADD(a02, tmp, a01, a02); \
    MKL_DC_MUL(a12, a12, a11); \
    MKL_DC_MUL(a02, a02, ajj); \
    MKL_DC_MUL(a12, a12, ajj); \
    MKL_DC_INV(a33, a33); \
    MKL_DC_NEG(ajj, a33); \
    tmp = a03; \
    MKL_DC_MUL(a03, a03, a00); \
    tmp = a13; \
    MKL_DC_MUL_ADD(a03, tmp, a01, a03); \
    MKL_DC_MUL(a13, a13, a11); \
    tmp = a23; \
    MKL_DC_MUL_ADD(a03, tmp, a02, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a12, a13); \
    MKL_DC_MUL(a23, a23, a22); \
    MKL_DC_MUL(a03, a03, ajj); \
    MKL_DC_MUL(a13, a13, ajj); \
    MKL_DC_MUL(a23, a23, ajj); \
    MKL_DC_INV(a44, a44); \
    MKL_DC_NEG(ajj, a44); \
    tmp = a04; \
    MKL_DC_MUL(a04, a04, a00); \
    tmp = a14; \
    MKL_DC_MUL_ADD(a04, tmp, a01, a04); \
    MKL_DC_MUL(a14, a14, a11); \
    tmp = a24; \
    MKL_DC_MUL_ADD(a04, tmp, a02, a04); \
    MKL_DC_MUL_ADD(a14, tmp, a12, a14); \
    MKL_DC_MUL(a24, a24, a22); \
    tmp = a34; \
    MKL_DC_MUL_ADD(a04, tmp, a03, a04); \
    MKL_DC_MUL_ADD(a14, tmp, a13, a14); \
    MKL_DC_MUL_ADD(a24, tmp, a23, a24); \
    MKL_DC_MUL(a34, a34, a33); \
    MKL_DC_MUL(a04, a04, ajj); \
    MKL_DC_MUL(a14, a14, ajj); \
    MKL_DC_MUL(a24, a24, ajj); \
    MKL_DC_MUL(a34, a34, ajj); \
    w4 = a43; \
    MKL_DC_SET_ZERO(a43); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a03, tmp, a04, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a14, a13); \
    MKL_DC_MUL_ADD(a23, tmp, a24, a23); \
    MKL_DC_MUL_ADD(a33, tmp, a34, a33); \
    MKL_DC_MUL_ADD(a43, tmp, a44, a43); \
    w3 = a32; \
    MKL_DC_SET_ZERO(a32); \
    w4 = a42; \
    MKL_DC_SET_ZERO(a42); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a02, tmp, a03, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a13, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a23, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a33, a32); \
    MKL_DC_MUL_ADD(a42, tmp, a43, a42); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a02, tmp, a04, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a14, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a24, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a34, a32); \
    MKL_DC_MUL_ADD(a42, tmp, a44, a42); \
    w2 = a21; \
    MKL_DC_SET_ZERO(a21); \
    w3 = a31; \
    MKL_DC_SET_ZERO(a31); \
    w4 = a41; \
    MKL_DC_SET_ZERO(a41); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a01, tmp, a02, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a12, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a22, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a32, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a42, a41); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a01, tmp, a03, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a13, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a23, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a33, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a43, a41); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a01, tmp, a04, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a14, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a24, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a34, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a44, a41); \
    w1 = a10; \
    MKL_DC_SET_ZERO(a10); \
    w2 = a20; \
    MKL_DC_SET_ZERO(a20); \
    w3 = a30; \
    MKL_DC_SET_ZERO(a30); \
    w4 = a40; \
    MKL_DC_SET_ZERO(a40); \
    MKL_DC_NEG(tmp, w1); \
    MKL_DC_MUL_ADD(a00, tmp, a01, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a11, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a21, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a31, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a41, a40); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a00, tmp, a02, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a12, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a22, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a32, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a42, a40); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a00, tmp, a03, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a13, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a23, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a33, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a43, a40); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a00, tmp, a04, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a14, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a24, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a34, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a44, a40); \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 0, 3) = a03; \
    a_access(a, lda, 0, 4) = a04; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 1, 3) = a13; \
    a_access(a, lda, 1, 4) = a14; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 2, 2) = a22; \
    a_access(a, lda, 2, 3) = a23; \
    a_access(a, lda, 2, 4) = a24; \
    a_access(a, lda, 3, 0) = a30; \
    a_access(a, lda, 3, 1) = a31; \
    a_access(a, lda, 3, 2) = a32; \
    a_access(a, lda, 3, 3) = a33; \
    a_access(a, lda, 3, 4) = a34; \
    a_access(a, lda, 4, 0) = a40; \
    a_access(a, lda, 4, 1) = a41; \
    a_access(a, lda, 4, 2) = a42; \
    a_access(a, lda, 4, 3) = a43; \
    a_access(a, lda, 4, 4) = a44; \
    jp = ipiv[3] - 1; \
    if (jp != 3) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 3), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 3), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 3), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 3), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 3), a_access(a, lda, 4, jp)); \
    } \
    jp = ipiv[2] - 1; \
    if (jp != 2) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 2), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 2), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 2), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 2), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 2), a_access(a, lda, 4, jp)); \
    } \
    jp = ipiv[1] - 1; \
    if (jp != 1) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 1), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 1), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 1), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 1), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 1), a_access(a, lda, 4, jp)); \
    } \
    jp = ipiv[0] - 1; \
    if (jp != 0) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 0), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 0), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 0), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 0), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 0), a_access(a, lda, 4, jp)); \
    } \
} while (0)

#define mkl_dc_getri_6x6(n, a, lda, ipiv, info, a_access) \
do { \
    mkl_dc_type a00 = a_access(a, lda, 0, 0); \
    mkl_dc_type a01 = a_access(a, lda, 0, 1); \
    mkl_dc_type a02 = a_access(a, lda, 0, 2); \
    mkl_dc_type a03 = a_access(a, lda, 0, 3); \
    mkl_dc_type a04 = a_access(a, lda, 0, 4); \
    mkl_dc_type a05 = a_access(a, lda, 0, 5); \
    mkl_dc_type a10 = a_access(a, lda, 1, 0); \
    mkl_dc_type a11 = a_access(a, lda, 1, 1); \
    mkl_dc_type a12 = a_access(a, lda, 1, 2); \
    mkl_dc_type a13 = a_access(a, lda, 1, 3); \
    mkl_dc_type a14 = a_access(a, lda, 1, 4); \
    mkl_dc_type a15 = a_access(a, lda, 1, 5); \
    mkl_dc_type a20 = a_access(a, lda, 2, 0); \
    mkl_dc_type a21 = a_access(a, lda, 2, 1); \
    mkl_dc_type a22 = a_access(a, lda, 2, 2); \
    mkl_dc_type a23 = a_access(a, lda, 2, 3); \
    mkl_dc_type a24 = a_access(a, lda, 2, 4); \
    mkl_dc_type a25 = a_access(a, lda, 2, 5); \
    mkl_dc_type a30 = a_access(a, lda, 3, 0); \
    mkl_dc_type a31 = a_access(a, lda, 3, 1); \
    mkl_dc_type a32 = a_access(a, lda, 3, 2); \
    mkl_dc_type a33 = a_access(a, lda, 3, 3); \
    mkl_dc_type a34 = a_access(a, lda, 3, 4); \
    mkl_dc_type a35 = a_access(a, lda, 3, 5); \
    mkl_dc_type a40 = a_access(a, lda, 4, 0); \
    mkl_dc_type a41 = a_access(a, lda, 4, 1); \
    mkl_dc_type a42 = a_access(a, lda, 4, 2); \
    mkl_dc_type a43 = a_access(a, lda, 4, 3); \
    mkl_dc_type a44 = a_access(a, lda, 4, 4); \
    mkl_dc_type a45 = a_access(a, lda, 4, 5); \
    mkl_dc_type a50 = a_access(a, lda, 5, 0); \
    mkl_dc_type a51 = a_access(a, lda, 5, 1); \
    mkl_dc_type a52 = a_access(a, lda, 5, 2); \
    mkl_dc_type a53 = a_access(a, lda, 5, 3); \
    mkl_dc_type a54 = a_access(a, lda, 5, 4); \
    mkl_dc_type a55 = a_access(a, lda, 5, 5); \
    mkl_dc_type w1; \
    mkl_dc_type w2; \
    mkl_dc_type w3; \
    mkl_dc_type w4; \
    mkl_dc_type w5; \
    mkl_dc_type tmp; \
    MKL_INT jp; \
    mkl_dc_type ajj; \
    if (MKL_DC_IS_ZERO(a00)) { \
        *info = 1; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a11)) { \
        *info = 2; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a22)) { \
        *info = 3; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a33)) { \
        *info = 4; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a44)) { \
        *info = 5; \
        break; \
    } \
    if (MKL_DC_IS_ZERO(a55)) { \
        *info = 6; \
        break; \
    } \
    *info = 0; \
    MKL_DC_INV(a00, a00); \
    MKL_DC_NEG(ajj, a00); \
    MKL_DC_INV(a11, a11); \
    MKL_DC_NEG(ajj, a11); \
    tmp = a01; \
    MKL_DC_MUL(a01, a01, a00); \
    MKL_DC_MUL(a01, a01, ajj); \
    MKL_DC_INV(a22, a22); \
    MKL_DC_NEG(ajj, a22); \
    tmp = a02; \
    MKL_DC_MUL(a02, a02, a00); \
    tmp = a12; \
    MKL_DC_MUL_ADD(a02, tmp, a01, a02); \
    MKL_DC_MUL(a12, a12, a11); \
    MKL_DC_MUL(a02, a02, ajj); \
    MKL_DC_MUL(a12, a12, ajj); \
    MKL_DC_INV(a33, a33); \
    MKL_DC_NEG(ajj, a33); \
    tmp = a03; \
    MKL_DC_MUL(a03, a03, a00); \
    tmp = a13; \
    MKL_DC_MUL_ADD(a03, tmp, a01, a03); \
    MKL_DC_MUL(a13, a13, a11); \
    tmp = a23; \
    MKL_DC_MUL_ADD(a03, tmp, a02, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a12, a13); \
    MKL_DC_MUL(a23, a23, a22); \
    MKL_DC_MUL(a03, a03, ajj); \
    MKL_DC_MUL(a13, a13, ajj); \
    MKL_DC_MUL(a23, a23, ajj); \
    MKL_DC_INV(a44, a44); \
    MKL_DC_NEG(ajj, a44); \
    tmp = a04; \
    MKL_DC_MUL(a04, a04, a00); \
    tmp = a14; \
    MKL_DC_MUL_ADD(a04, tmp, a01, a04); \
    MKL_DC_MUL(a14, a14, a11); \
    tmp = a24; \
    MKL_DC_MUL_ADD(a04, tmp, a02, a04); \
    MKL_DC_MUL_ADD(a14, tmp, a12, a14); \
    MKL_DC_MUL(a24, a24, a22); \
    tmp = a34; \
    MKL_DC_MUL_ADD(a04, tmp, a03, a04); \
    MKL_DC_MUL_ADD(a14, tmp, a13, a14); \
    MKL_DC_MUL_ADD(a24, tmp, a23, a24); \
    MKL_DC_MUL(a34, a34, a33); \
    MKL_DC_MUL(a04, a04, ajj); \
    MKL_DC_MUL(a14, a14, ajj); \
    MKL_DC_MUL(a24, a24, ajj); \
    MKL_DC_MUL(a34, a34, ajj); \
    MKL_DC_INV(a55, a55); \
    MKL_DC_NEG(ajj, a55); \
    tmp = a05; \
    MKL_DC_MUL(a05, a05, a00); \
    tmp = a15; \
    MKL_DC_MUL_ADD(a05, tmp, a01, a05); \
    MKL_DC_MUL(a15, a15, a11); \
    tmp = a25; \
    MKL_DC_MUL_ADD(a05, tmp, a02, a05); \
    MKL_DC_MUL_ADD(a15, tmp, a12, a15); \
    MKL_DC_MUL(a25, a25, a22); \
    tmp = a35; \
    MKL_DC_MUL_ADD(a05, tmp, a03, a05); \
    MKL_DC_MUL_ADD(a15, tmp, a13, a15); \
    MKL_DC_MUL_ADD(a25, tmp, a23, a25); \
    MKL_DC_MUL(a35, a35, a33); \
    tmp = a45; \
    MKL_DC_MUL_ADD(a05, tmp, a04, a05); \
    MKL_DC_MUL_ADD(a15, tmp, a14, a15); \
    MKL_DC_MUL_ADD(a25, tmp, a24, a25); \
    MKL_DC_MUL_ADD(a35, tmp, a34, a35); \
    MKL_DC_MUL(a45, a45, a44); \
    MKL_DC_MUL(a05, a05, ajj); \
    MKL_DC_MUL(a15, a15, ajj); \
    MKL_DC_MUL(a25, a25, ajj); \
    MKL_DC_MUL(a35, a35, ajj); \
    MKL_DC_MUL(a45, a45, ajj); \
    w5 = a54; \
    MKL_DC_SET_ZERO(a54); \
    MKL_DC_NEG(tmp, w5); \
    MKL_DC_MUL_ADD(a04, tmp, a05, a04); \
    MKL_DC_MUL_ADD(a14, tmp, a15, a14); \
    MKL_DC_MUL_ADD(a24, tmp, a25, a24); \
    MKL_DC_MUL_ADD(a34, tmp, a35, a34); \
    MKL_DC_MUL_ADD(a44, tmp, a45, a44); \
    MKL_DC_MUL_ADD(a54, tmp, a55, a54); \
    w4 = a43; \
    MKL_DC_SET_ZERO(a43); \
    w5 = a53; \
    MKL_DC_SET_ZERO(a53); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a03, tmp, a04, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a14, a13); \
    MKL_DC_MUL_ADD(a23, tmp, a24, a23); \
    MKL_DC_MUL_ADD(a33, tmp, a34, a33); \
    MKL_DC_MUL_ADD(a43, tmp, a44, a43); \
    MKL_DC_MUL_ADD(a53, tmp, a54, a53); \
    MKL_DC_NEG(tmp, w5); \
    MKL_DC_MUL_ADD(a03, tmp, a05, a03); \
    MKL_DC_MUL_ADD(a13, tmp, a15, a13); \
    MKL_DC_MUL_ADD(a23, tmp, a25, a23); \
    MKL_DC_MUL_ADD(a33, tmp, a35, a33); \
    MKL_DC_MUL_ADD(a43, tmp, a45, a43); \
    MKL_DC_MUL_ADD(a53, tmp, a55, a53); \
    w3 = a32; \
    MKL_DC_SET_ZERO(a32); \
    w4 = a42; \
    MKL_DC_SET_ZERO(a42); \
    w5 = a52; \
    MKL_DC_SET_ZERO(a52); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a02, tmp, a03, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a13, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a23, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a33, a32); \
    MKL_DC_MUL_ADD(a42, tmp, a43, a42); \
    MKL_DC_MUL_ADD(a52, tmp, a53, a52); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a02, tmp, a04, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a14, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a24, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a34, a32); \
    MKL_DC_MUL_ADD(a42, tmp, a44, a42); \
    MKL_DC_MUL_ADD(a52, tmp, a54, a52); \
    MKL_DC_NEG(tmp, w5); \
    MKL_DC_MUL_ADD(a02, tmp, a05, a02); \
    MKL_DC_MUL_ADD(a12, tmp, a15, a12); \
    MKL_DC_MUL_ADD(a22, tmp, a25, a22); \
    MKL_DC_MUL_ADD(a32, tmp, a35, a32); \
    MKL_DC_MUL_ADD(a42, tmp, a45, a42); \
    MKL_DC_MUL_ADD(a52, tmp, a55, a52); \
    w2 = a21; \
    MKL_DC_SET_ZERO(a21); \
    w3 = a31; \
    MKL_DC_SET_ZERO(a31); \
    w4 = a41; \
    MKL_DC_SET_ZERO(a41); \
    w5 = a51; \
    MKL_DC_SET_ZERO(a51); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a01, tmp, a02, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a12, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a22, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a32, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a42, a41); \
    MKL_DC_MUL_ADD(a51, tmp, a52, a51); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a01, tmp, a03, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a13, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a23, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a33, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a43, a41); \
    MKL_DC_MUL_ADD(a51, tmp, a53, a51); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a01, tmp, a04, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a14, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a24, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a34, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a44, a41); \
    MKL_DC_MUL_ADD(a51, tmp, a54, a51); \
    MKL_DC_NEG(tmp, w5); \
    MKL_DC_MUL_ADD(a01, tmp, a05, a01); \
    MKL_DC_MUL_ADD(a11, tmp, a15, a11); \
    MKL_DC_MUL_ADD(a21, tmp, a25, a21); \
    MKL_DC_MUL_ADD(a31, tmp, a35, a31); \
    MKL_DC_MUL_ADD(a41, tmp, a45, a41); \
    MKL_DC_MUL_ADD(a51, tmp, a55, a51); \
    w1 = a10; \
    MKL_DC_SET_ZERO(a10); \
    w2 = a20; \
    MKL_DC_SET_ZERO(a20); \
    w3 = a30; \
    MKL_DC_SET_ZERO(a30); \
    w4 = a40; \
    MKL_DC_SET_ZERO(a40); \
    w5 = a50; \
    MKL_DC_SET_ZERO(a50); \
    MKL_DC_NEG(tmp, w1); \
    MKL_DC_MUL_ADD(a00, tmp, a01, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a11, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a21, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a31, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a41, a40); \
    MKL_DC_MUL_ADD(a50, tmp, a51, a50); \
    MKL_DC_NEG(tmp, w2); \
    MKL_DC_MUL_ADD(a00, tmp, a02, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a12, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a22, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a32, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a42, a40); \
    MKL_DC_MUL_ADD(a50, tmp, a52, a50); \
    MKL_DC_NEG(tmp, w3); \
    MKL_DC_MUL_ADD(a00, tmp, a03, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a13, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a23, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a33, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a43, a40); \
    MKL_DC_MUL_ADD(a50, tmp, a53, a50); \
    MKL_DC_NEG(tmp, w4); \
    MKL_DC_MUL_ADD(a00, tmp, a04, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a14, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a24, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a34, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a44, a40); \
    MKL_DC_MUL_ADD(a50, tmp, a54, a50); \
    MKL_DC_NEG(tmp, w5); \
    MKL_DC_MUL_ADD(a00, tmp, a05, a00); \
    MKL_DC_MUL_ADD(a10, tmp, a15, a10); \
    MKL_DC_MUL_ADD(a20, tmp, a25, a20); \
    MKL_DC_MUL_ADD(a30, tmp, a35, a30); \
    MKL_DC_MUL_ADD(a40, tmp, a45, a40); \
    MKL_DC_MUL_ADD(a50, tmp, a55, a50); \
    a_access(a, lda, 0, 0) = a00; \
    a_access(a, lda, 0, 1) = a01; \
    a_access(a, lda, 0, 2) = a02; \
    a_access(a, lda, 0, 3) = a03; \
    a_access(a, lda, 0, 4) = a04; \
    a_access(a, lda, 0, 5) = a05; \
    a_access(a, lda, 1, 0) = a10; \
    a_access(a, lda, 1, 1) = a11; \
    a_access(a, lda, 1, 2) = a12; \
    a_access(a, lda, 1, 3) = a13; \
    a_access(a, lda, 1, 4) = a14; \
    a_access(a, lda, 1, 5) = a15; \
    a_access(a, lda, 2, 0) = a20; \
    a_access(a, lda, 2, 1) = a21; \
    a_access(a, lda, 2, 2) = a22; \
    a_access(a, lda, 2, 3) = a23; \
    a_access(a, lda, 2, 4) = a24; \
    a_access(a, lda, 2, 5) = a25; \
    a_access(a, lda, 3, 0) = a30; \
    a_access(a, lda, 3, 1) = a31; \
    a_access(a, lda, 3, 2) = a32; \
    a_access(a, lda, 3, 3) = a33; \
    a_access(a, lda, 3, 4) = a34; \
    a_access(a, lda, 3, 5) = a35; \
    a_access(a, lda, 4, 0) = a40; \
    a_access(a, lda, 4, 1) = a41; \
    a_access(a, lda, 4, 2) = a42; \
    a_access(a, lda, 4, 3) = a43; \
    a_access(a, lda, 4, 4) = a44; \
    a_access(a, lda, 4, 5) = a45; \
    a_access(a, lda, 5, 0) = a50; \
    a_access(a, lda, 5, 1) = a51; \
    a_access(a, lda, 5, 2) = a52; \
    a_access(a, lda, 5, 3) = a53; \
    a_access(a, lda, 5, 4) = a54; \
    a_access(a, lda, 5, 5) = a55; \
    jp = ipiv[4] - 1; \
    if (jp != 4) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 4), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 4), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 4), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 4), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 4), a_access(a, lda, 4, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 5, 4), a_access(a, lda, 5, jp)); \
    } \
    jp = ipiv[3] - 1; \
    if (jp != 3) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 3), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 3), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 3), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 3), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 3), a_access(a, lda, 4, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 5, 3), a_access(a, lda, 5, jp)); \
    } \
    jp = ipiv[2] - 1; \
    if (jp != 2) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 2), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 2), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 2), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 2), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 2), a_access(a, lda, 4, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 5, 2), a_access(a, lda, 5, jp)); \
    } \
    jp = ipiv[1] - 1; \
    if (jp != 1) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 1), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 1), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 1), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 1), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 1), a_access(a, lda, 4, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 5, 1), a_access(a, lda, 5, jp)); \
    } \
    jp = ipiv[0] - 1; \
    if (jp != 0) { \
        MKL_DC_SWAP(a_access(a, lda, 0, 0), a_access(a, lda, 0, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 1, 0), a_access(a, lda, 1, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 2, 0), a_access(a, lda, 2, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 3, 0), a_access(a, lda, 3, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 4, 0), a_access(a, lda, 4, jp)); \
        MKL_DC_SWAP(a_access(a, lda, 5, 0), a_access(a, lda, 5, jp)); \
    } \
} while (0)

#define mkl_dc_getri_inverse_upper(n, a, lda, a_access) \
do { \
    MKL_INT i, j, ii, jj; \
    mkl_dc_type ajj; \
    for (j = 0; j < n; j++) { \
        MKL_DC_INV(a_access(a, lda, j, j), a_access(a, lda, j, j)); \
        MKL_DC_NEG(ajj, a_access(a, lda, j, j)); \
        for (jj = 0; jj < j; jj++) { \
            mkl_dc_type tmp = a_access(a, lda, jj, j); \
            for (ii = 0; ii < jj; ii++) { \
                MKL_DC_MUL_ADD(a_access(a, lda, ii, j), tmp, a_access(a, lda, ii, jj), a_access(a, lda, ii, j)); \
            } \
            MKL_DC_MUL(a_access(a, lda, jj, j), a_access(a, lda, jj, j), a_access(a, lda, jj, jj)); \
        } \
        for (i = 0; i < j; i++) { \
            MKL_DC_MUL(a_access(a, lda, i, j), a_access(a, lda, i, j), ajj); \
        } \
    } \
} while (0)

#define mkl_dc_getri_solve_lower(n, a, lda, work, a_access) \
do { \
    MKL_INT i, j, ii, jj; \
    mkl_dc_type tmp; \
    for (j = n - 1; j >= 0; j--) { \
        for (i = j + 1; i < n; i++) { \
            work[i] = a_access(a, lda, i, j); \
            MKL_DC_SET_ZERO(a_access(a, lda, i, j)); \
        } \
        for (jj = j + 1; jj < n; jj++) { \
            MKL_DC_NEG(tmp, work[jj]); \
            for (ii = 0; ii < n; ii++) { \
                MKL_DC_MUL_ADD(a_access(a, lda, ii, j), tmp, a_access(a, lda, ii, jj), a_access(a, lda, ii, j)); \
            } \
        } \
    } \
} while (0)

#define mkl_dc_getri_n(n, a, lda, ipiv, work, lwork, info, a_access) \
do { \
    MKL_INT i, j, jp; \
    *info = 0; \
    for (i = 0; i < n; i++) { \
        if (MKL_DC_IS_ZERO(a_access(a, lda, i, i))) { \
            *info = i + 1; \
            break; \
        } \
    } \
    if (*info == 0) { \
        mkl_dc_getri_inverse_upper(n, a, lda, a_access); \
        mkl_dc_getri_solve_lower(n, a, lda, work, a_access); \
        for (j = n - 2; j >= 0; j--) { \
            jp = ipiv[j] - 1; \
            if (jp != j) { \
                for (i = 0; i < n; i++) { \
                    MKL_DC_SWAP(a_access(a, lda, i, j), a_access(a, lda, i, jp)); \
                } \
            } \
        } \
    } \
} while (0)

#define mkl_dc_getri_generic(n, a, lda, ipiv, work, lwork, info, a_access) \
do { \
    if (lwork != -1) { \
        switch (n) { \
            case 0: \
                break; \
            case 1: \
                mkl_dc_getri_1x1(1, a, lda, ipiv, info, a_access); \
                break; \
            case 2: \
                mkl_dc_getri_2x2(2, a, lda, ipiv, info, a_access); \
                break; \
            case 3: \
                mkl_dc_getri_3x3(3, a, lda, ipiv, info, a_access); \
                break; \
            case 4: \
                mkl_dc_getri_4x4(4, a, lda, ipiv, info, a_access); \
                break; \
            case 5: \
                mkl_dc_getri_5x5(5, a, lda, ipiv, info, a_access); \
                break; \
            case 6: \
                mkl_dc_getri_6x6(6, a, lda, ipiv, info, a_access); \
                break; \
            default: \
                mkl_dc_getri_n(n, a, lda, ipiv, work, lwork, info, a_access); \
                break; \
        } \
    } \
    MKL_DC_CONVERT_INT(work[0], MKL_DC_MAX(1, n)); \
} while (0)

static __inline void mkl_dc_getri(MKL_INT n, mkl_dc_type* a, MKL_INT lda, const MKL_INT* ipiv,
                                  mkl_dc_type* work, MKL_INT lwork, MKL_INT* info)
{
    mkl_dc_getri_generic(n, a, lda, ipiv, work, lwork, info, MKL_DC_MN);
}

#ifndef MKL_STDCALL
static __inline lapack_int mkl_dc_lapacke_getri_convert(int matrix_layout, lapack_int n, mkl_dc_type* a,
                                                        lapack_int lda, const lapack_int* ipiv,
                                                        mkl_dc_type* work, lapack_int lwork)
{
    lapack_int info = 0;
    if (MKL_DC_GETRI_CHECKSIZE(n)) {
        mkl_dc_type work_small[10];
        lapack_int lwork_small = 10;
        if (!work) {
            work = work_small;
            lwork = lwork_small;
        }
        if (matrix_layout == LAPACK_ROW_MAJOR) {
            mkl_dc_getri_generic(n, a, lda, ipiv, work, lwork, &info, MKL_DC_MT);
        } else {
            mkl_dc_getri_generic(n, a, lda, ipiv, work, lwork, &info, MKL_DC_MN);
        }
        return info;
    }
    if (work) {
        return MKL_DC_CONCAT3(LAPACKE_, MKL_DC_PREC_LETTER, getri_work)(matrix_layout, n, a, lda, ipiv, work, lwork);
    }
    return MKL_DC_CONCAT3(LAPACKE_, MKL_DC_PREC_LETTER, getri)(matrix_layout, n, a, lda, ipiv);
}
#endif
