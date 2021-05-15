#include "pch.h"
#include "Scope.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(size_t capacity)
	{
		_orderList.Reserve(capacity);
	}

	Scope::Scope(const Scope& other)
	{
		_orderList.Reserve(other._orderList.Capacity());
		
		for (size_t i = 0; i < other.Size(); ++i)
		{
			auto [key, existingDatum] = *other._orderList[i];
			Datum& newDatum = Append(key);

			if (existingDatum.Type() != Datum::DatumType::Table)
			{
				newDatum = existingDatum;
			}
			else
			{
				newDatum.SetType(Datum::DatumType::Table);
				newDatum.Reserve(existingDatum.Size());

				for (size_t j = 0; j < existingDatum.Size(); ++j)
				{
					Scope* s = (existingDatum[j]).Clone();
					s->_parent = this;
					newDatum.PushBack(*s);
				}
			}
		}
	}

	Scope::Scope(Scope&& other) noexcept
	{
		//	Update the Parent's pointer to it's new child.
		if (other._parent != nullptr)
		{
			size_t index = 0;
			Datum* d = other._parent->FindContainedScope(&other, index);
			d[index] = *this;
		}

		_orderList = std::move(other._orderList);
		_table = std::move(other._table);
		_parent = other._parent;

		for (size_t i = 0; i < Size(); ++i)
		{
			Datum& d = _orderList[i]->second;
			if (d.Type() == Datum::DatumType::Table)
			{
				for (size_t j = 0; j < d.Size(); ++j)
				{
					(d[j])._parent = this;
				}
			}
		}

		other._table.Clear();
		other._orderList.Clear();
		other._orderList.ShrinkToFit();
		other._parent = nullptr;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			_orderList.Clear();
			_orderList.ShrinkToFit();

			//	Update the Parent's pointer to it's new child.
			if (other._parent != nullptr)
			{
				size_t index = 0;
				Datum* d = other._parent->FindContainedScope(&other, index);
				d[index] = *this;
			}

			_orderList = std::move(other._orderList);
			_table = std::move(other._table);
			_parent = other._parent;

			for (size_t i = 0; i < Size(); ++i)
			{
				Datum& d = _orderList[i]->second;
				if (d.Type() == Datum::DatumType::Table)
				{
					for (size_t j = 0; j < d.Size(); ++j)
					{
						(d[j])._parent = this;
					}
				}
			}

			other._table.Clear();
			other._orderList.Clear();
			other._orderList.ShrinkToFit();
			other._parent = nullptr;
		}
		return *this;
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Clear();

			_orderList.Clear();
			_orderList.ShrinkToFit();
			_orderList.Reserve(other._orderList.Capacity());

			for (size_t i = 0; i < other.Size(); ++i)
			{
				auto [key, existingDatum] = *other._orderList[i];
				Datum& newDatum = Append(key);

				if (existingDatum.Type() != Datum::DatumType::Table)
				{
					newDatum = existingDatum;
				}
				else
				{
					newDatum.SetType(Datum::DatumType::Table);
					newDatum.Reserve(existingDatum.Size());

					for (size_t j = 0; j < existingDatum.Size(); ++j)
					{
						Scope* s = (existingDatum[j]).Clone();
						s->_parent = this;
						newDatum.PushBack(*s);
					}
				}
			}
		}

		return *this;
	}

	Scope::~Scope()
	{
		//	Clear data members - but need to walk through and destruct any child scopes first	
		Clear();

		_parent = nullptr;
	}

	void Scope::Adopt(Scope& child, const string& name)
	{
		//	Will either find the datum with the key or make a default constructed one if key not in the scope
		Datum& dp = Append(name);

		if (dp.Type() == Datum::DatumType::Unknown)
		{
			dp.SetType(Datum::DatumType::Table);
		}

		if (dp.Type() != Datum::DatumType::Table)
		{
			throw runtime_error("Attempting to place a Scope into a non-Table datum. Scope::Adopt()");
		}

		child.Orphan();
		child._parent = this;

		//	Pushback the child into the datum
		dp.PushBack(child);
	}

	Datum& Scope::Append(const string& keyString)
	{
		if (keyString.empty())
		{
			throw invalid_argument("Cant append an empty keystring to Scope");
		}
		
		auto [it, wasInserted] = _table.Insert(make_pair(keyString, Datum()));

		if (wasInserted)
		{
			_orderList.PushBack(&(*it));
		}
		
		return it->second;
	}

	Scope& Scope::AppendScope(const string& keyString)
	{
		Datum& datum = Append(keyString);

		if (datum.Type() == Datum::DatumType::Unknown)
		{
			datum.SetType(Datum::DatumType::Table);
		}

		if (datum.Type() != Datum::DatumType::Table)
		{
			throw runtime_error("Attempting to append a scope with a key paired with a pre-existing non-table datum.");
		}

		Scope* s = new Scope();
		s->_parent = this;
		datum.PushBack(*s);

		return *s;
	}

	void Scope::Clear()
	{
		Orphan();

		for (OrderListType::Iterator it = _orderList.begin(); it != _orderList.end(); ++it)
		{
			Datum& d = (*it)->second;
			if (d.Type() == Datum::DatumType::Table)
			{
				for (size_t i = 0; i < d.Size(); ++i)
				{
					Scope& s = (d[i]);
					s._parent = nullptr; // Short Circuit the orphan call
					delete &s;
					//return;
				}
			}
		}
		_table.Clear();
		_orderList.Clear();
	}

	Datum* Scope::Find(const string& keyString)
	{
		auto it = _table.Find(keyString);
		return it != _table.end() ? &it->second : nullptr;
	}

	const Datum* Scope::Find(const string& keyString) const
	{
		auto it = _table.Find(keyString);
		return it != _table.end() ? &it->second : nullptr;
	}

	Datum* Scope::FindContainedScope(const Scope* sp, size_t& index)
	{
		for (size_t i = 0; i < Size(); ++i)
		{
			Datum* d = &(_orderList[i]->second);
			if (d->Type() == Datum::DatumType::Table)
			{
				for (size_t j = 0; j < d->Size(); ++j)
				{
					if (&((*d)[j]) == sp)
					{
						index = j;
						return d;
					}
				}
			}
		}

		return nullptr;
	}

	const Datum* Scope::FindContainedScope(const Scope* sp, size_t& index) const
	{
		for (size_t i = 0; i < Size(); ++i)
		{
			Datum* d = &(_orderList[i]->second);
			if (d->Type() == Datum::DatumType::Table)
			{
				for (size_t j = 0; j < d->Size(); ++j)
				{
					if (((*d)[j]) == *sp)
					{
						index = j;
						return d;
					}
				}
			}
		}

		return nullptr;
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::operator[](const string& keyString)
	{
		return Append(keyString);
	}

	Datum& Scope::operator[](size_t index)
	{
		if (index >= Size())
		{
			throw runtime_error("Attempting to dereference at an index greater than size. Scope::operator[].");
		}

		return _orderList[index]->second;
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (Size() != other.Size())
		{
			return false;
		}

		//	Loop through the inserted pairs and compare their values
		for (size_t i = 0; i < Size(); ++i)
		{
			//	Compare keys in insertion order, then compare Datums. Datums of type table will recursively call this operator==()
			if (_orderList[i]->first != other._orderList[i]->first || _orderList[i]->second != other._orderList[i]->second)
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !(operator==(other));
	}

	void Scope::Orphan()
	{
		if (_parent != nullptr)
		{
			size_t index;
			Datum* d = _parent->FindContainedScope(this, index);
			assert(d != nullptr);
			d->RemoveAt(index);
			_parent = nullptr;
		}
	}

	Datum* Scope::Search(const string& keyString, Scope*& scope)
	{
		Datum* retVal = Find(keyString);

		if (retVal != nullptr)
		{
			scope = this;
			return retVal;
		}

		if (_parent != nullptr)
		{
			return _parent->Search(keyString, scope);
		}

		scope = nullptr;
		return nullptr;
	}

	const Datum* Scope::Search(const string& keyString, const Scope*& scope) const
	{
		const Datum* retVal = Find(keyString);

		if (retVal != nullptr)
		{
			scope = this;
			return retVal;
		}

		if (_parent != nullptr)
		{
			return _parent->Search(keyString, scope);
		}

		scope = nullptr;
		return nullptr;
	}

	Datum* Scope::Search(const string& keyString)
	{
		Scope* s;
		return Search(keyString, s);
	}

	const Datum* Scope::Search(const string& keyString) const
	{
		const Scope* s;
		return Search(keyString, s);
	}

	size_t Scope::Size() const
	{
		return _orderList.Size();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		const Scope* other = rhs->As<Scope>();
		return other != nullptr ? *this == *other : false;
	}

}