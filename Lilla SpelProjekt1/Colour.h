#pragma once
typedef unsigned char Byte;

class Colour
{
private:
	union
	{
		Byte rgba[4];
		unsigned int colour;
	};
public:
	Colour();
	Colour(unsigned int value);
	Colour(Byte r, Byte g, Byte b);
	Colour(Byte r, Byte g, Byte b, Byte a);
	Colour(const Colour& source);

	Colour& operator=(const Colour& source);
	bool operator==(const Colour& rhs) const;
	bool operator!=(const Colour& rhs) const;
	
	constexpr Byte getR() const;
	void setR(Byte r);

	constexpr Byte getG() const;
	void setG(Byte g);

	constexpr Byte getB() const;
	void setB(Byte b);

	constexpr Byte getA() const;
	void setA(Byte a);
	
};