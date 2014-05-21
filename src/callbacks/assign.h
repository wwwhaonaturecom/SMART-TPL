/**
 *  Assign.h
 *
 *  Signature of the assign callback. This is a callback that will call back
 *  to the native function smart_tpl_assign(void*, const char *data, size_t size, void*);
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace SmartTpl { namespace Internal {

/**
 *  Class definition
 */
class AssignCallback : public AssignBaseCallback
{
public:
    /**
     *  Constructor
     */
    AssignCallback()
    : AssignBaseCallback(jit_type_void_ptr)
    {
    }

    /**
     *  Destructor
     */
    virtual ~AssignCallback() {}
};

/**
 *  End namespace
 */
}}