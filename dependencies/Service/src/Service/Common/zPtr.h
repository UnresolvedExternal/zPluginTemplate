namespace Service
{
	namespace Internals
	{
		template <typename T>
		concept AddRefableByMethod =
			requires(T object)
			{
				object.AddRef();
			};

		template <typename T>
		concept AddRefableByMember = !AddRefableByMethod<T> &&
			requires(T object)
			{
				object.refCtr++;
			};

		template <typename T>
		concept AddRefableByMemberMenuItem = !AddRefableByMethod<T> && !AddRefableByMember<T> &&
			requires(T object)
			{
				object.m_iRefCtr++;
			};

		template <typename T>
		concept AddRefable = AddRefableByMethod<T> || AddRefableByMember<T> || AddRefableByMemberMenuItem<T>;

		template <typename T>
		concept Releaseable = 
			requires(T object)
			{
				object.Release();
			};

		template <typename T>
		concept RefCountable = AddRefable<T> && Releaseable<T>;
	}

	template <Internals::RefCountable T>
	struct zPtr
	{
		zPtr();
		explicit zPtr(const T* pointer, bool addRef = false);
		zPtr(const zPtr& other);

		template <typename E>
			requires std::assignable_from<T*, E*>
		zPtr(const zPtr<E>& other);

		zPtr(zPtr&& pointer);
		~zPtr();
		zPtr& operator=(const zPtr& other);

		template <typename E>
			requires std::assignable_from<T*, E*>
		zPtr& operator=(const zPtr<E>& other);

		template <typename E>
			requires std::assignable_from<T*, E*>
		zPtr& operator=(zPtr<E>&& other);

		zPtr& operator=(zPtr&& other);
		operator T*() const;

		template <typename E>
		operator zPtr<E>() const;

		std::weak_ordering operator<=>(const zPtr& other) const;

		template <typename E>
			requires std::assignable_from<T*, E*> || std::assignable_from<E*, T*>
		std::weak_ordering operator<=>(const zPtr<E>& other) const;

		T* operator->();
		const T* operator->() const;
		T& operator*();
		const T& operator*() const;

		void AddRef() const;
		void Release() const;

		T* Detach();

	private:
		template <Internals::RefCountable E>
		friend class zPtr;

		T* pointer;
	};
}

namespace Service
{
	template <Internals::RefCountable T>
	zPtr<T>::zPtr() :
		pointer{}
	{

	}

	template <Internals::RefCountable T>
	zPtr<T>::zPtr(const T* pointer, bool addRef) :
		pointer{ const_cast<T*>(pointer) }
	{
		if (addRef)
			AddRef();
	}

	template <Internals::RefCountable T>
	zPtr<T>::zPtr(const zPtr& other) :
		pointer{ other.pointer }
	{
		AddRef();
	}

	template <Internals::RefCountable T>
	template <typename E>
		requires std::assignable_from<T*, E*>
	zPtr<T>::zPtr(const zPtr<E>& other)  :
		pointer{ other.pointer }
	{
		AddRef();
	}

	template <Internals::RefCountable T>
	zPtr<T>::zPtr(zPtr&& other) :
		pointer{ other.pointer }
	{
		other.pointer = nullptr;
	}

	template <Internals::RefCountable T>
	zPtr<T>::~zPtr()
	{
		Release();
	}

	template <Internals::RefCountable T>
	zPtr<T>& zPtr<T>::operator=(const zPtr& other)
	{
		other.AddRef();
		Release();
		pointer = other.pointer;
		return *this;
	}

	template <Internals::RefCountable T>
	template <typename E>
		requires std::assignable_from<T*, E*>
	zPtr<T>& zPtr<T>::operator=(const zPtr<E>& other)
	{
		other.AddRef();
		Release();
		pointer = other.pointer;
		return *this;
	}

	template <Internals::RefCountable T>
	template <typename E>
		requires std::assignable_from<T*, E*>
	zPtr<T>& zPtr<T>::operator=(zPtr<E>&& other)
	{
		Release();
		pointer = other.pointer;
		other.pointer = nullptr;
		return *this;
	}

	template <Internals::RefCountable T>
	zPtr<T>& zPtr<T>::operator=(zPtr&& other)
	{
		if (this == &other)
			return;

		Release();
		pointer = other.pointer;
		other.pointer = nullptr;
		return *this;
	}

	template <Internals::RefCountable T>
	zPtr<T>::operator T*() const
	{
		return pointer;
	}

	template <Internals::RefCountable T>
	template <typename E>
	zPtr<T>::operator zPtr<E>() const
	{
		static_assert(std::is_base_of_v<E, T>);
		return zPtr<E>{ pointer, true };
	}

	template <Internals::RefCountable T>
	std::weak_ordering zPtr<T>::operator<=>(const zPtr& other) const
	{
		return pointer <=> other.pointer;
	}

	template <Internals::RefCountable T>
	template <typename E>
		requires std::assignable_from<T*, E*> || std::assignable_from<E*, T*>
	std::weak_ordering zPtr<T>::operator<=>(const zPtr<E>& other) const
	{
		return pointer <=> other.pointer;
	}

	template <Internals::RefCountable T>
	T* zPtr<T>::operator->()
	{
		return pointer;
	}

	template <Internals::RefCountable T>
	const T* zPtr<T>::operator->() const
	{
		return pointer;
	}

	template <Internals::RefCountable T>
	T& zPtr<T>::operator*()
	{
		return *pointer;
	}

	template <Internals::RefCountable T>
	const T& zPtr<T>::operator*() const
	{
		return *pointer;
	}

	template <Internals::RefCountable T>
	void zPtr<T>::AddRef() const
	{
		if (!pointer)
			return;

		if constexpr (Internals::AddRefableByMethod<T>)
			pointer->AddRef();
		else if constexpr (Internals::AddRefableByMember<T>)
			pointer->refCtr += 1;
		else
			pointer->m_iRefCtr += 1;
	}

	template <Internals::RefCountable T>
	void zPtr<T>::Release() const
	{
		if (pointer)
			pointer->Release();
	}

	template <Internals::RefCountable T>
	T* zPtr<T>::Detach()
	{
		T* backup = pointer;
		pointer = nullptr;
		return backup;
	}
}
