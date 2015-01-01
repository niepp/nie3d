#pragma once

template<typename T>
struct Rect
{
	T left, top, right, bottom;

	Rect(){ }
	Rect(T l, T t, T r, T b) : left(l), top(t), right(r), bottom(b)
	{
	}

	void Set(T l, T t, T r, T b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	T GetWidth() const{ return right - left; } const
	T GetHeight() const{ return bottom - top; } const

	inline Bool IsRectIn(const Rect& rect) const
	{
		if(rect.left < left || rect.right > right ||
			rect.top < top || rect.bottom > bottom)
			return false;
		return true;
	}

	Bool IsPointIn(T x, T y) const
	{
		if(x >= left && x <= right &&
			y >= top && y <= bottom)
			return true;
		return false;
	}

};

//get the intersect of two rect
template<typename T>
inline Bool IsIntersect(const Rect<T>& a, const Rect<T>& b)
{
	if(a.right <= b.left || a.bottom <= b.top || 
		b.right <= a.left || b.bottom <= a.top)
		return false;
	return true;
}

//get the intersect of two rect
template<typename T>
inline Rect<T> Intersect(const Rect<T>& a, const Rect<T>& b)
{
	Rect<T> rect;
	if(a.left < b.left)
		rect.left = b.left;
	if(a.top < b.top)
		rect.top = b.top;
	if(a.right > b.right)
		rect.right = b.right;
	if(a.bottom > b.bottom)
		rect.bottom = b.bottom;
	return rect;
}

typedef Rect<Int32> RectInt;
typedef Rect<Float> RectFloat;
