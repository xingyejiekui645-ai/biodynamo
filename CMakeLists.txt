cmake_minimum_required(VERSION 3.16)
project(biodynamo VERSION 1.0.0 LANGUAGES CXX C)

# ... existing content up to line 152 ...

option(website   "Enable website generation (make website<-live>)." OFF)
option(valgrind  "Enable valgrind tests and make build compatible with valgrind tool." ON)
option(rpath     "Link libraries with built-in RPATH (run-time search path)." OFF)
option(real_t    "Define data type for real numbers. Currently supported: float, double" double)
option(boost     "Enable Boost libraries (optional)." OFF)

# ... existing content ...

if(boost)
  find_package(Boost REQUIRED COMPONENTS system filesystem)
  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(-DBIODYNAMO_USE_BOOST)
  endif()
endif()

# ... existing content ...

# Example target linking (adjust as needed for actual targets)
# target_link_libraries(biodynamo PUBLIC ${Boost_LIBRARIES})

# ... rest of existing CMakeLists.txt ...