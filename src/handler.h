/**
 *  Handler.h
 *
 *  Internal class that captures the output while the template is being
 *  processed.
 *
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2014 Copernica BV
 */

/**
 *  Namespace
 */
namespace SmartTpl {

/**
 *  Class definition
 */
class Handler
{
private:
    /**
     *  Output buffer
     *  @var    std::string
     */
    std::string _buffer;

    /**
     *  The underlying data
     *  @var    Data
     */
    const Data *_data;

    /**
     *  Map values to the modifiers that created them so we know who
     *  needs to clean them up
     */
    std::map<Modifier*, std::set<Value*>> _destroy_later;

    /**
     *  Stack of our iterators
     */
    std::stack<std::pair<const char*, size_t>> _iterator_stack;

    /**
     *  Compare functor necessary for the map
     */
    struct cmp_str
    {
        /**
         *  Invoke operator
         *  @param  a
         *  @param  b
         *  @return bool
         */
        bool operator()(char const *a, char const *b) const
        {
            return std::strcmp(a, b) < 0;
        }
    };

    /**
     *  This map will contain values assigned during runtime, these can be
     *  assigned using "assign .. to .." or they're simply the magic values in
     *  a foreach loop
     */
    std::map<const char *, Value*, cmp_str> _local_values;

    /**
     *  Will contain the local values that were created just here and should
     *  because of that be deleted
     */
    std::set<std::unique_ptr<Value>> _managed_local_values;

public:
    /**
     *  Constructor
     *  @param  data        pointer to the data
     */
    Handler(const Data *data) : _data(data) 
    {
        _buffer.reserve(4096);
    }

    /**
     *  Destructor
     */
    virtual ~Handler()
    {
        // Loop through all the modifiers in the _destroy_later map
        for (auto & m : _destroy_later)
        {
            // For each modifier loop through all the values and clean them up
            for (auto & v : m.second) m.first->cleanup(v);
        }
    }

    /**
     *  Write data to the buffer
     *  @param  buffer
     *  @param  size
     */
    void write(const char *buffer, size_t size)
    {
        _buffer.append(buffer, size);
    }

    /**
     *  Get access to a variable
     *  @param  name
     *  @param  size
     *  @return Value
     */
    Value *variable(const char *name, size_t size)
    {
        auto iter = _local_values.find(name);
        if (iter != _local_values.end()) return iter->second;
        // get the variable from the data object
        return _data->value(name,size);
    }

    /**
     *  Iterate over a multi value value object
     *  @param value       The value to iterate over
     *  @param key         The magic key to assign the next value to
     *  @param size        The size of the magic key
     *  @param keyvar      The magic key to assign the member key to
     *  @param keyvar_size The size of keyvar (may be 0!)
     *  @return True if we should continue looping, false otherwise
     */
    bool iterate(Value *value, const char *key, size_t size, const char* keyvar, size_t keyvar_size)
    {
        // Retrieve the amount of members in value
        size_t len = value->memberCount();

        // We can't even iterate over this...
        if (len == 0) return false;

        // If our iterator stack is empty create a new iterator
        if (_iterator_stack.empty())
        {
            std::pair<const char*, size_t> iter(key, 0);
            _iterator_stack.push(iter);

            // assign the first element of the iteration to our magic key
            _local_values[key] = value->member(0);

            // Let's see if they want the key as well
            if (keyvar_size > 0)
            {
                // The foreach wants a key, so let's try to get one
                Value *k = value->key(iter.second);

                // Value isn't required to return a key so let's check if it is null
                if (k != nullptr) _local_values[keyvar] = k;
            }
            return true;
        }
        else
        {
            // Retrieve the last used iterator
            std::pair<const char*, size_t> &iter = _iterator_stack.top();

            // If it has the same key as we do it is the iterator we need
            if (std::strcmp(key, iter.first) == 0)
            {
                // Increase our iterator by 1 and check if it goes over the bounds
                if (++iter.second >= len)
                {
                    // if it does go over the boundaries just remove it from the stack
                    _iterator_stack.pop();

                    // and remove our magic value
                    auto liter = _local_values.find(key);
                    if (liter != _local_values.end()) _local_values.erase(liter);

                    // Let's look for our magic key var as well if we used it that is
                    if (keyvar_size > 0)
                    {
                        liter = _local_values.find(keyvar);
                        if (liter != _local_values.end()) _local_values.erase(liter);
                    }

                    // and tell the callback to stop looping
                    return false;
                }

                // assign the next element in the iteration to our magic key
                _local_values[key] = value->member(iter.second);
                if (keyvar_size > 0)
                {
                    // The foreach wants a key, so let's try to get one
                    Value *k = value->key(iter.second);

                    // Value isn't required to return a key so let's check if it is null
                    if (k != nullptr) _local_values[keyvar] = k;
                }
                return true;
            }
            else
            {
                // If it is not the same key just create a new iterator
                std::pair<const char*,size_t> iter(key, 0);
                _iterator_stack.push(iter);

                // assign the first element of the iteration to our magic key
                _local_values[key] = value->member(0);
                if (keyvar_size > 0)
                {
                    // The foreach wants a key, so let's try to get one
                    Value *k = value->key(0);

                    // Value isn't required to return a key so let's check if it is null
                    if (k != nullptr) _local_values[keyvar] = k;
                }
                return true;
            }
        }
    }

    /**
     *  Return the generated output
     *  @return std::string
     */
    const std::string &output() const
    {
        return _buffer;
    }

    /**
     *  Return a modifier by name
     *  @param name
     *  @param size
     *  @return Modifier
     */
    Modifier* modifier(const char* name, size_t size)
    {
        return _data->modifier(name, size);
    }

    /**
     *  Mark a value as clean up later on
     *  @param modifier
     *  @param value
     */
    void destroyValue(Modifier *modifier, Value *value)
    {
        _destroy_later[modifier].insert(value);
    }

    /**
     *  Assign an existing value to a local variable
     *  @param value       The value we would like to assign
     *  @param key         The name for our local variable
     *  @param key_size    The size of key
     */
    void assign(Value *value, const char *key, size_t key_size)
    {
        _local_values[key] = value;
    }

    /**
     *  Assign a boolean value to a local variable
     *  @param boolean     The boolean value we want to assign
     *  @param key         The name for our local variable
     *  @param key_size    The size of key
     */
    void assignBoolean(bool boolean, const char *key, size_t key_size)
    {
        _local_values[key] = BooleanValue::get(boolean);
    }

    /**
     *  Assign a numeric value to a local variable
     *  @param value        The numeric value we want to assign
     *  @param key          The name of our local variable
     *  @param key_size     The size of key
     */
    void assignNumeric(long value, const char *key, size_t key_size)
    {
        Value *v = new NumericValue(value);
        _managed_local_values.insert(std::unique_ptr<Value>(v));
        _local_values[key] = v;
    }

    /**
     *  Assign a string value to a local variable
     *  @param value          The string value we want to assign
     *  @param key            The name of our local variable
     *  @param key_size       The size of key
     */
    void assignString(const std::string &value, const char *key, size_t key_size)
    {
        Value *v = new StringValue(value);
        _managed_local_values.insert(std::unique_ptr<Value>(v));
        _local_values[key] = v;
    }
};

/**
 *  End namespace
 */
}

