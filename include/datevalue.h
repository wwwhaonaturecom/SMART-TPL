/**
 *  DateValue.h
 *
 *  Class that represents a date variable
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
class DateValue : public Value
{
private:
    /**
     *  The date printing format
     *  @var    std::string
     */
    const std::string _format;

    /**
     *  The timestamp to format, in case this is 0 we'll print the current time
     *  @var   std::time_t
     */
    const std::time_t _timestamp;

    /**
     *  Buffer that will hold the current output
     */
    mutable std::string _buffer;

    /**
     *  Put the current date/time in the _buffer
     *  @note This method is only const as it is called from const methods
     */
    void initializeDate() const
    {
        // Get the current timestamp
        std::time_t time = _timestamp;

        // If time is 0 we want to current time
        if (time == 0) time = std::time(NULL);
        // if our buffer isn't empty and our time was already set we have everything we need so we return
        else if (!_buffer.empty()) return;

        // Convert it to our local time
        std::tm *timeinfo = std::localtime(&time);

        // Print it into _buffer using strftime
        // http://en.cppreference.com/w/cpp/chrono/c/strftime
        // First declare a char buffer
        char buf[_buffer.capacity()];

        // Then print into our character buffer
        std::size_t len = std::strftime(buf, _buffer.capacity(), _format.c_str(), timeinfo);

        // as long as len is 0 we failed printing it, so increase our _buffer and redeclare a char buffer
        while (len == 0)
        {
            // reserve more space in _buffer
            _buffer.reserve(_buffer.capacity() * 2);

            // declare a new char buffer
            char nbuf[_buffer.capacity()];

            // call strftime again
            len = std::strftime(nbuf, _buffer.capacity(), _format.c_str(), timeinfo);
            if (len > 0)
            {
                // if std::strftime returned more than 0 assign nbuf to _buffer and return
                _buffer = nbuf;
                return;
            }
        }

        // If we reach this we never entered the loop, so just assign the original buf
        _buffer = buf;
    }

public:
    /**
     *  Constructor
     *  @param format The format in which the date should be represented
     *                an empty format is undefined behavior and will throw for
     *                documentation regarding the format please head over to
     *                http://en.cppreference.com/w/cpp/chrono/c/strftime
     */
    DateValue(const std::string &format, const std::time_t timestamp = 0)
    : _format(format)
    , _timestamp(timestamp) {
        _buffer.reserve(_format.size());
        if (_format.empty()) throw std::runtime_error("A DateValue with an empty format is undefined");
    }

    /**
     *  Destructor
     */
    virtual ~DateValue() {}

    /**
     *  Convert the variable to a string
     *  @return std::string
     */
    std::string toString() const override
    {
        // Put the current date/time in our buffer
        initializeDate();

        // return the buffer as a C String
        return _buffer;
    }

    /**
     *  Returns the current unix timestamp
     *  @return numeric
     */
    numeric_t toNumeric() const override
    {
        // Get the current timestamp
        std::time_t time = _timestamp;

        // If time is 0 we want to current time
        if (time == 0) time = std::time(NULL);

        // return the timestamp
        return time;
    }

    /**
     *  Convert the variable to a floating point value
     *  @return double
     */
    double toDouble() const override { return toNumeric(); };

    /**
     *  Convert the variable to a boolean value
     *  @return bool
     */
    bool toBoolean() const override
    {
        return false;
    }

    /**
     *  Get access to a member variable
     *
     *  @param  name        name of the member
     *  @param  size        size of the name
     *  @return VariantValue
     */
    VariantValue member(const char *name, size_t size) const override
    {
        return nullptr;
    }

    /**
     *  Get access to the amount of members this value has
     */
    size_t memberCount() const override
    {
        return 0;
    }

    /**
     *  Get access to a member at a certain position
     *  @param  position    Position of the item we want to retrieve
     *  @return VariantValue
     */
    VariantValue member(size_t position) const override
    {
        return nullptr;
    }

    /**
     *  Create a new iterator that allows you to iterate over the subvalues
     *  feel free to return nullptr if you don't want to be able to iterate
     *  over your type
     *
     *  @return Newly allocated Iterator
     */
    Iterator *iterator() const override
    {
        return nullptr;
    }
};

/**
 *  End namespace
 */
}
