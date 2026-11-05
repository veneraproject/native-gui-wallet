                                       
                                              
   
                       
   
                                                                                       
                                                            
   
                                                                                         
                                              
   
                                                                                         
                                                                                  
                                               
   
                                                                                       
                                                                                     
                               
   
                                                                                      
                                                                                          
                                                                                         
                                                                                       
                                                                               
                                                                                          
                                                                                    
                                                                                          
                                                                               

 
 
 
 
 
 
 
 
 

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include "wipeable_string.h"
#include "misc_language.h"
#include "int-util.h"
#include "mnemonics/electrum-words.h"
#include <boost/crc.hpp>

#include "chinese_simplified.h"
#include "english.h"
#include "dutch.h"
#include "french.h"
#include "italian.h"
#include "german.h"
#include "spanish.h"
#include "portuguese.h"
#include "japanese.h"
#include "russian.h"
#include "esperanto.h"
#include "lojban.h"
#include "english_old.h"
#include "language_base.h"
#include "singleton.h"

#undef MONERO_DEFAULT_LOG_CATEGORY
#define MONERO_DEFAULT_LOG_CATEGORY "mnemonic"

namespace crypto
{
  namespace ElectrumWords
  {
    std::vector<const Language::Base*> get_language_list();
  }
}

namespace
{
  uint32_t create_checksum_index(const std::vector<epee::wipeable_string> &word_list,
    const Language::Base *language);
  bool checksum_test(std::vector<epee::wipeable_string> seed, const Language::Base *language);

   
 
 
 
 
 
 
 
 
  bool find_seed_language(const std::vector<epee::wipeable_string> &seed,
    bool has_checksum, std::vector<uint32_t> &matched_indices, Language::Base **language)
  {
     
    std::vector<Language::Base*> language_instances({
      Language::Singleton<Language::Chinese_Simplified>::instance(),
      Language::Singleton<Language::English>::instance(),
      Language::Singleton<Language::Dutch>::instance(),
      Language::Singleton<Language::French>::instance(),
      Language::Singleton<Language::Spanish>::instance(),
      Language::Singleton<Language::German>::instance(),
      Language::Singleton<Language::Italian>::instance(),
      Language::Singleton<Language::Portuguese>::instance(),
      Language::Singleton<Language::Japanese>::instance(),
      Language::Singleton<Language::Russian>::instance(),
      Language::Singleton<Language::Esperanto>::instance(),
      Language::Singleton<Language::Lojban>::instance(),
      Language::Singleton<Language::EnglishOld>::instance()
    });
    Language::Base *fallback = NULL;

    std::vector<epee::wipeable_string>::const_iterator it2;
    matched_indices.reserve(seed.size());

     
    for (std::vector<Language::Base*>::iterator it1 = language_instances.begin();
      it1 != language_instances.end(); it1++)
    {
      const std::unordered_map<epee::wipeable_string, uint32_t, Language::WordHash, Language::WordEqual> &word_map = (*it1)->get_word_map();
      const std::unordered_map<epee::wipeable_string, uint32_t, Language::WordHash, Language::WordEqual> &trimmed_word_map = (*it1)->get_trimmed_word_map();
       
      bool full_match = true;

      epee::wipeable_string trimmed_word;
       
      for (it2 = seed.begin(); it2 != seed.end(); it2++)
      {
        if (has_checksum)
        {
          trimmed_word = Language::utf8prefix(*it2, (*it1)->get_unique_prefix_length());
           
          if (trimmed_word_map.count(trimmed_word) == 0)
          {
            full_match = false;
            break;
          }
          matched_indices.push_back(trimmed_word_map.at(trimmed_word));
        }
        else
        {
          if (word_map.count(*it2) == 0)
          {
            full_match = false;
            break;
          }
          matched_indices.push_back(word_map.at(*it2));
        }
      }
      if (full_match)
      {
         
         
        if (has_checksum)
          if (!checksum_test(seed, *it1))
          {
            fallback = *it1;
            full_match = false;
          }
      }
      if (full_match)
      {
        *language = *it1;
        MINFO("Full match for language " << (*language)->get_english_language_name());
        return true;
      }
       
      memwipe(matched_indices.data(), matched_indices.size() * sizeof(matched_indices[0]));
      matched_indices.clear();
    }

     
     
     
    if (fallback)
    {
      *language = fallback;
      MINFO("Fallback match for language " << (*language)->get_english_language_name());
      return true;
    }

    MINFO("No match found");
    memwipe(matched_indices.data(), matched_indices.size() * sizeof(matched_indices[0]));
    return false;
  }

   
 
 
 
 
 
  uint32_t create_checksum_index(const std::vector<epee::wipeable_string> &word_list,
    const Language::Base *language)
  {
    epee::wipeable_string trimmed_words = "", word;

    const auto &trimmed_word_map = language->get_trimmed_word_map();
    const uint32_t unique_prefix_length = language->get_unique_prefix_length();
    for (std::vector<epee::wipeable_string>::const_iterator it = word_list.begin(); it != word_list.end(); it++)
    {
      word = Language::utf8prefix(*it, unique_prefix_length);
      auto it2 = trimmed_word_map.find(word);
      if (it2 == trimmed_word_map.end())
        throw std::runtime_error("Word \"" + std::string(word.data(), word.size()) + "\" not found in trimmed word map in " + language->get_english_language_name());
      trimmed_words += it2->first;
    }
    boost::crc_32_type result;
    result.process_bytes(trimmed_words.data(), trimmed_words.length());
    return result.checksum() % word_list.size();
  }

   
 
 
 
 
 
  bool checksum_test(std::vector<epee::wipeable_string> seed, const Language::Base *language)
  {
    if (seed.empty())
      return false;
     
    epee::wipeable_string last_word = seed.back();
    seed.pop_back();

    const uint32_t unique_prefix_length = language->get_unique_prefix_length();

    auto idx = create_checksum_index(seed, language);
    epee::wipeable_string checksum = seed[idx];

    epee::wipeable_string trimmed_checksum = checksum.length() > unique_prefix_length ? Language::utf8prefix(checksum, unique_prefix_length) :
      checksum;
    epee::wipeable_string trimmed_last_word = last_word.length() > unique_prefix_length ? Language::utf8prefix(last_word, unique_prefix_length) :
      last_word;
    bool ret = Language::WordEqual()(trimmed_checksum, trimmed_last_word);
    MINFO("Checksum is " << (ret ? "valid" : "invalid"));
    return ret;
  }
}

 
 
 
 
 
namespace crypto
{
   
 
 
 
 
  namespace ElectrumWords
  {
     
 
 
 
 
 
 
 
 
    bool words_to_bytes(const epee::wipeable_string &words, epee::wipeable_string& dst, size_t len, bool duplicate,
      std::string &language_name)
    {
      std::vector<epee::wipeable_string> seed;

      words.split(seed);

      if (len % 4)
      {
        MERROR("Invalid seed: not a multiple of 4");
        return false;
      }

      bool has_checksum = true;
      if (len)
      {
         
        const size_t expected = len * 8 * 3 / 32;
        if (seed.size() != expected/2 && seed.size() != expected &&
          seed.size() != expected + 1)
        {
          MERROR("Invalid seed: unexpected number of words");
          return false;
        }

         
        has_checksum = seed.size() == (expected + 1);
      }

      std::vector<uint32_t> matched_indices;
      auto wiper = epee::misc_utils::create_scope_leave_handler([&](){memwipe(matched_indices.data(), matched_indices.size() * sizeof(matched_indices[0]));});
      Language::Base *language;
      if (!find_seed_language(seed, has_checksum, matched_indices, &language))
      {
        MERROR("Invalid seed: language not found");
        return false;
      }
      language_name = language->get_language_name();
      uint32_t word_list_length = language->get_word_list().size();

      if (has_checksum)
      {
        if (!checksum_test(seed, language))
        {
           
          MERROR("Invalid seed: invalid checksum");
          return false;
        }
        seed.pop_back();
      }

      for (unsigned int i=0; i < seed.size() / 3; i++)
      {
        uint32_t w[4];
        w[1] = matched_indices[i*3];
        w[2] = matched_indices[i*3 + 1];
        w[3] = matched_indices[i*3 + 2];

        w[0]= w[1] + word_list_length * (((word_list_length - w[1]) + w[2]) % word_list_length) +
          word_list_length * word_list_length * (((word_list_length - w[2]) + w[3]) % word_list_length);

        if (!(w[0]% word_list_length == w[1]))
        {
          memwipe(w, sizeof(w));
          MERROR("Invalid seed: mumble mumble");
          return false;
        }

        w[0] = SWAP32LE(w[0]);
        dst.append((const char*)&w[0], 4);   
        memwipe(w, sizeof(w));
      }

      if (len > 0 && duplicate)
      {
        const size_t expected = len * 3 / 32;
        if (seed.size() == expected/2)
        {
          dst.append(dst.data(), dst.size());  
        }
      }

      return true;
    }

     
 
 
 
 
 
 
    bool words_to_bytes(const epee::wipeable_string &words, crypto::secret_key& dst,
      std::string &language_name)
    {
      epee::wipeable_string s;
      if (!words_to_bytes(words, s, sizeof(dst), true, language_name))
      {
        MERROR("Invalid seed: failed to convert words to bytes");
        return false;
      }
      if (s.size() != sizeof(dst))
      {
        MERROR("Invalid seed: wrong output size");
        return false;
      }
      dst = *(const crypto::secret_key*)s.data();
      return true;
    }

     
 
 
 
 
 
 
    bool bytes_to_words(const char *src, size_t len, epee::wipeable_string& words,
      const std::string &language_name)
    {

      if (len % 4 != 0 || len == 0) return false;

      const Language::Base *language = NULL;
      const std::vector<const Language::Base*> language_list = crypto::ElectrumWords::get_language_list();
      for (const Language::Base *l: language_list)
      {
        if (language_name == l->get_language_name() || language_name == l->get_english_language_name())
          language = l;
      }
      if (!language)
      {
        return false;
      }
      const std::vector<std::string> &word_list = language->get_word_list();
       
      std::vector<epee::wipeable_string> words_store;

      uint32_t word_list_length = word_list.size();
       
      for (unsigned int i=0; i < len/4; i++, words.push_back(' '))
      {
        uint32_t w[4];

        w[0] = SWAP32LE(*(const uint32_t*)(src + (i * 4)));

        w[1] = w[0] % word_list_length;
        w[2] = ((w[0] / word_list_length) + w[1]) % word_list_length;
        w[3] = (((w[0] / word_list_length) / word_list_length) + w[2]) % word_list_length;

        words += word_list[w[1]];
        words += ' ';
        words += word_list[w[2]];
        words += ' ';
        words += word_list[w[3]];

        words_store.push_back(word_list[w[1]]);
        words_store.push_back(word_list[w[2]]);
        words_store.push_back(word_list[w[3]]);

        memwipe(w, sizeof(w));
      }

      words += words_store[create_checksum_index(words_store, language)];
      return true;
    }

    bool bytes_to_words(const crypto::secret_key& src, epee::wipeable_string& words,
      const std::string &language_name)
    {
      return bytes_to_words(src.data, sizeof(src), words, language_name);
    }

    std::vector<const Language::Base*> get_language_list()
    {
      static const std::vector<const Language::Base*> language_instances({
        Language::Singleton<Language::German>::instance(),
        Language::Singleton<Language::English>::instance(),
        Language::Singleton<Language::Spanish>::instance(),
        Language::Singleton<Language::French>::instance(),
        Language::Singleton<Language::Italian>::instance(),
        Language::Singleton<Language::Dutch>::instance(),
        Language::Singleton<Language::Portuguese>::instance(),
        Language::Singleton<Language::Russian>::instance(),
        Language::Singleton<Language::Japanese>::instance(),
        Language::Singleton<Language::Chinese_Simplified>::instance(),
        Language::Singleton<Language::Esperanto>::instance(),
        Language::Singleton<Language::Lojban>::instance()
      });
      return language_instances;
    }

     
 
 
 
    void get_language_list(std::vector<std::string> &languages, bool english)
    {
      const std::vector<const Language::Base*> language_instances = get_language_list();
      for (std::vector<const Language::Base*>::const_iterator it = language_instances.begin();
        it != language_instances.end(); it++)
      {
        languages.push_back(english ? (*it)->get_english_language_name() : (*it)->get_language_name());
      }
    }

     
 
 
 
 
    bool get_is_old_style_seed(const epee::wipeable_string &seed)
    {
      std::vector<epee::wipeable_string> word_list;
      seed.split(word_list);
      return word_list.size() != (seed_length + 1);
    }

    std::string get_english_name_for(const std::string &name)
    {
      const std::vector<const Language::Base*> language_instances = get_language_list();
      for (std::vector<const Language::Base*>::const_iterator it = language_instances.begin();
        it != language_instances.end(); it++)
      {
        if ((*it)->get_language_name() == name)
          return (*it)->get_english_language_name();
      }
      return "<language not found>";
    }

    bool is_valid_language(const std::string &language)
    {
      const std::vector<const Language::Base*> language_instances = get_language_list();
      for (std::vector<const Language::Base*>::const_iterator it = language_instances.begin(); it != language_instances.end(); it++)
        if ((*it)->get_english_language_name() == language || (*it)->get_language_name() == language)
          return true;
      return false;
    }
  }

}
