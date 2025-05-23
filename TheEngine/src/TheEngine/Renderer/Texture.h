#pragma once

#include <string>

#include "TheEngine/Core/Core.h"


namespace TheEngine {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		void SetType(const std::string& type = "none") { m_Type = type; }
		const std::string& GetType() const { return m_Type; }

	private:
		std::string m_Type;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};

	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(const std::vector<std::string>& faces);
		static Ref<TextureCube> Create(const std::string& path);
	};
}