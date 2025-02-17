/*
 * Copyright (c) 2008 James Molloy, Jörg Pfähler, Matthew Iselin
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef KERNEL_PROCESSOR_ARMV7_STACKFRAME_H
#define KERNEL_PROCESSOR_ARMV7_STACKFRAME_H

#include <compiler.h>
#include <processor/types.h>
#include <processor/StackFrameBase.h>

/** @addtogroup kernelprocessorARMV7
 * @{ */

/** mips32 StackFrame */
class ARMV7StackFrame : public StackFrameBase
{
  public:
    /** Creates a stack frame based on the given processor state and also the given
     *  symbol name (mangled). */
    inline ARMV7StackFrame(const ProcessorState &State, uintptr_t basePointer, LargeStaticString mangledSymbol)
      : StackFrameBase(State, basePointer, mangledSymbol){}
    /** The destructor does nothing */
    inline ~ARMV7StackFrame(){}
    
    /** Construct a stack frame, given a ProcessorState. The stack frame should be constructed
     *  to comply with the default ABI for the current architecture - that implies the stack
     *  may be changed, so the getStackPointer() member of ProcessorState must be valid.
     *  \param[out] state The state to modify (construct a stack frame in).
     *  \param returnAddress The return address of the stack frame.
     *  \param nParams The number of parameters to the stack frame.
     *  \param ... The parameters, each sizeof(uintptr_t). */
    static void construct(ProcessorState &state,
                          uintptr_t returnAddress,
                          unsigned int nParams,
                          ...);
  private:
    /** Returns the n'th 64-bit parameter in the stack frame. */
    virtual uintptr_t getParameter(size_t n);
};

/** @} */

#endif
