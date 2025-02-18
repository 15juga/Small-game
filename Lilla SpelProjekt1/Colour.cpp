#include "Colour.h"

Colour::Colour()
{}

Colour::Colour(unsigned int value):
	colour(value)
{}

Colour::Colour(Byte r, Byte g, Byte b):
	Colour(r,g,b, 255)
{}

Colour::Colour(Byte r, Byte g, Byte b, Byte a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

Colour::Colour(const Colour& source):
	colour(source.colour)
{}

Colour& Colour::operator=(const Colour & source)
{
	this->colour = source.colour;
	return *this;
}

bool Colour::operator==(const Colour& rhs) const
{
	return (this->colour == rhs.colour);
}

bool Colour::operator!=(const Colour& rhs) const
{
	return !(*this == rhs);
}

constexpr Byte Colour::getR() const
{
	return this->rgba[0];
}

void Colour::setR(Byte r)
{
	this->rgba[0] = r;
}

constexpr Byte Colour::getG() const
{
	return this->rgba[1];
}

void Colour::setG(Byte g)
{
	this->rgba[1] = g;
}

constexpr Byte Colour::getB() const
{
	return this->rgba[2];
}

void Colour::setB(Byte b)
{
	this->rgba[2] = b;
}

constexpr Byte Colour::getA() const
{
	return this->rgba[3];
}

void Colour::setA(Byte a)
{
	this->rgba[3] = a;
}