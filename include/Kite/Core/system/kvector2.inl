template <typename T>
inline KVector2<T>::KVector2():
    x(0), y(0)
{}

template <typename T>
inline KVector2<T>::KVector2( T mX, T mY):
    x(mX), y(mY)
{}

template <typename T>
template <typename U>
inline KVector2<T>::KVector2( const KVector2<U> &v ):
    x(static_cast<T>(v.x)),
    y(static_cast<T>(v.y))
{}

template <typename T>
inline KVector2<T> operator -(const KVector2<T>& right){
    return KVector2<T>(-right.x, -right.y);
}

template <typename T>
inline KVector2<T>& operator +=(KVector2<T>& left, const KVector2<T>& right){
    left.x += right.x;
    left.y += right.y;
    return left;
}

template <typename T>
inline KVector2<T>& operator -=(KVector2<T>& left, const KVector2<T>& right){
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

template <typename T>
inline KVector2<T> operator +(const KVector2<T>& left, const KVector2<T>& right){
    return KVector2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline KVector2<T> operator -(const KVector2<T>& left, const KVector2<T>& right){
    return KVector2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline KVector2<T> operator *(const KVector2<T>& left, T right){
    return KVector2<T>(left.x * right, left.y * right);
}

template <typename T>
inline KVector2<T> operator *(T left, const KVector2<T>& right){
    return KVector2<T>(right.x * left, right.y * left);
}

template <typename T>
inline KVector2<T>& operator *=(KVector2<T>& left, T right){
    left.x *= right;
    left.y *= right;
    return left;
}

template <typename T>
inline KVector2<T> operator /(const KVector2<T>& left, T right){
    return KVector2<T>(left.x / right, left.y / right);
}

template <typename T>
inline KVector2<T>& operator /=(KVector2<T>& left, T right){
    left.x /= right;
    left.y /= right;

    return left;
}

template <typename T>
inline bool operator ==(const KVector2<T>& left, const KVector2<T>& right){
    return (left.x == right.x) && (left.y == right.y);
}

template <typename T>
inline bool operator !=(const KVector2<T>& left, const KVector2<T>& right){
    return (left.x != right.x) || (left.y != right.y);
}

// distance only coded for 2D
template <typename T>
inline T distance(const KVector2<T> &v1, const KVector2<T> &v2){
    return sqrt((v2.x-v1.x)*(v2.x-v1.x) + (v2.y-v1.y)*(v2.y-v1.y));
}

// Vector3 length is distance from the origin
template <typename T>
inline T length(const KVector2<T> &v){
    return sqrt(v.x*v.x + v.y*v.y);
}

// dot/scalar product: difference between two directions
template <typename T>
inline T dotProduct( const KVector2<T> &v1 , const KVector2<T> &v2){
return (v1.x*v2.x + v1.y*v2.y);
}

// calculate normal angle of the Vector
template <typename T>
inline KVector2<T> normal(const KVector2<T> &v){
    T len;
    if (length(v) == 0)
        len = 0;
    else
        len = 1 / length(v);
    T nx = v.x*len;
    T ny = v.y*len;
    return KVector2<T>(nx,ny);
}