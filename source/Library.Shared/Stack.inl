namespace FieaGameEngine
{
	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		_list.PushFront(value);
	}

	template <typename T>
	inline void Stack<T>::Push(T&& value)
	{
		_list.PushFront(std::move(value));
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		_list.PopFront();
	}

	template <typename T>
	inline T& Stack<T>::Peek()
	{
		return _list.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Peek() const
	{
		return _list.Front();
	}

	template <typename T>
	inline std::size_t Stack<T>::Size() const
	{
		return _list.Size();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return _list.IsEmpty();
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		return _list.Clear();
	}
}