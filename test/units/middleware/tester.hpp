/*
-----
test middleware against input that throws unexpected low level exceptions
test middleware against input that throws application level exceptions
test middleware against valid and invalid request
-----


T1. Construct middleware object and log it
T2. Construct path middleware and test it against a valid as well as invalid request
T3. Construct default middleware and test it against a valid request

*/

#include "../../../include/zen/middleware/defaultMiddleware.hpp"
#include "../../../include/zen/middleware/pathMiddleware.hpp"
#include "../../../include/zen/middleware/errorMiddleware.hpp"

