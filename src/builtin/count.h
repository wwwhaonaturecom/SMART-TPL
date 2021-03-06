/**
 *  Count.h
 *
 *  Built-in "|count" modifier
 *
 *  @author Toon Schoenmakers <toon.schoenmakers@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace SmartTpl { namespace Internal {

/**
 *  Class definition
 */
class CountModifier : public Modifier
{
public:
    /**
     *  Destructor
     */
    virtual ~CountModifier() {};

    /**
     *  Modify a value object
     *  @param  input
     *  @param  params      Parameters used for this modification
     *  @return Value
     */
    VariantValue modify(const Value &input, const SmartTpl::Parameters &params) override
    {
        // Simply return the member count
        return (int64_t) input.memberCount();
    }
};

/**
 *  End namespace
 */
}}