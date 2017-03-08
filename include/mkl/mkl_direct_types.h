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
!      Intel(R) Math Kernel Library (Intel(R) MKL) types definition for functions
!      than can be inlined
!******************************************************************************/
#include "mkl_types.h"

#undef mkl_dc_type
#undef mkl_dc_real_type
#undef mkl_dc_native_type
#undef MKL_REAL_DATA_TYPE
#undef mkl_dc_veclen
#undef MKL_DC_PREC_LETTER
#undef MKL_DC_IS_ONE
#undef MKL_DC_IS_ZERO
#undef MKL_DC_ABS1
#undef MKL_DC_SET_ONE
#undef MKL_DC_SET_ZERO
#undef MKL_DC_ADD
#undef MKL_DC_SUB
#undef MKL_DC_CONJ
#undef MKL_DC_DIV
#undef MKL_DC_SUB_MUL
#undef MKL_DC_MUL
#undef MKL_DC_MUL_ADD
#undef MKL_DC_ZERO_C
#undef MKL_DC_MUL_C
#undef MKL_DC_DIV_B
#undef MKL_DC_SWAP
#undef MKL_DC_NEG
#undef MKL_DC_INV
#undef MKL_DC_CONVERT_INT

#if defined(MKL_DOUBLE)
#define mkl_dc_type double
#define mkl_dc_real_type double
#define mkl_dc_native_type double
#define MKL_REAL_DATA_TYPE
#define mkl_dc_veclen 4
#define MKL_DC_PREC_LETTER d

#elif defined(MKL_SINGLE)
#define mkl_dc_type float
#define mkl_dc_real_type float
#define mkl_dc_native_type float
#define MKL_REAL_DATA_TYPE
#define mkl_dc_veclen 8
#define MKL_DC_PREC_LETTER s

#elif defined(MKL_COMPLEX)
#define mkl_dc_type MKL_Complex8
#define mkl_dc_real_type float
#define mkl_dc_native_type float _Complex
#define mkl_dc_veclen 4
#define MKL_DC_PREC_LETTER c

#elif defined(MKL_COMPLEX16)
#define mkl_dc_type MKL_Complex16
#define mkl_dc_real_type double
#define mkl_dc_native_type double _Complex
#define mkl_dc_veclen 2
#define MKL_DC_PREC_LETTER z

#endif

#if defined(MKL_COMPLEX) || defined(MKL_COMPLEX16)
#define MKL_DC_ABS1(x) (MKL_DC_ABS((x).real) + MKL_DC_ABS((x).imag))
#define MKL_DC_SET_ONE(x) ((x).real = 1.0, (x).imag = 0.0)
#define MKL_DC_IS_ONE(x) ((x).real == 1.0 && (x).imag == 0.0)
#define MKL_DC_IS_ZERO(x) ((x).real == 0.0 && (x).imag == 0.0)
#define MKL_DC_SET_ZERO(x) ((x).real = 0.0, (x).imag = 0.0)
#define MKL_DC_ADD(x,y,z) do { \
	(x).real = (y).real + (z).real; \
	(x).imag = (y).imag + (z).imag; \
} while (0)
#define MKL_DC_SUB(x,y,z) do { \
	(x).real = (y).real - (z).real; \
	(x).imag = (y).imag - (z).imag; \
} while (0)
#define MKL_DC_CONJ(x,y) ((x).real = (y).real, (x).imag = - (y).imag)
#define MKL_DC_MUL(z,x,y) do { \
	mkl_dc_type t; \
	t.real = (x).real * (y).real - (x).imag * (y).imag; \
	t.imag = (x).real * (y).imag + (x).imag * (y).real; \
	(z) = t; \
} while (0)
#define MKL_DC_MUL_ADD(r,x,y,z) do { \
	mkl_dc_type t; \
	t.real = (x).real * (y).real - (x).imag * (y).imag; \
	t.imag = (x).real * (y).imag + (x).imag * (y).real; \
	(r).real = t.real + (z).real; \
	(r).imag = t.imag + (z).imag; \
} while (0)
#define MKL_DC_DIV(z,x,y) do { \
	mkl_dc_type t; \
	t.real = ((x).real * (y).real + (x).imag * (y).imag) / ((y).real * (y).real + (y).imag * (y).imag); \
	t.imag = ((x).imag * (y).real - (x).real * (y).imag) / ((y).real * (y).real + (y).imag * (y).imag); \
	(z) = t; \
} while (0)
#define MKL_DC_SUB_MUL(r,x,y,z) do { \
	mkl_dc_type t; \
	t.real = (y).real * (z).real - (y).imag * (z).imag; \
	t.imag = (y).real * (z).imag + (y).imag * (z).real; \
	(r).real = (x).real - t.real; \
	(r).imag = (x).imag - t.imag; \
} while (0)
#define MKL_DC_SWAP(x, y) do { \
    mkl_dc_type t; \
    t.real = x.real; \
    t.imag = x.imag; \
    x.real = y.real; \
    x.imag = y.imag; \
    y.real = t.real; \
    y.imag = t.imag; \
} while (0)
#define MKL_DC_NEG(x, y) ((x).real = -(y).real, (x).imag = -(y).imag)
#define MKL_DC_INV(x, y) do { \
    mkl_dc_type one; \
    MKL_DC_SET_ONE(one); \
    MKL_DC_DIV(x, one, y); \
} while (0)
#define MKL_DC_CONVERT_INT(x, y) do { \
    x.real = (mkl_dc_real_type)(y); \
    x.imag = (mkl_dc_real_type)(0); \
} while (0)
 
#else
#define MKL_DC_ABS1(x) MKL_DC_ABS(x)
#define MKL_DC_SET_ONE(x) ((x) = 1.0)
#define MKL_DC_IS_ONE(x) ((x) == 1.0)
#define MKL_DC_IS_ZERO(x) ((x) == 0.0)
#define MKL_DC_SET_ZERO(x) ((x) = 0.0)
#define MKL_DC_ADD(x,y,z) ((x) = (y) + (z))
#define MKL_DC_SUB(x,y,z) ((x) = (y) - (z))
#define MKL_DC_CONJ(x,y) ((x) = (y))
#define MKL_DC_MUL(z,x,y) ((z) = (x) * (y))
#define MKL_DC_MUL_ADD(r,x,y,z) ((r) = (x) * (y) + (z))
#define MKL_DC_DIV(z,x,y) ((z) = (x) / (y))
#define MKL_DC_SUB_MUL(r,x,y,z) ((r) = (x) - (y) * (z))
#define MKL_DC_SWAP(x, y) do { \
    mkl_dc_type t; \
    t = x; \
    x = y; \
    y = t; \
} while (0)
#define MKL_DC_NEG(x, y) ((x) = -(y))
#define MKL_DC_INV(x, y) ((x) = 1.0 / (y))
#define MKL_DC_CONVERT_INT(x, y) ((x) = (mkl_dc_type)(y))
#endif

#define MKL_DC_MisN(t) ((t) == 'N' || (t) == 'n')
#define MKL_DC_MisT(c) ((c) == 'T' || (c) == 't')
#define MKL_DC_MisU(c) ((c) == 'U' || (c) == 'u')
#define MKL_DC_MisL(c) ((c) == 'L' || (c) == 'l')
#define MKL_DC_MN(M, LDM, r, c) M[r + c * LDM]
#define MKL_DC_MT(M, LDM, r, c) M[c + r * LDM]
#define MKL_DC_MOV(x, y) x = y
#define MKL_DC_NOOP(x, y)
#define MKL_DC_ZERO_C(c, beta) MKL_DC_SET_ZERO(c)
#define MKL_DC_MUL_C(c, beta) MKL_DC_MUL(c, beta, c)
#define MKL_DC_DIV_B(b, a) MKL_DC_DIV((b), (b), (a))
#define MKL_DC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define MKL_DC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define MKL_DC_ABS(x) ((x) > 0 ? (x) : -(x))
#define MKL_DC_XCONCAT3(x, y, z) x ## y ## z
#define MKL_DC_CONCAT3(x, y, z) MKL_DC_XCONCAT3(x, y, z)
