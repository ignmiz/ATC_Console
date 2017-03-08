/*******************************************************************************
* Copyright 1999-2017 Intel Corporation All Rights Reserved.
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
!      Intel(R) Math Kernel Library (MKL) interface for BLACS routines
!******************************************************************************/

#ifndef _MKL_BLACS_H_
#define _MKL_BLACS_H_

#include "mkl_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* <name>_ declarations */

void	IGAMX2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	SGAMX2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	DGAMX2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	CGAMX2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	ZGAMX2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);

void	IGAMN2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	SGAMN2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	DGAMN2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	CGAMN2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);
void	ZGAMN2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, MKL_INT *rA, MKL_INT *cA, const MKL_INT *ldia, const MKL_INT *rdest, const MKL_INT *cdest);

void	IGSUM2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	SGSUM2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	DGSUM2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	CGSUM2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	ZGSUM2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);

void	IGESD2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	SGESD2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	DGESD2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	CGESD2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	ZGESD2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);

void	ITRSD2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const MKL_INT *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	STRSD2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	DTRSD2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	CTRSD2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);
void	ZTRSD2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda, const MKL_INT *rdest, const MKL_INT *cdest);

void	IGERV2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	SGERV2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	DGERV2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	CGERV2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	ZGERV2D(const MKL_INT *ConTxt, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);

void	ITRRV2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	STRRV2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	DTRRV2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	CTRRV2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	ZTRRV2D(const MKL_INT *ConTxt, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);

void	IGEBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, const MKL_INT *A, const MKL_INT *lda);
void	SGEBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda);
void	DGEBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda);
void	CGEBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda);
void	ZGEBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda);

void	ITRBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const MKL_INT *A, const MKL_INT *lda);
void	STRBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda);
void	DTRBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda);
void	CTRBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const float *A, const MKL_INT *lda);
void	ZTRBS2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, const double *A, const MKL_INT *lda);

void	IGEBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	SGEBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	DGEBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	CGEBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	ZGEBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);

void	ITRBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, MKL_INT *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	STRBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	DTRBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	CTRBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, float *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);
void	ZTRBR2D(const MKL_INT *ConTxt, const char *scope, const char *top, const char *uplo, const char *diag, const MKL_INT *m, const MKL_INT *n, double *A, const MKL_INT *lda, const MKL_INT *rsrc, const MKL_INT *csrc);

void	BLACS_PINFO(MKL_INT *mypnum, MKL_INT *nprocs);
void	BLACS_SETUP(MKL_INT *mypnum, MKL_INT *nprocs);
void	BLACS_GET(const MKL_INT *ConTxt, const MKL_INT *what, MKL_INT *val);
void	BLACS_SET(const MKL_INT *ConTxt, const MKL_INT *what, const MKL_INT *val);
void	BLACS_GRIDINIT(MKL_INT *ConTxt, const char *layout, const MKL_INT *nprow, const MKL_INT *npcol);
void	BLACS_GRIDMAP(MKL_INT *ConTxt, const MKL_INT *usermap, const MKL_INT *ldup, const MKL_INT *nprow0, const MKL_INT *npcol0);

void	BLACS_FREEBUFF(const MKL_INT *ConTxt, const MKL_INT *Wait);
void	BLACS_GRIDEXIT(const MKL_INT *ConTxt);
void	BLACS_ABORT(const MKL_INT *ConTxt, const MKL_INT *ErrNo);
void	BLACS_EXIT(const MKL_INT *notDone);

void	BLACS_GRIDINFO(const MKL_INT *ConTxt, MKL_INT *nprow, MKL_INT *npcol, MKL_INT *myrow, MKL_INT *mycol);
MKL_INT	BLACS_PNUM(const MKL_INT *ConTxt, const MKL_INT *prow, const MKL_INT *pcol);
void	BLACS_PCOORD(const MKL_INT *ConTxt, const MKL_INT *nodenum, MKL_INT *prow, MKL_INT *pcol);

void	BLACS_BARRIER(const MKL_INT *ConTxt, const char *scope);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MKL_BLACS_H_ */
