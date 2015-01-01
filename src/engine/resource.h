#pragma once

enum ResourceType 
{
	cResTypeTexture = 0,
	cResTypeMaterial,
	cResTypeMesh,

	cResTypeCount
};

class ENGINE_API Resource : public RefObject
{
public:
	Resource(const Filepath &file_path, ResourceType res_type)
		: m_file_path(file_path), m_res_type(res_type), m_mem_size(0), m_vdmem_size(0), m_last_time_used(0)
	{
	}

	virtual ~Resource()
	{
		assert(m_ref_count == 0);
	}

	virtual void Release()
	{
		DecRefCount();
		if (GetRefCount() == 0)
		{
			delete this;
		}
	}

	// do loading resource
	virtual bool Load() = 0;

	const Filepath& GetFilePath() const
	{
		return m_file_path;
	}

	ResourceType GetResType() const
	{
		return m_res_type;
	}

	Uint32 GetMemSize() const
	{
		return m_mem_size;
	}

	Uint32 GetVideoMemSize() const
	{
		return m_vdmem_size;
	}

protected:
	Filepath m_file_path;					// 资源路径
	ResourceType m_res_type;
	Uint32 m_mem_size;						// 占用内存空间
	Uint32 m_vdmem_size;					// 占用显存空间
	Uint32 m_last_time_used;				// 最近使用时间
};

ENGINE_API IRenderer* GetRenderer();
