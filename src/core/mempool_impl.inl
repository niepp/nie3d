public:
	T* get()
	{
		if (_link == 0)
		{
			refill();
		}

		assert(_link != 0);

		T* result = (T*)_link;
		_link = _link->next;

		::new (result) T();
		return result;
	}

	void free(T *p)
	{
		p->~T();
		Block *pb = (Block *)p;
		pb->next = _link;
		_link = pb;
	}
protected:
	void clear_all()
	{
		while (_head)
		{
			Chunk *temp = _head->next;
			::free(_head);
			_head = temp;
		}
	}

	void refill()
	{
		assert(_link == 0);
		Chunk *chunk = (Chunk*)malloc(get_chunk_size());
		chunk->next = _head;
		_head = chunk;

		char *p = chunk->data;
		int count = Size / BlockSize;
		for (int i = 0; i < count; ++i)
		{
			((Block*)p)->next = _link;
			_link = (Block *)p;
			p += BlockSize;
		}
	}
