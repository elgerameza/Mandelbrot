#include "ComplexPlane.h"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = pixelHeight / pixelWidth ;
	m_plane_center = { 0.f,0.f };
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
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = State::Calculating;
}
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
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

	cout << "Mouse Location updated: (" << m_mouselocation.x << ", " << m_mouselocation.y << ")" << endl;
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

	cout << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
	cout << "Cursor: (" << m_mouselocation.x << ", " << m_mouselocation.y << ")" << endl;
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
				Vector2f coord = mapPixelToCoords({j,i});
				int iterations = countIterations(coord);

				Uint8 r;
				Uint8 g;
				Uint8 b;
				iterationsToRGB(iterations, r, g, b);

				m_vArray[i * pixelWidth + j].position = Vector2f{ (float)j,(float)i };
				m_vArray[i * pixelWidth + j].color = Color{ r ,g, b };
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
	float norm = float(count) / MAX_ITER;
	if (count == MAX_ITER)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	else
	{
		r = (255 * norm);
		g = (255 * norm);
		b = (255 * norm);
	}
	

}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) 
{
	float x_offset = m_plane_center.x - m_plane_size.x / 2.0f;
	float y_offset = m_plane_center.y - m_plane_size.y / 2.0f;
	float x = (float)(mousePixel.x) / m_pixel_size.x * m_plane_size.x;
	float y = (float)(mousePixel.y) / m_pixel_size.y * m_plane_size.y;

		sf::Vector2f mapped = { x + x_offset, y + y_offset };
		return mapped;
}