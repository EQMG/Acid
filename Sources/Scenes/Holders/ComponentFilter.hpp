#pragma once

#include <bitset>
#include "Scenes/Component.hpp"

namespace acid {
class Filter;
class ACID_EXPORT ComponentFilter {
public:
	using Mask = std::bitset<MAX_COMPONENTS>;

	ComponentFilter();

	~ComponentFilter() = default;

	/**
	 * Checks if an Entity matches the requirements.
	 * @param mask The requirements mask.
	 * @return If the Entity matches.
	 */
	bool Check(const Mask &mask) const;

	/**
	 * Sets the Component Filter logic, using {@link Filter#Compute()}.
	 * @param filter The Filter object to check Component Masks on.
	 */
	void Set(const Filter &filter);

private:
	std::unique_ptr<Filter> m_filter;
};

class ACID_EXPORT Filter {
public:
	Filter() = default;
	virtual ~Filter() = default;

	virtual std::unique_ptr<Filter> Clone() const = 0;
	virtual bool Compute(const ComponentFilter::Mask &mask) const = 0;
};

class ACID_EXPORT FilterNone : public Filter {
public:
	FilterNone() = default;

	std::unique_ptr<Filter> Clone() const override;
	bool Compute(const ComponentFilter::Mask &mask) const override;
};

class ACID_EXPORT LogicalAnd : public Filter {
public:
	LogicalAnd(const Filter &left, const Filter &right);

	std::unique_ptr<Filter> Clone() const override;
	bool Compute(const ComponentFilter::Mask &mask) const override;

private:
	std::unique_ptr<Filter> m_left;
	std::unique_ptr<Filter> m_right;
};

class ACID_EXPORT LogicalOr : public Filter {
public:
	LogicalOr(const Filter &left, const Filter &right);

	std::unique_ptr<Filter> Clone() const override;
	bool Compute(const ComponentFilter::Mask &mask) const override;

private:
	std::unique_ptr<Filter> m_left;
	std::unique_ptr<Filter> m_right;
};

class ACID_EXPORT LogicalNot : public Filter {
public:
	explicit LogicalNot(const Filter &right);

	bool Compute(const ComponentFilter::Mask &mask) const override;
	std::unique_ptr<Filter> Clone() const override;
	
private:
	std::unique_ptr<Filter> m_right;
};

template<typename T>
class FilterMatch : public Filter {
public:
	FilterMatch() = default;

	std::unique_ptr<Filter> Clone() const override;
	bool Compute(const ComponentFilter::Mask &mask) const override;
};
}

#include "ComponentFilter.inl"
