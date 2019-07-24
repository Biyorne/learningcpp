#ifndef EXAMPLES_NESTED_FOR_HPP_INCLUDED
#define EXAMPLES_NESTED_FOR_HPP_INCLUDED
//
// nested-for.hpp
//
#include <cstdlib>
#include <iomanip>
#include <iostream>

namespace nested_for
{
    // Expected Output:
    //
    //   0   1   2   3   4   5   6   7   8   9
    //  10  11  12  13  14  15  16  17  18  19
    //  20  21  22  23  24  25  26  27  28  29
    //  30  31  32  33  34  35  36  37  38  39
    //  40  41  42  43  44  45  46  47  48  49
    //  50  51  52  53  54  55  56  57  58  59
    //  60  61  62  63  64  65  66  67  68  69
    //  70  71  72  73  74  75  76  77  78  79
    //  80  81  82  83  84  85  86  87  88  89
    //  90  91  92  93  94  95  96  97  98  99
    //
    inline void Ex1_print__Grid_10x10__FullOf_CellIndexes()
    {
        // TODO
    }

    inline void Ex2_print__Grid_XxY__FullOf_Param(const int, const int, const char)
    {
        // TODO
    }

    inline void Ex3_print__Grid_XxY__FullOf_Param__WallsOf_Param()
    {
        // TODO
    }

    inline void Ex4_print__Grid_XxY__FullOf_Param__WallsOf_Param__CrossOf_Param()
    {
        // TODO
    }

    inline void Ex4_print__Grid_XxY__FullOf_Param__WallsOf_Param__XOf_Param()
    {
        // TODO
    }

    //

    inline void runAll()
    {
        //
        // Expected Output:
        //
        //   0   1   2   3   4   5   6   7   8   9
        //  10  11  12  13  14  15  16  17  18  19
        //  20  21  22  23  24  25  26  27  28  29
        //  30  31  32  33  34  35  36  37  38  39
        //  40  41  42  43  44  45  46  47  48  49
        //  50  51  52  53  54  55  56  57  58  59
        //  60  61  62  63  64  65  66  67  68  69
        //  70  71  72  73  74  75  76  77  78  79
        //  80  81  82  83  84  85  86  87  88  89
        //  90  91  92  93  94  95  96  97  98  99
        //
        Ex1_print__Grid_10x10__FullOf_CellIndexes();
        std::cout << std::endl << std::endl;

        //
        // Expected Output:
        //
        //  ######
        //  ######
        //  ######
        //
        Ex2_print__Grid_XxY__FullOf_Param(6, 3, '#');
        std::cout << std::endl << std::endl;

        //
        // Expected Output:
        //
        //  XXX
        //  XXX
        //  XXX
        //  XXX
        //  XXX
        //  XXX
        //
        // Ex2_print__Grid_XxY__FullOf_Param(3, 6, '#');
        std::cout << std::endl << std::endl;

        //
        // Expected Output:
        //
        //  ##########
        //  #        #
        //  #        #
        //  #        #
        //  #        #
        //  #        #
        //  ##########
        //
        // Ex3_print__Grid_XxY__FullOf_Param__WallsOf_Param(11, 7, ' ', '#');
        std::cout << std::endl << std::endl;

        //
        // Expected Output:
        //
        //  #####C#####
        //  #    C    #
        //  #    C    #
        //  CCCCCCCCCCC
        //  #    C    #
        //  #    C    #
        //  #####C#####
        //
        // Ex4_print__Grid_XxY__FullOf_Param__WallsOf_Param__CrossOf_Param(11, 7, ' ', '#', 'C');
        std::cout << std::endl << std::endl;

        //
        // Expected Output:
        //
        //  X#######X
        //  #X     X#
        //  # X   X #
        //  #  X X  #
        //  #   X   #
        //  #  X X  #
        //  # X   X #
        //  #X     X#
        //  X#######X
        //
        // Ex5_print__Grid_XxY__FullOf_Param__WallsOf_Param__XOf_Param(9, 9, ' ', '#', 'X');
        std::cout << std::endl << std::endl;
    }

} // namespace nested_for

#endif // EXAMPLES_NESTED_FOR_HPP_INCLUDED
