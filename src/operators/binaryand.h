/**
 *  BinaryAnd.h
 *
 *  Implementation of the binary && operator
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace SmartTpl { namespace Internal {

/**
 *  Class definition
 */
class BinaryAndOperator : public BinaryBooleanOperator
{
public:
    /**
     *  Constructor
     *  @param  left
     *  @param  right
     */
    BinaryAndOperator(Expression *left, Expression *right) :
        BinaryBooleanOperator(left, right) {}

    /**
     *  Destructor
     */
    virtual ~BinaryAndOperator() {}

    /**
     *  Generate the instruction
     *  @param  generator
     */
    void numeric(Generator *generator) const override
    {
        generator->booleanAnd(_left.get(), _right.get());
    }

    /**
     *  Generate the instruction
     *  @param  generator
     */
    void boolean(Generator *generator) const override
    {
        generator->booleanAnd(_left.get(), _right.get());
    }
};

/**
 *  End of namespace
 */
}}