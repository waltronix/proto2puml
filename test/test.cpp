#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "proto2puml.h"

TEST(Dependencies, contains_message)
{
    const std::string proto =
        R"RAW(
syntax = "proto3";

message sub_msg{
    int32 id = 0;
}

message msg{
  sub_msg sub = 0;
}
)RAW";

    std::string puml = proto_to_puml(proto.c_str());

    std::string regex = R"R(msg *o-- *sub_msg)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));
}

TEST(Dependencies, contains_message_list)
{
    const std::string proto =
        R"RAW(
syntax = "proto3";

message sub_msg{
    int32 id = 0;
}

message msg{
  repeated sub_msg sub = 0;
}
)RAW";

    std::string puml = proto_to_puml(proto.c_str());

    std::string regex = R"R(msg *\*-- *sub_msg)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));
}

TEST(Dependencies, contains_message_map)
{
    const std::string proto =
        R"RAW(
syntax = "proto3";

message sub_msg{
    int32 id = 0;
}

message msg{
  map<int32, sub_msg> sub = 0;
}
)RAW";

    std::string puml = proto_to_puml(proto.c_str());

    std::string regex = R"R(msg *\*-- *sub_msg)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));
}

TEST(Dependencies, contains_oneof_message)
{
    const std::string proto =
        R"RAW(
syntax = "proto3";

message sub_msg_one{
    int32 id = 0;
}

message sub_msg_two{
    int32 id = 0;
}

message msg{
    oneof sub_msg {
      sub_msg_one sub_one = 1;
      sub_msg_two sub_two = 2;
    }
}
)RAW";

    std::string puml = proto_to_puml(proto.c_str());

    std::string regex = R"R(msg *o-- *sub_msg_one)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));

    regex = R"R(msg *o-- *sub_msg_two)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));
}

TEST(Fields, contains_oneof_message)
{
    const std::string proto =
        R"RAW(
syntax = "proto3";

message sub_msg_one{
    int32 id = 0;
}

message sub_msg_two{
    int32 id = 0;
}

message msg{
    oneof sub_msg {
      sub_msg_one sub_one = 1;
      sub_msg_two sub_two = 2;
    }
}
)RAW";

    std::string puml = proto_to_puml(proto.c_str());

    std::string regex = R"R(union\<)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));

    regex = R"R(sub_msg_one)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));

    regex = R"R(sub_msg_two)R";
    EXPECT_THAT(puml, testing::ContainsRegex(regex));
}
