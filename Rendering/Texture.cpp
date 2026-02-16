#include"Texture.h"

Texture::Texture()
{
	this->gTex = nullptr;
}
Texture::Texture(std::shared_ptr<GTexture> tex,
	std::shared_ptr<ShaderResourceView> srv)
{
	this->gSrv = srv;
	this->gTex = tex;
}
GTexture* Texture::GetTextureGraphicsResource()
{
	return gTex.get();
}
void Texture::SetTextureGraphicsResource(std::shared_ptr<GTexture> tex)
{
	gTex = tex;
}