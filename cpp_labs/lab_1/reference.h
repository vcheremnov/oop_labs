#ifndef REFERENCE_H
#define REFERENCE_H

#include "tritset.h"
#include "trits.h"

// A proxy class for accessing individual trits in TritSet

class TritSet::Reference {
    friend class TritSet;
public:
    operator Trit() const;
    Reference &operator= (Trit value);
    Reference &operator= (const Reference &ref);
    Reference &operator|= (Trit value);
    Reference &operator&= (Trit value);
private:
    Reference(TritSet &set, size_type tritIndex);
    Trit _value() const;                            // value of the reference
    // internal data
    TritSet &_set;                                  // reference to the associated tritset
    size_type _tritIndex;                           // trit's index in the tritset
};

#endif // REFERENCE_H
