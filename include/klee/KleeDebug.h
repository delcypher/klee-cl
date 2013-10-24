//===-- Constraints.h -------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//

#ifndef KLEE_DEBUG_H
#define KLEE_DEBUG_H
#include "llvm/Support/Debug.h"

/* This Handy macro uses LLVM's debug infrastructure.
 * If we do a debug build then commands (X) in
 *
 * KLEE_DEBUG(X)
 *
 * will be executed if the -debug-only=klee
 * flag is passed. Otherwise they will not be executed.
 *
 * If we do a Release build then KLEE_DEBUG() is a no-op.
 *
 * Example:
 * KLEE_DEBUG( dbgs() << "This is a test.\n" );
 *
 */
#ifndef NDEBUG
#define KLEE_DEBUG(X) DEBUG_WITH_TYPE("klee", X)
#else
#define KLEE_DEBUG(X) do { } while(0)
#endif

#endif
