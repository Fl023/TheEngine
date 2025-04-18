#include "tepch.h"
#include "Mesh.h"

namespace TheEngine {

    Mesh::Mesh(float* vertices, BufferLayout& layout, uint32_t* indices, std::vector<Ref<TheEngine::Texture2D>>& textures)
        : m_Textures(textures)
    {
        m_VertexArray = VertexArray::Create();

        Ref<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));
        VB->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(VB);

        Ref<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(IB);

    }

    Mesh::Mesh(std::vector<float>& vertices, BufferLayout& layout, std::vector<uint32_t>& indices, std::vector<Ref<TheEngine::Texture2D>>& textures)
        : m_Textures(textures)
    {
        // Create and bind the vertex array
        m_VertexArray = VertexArray::Create();

        // Create and set up the vertex buffer
        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        // Create and set up the index buffer
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
        m_VertexArray->SetIndexBuffer(indexBuffer);
    }

}