/**
 *  AssignNumeric.h
 *
 *  Signature of the assign_numeric callback. This is a callback that will call back
 *  to the native function smart_tpl_assign_numeric(void *, const char *data, size_t size, long);
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
class AssignNumericCallback : public BaseCallback
{
public:
    /**
     *  Constructor
     */
    AssignNumericCallback()
    {
        // parameters that are supported
        jit_type_t params[] = {
            jit_type_void_ptr,
            jit_type_void_ptr,
            jit_type_ulong,
            jit_type_sys_int,
        };

        // create the signature
        _signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, params, sizeof(params)/sizeof(jit_type_t), 1);
    }

    /**
     *  Destructor
     */
    virtual ~AssignNumericCallback() {}
};

/**
 *  End namespace
 */
}}