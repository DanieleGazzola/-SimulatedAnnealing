# Notes on the project

# Readme and compiling
Readme is well done. Compiling and executing was easy and there are good examples. Shows nice scalability.

However, there is an error when updating the linker path for `muparser-2.3.4`: the path is installation dependent, that is you are supposing that the user is downloading the repo is the same folder you have. You should either mention this or put a command that works in general, something like `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD`.

Moreover, use `mpic++` not `mpicc` + standard c++ libraries. The latter solution does not always work!

# Code
* Many local variable that could be const are not (see `T, alpha`, ... in `main.cpp`)
* In `Domain`:
    * Probably is makes more sense to put `engine` as mutable and make most of the methods of this class const. This is a case where `mutable` permits to maintain constness of methods that are "morally const".
    * In `generateNewPoint`, instead of the `if` you could map `[0, 1]` in `[-1, 1]`
* In `SimulatedAnnealing`:
    * You often pass `std::string function`, `Domain` and `mu::Parser` by copy instead of reference
    * You pass `int& domainDimension` and `int& dimensions` by reference, you either pass it by copy of const reference
    * You keep shuffling around copies and references of `mu::Parser`, it should be better to add it as a private attribute of the class. Moreover, if you use `parser.DefineVar(&newPoint[i])`, exploit the fact that you pass pointers to the `newPoint` elements and thus you can call `setNewPoint` just once if you do not reallocate the `newPoint`.
    * In many points you use both `reserve` and `resize`, it is redundant
    * The variables `solution = domain.generateInitialSolution(); stepsize = domain.generateStepsize();` have a larger scope than needed. Moreover, the initialization that you do before the assignment is useless.
 

Take the habit of using `const &` when passing possibly large objects. It is more efficient than passing by value.
    `Domain(domainBounds passedBounds, int rank)` could be changed into `Domain(domainBounds const & passedBounds, int rank)`. 
    You avoid making a copy. Remember that when you pass by value the parameters are initialized with the argument, so they are effectively a copy of the arguments. When you pass by reference (and in particular by const reference) you just refer to the object, creating an alias.
    In modern C++ normally you pass by reference (const reference when the parameter is not not meant to be changed), a part possibly 
    `int` or `double` (but you can pass them by const reference as well if you wish).
    - No use of calling `resize` after `reserve`! You either use `resize` and then the address operator `[]`, or (possibly more efficient) `reserve` and `emplace_back`:
    

``` C++
    std::vector<double> Domain::generateInitialSolution(){
    std::vector<double> initialSolution;
    initialSolution.reserve(dimensions); // not needed 
    initialSolution.resize(dimensions);
    
    for(int i = 0; i < dimensions; ++i){
        initialSolution[i] = bounds[i].first + (bounds[i].second - bounds[i].first) * randomUnitary(); 
    }

    return initialSolution;
    }

```

with just `reserve` becomes

```
    std::vector<double> Domain::generateInitialSolution(){
    std::vector<double> initialSolution;
    initialSolution.reserve(dimensions);
    
    for(int i = 0; i < dimensions; ++i){
        initialSolution.emplace_back(bounds[i].first + (bounds[i].second - bounds[i].first) 
        * randomUnitary()); 
    }

    return initialSolution;
}
```
I remind the meaning:

  - `reserve(n)`: make sure that the vector can store `n` elements without need of memory requests. The size of the vector remains however unchanged.
  - `resize(n)`: resize the vector so that the size is `n` (and consequently the capacity is at least `n`). If new elements are needed they will be created using the default constructor.

So if you `resize` you do not need to `reserve` before.

- Very bad idea to have the name of the directory starting with -. Is
  it to make things more complicated? A form of protection?

- Good the idea of using muParser. 

- Using `&tempSol[0]` to get a pointer to the vector buffer is fine, but you have `tempSol.data()` that does exactly that, and is less criptic!


For the rest, good standard C++ programming. Use of MPI 

