#===-- tools/klee/Makefile ---------------------------------*- Makefile -*--===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#

LEVEL=../..
TOOLNAME = klee

include $(LEVEL)/Makefile.config

USEDLIBS = kleeCore.a kleeModule.a kleaverSolver.a kleaverExpr.a kleeSupport.a kleeBasic.a

ifeq ($(ENABLE_OPENCL),1)
  LLVMLIBS = clangFrontend.a clangSerialization.a clangDriver.a clangCodeGen.a clangParse.a clangSema.a clangIndex.a clangAnalysis.a clangRewrite.a clangAST.a clangLex.a clangBasic.a
endif

LINK_COMPONENTS = jit bitreader bitwriter ipo linker engine asmparser
ifeq ($(LLVM_VERSION_MAJOR),3)
  LINK_COMPONENTS += instrumentation
endif

include $(LEVEL)/Makefile.common

LIBS += -lstp
