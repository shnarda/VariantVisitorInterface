#pragma once
#include <variant>
#include <utility>
#include "map.h"
#include <optional>


#define VARVIS_ADD_VISITOR_FUNCTION(function_name) \
template <typename ...Args>\
auto function_name(Args&&... args) const\
{\
	const auto functor = [&](const auto& obj) {return obj.function_name(std::forward<Args>(args)...); };\
	return std::visit(functor, m_variant);\
};\
template <typename ...Args>\
auto function_name(Args&&... args)\
{\
	const auto functor = [&](auto& obj) {return obj.function_name(std::forward<Args>(args)...); };\
	return std::visit(functor, m_variant);\
};

#define VARVIS_ADD_VISITOR_FUNCTIONS(...) MAP(VARVIS_ADD_VISITOR_FUNCTION, __VA_ARGS__)


#define VARVIS_FILL_CLASS_TEMPLATE(FIRST_TYPE, ...) template<typename FIRST_TYPE, typename...args>
#define VARVIS_FILL_CLASS_TEMPLATE_UNPACK(VARIANT_TYPES) VARVIS_FILL_CLASS_TEMPLATE VARIANT_TYPES

#define VARVIS_FILL_VARIANT_TEMPLATE(FIRST_TYPE, ...) FIRST_TYPE, __VA_ARGS__
#define VARVIS_FILL_VARIANT_TEMPLATE_UNPACK(VARIANT_TYPES) VARVIS_FILL_VARIANT_TEMPLATE VARIANT_TYPES



#define VARVIS_DEFINE_VARIANT_VISITOR(CLASS_NAME, VARIANT_TYPES, ...) \
class CLASS_NAME {\
public:\
	template<class ...Args>\
	constexpr CLASS_NAME(Args&&... args) : m_variant(std::forward<Args>(args)...){};\
	template<class T, class ...Args>\
	constexpr explicit CLASS_NAME(std::in_place_type_t<T> t, Args&&... args) : m_variant(t, std::forward<Args>(args)...){};\
	template< class T, class U,	class... Args >\
	constexpr explicit CLASS_NAME(std::in_place_type_t<T> t, std::initializer_list<U> il, Args&&... args) : m_variant(t, il, std::forward<Args>(args)...){};\
	template< std::size_t I, class... Args >\
	constexpr explicit CLASS_NAME(std::in_place_index_t<I> i, Args&&... args) : m_variant(i, std::forward<Args>(args)...){};\
	template< std::size_t I, class U, class... Args >\
	constexpr explicit CLASS_NAME(std::in_place_index_t<I> i, std::initializer_list<U> il, Args&&... args) : m_variant(i, il, std::forward<Args>(args)...){};\
	auto && operator*() &&{ return std::move(m_variant); };\
	auto& operator*() &{ return m_variant; };\
	const auto& operator*() const& { return m_variant; };\
	auto && get() &&{ return std::move(m_variant); };\
	auto& get() &{ return m_variant; };\
	const auto& get() const& { return m_variant; };\
	VARVIS_ADD_VISITOR_FUNCTIONS( __VA_ARGS__)\
private:\
	std::variant<VARVIS_FILL_VARIANT_TEMPLATE_UNPACK(VARIANT_TYPES)> m_variant;\
};

#define VARVIS_DEFINE_VARIANT_VISITOR_REF(CLASS_NAME, ...) \
template<typename ...Args>\
class CLASS_NAME {\
public:\
	CLASS_NAME(std::variant<Args...>& variant) : m_variant(variant){};\
	auto && operator*() &&{ return std::move(m_variant); };\
	auto& operator*() &{ return m_variant; };\
	const auto& operator*() const& { return m_variant; };\
	auto && get() &&{ return std::move(m_variant); };\
	auto& get() &{ return m_variant; };\
	const auto& get() const& { return m_variant; };\
	VARVIS_ADD_VISITOR_FUNCTIONS( __VA_ARGS__)\
private:\
	std::variant<Args...>& m_variant;\
};


namespace VariantVisitorInterface {

	template<typename Destination, typename Source>
	std::optional<Destination> interface_cast(Source&& source)
	{
		const auto lambda_getSourceValue = [](auto&& val) -> std::optional<Destination> {
			using T_Val = decltype(val);
			using T_DestinationVariant = std::decay_t<decltype(std::declval<Destination>().get())>;

			if constexpr (std::is_constructible_v<T_DestinationVariant, T_Val>)
				return Destination(std::forward<T_Val>(val));
			else
				return std::nullopt;

		};

		return std::visit(lambda_getSourceValue, *source);
	}

}