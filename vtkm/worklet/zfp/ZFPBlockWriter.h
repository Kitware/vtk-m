//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2014 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2014 UT-Battelle, LLC.
//  Copyright 2014 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_worklet_zfp_block_writer_h
#define vtk_m_worklet_zfp_block_writer_h

#include <vtkm/worklet/zfp/ZFPTypeInfo.h>

namespace vtkm
{
namespace worklet
{
namespace zfp
{

using Word = vtkm::UInt64;

template <int block_size, typename AtomicPortalType>
struct BlockWriter
{
  union UIntInt {
    vtkm::UInt64 uintpart;
    vtkm::Int64 intpart;
  };

  vtkm::Id m_word_index;
  vtkm::Int32 m_start_bit;
  vtkm::Int32 m_current_bit;
  const int m_maxbits;
  AtomicPortalType& Portal;

  //int debug_index;

  VTKM_EXEC BlockWriter(AtomicPortalType& portal, const int& maxbits, const vtkm::Id& block_idx)
    : m_current_bit(0)
    , m_maxbits(maxbits)
    , Portal(portal)
  {
    m_word_index = (block_idx * maxbits) / (sizeof(Word) * 8);
    // debug_index = m_word_index;
    //std::cout<<"** Block "<<block_idx<<" start "<<m_word_index<<"\n";
    m_start_bit = vtkm::Int32((block_idx * maxbits) % (sizeof(Word) * 8));
  }

  template <typename T>
  void print_bits(T bits)
  {
    const int bit_size = sizeof(T) * 8;
    for (int i = bit_size - 1; i >= 0; --i)
    {
      T one = 1;
      T mask = one << i;
      int val = (bits & mask) >> i;
      printf("%d", val);
    }
    printf("\n");
  }

  void print()
  {
    //vtkm::Int64 v = Portal.Add(debug_index,0);
    //std::cout<<"current bit "<<m_current_bit<<" debug_index "<<debug_index<<" ";
    //print_bits(*reinterpret_cast<vtkm::UInt64*>(&v));
  }

  void print(int index)
  {
    vtkm::Int64 v = Portal.Add(index, 0);
    //print_bits(*reinterpret_cast<vtkm::UInt64*>(&v));
  }

  inline VTKM_EXEC void Add(const vtkm::Id index, Word& value)
  {
    UIntInt newval;
    UIntInt old;
    (void)old;
    newval.uintpart = value;
    Portal.Add(index, newval.intpart);
    //old.uintpart = 0;
    //UIntInt expected;
    //expected.uintpart = newval.uintpart;
    //while(old.uintpart != expected.uintpart)
    //{
    //  expected.uintpart = old.uintpart + newval.uintpart;
    //  old.intpart = Portal.CompareAndSwap(index, expected.intpart, old.intpart);
    //}
  }

  inline VTKM_EXEC
    //void write_bits(const unsigned int &bits, const uint &n_bits, const uint &bit_offset)
    vtkm::UInt64
    write_bits(const vtkm::UInt64& bits, const uint& n_bits)
  {
    //std::cout<<"write nbits "<<n_bits<<" "<<m_current_bit<<"\n";
    //bool print = m_word_index == 0  && m_start_bit == 0;
    const uint wbits = sizeof(Word) * 8;
    //if(bits == 0) { printf("no\n"); return;}
    //uint seg_start = (m_start_bit + bit_offset) % wbits;
    //int write_index = m_word_index + (m_start_bit + bit_offset) / wbits;
    uint seg_start = (m_start_bit + m_current_bit) % wbits;
    vtkm::Id write_index = m_word_index + vtkm::Id((m_start_bit + m_current_bit) / wbits);
    uint seg_end = seg_start + n_bits - 1;
    //int write_index = m_word_index;
    uint shift = seg_start;
    // we may be asked to write less bits than exist in 'bits'
    // so we have to make sure that anything after n is zero.
    // If this does not happen, then we may write into a zfp
    // block not at the specified index
    // uint zero_shift = sizeof(Word) * 8 - n_bits;
    Word left = (bits >> n_bits) << n_bits;

    Word b = bits - left;
    Word add = b << shift;
    Add(write_index, add);
    //debug_index = write_index;
    //if(write_index == 0)
    //{
    //  std::cout<<"*******\n";
    //  std::cout<<"Current bit "<<m_current_bit<<" writing ";
    //  print_bits(add);
    //  print();
    //  std::cout<<"*******\n";
    //}
    // n_bits straddles the word boundary
    bool straddle = seg_start < sizeof(Word) * 8 && seg_end >= sizeof(Word) * 8;
    if (straddle)
    {
      Word rem = b >> (sizeof(Word) * 8 - shift);
      Add(write_index + 1, rem);
      //std::cout<<"======\n";
      //print_bits(rem);
      //std::cout<<"======\n";
      //  printf("Straddle "); print_bits(rem);
      //debug_index = write_index +1;
    }
    m_current_bit += n_bits;
    return bits >> (Word)n_bits;
  }

  // TODO: optimize
  vtkm::UInt32 VTKM_EXEC write_bit(const unsigned int& bit)
  {
    //bool print = m_word_index == 0  && m_start_bit == 0;
    const uint wbits = sizeof(Word) * 8;
    //if(bits == 0) { printf("no\n"); return;}
    //uint seg_start = (m_start_bit + bit_offset) % wbits;
    //int write_index = m_word_index + (m_start_bit + bit_offset) / wbits;
    uint seg_start = (m_start_bit + m_current_bit) % wbits;
    vtkm::Id write_index = m_word_index + vtkm::Id((m_start_bit + m_current_bit) / wbits);
    //uint seg_end = seg_start;
    //int write_index = m_word_index;
    uint shift = seg_start;
    // we may be asked to write less bits than exist in 'bits'
    // so we have to make sure that anything after n is zero.
    // If this does not happen, then we may write into a zfp
    // block not at the specified index
    // uint zero_shift = sizeof(Word) * 8 - n_bits;

    Word add = (Word)bit << shift;
    Add(write_index, add);
    m_current_bit += 1;

    return bit;
  }
};

} // namespace zfp
} // namespace worklet
} // namespace vtkm
#endif //  vtk_m_worklet_zfp_type_info_h
