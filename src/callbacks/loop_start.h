/**
 *  LoopStart.h
 *
 *  Signature of the start_loop callback. This is a callback that will call back
 *  to the native function smart_tpl_loop_start(void*);
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace SmartTpl {

/**
 *  Class definition
 */
class LoopStartCallback : public Callback
{
public:
    /**
     *  Constructor
     */
    LoopStartCallback()
    {
        // parameters that are supported
        jit_type_t params[] = {
            jit_type_void_ptr
        };

        // create the signature
        _signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, params, sizeof(params)/sizeof(jit_type_t), 1);
    }

    /**
     *  Destructor
     */
    virtual ~LoopStartCallback() {}
};

/**
 *  End namespace
 */
}
