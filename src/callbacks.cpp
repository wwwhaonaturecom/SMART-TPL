/**
 *  Callbacks.cpp
 *
 *  Implementation of the callbacks
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */
#include "includes.h"

/**
 *  Set up namespace
 */
namespace SmartTpl {

/**
 *  We always have a empty variable in place
 */
static EmptyValue empty;

/**
 *  Signature of the write callback
 *  @var    WriteCallback
 */
WriteCallback Callbacks::_write;

/**
 *  Signature of the output callback
 *  @var    OutputCallback
 */
OutputCallback Callbacks::_output;

/**
 *  Signature of the member callback
 *  @var    MemberCallback
 */
MemberCallback Callbacks::_member;

/**
 *  Signature of the member_at callback
 *  @var    MemberAtCallback
 */
MemberAtCallback Callbacks::_member_at;

/**
 *  Signature of the member iter callback
 *  @var MemberIterCallback
 */
MemberIterCallback Callbacks::_member_iter;

/**
 *  Signature of the variable callback
 *  @var    MemberCallback
 */
VariableCallback Callbacks::_variable;

/**
 *  Signature of the function to convert a variable to a string
 *  @var    ToStringCallback
 */
ToStringCallback Callbacks::_toString;

/**
 *  Signature of the function to convert a variable to a numeric value
 *  @var    ToNumericCallback
 */
ToNumericCallback Callbacks::_toNumeric;

/**
 *  Signature of the function to convert a variable to a boolean value
 *  @var    ToBooleanCallback
 */
ToBooleanCallback Callbacks::_toBoolean;

/**
 *  Signature of the function to retrieve the size/strlen of a variable
 *  @var    SizeCallback
 */
SizeCallback Callbacks::_size;

/**
 *  Signature of the function to retrieve the modifier
 *  @var    ModifierCallback
 */
ModifierCallback Callbacks::_modifier;

/**
 *  Signature of the function to modify a variable
 *  @var    ModifyVariableCallback
 */
ModifyVariableCallback Callbacks::_modify_variable;

/**
 *  Signature of the function to modify a numeric variable
 *  @var ModifyNumericCallback
 */
ModifyNumericCallback Callbacks::_modify_numeric;

/**
 *  Signature of the function to modify a string variable
 *  @var ModifyStringCallback
 */
ModifyStringCallback Callbacks::_modify_string;

/**
 *  Signature of the function to compare 2 strings
 *  @var StrCmpCallback
 */
StrCmpCallback Callbacks::_strcmp;

/**
 *  Signature of the function to assign a variable to a local variable
 *  @var AssignCallback
 */
AssignCallback Callbacks::_assign;

/**
 *  Signature of the function to assign a boolean to a local variable
 *  @var AssignBooleanCallback
 */
AssignBooleanCallback Callbacks::_assign_boolean;

/**
 *  Signature of the function to assign a numeric value to a local variable
 *  @var AssignNumericCallback
 */
AssignNumericCallback Callbacks::_assign_numeric;

/**
 *  Signature of the function to assign a string to a local variable
 *  @var AssignStringCallback
 */
AssignStringCallback Callbacks::_assign_string;


/**
 *  Function to write raw data
 *  @param  userdata        pointer user-supplied data
 *  @param  data            pointer to the buffer
 *  @param  size            size of the data to write
 */
void smart_tpl_write(void *userdata, const char *data, size_t size)
{
    // convert the userdata to a handler object
    auto *handler = (Handler *)userdata;

    // call the handler
    handler->write(data, size);
}

/**
 *  Function to output a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to the variable
 */
void smart_tpl_output(void *userdata, void *variable)
{
    // convert the userdata to a handler object
    auto *handler = (Handler *)userdata;

    // convert the variable to a variable object
    auto *var = (Value *)variable;

    // output the variable
    handler->write(var->toString(), var->size());
}

/**
 *  Retrieve a pointer to a member
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @param  name            name of the variable
 *  @param  size            size of the variable
 *  @return                 pointer to a new variable
 */
void *smart_tpl_member(void *userdata, void *variable, const char *name, size_t size)
{
    // convert the variable to a variable object
    auto *var = (Value *)variable;

    // fetch the member
    auto *result = var->member(name, size);

    // ensure that we always return an object
    return result ? result : &empty;
}

/**
 *  Retrieve a pointer to a member at a certain position
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @param  position        what position would we like
 *  @return                 pointer to a new variable
 */
void* smart_tpl_member_at(void* userdata, void* variable, long position)
{
    // convert the variable to a value object
    auto *var = (Value *)variable;

    // fetch the member
    auto *result = var->member(position);

    // ensure that we always return an object
    return result ? result : &empty;
}

/**
 *  Check if we can continue iterating over variable and set the magic key to the next value
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @param  key             name of the magic variable
 *  @param  size            length of key
 *  @param  keyvar          name of the magic variable for the key
 *  @param  keyvar_size     length of the magic variable for the key
 *  @return                 1 if we can continue iterating, 0 otherwise
 */
int smart_tpl_member_iter(void *userdata, void *variable, const char *key, size_t size, const char *keyvar, size_t keyvar_size)
{
    // convert the variable to a Value object
    auto *var = (Value *) variable;

    // convert the userdata to our Handler object
    auto *handler = (Handler *) userdata;

    // execute the iterate method and return the result
    return handler->iterate(var, key, size, keyvar, keyvar_size) ? 1 : 0;
}

/**
 *  Retrieve a pointer to a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  name            name of the variable
 *  @param  size            size of the variable
 *  @return                 pointer to a new variable
 */
void *smart_tpl_variable(void *userdata, const char *name, size_t size)
{
    // convert the userdata to a handler object
    auto *handler = (Handler *)userdata;

    // convert to a variable
    auto *result = handler->variable(name, size);

    // ensure that we always return an object
    return result ? result : &empty;
}

/**
 *  Retrieve the string representation of a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @return                 string buffer
 */
const char *smart_tpl_to_string(void *userdata, void *variable)
{
    // convert the variable to a value object
    auto *var = (Value *)variable;

    // convert to string
    auto *result = var->toString();

    // ensure that a string is always returned
    return result ? result : empty.toString();
}

/**
 *  Retrieve the numeric representation of a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @return                 numeric value
 */
size_t smart_tpl_to_numeric(void *userdata, void *variable)
{
    // convert the variable to a value object
    auto *var = (Value *)variable;

    // convert to numeric
    return var->toNumeric();
}

/**
 *  Retrieve the boolean representation of a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @return                 numeric value
 */
int smart_tpl_to_boolean(void *userdata, void *variable)
{
    // convert the variable to a value object
    auto *var = (Value *)variable;

    // convert to bool
    return var->toBoolean();
}

/**
 *  Retrieve the length of the string representation of a variable
 *  @param  userdata        pointer to user-supplied data
 *  @param  variable        pointer to variable
 *  @return                 string length
 */
size_t smart_tpl_size(void *userdata, void *variable)
{
    // convert the variable to a value object
    auto *var = (Value *)variable;

    // return the size
    return var->size();
}

/**
 *  Retrieve the modifier by name
 *  @param userdata       pointer to user-supplied data
 *  @param name           name of the modifier the caller wants
 *  @param size           length of the name that the caller wants
 *  @return               A pointer to the modifier, or a nullptr if it wasn't found
 */
void* smart_tpl_modifier(void *userdata, const char *name, size_t size)
{
    // convert to Handler
    auto *handler = (Handler *)userdata;

    // Lookup and return the modifier in the Handler
    return handler->modifier(name, size);
}

/**
 *  Apply a modifier from smart_tpl_modifier on a value
 *  @param userdata       pointer to user-supplied data
 *  @param modifier_ptr   pointer to the modifier that should be applied
 *  @param variable       pointer to a value that we should apply the modifier on
 */
void* smart_tpl_modify_variable(void *userdata, void *modifier_ptr, void *variable)
{
    // In case the modifier is a nullptr just return the original value
    if (modifier_ptr == nullptr || variable == nullptr)
        return variable;
    // convert to the Modifier
    auto *modifier = (Modifier*) modifier_ptr;
    // convert to the Value object
    auto *value = (Value*) variable;

    auto *output = modifier->modify(value);
    if (output != value)
    {  // As our output value is different from our input value we mark it as destroy later
        auto *handler = (Handler *)userdata;
        handler->destroyValue(modifier, output);
    }
    return output;
}

/**
 *  Apply a modifier on a numeric type
 *  @param userdata     pointer to user-supplied data
 *  @param modifier     pointer to the modifier that should be applied
 *  @param value        the numeric type that should go through the modifier
 *  @todo Actually start using this method
 */
void* smart_tpl_modify_numeric(void *userdata, void *modifier, long value)
{
    return nullptr;
}

/**
 *  Apply a modifier on a string
 *  @param userdata     pointer to user-supplied data
 *  @param modifier     pointer to the modifier that should be applied
 *  @param value        the string that should be modified
 *  @param size         the length of value
 *  @todo Actually start using this method
 */
void* smart_tpl_modify_string(void *userdata, void *modifier, const char *value, size_t size)
{
    return nullptr;
}

/**
 *  Assign a numeric value to a local variable
 *  @param userdata        pointer to user-supplied data
 *  @param value           the numeric value we would like to assign
 *  @param key             the key we would like to assign it to
 *  @param keysize         the size of key
 */
void smart_tpl_assign_numeric(void *userdata, long value, const char *key, size_t keysize)
{
    // Convert userdata to our Handler
    auto handler = (Handler *) userdata;

    // Assign numeric value to key
    handler->assignNumeric(value, key, keysize);
}

/**
 *  Assign a boolean value to a local variable
 *  @param userdata             pointer to user-supplied data
 *  @param boolean              the boolean value
 *  @param key                  the key we would like to assign it to
 *  @param keysize              the size of key
 */
void smart_tpl_assign_boolean(void *userdata, int boolean, const char *key, size_t keysize)
{
    // Convert userdata to our Handler
    auto handler = (Handler *) userdata;

    // Assign boolean to key
    handler->assignBoolean(boolean == 1, key, keysize);
}

/**
 *  Assign a string value to a local variable
 *  @param userdata             pointer to user-supplied data
 *  @param buf                  the string we would like to assign
 *  @param buf_size             the size of buf
 *  @param key                  the key we would like to assign it to
 *  @param keysize              the size of key
 */
void smart_tpl_assign_string(void *userdata, const char *buf, size_t buf_size, const char *key, size_t keysize)
{
    // Convert userdata to our Handler
    auto handler = (Handler *) userdata;

    // Assign value to key
    handler->assignString(std::string(buf, buf_size), key, keysize);
}

/**
 *  Assign a variable to a local variable
 *  @param userdata             pointer to user-supplied data
 *  @param variable             the variable object we would like to assign
 *  @param key                  the key we would like to assign it to
 *  @param keysize              the size of key
 */
void smart_tpl_assign(void *userdata, void *variable, const char *key, size_t keysize)
{
    // Convert userdata to our Handler
    auto handler = (Handler *) userdata;

    // Convert value to type Value
    auto value = (Value *) variable;

    // Assign value to key
    handler->assign(value, key, keysize);
}

/**
 *  Compares 2 c strings
 *  @param userdata           Pointer to user-supplied data
 *  @param a
 *  @param a_len              Length of a
 *  @param b
 *  @param b_len              Length of b
 *  This method is just here to avoid having to rewrite the way CCode writes strings
 *  which is "string",6. And it makes comparing strings from libjit a lot easier.
 */
int smart_tpl_strcmp(void *userdata, const char *a, size_t a_len, const char *b, size_t b_len)
{
    if (a_len != b_len)
        return -1;
    return strncmp(a, b, a_len) == 0;
}

/**
 *  End namespace
 */
}

