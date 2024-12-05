#include <assert.h>
#include <string.h>

#include "../src/dataHandling.h"
#include "../src/general.h"
#include "../src/terminal.h"

void test_add_route();
void test_remove_route();

int main() {
    test_add_route();
    test_remove_route();

    printf("Test done\n");
    return 0;
}

/*
void test() {
    int value = 1;
    int expected = 2;

    int result = f(value);

    assert(result == expected);
}
*/

void test_add_route() {
    route_s *routes = NULL;
    int routesLength = 0;

    append_route(&routes, &routesLength,
        "or", "de", "or n", "de n",
        1, 2, 3, 4, tt_plane);

    assert(routes != NULL);
    assert(routesLength == 1);

    assert(strcmp(routes->origin,"or") == 0);
    assert(strcmp(routes->destination,"de") == 0);
    assert(strcmp(routes->originName,"or n") == 0);
    assert(strcmp(routes->destinationName,"de n") == 0);

    assert(routes->travelTime == 1);
    assert(routes->emission == 2);
    assert(routes->price == 3);
    assert(routes->downtime == 4);
    assert(routes->transportType == tt_plane);
}

void test_remove_route() {
    route_s *routes = memory_allocation(NULL, sizeof(route_s), 0);
    int routesLength = 1;

    routes->origin = memory_allocation(NULL, 2, 1);
    routes->destination = memory_allocation(NULL, 2, 1);
    routes->originName = memory_allocation(NULL, 2, 1);
    routes->destinationName = memory_allocation(NULL, 2, 1);

    remove_route(&routes, &routesLength, 0);

    assert(routes == NULL);
    assert(routesLength == 0);
}