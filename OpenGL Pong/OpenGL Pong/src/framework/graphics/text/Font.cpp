#include "Font.h"

#include <sstream>
#include <stdexcept>

#include "../../utility/Logger.h"

Font::Font(const std::string& fontFilepath, const unsigned int fontSize)
{
	m_glyphs.clear();
	
	FT_Library ftLibrary = nullptr;

	if (FT_Init_FreeType(&ftLibrary) != 0)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to initialise FreeType library.";

		throw std::runtime_error(errorMessageStream.str());
	}

	FT_Face face = nullptr;

	if (FT_New_Face(ftLibrary, fontFilepath.c_str(), 0, &face) != 0)
	{
		std::ostringstream errorMessageStream;
		errorMessageStream << "ERROR: Failed to load font file: " << fontFilepath << ".";

		throw std::runtime_error(errorMessageStream.str());
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	constexpr unsigned char MaxASCIICount = 128u;

	for (unsigned char c = 0; c < MaxASCIICount; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			std::ostringstream warningMessageStream;
			warningMessageStream << "WARNING: Failed to load glyph " << c << " for font " << fontFilepath << ".";

			Logger::Log(warningMessageStream.str());

			continue;
		}

		m_glyphs.insert({ c, CreateGlyph(face) });
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);
}

Font::Glyph Font::CreateGlyph(const FT_Face& face) const
{
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const Glyph glyph{
		textureID,
		glm::ivec2{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
		glm::ivec2{ face->glyph->bitmap_left, face->glyph->bitmap_top },
		static_cast<unsigned int>(face->glyph->advance.x)
	};

	return glyph;
}
