#ifndef LABS_4_SEM_LOGGER_BUILDER_CONCRETE_H
#define LABS_4_SEM_LOGGER_BUILDER_CONCRETE_H
#include <map>
#include <iostream>
#include "logger_builder.h"
#include "logger_concrete.h"
#include <nlohmann/json.hpp>

class logger_builder_concrete final: public logger_builder{
private:
    std::map<std::string, logger::severity> _construct_info;
public:
    logger_builder* add_stream(std::string const & path, logger::severity severety){
        _construct_info[path]=severety;
        return this;
    }
    logger* construct() const override {
        return new logger_concrete(_construct_info);

    }
    logger_builder* add_in_file(const std::string& way) override {
        std::string file_way, type;
        std::map <std::string, logger::severity> maps;
        maps={
                {"trace", logger::severity::trace},
                {"debug", logger::severity::debug},
                {"information", logger::severity::information},
                {"warning", logger::severity::warning},
                {"error", logger::severity::error},
                {"critical", logger::severity::critical}
        };
        std::ifstream file(way);
        while(file>>file_way>>type){
            add_stream(file_way, maps[type]);
        }
        file.close();
        return this;

    }

    logger_builder* add_json_configuration(const std::string& way) override {
        std::ifstream config(way);
        if (!config.is_open())
        {
            throw std::logic_error("Problems with opening file");
        }

        nlohmann::json jsonConfig = nlohmann::json::parse(config);
        auto loggerJsonStreamsConfig = jsonConfig.at("streams");
        std::cout << loggerJsonStreamsConfig << std::endl << std::endl;
        std::map <std::string, logger::severity> maps;
        maps={
                {"trace", logger::severity::trace},
                {"debug", logger::severity::debug},
                {"information", logger::severity::information},
                {"warning", logger::severity::warning},
                {"error", logger::severity::error},
                {"critical", logger::severity::critical}
        };

        for (auto loggerJsonStreamConfig : loggerJsonStreamsConfig)
        {
            //std::cout << loggerJsonStreamConfig << std::endl;

            //add_stream(loggerJsonStreamConfig.at("path"), maps[loggerJsonStreamConfig.at("severity")]);
            std::cout << "Path to file to log: " << loggerJsonStreamConfig.at("path") << ", file severity: " << loggerJsonStreamConfig.at("severity") << std::endl;
        }

        config.close();

        //builder->from_json("file.path", "loggers[0]"); // jpath




        return this;
    }
};
#endif //LABS_4_SEM_LOGGER_BUILDER_CONCRETE_H
