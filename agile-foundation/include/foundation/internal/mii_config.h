/*
 * xacroparser.h
 *
 *  Created on: Dec 1, 2017
 *      Author: bibei
 */

#ifndef INTERNAL_MII_CONFIG_H_
#define INTERNAL_MII_CONFIG_H_

// #define LINK_CFG_READER

#include "../utf.h"
#include <functional>
#include <tinyxml.h>

namespace internal {

#define MACRO_CREATE          internal::MiiConfig::create_instance();
#define MACRO_DESTROY         internal::MiiConfig::destroy_instance();
#define MACRO_INST            internal::MiiConfig::instance()
#define MACRO_PARSE(...)      internal::MiiConfig::instance()->parse(__VA_ARGS__);

class MiiConfig {
  SINGLETON_DECLARE(MiiConfig)

public:
  void parse(const std::string& _fn, TiXmlDocument** _out_xml = nullptr,
      const std::string& _out_fn = "", bool print = false);

  static void add_path(const std::string& _p);

protected:
  void parse_helper(TiXmlDocument*, TiXmlDocument* root, const std::string& _out = "", bool print = false);

  void init_path     (TiXmlDocument*, TiXmlElement*);
  void init_include  (TiXmlDocument*, TiXmlElement*);
  void init_property (TiXmlDocument*, TiXmlElement*);
  void init_protoType(TiXmlDocument*, TiXmlElement*);

  /*!
   * @brief The callback is type as void(const std::string&, void*), The first
   *        parameter is the label with given tag( or attribute), The second
   *        parameter is the TiXmlElement*( or char* for the attribute content).
   */
  void regTagCb(TiXmlDocument*, const std::string& _tag,
      std::function<void(TiXmlElement*)>&);

protected:
  TiXmlDocument*                             root_file_;
  static std::map<std::string, std::string>        s_property_map_;
  static std::map<std::string, class ProtoInfo*> s_prototype_map_;

  static std::vector<std::string>                s_cfg_paths_;
};

} /* namespace internal */

#endif /* INTERNAL_MII_CONFIG_H_ */
