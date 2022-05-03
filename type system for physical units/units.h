#ifndef UNITS_H_
#define UNITS_H_

#include <iostream>
using namespace std;


template <int ... TValues>
struct static_vector;



template <typename TEnum, typename TPowers>
struct unit
{
    typedef TEnum tenum;
    typedef TPowers tpowers;
};



template <int Size, int Index, typename Vector>
struct create_vector
{};

template <int Index, int ... Values>
struct create_vector<0, Index, static_vector<Values ...> >
{
    using type = static_vector<Values ...> ;
};

template <int Size, int ... Values>
struct create_vector<Size, 0, static_vector<Values ...> >
{
    using type = typename create_vector<Size - 1, -1, static_vector<Values ..., 1 > >::type;
};

template <int Index, int Size, int ... Values>
struct create_vector<Size, Index, static_vector<Values ...> >
{
    using type = typename create_vector<Size - 1, Index - 1, static_vector<Values ..., 0 > >::type;
};



template <typename Vector, int Added>
struct push_front
{};

template <int Added, int ... Values>
struct push_front<static_vector<Values ...>, Added>
{
    using type = static_vector<Added, Values ...>;
};



template <typename Vector1, typename Vector2>
struct sum
{};

template <int Head1, int Head2>
struct sum<static_vector<Head1>, static_vector<Head2>>
{
    using type = static_vector<Head1 + Head2>;
};

template <int Head1, int ... Tail1, int Head2, int ... Tail2>
struct sum<static_vector<Head1, Tail1 ...>, static_vector<Head2, Tail2 ...>>
{
    using type = typename push_front<typename sum<static_vector<Tail1...>, static_vector<Tail2...> >::type, Head1 + Head2>::type;
};



template <typename Vector1, typename Vector2>
struct diff
{};

template <int Head1, int Head2>
struct diff<static_vector<Head1>, static_vector<Head2>>
{
    using type = static_vector<Head1 - Head2>;
};

template <int Head1, int ... Tail1, int Head2, int ... Tail2>
struct diff<static_vector<Head1, Tail1 ...>, static_vector<Head2, Tail2 ...>>
{
    using type = typename push_front<typename sum<static_vector<Tail1...>, static_vector<Tail2...> >::type, Head1 - Head2>::type;
};



template <typename TFirstUnit, typename ... TOtherUnits>
struct mult_units
{};

template <typename TUnit>
struct mult_units<TUnit>
{
    using type = TUnit;
};

template <typename TFirstUnit, typename TSecondUnit>
struct mult_units<TFirstUnit, TSecondUnit>
{
    using s = typename sum<typename TFirstUnit::tpowers, typename TSecondUnit::tpowers>::type;
    using type = unit<typename TFirstUnit::tenum, s>;
};

template <typename TFirstUnit, typename TSecondUnit, typename ... TOtherUnits>
struct mult_units<TFirstUnit, TSecondUnit, TOtherUnits ...>
{
    using s = typename mult_units<TFirstUnit, TSecondUnit>::type;
    using type = typename mult_units<s, TOtherUnits ...>::type;
};



template <typename TDividendUnit, typename TDivisorUnit>
struct div_units
{};

template <typename TDivisorUnit, typename TDividendUnit>
struct div_units<TDividendUnit, TDivisorUnit>
{   
    using type = unit<typename TDividendUnit::tenum, typename diff<typename TDividendUnit::tpowers, typename TDivisorUnit::tpowers>::type>;
};



template <typename TEnum, TEnum Index>
using basic_unit = unit <TEnum, typename create_vector<int(TEnum::_count), int(Index), static_vector <>>::type >;


template <typename TFirstUnit, typename ... TOtherUnits>
using multiplied_unit = typename mult_units<TFirstUnit, TOtherUnits ...>::type;


template <typename TDividendUnit, typename TDivisorUnit>
using divided_unit = typename div_units<TDividendUnit, TDivisorUnit>::type;



template <typename TUnit, typename TValue = double>
struct quantity
{};

template <typename TValue, typename TEnum, typename TPowers>
struct quantity<unit<TEnum, TPowers>, TValue>
{
    explicit quantity(TValue val) : val_(val) {};

    TValue value(){
        return val_;
    };

    quantity operator+(quantity &a) {
        return quantity (val_ + a.value());
    };

    quantity operator-(quantity &a) {
        return quantity (val_ - a.value());
    };

    template<typename TPowers2>
    auto operator*(quantity<unit<TEnum, TPowers2>, TValue> &a) {
        return quantity<multiplied_unit<unit<TEnum, TPowers>, unit<TEnum, TPowers2>>> (val_ * a.value());
    };

    template<typename TPowers2>
    auto operator/(quantity<unit<TEnum, TPowers2>, TValue> &a) {
        return quantity<divided_unit<unit<TEnum, TPowers>, unit<TEnum, TPowers2>>> (val_ / a.value()); 
    };

    TValue val_;
};


#endif