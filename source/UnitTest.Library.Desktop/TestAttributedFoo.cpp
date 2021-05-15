#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include "AttributedFoo.h"
#include "Foo.h"
#include "TypeManager.h"
#include "RTTI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}
}

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(AttributedFooTests)
	{
	public:
		//	Runs before every Test_Method
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		//	Runs after every Test_Method
		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			TypeManager::Clear();
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestAttributedFoo)
		{
			TypeManager::AddType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			AttributedFoo af;

			//	This should invoke Clone(). Copy Constructor.
			AttributedFoo afCopy(af);

			const Vector<Signature>& a = afCopy.Attributes();
			const Vector<Signature>& b = af.Attributes();

			for (size_t i = 0; i < a.Size(); ++i)
			{
				Assert::IsTrue(a[i] == b[i]);
			}

			AttributedFoo afMove(std::move(af));

			//	Test Move Constructor
			const Vector<Signature>& c = afMove.Attributes();

			for (size_t i = 0; i < b.Size(); ++i)
			{
				Assert::IsTrue(b[i] == c[i]);
			}

			AttributedFoo afCpyAssign;
			AttributedFoo afMveAssign;

			//	Test Copy and Move Assignment Operators
			afCpyAssign = afCopy;
			afMveAssign = std::move(afMove);

			const Vector<Signature>& d = afCpyAssign.Attributes();
			const Vector<Signature>& e = afMveAssign.Attributes();

			for (size_t i = 0; i < d.Size(); ++i)
			{
				Assert::IsTrue(d[i] == e[i]);
			}

			// Test Auxiliary Attributes
			Datum& auxDatum = afCpyAssign.AppendAuxiliaryAttribute("AuxTestOne");
			auxDatum.SetType(Datum::DatumType::Integer);
			auxDatum = 10;

			Vector<Signature> testAux = afCpyAssign.AuxiliaryAttributes();

			Assert::IsTrue(testAux.Size() == 2_z);
			Assert::IsTrue(testAux[1].type == Datum::DatumType::Integer);
			Assert::IsTrue(afCpyAssign.IsAuxiliaryAttribute("AuxTestOne"s));
			Assert::IsFalse(afCpyAssign.IsAuxiliaryAttribute("Integer"s));
			Assert::IsTrue(afCpyAssign.IsPrescribedAttribute("Integer"s));
			Assert::IsFalse(afCpyAssign.IsPrescribedAttribute("AuxTestOne"s));
			Assert::IsTrue(afCpyAssign.IsAttribute("AuxTestOne"s));

			const Vector<Signature>& f = afCpyAssign.PrescribedAttributes();
			const Vector<Signature>& g = afMveAssign.PrescribedAttributes();

			for (size_t i = 0; i < f.Size(); ++i)
			{
				Assert::IsTrue(f[i] == g[i]);
			}

			Assert::ExpectException<std::runtime_error>([&afCpyAssign] {afCpyAssign.AppendAuxiliaryAttribute("Integer"s); });
			

			AttributedFoo helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::IsTrue(AttributedFoo::TypeIdClass() == rtti->TypeIdInstance());

			Bar* bar = rtti->As<Bar>();
			Assert::IsNull(bar);
			
			AttributedFoo* h = rtti->As<AttributedFoo>();
			Assert::IsNotNull(h);
			Assert::IsTrue(&helper == h);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState AttributedFooTests::_startMemState;
}
