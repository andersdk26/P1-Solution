add_library(dataHandling dataHandling.c)
add_library(general_ general.c)
add_library(terminal terminal.c)

add_executable(P1_Solution main.c)
target_link_libraries(P1_Solution dataHandling general_ terminal)

#configure_file(airplanes.csv airplanes.csv COPYONLY)