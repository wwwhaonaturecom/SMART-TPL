/**
 *  Template.cpp
 *
 *  Implementation of the Template class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 - 2017 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace SmartTpl {

/**
 *  Constructor
 *  @param  source        Source of the template to load
 */
Template::Template(const Source &source)
{
    // is the source a shared library?
    if (source.library())
    {
        // hey that's cool, we can create create a shard library
        _executor = new Internal::Library(source.name());
    }
    else
    {
        // it was not a shared library, we're going to compile it into bytecode ourselves
        _executor = new Internal::Bytecode(source);
    }

    // Set the _encoding using the encoding() method on our executor
    _encoding = _executor->encoding();
}

/**
 *  Destructor
 */
Template::~Template()
{
    // we no longer need the executor
    delete _executor;
}

/**
 *  Is this template dependent on data to be personalised?
 *
 *  @return Whether the template uses personalisationd ata
 */
bool Template::personalized() const
{
    return _executor->personalized();
}

/**
 *  Get the template representation in C that can be compiled into a shared
 *  object. This method only works for templates that were not already a
 *  shared library.
 *
 *  @return std::string
 */
std::string Template::compile() const
{
    return _executor->compile();
}

/**
 *  Process the template, given a certain data source
 *
 *  The data object that needs to be passed to this method is an object
 *  that contains the values of all variables that can be user inside the
 *  template.
 *
 *  @param  data         Data source
 *  @param  outencoding  The encoding that should be used for the output
 *  @return std::string
 */
std::string Template::process(const Data &data, const std::string &outencoding) const
{
    // we need a handler object
    Internal::Handler handler(&data, Internal::Escaper::get(outencoding));

    // ask the executor to display the template
    _executor->process(handler);

    // In case our handler is set in failed mode we have to throw a runtime error
    if (handler.failed()) throw RunTimeError(handler.error());

    // return the generated output string
    return handler.output();
}

/**
 *  End namespace
 */
}

