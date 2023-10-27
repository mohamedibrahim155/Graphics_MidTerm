//#include "Textures.h"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "GLFW/stb_image.h"
//
//
//
//std::vector<Texture> Texture::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
//{
//    std::vector<Texture> textures;
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//
//      
//
//        bool skip = false;
//        for (unsigned int j = 0; j < textures_loaded.size(); j++)
//        {
//            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//            {
//                textures.push_back(textures_loaded[j]);
//                skip = true;
//                break;
//            }
//        }
//        if (!skip)
//        {
//            Texture texture;
//            texture.id = TextureLoading(str.C_Str(), this->directory);
//            texture.type = typeName;
//            std::cout << "Texture Loaded: " << texture.type << std::endl;
//            texture.path = str.C_Str();
//            textures.push_back(texture);
//            textures_loaded.push_back(texture);
//        }
//    }
//
//    return textures;
//}
//
//
//unsigned int Texture::TextureLoading(const char* path, const std::string& directory)
//{
//    std::string filename = std::string(path);
//    filename = directory + '/' + filename;
//
//    
//    glGenTextures(1, &id);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//    if (data)
//    {
//
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, id);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        std::cout << "           " << std::endl;
//        std::cout << "TEXTURES LOADING......... :" << std::endl;
//        std::cout << path << directory << std::endl;
//
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//
//    return id;
//}
