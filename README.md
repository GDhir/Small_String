# Small String Optimization

### Custom String class Implementation with small string optimization.  

The implementation requires that for small enough strings whose size is 
less than the template parameter, the string is not dynamically allocated. 

For strings of size greater than the template parameter, the string is
allocated as usual dynamically. 

Static and Dynamic char arrays are used for this purpose.

All assignment operators (copy and move), copy constructors (copy and move) and
allocators from raw strings are customized to suit this implementation.

For out of bounds access, the string throws an exception.
