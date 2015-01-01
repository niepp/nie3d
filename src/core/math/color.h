#pragma once

struct CORE_API Color
{
	union
	{
		struct
		{
			Uint8 r;
			Uint8 g;
			Uint8 b;
			Uint8 a;
		};
		Uint32 c;
	};

	static const Color White;
	static const Color Black;
	static const Color Zero;
	static const Color Gray;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;

	Color();
	Color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255);
	Color(Uint32 color);

	void Set(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255);
	void Set(Uint32 color);

	bool operator ==(const Color &color) const;
	bool operator !=(const Color &color) const;

};

inline Color Lerp(const Color color1, const Color color2, float t)
{
	return Color((Uint8)(color1.r + (color2.r - color1.r) * t),
                 (Uint8)(color1.g + (color2.g - color1.g) * t),
                 (Uint8)(color1.b + (color2.b - color1.b) * t),
                 (Uint8)(color1.a + (color2.a - color1.a) * t));

}

inline Color::Color()
{
	c = 0;
}

inline Color::Color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a /*= 255*/)
{
	r = _r, g = _g, b = _b, a = _a;
}

inline Color::Color(Uint32 color)
{
	c = color;
}

inline void Color::Set(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a /*= 255*/)
{
	r = _r, g = _g, b = _b, a = _a;
}

inline void Color::Set(Uint32 color)
{
	c = color;
}

inline bool Color::operator ==(const Color &color) const
{
	if (r == color.r && g == color.g && b == color.b && a == color.a)
	{
		return true;
	}
	return false;
}

inline bool Color::operator !=(const Color &color) const
{
	return !(*this == color);
}



