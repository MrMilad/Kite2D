#ifndef KNONCOPYABLE_H
#define KNONCOPYABLE_H

namespace Kite{
    class KNonCopyable{
    protected:
        KNonCopyable () {}
        ~KNonCopyable () {} // Protected non-virtual destructor
    private:
        KNonCopyable (const KNonCopyable &);
        KNonCopyable & operator = (const KNonCopyable &);
    };
}

#endif // KNONCOPYABLE_H
