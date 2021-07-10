#pragma once
#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <variant>

#include <nlohmann/json_fwd.hpp>

namespace appmap {
    using cor_value = std::variant<std::string, uint64_t, int64_t, bool, nullptr_t>;

    struct event {
        virtual bool operator==(const event &other) const noexcept {
            return typeid(*this) == typeid(other);
        }
        virtual ~event() {}
        virtual operator nlohmann::json() const = 0;
    };

    struct call_event: event {};

    struct function_call_event: call_event {
        size_t function;
        std::vector<cor_value> arguments;

        function_call_event(size_t fun, std::vector<cor_value> &&args = {}):
            function(fun), arguments(std::move(args)) {}

        bool operator==(const event &other) const noexcept override {
            if (!call_event::operator==(other))
                return false;

            return function == static_cast<const function_call_event &>(other).function;
        }

        operator nlohmann::json() const override;
    };

    struct http_request_event: call_event {
        std::string meth;
        std::string path;

        http_request_event(std::string method, std::string path_info):
            meth(std::move(method)), path(std::move(path_info)) {}

        operator nlohmann::json() const override;
    };

    struct return_event: event {
        const call_event *call;
        std::optional<cor_value> value = std::nullopt;

        return_event(const call_event *call_ev, std::optional<cor_value> return_value = std::nullopt):
            call(call_ev), value(return_value) {}

        bool operator==(const event &other) const noexcept override {
            if (!event::operator==(other))
                return false;

            const auto &other_ret = static_cast<const return_event &>(other);
            return call == other_ret.call && value == other_ret.value;
        }

        operator nlohmann::json() const override;
    };

    struct http_response_event: return_event {
        int status;

        http_response_event(const call_event *call_ev, int status_code):
            return_event(call_ev), status(status_code) {}

        operator nlohmann::json() const override;
    };
}
