//stater code

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 
#include <sstream> 
#include <complex>
#include <vector>

enum class State
{
  Calculating,
  DISPLAYING
};
class ComplexPlane : public VertexArray
{
public:
  ComplexPlane(int pixelWidth, int pixelHeight);
  void draw(RenderTarget& target, RenderStates states) const;
  void zoomIn();
  void zoomOut();
  void setCenter(Vector2i mousePixel);
  void setMouseLocation(Vector2i mousePixel);
  void loadText(Text& text);
  void updateRender();
private:
  int countIterations(Vector2i coord);
  void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
  Vector2f mapPixelToCoords(Vector2i mousePixel);
  
}
