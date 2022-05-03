# Type system for physical units

The goal of this program is to allow developers to use helper types that allow the information about values, quantities and units to be stored in variables.    
The user will be able to derive the unit of multiple or quotient of two variables. The assignment, addition, or subtraction of two variables of different units is not allowed.  
It is general set of templates, which the user will be able to make his own set of units, e.g. [SI](https://en.wikipedia.org/wiki/International_System_of_Units). 

Each variable can then be of one of the following types of units:

- A scalar, i.e. the variable explicitly has no unit.
- Base unit, e.g. m, kg, s.
- Derived units, e.g. m/s, N = kg. m. s⁻².

# Usage
        #include "units.h"
        #include <iostream>

        enum class si_units
        {
            second,
            metre,
            kilogram,
            ampere,
            kelvin,
            mole,
            candela,
            _count
        };


        using second = basic_unit<si_units, si_units::second>;
        using metre = basic_unit<si_units, si_units::metre>;
        using kilogram = basic_unit<si_units, si_units::kilogram>;

        using metre_per_second = divided_unit<metre, second>;
        using newton = divided_unit<multiplied_unit<kilogram, metre>, multiplied_unit<second, second>>;


        int main()
        {
            quantity<metre> l(2.1);
            quantity<second> t(0.9);
            auto v1(l / t);

            quantity<metre_per_second> v2{ 2.5 };

            std::cout << (v1 + v2).value() << std::endl;

            // Won't compile
            //std::cout << (l + t).value() << std::endl;
        }
