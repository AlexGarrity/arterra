#include "PCH.hpp"
#include <glm/glm.hpp>

#include "ui/GUIElement.hpp"
#include "texture/TextureAtlas.hpp"

namespace arterra {
	
	enum UIElementAnchor {
		TopLeft = 0,
		TopCentre = 1,
		TopRight = 2,
		Left = 3,
		Centre = 4,
		Right = 5,
		BottomLeft = 6,
		BottomCentre = 7,
		BottomRight = 8
	};
	
	class UIElement {
		
		public:
			UIElement();
			UIElement(int width, int height, glm::vec2 position, UIElementAnchor anchor, AtlasTexture* texture);
			
			inline GuiElement* GetGUIElement() { return &_guiElement; }
		
		private:
			// Reference to this ui elements gui stored in the GuiRenderer.
			GuiElement _guiElement;
			
			// Width, height, and coordinates of this element.
			int _width, _height;
			glm::vec2 _position;
			// The position of the anchor.
			UIElementAnchor _anchor;
			
	};
	
}