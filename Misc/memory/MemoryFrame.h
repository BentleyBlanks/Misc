#pragma once
#ifndef __MEMORYFRAEM_H__
#define __MEMORYFRAME_H__


//#include "RBBasedata.h"
#include "..\\RBMath\\Inc\\Platform\\RBBasedata.h"

typedef bool MPOS;
//内存对齐：byte对齐
#define BYTEALIGN (size_t)8
//强制对齐
#define ALIGNUP(naddress,nbytes) ((((size_t)naddress)+\
	(nbytes)-1 ) & (~((nbytes)-1)))

struct MemoryFrame
{
	MPOS frame_pos;
	u8* memory_ptr;
};

class RBFrameAlloctor
{
public:
	INI_RESULT init(size_t tsize);

	void* alloc(size_t tsize,MPOS tpos);
	INI_RESULT shutdown();
	void getframe(MemoryFrame& mf,MPOS tpos);
	void release(MemoryFrame& mf);
	//++帧释放函数

	size_t get_all_memory();
  size_t get_allocated_memory();
	size_t get_remain_memory();

  void serialize(const char* filename,MPOS tpos);
  void* deserialize(const char* filename,MPOS tpos);
  size_t get_alloc_size(size_t osize)
  {
    return ALIGNUP(osize, _byte_alignment);
  }

private:
	void* alloc(size_t tsize){return 0;}


	int _byte_alignment;
	u8* _pmemory;
	u8* _pbase;
	u8* _pcap;
	u8* _pframe_base;
	u8* _pframe_cap;
	size_t _all_memory;
};
struct ThreadMem
{
  MemoryFrame mf;
  RBFrameAlloctor frame;
};
#endif