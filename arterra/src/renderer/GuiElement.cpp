#include "renderer/GuiElement.hpp"

namespace arterra {

	GuiElement::GuiElement() {}

	GuiElement::GuiElement(std::vector<float> &posVertices, std::vector<float> &uvVertices)
		: _posVertices(posVertices)
		  , _uvVertices(uvVertices) { GenerateMesh(); }

	void GuiElement::Destroy()
	{
		_vertexArray.Destroy();
		_posBuffer.Destroy();
		_texBuffer.Destroy();
	}

	void GuiElement::GenerateMesh()
	{
		_posBuffer.Create(_posVertices, 2, GL_FLOAT);
		_texBuffer.Create(_uvVertices, 2, GL_FLOAT);
		_vertexArray.AddBuffer(_posBuffer);
		_vertexArray.AddBuffer(_texBuffer);
	}

	void GuiElement::SetShouldRender(bool shouldRender)
	{
		_shouldRender = shouldRender;
	}
	
	bool GuiElement::ShouldRender() const
	{
		return _shouldRender;
	}
	
	void GuiElement::Bind()
	{
		_vertexArray.Bind();
	}
	
	GLuint GuiElement::GetVertexCount()
	{
		return _vertexArray.GetVertexCount();
	}

}
