#include "ComplexPlane.h"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size = { pixelWidth, pixelHeight};
	m_aspectRatio = pixelHeight / pixelWidth;
	m_plane_center = {0.0,0.0};
	m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
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
	int x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	int y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = {x, y};
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
	oss << "Center: ("<< m_plane_center.x << ", " << m_plane_center.y <<")" << endl;
	oss<< "Cursor ("<< m_mouselocation.x << "," << m_mouselocation.y <<")" << endl;
	oss << "Left Click to zoom in." << endl;
	cout << "Right Click to zoom out." << endl;
	//text.setString(...)
	text.setString(oss.str());
}
void ComplexPlane::updateRender()
{

	if (m_state == State::Calculating)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; i++)
			{
				Vector2i pixelWidth(j, i);
				Vector2f coord = mapPixelToCoords(pixelWidth);
				int iterations = countIterations(coord);

				Uint8 r;
				Uint8 g;
				Uint8 b;
				iterationsToRGB(iterations, r, g, b);

				m_vArray[j + i * pixelWidth.x].position = Vector2f {j,i};
				m_vArray[j + i * pixelWidth.x].color = Color{r ,g, b};
			}
		}
		m_state = State::DISPLAYING;
	}
}
int ComplexPlane::countIterations(Vector2f coord)
{
	complex<double> c(coord.x, coord.y); //
	complex<double> z = c;
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
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == 64)
	{
		r = 0;
		g = 0;
		b = 0;
	}//setting it to black
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	float scaleX = m_plane_center.x - m_plane_size.x ;
	float scaleY = m_plane_center.x - m_plane_size.x;

	float coordX = m_plane_center.x + (mousePixel.x - m_pixel_size.x / 2.0) * scaleX;
	float coordY = m_plane_center.x + (mousePixel.y - m_pixel_size.y / 2.0) * scaleY;

	return Vector2f(coordX, coordY);
}