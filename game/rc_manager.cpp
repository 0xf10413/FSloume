#include "rc_manager.h"
#include "rc.h"
#include <stdexcept>
#include <iostream>

ResourceStream::ResourceStream(const char *resource[],
    size_t resource_size) :
  m_resource(),
  m_resource_sizes(),
  m_cur_byte(0),
  m_total_size((sf::Int64)resource_size)
{
  for (sf::Int64 i = 0; resource[i] != nullptr; ++i)
  {
    m_resource.push_back(resource[i]);
    m_resource_sizes.push_back(RC_MAX_BYTES_PER_ARRAY);
  }
  m_resource_sizes.back() = resource_size % RC_MAX_BYTES_PER_ARRAY;
}

sf::Int64 ResourceStream::read(void* data, sf::Int64 size)
{
  sf::Int64 to = std::min(size, m_total_size - m_cur_byte);
  for (sf::Int64 i = 0; i < to; ++i, ++m_cur_byte)
    static_cast<char *>(data)[i] =
      m_resource[(size_t)(m_cur_byte / RC_MAX_BYTES_PER_ARRAY)]
      [(size_t)(m_cur_byte % RC_MAX_BYTES_PER_ARRAY)];
  return to;
}

sf::Int64 ResourceStream::seek(sf::Int64 position)
{
  return m_cur_byte = std::min(position, m_total_size - 1);
}

sf::Int64 ResourceStream::tell()
{
  return m_cur_byte;
}

sf::Int64 ResourceStream::getSize()
{
  return m_total_size;
}

ResourceStream ResourceManager::fetchMe (const std::string &name)
{
  if (name == "rc_8bitoperator_ttf")
    return ResourceStream(rc_8bitoperator_ttf, rc_8bitoperator_ttf_size);
  throw std::runtime_error("Resource not found !");
}
