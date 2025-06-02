#include "json.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

struct TestAddress {
    std::string street;
    std::string number;
    std::string postcode;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TestAddress, street, number, postcode);
};

struct TestPerson {
    std::string              name;
    int                      age;
    std::vector<TestAddress> addresses;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TestPerson, name, age, addresses);
};


void test_json_decode_1() {

    std::string json_str = R"(
        {
            "name": "Jane Doe",
            "age": 25,
            "addresses":[
                {
                    "street":"jiangxia",
                    "number":"258",
                    "postcode":"54321"
                },
                {
                    "street":"wuchang",
                    "number":"369",
                    "postcode":"54321"
                }
            ]
        }
    )";

    // example
    auto result = json_decode<TestPerson>(json_str);
    if (JsonError* err = std::get_if<JsonError>(&result)) {
        // hanle error
        std::cerr << "Error (" << static_cast<int>(err->type) << "): " << err->message << "\n";
    } else {
        TestPerson* person = std::get_if<TestPerson>(&result);
        // handle success
        std::cout << person->name << "\n";
    }
}

void test_json_decode_2() {
    std::string json_str = R"(
        {
            "name": "Jane Doe",
            "age": 25,
            "addresses":[
                {
                    "street":"jiangxia",
                    "number":"258",
                    "postcode":"54321"
                },
                {
                    "street":"wuchang",
                    "number":"369",
                    "postcode":"54321"
                }
            ]
        }
    )";

    TestPerson  person;

    auto        err = json_decode(json_str, &person);
    if (err != nullptr) {
        // hanle error
        std::cerr << "Error (" << static_cast<int>(err->type) << "): " << err->message << "\n";
        return;
    }

    std::cout << person.name << "\n";
}

void test_json_encode_1() {
    TestPerson person{"Alice", 28};

    person.name = "Alice--------";

    auto result = json_encode(person, 4, ' ');
    if (JsonError* err = std::get_if<JsonError>(&result)) {
        // hanle error
        std::cerr << "Error (" << static_cast<int>(err->type) << "): " << err->message << "\n";
        return;
    }

    std::string* str = std::get_if<std::string>(&result);


    std::cout << *str << "\n";
}

/*

int json_test_main() {
    test_json_decode_1();
    test_json_decode_2();
    test_json_encode_1();

    return 0;
}

*/