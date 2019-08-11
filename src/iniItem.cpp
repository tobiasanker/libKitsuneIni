/**
 *  @file    iniItem.cpp
 *
 *  @author  Tobias Anker
 *  Contact: tobias.anker@kitsunemimi.moe
 *
 *  MIT License
 */

#include "iniItem.hpp"

#include <data_structure/dataItems.hpp>
#include <ini_parsing/iniParserInterface.hpp>

using Kitsune::Common::DataItem;
using Kitsune::Common::DataArray;
using Kitsune::Common::DataValue;
using Kitsune::Common::DataObject;

namespace Kitsune
{
namespace Ini
{

/**
 * constructor
 */
IniItem::IniItem() {}

/**
 * destructor
 */
IniItem::~IniItem()
{
    if(m_content != nullptr) {
        delete m_content;
    }
}

/**
 * @brief IniObject::parse
 * @param content
 * @return
 */
pair<std::string, bool>
IniItem::parse(const std::string &content,
               const bool traceParsing)
{
    pair<std::string, bool> result;
    IniParserInterface parser(traceParsing);

    // parse jinja2-template into a json-tree
    result.second = parser.parse(content);

    // process a failure
    if(result.second == false) {
        result.first = parser.getErrorMessage();
        return result;
    }

    m_content = parser.getOutput();

    return result;
}

/**
 * @brief IniObject::get
 * @param group
 * @param item
 * @return
 */
Common::DataItem*
IniItem::get(const std::string &group,
             const std::string &item)
{
    DataItem* groupContent = m_content->get(group);
    if(groupContent == nullptr) {
        return nullptr;
    }
    return groupContent->get(item);
}

/**
 * @brief IniItem::set
 * @return
 */
bool
IniItem::set(const std::string &group,
             const std::string &item,
             const std::string value,
             const bool force)
{
    return set(group, item, new DataValue(value), force);
}

/**
 * @brief IniItem::set
 * @return
 */
bool
IniItem::set(const std::string &group,
             const std::string &item,
             const int value,
             const bool force)
{
    return set(group, item, new DataValue(value), force);
}

/**
 * @brief IniItem::set
 * @return
 */
bool
IniItem::set(const std::string &group,
             const std::string &item,
             const float value,
             const bool force)
{
    return set(group, item, new DataValue(value), force);
}

/**
 * @brief IniItem::set
 * @return
 */
bool
IniItem::set(const std::string &group,
             const std::string &item,
             const std::vector<std::string> value,
             const bool force)
{
    DataArray* array = new DataArray();
    for(uint64_t i = 0; i < value.size(); i++)
    {
        array->append(new DataValue(value.at(i)));
    }

    return set(group, item, array, force);
}

/**
 * @brief IniItem::set
 * @param group
 * @param item
 * @param value
 * @param force
 * @return
 */
bool
IniItem::set(const std::string &group,
             const std::string &item,
             DataItem *value,
             const bool force)
{
    DataItem* groupContent = m_content->get(group);

    // if group doesn't exist, create the group with the new content
    if(groupContent == nullptr)
    {
        groupContent = new DataObject();
        groupContent->toObject()->insert(item, value);
        m_content->toObject()->insert(group, groupContent);
        return true;
    }

    DataItem* groupItem = groupContent->get(item);

    // item doesn't exist or should be overrided by force
    if(groupItem == nullptr
            || force)
    {
        groupContent->toObject()->insert(item, value, force);
        return true;
    }

    // I delete the value here already to avoid too much redundant code in the other set-methods
    // even this is not so nice. Thats why this is only a private methods with should be used only
    // interally
    delete value;

    return false;
}

/**
 * @brief IniObject::print
 * @return
 */
std::string IniItem::print()
{
    std::string output = "";

    // iterate over all groups
    std::map<std::string, DataItem*> completeContent = m_content->toObject()->m_objects;
    std::map<std::string, DataItem*>::iterator itGroup;
    for(itGroup = completeContent.begin();
        itGroup != completeContent.end();
        itGroup++)
    {
        // print group-header
        output.append("[");
        output.append(itGroup->first);
        output.append("]\n");

        // iterate over group-content
        std::map<std::string, DataItem*> groupContent = itGroup->second->toObject()->m_objects;
        map<string, DataItem*>::iterator itItem;
        for(itItem = groupContent.begin();
            itItem != groupContent.end();
            itItem++)
        {
            // print line of group-content
            output.append(itItem->first);
            output.append(" = ");

            if(itItem->second->getType() == DataItem::ARRAY_TYPE)
            {
                // print arrays
                std::vector<DataItem*> array = itItem->second->toArray()->m_array;
                for(uint64_t i = 0; i < array.size(); i++)
                {
                    if(i != 0) {
                        output.append(", ");
                    }
                    output.append(array.at(i)->print());
                }
            }
            else
            {
                // print simple items
                itItem->second->print(&output);
            }

            output.append("\n");
        }

        output.append("\n");
    }

    return output;
}

/**
 * @brief IniItem::setContent
 * @param item
 */
void
IniItem::setContent(DataItem *item)
{
    if(m_content != nullptr) {
        delete m_content;
    }
    m_content = item;
}

}  // namespace Ini
}  // namespace Kitsune
