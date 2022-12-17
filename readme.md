# Graph Algorithms

This repo contains the following algorithms:

1.  [Ullmann's](https://dl.acm.org/doi/10.1145/321921.321925) algorithm for the [subgraph isomorphism problem](https://en.wikipedia.org/wiki/Subgraph_isomorphism_problem)


# Supported platforms

1. Ubuntu 20.04

# Dependencies

1. g++-8 or newer
2. cmake 3.16 or newer
3. vcpkg (see Build and test)

# Build and test

1. clone repo

```
git clone https://github.com/ashwin-hari/GraphAlgos.git
```

2. install vcpkg - taken from [vcpkg's guide](https://vcpkg.io/en/getting-started.html)

```
$ git clone https://github.com/Microsoft/vcpkg.git
$ ./vcpkg/bootstrap-vcpkg.sh
```

3. install boost-graph

```
$ ./vcpkg/vcpkg install boost-graph
```

3. Build repo 

```
$ cmake -DCMAKE_TOOLCHAIN_FILE:STRING=<absolute path to vcpkg clone>/scripts/buildsystems/vcpkg.cmake \
-DCMAKE_BUILD_TYPE:STRING=Debug \
-HGraphAlgos -BGraphAlgos/build \
-G "Unix Makefiles"
$ cmake --build GraphAlgos/build --config Debug
```

4. Run tests

```
$ GraphAlgos/build/src/tests/GraphAlgosTests
```