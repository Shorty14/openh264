/*!
 * \copy
 *     Copyright (c)  2010-2013, Cisco Systems
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in
 *          the documentation and/or other materials provided with the
 *          distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *     COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * \file	slice_multi_threading.c
 *
 * \brief	slice based multiple threading
 *
 * \date	04/16/2010 Created
 *
 *************************************************************************************
 */
#ifndef SVC_SLICE_MULTIPLE_THREADING_H__
#define SVC_SLICE_MULTIPLE_THREADING_H__

#if defined(MT_ENABLED)

#include "typedefs.h"
#include "codec_app_def.h"
#include "param_svc.h"
#include "encoder_context.h"
#include "svc_enc_frame.h"
#include "svc_enc_macroblock.h"
#include "svc_enc_slice_segment.h"
#include "WelsThreadLib.h"

namespace WelsSVCEnc {
void UpdateMbListNeighborParallel (SSliceCtx* pSliceCtx,
                                   SMB* pMbList,
                                   const int32_t kiSliceIdc);

void CalcSliceComplexRatio (void* pRatio, SSliceCtx* pSliceCtx, uint32_t* pSliceConsume);

#if defined(MT_ENABLED) && defined(DYNAMIC_SLICE_ASSIGN) && defined(NOT_ABSOLUTE_BALANCING)
int32_t NeedDynamicAdjust (void* pConsumeTime, const int32_t kiSliceNum);
#endif//..

#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE)
void DynamicAdjustSlicing (sWelsEncCtx* pCtx,
                           SDqLayer* pCurDqLayer,
                           void* pComplexRatio,
                           int32_t iCurDid);
#endif//#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE)

#ifdef PACKING_ONE_SLICE_PER_LAYER
void reset_env_mt (sWelsEncCtx* pCtx);
#endif//PACKING_ONE_SLICE_PER_LAYER


int32_t RequestMtResource (sWelsEncCtx** ppCtx, SWelsSvcCodingParam* pParam, const int32_t kiCountBsLen,
                           const int32_t kiTargetSpatialBsSize);

void ReleaseMtResource (sWelsEncCtx** ppCtx);

int32_t AppendSliceToFrameBs (sWelsEncCtx* pCtx, SLayerBSInfo* pLbi, const int32_t kiSliceCount);

//int32_t WriteSliceToFrameBs (sWelsEncCtx* pCtx, SLayerBSInfo* pLbi, uint8_t*& pOrigBsBuffer, const int32_t kiBufferOffset, const int32_t kiSliceIdx);
int32_t WriteSliceToFrameBs (sWelsEncCtx* pCtx, SLayerBSInfo* pLbi, uint8_t* pFrameBsBuffer, const int32_t iSliceIdx, int32_t& iSliceSize);

#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE)
#if defined(__GNUC__)
WELS_THREAD_ROUTINE_TYPE UpdateMbListThreadProc (void* arg);
#endif//__GNUC__
#endif//#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE)

WELS_THREAD_ROUTINE_TYPE CodingSliceThreadProc (void* arg);

int32_t CreateSliceThreads (sWelsEncCtx* pCtx);

#ifdef PACKING_ONE_SLICE_PER_LAYER
void ResetCountBsSizeInPartitions (uint32_t* pCountBsSizeList, const int32_t kiPartitionCnt);
#endif//PACKING_ONE_SLICE_PER_LAYER

#ifdef _WIN32
int32_t FiredSliceThreads (SSliceThreadPrivateData* pPriData, WELS_EVENT* pEventsList, SLayerBSInfo* pLayerBsInfo,
                           const uint32_t kuiNumThreads/*, int32_t *iLayerNum*/, SSliceCtx* pSliceCtx, const bool kbIsDynamicSlicingMode);
#else
int32_t FiredSliceThreads (SSliceThreadPrivateData* pPriData, WELS_EVENT** ppEventsList, SLayerBSInfo* pLayerBsInfo,
                           const uint32_t kuiNumThreads/*, int32_t *iLayerNum*/, SSliceCtx* pSliceCtx, const bool kbIsDynamicSlicingMode);
#endif//_WIN32

int32_t DynamicDetectCpuCores();

#if defined(MT_ENABLED) && defined(DYNAMIC_SLICE_ASSIGN)

int32_t AdjustBaseLayer (sWelsEncCtx* pCtx);
int32_t AdjustEnhanceLayer (sWelsEncCtx* pCtx, int32_t iCurDid);

#endif//MT_ENABLED && DYNAMIC_SLICE_ASSIGN

#if defined(MT_ENABLED)

#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE) && defined(MT_DEBUG)
void TrackSliceComplexities (sWelsEncCtx* pCtx, const int32_t kiCurDid);
#endif//#if defined(DYNAMIC_SLICE_ASSIGN) && defined(TRY_SLICING_BALANCE)
#if defined(DYNAMIC_SLICE_ASSIGN) && defined(MT_DEBUG)
void TrackSliceConsumeTime (sWelsEncCtx* pCtx, int32_t* pDidList, const int32_t kiSpatialNum);
#endif//#if defined(DYNAMIC_SLICE_ASSIGN) && defined(MT_DEBUG)

#endif//MT_ENABLED
}
#endif//MT_ENABLED

#endif//SVC_SLICE_MULTIPLE_THREADING_H__

