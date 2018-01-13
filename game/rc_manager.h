#ifndef RC_MANAGER_H
#define RC_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/InputStream.hpp>

class ResourceStream : public sf::InputStream
{
private:
  std::vector<const char *> m_resource; // Tableau des tableaux
  std::vector<size_t> m_resource_sizes; // Tableau des tailles de tableaux
  sf::Int64 m_cur_byte; // Indice de l'octet du tableau actuel (pas encore lu)
  sf::Int64 m_total_size; // Taille totale de la ressource

public:
  ResourceStream(const char *resource[], size_t resourceSizes);
  ResourceStream(const ResourceStream &) = delete;
  ResourceStream &operator =(const ResourceStream &) = delete;
  ResourceStream(ResourceStream &&) = default;
  virtual ~ResourceStream() = default;

  virtual sf::Int64 read(void* data, sf::Int64 size) override;
  virtual sf::Int64 seek(sf::Int64 position) override;
  virtual sf::Int64 tell() override;
  virtual sf::Int64 getSize() override;
};

class ResourceManager
{
private:
  static std::map<std::string, std::shared_ptr<sf::Texture>> m_cache;
  static std::map<std::string, std::shared_ptr<sf::Font>> m_font_cache;
  static std::map<std::string, std::shared_ptr<ResourceStream>> m_stream_cache;
  static ResourceStream fetchStream (const std::string &name);
public:
  static std::shared_ptr<sf::Texture> getTexture(const std::string &name);
  static std::shared_ptr<sf::Font> getFont(const std::string &name);
};

#endif /* !RC_MANAGER_H */
