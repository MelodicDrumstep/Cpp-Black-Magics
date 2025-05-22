#include <string_view>
#include "compile_time_json_parser.hpp"

using namespace metaprog;

int main()
{
    std::string_view responseBody {R"(
    [
    {
        "exclusive": false,
        "users": [
        {"name": "toddler", "age": 2},
        {"name": "baby", "age": 1}
        ]
    },
    {
        "exclusive": true,
        "users": [
        {"name": "user1", "age": 30},
        {"name": "user2", "age": 25}
        ]
    },
    {
        "exclusive": null,
        "users": []
    }
    ]
    )"};
    JSON<ListOf<UserGroup>> userGroups {responseBody};
    return 0;
}