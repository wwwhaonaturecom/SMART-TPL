/**
 *  Runtime.cpp
 *
 *  Runtime related tests, these tests will be running some basic templates
 *  with both jit and the compiled shared libraries
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

#include <gtest/gtest.h>
#include <../smarttpl.h>

#include "ccode.h"

using namespace SmartTpl;
using namespace std;

TEST(RunTime, ForEach)
{
    string input("{foreach $item in $list}item: {$item}\n{/foreach}");
    Template tpl((Buffer(input)));

    std::vector<Variant::Value> list;
    for (int i = 0; i < 5; ++i) list.push_back(i);

    Data data;
    data.assign("list", list);

    string expectedOutput("item: 0\nitem: 1\nitem: 2\nitem: 3\nitem: 4\n");
    EXPECT_EQ(expectedOutput, tpl.process(data));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, library.process(data));
    }
}

TEST(RunTime, ForEachWithKeys)
{
    string input("{foreach $map as $key => $value}key: {$key}\nvalue: {$value}{/foreach}");
    Template tpl((Buffer(input)));

    std::map<std::string, Variant::Value> map;
    map["1"] = 1;
    map["2"] = 2;
    map["3"] = 3;
    map["4"] = 4;
    map["5"] = 5;
    Data data;
    data.assign("map", map);

    string expectedOutput("key: 1\nvalue: 1key: 2\nvalue: 2key: 3\nvalue: 3key: 4\nvalue: 4key: 5\nvalue: 5");
    EXPECT_EQ(expectedOutput, tpl.process(data));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, library.process(data));
    }
}

TEST(RunTime, ForEachElse)
{
    string input("{foreach $item in $list}item: {$item}\n{foreachelse}else{/foreach}");
    Template tpl((Buffer(input)));

    std::vector<Variant::Value> list;
    for (int i = 0; i < 5; ++i) list.push_back(i);

    Data data;
    data.assign("list", list);

    Data data1;
    data1.assign("list", "Not loopable");

    string expectedOutput("item: 0\nitem: 1\nitem: 2\nitem: 3\nitem: 4\n");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    EXPECT_EQ("else", tpl.process(data1));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, library.process(data));
        EXPECT_EQ("else", library.process(data1));
    }
}

TEST(RunTime, ForEachWithKeysElse)
{
    string input("{foreach $map as $key => $value}key: {$key}\nvalue: {$value}{foreachelse}else{/foreach}");
    Template tpl((Buffer(input)));

    std::map<std::string, Variant::Value> map;
    map["1"] = 1;
    map["2"] = 2;
    map["3"] = 3;
    map["4"] = 4;
    map["5"] = 5;
    Data data;
    data.assign("map", map);

    Data data1;
    data1.assign("map", "Not loopable");

    string expectedOutput("key: 1\nvalue: 1key: 2\nvalue: 2key: 3\nvalue: 3key: 4\nvalue: 4key: 5\nvalue: 5");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    EXPECT_EQ("else", tpl.process(data1));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, library.process(data));
        EXPECT_EQ("else", library.process(data1));
    }
}

TEST(RunTime, If)
{
    string input("{if true}true{else}false{/if}");
    Template tpl((Buffer(input)));

    string expectedOutput("true");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, IfElse)
{
    string input("{if $var}true{elseif $var1}false{/if}");
    Template tpl((Buffer(input)));

    Data data1;
    data1.assign("var", true);
    Data data2;
    data2.assign("var1", true);
    EXPECT_EQ("", tpl.process());
    EXPECT_EQ("true", tpl.process(data1));
    EXPECT_EQ("false", tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("", library.process());
        EXPECT_EQ("true", library.process(data1));
        EXPECT_EQ("false", library.process(data2));
    }
}

TEST(RunTime, StringComparisonEquals)
{
    string input("{if \"string1\" == \"string2\"}true{else}false{/if}");
    Template tpl((Buffer(input)));

    string expectedOutput("false");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, StringComparisonNotEquals)
{
    string input("{if \"string1\" != \"string2\"}true{else}false{/if}");
    Template tpl((Buffer(input)));

    string expectedOutput("true");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, NumericComparison)
{
    string input("{if 1 == 1}true{else}false{/if}");
    Template tpl((Buffer(input)));

    string expectedOutput("true");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, BooleanComparison)
{
    string input("{if true == true}true{else}false{/if}");
    Template tpl((Buffer(input)));

    string expectedOutput("true");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, Assigning)
{
    string input("{$var}-{$var=1}-{$var}");
    Template tpl((Buffer(input)));

    string expectedOutput("--1");
    EXPECT_EQ(tpl.process(), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(), expectedOutput);
    }
}

TEST(RunTime, AssigningInForEach)
{
    string input("{foreach $item in $list}{$output=$item}{/foreach}{$output}");
    Template tpl((Buffer(input)));

    std::vector<Variant::Value> list;
    for (int i = 0; i < 5; ++i) list.push_back(i);

    Data data;
    data.assign("list", list);

    string expectedOutput("4");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
    }
}

TEST(RunTime, ArrayAccess)
{
    string input("{$list[3]}");
    Template tpl((Buffer(input)));

    std::vector<Variant::Value> list;
    for (int i = 0; i < 5; ++i) list.push_back(i);

    Data data;
    data.assign("list", list);

    string expectedOutput("3");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
    }
}

TEST(RunTime, KeyArrayAccess)
{
    string input("{$map[\"key\"]}");
    Template tpl((Buffer(input)));

    std::map<std::string, Variant::Value> map;
    map["key"] = "test";
    Data data;
    data.assign("map", map);

    string expectedOutput("test");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
    }
}

TEST(RunTime, CompareVarConstantNumeric)
{
    string input("{if $var == 1}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data1;
    data1.assign("var", 1);

    Data data2;
    data2.assign("var", 2);

    EXPECT_EQ("true", tpl.process(data1));
    EXPECT_EQ("false", tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("true", library.process(data1));
        EXPECT_EQ("false", library.process(data2));
    }
}

TEST(RunTime, CompareVarConstantBoolean)
{
    string input("{if $var == true}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data1;
    data1.assign("var", true);

    Data data2;
    data2.assign("var", false);

    EXPECT_EQ("true", tpl.process(data1));
    EXPECT_EQ("false", tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("true", library.process(data1));
        EXPECT_EQ("false", library.process(data2));
    }
}

TEST(RunTime, CompareVarConstantString)
{
    string input("{if $var == \"string\"}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data1;
    data1.assign("var", "string");

    Data data2;
    data2.assign("var", "false string");

    EXPECT_EQ("true", tpl.process(data1));
    EXPECT_EQ("false", tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("true", library.process(data1));
        EXPECT_EQ("false", library.process(data2));
    }
}

TEST(RunTime, CompareVarVar)
{
    string input("{if $var1 == $var2}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data1;
    data1.assign("var1", "string")
         .assign("var2", "string");

    Data data2;
    data2.assign("var1", "string")
         .assign("var2", "other string");

    EXPECT_EQ("true", tpl.process(data1));
    EXPECT_EQ("false", tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("true", library.process(data1));
        EXPECT_EQ("false", library.process(data2));
    }
}

TEST(RunTime, Int64)
{
    string input("{if $int64 > 2147483647}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("int64", 922337203685477580);

    EXPECT_EQ("true", tpl.process(data));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ("true", tpl.process(data));
    }
}

TEST(RunTime, OutputMath)
{
    // Spaces here after the + and the - are purely required so that the parser
    // actually knows what is a part of the number and what isn't
    string input("1+3-2*10={1+ 3- 2*10}\n(1+3-2)*10={(1+ 3- 2)*10}");
    Template tpl((Buffer(input)));

    string expectedOutput("1+3-2*10=-16\n(1+3-2)*10=20");
    EXPECT_EQ(expectedOutput, tpl.process());

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process());
    }
}

TEST(RunTime, OutputMathVariables)
{
    string input("10*var={10*$var}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("var", 200);
    Data data2;
    data2.assign("var", 22);

    string expectedOutput("10*var=2000");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    string expectedOutput2("10*var=220");
    EXPECT_EQ(expectedOutput2, tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process(data));
        EXPECT_EQ(expectedOutput2, tpl.process(data2));
    }
}

TEST(RunTime, OutputMathOnlyVariables)
{
    string input("var*var={$var*$var}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("var", 200);
    Data data2;
    data2.assign("var", 22);

    string expectedOutput("var*var=40000");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    string expectedOutput2("var*var=484");
    EXPECT_EQ(expectedOutput2, tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process(data));
        EXPECT_EQ(expectedOutput2, tpl.process(data2));
    }
}

TEST(RunTime, Modulo)
{
    string input("1+3*10%5={1+ 3*10%5}");
    Template tpl((Buffer(input)));

    string expectedOutput("1+3*10%5=1");
    EXPECT_EQ(expectedOutput, tpl.process());

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process());
    }
}

TEST(RunTime, AssignMath)
{
    string input("{assign $var*11 to $test}{$test}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("var", 456);

    string expectedOutput("5016");;
    EXPECT_EQ(expectedOutput, tpl.process(data));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process(data));
    }
}

TEST(RunTime, MathIf)
{
    string input("{if $var%3 == 1}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("var", 10);
    Data data2;
    data2.assign("var", 11);

    string expectedOutput("true");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    string expectedOutput2("false");
    EXPECT_EQ(expectedOutput2, tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process(data));
        EXPECT_EQ(expectedOutput2, tpl.process(data2));
    }
}

TEST(RunTime, DoubleVariableComparison)
{
    string input("{if $double > 3}true{else}false{/if}");
    Template tpl((Buffer(input)));

    Data data;
    data.assign("double", 3.14);

    Data data2;
    data.assign("double", 12.3456789);

    string expectedOutput("true");
    EXPECT_EQ(expectedOutput, tpl.process(data));
    string expectedOutput2("false");
    EXPECT_EQ(expectedOutput2, tpl.process(data2));

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process(data));
        EXPECT_EQ(expectedOutput2, tpl.process(data2));
    }
}

TEST(RunTime, AssignDouble)
{
    string input("{assign 1.5e17 to $test}{$test}");
    Template tpl((Buffer(input)));

    string expectedOutput("150000000000000000.000000");
    EXPECT_EQ(expectedOutput, tpl.process());

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(expectedOutput, tpl.process());
    }
}

TEST(RunTime, ZeroDivision)
{
    string input("{1/0}");
    Template tpl((Buffer(input)));
    EXPECT_THROW(tpl.process(), std::runtime_error);
}