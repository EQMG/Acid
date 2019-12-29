#pragma once

#include <iterator>

namespace acid {
template<typename Iterable>
class EnumerateObject {
private:
	Iterable m_iter;
	std::size_t m_size = 0;
	decltype(std::begin(m_iter)) m_begin;
	const decltype(std::end(m_iter)) m_end;

public:
	EnumerateObject(Iterable iter) :
		m_iter(iter),
		m_begin(std::begin(iter)),
		m_end(std::end(iter)) {
	}

	const EnumerateObject &begin() const { return *this; }
	const EnumerateObject &end() const { return *this; }

	bool operator!=(const EnumerateObject &) const {
		return m_begin != m_end;
	}

	void operator++() {
		++m_begin;
		++m_size;
	}

	auto operator*() const -> std::pair<std::size_t, decltype(*m_begin)> {
		return {m_size, *m_begin};
	}
};

template<typename Iterable>
auto Enumerate(Iterable &&iter) -> EnumerateObject<Iterable> {
	return {std::forward<Iterable>(iter)};
}
}
