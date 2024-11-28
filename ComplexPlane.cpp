#include "ComplexPlane.h"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = pixelHeight / pixelWidth ;
	m_plane_center = { 0.0,0.0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::Calculating;
	m_vArray.resize(pixelWidth * pixelHeight);
	m_vArray.setPrimitiveType(Points);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	double x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	double y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = State::Calculating;
}
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	int x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	int y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = State::Calculating;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = State::Calculating;

}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouselocation = mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text) 
{
	//ostringstream oss
	ostringstream oss;
	oss << "Mandelbtrot set" << endl;
	oss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
	oss << "Cursor (" << m_mouselocation.x << "," << m_mouselocation.y << ")" << endl;
	oss << "Left Click to zoom in." << endl;
	oss << "Right Click to zoom out." << endl;
	//text.setString(...)
	text.setString(oss.str());
}
void ComplexPlane::updateRender()
{

	if (m_state == State::Calculating)
	{
		int pixelHeight = m_pixel_size.y;
		int pixelWidth = m_pixel_size.x;

		for (int i = 0; i < pixelHeight; i++)
		{
			for (int j = 0; j < pixelWidth; j++)
			{
				Vector2i pixelWidth(j,i);
				Vector2f coord = mapPixelToCoords(pixelWidth);
				int iterations = countIterations(coord);

				Uint8 r;
				Uint8 g;
				Uint8 b;
				iterationsToRGB(iterations, r, g, b);

				m_vArray[i * pixelWidth.x + j].position = Vector2f{ (float)j,(float)i };
				m_vArray[i * pixelWidth.x + j].color = Color{ r ,g, b };
			}
		}
		m_state = State::DISPLAYING;
	}
}
int ComplexPlane::countIterations(Vector2f coord)
{
	complex<float> c(coord.x, coord.y); //
	complex<float> z = c;
	int i = 0;
	while (abs(z) < 2.0 && i < 64)
	{
		z = z * z + c;
		i++;
	}
	if (i == 64)
	{
		cout << c << " converges!" << endl;
	}
	else
	{
		cout << c << " escapes after " << i << " iterations" << endl;
	}
	return i;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == 64)
	{
		r = 0;
		g = 0;
		b = 0;
	}//setting it to black
	else if (count < 32)
	{
		r = 255;
		g = 0;
		b = 0;
	}//setting it to red
	else if (count < 16 && count == 29)
	{
		r = 0;
		g = 255;
		b = 0;
	}//setting it to green
	else if (count < 8)
	{
		r = 0;
		g = 0;
		b = 255;
	}//setting it to blue
	else if (count < 4)
	{
		r = 255;
		g = 255;
		b = 0;
	}//setting it to yellow
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) 
{
		float x_offset = m_plane_center.x - m_plane_size.x / 2.0f,
			y_offset = m_plane_center.y - m_plane_size.y / 2.0f,
			x = (float)(mousePixel.x) / m_pixel_size.x * m_plane_size.x,
			y = (float)(mousePixel.y - m_pixel_size.y) / (0 - m_pixel_size.y) * m_plane_size.y;

		sf::Vector2f mapped = { (x + x_offset), (y + y_offset) };
		return mapped;
}