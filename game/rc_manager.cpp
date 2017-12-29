#include "rc_manager.h"
#include "rc.h"
#include "utils.h"
#include "config.h"
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

std::weak_ptr<sf::Texture> ResourceManager::getTexture(const std::string &name)
{
  auto iter = m_cache.find(name);
  if (iter != m_cache.end())
    return iter->second;

  /* Pas encore en cache */
  sf::Image buffer;
  if (name == "rSlime" || name == "bSlime")
  {
    bool m_alignLeft = name == "bSlime";
    // Dessin de la forme hémi-circulaire caractéristique
    // Calcul : tout pixel distant du point au milieu (horizontalement)
    // en bas (verticalement) de plus de width/2 est transparent.
    buffer.create (CG::SLIME_WIDTH, CG::SLIME_HEIGHT, m_alignLeft ? sf::Color::Blue : sf::Color::Red);
    for (int i = 0; i < CG::SLIME_WIDTH; i++)
      for (int j=0; j<CG::SLIME_HEIGHT; j++)
        if ((i - CG::SLIME_WIDTH/2) * (i - CG::SLIME_WIDTH/2) +
            (j - CG::SLIME_HEIGHT) * (j-CG::SLIME_HEIGHT) >=
            (CG::SLIME_WIDTH*CG::SLIME_WIDTH/4))
          buffer.setPixel (i, j, sf::Color (0, 0, 0, 0));
        else if ((i - CG::SLIME_WIDTH/2) * (i - CG::SLIME_WIDTH/2) +
            (j - CG::SLIME_HEIGHT) * (j-CG::SLIME_HEIGHT) >=
            ((CG::SLIME_WIDTH-5)*(CG::SLIME_WIDTH-5)/4))
          buffer.setPixel (i, j, sf::Color::Black);
    if (m_alignLeft)
    {
      // Dessin du globe oculaire (coordonnées locales du centre : (3*w/4, h/2))
      for (int i = 0; i < CG::SLIME_WIDTH; i++)
        for (int j=0; j < CG::SLIME_HEIGHT; j++)
          if ((i - 3*CG::SLIME_WIDTH/4) * (i - 3*CG::SLIME_WIDTH/4) +
              (j - CG::SLIME_HEIGHT/2) * (j-CG::SLIME_HEIGHT/2) <=
              CG::EYE_RADIUS_SQUARED)
            buffer.setPixel (i, j, sf::Color::White);
    }
    else
      // Dessin du globe oculaire (coordonnées locales du centre : (w/4, h/2))
      for (int i = 0; i < CG::SLIME_WIDTH; i++)
        for (int j=0; j < CG::SLIME_HEIGHT; j++)
          if ((i - CG::SLIME_WIDTH/4) * (i - CG::SLIME_WIDTH/4) +
              (j - CG::SLIME_HEIGHT/2) * (j-CG::SLIME_HEIGHT/2) <=
              CG::EYE_RADIUS_SQUARED)
            buffer.setPixel (i, j, sf::Color::White);

  }
  else if (name == "background")
  {
    buffer.create(CG::WIDTH, CG::HEIGHT, sf::Color::White);
    for (int i = 0; i < CG::WIDTH; ++i)
      for (int j = 0; j < CG::HEIGHT; ++j)
        buffer.setPixel(i, j, sf::Color(0, j*100/CG::HEIGHT, j*255/CG::HEIGHT));
  }
  else if (name == "ball")
  {
    buffer.create(CG::BALL_RADIUS*2, CG::BALL_RADIUS*2, sf::Color::Red);
    makeADisk(buffer, 2);
  }
  else if (name == "cloud")
  {
    buffer.create(CG::CLOUD_WIDTH, CG::CLOUD_HEIGHT, sf::Color::White);
    ResourceStream stream = fetchStream("rc_cloud_png");
    buffer.loadFromStream(stream);
  }
  else if (name == "danger_pt")
  {
    buffer.create(CG::DANGERPT_WIDTH, CG::DANGERPT_WIDTH, CG::DANGERPT_COLOR);
    for (int i = CG::DANGERPT_WIDTH/10; i < 7*CG::DANGERPT_WIDTH/10; ++i)
      for (int j = CG::DANGERPT_WIDTH/2-1; j < CG::DANGERPT_WIDTH/2+2; ++j)
        buffer.setPixel(j, i, sf::Color::Black);
    for (int i = CG::DANGERPT_WIDTH/2-2; i < CG::DANGERPT_WIDTH/2+3; ++i)
      for (int j = 9*CG::DANGERPT_WIDTH/10-2; j < 9*CG::DANGERPT_WIDTH/10+2; ++j)
        buffer.setPixel(i, j, sf::Color::Black);
    for (int i = 0; i < CG::DANGERPT_WIDTH; ++i)
      for (int j = 0; j < CG::DANGERPT_WIDTH; ++j)
      {
        if (-2*i + CG::DANGERPT_WIDTH > j ||
            2*i - CG::DANGERPT_WIDTH > j)
          buffer.setPixel(i,j,sf::Color(0,0,0,0));
      }
  }
  else if (name == "rEye" || name == "bEye")
  {
    bool alignLeft = name == "bEye";
    buffer.create (
        CG::PUPIL_RADIUS*2,
        CG::PUPIL_RADIUS*2,
        alignLeft ? sf::Color::Cyan : sf::Color::Magenta );
    makeADisk(buffer, 1);
  }
  else if (name == "net")
  {
    buffer.create (CG::NET_WIDTH, CG::NET_HEIGHT, sf::Color::Yellow);
  }
  else if (name == "target")
  {
    buffer.create(CG::TARGET_WIDTH, CG::TARGET_HEIGHT, sf::Color(255, 255, 127, 0));
    for (unsigned int i = 0; i < CG::TARGET_WIDTH; ++i)
      buffer.setPixel(i, CG::TARGET_HEIGHT/2, sf::Color(255, 255, 127, 255));
    for (unsigned int j = 0; j < CG::TARGET_HEIGHT; ++j)
      buffer.setPixel(CG::TARGET_WIDTH/2, j, sf::Color(255, 255, 127, 255));
  }
  else
    throw std::runtime_error("Texture not found !");

  m_cache[name] = std::make_shared<sf::Texture>();
  m_cache[name]->loadFromImage(buffer);
  return m_cache[name];
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string &name)
{
  auto iter = m_font_cache.find(name);
  if (iter != m_font_cache.end())
    return iter->second;
  m_stream_cache[name] = std::make_shared<ResourceStream>(fetchStream("rc_" + name + "_ttf"));
  sf::Font font;
  font.loadFromStream(*m_stream_cache[name]);
  return m_font_cache[name] = std::make_shared<sf::Font> (font);
}

std::map<std::string, std::shared_ptr<sf::Texture>> ResourceManager::m_cache;
std::map<std::string, std::shared_ptr<sf::Font>> ResourceManager::m_font_cache;
std::map<std::string, std::shared_ptr<ResourceStream>> ResourceManager::m_stream_cache;

ResourceStream ResourceManager::fetchStream (const std::string &name)
{
#include "../game/rc_manager.inc"
  throw std::runtime_error("Resource not found !");
}
