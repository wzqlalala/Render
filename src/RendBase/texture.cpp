
#include <spdlog/logger.h>

#include "log.h"
#include "shader.h"
#include "texture.h"
#include <filesystem>
#include <QImage>
//#include "qopengl"
	// optimize context switching by avoiding unnecessary binds and unbinds


namespace mxr
{
	constexpr int n_texture_units = 32;
	static std::vector<GLuint> textures_binding_table(32, 0);  // keep track of textures bound in each unit


	TexView::TexView(const Texture& texture) : IAsset(), host(texture)
	{
		glGenTextures(1, &id);  // create the view's name only w/o initializing a texture object
	}

	TexView::~TexView()
	{
		glDeleteTextures(1, &id);
	}

	void TexView::SetView(GLenum target, GLuint fr_level, GLuint levels, GLuint fr_layer, GLuint layers)
	{
		glTextureView(id, target, host.id, host.i_format, fr_level, levels, fr_layer, layers);
	}

	void TexView::Bind(GLuint index)
	{
		if (id != textures_binding_table[index])
		{
			glBindTextureUnit(index, id);
			textures_binding_table[index] = id;
		}
	}

	void TexView::Unbind(GLuint index)
	{
		if (id == textures_binding_table[index])
		{
			glBindTextureUnit(index, 0);
			textures_binding_table[index] = 0;
		}
	}

	GLenum GetFormatFromIFormat(GLenum _format)
	{
		switch (_format)
		{
		case GL_R8:	
		case GL_R16F:
			return GL_RED;
		case GL_RG8:	
		case GL_RG16F:
			return GL_RG;
		case GL_RGB8:
		case GL_RGB8UI:
		case GL_RGB8I:
		case GL_RGB16F:
			return GL_RGB;
		case GL_RGBA8:
		case GL_RGBA16F:
			return GL_RGBA;
		default:
			break;
		}
	}

	GLenum GetTypeFromIFormat(GLenum _format)
	{
		switch (_format)
		{
		case GL_R8:
		case GL_RGB8:
		case GL_RGBA8:
		case GL_RGB8UI:
		case GL_RGB8I:
			return GL_UNSIGNED_BYTE;
		}
	}



	///////////////////////////////////////////////////////////////////////////////////////////////

	Texture::Texture(const QString& img_path, GLuint levels)
		: IAsset(), target(GL_TEXTURE_2D), depth(1), n_levels(levels)
	{

		QImage image = QImage(img_path).convertToFormat(QImage::Format_RGB888);
		this->width = image.width();
		this->height = image.height();
		if (image.format() == QImage::Format_RGB888)
		{
			this->format = GL_RGB;
			this->i_format = GL_RGB8;
		}
		else if (image.format() == QImage::Format_RGBA8888)
		{
			this->format = GL_RGBA;
			this->i_format = GL_RGBA8;
		}
		else
		{
			this->format = GL_RGB;
			this->i_format = GL_RGB8;
			//qDebug() << "Unsupported image format.";
			//return;
		}

		// �Զ����� mipmap ����
		if (levels == 0)
		{
			n_levels = 1 + static_cast<GLuint>(floor(std::log2(std::max(width, height))));
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, n_levels, i_format, width, height);

		// �� QImage ���ϴ�ͼ�����ݵ� GPU
		if (image.format() == QImage::Format_RGB888)
		{
			glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, image.bits());
		}
		else if (image.format() == QImage::Format_RGBA8888)
		{
			glTextureSubImage2D(id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, image.bits());
		}

		// ���� mipmap ����
		if (n_levels > 1)
		{
			glGenerateTextureMipmap(id);
		}

		SetSampleState();
	}

	Texture::Texture(const QString& img_path, GLuint resolution, GLuint levels)
		: IAsset(), target(GL_TEXTURE_CUBE_MAP), width(resolution), height(resolution), depth(6), n_levels(levels)
	{

		// a cubemap texture should be preferably created from a high dynamic range image
		//if (auto path = std::filesystem::path(img_path); path.extension() != ".hdr") 
		//{
		//	CORE_WARN("Attempting to build a cubemap from a non-HDR image...");
		//	CORE_WARN("Visual quality might drop seriously after tone mapping...");
		//}
		//
		//// image load store does not allow 3-channel formats, we have to use GL_RGBA
		//this->format = GL_RGBA;
		//this->i_format = GL_RGBA16F;
		//
		//if (levels == 0) 
		//{
		//	n_levels = 1 + static_cast<GLuint>(floor(std::log2(std::max(width, height))));
		//}
		//
		//// load the equirectangular image into a temporary 2D texture (base level, no mipmaps)
		//GLuint equirectangle = 0;
		//glCreateTextures(GL_TEXTURE_2D, 1, &equirectangle);
		//
		//if (equirectangle > 0) 
		//{
		//	auto image = Image(img_path, 3);
		//
		//	GLuint im_w = image.Width();
		//	GLuint im_h = image.Height();
		//	GLuint im_fmt = image.Format();
		//	GLenum im_ifmt = image.IFormat();
		//
		//	glTextureParameteri(equirectangle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTextureParameteri(equirectangle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTextureParameteri(equirectangle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTextureParameteri(equirectangle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//
		//	if (image.IsHDR()) 
		//	{
		//		glTextureStorage2D(equirectangle, 1, im_ifmt, im_w, im_h);
		//		glTextureSubImage2D(equirectangle, 0, 0, 0, im_w, im_h, im_fmt, GL_FLOAT, image.GetPixels<float>());
		//	}
		//	else 
		//	{
		//		glTextureStorage2D(equirectangle, 1, im_ifmt, im_w, im_h);
		//		glTextureSubImage2D(equirectangle, 0, 0, 0, im_w, im_h, im_fmt, GL_UNSIGNED_BYTE, image.GetPixels<uint8_t>());
		//	}
		//}
		//
		//// create this texture as an empty cubemap to hold the equirectangle
		//glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
		//glTextureStorage2D(id, n_levels, i_format, width, height);
		//
		//// project the 2D equirectangle onto the six faces of our cubemap using compute shader
		//CORE_INFO("Creating cubemap from {0}", img_path);
		//auto convert_shader = asset::CShader(utils::paths::shader + "core\\equirect2cube.glsl");
		//
		//if (convert_shader.Bind(); true) 
		//{
		//	glBindTextureUnit(0, equirectangle);
		//	glBindImageTexture(0, id, 0, GL_TRUE, 0, GL_WRITE_ONLY, i_format);
		//	glDispatchCompute(resolution / 32, resolution / 32, 6);  // six faces
		//	glMemoryBarrier(GL_ALL_BARRIER_BITS);  // sync wait
		//	glBindTextureUnit(0, 0);
		//	glBindImageTexture(0, 0, 0, GL_TRUE, 0, GL_WRITE_ONLY, i_format);
		//	convert_shader.Unbind();
		//}
		//
		//glDeleteTextures(1, &equirectangle);  // delete the temporary 2D equirectangle texture
		//
		//if (n_levels > 1) 
		//{
		//	glGenerateTextureMipmap(id);
		//}
		//
		//SetSampleState();
	}

	Texture::Texture(const std::string& directory, const std::string& extension, GLuint resolution, GLuint levels)
		: IAsset(), target(GL_TEXTURE_CUBE_MAP), width(resolution), height(resolution),
		depth(6), format(GL_RGBA), i_format(GL_RGBA16F), n_levels(levels)
	{
		// resolution must be a power of 2 in order to achieve high-fidelity visual effects
		//if (!utils::math::IsPowerOfTwo(resolution)) 
		//{
		//	CORE_ERROR("Attempting to build a cubemap whose resolution is not a power of 2...");
		//	return;
		//}
		//
		//// this ctor expects 6 HDR images for the 6 cubemap faces, named as follows
		//static const std::vector<std::string> faces{ "px", "nx", "py", "ny", "pz", "nz" };
		//
		//// the stb image library currently does not support ".exr" format ...
		//CORE_ASERT(extension == ".hdr", "Invalid file extension, expected HDR-format faces...");
		//
		//std::string test_face = directory + faces[0] + extension;
		//if (!std::filesystem::exists(std::filesystem::path(test_face))) 
		//{
		//	CORE_ERROR("Cannot find cubemap face {0} in the directory...", test_face);
		//	return;
		//}
		//
		//if (levels == 0) 
		//{
		//	n_levels = 1 + static_cast<GLuint>(floor(std::log2(std::max(width, height))));
		//}
		//
		//glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
		//glTextureStorage2D(id, n_levels, i_format, width, height);
		//
		//for (GLuint face = 0; face < 6; face++) 
		//{
		//	auto image = utils::Image(directory + faces[face] + extension, 3, true);
		//	glTextureSubImage3D(id, 0, 0, 0, face, width, height, 1, format, GL_FLOAT, image.GetPixels<float>());
		//}
		//
		//if (n_levels > 1) 
		//{
		//	glGenerateTextureMipmap(id);
		//}
		//
		//SetSampleState();
	}

	Texture::Texture(GLenum target, GLuint width, GLuint height, GLuint depth, GLenum i_format, GLuint levels)
		: IAsset(), target(target), width(width), height(height), depth(depth),
		n_levels(levels), format(0), i_format(i_format)
	{
		if (levels == 0)
		{
			n_levels = 1 + static_cast<GLuint>(floor(std::log2(std::max(width, height))));
		}

		// TODO: deduce format from i_format
		glCreateTextures(target, 1, &id);
		switch (target)
		{
		case GL_TEXTURE_1D:
		{
			glTextureStorage1D(id, n_levels, i_format, width);
			break;
		}

		case GL_TEXTURE_2D:
		case GL_TEXTURE_CUBE_MAP:
		{  // depth must = 6
			glTextureStorage2D(id, n_levels, i_format, width, height);
			break;
		}
		case GL_TEXTURE_2D_MULTISAMPLE:
		{
			glTextureStorage2DMultisample(id, 4, i_format, width, height, GL_TRUE);
			break;
		}
		case GL_TEXTURE_2D_ARRAY:
		case GL_TEXTURE_CUBE_MAP_ARRAY:
		{  // depth must = 6 * n_layers
			glTextureStorage3D(id, n_levels, i_format, width, height, depth);
			break;
		}
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
		{
			glTextureStorage3DMultisample(id, 4, i_format, width, height, depth, GL_TRUE);
			break;
		}
		default:
		{
			//throw NotImplementedError("Unsupported texture target...");
		}
		}

		SetSampleState();
	}

	Texture::~Texture()
	{
		if (id == 0) return;

		glDeleteTextures(1, &id);  // texture 0 (a fallback texture that is all black) is silently ignored

		for (int i = 0; i < n_texture_units; i++)
		{
			if (textures_binding_table[i] = id)
			{
				textures_binding_table[i] = 0;
			}
		}
	}

	void Texture::Bind(GLuint index)
	{
		if (id != textures_binding_table[index]) {
			textures_binding_table[index] = id;
			glBindTextureUnit(index, id);
		}

	}

	void Texture::Unbind(GLuint index)
	{
		if (textures_binding_table[index] != 0) {
			textures_binding_table[index] = 0;
			glBindTextureUnit(index, 0);
		}
	}

	void Texture::BindILS(GLuint level, GLuint index, GLenum access)
	{
		//CORE_ASERT(level < n_levels, "Mipmap level {0} is not valid in the texture...", level);
		glBindImageTexture(index, id, level, GL_TRUE, 0, access, i_format);
	}

	void Texture::UnbindILS(GLuint index)
	{
		glBindImageTexture(index, 0, 0, GL_TRUE, 0, GL_READ_ONLY, i_format);
	}

	void Texture::GenerateMipmap()
	{
		//CORE_ASERT(n_levels > 1, "Unable to generate mipmaps, levels must be greater than 1...");
		glGenerateTextureMipmap(id);
	}

	void Texture::Clear(GLuint level)
	{
		switch (i_format) {
		case GL_RG16F: case GL_RGB16F: case GL_RGBA16F:
		case GL_RG32F: case GL_RGB32F: case GL_RGBA32F: {
			glClearTexSubImage(id, level, 0, 0, 0, width, height, depth, format, GL_FLOAT, NULL);
			break;
		}
		default: {
			glClearTexSubImage(id, level, 0, 0, 0, width, height, depth, format, GL_UNSIGNED_BYTE, NULL);
			return;
		}
		}
	}

	void Texture::Invalidate(GLuint level)
	{
		glInvalidateTexSubImage(id, level, 0, 0, 0, width, height, depth);
	}

	void Texture::SetSampleState()
	{
		// for magnification, bilinear filtering is more than enough, for minification,
		// trilinear filtering is only necessary when we need to sample across mipmaps
		GLint mag_filter = GL_LINEAR;
		GLint min_filter = n_levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

		// anisotropic filtering requires OpenGL 4.6, where maximum anisotropy is implementation-defined
		static GLfloat anisotropy = -1.0f;
		if (anisotropy < 0)
		{
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
			anisotropy = std::clamp(anisotropy, 1.0f, 8.0f);  // limit anisotropy to 8
		}

		switch (target)
		{
		case GL_TEXTURE_1D:
		{
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case GL_TEXTURE_2D:
		case GL_TEXTURE_2D_ARRAY:
		{
			if (i_format == GL_R8)
			{
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else if (i_format == GL_RG16F)
			{  // 2D BRDF LUT, inverse LUT, fake BRDF maps, etc
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else if (i_format == GL_RGB16F)
			{  // 3D BRDF LUT, cloth DFG LUT, etc
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			else if (i_format == GL_RGBA16F)
			{  // 3D BRDF DFG LUT used as ILS (uniform image2D)
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			else if (i_format == GL_DEPTH_COMPONENT)
			{  // depth texture and shadow maps
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, min_filter);
				glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
				glTextureParameterf(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
			}
			break;
		}
		case GL_TEXTURE_2D_MULTISAMPLE:
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
		{
			// multisampled textures are not filtered at all, there's nothing we need to do here because
			// we'll never sample them, the hardware takes care of all the multisample operations for us
			// in fact, trying to set any of the sampler states will cause a `GL_INVALID_ENUM` error.
			return;
		}
		case GL_TEXTURE_CUBE_MAP:  // skybox and IBL maps
		case GL_TEXTURE_CUBE_MAP_ARRAY:
		{
			glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, min_filter);
			glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
			glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
			const float border[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTextureParameterfv(id, GL_TEXTURE_BORDER_COLOR, border);
			break;
		}
		default:
		{
			//throw NotImplementedError("Unsupported texture target...");
		}
		}
	}

	void Texture::PixelStorei(GLenum panel, GLint param)
	{
		glPixelStorei(panel, param);
	}

	void Texture::Copy(const Texture& fr, GLuint fr_level, const Texture& to, GLuint to_level)
	{
		//CORE_ASERT(fr_level < fr.n_levels, "Mipmap level {0} is not valid in texture {1}!", fr_level, fr.id);
		//CORE_ASERT(to_level < to.n_levels, "Mipmap level {0} is not valid in texture {1}!", to_level, to.id);

		GLuint fr_scale = static_cast<GLuint>(std::pow(2, fr_level));
		GLuint to_scale = static_cast<GLuint>(std::pow(2, to_level));

		GLuint fw = fr.width / fr_scale;
		GLuint fh = fr.height / fr_scale;
		GLuint fd = fr.depth;

		GLuint tw = to.width / to_scale;
		GLuint th = to.height / to_scale;
		GLuint td = to.depth;

		if (fw != tw || fh != th || fd != td)
		{
			CORE_ERROR("Unable to copy image data, mismatch width, height or depth!");
			return;
		}

		if (fr.target != to.target)
		{
			CORE_ERROR("Unable to copy image data, incompatible targets!");
			return;
		}

		glCopyImageSubData(fr.id, fr.target, fr_level, 0, 0, 0, to.id, to.target, to_level, 0, 0, 0, fw, fh, fd);




	}

	void Texture::SetData(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, const void * pixels)
	{
		//glPixelStorei
		
		GLenum _mat = GetFormatFromIFormat(i_format);
		GLenum _type = GetTypeFromIFormat(i_format);
		glTextureSubImage2D(id, level, xoffset, yoffset, width, height, _mat, _type, pixels);
	}

	void Texture::SetData(GLint level, GLint xoffset, GLsizei width, const void * pixels)
	{
		GLenum _mat = GetFormatFromIFormat(i_format);
		GLenum _type = GetTypeFromIFormat(i_format);
		glTextureSubImage1D(id, level, xoffset, width, _mat, _type, pixels);
	}

	void Texture::setTexParameteri(GLint min_filter, GLint mag_filter)
	{
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, min_filter);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, mag_filter);
	}

}

