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

    ListValue *list = new ListValue;
    for (int i = 0; i < 5; ++i) list->add(i);

    Data data;
    data.assign("list", std::shared_ptr<Value>(list));

    string expectedOutput("item: 0\nitem: 1\nitem: 2\nitem: 3\nitem: 4\n");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
    }
}

TEST(RunTime, ForEachWithKeys)
{
    string input("{foreach $map as $key => $value}key: {$key}\nvalue: {$value}{/foreach}");
    Template tpl((Buffer(input)));

    MapValue *map = new MapValue;
    map->assign("1", 1)
       .assign("2", 2)
       .assign("3", 3)
       .assign("4", 4)
       .assign("5", 5);
    Data data;
    data.assign("map", std::shared_ptr<Value>(map));

    string expectedOutput("key: 1\nvalue: 1key: 2\nvalue: 2key: 3\nvalue: 3key: 4\nvalue: 4key: 5\nvalue: 5");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
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

TEST(RunTime, ArrayAccess)
{
    string input("{$list[3]}");
    Template tpl((Buffer(input)));

    ListValue *list = new ListValue;
    for (int i = 0; i < 5; ++i) list->add(i);

    Data data;
    data.assign("list", std::shared_ptr<Value>(list));

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

    MapValue *map = new MapValue;
    map->assign("key", "test");
    Data data;
    data.assign("map", std::shared_ptr<Value>(map));

    string expectedOutput("test");
    EXPECT_EQ(tpl.process(data), expectedOutput);

    if (compile(tpl)) // This will compile the Template into a shared library
    {
        Template library(File(SHARED_LIBRARY)); // Here we load that shared library
        EXPECT_EQ(library.process(data), expectedOutput);
    }
}