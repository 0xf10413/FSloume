#ifndef RC_MANAGER_H
#define RC_MANAGER_H

#include <vector>
#include <string>
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
public:
  static ResourceStream fetchMe (const std::string &name);
};

#endif /* !RC_MANAGER_H */
