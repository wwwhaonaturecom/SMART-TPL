/**
 *  Data.cpp
 *
 *  Implementation file for the Data class
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Namespace
 */
namespace SmartTpl {

/**
 *  Constructor
 */
Data::Data()
{
    // register two built-in modifiers
    // @todo would be better if these were global or static variables, instead
    // of objects allocated on the heap
    modifier("toupper", new ToUpperModifier);
    modifier("tolower", new ToLowerModifier);
}
    
/**
 *  Assign data
 *  @param  name        Name of the variable
 *  @param  value       Value of the variable
 *  @return Data        Same object for chaining
 */
Data &Data::assign(const char *name, const std::string &value)
{
    // append variable
    _variables[name] = std::unique_ptr<Value>(new StringValue(value));
    
    // allow chaining
    return *this;
}
    
/**
 *  Assign data
 *  @param  name        Name of the variable
 *  @param  value       Value of the variable
 *  @return Data        Same object for chaining
 */
Data &Data::assign(const char *name, int value)
{
    // append variable
    _variables[name] = std::unique_ptr<Value>(new NumericValue(value));

    // allow chaining
    return *this;
}

/**
 * Assign data
 * @param  name         Name of the variable
 * @param  value        Value of the variable
 * @return Data         Same object for chaining
 */
Data &Data::assign(const char *name, Value* value)
{
    // append variable
    // @todo the unique_ptr takes over ownership, this is probably now something we want!
    _variables[name] = std::unique_ptr<Value>(value);

    // allow chaining
    return *this;
}

/**
 *  Assign a modifier
 *  @param  name        Name of the modifier
 *  @param  modifier    Pointer to the modifier object
 *  @return Data
 */
Data &Data::modifier(const char *name, Modifier* modifier)
{
    // assign variable
    // @todo the unique_ptr takes over ownership, this is probably now something we want!
    _modifiers[name] = std::unique_ptr<Modifier>(modifier);
    
    // allow chaining
    return *this;
}

/**
 *  Retrieve a variable pointer by name
 *  @param  name        the name
 *  @param  size        size of the name
 *  @return Value*
 */
Value *Data::value(const char *name, size_t size) const
{
    // look up variable
    auto iter = _variables.find(name);
    if (iter == _variables.end()) return nullptr;
    
    // get the pointer
    return iter->second.get();
}

/**
 *  Retrieve a pointer to a modifier
 *  @param  name        Name of the modifier
 *  @param  size        Length of the name
 *  @return Modifier*
 */
Modifier* Data::modifier(const char* name, size_t size) const
{
    // check if the modifier is listed
    auto iter = _modifiers.find(name);
    if (iter == _modifiers.end()) return nullptr;
    
    // get the pointer
    return iter->second.get();
}

/**
 *  End namespace
 */
}

