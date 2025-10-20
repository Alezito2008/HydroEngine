#include "graphics/VertexArray.h"
#include "Debug.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

// Constructor de movimiento
VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_RendererID(other.m_RendererID)
{
	other.m_RendererID = 0; // Invalidar el ID en el objeto origen
}

// Operador de asignación de movimiento
VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other) {
		// Liberar el VAO actual si existe
		if (m_RendererID != 0) {
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}

		// Transferir el ID
		m_RendererID = other.m_RendererID;
		other.m_RendererID = 0; // Invalidar el ID en el objeto origen
	}
	return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) {
		const VertexBufferElement& elem = elements[i];
		glVertexAttribPointer(i, elem.count, elem.type, static_cast<GLboolean>(elem.normalized), layout.GetStride(), (void*)(static_cast<size_t>(offset)));
		GLCall(glEnableVertexAttribArray(i));
		offset += elem.count * elem.GetSizeOfType(elem.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
