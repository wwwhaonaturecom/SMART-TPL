/**
 *  Boolean.h
 *
 *  Class that represents a boolean
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace SmartTpl {

/**
 *  Class definition
 */
class BooleanValue : public Value
{
public:
    static std::shared_ptr<BooleanValue> get(bool b)
    {
        if (b)
        {
            static std::shared_ptr<BooleanValue> _true(std::make_shared<BooleanValue>(true));
            return _true;
        }
        else
        {
            static std::shared_ptr<BooleanValue> _false(std::make_shared<BooleanValue>(false));
            return _false;
        }
    }
private:

    /**
     *  The boolean value
     */
    const bool _boolean;
public:
    /**
     *  Constructor
     */
    BooleanValue(bool b) : _boolean(b) {};

    /**
     *  Deconstructor
     */
    virtual ~BooleanValue() {};
    /**
     *  Convert the variable to a string
     *  @return const char *
     */
    virtual const char *toString() const override
    {
        return _boolean ? "true" : "false";
    }

    /**
     *  Convert the variable to a numeric value
     *  @return numeric
     */
    virtual numeric_t toNumeric() const override
    {
        return 0;
    }

    /**
     *  Convert the variable to a boolean value
     *  @return bool
     */
    virtual bool toBoolean() const override
    {
        return _boolean;
    }

    /**
     *  Get access to a member variable
     *
     *  @param  name        name of the member
     *  @param  size        size of the name
     *  @return Variant
     */
    virtual Variant member(const char *name, size_t size) const override
    {
        return nullptr;
    }

    /**
     *  Get access to the amount of members this value has
     */
    virtual size_t memberCount() const override
    {
        return 0;
    }

    /**
     *  Get access to a member at a certain position
     *  @param  position    Position of the item we want to retrieve
     *  @return Variant
     */
    virtual Variant member(size_t position) const override
    {
        return nullptr;
    }

    /**
     *  Get access to the key at a certain position
     *  @param  position     Position of the key we want to retrieve
     *  @return Variant      Variant object, probably a string
     */
    virtual Variant key(size_t position) const override
    {
        return nullptr;
    }

    /**
     *  String length of the variable
     *
     *  @return int
     */
    virtual size_t size() const override
    {
        return _boolean ? 4 : 5; // "true" is 4 characters long, "false" is 5 characters long
    }

    /**
     *  Create a new iterator that allows you to iterate over the subvalues
     *  feel free to return nullptr from here in case memberCount returns 0
     *  as this method won't ever get called in that case anyway.
     *
     *  @return Newly allocated Iterator
     */
    virtual Iterator *iterator() const override
    {
        return nullptr;
    }
};

/**
 *  End namespace
 */
}