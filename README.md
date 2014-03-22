SMART-TPL
=========

Experimental library to see if it is somehow possible to compile Smarty templates 
into native code.  Although Smarty is a PHP library, the SmartTpl project is _not_ 
limited to PHP. SmartTpl is a pure C++ library to work with templates.


What does it do?
----------------

Templates are stored in *.tpl files, and contain HTML code mixed with special 
{$variables} and {if} and {foreach} statements for variable content.

````html
<html>
 <body>
  Hello {$name}!
  
  {if $age > 18}
    You are older than 18.
  {/if}
 </body>
</html>
````

Templates are not limited to just HTML, you can also use them for other file
formats. The syntax of the templates is loosely based on the syntax that the
Smarty template engine offers.

The SmartTpl library does nothing exciting: it turns the templates into HTML, 
by replacing all the variables with their actual value, and evaluating the {if} 
and {foreach} statements.

SmartTpl is not a real programming language - it only offers very basic 
statements, and does not allow one to create and call functions or make 
complicated data structures. In this it also is (intentionally) less powerful
than Smarty. If you want to do real programming, you can combine SmartTpl with
a real programming language.


Compiling templates
-------------------

The SmartTpl library allows you to compile your templates into native executable
code. This makes your templates much faster, because they do not have to be
evaluated at runtime. By compiling your templates, they are directly turned
into native executable code that runs directly on the CPU. 

An added benefit of compiling templates is that the source code becomes next to 
unreadable. If you want to distribute your templates, but do not want external
parties to modify them, you can compile them to make editing much harder.

To compile a template, you simply use the 'smarttpl' compiler:

````
smarttpl mytemplate.tpl
````

This will turn your mytemplate.tpl into a mytemplate.so file, which can later
be passed to the Template class.


JIT compiler
------------

Compiling your templates is <i>optional</i>. If you choose not to compile them,
the template will be compiled to native code at runtime with the built-in 
Just-In-Time compiler. If you use the same template object a couple of times, 
it will only have to be runtime compiled once.

Therefore, with this JIT compiler, only the first time that a template is 
displayed it is a little slower. For all subsequent calls it is as fast as a 
compiled template.


Displaying a template
---------------------

A template object first need to be loaded from a certain source. The SmartTpl
library has two classes that can be used for this: The SmartTpl::File class
for templates that are loaded from the filesystem, and the SmartTpl::Memory
class for templates that are "loaded" from an in-memory buffer.

Both the SmartTpl::Memory and SmartTpl::File class are derived from the
SmartTpl::Source class. If you want to add an alternative template source 
(for example for templates that are loaded from a database) you can create your 
own class that also extends from SmartTpl::Source.

````c++
// required code
#include <smarttpl.h>

// example function that displays a template that is loaded from the filesystem
void example1()
{
    // use a template stored on the filesystem
    SmartTpl::File source("mytemplate.tpl");

    // create the template object
    SmartTpl::Template tpl(source);
    
    // output the template to stdout
    std::cout << tpl;
}

// example function that displays a template that is already stored in memory
void example2()
{
    // use a template available in mempory
    SmartTpl::Memory source("<html>....</html>", 12345);
    
    // create the template object
    SmartTpl::Template tpl(source);
    
    // output the template to stdout
    std::cout << tpl;
}

// use an already-compiled template
void example3()
{
    // use a template stored on the filesystem
    SmartTpl::File source("mytemplate.so");

    // create the template object
    SmartTpl::Template tpl(source);
    
    // output the template to stdout
    std::cout << tpl;
}
````

Assigning data
--------------

Inside a template you can make use of {$variables}. Before you display a 
template, you should first assign the values of all these variables to a 
data object which is used by the template.

You can assign integers and strings to a data object.


````c++
// required code
#include <smarttpl.h>

// example how to assign data
void example()
{
    // create a template
    SmartTpl::Template tpl(SmartTpl::File("mytemplate.tpl"));
    
    // create a data object
    SmartTpl::Data data;
    
    // assign data
    data.assign("name", "John Doe");
    data.assign("age", 32);
    
    // show the template
    std::cout << tpl.process(data);
}
````

More complicated data can be assigned to, like maps and vectors. These can be
used in the templates with variables like {$varname.member} or {$varname[0]}.

The next powerful feature of SmartTpl is that it supports callbacks. This is 
especially useful if you do not know if a variable is going to be used in a 
template, and you do not want to fetch all information in advance.

````c++
// required code
#include <smarttpl.h>

// example how to assign data
void example()
{
    // create a template
    SmartTpl::Template tpl(SmartTpl::File("mytemplate.tpl"));
    
    // create a data object
    SmartTpl::Data data;
    
    // assign data
    data.assign("name", []() -> const char * {
    
        // @todo 
        //      find out the value of the name, for example by starting
        //      a database query
    
        return name;
    });
    
    // show the template
    std::cout << tpl.process(data);
}

In the example above the {$name} variable is assigned to the template, but 
the value is a callback function. Only when the {$name} variable is actually 
_used_, this callback gets called, and your application has to run the
algorithm to retrieve the name.

The SmartTpl template language allows you to use nested variables. You can
use variables like {$name.member.submember[$index].member}. To make this 
fully flexible, you can override the SmartTpl::Variable class to create
your own variable.

````c++
// required code
#include <smarttpl.h>

/**
 *  our own variable class
 */
class MyVariable : public SmartTpl::Variable
{
public:
    /**
     *  Method that gets called when the variable has to be turned into a
     *  string. This is for example called when a {$variable} is used in
     *  a template.
     *
     *  @return const char *
     */
    virtual const char *toString() override
    {
        return "abcd";
    }
    
    /**
     *  Method that returns the size of the variable (which is the length of
     *  the string).
     *
     *  @return size_t
     */
    virtual size_t size() override
    {
        return 4;
    }
    
    /**
     *  Method that gets called when the variable is used in a numeric 
     *  context, to turn the variable into an integer
     *
     *  @return int64_t
     */
    virtual int64_t toNumeric() override
    {
        return 0;
    }
    
    /**
     *  Method that gets called when a member is accessed. This is called
     *  when the variable is accessed in the template as {$var.x} or {$var[$x]}
     *
     *  This method should return a SmartTpl::Variable object.
     *
     *  @param  name        name of the member
     *  @return Variable    Pointer to a new variable
     */
    virtual SmartTpl::Variable member(const char *name) override
    {
        // check the name
        if (strcmp(name, "x") == 0) return SomeOtherVariable();
        if (strcmp(name, "self") == 0) return this;
        
        // not found
        return nullptr;
    }
};

// example how to assign data
void example()
{
    // create a template
    SmartTpl::Template tpl(SmartTpl::File("mytemplate.tpl"));
    
    // create a data object
    SmartTpl::Data data;
    
    // assign data
    data.assign("name", MyVariable());
    
    // show the template
    std::cout << tpl.process(data);
}

````

By overriding from SmartTpl::Variable, you can create all sorts of variables
that behave like arrays or objects. The SmartTpl library already has built-in
types for a number of types, like SmartTpl::Numeric and SmartTpl::String.


