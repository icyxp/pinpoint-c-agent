////////////////////////////////////////////////////////////////////////////////
// Copyright 2018 NAVER Corp.
// 
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License.  You may obtain a copy
// of the License at
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations under
// the License.
////////////////////////////////////////////////////////////////////////////////
#include "php_config.h"
#include "php_plugin.h"
#include "plugins/request_plugin.h"
#include "plugins/internel_functions_plugin.h"


using namespace Pinpoint::Plugin;
using namespace Pinpoint::log;

PhpPluginManager* PhpPluginManager::instance(NULL);

PhpPluginManager::PhpPluginManager()
{

}

PhpPluginManager* PhpPluginManager::getInstance()
{
    if (instance == NULL)
    {
        try
        {
            instance = new PhpPluginManager;
        }
        catch (std::bad_alloc&)
        {
            LOGE("new PhpPluginManager failed");
            return NULL;
        }
    }

    return instance;
}

int32_t PhpPluginManager::registerPlugins()
{
    this->plugins.reserve(PhpPluginManager::DEFAULT_PLUGIN_NUM);

    PluginPtr pluginPtr;
    // 1, request plugin
    try
    {
        pluginPtr.reset(new PhpRequestPlugin);
    }
    catch (std::bad_alloc&)
    {
        LOGE("add PhpRequestPlugin failed");
        return Pinpoint::FAILED;
    }
    this->plugins.push_back(pluginPtr);

#if defined(DEBUG) || defined(TEST_SIMULATE)
    // 2, internal functions plugin
    try
    {
        pluginPtr.reset(new PhpInternalFunctionsPlugin);
    }
    catch (std::bad_alloc&)
    {
        LOGE("add PhpInternalFunctionsPlugin failed");
        return Pinpoint::FAILED;
    }
    this->plugins.push_back(pluginPtr);
#endif

    return Pinpoint::SUCCESS;
}

Pinpoint::Plugin::PluginPtrVector& PhpPluginManager::getAllPlugins()
{
    return this->plugins;
}
