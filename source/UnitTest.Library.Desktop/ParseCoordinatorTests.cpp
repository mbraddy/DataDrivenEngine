#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include <exception>
#include <stdexcept>
#include <functional>
#include "Foo.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "IFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{

	class JsonNotValidData : public SharedData
	{
		friend JsonTableParseHelper;

		RTTI_DECLARATIONS(JsonNotValidData, SharedData)

	public:
		virtual ~JsonNotValidData() = default;

		gsl::owner<JsonNotValidData*> Create() override
		{
			return new JsonNotValidData();
		}
	};

	RTTI_DEFINITIONS(JsonNotValidData)

	class Power : public Scope
	{
	};

	ConcreteFactory(Power, Scope)

	class TestHelper : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(TestHelper, IJsonParseHelper)

		bool StartHandler(SharedData& data, const std::string& key, const Json::Value& object, bool inArray, size_t index) override 
		{
			UNREFERENCED_LOCAL(data);
			UNREFERENCED_LOCAL(key);
			UNREFERENCED_LOCAL(object);
			UNREFERENCED_LOCAL(inArray);
			UNREFERENCED_LOCAL(index);
			return false;
		}
		
		bool EndHandler(SharedData& data, const std::string& key) override
		{
			UNREFERENCED_LOCAL(data);
			UNREFERENCED_LOCAL(key);
			return false;
		}
		gsl::owner<TestHelper*> Create() override
		{
			return new TestHelper();
		}
	};

	RTTI_DEFINITIONS(TestHelper)

	TEST_CLASS(ParseCoordinatorTests)
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
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestConstructors)
		{
			JsonNotValidData invalidData;
			JsonParseCoordinator parseMaster(invalidData);
			JsonTableParseHelper tableHelper;
		}

		TEST_METHOD(TestAddingAndRemovingHelpers)
		{
			Scope s;
			SharedTableData tableData(s);
			JsonParseCoordinator parseMaster(tableData);

			JsonTableParseHelper tableHelper;

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());

			parseMaster.AddHelper(tableHelper);

			Assert::IsTrue(1_z == parseMaster.GetHelperCount());

			parseMaster.RemoveHelper(tableHelper);

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());
		}

		TEST_METHOD(TestParsing)
		{
			Scope s;
			SharedTableData tableData(s);
			JsonParseCoordinator parseMaster(tableData);

			string inputString = R"({ "soloStringInt": {
										"type": "integer",
										"value": 10 } })";

			string inputArrayString = R"({ "integerArray": {
										"type": "integer",
										"value": [ 10, 20, 30, 40 ] } })";

			JsonTableParseHelper tableHelper;
			parseMaster.AddHelper(tableHelper);

			parseMaster.Parse(inputString);
			parseMaster.Parse(inputArrayString);
		}

		TEST_METHOD(TestCloning)
		{
			Scope s;
			SharedTableData tableData(s);
			JsonParseCoordinator parseMaster(tableData);

			JsonTableParseHelper tableHelper;

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());

			parseMaster.AddHelper(tableHelper);

			Assert::IsTrue(1_z == parseMaster.GetHelperCount());

			JsonParseCoordinator* parseClone = parseMaster.Clone();

			Assert::IsTrue(1_z == parseClone->GetHelperCount());

			Assert::IsFalse(tableData.GetJsonParseCoordinator() == parseClone->GetSharedData()->GetJsonParseCoordinator());
			Assert::IsFalse(&tableData == reinterpret_cast<SharedTableData*>((parseClone->GetSharedData())));

			JsonTableParseHelper alreadyAddedHelper;
			Assert::ExpectException<std::runtime_error>([&parseMaster, &alreadyAddedHelper] {parseMaster.AddHelper(alreadyAddedHelper); });
			Assert::ExpectException<std::runtime_error>([&parseClone, &alreadyAddedHelper] {parseClone->AddHelper(alreadyAddedHelper); });
			Assert::ExpectException<std::runtime_error>([&parseClone, &alreadyAddedHelper] {parseClone->RemoveHelper(alreadyAddedHelper); });
			Assert::ExpectException<std::runtime_error>([&parseClone, &tableData] {parseClone->SetSharedData(tableData); });

			delete parseClone;
		}

		TEST_METHOD(TestMoveSemantics)
		{
			Scope s;
			SharedTableData tableData(s);
			JsonParseCoordinator parseMaster(tableData);

			JsonTableParseHelper tableHelper;

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());

			parseMaster.AddHelper(tableHelper);

			Assert::IsTrue(1_z == parseMaster.GetHelperCount());

			JsonParseCoordinator* parseClone = parseMaster.Clone();

			Assert::IsTrue(1_z == parseClone->GetHelperCount());

			Assert::IsFalse(tableData.GetJsonParseCoordinator() == parseClone->GetSharedData()->GetJsonParseCoordinator());
			Assert::IsFalse(&tableData == reinterpret_cast<SharedTableData*>((parseClone->GetSharedData())));

			JsonParseCoordinator parseMovedMaster = std::move(parseMaster);

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());
			Assert::IsTrue(1_z == parseMovedMaster.GetHelperCount());

			parseMaster = std::move(*parseClone);

			Assert::IsTrue(1_z == parseMaster.GetHelperCount());
			Assert::IsTrue(0_z == parseClone->GetHelperCount());

			delete parseClone;
		}

		TEST_METHOD(ExtraCodeCoverageTests)
		{
			Scope s;
			SharedTableData tableData(s);
			JsonParseCoordinator parseMaster(tableData);
			TestHelper testAddHelper;

			SharedTableData tableDataTwo(s);

			parseMaster.SetSharedData(tableDataTwo);
			parseMaster.GetFileName();

			JsonTableParseHelper tableHelper;

			tableHelper.Initialize();

			Assert::IsTrue(0_z == parseMaster.GetHelperCount());

			parseMaster.AddHelper(tableHelper);

			Assert::IsTrue(1_z == parseMaster.GetHelperCount());

			parseMaster.AddHelper(testAddHelper);
			parseMaster.RemoveHelper(testAddHelper);

			JsonParseCoordinator* parseClone = parseMaster.Clone();

			Assert::IsTrue(1_z == parseClone->GetHelperCount());

			Assert::IsFalse(tableData.GetJsonParseCoordinator() == parseClone->GetSharedData()->GetJsonParseCoordinator());
			Assert::IsFalse(&tableData == reinterpret_cast<SharedTableData*>((parseClone->GetSharedData())));

			*parseClone = std::move(parseMaster);

			parseClone->RemoveHelper(tableHelper);

			std::string test = R"({ "soloStringInt": {
										"type": "integer",
										"value": 10 } })";

			parseClone->Parse(test);

			parseClone->AddHelper(tableHelper);

			{
				JsonNotValidData notData;
				JsonParseCoordinator testAndFail(notData);
				JsonTableParseHelper testHelper;
				testAndFail.AddHelper(testHelper);

				testAndFail.Parse(test);

				testAndFail.RemoveHelper(testHelper);

				Assert::IsFalse(testHelper.EndHandler(notData, "invalid"));
			}

			delete parseClone;
		}

		TEST_METHOD(TestTableParsing)
		{
			Scope s;
			ScopeFactory scopeFactory;
			SharedTableData tData(s);
			JsonNotValidData notData; // For Type Checking against SharedData Type
			tData.SetRootScope(s); // Redundant, just for coverage.
			JsonParseCoordinator parseMaster(notData);

			Datum* d;

			JsonTableParseHelper tHelper;
			parseMaster.AddHelper(tHelper);

			std::string fileName = "Content/JsonTableInputTest.json";

			parseMaster.ParseFromFile(fileName); // Shouldnt do anything

			d = s.Find("Name");
			Assert::IsTrue(nullptr == d);	//	Not correct shared data

			Assert::IsFalse(tHelper.EndHandler(notData, "Name"s));

			parseMaster.SetSharedData(tData);	//	Correct data

			fileName = "Content/JsonTableInputTest.json";

			//	Register the Factory
			PowerFactory powerFactory;

			parseMaster.ParseFromFile(fileName);	//	Parses correctly

			Assert::IsTrue(s.Size() == 8_z);

			d = s.Find("Name");
			Assert::IsTrue("Test Name"s == *(d));

			d = s.Find("Health");
			Assert::IsTrue(100 == *(d));

			d = s.Find("Direction");
			Assert::IsTrue(vec4(1, 0, 0, 1) == *(d));

			d = s.Find("Transform");
			mat4x4 testMatrix = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
			Assert::IsTrue(testMatrix == *(d));
			
			//	String Array
			d = s.Find("Aliases");
			Assert::IsTrue(3_z == d->Size());
			Assert::IsTrue("Bob"s == d->Get<string>(0));
			Assert::IsTrue("Jim"s == d->Get<string>(1));
			Assert::IsTrue("Fred"s == d->Get<string>(2));

			//	Nested Table
			d = s.Find("Address");
			Assert::IsTrue(1_z == d->Size());
			Scope* sp = &d->operator[](0);

			d = sp->Find("Street");
			Assert::IsTrue("123 Anystreet St."s == *d);

			d = sp->Find("City");
			Assert::IsTrue("Orlando"s == *d);

			d = sp->Find("Postal Code");
			Assert::IsTrue(Datum::DatumType::Table == d->Type());

			sp = &(*d)[0];

			d = sp->Find("Zip Code");
			Assert::IsTrue(12345 == *d);

			d = sp->Find("Zip Code + 4");
			Assert::IsTrue(6789 == *d);

			d = s.Find("Powers");
			
			Assert::IsTrue(2_z == d->Size());
			
			sp = &(*d)[0];

			Datum* dTwo = sp->Find("Name");
			Assert::IsTrue("Laser Vision"s == *dTwo);

			dTwo = sp->Find("Dps");
			Assert::IsTrue(1.5f == *dTwo);

			sp = &(*d)[1];

			dTwo = sp->Find("Name");
			Assert::IsTrue("X-Ray Vision"s == *dTwo);

			dTwo = sp->Find("Dps");
			Assert::IsTrue(0.0f == *dTwo);

			Assert::IsTrue(0_z == tData.Depth());

			JsonParseCoordinator* clone = parseMaster.Clone();

			delete clone;
		}

		TEST_METHOD(RTTIMacroCoverage)
		{
			JsonTableParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("Bar"s));
			Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
			Assert::IsTrue(rtti->Is("JsonTableParseHelper"s));
			Assert::IsFalse(rtti->Is(Bar::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTableParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::IsTrue(JsonTableParseHelper::TypeIdClass() == rtti->TypeIdInstance());

			Bar* b = rtti->As<Bar>();
			Assert::IsNull(b);

			JsonTableParseHelper* h = rtti->As<JsonTableParseHelper>();
			Assert::IsNotNull(h);
			Assert::IsTrue(&helper == h);
			RTTI* r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Bar::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(CoverageGaps)
		{
			Scope s;
			SharedTableData tableData(s);

			Assert::IsTrue(&s == tableData.GetRootScope());

			JsonTableParseHelper th;
			JsonParseCoordinator parseMaster(tableData);
			parseMaster.AddHelper(th);

			std::string intentionallyBrokenString =
				R"({ "SomeName": {
						"class": "Nonexistent",
						"type": "table",
						"value": {
							"Street": {
								"type": "string",
								"value": "123 Anystreet St."
							}
						} 
					} 
				})";

			Assert::ExpectException<std::runtime_error>([&intentionallyBrokenString, &parseMaster] { parseMaster.Parse(intentionallyBrokenString); });

			intentionallyBrokenString =
				R"({ "Name": {
						"type": 10,
						"value": 10 } })";

			Assert::ExpectException<std::runtime_error>([&intentionallyBrokenString, &parseMaster] { parseMaster.Parse(intentionallyBrokenString); });

			intentionallyBrokenString =
				R"({ "SomeName": {
						"class": 10,
						"type": "table",
						"value": {
							"Street": {
								"type": "string",
								"value": "123 Anystreet St."
							}
						} 
					} 
				})";

			Assert::ExpectException<std::runtime_error>([&intentionallyBrokenString, &parseMaster] { parseMaster.Parse(intentionallyBrokenString); });
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ParseCoordinatorTests::_startMemState;
}
