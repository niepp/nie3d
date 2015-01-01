#pragma once

#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

// 以固定的Chunk大小来分配内存
template <class T, int Size = 4096>
class CORE_API mem_pool
{
private:
	struct Chunk
	{
		Chunk *next;
		char data[Size];
	};

	struct Block
	{
		Block *next;
	};

	enum
	{
		BlockSize = (sizeof(Block) > sizeof(T)) ? sizeof(Block) : sizeof(T),
	};
public:
	mem_pool(): _head(0), _link(0)
	{}

	~mem_pool()
	{
		clear_all();
	}

#include "mempool_impl.inl"

protected:
	size_t get_chunk_size() { return sizeof(Chunk); }

private:
	Chunk *_head;
	Block *_link;
};

// 特化版本，以任意的Chunk大小来分配内存
template <class T>
class CORE_API mem_pool<T, 0>
{
private:
    const static int Size = 0;
    
	struct Chunk
	{
		Chunk *next;
		char data[1];
	};

	struct Block
	{
		Block *next;
	};

	enum
	{
		BlockSize = (sizeof(Block) > sizeof(T)) ? sizeof(Block) : sizeof(T),
	};
public:
	explicit mem_pool(size_t chunk_size = 0): _head(0), _link(0), _chunk_size((chunk_size == 0) ? 4096 : chunk_size)
	{}

	~mem_pool()
	{
		clear_all();
	}

#include "mempool_impl.inl"

public:
	size_t get_chunk_size() { return _chunk_size; }
	void set_chunk_size(size_t chunk_size)
	{
		_chunk_size = (chunk_size == 0) ? 4096 : chunk_size;
	}

private:
	Chunk *_head;
	Block *_link;
	size_t _chunk_size;
};

#endif // __MEMPOOL_H__
