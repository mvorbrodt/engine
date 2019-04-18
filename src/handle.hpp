#pragma once

namespace engine
{
	template<typename T, template<typename> typename ReleasePolicy>
	class handle
	{
	public:
		handle(T h = T{}) : m_handle(h) {}

		handle(const handle&) = delete;

		handle(handle&& h)
		{
			m_handle = h.m_handle;
			h.m_handle = T{};
		}

		handle& operator = (const handle&) = delete;

		handle& operator = (handle&& h)
		{
			m_handle = h.m_handle;
			h.m_handle = T{};
			return *this;
		}

		~handle() { ReleasePolicy<T>::Execute(m_handle); }

		T* operator&() { return &m_handle; }
		const T* operator&() const { return &m_handle; }

		operator T () const { return m_handle; }

	private:
		T m_handle;
	};
}
